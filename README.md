# IoT Bed-Exit Monitoring System

## Overview

This project is an IoT-based bed-exit monitoring system built with ESP32 and an ultrasonic sensor (HC-SR04).

The system measures distance in real time and sends data to a Python HTTP server. 
Measurement logs are stored in a SQLite database and can be monitored for anomaly detection.

This project demonstrates:
- Embedded development with ESP32 (PlatformIO)
- Sensor integration (HC-SR04)
- HTTP communication
- Python backend implementation
- SQLite database handling

## System Architecture
![alt text](image.png)

## Hardware

- ESP32 DevKit
- HC-SR04 Ultrasonic Sensor
- Voltage Divider (10kΩ / 20kΩ)

### Wiring Diagram
![alt text](IMG_4079.jpeg) ![alt text](IMG_4081.jpeg) ![alt text](IMG_4080.jpeg)

## Software Stack

| Component | Technology |
|-----------|------------|
| Firmware  | ESP32 (PlatformIO) |
| Backend   | Python |
| Database  | SQLite |
| Protocol  | HTTP |

## How It Works

1. ESP32 triggers the ultrasonic sensor.
2. Distance is calculated based on echo pulse duration.
3. Data is sent via HTTP POST request.
4. Python server stores data in SQLite.
5. Threshold detection determines bed-exit events.

## Test Results
![alt text](image-1.png)
![alt text](image-2.png)
![alt text](image-3.png)
![alt text](image-4.png)
![alt text](image-5.png)

## Future Improvements

- Implement MQTT communication instead of HTTP
- Add real-time dashboard (Web UI)
- Integrate Slack or LINE notification
- Deploy backend to cloud environment (AWS / GCP)
- Improve anomaly detection logic

## What I Learned

- Handling voltage level differences between 5V sensor and 3.3V microcontroller
- Designing RESTful communication between embedded device and server
- Database schema design for time-series sensor data
- Basic IoT system architecture