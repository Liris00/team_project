// MySQL 데이터베이스 작업을 위한 필수 헤더 파일들
#include <mysql.h>    // MySQL C API 함수들을 사용하기 위한 헤더
#include <stdio.h>          // 표준 입출력 함수(printf, fprintf 등)를 위한 헤더
#include <stdlib.h>         // 메모리 할당 및 프로그램 종료(exit)를 위한 헤더
#include <string.h>         // 문자열 처리 함수들을 위한 헤더

// 데이터베이스 연결 및 테이블 설정을 위한 상수 정의
#define DB_HOST "localhost"     // MySQL 서버 주소
#define DB_USER "root"         // MySQL 사용자 이름
#define DB_PASS "12345678"     // MySQL 비밀번호
#define DB_PORT 3306           // MySQL 서버 포트
#define DB_NAME "user_db"      // 사용할 데이터베이스 이름
#define TABLE_NAME "users1"     // 생성할 테이블 이름
#define CONNECT_TIMEOUT 3      // 데이터베이스 연결 시도 제한 시간(초)

// 쿼리문 작성을 위한 버퍼 크기 설정
// 충분한 크기를 확보하여 긴 SQL 쿼리도 처리할 수 있도록 함
#define QUERY_BUFFER_SIZE 1024

/**
 * MySQL 오류 처리를 위한 함수
 * @param conn: MySQL 연결 핸들러
 * @param message: 출력할 에러 메시지
 */
void handle_error(MYSQL *conn, const char *message) {
    fprintf(stderr, "%s: %s\n", message, mysql_error(conn));  // 에러 메시지와 MySQL 에러 내용 출력
    mysql_close(conn);    // MySQL 연결 종료
    exit(1);             // 프로그램 종료
}

/**
 * 데이터베이스 존재 여부를 확인하고 없으면 생성하는 함수
 * @param conn: MySQL 연결 핸들러
 */
void check_database_exists(MYSQL *conn) {
    char query[QUERY_BUFFER_SIZE];  // SQL 쿼리를 저장할 버퍼

    // 데이터베이스 사용 시도
    snprintf(query, sizeof(query), "USE %s", DB_NAME);
    
    if (mysql_query(conn, query)) {  // 데이터베이스가 없는 경우
        // 데이터베이스 생성 쿼리 준비
        snprintf(query, sizeof(query), "CREATE DATABASE IF NOT EXISTS %s", DB_NAME);
        
        // 데이터베이스 생성 시도
        if (mysql_query(conn, query)) {
            handle_error(conn, "데이터베이스 생성 실패");
        }
        
        // 생성된 데이터베이스 선택
        snprintf(query, sizeof(query), "USE %s", DB_NAME);
        if (mysql_query(conn, query)) {
            handle_error(conn, "데이터베이스 선택 실패");
        }
        printf("데이터베이스 '%s'가 성공적으로 생성되었습니다.\n", DB_NAME);
    }
}

/**
 * 테이블 존재 여부를 확인하는 함수
 * @param conn: MySQL 연결 핸들러
 */
void check_table_exists(MYSQL *conn) {
    char query[QUERY_BUFFER_SIZE];
    
    // 테이블 존재 여부 확인 쿼리 준비
    snprintf(query, sizeof(query), "SHOW TABLES LIKE '%s'", TABLE_NAME);
    
    // 쿼리 실행
    if (mysql_query(conn, query)) {
        handle_error(conn, "테이블 확인 중 오류 발생");
    }
    
    // 쿼리 결과 저장
    MYSQL_RES *result = mysql_store_result(conn);
    if (!result) {
        handle_error(conn, "결과 저장 실패");
    }
    
    // 결과 행이 있으면 테이블이 이미 존재
    if (mysql_num_rows(result) > 0) {
        printf("테이블 '%s'가 이미 존재합니다.\n", TABLE_NAME);
        mysql_free_result(result);  // 결과 메모리 해제
        return;
    }
    mysql_free_result(result);  // 결과 메모리 해제
}

