# 컴파일러 설정
CC = gcc

# 헤더 파일 경로
CFLAGS = -I/usr/include/mysql

# 라이브러리 경로
LDFLAGS = -L/usr/lib/x86_64-linux-gnu

# 링크할 라이브러리
LDLIBS = -lmariadb

# 링크할 라이브러리
LDLIBS2 = -lmariadb -lssl -lcrypto  # OpenSSL 라이브러리 추가

# 디버그 플래그 추가
DEBUG = -g

# 실행 파일 이름
TARGETS = create_table register_user_desk

# 소스 파일들
SRCS_CREATE = create_table.c
SRCS_DESK = register_user_desk.c

# 기본 타겟
all: $(TARGETS)

# 컴파일 규칙

create_table: $(SRCS_CREATE)
	@echo Compiling create_table...
	$(CC) $(DEBUG) $(CFLAGS) $^ -o build/create_table $(LDFLAGS) $(LDLIBS)
	@echo create_table compiled successfully!

register_user_desk: $(SRCS_DESK)
	@echo Compiling register_user_desk...
	$(CC) $(DEBUG) $(CFLAGS) $^ -o build/register_user_desk $(LDFLAGS) $(LDLIBS2)
	@echo register_user_desk compiled successfully!


# 실행 규칙 추가
run_create: create_table
	@echo Running create_table...
	./create_table

run_desk: register_user_desk
	@echo Running register_user_desk...
	./register_user_desk

# 청소 규칙
clean:
	@echo Cleaning up...
	rm -f $(TARGETS)
	@echo Cleanup complete!

# .PHONY는 clean이 파일이름이 아닌 타겟임을 명시
.PHONY: clean all run_create run_desk