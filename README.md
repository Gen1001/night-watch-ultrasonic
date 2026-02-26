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
<img width="613" height="232" alt="スクリーンショット 2026-02-26 123408" src="https://github.com/user-attachments/assets/c81c49ab-bf55-4fc4-8d19-601fa1f3f97f" />

## Hardware

- ESP32 DevKit
- HC-SR04 Ultrasonic Sensor
- Voltage Divider (10kΩ / 20kΩ)

### Wiring Diagram
![IMG_4079](https://github.com/user-attachments/assets/2f9b85f3-77cd-4497-8f7c-3e7e8031a01a)
![IMG_4080](https://github.com/user-attachments/assets/f4d19014-3c04-4950-9822-267a42f89488)
![IMG_4081](https://github.com/user-attachments/assets/54088e12-d868-4011-b823-e21e01003e07)

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
<img width="1920" height="1080" alt="スクリーンショット (7)" src="https://github.com/user-attachments/assets/bc4446f5-a283-403e-9020-334415b13af5" />
<img width="1920" height="1080" alt="スクリーンショット (8)" src="https://github.com/user-attachments/assets/d0887e19-86b2-4454-afc3-13f7f2c6f4f7" />
<img width="1920" height="1080" alt="スクリーンショット (9)" src="https://github.com/user-attachments/assets/000a97a9-9cab-42d9-911c-f42c8b506036" />
<img width="1920" height="1080" alt="スクリーンショット (10)" src="https://github.com/user-attachments/assets/6c0ecf6b-43c6-47da-b267-259a3c4af2d8" />
<img width="1920" height="1080" alt="スクリーンショット (11)" src="https://github.com/user-attachments/assets/e89d4f75-37b4-49d8-bb84-121f7b3f0e22" />

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