int main() {
    MYSQL *conn;  // MySQL 연결을 위한 핸들러
    
    // MySQL 연결 초기화
    conn = mysql_init(NULL);
    if (conn == NULL) {
        fprintf(stderr, "MySQL 초기화 실패: 메모리 부족\n");
        exit(1);
    }

    // 연결 타임아웃 설정 수정
    unsigned int timeout = CONNECT_TIMEOUT;  // unsigned int 변수로 선언
    if (mysql_options(conn, MYSQL_OPT_CONNECT_TIMEOUT, (const void *)&timeout)) {
        handle_error(conn, "연결 옵션 설정 실패");
    }

    // MySQL 서버에 연결
    if (mysql_real_connect(conn, DB_HOST, DB_USER, DB_PASS, 
        NULL, DB_PORT, NULL, 0) == NULL) {
        handle_error(conn, "MySQL 연결 실패");
    }
    printf("MySQL 서버에 성공적으로 연결되었습니다.\n");

    // 데이터베이스 확인 및 생성
    check_database_exists(conn);
    // 테이블 존재 여부 확인
    check_table_exists(conn);

    // CREATE TABLE 쿼리 문자열 생성
    char create_table_query[QUERY_BUFFER_SIZE * 4];  // 큰 쿼리를 위한 충분한 버퍼
    snprintf(create_table_query, sizeof(create_table_query),
        "CREATE TABLE IF NOT EXISTS %s ("
        "id INT AUTO_INCREMENT PRIMARY KEY,"          // 자동 증가하는 고유 식별자
        "username VARCHAR(150) UNIQUE NOT NULL,"      // 중복 불가능한 사용자명
        "password VARCHAR(200) NOT NULL,"             // 암호화된 비밀번호 저장
        "email VARCHAR(120) UNIQUE NOT NULL,"         // 중복 불가능한 이메일
        "age INT NOT NULL,"                          // 사용자 나이
        "sex VARCHAR(20) NOT NULL,"                  // 사용자 성별
        "job VARCHAR(150) NOT NULL,"                 // 사용자 직업
        "keyword1 VARCHAR(150) NOT NULL,"            // 관심 키워드 1
        "keyword2 VARCHAR(150) NOT NULL,"            // 관심 키워드 2
        "keyword3 VARCHAR(150) NOT NULL,"            // 관심 키워드 3
        "created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP," // 계정 생성 시간
        "INDEX idx_username (username),"             // 사용자명 검색 최적화
        "INDEX idx_email (email)"                    // 이메일 검색 최적화
        ") ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci", // UTF-8 문자셋 사용
        TABLE_NAME);

    // 테이블 생성 쿼리 실행
    if (mysql_query(conn, create_table_query)) {
        handle_error(conn, "테이블 생성 실패");
    }

    // 생성된 테이블 구조 확인
    char describe_query[QUERY_BUFFER_SIZE];
    snprintf(describe_query, sizeof(describe_query), "DESCRIBE %s", TABLE_NAME);
    
    if (mysql_query(conn, describe_query)) {
        handle_error(conn, "테이블 정보 조회 실패");
    }

    // 테이블 구조 정보 가져오기
    MYSQL_RES *result = mysql_store_result(conn);
    if (!result) {
        handle_error(conn, "결과 저장 실패");
    }

    // 테이블 구조 출력
    printf("\n테이블 '%s' 구조:\n", TABLE_NAME);
    MYSQL_ROW row;
    
    // 컬럼 헤더 출력
    printf("%-15s %-15s %-10s %-10s\n", "Field", "Type", "Null", "Key");
    printf("----------------------------------------\n");
    
    // 각 필드 정보 출력
    while ((row = mysql_fetch_row(result))) {
        printf("%-15s %-15s %-10s %-10s\n", 
            row[0], row[1], row[2], row[3]);
    }

    // 메모리 정리 및 연결 종료
    mysql_free_result(result);
    printf("\n테이블 '%s'가 성공적으로 생성되었습니다!\n", TABLE_NAME);
    mysql_close(conn);
    
    return 0;
}