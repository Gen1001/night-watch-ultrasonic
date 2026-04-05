# 離床検知センサー

## 概要

このプロジェクトはESP32とセンサー（HC-SR04）を使用した離床を検知するセンサーです。

システムが距離を正確に計測し、そのデータをPythonで作成したHTTPサーバに送信する仕組みになっています。
計測したログはSQLiteデータベースに保存され、異常な動作を検知するために監視できるようにしました。

以下の機能を実現しました：
ESP32（PlatformIO）を用いた組み込み開発
-FreeRTOSを使用したリアルタイムなタスクの管理
-キューの制御
-割り込み処理（ISR）
-状態遷移
-ポインタ管理
センサー（HC-SR04）の統合
WiFi接続、HTTP通信
Pythonによるバックエンド実装
SQLiteデータベースの取り扱い

## 状態遷移図
<img width="613" height="232" alt="スクリーンショット 2026-02-26 123408" src="https://github.com/user-attachments/assets/c81c49ab-bf55-4fc4-8d19-601fa1f3f97f" />

## ハードウェア
・ESP32  
https://www.amazon.co.jp/dp/B0C9TGJRPH?ref=ppx_yo2ov_dt_b_fed_asin_title&th=1  
・HC-SR04  
https://www.amazon.co.jp/dp/B0CPVDX544?ref=ppx_yo2ov_dt_b_fed_asin_title

### 回路図
<img width="1720" height="797" alt="image" src="https://github.com/user-attachments/assets/aadeda0a-8be3-4f16-aeac-d8a8db5a28c1" />

## Software Stack
1. VS

## 今後の展望

・省電力モードの実装を行い、よりメモリを意識した設計を行う

