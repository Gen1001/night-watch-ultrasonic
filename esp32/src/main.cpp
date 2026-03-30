#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include "ultrasonic.h"
#include "esp_sleep.h"

// ピン設定
#define TRIG_PIN 18
#define ECHO_PIN 19

// 閾値設定
#define ALERT_DISTANCE 50.0

// WiFi設定
const char* ssid = "";
const char* password = "";
const char* serverUrl = "http://192.168.11.7:5000/sensor";

// FreeRTOS用オブジェクト
QueueHandle_t distanceQueue;
QueueHandle_t stateQueue;

// 状態定義（状態遷移）
enum SystemState {
    NORMAL,
    WARNING,
    ABNORMAL
};

SystemState currentState = NORMAL;

// 最新距離（送信用）
float latestDistance = 0;

// センサーのクラス生成
Ultrasonic sensor(TRIG_PIN, ECHO_PIN);

// ECHO割り込み用変数
volatile unsigned long echoStart = 0;
volatile unsigned long echoEnd = 0;

/* --- 割り込み処理（ISR）--- */
void IRAM_ATTR echoISR() {

    // 立ち上がりで計測開始
    if (digitalRead(ECHO_PIN) == HIGH) {
        echoStart = micros();
    // 立ち下がりで計測終了
    } else {
        echoEnd = micros();
        unsigned long duration = echoEnd - echoStart;

        // タスクフラグの初期化
        BaseType_t xHigherPriorityTaskWoken = pdFALSE;

        // ISRからキューの送信
        xQueueSendFromISR(distanceQueue, &duration, &xHigherPriorityTaskWoken);

        // ISR終了後すぐタスク切り替え
        if (xHigherPriorityTaskWoken) {
            portYIELD_FROM_ISR();
        }
    }
}

/* --- SensorTask（計測タスク） --- */
void SensorTask(void *pvParameters) {

    // 変数（時間、距離）
    unsigned long duration;
    float distance;

    // 計測タスク
    while (1) {

        // トリガーパルス送信
        sensor.trigger();

        // Queueからduration受信（ISRから）
        if (xQueueReceive(distanceQueue, &duration, pdMS_TO_TICKS(100))) {

            // 距離計算
            distance = duration * 0.034 / 2.0;

            // 最新距離保存
            latestDistance = distance;

            // シリアル表示
            Serial.print("Distance: ");
            Serial.println(distance);

            // キューの送信
            xQueueSend(stateQueue, &distance, portMAX_DELAY);
        }

        // 低消費電力モード（1秒）
        /* esp_sleep_enable_timer_wakeup(1000000);
        esp_light_sleep_start(); */
    }
}

// JudgeTask（状態遷移管理）
void JudgeTask(void *pvParameters) {

    // 変数（距離、回数）
    float distance;
    int alertCount = 0;

    // 状態管理タスク
    while (1) {

        // キューから距離データ受信（SensorTask）
        if (xQueueReceive(stateQueue, &distance, portMAX_DELAY)) {

            // 距離データが0より大きくしきい値以下の場合alertCountを増やす（ノイズ除去）
            if (distance > 0 && distance < ALERT_DISTANCE) {
                alertCount++;
            // それ以外の場合alertCountをリセット
            } else {
                alertCount = 0;
            }

            // alertCountの値によって状態遷移
            if (alertCount >= 6) {
                currentState = ABNORMAL;
            } else if (alertCount >= 3) {
                currentState = WARNING;
            } else {
                currentState = NORMAL;
            }

            // シリアル表示
            Serial.print("State: ");
            Serial.println(currentState);
        }
    }
}

/* --- SendTask（WiFi送信） --- */
void SendTask(void *pvParameters) {

    // 送信タスク
    while (1) {

        // WiFiに接続時
        if (WiFi.status() == WL_CONNECTED) {

            // HTTPクライアント生成
            HTTPClient http;

            // 接続先設定（Flaskサーバ）
            http.begin(serverUrl);

            // HTTPヘッダ設定
            http.addHeader("Content-Type", "application/json");

            // alert判定
            bool alert = (currentState != NORMAL);

            // JSONの作成
            String json =
            "{\"distance\":" + String(latestDistance) +
                ",\"alert\":" + String(alert) +
                ",\"state\":" + String(currentState) +
            "}";
            
            // 作成したJSONをHTTP POST送信
            http.POST(json);

            // デバック用
            int httpCode = http.POST(json);

            Serial.print("HTTP Code: ");
            Serial.println(httpCode);

            String payload = http.getString();
            Serial.println(payload);

            // 接続終了
            http.end();
        }

        vTaskDelay(pdMS_TO_TICKS(2000));
    }
}

/* --- 初期化 --- */
void setup() {

    // シリアル通信開始
    Serial.begin(115200);

    // WiFi接続
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
    }

    // デバック用
    Serial.print("ESP32 IP: ");
    Serial.println(WiFi.localIP());

    // Queue作成
    distanceQueue = xQueueCreate(5, sizeof(unsigned long));
    stateQueue = xQueueCreate(5, sizeof(float));

    // 割り込み設定
    attachInterrupt(digitalPinToInterrupt(ECHO_PIN), echoISR, CHANGE);

    // タスク生成
    xTaskCreate(SensorTask, "SensorTask", 4096, NULL, 2, NULL);
    xTaskCreate(JudgeTask, "JudgeTask", 4096, NULL, 1, NULL);
    xTaskCreate(SendTask, "SendTask", 4096, NULL, 1, NULL);
}

/* --- 繰り返し処理（RTOSがあるため中身は空）*/
void loop() {}
