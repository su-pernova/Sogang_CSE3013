#include "Header.h"

// 입력 파일의 정보를 읽어 저장하는 함수
// testcase의 총 개수를 저장하는 변수 T와, 열고자 하는 txt파일의 이름을 매개변수로 받음  

int* input(int *T, char *test) {
	FILE *fp;
	fp = fopen(test, "r"); // 입력 정보가 들어있는 텍스트파일 open
	if (fp == NULL) {
		fprintf(stderr, "file open error \n");
		exit(1); }

	fscanf(fp, "%d", T); // testcase의 개수를 변수 T에 저장
	int* pages = malloc(sizeof(int) * (*T)); // 각 testcase의 페이지수를 저장하는 배열 동적할당

	for (int i = 0; i < (*T); i++) {
		fscanf(fp, "%d", &(pages[i])); // 각 testcase의 페이지수를 배열에 저장
	}
	fclose(fp);

	return pages; // 입력받은 testcase에 대한 정보가 저장된 배열 pages를 return
}
