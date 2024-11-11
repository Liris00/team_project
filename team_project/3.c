#include <mysql/mysql.h>
#include <stdio.h>
#include <stdlib.h>
// 테이블 데이터 csv로 변환
void export_table_to_csv(MYSQL *conn, const char *table_name, const char *file_path) {
    char query[256];
    
    sprintf(query, "SELECT * FROM %s INTO OUTFILE '%s' FIELDS TERMINATED BY ',' LINES TERMINATED BY '\n'", table_name, file_path);

    if (mysql_query(conn, query)) {
        fprintf(stderr, "데이터 내보내기 실패: %s\n", mysql_error(conn));
        exit(1);
    } else {
        printf("'%s' 파일로 데이터가 성공적으로 내보내졌습니다!\n", file_path);
    }
}

int main() {
    MYSQL *conn = mysql_init(NULL);

    // MySQL 서버 및 데이터베이스에 연결
    if (mysql_real_connect(conn, "localhost", "root", "12345678", "user_db", 3306, NULL, 0) == NULL) {
        fprintf(stderr, "mysql_real_connect() 실패: %s\n", mysql_error(conn));
        mysql_close(conn);
        exit(1);
    }

    // 테이블 데이터를 CSV 파일로 내보내기
    export_table_to_csv(conn, "users", "/tmp/users.csv");

    // 연결 종료
    mysql_close(conn);
    
    return 0;
}