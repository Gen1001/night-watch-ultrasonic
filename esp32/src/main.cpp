#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <iostream>
#include <cstdlib>
#include "ultrasonic.h"

// ピンの設定
#define TRIG_PIN 18
#define ECHO_PIN 19

// 異常値の設定
#define ALERT_DISTANCE 50.0

// Wi-Fi設定
const char* ssid = "Buffalo-G-1380";
const char* password = "k6ubhsfuunahr";

// サーバのURL
const char* serverUrl = "http://192.168.11.9:5000/sensor";

Ultrasonic sensor(TRIG_PIN, ECHO_PIN);

unsigned long lastSend = 0;
int alertCount = 0;

// シリアル通信の開始
void setup() {
	Serial.begin(115200);

	// Wi-Fi接続
	WiFi.begin(ssid, password);
	Serial.print("Connectiong to Wi-Fi");
	while (WiFi.status() != WL_CONNECTED) {
		delay(500);
		Serial.print(".");
	}
	Serial.println("\nConnected to Wi-Fi");
}

void loop() {

	// 前回実行から1秒経ったら実行する
	if (millis() - lastSend >= 1000) {

		// 現在時刻を保存
		lastSend = millis();

		// 距離を取得
		float dist = sensor.getDistanceCm();

		// 距離がしきい値の範囲内の場合エラーを追加
		if (dist > 0 && dist < ALERT_DISTANCE) {
			alertCount++;

		// それ以外の場合エラーを削除
		} else {
			alertCount = 0;
		}

		// 3回連続で近かったら判定を行う
		bool alert = (alertCount >= 3);

		// JSON生成
		String json = "{\"distance\":" + String(dist, 1) + ",\"alert\":" + (alert ? "true" : "false") + "}";
		Serial.println("Sending: " + json);
		
		// Wi-Fi接続確認
		if (WiFi.status() == WL_CONNECTED) {
			HTTPClient http;
			http.begin(serverUrl);
			http.addHeader("Content-Type", "application/json");
			int httpResponseCode = http.POST(json);

			if (httpResponseCode > 0) {
				Serial.println("Response: " + http.getString());
			} else {
				Serial.println("Error on sending POST: " + String(httpResponseCode));
			}
			http.end();
		} else {
			Serial.println("Wi-Fi disconnected");
		}
	}
}