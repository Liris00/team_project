from flask import Flask, render_template, request, redirect, url_for
import subprocess

app = Flask(__name__)

@app.route('/', methods=['GET', 'POST'])
def register():
    if request.method == 'POST':
        # 폼 데이터 수집
        username = request.form['username']
        password = request.form['password']
        email = request.form['email']
        age = request.form['age']
        sex = request.form['sex']
        job = request.form['job']
        keyword1 = request.form['keyword1']
        keyword2 = request.form['keyword2']
        keyword3 = request.form['keyword3']

        # C 프로그램 실행 (테이블 생성)
        result_create_table = subprocess.run(
            ['./c_program/build/create_table'],
            capture_output=True,
            text=True,
            encoding='utf-8',  # UTF-8 인코딩 시도
            errors='replace'   # 디코딩 오류 발생 시 대체 문자를 사용
        )
        print("C 프로그램 출력 (테이블 생성):")
        print(result_create_table.stdout)
        # 테이블 생성 실패 처리
        if result_create_table.returncode != 0:
            print("테이블 생성 실패:", result_create_table.stderr)
            return f"테이블 생성 실패: {result_create_table.stderr}", 500

        # C 프로그램 실행 (사용자 등록)
        result_register_user = subprocess.run(
            ['./c_program/build/register_user_desk', username, password, email, age, sex, job, keyword1, keyword2, keyword3],
            capture_output=True,
            text=True,
            encoding='utf-8',  # UTF-8 인코딩 시도
            errors='replace'   # 디코딩 오류 발생 시 대체 문자를 사용
        )

        # 사용자 등록 실패 처리
        if result_register_user.returncode != 0:
            print("사용자 등록 실패:", result_register_user.stderr)
            return f"사용자 등록 실패: {result_register_user.stderr}", 500


        print("C 프로그램 출력 (테이블 생성):")
        print(result_create_table.stdout)

        
        # 사용자 등록 성공 시, 성공 메시지 출력
        print("C 프로그램 출력 (사용자 등록):")
        print(result_register_user.stdout)
        return redirect(url_for('success'))

    return render_template('register.html')

@app.route('/success')
def success():
    return "사용자 등록 성공!"

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5000)
