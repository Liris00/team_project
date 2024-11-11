#include <mysql/mysql.h>
#include <stdio.h>
#include <stdlib.h>
//데이터베이스 생성
void create_database(MYSQL *conn, const char *db_name) {
    char query[256];
    sprintf(query, "CREATE DATABASE IF NOT EXISTS %s", db_name);

    if (mysql_query(conn, query)) {
        fprintf(stderr, "데이터베이스 생성 실패: %s\n", mysql_error(conn));
        exit(1);
    } else {
        printf("데이터베이스 '%s'가 성공적으로 생성되었습니다!\n", db_name);
    }
}

int main() {
    MYSQL *conn = mysql_init(NULL);
    
    // MySQL 서버에 연결
    if (mysql_real_connect(conn, "localhost", "root", "12345678", NULL, 3306, NULL, 0) == NULL) {
        fprintf(stderr, "mysql_real_connect() 실패: %s\n", mysql_error(conn));
        mysql_close(conn);
        exit(1);
    }

    // 새로운 데이터베이스 생성
    create_database(conn, "new_user_db");

    // 연결 종료
    mysql_close(conn);
    return 0;
}