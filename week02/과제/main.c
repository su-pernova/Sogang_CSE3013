#include "Header.h"

int main() {
	int T; // tescase의 총 개수를 저장하는 변수
	int* pages; // 각 testcase에서의 페이지수를 저장하는 배열
	char filename[] = "Text.txt"; // open할 txt파일 이름

	pages = input(&T, filename);
	output(T, pages);

	return 0;
}
