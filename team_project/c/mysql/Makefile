# 컴파일러 설정
CC = gcc

# MySQL Connector/C 경로 설정 (실제 설치 경로로 수정)
MYSQL_DIR = C:\\Program Files\\MySQL\\MySQL Server 8.0

# 헤더 파일 경로
CFLAGS = -I"$(MYSQL_DIR)\\include"

# 라이브러리 경로
LDFLAGS = -L"$(MYSQL_DIR)\\lib"

# 링크할 라이브러리
LDLIBS = -lmysql

LDLIBS1 = -lmysql -lcrypt32

# 디버그 플래그 추가
DEBUG = -g

# 실행 파일 이름
TARGETS = register_user.exe create_table.exe register_user_desk.exe

# 소스 파일들
SRCS_REGISTER = register_user.c
SRCS_CREATE = create_table.c
SRCS_DESK = register_user_desk.c

# 기본 타겟
all: $(TARGETS)

# 컴파일 규칙
register_user.exe: $(SRCS_REGISTER)
	@echo Compiling register_user.exe...
	$(CC) $(DEBUG) $(CFLAGS) $^ -o $@ $(LDFLAGS) $(LDLIBS)
	@echo register_user.exe compiled successfully!

#register_user_desk.exe 규칙 추가
register_user_desk.exe: $(SRCS_DESK)
	@echo Compiling register_user_desk.exe...
	$(CC) $(DEBUG) $(CFLAGS) $^ -o $@ $(LDFLAGS) $(LDLIBS1)
	@echo register_user_desk.exe compiled successfully!

create_table.exe: $(SRCS_CREATE)
	@echo Compiling create_table.exe...
	$(CC) $(DEBUG) $(CFLAGS) $^ -o $@ $(LDFLAGS) $(LDLIBS)
	@echo create_table.exe compiled successfully!

# 실행 규칙 추가
run_register: register_user.exe
	@echo Running register_user.exe...
	./register_user.exe

run_create: create_table.exe
	@echo Running create_table.exe...
	./create_table.exe

run_desk: register_user_desk.exe
	@echo Running register_user_desk.exe...
	./register_user_desk.exe

# 청소 규칙
clean:
	@echo Cleaning up...
	del /F /Q $(TARGETS)
	@echo Cleanup complete!

# .PHONY는 clean이 파일이름이 아닌 타겟임을 명시
.PHONY: clean all run_register run_create run_desk