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

  // 네비게이션 바 색상 랜덤 변경
  const navbar = document.getElementById("navbar");
  if (navbar) {
      function getRandomPastelColor() {
          const red = Math.floor(Math.random() * 127 + 128);
          const green = Math.floor(Math.random() * 127 + 128);
          const blue = Math.floor(Math.random() * 127 + 128);
          return `rgb(${red}, ${green}, ${blue})`;
      }

      function changeNavbarColor() {
          navbar.style.backgroundColor = getRandomPastelColor();
      }

      setInterval(changeNavbarColor, 2000);
  }

  // 메뉴 토글 기능
  const menuToggle = document.getElementById("menu-toggle");
  const navLinks = document.getElementById("nav-links");

  if (menuToggle && navLinks) {
      menuToggle.addEventListener("click", () => {
          navLinks.classList.toggle("active");
      });
  }

  // 질문 데이터 (직접 입력)
  const questions = [
      "당신은 사회적 평등을 중요하게 생각합니까?",
      "경제적 자유가 중요하다고 생각합니까?",
      "환경 보호를 위해 개인의 불편함을 감수할 의향이 있습니까?",
      "정부의 역할이 경제에서 중요한가요?",
      "표현의 자유가 무엇보다 중요하다고 생각합니까?",
      "모든 시민에게 기본 소득이 제공되어야 한다고 생각합니까?",
      "국가 안보가 개인의 자유보다 우선입니까?",
      "다양성을 존중해야 한다고 생각합니까?",
      "과학 기술 발전이 인간의 삶을 항상 향상시킨다고 생각합니까?",
      "지역 사회가 중요한가요?"

  ];

  const questionsPerPage = 5;
  let currentPage = 1;

  // 질문 렌더링
  function renderQuestions(page) {
      const startIndex = (page - 1) * questionsPerPage;
      const visibleQuestions = questions.slice(startIndex, startIndex + questionsPerPage);

      const questionContainer = document.getElementById("question-container");
      questionContainer.innerHTML = ""; // 기존 질문 초기화

      visibleQuestions.forEach((question) => {
          const questionDiv = document.createElement("div");
          questionDiv.classList.add("question");
          questionDiv.innerHTML = `
              <p>${question}</p>
              <div class="options">
                  <button class="option" data-value="1">완전 비동의</button>
                  <button class="option" data-value="2">비동의</button>
                  <button class="option" data-value="3">중립</button>
                  <button class="option" data-value="4">동의</button>
                  <button class="option" data-value="5">완전 동의</button>
              </div>
          `;
          questionContainer.appendChild(questionDiv);
      });

      // 옵션 클릭 이벤트 추가
      document.querySelectorAll(".options").forEach((group) => {
          group.addEventListener("click", (event) => {
              if (event.target.classList.contains("option")) {
                  group.querySelectorAll(".option").forEach((btn) => btn.classList.remove("active"));
                  event.target.classList.add("active");
              }
          });
      });
  }

  // 페이지 버튼 렌더링
  function renderPagination() {
      const totalPages = Math.ceil(questions.length / questionsPerPage);
      const paginationButtons = document.getElementById("pagination-buttons");
      paginationButtons.innerHTML = "";

      for (let i = 1; i <= totalPages; i++) {
          const button = document.createElement("button");
          button.classList.add("page-btn");
          button.textContent = i;
          button.disabled = i === currentPage;
          button.addEventListener("click", () => goToPage(i));
          paginationButtons.appendChild(button);
      }
  }

  // 페이지 이동
  function goToPage(page) {
      currentPage = page;
      renderQuestions(currentPage);
      renderPagination();
  }

  // 결과 보기
  function showResults() {
      const results = {};
      document.querySelectorAll(".options").forEach((group, index) => {
          const selected = group.querySelector(".option.active");
          results[`질문 ${index + 1}`] = selected ? selected.textContent : "선택 안 됨";
      });

      const resultDiv = document.getElementById("result");
      resultDiv.innerHTML = Object.entries(results)
          .map(([question, answer]) => `<p>${question}: ${answer}</p>`)
          .join("");
  }

  // 초기 렌더링
  renderQuestions(currentPage);
  renderPagination();

  // 결과 버튼 이벤트
  const resultBtn = document.getElementById("result-btn");
  if (resultBtn) {
      resultBtn.addEventListener("click", showResults);
  }
});
