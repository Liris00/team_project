FROM python:3.9

# 필수 패키지 설치
RUN apt-get update && apt-get install -y \
    gcc \
    libmariadb-dev \
    make \
    libssl-dev

# 작업 디렉토리 설정
WORKDIR /app

# 소스 코드 복사
COPY . .



# Python 패키지 설치
RUN pip install --no-cache-dir -r requirements.txt

RUN make -C c_program

# Flask 애플리케이션 실행
CMD ["flask", "run", "--host=0.0.0.0"]


