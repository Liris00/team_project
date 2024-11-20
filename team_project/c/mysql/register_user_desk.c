#include <mysql.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <wincrypt.h>
#pragma comment(lib, "crypt32.lib")

// MySQL 연결 정보 정의
#define DB_HOST "localhost"
#define DB_USER "root"
#define DB_PASS "12345678"
#define DB_NAME "user_db"
#define DB_PORT 3306
#define TABLE_NAME "users1"

// 테스트용 사용자 정보 상수 정의
#define TEST_USERNAME "test2"
#define TEST_PASSWORD "testpw2"
#define TEST_EMAIL "test2@example.com"
#define TEST_AGE 26
#define TEST_SEX "Female"
#define TEST_JOB "Engineer"
#define TEST_KEYWORD1 "Security"
#define TEST_KEYWORD2 "Cryptography"
#define TEST_KEYWORD3 "Networking"

// 사용자 정보 구조체 정의
typedef struct {
    const char *username;
    const char *password;
    const char *email;
    int age;
    const char *sex;
    const char *job;
    const char *keyword1;
    const char *keyword2;
    const char *keyword3;
} UserInfo;

// SHA-256 해시 함수
void sha256_hash(const char *password, char *hashed_password) {
    HCRYPTPROV hProv = 0;
    HCRYPTHASH hHash = 0;
    BYTE rgbHash[32];
    DWORD cbHash = 0;
    char hex_digits[] = "0123456789abcdef";

    if (!CryptAcquireContext(&hProv, NULL, NULL, PROV_RSA_AES, CRYPT_VERIFYCONTEXT)) {
        fprintf(stderr, "CryptAcquireContext failed: %lu\n", GetLastError());
        exit(1);
    }

    if (!CryptCreateHash(hProv, CALG_SHA_256, 0, 0, &hHash)) {
        fprintf(stderr, "CryptCreateHash failed: %lu\n", GetLastError());
        CryptReleaseContext(hProv, 0);
        exit(1);
    }

    if (!CryptHashData(hHash, (BYTE*)password, strlen(password), 0)) {
        fprintf(stderr, "CryptHashData failed: %lu\n", GetLastError());
        CryptDestroyHash(hHash);
        CryptReleaseContext(hProv, 0);
        exit(1);
    }

    cbHash = 32;
    if (!CryptGetHashParam(hHash, HP_HASHVAL, rgbHash, &cbHash, 0)) {
        fprintf(stderr, "CryptGetHashParam failed: %lu\n", GetLastError());
        CryptDestroyHash(hHash);
        CryptReleaseContext(hProv, 0);
        exit(1);
    }

    // 해시값을 16진수 문자열로 변환
    for (DWORD i = 0; i < cbHash; i++) {
        hashed_password[i * 2] = hex_digits[rgbHash[i] >> 4];
        hashed_password[i * 2 + 1] = hex_digits[rgbHash[i] & 0xf];
    }
    hashed_password[cbHash * 2] = '\0';

    if (hHash) 
        CryptDestroyHash(hHash);
    if (hProv) 
        CryptReleaseContext(hProv, 0);
}

// 사용자 등록 함수 선언
void sha256_hash(const char *password, char *hashed_password);
void register_user(const UserInfo *user);

int main() {
    // 테스트용 사용자 정보를 상수를 사용하여 초기화
    UserInfo user = {
        .username = TEST_USERNAME,
        .password = TEST_PASSWORD,
        .email = TEST_EMAIL,
        .age = TEST_AGE,
        .sex = TEST_SEX,
        .job = TEST_JOB,
        .keyword1 = TEST_KEYWORD1,
        .keyword2 = TEST_KEYWORD2,
        .keyword3 = TEST_KEYWORD3
    };

    // 사용자 정보 출력 (디버깅용)
    printf("다음 정보로 사용자를 등록합니다:\n");
    printf("사용자명: %s\n", user.username);
    printf("이메일: %s\n", user.email);
    printf("나이: %d\n", user.age);
    printf("성별: %s\n", user.sex);
    printf("직업: %s\n", user.job);
    printf("키워드: %s, %s, %s\n", 
           user.keyword1, user.keyword2, user.keyword3);

    register_user(&user);
    
    printf("\n종료하려면 Enter 키를 누르세요...");
    getchar();
    return 0;
}

