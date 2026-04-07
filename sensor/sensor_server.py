from flask import Flask, request, jsonify
import sqlite3
from datetime import datetime
import logging

# アプリの作成
app = Flask(__name__)

# ログの設定
logging.basicConfig(
    filename="sensor.log",
    level=logging.INFO,
    format="%(asctime)s %(message)s"
)

# DBの初期化
def init_db():
    conn = sqlite3.connect("sensor.db")
    c = conn.cursor()
    c.execute("""
        CREATE TABLE IF NOT EXISTS sensor_data (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            timestamp TEXT,
            distance REAL,
            state INTEGER
        )
    """)
    conn.commit()
    conn.close()

init_db()

# ルーティング設定
@app.route("/sensor", methods=["POST"])

# センサーデータ受信処理
def receive_sensor():
    data = request.get_json()
    distance = data.get("distance")
    state = data.get("state")
    timestamp = datetime.now().strftime("%Y-%m-%d %H:%M:%S")

    # DB保存
    conn = sqlite3.connect("sensor.db")
    c = conn.cursor()
    c.execute("INSERT INTO sensor_data (timestamp, distance, state) VALUES (?, ?, ?)",
                (timestamp, distance, state))
    conn.commit()
    conn.close()

    # ログ書き込み
    logging.info(f"Distance={distance} State={state}")

    # レスポンス返却
    return jsonify({"status": "ok"})

# サーバー起動
if __name__ == "__main__":
          app.run(host="0.0.0.0", port=5000)