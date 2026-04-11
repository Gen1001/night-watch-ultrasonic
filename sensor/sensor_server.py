from flask import Flask, request, jsonify
import sqlite3
from datetime import datetime
import logging

# Flaskクラスの作成
app = Flask(__name__)

# INFOより大きいログ（時間とメッセージ）をsensor.logに保存する設定
logging.basicConfig(
    filename="sensor.log",
    level=logging.INFO,
    format="%(asctime)s %(message)s"
)

# SQLite設定
def init_db():
    # sensor.dbに接続し、テーブルがなければ作成する
    conn = sqlite3.connect("sensor.db")

    # カーソルオブジェクトを作成する
    c = conn.cursor()

    # sensor_dataテーブルを作成し、id（自動増分）、timestamp（テキスト）、distance（小数点）、state（整数）を定義する
    c.execute("""
        CREATE TABLE IF NOT EXISTS sensor_data (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            timestamp TEXT,
            distance REAL,
            state INTEGER
        )
    """)

    # コミットの実行
    conn.commit()

    # 接続を閉じる
    conn.close()

# テーブルの初期化
init_db()

# Flaskサーバが/sensorフォルダに対してPOSTメソッドでルーティングするように設定
@app.route("/sensor", methods=["POST"])

# POSTメソッドを受信したときに自動でセンサーデータ受信処理を実行する
def receive_sensor():

    # FlaskでPOSTリクエストのボディに含まれるJSONデータを取得する
    data = request.get_json()

    # JSONからdistanceの値を取得
    distance = data.get("distance")

    # JSONからstateの値を取得
    state = data.get("state")

    # 現在日時を取得
    timestamp = datetime.now().strftime("%Y-%m-%d %H:%M:%S")

    # DB更新処理
    # sensor.dbに接続
    conn = sqlite3.connect("sensor.db")

    # カーソルオブジェクトを作成する
    c = conn.cursor()

    # sensor_dataテーブルにtimestamp、distance、stateを挿入する
    c.execute("INSERT INTO sensor_data (timestamp, distance, state) VALUES (?, ?, ?)",
                (timestamp, distance, state))

    # コミットの実行
    conn.commit()

    # 接続を閉じる
    conn.close()

    # 距離、状態のログ出力
    logging.info(f"Distance={distance} State={state}")

    # レスポンス返却
    return jsonify({"status": "ok"})

# スクリプトとして実行されたときだけサーバを起動
if __name__ == "__main__":
          app.run(host="0.0.0.0", port=5000)