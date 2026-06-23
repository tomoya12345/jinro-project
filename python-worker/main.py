import time
import psycopg2

DB_CONFIG = "dbname=jinro_db user=jinro_user password=jinro_password host=localhost port=5432"

def process_voting(room_id):
    conn = psycopg2.connect(DB_CONFIG)
    cur = conn.cursor()
    
    print(f"部屋 {room_id}: 昼の処刑を実行します！")

    # 【修正版】PostgreSQLで正しく「1人だけ処刑する」SQL
    cur.execute('''
        UPDATE "Player" 
        SET "isAlive" = false 
        WHERE id IN (
            SELECT id FROM "Player" 
            WHERE role = 'VILLAGER' AND "isAlive" = true AND "roomId" = %s 
            LIMIT 1
        );
    ''', (room_id,))
    
    # 部屋のフェーズを「4 (夜の行動)」に進める
    cur.execute('UPDATE "Room" SET phase = 4 WHERE id = %s;', (room_id,))
    
    conn.commit()
    cur.close()
    conn.close()
    print("処刑完了。フェーズを4に進めました。")

def check_db():
    conn = psycopg2.connect(DB_CONFIG)
    cur = conn.cursor()
    cur.execute('SELECT id, phase FROM "Room" WHERE phase = 3;')
    rooms = cur.fetchall()
    
    for room_id, phase in rooms:
        process_voting(room_id)
            
    cur.close()
    conn.close()

if __name__ == "__main__":
    print("Python ロジックワーカー起動。DBを監視中...")
    while True:
        try:
            check_db()
        except Exception as e:
            # エラーをもみ消さず、ちゃんとターミナルに表示する！
            print(f"エラー発生: {e}")
        time.sleep(2)