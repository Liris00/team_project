#include <stdio.h>
#include <string.h>
#include <stdlib.h>  // abs 함수가 정의된 헤더 파일을 추가

// 후보자 구조체
typedef struct {
    char name[50];
    int survey_answers[10]; // 후보자의 성향
} Candidate;

// 사용자와 후보자의 일치율을 계산하는 함수 (가중치 적용)
int calculate_match_percentage(int user_answers[], int candidate_answers[], int weights[], int length) {
    int total_weight = 0;
    int weighted_match_score = 0;

    for (int i = 0; i < length; i++) {
        total_weight += weights[i];
        int difference = abs(user_answers[i] - candidate_answers[i]); // abs 함수 사용
        int match_score = weights[i] * (5 - difference); // 최대 일치 점수는 5 * weight
        weighted_match_score += match_score;
    }

    return (weighted_match_score * 100) / (total_weight * 5); // 일치율 계산
}

int main() {
    // 10가지 설문조사 질문에 대한 사용자 답변 (1: 매우 반대, 5: 매우 찬성)
    int user_answers[10] = {5, 3, 4, 2, 5, 1, 3, 4, 2, 5};

    // 각 질문의 가중치 설정 (임의로 설정)
    int weights[10] = {5, 3, 4, 2, 5, 3, 4, 1, 2, 4};

    // 후보자 정보 설정
    Candidate candidates[2] = {
        {"Candidate A", {5, 3, 4, 1, 5, 2, 3, 4, 3, 4}},
        {"Candidate B", {4, 2, 3, 5, 3, 5, 2, 1, 5, 3}}
    };

    // 각 후보자와의 일치율 계산
    for (int i = 0; i < 2; i++) {
        int match_percentage = calculate_match_percentage(user_answers, candidates[i].survey_answers, weights, 10);
        printf("%s의 일치율: %d%%\n", candidates[i].name, match_percentage);
    }

    return 0;
}
