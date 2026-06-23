#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <libpq-fe.h> // PostgreSQLと会話するためのライブラリ

// ==========================================================
// 💡 C言語班の1年生への宿題・改造指針（ここを編集してもらう！）
// ==========================================================
// 【やってみよう！】
// 村人と人狼の数を受け取って、勝敗を判定する関数です。
// 戻り値(返す数字)を以下のように決めて、中身を書き換えてみてください！
// 0 = まだゲーム継続 / 1 = 村人の勝ち / 2 = 人狼の勝ち
// ==========================================================
int check_victory_condition(int villager_count, int werewolf_count) {
    
    // 【ここにif文を書いてみよう！】
    // 例: もし werewolf_count が villager_count 以上になったら 2 を返す、など。
    if (werewolf_count >= villager_count && werewolf_count > 0) {
        return 2; // 人狼の勝ち
    }
    
    return 0; // まだ誰も勝っていない
}

// ==========================================================
// メインの処理（ここはデータベースの面倒な処理なのでリーダーが守る！）
// ==========================================================
int main() {
    printf("C言語 GMサーバーが起動しました。勝敗を見張っています...\n");
    const char *conninfo = "dbname=jinro_db user=jinro_user password=jinro_password host=localhost port=5432";
    
    while(1) {
        PGconn *conn = PQconnectdb(conninfo); // データベースに接続
        if (PQstatus(conn) == CONNECTION_OK) {
            
            // 1. 夜(フェーズ4)の部屋があるかチェック
            PGresult *res_room = PQexec(conn, "SELECT id FROM \"Room\" WHERE phase = 4 LIMIT 1;");
            if (PQntuples(res_room) > 0) {
                
                // 2. 生きている人狼の数を数える(SQLの結果を数字に変換)
                PGresult *res_w = PQexec(conn, "SELECT COUNT(*) FROM \"Player\" WHERE role='WEREWOLF' AND \"isAlive\"=true;");
                int werewolf_alive = atoi(PQgetvalue(res_w, 0, 0));
                
                // 3. 生きている村人の数を数える(SQLの結果を数字に変換)
                PGresult *res_v = PQexec(conn, "SELECT COUNT(*) FROM \"Player\" WHERE role='VILLAGER' AND \"isAlive\"=true;");
                int villager_alive = atoi(PQgetvalue(res_v, 0, 0));
                
                // 4. 1年生が作った「勝敗判定関数」を呼び出す！
                int result = check_victory_condition(villager_alive, werewolf_alive);
                
                // 5. 判定結果に合わせてデータベースを「フェーズ5（ゲーム終了）」に書き換える
                if (result == 2) {
                    printf("【C言語】判定：人狼陣営の勝利！ゲームを終了します。\n");
                    PQexec(conn, "UPDATE \"Room\" SET phase = 5;");
                }
                
                PQclear(res_w);
                PQclear(res_v);
            }
            PQclear(res_room);
        }
        PQfinish(conn); // データベース切断
        sleep(2);       // 2秒眠る
    }
    return 0;
}