#include <mysql/mysql.h>
#include <stdio.h>
#include <stdlib.h>
//테이블 목록 조회
void list_tables(MYSQL *conn) {
    if (mysql_query(conn, "SHOW TABLES")) {
        fprintf(stderr, "테이블 목록 조회 실패: %s\n", mysql_error(conn));
        exit(1);
    }

    MYSQL_RES *res = mysql_store_result(conn);
    if (res == NULL) {
        fprintf(stderr, "mysql_store_result() 실패: %s\n", mysql_error(conn));
        exit(1);
    }

    MYSQL_ROW row;
    printf("데이터베이스의 테이블 목록:\n");
    while ((row = mysql_fetch_row(res)) != NULL) {
        printf("%s\n", row[0]);
    }

    mysql_free_result(res);
}

int main() {
    MYSQL *conn = mysql_init(NULL);

    // MySQL 서버 및 데이터베이스에 연결
    if (mysql_real_connect(conn, "localhost", "root", "12345678", "user_db", 3306, NULL, 0) == NULL) {
        fprintf(stderr, "mysql_real_connect() 실패: %s\n", mysql_error(conn));
        mysql_close(conn);
        exit(1);
    }

    // 테이블 목록 조회
    list_tables(conn);

    // 연결 종료
    mysql_close(conn);
    return 0;
}
