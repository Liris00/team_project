// main.js

// 총 질문 수와 페이지당 질문 수
const totalQuestions = 60
const questionsPerPage = 6;
const totalPages = Math.ceil(totalQuestions / questionsPerPage);

// 현재 페이지
let currentPage = 1;

// 질문 페이지 설정
function setupPagination() {
    const formContainer = document.querySelector('.form-container');
    const questions = document.querySelectorAll('.question');

    // 페이지 내 질문 표시/숨김
    questions.forEach((question, index) => {
        const pageIndex = Math.floor(index / questionsPerPage) + 1;
        question.setAttribute('data-page', pageIndex);

        if (pageIndex !== currentPage) {
            question.style.display = 'none';
        }
    });

    // 페이지 버튼 추가
    const pagination = document.createElement('div');
    pagination.classList.add('pagination');
    pagination.innerHTML = `
        <button id="prevPage" onclick="changePage(-1)" disabled>이전</button>
        <span id="pageInfo">${currentPage} / ${totalPages}</span>
        <button id="nextPage" onclick="changePage(1)">다음</button>
    `;
    formContainer.appendChild(pagination);
}

// 페이지 이동
function changePage(direction) {
    const questions = document.querySelectorAll('.question');

    // 현재 페이지 업데이트
    currentPage += direction;

    // 페이지 정보 업데이트
    document.getElementById('pageInfo').textContent = `${currentPage} / ${totalPages}`;

    // 이전/다음 버튼 활성화/비활성화
    document.getElementById('prevPage').disabled = currentPage === 1;
    document.getElementById('nextPage').disabled = currentPage === totalPages;

    // 질문 표시/숨김
    questions.forEach((question) => {
        const questionPage = parseInt(question.getAttribute('data-page'), 10);
        question.style.display = questionPage === currentPage ? 'block' : 'none';
    });
}

// 결과 보기
function showResults() {
    // 사용자가 선택한 값을 수집
    const userSelections = {};
    document.querySelectorAll('.question .options .option.selected').forEach((option) => {
        const questionId = option.closest('.question').id;
        const value = option.getAttribute('data-value');
        userSelections[questionId] = parseInt(value, 10);
    });

    // 결과 계산 (여기서는 단순히 평균 점수 계산)
    const totalScore = Object.values(userSelections).reduce((sum, value) => sum + value, 0);
    const averageScore = totalScore / Object.keys(userSelections).length;

    let resultMessage = '';
    if (averageScore <= 2) {
        resultMessage = '당신의 성향은 보수적입니다!';
    } else if (averageScore > 2 && averageScore <= 4) {
        resultMessage = '당신의 성향은 중도적입니다!';
    } else {
        resultMessage = '당신의 성향은 진보적입니다!';
    }

    // 결과 표시
    const resultDiv = document.getElementById('result');
    resultDiv.innerHTML = `<h2>테스트 결과</h2><p>${resultMessage}</p>`;
    resultDiv.style.display = 'block';

    console.log('User Selections:', userSelections);
    console.log('Average Score:', averageScore);
}

// 선택 이벤트 추가
document.querySelectorAll('.options .option').forEach((option) => {
    option.addEventListener('click', function () {
        const siblings = this.parentNode.querySelectorAll('.option');
        siblings.forEach((sibling) => sibling.classList.remove('selected')); // 다른 선택 해제
        this.classList.add('selected'); // 현재 선택
    });
});

// 페이지 설정 실행
document.addEventListener('DOMContentLoaded', setupPagination);
