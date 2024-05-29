//2-4까지 완료
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>

#define LEN_MIN 15
#define LEN_MAX 50
#define STM_MIN 0
#define STM_MAX 5
#define PROB_MIN 10
#define PROB_MAX 90
#define AGGRO_MIN 0
#define AGGRO_MAX 5

#define MOVE_LEFT 1
#define MOVE_STAY 0

#define ATK_NONE 0
#define ATK_CITIZEN 1
#define ATK_DONGSEOK 2

#define ACTION_REST 0
#define ACTION_PROVOKE 1
#define ACTION_PULL 2

int len, prob, stamina;
int C, M, Z;
int citizen_aggro = 1, madongseok_aggro = 1;
int count = 1;
int preC, preZ, preM;

void initialize();
void print_train();
void move_phase();
void action_phase();

int main() {
	initialize();
	while (1) {
		move_phase();
		action_phase();

		// 종료 조건
		if (C == 1) {
			printf("SUCCESS!\n");
			printf("citizen(s) escaped to the next train\n");
			return 0;
		}

		if (C == Z - 1) {
			printf("Game OVER!\n");
			printf("Citizen(s) has(have) been attacked by a zombie\n");
			return 0;
		}

		count += 1;
	}
}

void initialize() {
	printf("train length(%d~%d)>> ", LEN_MIN, LEN_MAX);
	while (scanf_s("%d", &len) != 1 || len < LEN_MIN || len > LEN_MAX) {
		printf("train length(%d~%d)>> ", LEN_MIN, LEN_MAX);
		while (getchar() != '\n');  // clear input buffer
	}

	printf("madongseok stamina(%d~%d)>> ", STM_MIN, STM_MAX);
	while (scanf_s("%d", &stamina) != 1 || stamina < STM_MIN || stamina > STM_MAX) {
		printf("madongseok stamina(%d~%d)>> ", STM_MIN, STM_MAX);
		while (getchar() != '\n');  // clear input buffer
	}

	printf("percentile probability 'p'(%d~%d)>> ", PROB_MIN, PROB_MAX);
	while (scanf_s("%d", &prob) != 1 || prob < PROB_MIN || prob > PROB_MAX) {
		printf("percentile probability 'p'(%d~%d)>> ", PROB_MIN, PROB_MAX);
		while (getchar() != '\n');  // clear input buffer
	}

	C = len - 6;
	Z = len - 3;
	M = len - 2;

	print_train();
}

void print_train() {
	for (int i = 0; i < len; i++) {
		printf("#");
	}
	printf("\n");
	for (int i = 0; i < len; i++) {
		if (i == 0 || i == len - 1) {
			printf("#");
		}
		else if (i == C) {
			printf("C");
		}
		else if (i == Z) {
			printf("Z");
		}
		else if (i == M) {
			printf("M");
		}
		else {
			printf(" ");
		}
	}
	printf("\n");
	for (int i = 0; i < len; i++) {
		printf("#");
	}
	printf("\n\n");
}

void move_phase() {
	int rCit = rand() % 101;
	int rZom = rand() % 101;

	preC = C;
	preZ = Z;
	preM = M;

	// 시민 이동
	if (rCit <= 100 - prob) {
		C -= 1;
		citizen_aggro++;
	}
	else {
		citizen_aggro--;
	}

	// 좀비 이동 (매 2턴마다)
	if (count % 2 == 1) {
		if (rZom <= prob) {
			if (citizen_aggro >= madongseok_aggro) {
				Z = (Z - 1 != C) ? Z - 1 : Z;
			}
			else {
				Z = (Z - 1 != M) ? Z - 1 : Z;
			}
		}
	}

	print_train();

	// 마동석 이동
	printf("madongseok move(0:stay, 1:left)>> ");
	int move;
	while (scanf_s("%d", &move) != 1 || (move != MOVE_LEFT && move != MOVE_STAY)) {
		printf("madongseok move(0:stay, 1:left)>> ");
		while (getchar() != '\n');  // 입력 버퍼 정리
	}
	printf("\n");
	if (move == MOVE_LEFT) {
		M -= 1;
		madongseok_aggro++;
	}
	else {
		madongseok_aggro--;
	}

	if (madongseok_aggro > AGGRO_MAX) madongseok_aggro = AGGRO_MAX;
	if (madongseok_aggro < AGGRO_MIN) madongseok_aggro = AGGRO_MIN;

	print_train();

	// 이동 후 상태 출력
	printf("citizen: %d -> %d (aggro: %d)\n", preC, C, citizen_aggro);
	printf("zombie: %d -> %d\n", preZ, Z);
	printf("madongseok: %d -> %d (aggro: %d)\n\n", preM, M, madongseok_aggro);
}

void action_phase() {
	printf("madongseok action(0.rest, 1.provoke, 2.pull)>> ");
	int action;
	while (scanf_s("%d", &action) != 1 || (action != ACTION_REST && action != ACTION_PROVOKE && action != ACTION_PULL)) {
		printf("madongseok action(0.rest, 1.provoke, 2.pull)>> ");
		while (getchar() != '\n');  // 입력 버퍼 정리
	}
	printf("\n");
	int preAggro = madongseok_aggro;
	int preStamina = stamina;

	switch (action) {
	case ACTION_REST:
		stamina++;
		madongseok_aggro--;
		break;
	case ACTION_PROVOKE:
		madongseok_aggro = AGGRO_MAX;
		break;
	case ACTION_PULL:
		madongseok_aggro += 2;
		stamina--;
		if (rand() % 101 <= 100 - prob) {
			printf("madongseok pulled zombie... Next turn, it can't move\n");
		}
		break;
	default:
		printf("Invalid action.\n");
		break;
	}

	if (madongseok_aggro > AGGRO_MAX) madongseok_aggro = AGGRO_MAX;
	if (madongseok_aggro < AGGRO_MIN) madongseok_aggro = AGGRO_MIN;
	if (stamina > STM_MAX) stamina = STM_MAX;
	if (stamina < STM_MIN) stamina = STM_MIN;

	// 행동 후 상태 출력
	printf("madongseok: aggro %d -> %d, stamina %d -> %d\n\n", preAggro, madongseok_aggro, preStamina, stamina);
}
