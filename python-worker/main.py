import time
import psycopg2

# データベースに接続するための秘密のパスワード
DB_CONFIG = "dbname=jinro_db user=jinro_user password=jinro_password host=localhost port=5432"

# ==========================================================
# 昼の投票が終わったときの処刑処理
# ==========================================================
def process_voting(room_id):
    # データベースに接続する
    conn = psycopg2.connect(DB_CONFIG)
    cur = conn.cursor()
    
    print(f"【Python】部屋 {room_id} で投票が終了しました。処刑を開始します。")

    # [SQL] 生きている村人を1人だけ選んで、死亡(isAlive = false)にする命令
    sql_kill = '''
        UPDATE "Player" 
        SET "isAlive" = false 
        WHERE id IN (
            SELECT id FROM "Player" 
            WHERE role = 'VILLAGER' AND "isAlive" = true AND "roomId" = %s 
            LIMIT 1
        );
    '''
    cur.execute(sql_kill, (room_id,))
    
    # [SQL] 処刑が終わったので、部屋のフェーズを「4 (夜)」に進める命令
    sql_next_phase = 'UPDATE "Room" SET phase = 4 WHERE id = %s;'
    cur.execute(sql_next_phase, (room_id,))
    
    # 変更をデータベースに確定させて、接続を閉じる
    conn.commit()
    cur.close()
    conn.close()
    print("【Python】処刑が完了しました。夜(フェーズ4)になります。")

# ==========================================================
# データベースに変なことが起きていないか「見守る」関数
# ==========================================================
def check_db():
    conn = psycopg2.connect(DB_CONFIG)
    cur = conn.cursor()
    
    # [SQL] フェーズが 3（投票終了）になっている部屋があるか探す命令
    cur.execute('SELECT id FROM "Room" WHERE phase = 3;')
    rooms = cur.fetchall()
    
    # もしフェーズ3の部屋を見つけたら、処刑の関数を実行する
    for room in rooms:
        room_id = room[0]
        process_voting(room_id)
            
    cur.close()
    conn.close()

# ==========================================================
# 💡 Python班の1年生への宿題・改造指針
# ==========================================================
# 【考えてみよう！】
# 今は「生きている村人を誰でもいいから1人殺す」という超雑なルールになっています。
# 本当の人狼ゲームにするために、以下のような機能を作ってみんなで書き換えてみよう！
#
# 1. 投票データを集計して、一番票が多かったプレイヤーのIDを見つけるロジック
# 2. 夜(フェーズ4)になったら、人狼が村人を噛み殺す「process_night」という新しい関数
# ==========================================================

if __name__ == "__main__":
    print("Pythonプログラムが起動しました。2秒に1回データベースを見張っています...")
    while True:
        try:
            check_db()  # データベースをチェックする
        except Exception as e:
            print(f"エラーが起きたよ: {e}")
        time.sleep(2)   # 2秒休憩する