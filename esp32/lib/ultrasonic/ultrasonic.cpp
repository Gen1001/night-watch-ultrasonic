#include "ultrasonic.h"

// 使用ピンを変更する関数
Ultrasonic::Ultrasonic(uint8_t trigPin, uint8_t echoPin)
    : _trig(trigPin), _echo(echoPin) {

    // 出力モード
    pinMode(_trig, OUTPUT);

    // 入力モード
    pinMode(_echo, INPUT);
}

// トリガーパルス送信専用関数
void Ultrasonic::trigger() {

    digitalWrite(_trig, LOW);
    delayMicroseconds(2);

    digitalWrite(_trig, HIGH);
    delayMicroseconds(10);
    digitalWrite(_trig, LOW);
}