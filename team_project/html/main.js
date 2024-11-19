document.addEventListener("DOMContentLoaded", function () {
    // 버튼 클릭 이벤트
    const buttonActions = {
        "login-btn": () => open("log-in.html"),
        "signup-btn": () => open("signup.html"),
        "log-in-btn": () => alert("님 어서 오세요."),
        "return-btn": () => alert("아직 구현 안 됨"),
        "home-btn": () => open("start.html"),
    };

    Object.entries(buttonActions).forEach(([id, action]) => {
        const button = document.getElementById(id);
        if (button) {
            button.addEventListener("click", action);
        }
    });

// 질문 데이터
const questions = [
    "1. 질문 1",
    "2. 질문 2",
    "3. 질문 3",
    "4. 질문 4",
    "5. 질문 5",
    "6. 질문 6",
    "7. 질문 7",
    "8. 질문 8",
    "9. 질문 9",
    "10. 질문 10",
    "11. 질문 11",
    "12. 질문 12"
];

let currentPage = 1;
const questionsPerPage = 6;

// 질문 생성 함수
function renderQuestions(page) {
    const startIndex = (page - 1) * questionsPerPage;
    const endIndex = startIndex + questionsPerPage;
    const visibleQuestions = questions.slice(startIndex, endIndex);

    const questionContainer = document.getElementById("question-container");
    questionContainer.innerHTML = ""; // 기존 질문 초기화

    visibleQuestions.forEach((question, index) => {
        const questionDiv = document.createElement("div");
        questionDiv.classList.add("question");
        questionDiv.innerHTML = `
            <p>${question}</p>
            <div class="options">
                <button type="button" class="option" data-value="1">완전 비동의</button>
                <button type="button" class="option" data-value="2">비동의</button>
                <button type="button" class="option" data-value="3">중립</button>
                <button type="button" class="option" data-value="4">동의</button>
                <button type="button" class="option" data-value="5">완전 동의</button>
            </div>
        `;
        questionContainer.appendChild(questionDiv);
    });

    // 페이지 버튼 상태 업데이트
    document.getElementById("prev-btn").disabled = page === 1;
    document.getElementById("next-btn").disabled = page === Math.ceil(questions.length / questionsPerPage);
}

// 페이지 이동 함수
function nextPage() {
    currentPage++;
    renderQuestions(currentPage);
}

function prevPage() {
    currentPage--;
    renderQuestions(currentPage);
}

// 초기 질문 렌더링
renderQuestions(currentPage);



  