import time
import psycopg2

DB_CONFIG = "dbname=jinro_db user=jinro_user password=jinro_password host=localhost port=5432"

def process_voting(room_id):
    conn = psycopg2.connect(DB_CONFIG)
    cur = conn.cursor()
    
    print(f"部屋 {room_id} の投票集計を開始します...")

    # ==========================================
    # 💡 Python班 1年生への開発タスク (昼の処理)
    # ==========================================
    
    # 【ステップ1】 投票データをDBから取得する（※ダミーデータでテストしてね）
    # cur.execute("SELECT target_player_id FROM Vote WHERE room_id = %s;", (room_id,))
    # votes = cur.fetchall()
    
    # 【ステップ2】 ここに、誰が一番多く票を集めたか（最多票）を計算するPythonのコードを書く！
    # 例: Pythonの辞書(dict)や collections.Counter を使って計算してみて。
    most_voted_player_id = "dummy_id" # ← 計算結果をここに入れる
    
    # 【ステップ3】 最多票のプレイヤーを死亡扱いにする（ここはリーダーが書いたよ）
    # cur.execute("UPDATE \"Player\" SET \"isAlive\" = false WHERE id = %s;", (most_voted_player_id,))
    
    # 【ステップ4】 部屋のフェーズを「4 (夜の行動)」に進める
    # cur.execute("UPDATE \"Room\" SET phase = 4 WHERE id = %s;", (room_id,))
    
    conn.commit()
    cur.close()
    conn.close()

def check_db():
    conn = psycopg2.connect(DB_CONFIG)
    cur = conn.cursor()
    cur.execute('SELECT id, phase FROM "Room" WHERE phase IN (3, 4);')
    rooms = cur.fetchall()
    
    for room_id, phase in rooms:
        if phase == 3:
            process_voting(room_id)
        elif phase == 4:
            print("夜の行動処理タスクをここに書く") # 夜のタスクも同様に作ろう
            
    cur.close()
    conn.close()

if __name__ == "__main__":
    print("Python ロジックワーカー起動...")
    while True:
        try:
            check_db()
        except Exception as e:
            pass 
        time.sleep(2)