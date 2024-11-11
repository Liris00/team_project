#include <mysql/mysql.h>    // MySQL API를 사용하기 위한 헤더 파일
#include <stdio.h>           // 표준 입출력을 위한 헤더 파일
#include <stdlib.h>          // 표준 라이브러리 함수들을 위한 헤더 파일
#include <string.h>          // 문자열 처리 함수들을 위한 헤더 파일
#include <crypt.h>           // crypt() 함수를 사용하기 위한 헤더 파일 (비밀번호 해싱)

// 사용자 등록 함수 선언
void register_user(const char *username, const char *password); 

int main() {
    const char *username = "test2";  // 사용자 이름 설정
    const char *password = "testpw2"; // 사용자의 비밀번호 설정

    register_user(username, password);  // 등록 함수 호출
    return 0;
}

// 사용자 등록 함수 정의
void register_user(const char *username, const char *password) {
    MYSQL *conn;                      // MySQL 연결을 위한 변수
    MYSQL_STMT *stmt;                  // SQL 구문 실행을 위한 변수
    MYSQL_BIND bind[2];                // 바인딩된 매개변수들을 위한 배열
    char hashed_password[128];         // 해시된 비밀번호를 저장할 배열

    // 비밀번호 해싱 (crypt() 함수를 사용하여 비밀번호를 암호화)
    char *salt = "$6$rounds=5000$usesomesillystringforsalt$";  // bcrypt 스타일의 salt 값 설정
    strcpy(hashed_password, crypt(password, salt));  // 비밀번호를 salt를 사용하여 해싱하고 결과를 hashed_password에 저장

    // MySQL 연결 초기화
    conn = mysql_init(NULL);           // MySQL 연결을 초기화 (NULL을 넣으면 기본 설정을 사용)
    if (conn == NULL) {                // 초기화 실패 시
        fprintf(stderr, "mysql_init() failed\n");  // 오류 메시지 출력
        exit(1);  // 프로그램 종료
    }

    // MySQL에 연결
    if (mysql_real_connect(conn, "localhost", "root", "12345678", "user_db", 3306, NULL, 0) == NULL) {
        // 연결 실패 시 오류 메시지 출력
        fprintf(stderr, "mysql_real_connect() failed: %s\n", mysql_error(conn));
        mysql_close(conn);    // MySQL 연결 종료
        exit(1);  // 프로그램 종료
    }

    // SQL 쿼리 준비
    const char *query = "INSERT INTO users (username, password) VALUES (?, ?)";  // 사용자를 등록하는 SQL 쿼리
    stmt = mysql_stmt_init(conn);          // MySQL 쿼리 준비
    if (!stmt) {                           // 쿼리 준비 실패 시
        fprintf(stderr, "mysql_stmt_init() out of memory\n");  // 메모리 부족 오류 출력
        mysql_close(conn);    // MySQL 연결 종료
        exit(1);  // 프로그램 종료
    }
    if (mysql_stmt_prepare(stmt, query, strlen(query))) {  // SQL 쿼리 준비 실패 시
        fprintf(stderr, "mysql_stmt_prepare() failed\n");  // 오류 메시지 출력
        fprintf(stderr, "%s\n", mysql_stmt_error(stmt));  // 추가 오류 메시지 출력
        mysql_stmt_close(stmt);    // MySQL 쿼리 종료
        mysql_close(conn);    // MySQL 연결 종료
        exit(1);  // 프로그램 종료
    }

    // 바인딩 설정
    memset(bind, 0, sizeof(bind));  // 바인딩 배열 초기화

    // username 바인딩
    bind[0].buffer_type = MYSQL_TYPE_STRING;  // 데이터 유형: 문자열
    bind[0].buffer = (char *)username;        // 바인딩할 데이터: 사용자 이름
    bind[0].buffer_length = strlen(username); // 바인딩할 데이터의 길이

    // hashed_password 바인딩
    bind[1].buffer_type = MYSQL_TYPE_STRING;  // 데이터 유형: 문자열
    bind[1].buffer = hashed_password;         // 바인딩할 데이터: 해시된 비밀번호
    bind[1].buffer_length = strlen(hashed_password); // 바인딩할 데이터의 길이

    if (mysql_stmt_bind_param(stmt, bind)) {  // 파라미터 바인딩
        fprintf(stderr, "mysql_stmt_bind_param() failed\n");  // 오류 메시지 출력
        fprintf(stderr, "%s\n", mysql_stmt_error(stmt));  // 추가 오류 메시지 출력
        mysql_stmt_close(stmt);    // MySQL 쿼리 종료
        mysql_close(conn);    // MySQL 연결 종료
        exit(1);  // 프로그램 종료
    }

    // SQL 쿼리 실행
    if (mysql_stmt_execute(stmt)) {  // SQL 쿼리 실행 실패 시
        fprintf(stderr, "mysql_stmt_execute() failed\n");  // 오류 메시지 출력
        fprintf(stderr, "%s\n", mysql_stmt_error(stmt));  // 추가 오류 메시지 출력
    } else {
        printf("User registered successfully!\n");  // 사용자 등록 성공 메시지 출력
    }

    // 정리
    mysql_stmt_close(stmt);  // MySQL 쿼리 종료
    mysql_close(conn);  // MySQL 연결 종료
}
