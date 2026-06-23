#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <libpq-fe.h>

int main() {
    printf("C言語 GMサーバー起動。勝敗を監視中...\n");
    const char *conninfo = "dbname=jinro_db user=jinro_user password=jinro_password host=localhost port=5432";
    
    while(1) {
        PGconn *conn = PQconnectdb(conninfo);
        if (PQstatus(conn) == CONNECTION_OK) {
            
            // フェーズ4（夜）の部屋があるかチェック
            PGresult *res_room = PQexec(conn, "SELECT id FROM \"Room\" WHERE phase = 4 LIMIT 1;");
            if (PQntuples(res_room) > 0) {
                char *room_id = PQgetvalue(res_room, 0, 0);
                
                // 生きている人狼の数を数える
                PGresult *res_w = PQexec(conn, "SELECT COUNT(*) FROM \"Player\" WHERE role='WEREWOLF' AND \"isAlive\"=true;");
                int werewolf_alive = atoi(PQgetvalue(res_w, 0, 0));
                
                // 生きている村人の数を数える
                PGresult *res_v = PQexec(conn, "SELECT COUNT(*) FROM \"Player\" WHERE role='VILLAGER' AND \"isAlive\"=true;");
                int villager_alive = atoi(PQgetvalue(res_v, 0, 0));
                
                // 勝利判定
                if (werewolf_alive >= villager_alive && werewolf_alive > 0) {
                    printf("【決着】人狼陣営の勝利！フェーズを5にします。\n");
                    PQexec(conn, "UPDATE \"Room\" SET phase = 5;");
                } else if (werewolf_alive == 0 && villager_alive > 0) {
                    printf("【決着】村人陣営の勝利！フェーズを5にします。\n");
                    PQexec(conn, "UPDATE \"Room\" SET phase = 5;");
                }
                
                PQclear(res_w);
                PQclear(res_v);
            }
            PQclear(res_room);
        }
        PQfinish(conn);
        sleep(2);
    }
    return 0;
}