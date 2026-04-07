#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include "esp_sleep.h"

// ピン設定
// ブートストラップピンではなくOutaputに対応しているためTRIG_PINを13に設定
#define TRIG_PIN 13

// ブートストラップピンではなくInputに対応しているためECHO_PINを14に設定
#define ECHO_PIN 14

// 状態を切り替えるため最低距離のしきい値を50に設定
#define ALERT_DISTANCE 50.0

// WiFi設定
const char* ssid = "";
const char* password = "";
const char* serverUrl = "http://192.168.11.7:5000/sensor";

// キューハンドラ設定
QueueHandle_t distanceQueue;
QueueHandle_t stateQueue;

// 状態定義（状態遷移）
enum SystemState {
    NORMAL,
    WARNING,
    ABNORMAL
};

// 現在の状態をNORMALで初期化
SystemState currentState = NORMAL;

// 最新距離を0で初期化
float latestDistance = 0;

// ECHO割り込み用変数
volatile unsigned long echoStart = 0;
volatile unsigned long echoEnd = 0;

/* --- 割り込み処理（ISR）--- */
void IRAM_ATTR echoISR() {

    // ECHO_PINが立ち上がっている場合の処理
    if (digitalRead(ECHO_PIN) == HIGH) {

        // 超音波の発生が完了した時間をmicrosで記録
        echoStart = micros();

    // ECHO_PINが立ち下がっている場合の処理
    } else {
        
        // 反射した超音波の計測が完了した時間をmicrosで記録
        echoEnd = micros();

        // 計測開始時間 - 計測終了時間で計測時間を求める
        unsigned long duration = echoEnd - echoStart;

        // ISRキュー送信のためにタスクフラグを初期化
        BaseType_t xHigherPriorityTaskWoken = pdFALSE;

        // ISRからキューの送信
        // 距離のポインタをSensorタスクが受信するように送信
        xQueueSendFromISR(distanceQueue, &duration, &xHigherPriorityTaskWoken);
    }
}

/* --- SensorTask（計測タスク） --- */
void SensorTask(void *pvParameters) {

    // Echoピンが立ち上がっている時間
    unsigned long duration;

    // センサーが取得した時間を変換した距離
    float distance;

    // 割り込み処理を使用して計測を行うタスク
    while (1) {

        // トリガーパルス送信
        // TRIG_PINを10usパルス立ち上げる
        digitalWrite(TRIG_PIN, LOW);
        delayMicroseconds(2); // 経過時間クリア
        digitalWrite(TRIG_PIN, HIGH);
        delayMicroseconds(10);
        digitalWrite(TRIG_PIN, LOW);

        // HC_SR04が超音波を発生し、ECHO_PINが立ち上がる
        // 47行目の割り込み処理（ISR）が起動
        // 超音波の反射を計測し終えてECHO_PINが立ち下がる
        // 50行目の割り込み処理が起動

        // ISRからEchoピンが立ち上がっている時間をキューで受信した場合の処理
        if (xQueueReceive(distanceQueue, &duration, pdMS_TO_TICKS(100))) {

            // Echoピンが立ち上がっている時間を距離に変換
            // ECHO_PINがHIGHの時間 x 音速（340 m / s）/ 2（反射）
            distance = duration / 58.00;

            // 最新距離を現在のdistanceの値で更新
            latestDistance = distance;

            // 計算した距離をシリアル表示
            Serial.print("Distance: ");
            Serial.println(distance);

            // JudgeTaskを呼び出すキューの送信
            xQueueSend(stateQueue, &distance, portMAX_DELAY);
        }
    }
}

// JudgeTask（状態遷移管理）
void JudgeTask(void *pvParameters) {

    // Sensorタスクが取得した距離
    float distance;

    // 状態を切り替えるカウンタ
    int alertCount = 0;

    // 計測した距離を使用して状態を切り替えるタスク
    while (1) {

        // Sensorタスクからキューの受信をした場合の処理
        if (xQueueReceive(stateQueue, &distance, portMAX_DELAY)) {

            // ノイズ除去
            // 距離データが0より大きくしきい値以下の場合alertCountを増やす
            if (distance > 0 && distance < ALERT_DISTANCE) {
                alertCount++;
            // それ以外の場合alertCountをリセット
            } else {
                alertCount = 0;
            }

            // alertCountが6以上の場合はABNORMAL
            if (alertCount >= 6) {
                currentState = ABNORMAL;

            // alertCountが3以上の場合はWARNING
            } else if (alertCount >= 3) {
                currentState = WARNING;

            // alertCountがそれ以外の場合はNORMAL
            } else {
                currentState = NORMAL;
            }

            // 現在の状態をシリアル表示
            Serial.print("State: ");
            Serial.println(currentState);
        }
    }
}

/* --- SendTask（WiFi送信） --- */
void SendTask(void *pvParameters) {

    // SensorタスクとJudgeタスクで取得した距離と状態をFlaskサーバに送信する
    while (1) {

        // WiFiに接続している場合の処理
        if (WiFi.status() == WL_CONNECTED) {

            // HTTPクライアント生成
            HTTPClient http;

            // HTTP接続先設定
            // Flaskサーバと通信するため
            http.begin(serverUrl);

            // HTTPヘッダ設定
            // HTTPボディをJSONに設定する
            http.addHeader("Content-Type", "application/json");

            // JSONの作成
            // 最新の距離と現在の状態のJSON
            String json =
            "{\"distance\":" + String(latestDistance) +
                ",\"state\":" + String(currentState) +
            "}";
            
            // 作成したJSONをHTTP POST送信
            http.POST(json);

            // HTTP接続終了
            http.end();
        }

        // タスクを2000msec中断する
        vTaskDelay(pdMS_TO_TICKS(2000));
    }
}

/* --- 初期化 --- */
void setup() {

    // シリアル通信開始
    Serial.begin(115200);

    // シリアルが接続されるまで待機する
    while (!Serial) {
        delay(500);
    }

    // WiFi接続
    WiFi.begin(ssid, password);

    // WiFI接続されるまで待機する
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
    }


    // Queue作成
    // サイズ5である距離のキュー
    distanceQueue = xQueueCreate(5, sizeof(unsigned long));

    // サイズ5である状態のキュー
    stateQueue = xQueueCreate(5, sizeof(float));

    // 割り込み設定
    // ECHO_PINが変更された時にechoISRを呼び出す（SensorタスクがTRIG_PINを変更した際に割り込み処理が発生するようにするため）
    attachInterrupt(digitalPinToInterrupt(ECHO_PIN), echoISR, CHANGE);

    // タスク生成
    // 計測タスクを中断しないために優先度を2に設定する
    xTaskCreate(SensorTask, "SensorTask", 4096, NULL, 2, NULL);

    // JudgeタスクとSendタスクは計測において優先度が低いため1に設定する
    xTaskCreate(JudgeTask, "JudgeTask", 4096, NULL, 1, NULL);
    xTaskCreate(SendTask, "SendTask", 4096, NULL, 1, NULL);
}

/* --- 繰り返し処理（RTOSがあるため中身は空）*/
void loop() {}
