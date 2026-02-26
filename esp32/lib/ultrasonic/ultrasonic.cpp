#include "ultrasonic.h"

// 使用ピンを変更する関数
Ultrasonic::Ultrasonic(uint8_t trigPin, uint8_t echoPin): _trig(trigPin), _echo(echoPin) {

	// 出力モード
	pinMode(_trig, OUTPUT);
	
	// 入力モード
	pinMode(_echo, INPUT);
}

// 距離を取得する関数
float Ultrasonic::getDistanceCm() {

	// 2μsパルス送信
	digitalWrite(_trig, LOW);
	delayMicroseconds(2);

	// 10μsパルス送信
	digitalWrite(_trig, HIGH);
	delayMicroseconds(10);
	digitalWrite(_trig, LOW);

	// HIGHの時間を測定する
	long duration = pulseIn(_echo, HIGH, 30000);

	// 距離が0の場合-1を返す
	if (duration == 0) return -1;

	// 距離(cm)を返す
	return duration * 0.034 / 2.0;
}
