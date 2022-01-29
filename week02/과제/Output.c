#include "Header.h"

// 입력 파일을 바탕으로 계산하고 결과를 출력하는 함수
// 각 숫자를 1, 10, 100, 1000... 등 10의 거듭제곱수로 나누어 각 자릿수의 숫자가 무엇인지 계산한다

void output(int T, int *pages) {
	int numbers[10] = { 0 }; // 0~9 까지의 숫자 개수를 저장하는 배열
	int operand; // 나누는 수
	int sub_op_1; // 연산자의 왼쪽에 오는 수(나눠지는 수)
	int sub_op_2; // 연산자의 오른쪽에 오는 수(나누는 수)
	int result; // 연산 결과값

	for (int i = 0; i < T; i++) {
		operand = 1; // 나누는 수는 1에서 시작
		for (int j = 1; j <= pages[i]; j++) {
			if (j == operand * 10) operand *= 10; // 페이지번호 자릿수가 증가할 때 마다 나누는 수 * 10을 해준다
			result = j / operand; // 페이지번호를 operand로 나눈 값을 result 변수에 저장
			numbers[result]++; // 해당 숫자 개수를 나타내는 numbers 배열의 값 +1

			// while루프는 현재 페이지 번호가 두자릿수 이상일 때 시행되며, 가장 높은자릿수 아래의 자릿수들에 대해 계산한다
			sub_op_2 = operand; // while 루프 안에서 사용될 나누는 수 
			sub_op_1 = j; // while 루프 안에서 사용될 나눠지는 수
			sub_op_1 -= (result*operand); // 제일 높은 자릿수의 숫자는 이미 계산 완료했으므로 삭제 

			while (sub_op_2 / 10 > 0) {
				sub_op_2 = sub_op_2 / 10; // 제일 높은 자릿수 바로 아래 자리부터 계산
				result = sub_op_1 / sub_op_2;
				numbers[result]++;
				sub_op_1 -= (result*sub_op_2); // 아직 남아있는 자릿수가 있다면 그것을 계산하기 위해 현재 자릿수 삭제
			}
		}
		// 숫자의 갯수를 센 결과를 출력한다
		for (int i = 0; i < 10; i++) {
			printf("%d ", numbers[i]);
			numbers[i] = 0;
		} printf("\n");
		// 출력 후 다음 testcase가 있다면 그것을 계산
	}
}
