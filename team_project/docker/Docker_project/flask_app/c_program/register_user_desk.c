#include <openssl/evp.h>  // EVP 인터페이스를 사용하기 위한 헤더
#include <mysql.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

// MySQL 연결 정보 정의
#define DB_HOST "mysql_db"  // Docker Compose에서 정의한 MySQL 서비스 이름
#define DB_USER "root"
#define DB_PASS "12345678"
#define DB_NAME "user_db"
#define DB_PORT 3306
#define TABLE_NAME "users1"

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

// SHA-256 해싱 함수
void sha256_hash(const char *input, char *output) {
    unsigned char hash[EVP_MAX_MD_SIZE];
    unsigned int length = 0;
    EVP_MD_CTX *context = EVP_MD_CTX_new();

    if (context != NULL) {
        if (EVP_DigestInit_ex(context, EVP_sha256(), NULL) &&
            EVP_DigestUpdate(context, input, strlen(input)) &&
            EVP_DigestFinal_ex(context, hash, &length)) {
            for (unsigned int i = 0; i < length; i++) {
                sprintf(output + (i * 2), "%02x", hash[i]);
            }
            output[length * 2] = 0;  // NULL 종료
        }
        EVP_MD_CTX_free(context);
    }
}

// 사용자 등록 함수 선언
void register_user(const UserInfo *user);

int main(int argc, char *argv[]) {
    
    setlocale(LC_ALL, "en_US.UTF-8");

    if (argc != 10) {
        fprintf(stderr, "Usage: %s <username> <password> <email> <age> <sex> <job> <keyword1> <keyword2> <keyword3>\n", argv[0]);
        return 1;
    }

    // 명령줄 인자로 받은 사용자 정보를 초기화
    UserInfo user = {
        .username = argv[1],
        .password = argv[2],
        .email = argv[3],
        .age = atoi(argv[4]),
        .sex = argv[5],
        .job = argv[6],
        .keyword1 = argv[7],
        .keyword2 = argv[8],
        .keyword3 = argv[9]
    };

    // 비밀번호 해싱
    char hashed_password[65];  // SHA-256 해시는 64자 + NULL
    sha256_hash(user.password, hashed_password);
    printf("해시된 비밀번호: %s\n", hashed_password);  // 디버깅용

    // 사용자 등록 함수 호출
    register_user(&user);

    return 0;
}

void register_user(const UserInfo *user) {
    MYSQL *conn;
    MYSQL_STMT *stmt;
    MYSQL_BIND bind[9];  // 9개의 필드에 대한 바인딩
    char hashed_password[65];  // SHA-256 해시는 32바이트(64자 16진수 문자열 + NULL)

    // 비밀번호 해싱 (해싱 함수는 별도로 구현해야 함)
    sha256_hash(user->password, hashed_password);
    printf("해시된 비밀번호: %s\n", hashed_password);  // 디버깅용

    // MySQL 초기화
    conn = mysql_init(NULL);
    if (conn == NULL) {
        fprintf(stderr, "MySQL 초기화 실패\n");
        exit(1);
    }

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
