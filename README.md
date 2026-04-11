## 概要

このプロジェクトはESP32とセンサー（HC-SR04）を使用した離床を検知するセンサーです。

システムが距離を正確に計測し、そのデータをPythonで作成したHTTPサーバに送信する仕組みになっています。
計測したログはSQLiteデータベースに保存され、異常な動作を検知するために監視できるようにしました。

## 機能
・ESP32（PlatformIO）を用いた組み込み開発　
-FreeRTOSを使用したリアルタイムなタスクの管理  
-キューの制御  
-割り込み処理（ISR）  
-状態遷移 
-ポインタ管理  
・センサー（HC-SR04）の統合  
・WiFi接続、HTTP通信  
・Pythonによるバックエンド実装  
・SQLiteデータベースの取り扱い  

## 状態遷移図
<img width="1447" height="1239" alt="状態遷移図_離床センサー drawio" src="https://github.com/user-attachments/assets/ae9fda51-2710-48d6-8f8f-4394c7b1a8bd" />

## 回路図
<img width="1720" height="797" alt="image" src="https://github.com/user-attachments/assets/aadeda0a-8be3-4f16-aeac-d8a8db5a28c1" />

## 環境 
PC:Windows11  
マイコン:ESP32  
https://www.amazon.co.jp/dp/B0C9TGJRPH?ref=ppx_yo2ov_dt_b_fed_asin_title&th=1  
センサー:HC-SR04  
https://www.amazon.co.jp/dp/B0CPVDX544?ref=ppx_yo2ov_dt_b_fed_asin_title  
エディタ:Visual Studio Code  
開発環境:PlatformIO  

## 実行手順
1. VS CodeにおいてPlatformIOプロジェクトを作成し、main.cpp、sensor_server.pyを編集する。
2. 回路図の通り配線を行い、USBを経由してPCと接続する。  
-全体図
![全体図](https://github.com/user-attachments/assets/265a6bc9-b5a6-43d7-82a7-3b8f189b5d2d)  
-HC-SR04側
![離床センサ_HC](https://github.com/user-attachments/assets/71505a31-4d3d-49c2-a89d-986d0bdd3008)  
-ESP32側
![離床センサ_ESP](https://github.com/user-attachments/assets/84d62cec-66af-477d-a381-0b3d40f817ab)  

4. PowerShellにおいて監視用のsensor_server.pyを実行し、Flaskサーバーを起動する。  
-sensorフォルダにおいてpython3コマンドを使用してsensor_server.pyを実行する。
<img width="1920" height="1080" alt="image" src="https://github.com/user-attachments/assets/f08eacc4-e69d-4f6c-941a-639576ebd27f" />
5. pioプロジェクトにおいてビルドを行う。  
-「pio run -t upload」コマンドを使用してプロジェクトをESP32にアップロードする
<img width="1920" height="1080" alt="image" src="https://github.com/user-attachments/assets/e460d31f-9f88-4aab-b3af-6cb3392452d5" />
-「pio device monitor」コマンドを使用して正常に動作していることを確認する
<img width="1920" height="1080" alt="image" src="https://github.com/user-attachments/assets/d0159d23-050d-444e-a6a8-05972ed1db5d" />
7. 監視用ログとDBを確認する。  
-監視用ログファイルを確認する
<img width="1920" height="1080" alt="image" src="https://github.com/user-attachments/assets/c340398c-dcbe-48b6-9f3c-e314f4a63cc2" />
-SQLite3を確認する
<img width="1920" height="1080" alt="image" src="https://github.com/user-attachments/assets/5dc6198d-8a19-4b93-9433-9ffeb6e40914" />

## 今後の展望
・省電力モードの実装を行い、よりメモリを意識した設計を行う
・IFTTTを使用した通信を行う
