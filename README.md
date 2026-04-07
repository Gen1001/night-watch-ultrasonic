## 概要

このプロジェクトはESP32とセンサー（HC-SR04）を使用した離床を検知するセンサーです。

システムが距離を正確に計測し、そのデータをPythonで作成したHTTPサーバに送信する仕組みになっています。
計測したログはSQLiteデータベースに保存され、異常な動作を検知するために監視できるようにしました。

過去にIFTTTを使用してスマホと連携するセンサーを作成したため、以下を参考にして作成しました。
https://qiita.com/Gen_00_/items/9c065e083f0b96886417

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
3. PowerShellにおいて監視用のsensor_server.pyを実行し、Flaskサーバーを起動する。
4. pioプロジェクトにおいてファイルのアップロード、ビルドを行う。  
アップロード：pio run -t upload
ビルド：pio run
5. 監視用ログとDBを確認する。

## 今後の展望
・省電力モードの実装を行い、よりメモリを意識した設計を行う
