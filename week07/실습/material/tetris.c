#include "tetris.h"

static struct sigaction act, oact;
int B,count;

int main(){
	int exit=0;

	initscr();
	noecho();
	keypad(stdscr, TRUE);

	srand((unsigned int)time(NULL));

	while(!exit){
		clear();
		switch(menu()){
		case MENU_PLAY: play(); break;
		case MENU_EXIT: exit=1; break;
		default: break;
		}
	}

	endwin();
	system("clear");
	return 0;
}

void InitTetris(){

}

void DrawOutline(){

}

int GetCommand(){

}

int ProcessCommand(int command){

}

void DrawField(){

}


void PrintScore(int score){

}

void DrawNextBlock(int *nextBlock){

}

void DrawBlock(int y, int x, int blockID,int blockRotate,char tile){

}

void DrawBox(int y,int x, int height, int width){

}

void play(){

}

char menu(){

}

int CheckToMove(char f[HEIGHT][WIDTH],int currentBlock,int blockRotate, int blockY, int blockX){

}

void DrawChange(char f[HEIGHT][WIDTH],int command,int currentBlock,int blockRotate, int blockY, int blockX){

}

void BlockDown(int sig){

}

void AddBlockToField(char f[HEIGHT][WIDTH],int currentBlock,int blockRotate, int blockY, int blockX){

}

int DeleteLine(char f[HEIGHT][WIDTH]){

}

void DrawShadow(int y, int x, int blockID,int blockRotate){
	
}

void createRankList(){
	// 목적: Input파일인 "rank.txt"에서 랭킹 정보를 읽어들임, 읽어들인 정보로 랭킹 목록 생성
	// 1. "rank.txt"열기
	// 2. 파일에서 랭킹정보 읽어오기
	// 3. LinkedList로 저장
	// 4. 파일 닫기
	FILE *fp;
	int i, j;

	//1. 파일 열기
	fp = fopen("rnak.txt", "r");

	// 2. 정보읽어오기
	/* int fscanf(FILE* stream, const char* format, ...);
	stream:데이터를 읽어올 스트림의 FILE 객체를 가리키는 파일포인터
	format: 형식지정자 등등
	변수의 주소: 포인터
	return: 성공할 경우, fscanf 함수는 읽어들인 데이터의 수를 리턴, 실패하면 EOF리턴 */
	// EOF(End Of File): 실제로 이 값은 -1을 나타냄, EOF가 나타날때까지 입력받아오는 if문
	if (fscanf() != EOF) {




	}
	else {


	}
	// 4. 파일닫기
	fclose(fp);
}

void rank(){
	//목적: rank 메뉴를 출력하고 점수 순으로 X부터~Y까지 출력함
	//1. 문자열 초기화
	int X=1, Y=score_number, ch, i, j;
	clear();

	//2. printw()로 3개의 메뉴출력


	//3. wgetch()를 사용하여 변수 ch에 입력받은 메뉴번호 저장


	//4. 각 메뉴에 따라 입력받을 값을 변수에 저장
	//4-1. 메뉴1: X, Y를 입력받고 적절한 input인지 확인 후(X<=Y), X와 Y사이의 rank 출력
	if (ch == '1') {

	}

	//4-2. 메뉴2: 문자열을 받아 저장된 이름과 비교하고 이름에 해당하는 리스트를 출력
	else if ( ch == '2') {
		char str[NAMELEN+1];
		int check = 0;


	}

	//4-3. 메뉴3: rank번호를 입력받아 리스트에서 삭제
	else if ( ch == '3') {
		int num;

	}
	getch();

}

void writeRankFile(){
	// 목적: 추가된 랭킹 정보가 있으면 새로운 정보를 "rank.txt"에 쓰고 없으면 종료
	int sn, i;
	//1. "rank.txt" 연다
	FILE *fp = fopen("rank.txt", "r");

	//2. 랭킹 정보들의 수를 "rank.txt"에 기록

	//3. 탐색할 노드가 더 있는지 체크하고 있으면 다음 노드로 이동, 없으면 종료
	if ( sn == score_number) return;
	else {


	}
	for ( i= 1; i < score_number+1 ; i++) {
		free(a.rank_name[i]);
	}
	free(a.rank_name);
	free(a.rank_score);
}

void newRank(int score){
	// 목적: GameOver시 호출되어 사용자 이름을 입력받고 score와 함께 리스트의 적절한 위치에 저장
	char str[NAMELEN+1];
	int i, j;
	clear();
	//1. 사용자 이름을 입력받음

	//2. 새로운 노드를 생성해 이름과 점수를 저장, score_number가
	if() {

	}
	else {

	}
	writeRankFile();
}

void DrawRecommend(int y, int x, int blockID,int blockRotate){
	// user code
}

int recommend(char fieldOri[HEIGHT][WIDTH],int lv){

}

void recommendedPlay(){
	// user code
}
