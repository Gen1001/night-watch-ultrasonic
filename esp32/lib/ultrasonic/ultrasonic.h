#pragma once
#include <Arduino.h>

// 超音波センサー用クラス
class Ultrasonic {

public:
    Ultrasonic(uint8_t trigPin, uint8_t echoPin);

    // トリガ送信
    void trigger();

private:
    uint8_t _trig;
    uint8_t _echo;
};