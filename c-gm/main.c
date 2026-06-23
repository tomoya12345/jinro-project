#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <libpq-fe.h>

// ==========================================
// 💡 C言語班 1年生への開発タスク
// ==========================================
// 以下の関数のなかみを作って、勝利判定をして！
// 戻り値: 0=ゲーム継続, 1=村人の勝ち, 2=人狼の勝ち
int check_victory_condition(int villager_alive_count, int werewolf_alive_count) {
    
    // 【タスク】
    // もし人狼の数が0なら、村人の勝ち(1)を返す。
    // もし人狼の数が村人の数以上になったら、人狼の勝ち(2)を返す。
    // どちらでもなければ、ゲーム継続(0)を返す。
    
    return 0; // とりあえず0を返しておく
}

int main() {
    printf("C言語 GMサーバー起動...\n");
    const char *conninfo = "dbname=jinro_db user=jinro_user password=jinro_password host=localhost port=5432";
    
    while(1) {
        PGconn *conn = PQconnectdb(conninfo);
        if (PQstatus(conn) == CONNECTION_OK) {
            
            int werewolf_alive_count = 1; // 仮のデータ
            int villager_alive_count = 3; // 仮のデータ
            
            // 1年生が作った関数を呼び出す！
            int result = check_victory_condition(villager_alive_count, werewolf_alive_count);
            
            if (result == 1) {
                printf("村人陣営の勝利！DBのフェーズを5(終了)に書き換えます。\n");
            } else if (result == 2) {
                printf("人狼陣営の勝利！DBのフェーズを5(終了)に書き換えます。\n");
            }
        }
        PQfinish(conn);
        sleep(2);
    }
    return 0;
}