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
<img width="613" height="232" alt="スクリーンショット 2026-02-26 123408" src="https://github.com/user-attachments/assets/c81c49ab-bf55-4fc4-8d19-601fa1f3f97f" />

## Hardware

- Voltage Divider (10kΩ / 20kΩ)

### Wiring Diagram
![alt text](IMG_4079.jpeg) ![alt text](IMG_4081.jpeg) ![alt text](IMG_4080.jpeg)
![IMG_4079](https://github.com/user-attachments/assets/2f9b85f3-77cd-4497-8f7c-3e7e8031a01a)
![IMG_4080](https://github.com/user-attachments/assets/f4d19014-3c04-4950-9822-267a42f89488)
![IMG_4081](https://github.com/user-attachments/assets/54088e12-d868-4011-b823-e21e01003e07)

## Software Stack

5. Threshold detection determines bed-exit events.

## Test Results


## Future Improvements

- Designing RESTful communication between embedded device and server
- Database schema design for time-series sensor data
- Basic IoT system architecture
