// ヘッダ多重読み込み防止
#pragma once

#include <Arduino.h>

// 超音波センサー用クラス
class Ultrasonic {

public:
	// 使用ピンを受け取るコンストラクタ
	Ultrasonic(uint8_t trigPin, uint8_t echoPind);

	// 距離取得関数
	float getDistanceCm();

private:
	// ピン番号変数
	uint8_t _trig;
	uint8_t _echo;
};