void register_user(const UserInfo *user) {
    MYSQL *conn;
    MYSQL_STMT *stmt;
    MYSQL_BIND bind[9];  // 9개의 필드에 대한 바인딩
    char hashed_password[65];  // SHA-256 해시는 32바이트(64자 16진수 문자열 + NULL)

    // 비밀번호 해싱
    sha256_hash(user->password, hashed_password);
    printf("해시된 비밀번호: %s\n", hashed_password);  // 디버깅용

    // MySQL 초기화
    conn = mysql_init(NULL);
    if (conn == NULL) {
        fprintf(stderr, "MySQL 초기화 실패\n");
        exit(1);
    }

    // SSL 모드 설정
    unsigned int ssl_mode = SSL_MODE_DISABLED;
    mysql_options(conn, MYSQL_OPT_SSL_MODE, &ssl_mode);

    // 연결 타임아웃 설정
    int timeout = 10;
    mysql_options(conn, MYSQL_OPT_CONNECT_TIMEOUT, &timeout);

    printf("MySQL 서버에 연결 중...\n");
    if (mysql_real_connect(conn, DB_HOST, DB_USER, DB_PASS, DB_NAME, DB_PORT, NULL, 0) == NULL) {
        fprintf(stderr, "MySQL 연결 실패: %s\n", mysql_error(conn));
        mysql_close(conn);
        exit(1);
    }
    printf("MySQL 서버 연결 성공!\n");

    // SQL 쿼리 준비
    const char *query = "INSERT INTO users1 (username, password, email, age, sex, job, keyword1, keyword2, keyword3) "
                       "VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?)";
    stmt = mysql_stmt_init(conn);
    if (!stmt) {
        fprintf(stderr, "MySQL 구문 초기화 실패\n");
        mysql_close(conn);
        exit(1);
    }

    printf("쿼리 준비 중...\n");
    if (mysql_stmt_prepare(stmt, query, strlen(query))) {
        fprintf(stderr, "쿼리 준비 실패: %s\n", mysql_stmt_error(stmt));
        mysql_stmt_close(stmt);
        mysql_close(conn);
        exit(1);
    }

    // 바인딩 설정
    printf("매개변수 바인딩 중...\n");
    memset(bind, 0, sizeof(bind));

    // username
    bind[0].buffer_type = MYSQL_TYPE_STRING;
    bind[0].buffer = (char *)user->username;
    bind[0].buffer_length = strlen(user->username);

    // password (hashed)
    bind[1].buffer_type = MYSQL_TYPE_STRING;
    bind[1].buffer = hashed_password;
    bind[1].buffer_length = strlen(hashed_password);

    // email
    bind[2].buffer_type = MYSQL_TYPE_STRING;
    bind[2].buffer = (char *)user->email;
    bind[2].buffer_length = strlen(user->email);

    // age
    bind[3].buffer_type = MYSQL_TYPE_LONG;
    bind[3].buffer = (char *)&user->age;

    // sex
    bind[4].buffer_type = MYSQL_TYPE_STRING;
    bind[4].buffer = (char *)user->sex;
    bind[4].buffer_length = strlen(user->sex);

    // job
    bind[5].buffer_type = MYSQL_TYPE_STRING;
    bind[5].buffer = (char *)user->job;
    bind[5].buffer_length = strlen(user->job);

    // keyword1
    bind[6].buffer_type = MYSQL_TYPE_STRING;
    bind[6].buffer = (char *)user->keyword1;
    bind[6].buffer_length = strlen(user->keyword1);

    // keyword2
    bind[7].buffer_type = MYSQL_TYPE_STRING;
    bind[7].buffer = (char *)user->keyword2;
    bind[7].buffer_length = strlen(user->keyword2);

    // keyword3
    bind[8].buffer_type = MYSQL_TYPE_STRING;
    bind[8].buffer = (char *)user->keyword3;
    bind[8].buffer_length = strlen(user->keyword3);

    if (mysql_stmt_bind_param(stmt, bind)) {
        fprintf(stderr, "mysql_stmt_bind_param() failed: %s\n", mysql_stmt_error(stmt));
        mysql_stmt_close(stmt);
        mysql_close(conn);
        exit(1);
    }

    // SQL 쿼리 실행
    printf("쿼리 실행 중...\n");
    if (mysql_stmt_execute(stmt)) {
        const char *error_msg = mysql_stmt_error(stmt);
        fprintf(stderr, "쿼리 실행 실패: %s\n", mysql_stmt_error(stmt));
        
        // 중복 사용자명 오류 확인
        if (strstr(error_msg, "Duplicate entry") && strstr(error_msg, "username")) {
            printf("\n[오류] 이미 존재하는 사용자명입니다: '%s'\n", user->username);
            printf("다른 사용자명을 사용해주세요.\n");
        } else {
            printf("\n[오류] 데이터베이스 오류가 발생했습니다.\n");
            printf("자세한 오류 내용: %s\n", error_msg);
        }
    } else {
        printf("사용자 등록 성공!\n");
    }

    // 정리
    printf("정리 작업 중...\n");
    mysql_stmt_close(stmt);
    mysql_close(conn);
    printf("완료!\n");
}
