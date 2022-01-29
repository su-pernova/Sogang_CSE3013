#include "tetris.h"

static struct sigaction act, oact;

typedef struct node{
	char player[NAMELEN+1];
	int scr;
	struct node *next;
} node;

int main(){
	int exit=0;

	initscr();
	noecho();
	keypad(stdscr, TRUE);	

	srand((unsigned int)time(NULL));

	createRankList();

	while(!exit){
		clear();
		switch(menu()){
		case MENU_PLAY: play(); break;
		case MENU_RANK: rank(head); break;
		case MENU_EXIT: exit=1; break;
		default: break;
		}
	}

	endwin();
	system("clear");
	return 0;
}

// O(HEIGHTxWIDTH), O(HEIGHTxWIDTH)
void InitTetris(){
	int i,j;

	for(j=0;j<HEIGHT;j++)
		for(i=0;i<WIDTH;i++)
			field[j][i]=0;

	nextBlock[0]=rand()%7;
	nextBlock[1]=rand()%7;
	nextBlock[2]=rand()%7;

	blockRotate=0;
	blockY=-1;
	blockX=WIDTH/2-2;
	score=0;	
	gameOver=0;
	timed_out=0;

	DrawOutline(); // O(HEIGHT), O(HEIGHTxWIDTH)
	DrawField(); // O(HEIGHTxWIDTH), O(HEIGHTxWIDTH)
	DrawBlock(blockY,blockX,nextBlock[0],blockRotate,' '); // O(BLOCK_HEIGHTxBLOCK_WIDTH), O(BLOCK_HEIGHTxBLOCK_WIDTH)
	DrawNextBlock(nextBlock); // O(BLOCK_HEIGHTxBLOCK_WIDTH), O(BLOCK_HEIGHTxBLOCK_WIDTH)
	PrintScore(score);
}

void DrawOutline(){	
	int i,j;
	/* 블럭이 떨어지는 공간의 테두리를 그린다 */
	DrawBox(0,0,HEIGHT,WIDTH); // O(HEIGHT), O(HEIGHTxWIDTH)

	/* next block을 보여주는 공간의 테두리를 그린다 */
	/* move = ncurses lib 함수. 커서를 (y,x)로 이동시킨다 */
	move(2,WIDTH+10);
	printw("NEXT BLOCK");
	DrawBox(3,WIDTH+10,4,8);
	move(8,WIDTH+10);
	DrawBox(9,WIDTH+10,4,8);

	/* score를 보여주는 공간의 테두리를 그린다 */
	move(15,WIDTH+10);
	printw("SCORE");
	DrawBox(16,WIDTH+10,1,8);
}

int GetCommand(){
	int command;
	command = wgetch(stdscr);
	switch(command){
	case KEY_UP:
		break;
	case KEY_DOWN:
		break;
	case KEY_LEFT:
		break;
	case KEY_RIGHT:
		break;
	case ' ':
		/* space key */
		/* fall block */
		break;
	case 'q':
	case 'Q':
		command = QUIT;
		break;
	default:
		command = NOTHING;
		break;
	}
	return command;
}

int ProcessCommand(int command){
	int ret=1;
	int drawFlag=0;
	switch(command){
	case QUIT:
		ret = QUIT;
		break;
	case KEY_UP:
		if((drawFlag = CheckToMove(field,nextBlock[0],(blockRotate+1)%4,blockY,blockX)))
			blockRotate=(blockRotate+1)%4;
		break;
	case KEY_DOWN:
		if((drawFlag = CheckToMove(field,nextBlock[0],blockRotate,blockY+1,blockX)))
			blockY++;
		break;
	case KEY_RIGHT:
		if((drawFlag = CheckToMove(field,nextBlock[0],blockRotate,blockY,blockX+1)))
			blockX++;
		break;
	case KEY_LEFT:
		if((drawFlag = CheckToMove(field,nextBlock[0],blockRotate,blockY,blockX-1)))
			blockX--;
		break;
	default:
		break;
	}
	if(drawFlag) DrawChange(field,command,nextBlock[0],blockRotate,blockY,blockX);
	return ret;	
}

// 1로 셋팅된 필드의 자리는 "▩"로 채우고, 나머지는 "." 으로 채운다
void DrawField(){
	int i,j;
	for(j=0;j<HEIGHT;j++){
		move(j+1,1);
		for(i=0;i<WIDTH;i++){
			if(field[j][i]==1){
				attron(A_REVERSE);
				printw(" ");
				attroff(A_REVERSE);
			}
			else printw(".");
		}
	}
	// 커서가 화면을 가리지 않도록 옮겨주기
	move(HEIGHT,WIDTH+10);
}


void PrintScore(int score){
	move(17,WIDTH+11);
	printw("%8d",score);
}

void DrawNextBlock(int *nextBlock){
	// nextBlock[1]
	for( int i = 0; i < BLOCK_HEIGHT; i++ ){
		move(4+i,WIDTH+13);
		for( int j = 0; j < BLOCK_WIDTH; j++ ){
			if( block[nextBlock[1]][0][i][j] == 1 ){
				attron(A_REVERSE);
				printw(" ");
				attroff(A_REVERSE);
			}
			else printw(" ");
		}
	}

	// nextBlock[2]
	for( int i = 0; i < BLOCK_HEIGHT; i++ ){
		move(10+i,WIDTH+13);
		for( int j = 0; j < BLOCK_WIDTH; j++ ){
			if( block[nextBlock[2]][0][i][j] == 1 ){
				attron(A_REVERSE);
				printw(" ");
				attroff(A_REVERSE);
			}
			else printw(" ");
		}
	}
}

// O(BLOCK_HEIGHT×BLOCK_WIDTH), O(BLOCK_HEIGHT×BLOCK_WIDTH)
void DrawBlock(int y, int x, int blockID,int blockRotate,char tile){
	// y = blockY, x = blockX
	int i,j;
	for(i=0;i<BLOCK_HEIGHT;i++)
		for(j=0;j<BLOCK_WIDTH;j++){
			if(block[blockID][blockRotate][i][j]==1 && i+y>=0){
				move(i+y+1,j+x+1); // 왜 +1 을 해주지?
				attron(A_REVERSE);
				printw("%c",tile);
				attroff(A_REVERSE);
			}
		}
	// 커서가 화면을 가리지 않도록 옮겨주기
	move(HEIGHT,WIDTH+10);
}

void DrawBox(int y,int x, int height, int width){
	int i,j;
	move(y,x);
	addch(ACS_ULCORNER);
	for(i=0;i<width;i++)
		addch(ACS_HLINE);
	addch(ACS_URCORNER);
	for(j=0;j<height;j++){
		move(y+j+1,x);
		addch(ACS_VLINE);
		move(y+j+1,x+width+1);
		addch(ACS_VLINE);
	}
	move(y+j+1,x);
	addch(ACS_LLCORNER);
	for(i=0;i<width;i++)
		addch(ACS_HLINE);
	addch(ACS_LRCORNER);
}

void play(){
	int command;
	clear();
	act.sa_handler = BlockDown;
	sigaction(SIGALRM,&act,&oact);
	InitTetris();
	do{
		if(timed_out==0){
			alarm(1);
			timed_out=1;
		}

		command = GetCommand();
		if(ProcessCommand(command)==QUIT){
			alarm(0);
			DrawBox(HEIGHT/2-1,WIDTH/2-5,1,10);
			move(HEIGHT/2,WIDTH/2-4);
			printw("Good-bye!!");
			refresh();
			getch();

			return;
		}
	}while(!gameOver);

	alarm(0);
	getch();
	DrawBox(HEIGHT/2-1,WIDTH/2-5,1,10);
	move(HEIGHT/2,WIDTH/2-4);
	printw("GameOver!!");
	refresh();
	getch();
	newRank(score);
}

char menu(){
	printw("1. play\n");
	printw("2. rank\n");
	printw("3. recommended play\n");
	printw("4. exit\n");
	return wgetch(stdscr);
}

///////////////////////// 1주차 과제에서 구현해야 할 함수 /////////////////////////

void DrawShadow(int y, int x, int blockID,int blockRotate){
	// user code
	// 현재 블록을 더 이상 내릴 수 없을 때 까지 blockY를 증가시킴
	// 해당 위치에 DrawBlock('/')
	while (CheckToMove(field, blockID, blockRotate, y+1, x)==1) y++; // O(BLOCK_HEIGHT×BLOCK_WIDTH), O(HEIGHT×WIDTH)
	DrawBlock(y, x, blockID, blockRotate, '/'); // O(BLOCK_HEIGHT×BLOCK_WIDTH), O(BLOCK_HEIGHT×BLOCK_WIDTH)
}

void DrawBlockWithFeatures(int y, int x, int blockID, int blockRotate){
	DrawShadow(y, x, blockID, blockRotate); // O(BLOCK_HEIGHT×BLOCK_WIDTH), O(HEIGHT×WIDTH)
	DrawBlock(y, x, blockID, blockRotate, ' '); // O(BLOCK_HEIGHT×BLOCK_WIDTH), O(BLOCK_HEIGHT×BLOCK_WIDTH)
}

///////////////////////// 1주차 실습에서 구현해야 할 함수 /////////////////////////

int CheckToMove(char f[HEIGHT][WIDTH],int currentBlock,int blockRotate,int blockY,int blockX){
	int i, j; // 블록은 4x4 행렬이므로 이중 loop 사용
	for (i = 0; i < BLOCK_HEIGHT; i++){
		for (j = 0; j < BLOCK_WIDTH; j++){
			if (block[currentBlock][blockRotate][i][j] == 1){
				// 1) 블록을 놓으려고 하는 필드에 이미 블록이 있는지
				if(f[blockY + i][blockX + j] == 1) return 0;
				// 2) 블록 요소가 실제 필드상의 y 좌표가 Height 보다 크거나 같은지(큰 경우만 아님?)
				else if ((blockY + i) >= HEIGHT) return 0;
				// 3) 블록 요소가 실제 필드상의 x 좌표가 0보다 작은지
				else if ((blockX + j) < 0) return 0;
				// 4) 블록 요소가 실제 필드상의 x 좌표가 WIDTH보다 크거나 같은지(큰 경우만 아님?)
				else if ((blockX + j) >= WIDTH) return 0;
			}
		}
	}
	return 1;
}

void DrawChange(char f[HEIGHT][WIDTH], int command, int currentBlock, int blockRotate, int blockY, int blockX){
	// 1) 이전 블록 정보를 찾는다. ProcessCommand의 switch문을 참조할 것
	int tempR = blockRotate, tempY = blockY, tempX = blockX;
	switch(command){
	case KEY_UP:
		tempR = (tempR+3) % 4; break;
	case KEY_DOWN:
		tempY--; break;
	case KEY_RIGHT:
		tempX--; break;
	case KEY_LEFT:
		tempX++; break;
	}

	// 2) 이전 블록 정보를 지운다. DrawBlock함수 참조
	for (int i = 0; i < BLOCK_HEIGHT; i++){
		for (int j = 0; j < BLOCK_WIDTH; j++){
			if (block[currentBlock][tempR][i][j] == 1 && tempY+i >= 0){
				move(i+tempY+1, j+tempX+1); // (DrawBlock에서 가져온 것)
				printw(".");
			}
		}
	}

	// 3) 이전 shadow 정보를 지운다. DrawBlock함수 참조
	while(CheckToMove(f, currentBlock, tempR, tempY+1, tempX)==1) tempY++;
	for (int i = 0; i < BLOCK_HEIGHT; i++){
		for (int j = 0; j < BLOCK_WIDTH; j++){
			if (block[currentBlock][tempR][i][j] == 1 && tempY+i >= 0){
				move(i+tempY+1, j+tempX+1); // (DrawBlock에서 가져온 것)
				printw(".");
			}
		}
	}

	// 4) 새로운 블록과 shadow 정보를 그린다.
	DrawBlockWithFeatures(blockY, blockX, currentBlock, blockRotate);
}

void BlockDown(int sig){	
	// 강의자료 p26-27의 flowchart 참고
	// 매개변수 int sig 는 신호를 받는 변수 이므로 함수 내부에서 사용하지 않아도 됨
	// 왼쪽부터 순서대로 시간복잡도, 공간복잡도
	timed_out = 0;
	
	if (CheckToMove(field, nextBlock[0], blockRotate, blockY+1, blockX)==1){ // O(BLOCK_HEIGHT×BLOCK_WIDTH), O(HEIGHT×WIDTH) 
		DrawChange(field, KEY_DOWN, nextBlock[0], blockRotate, ++blockY, blockX); // O(BLOCK_HEIGHT×BLOCK_WIDTH), O(HEIGHT×WIDTH) 
		return;
	}
	else {
		score += AddBlockToField(field,nextBlock[0],blockRotate,blockY,blockX); // O(BLOCK_HEIGHT×BLOCK_WIDTH), O(HEIGHT×WIDTH)
		if (blockY == -1) gameOver = 1;
	}
	
	// 한 줄을 가득 채워 없애는 경우 +100점 
	score += DeleteLine(field); // O(HEIGHT×WIDTH), O(HEIGHT×WIDTH)
	PrintScore(score); // O(1)

	// 현재 블럭을 다음 블럭으로 바꾸고, 다음 블럭 업데이트
	nextBlock[0] = nextBlock[1];
	nextBlock[1] = nextBlock[2];
	nextBlock[2] = rand()%7;
	DrawNextBlock(nextBlock); // O(BLOCK_HEIGHT×BLOCK_WIDTH), O(BLOCK_HEIGHT×BLOCK_WIDTH)

	// 현재 블록의 위치를 초기화
	blockX = WIDTH/2 -2;
	blockY = -1;
	blockRotate = 0;
	DrawField(); // O(HEIGHT×WIDTH), O(HEIGHT×WIDTH)

}

int AddBlockToField(char f[HEIGHT][WIDTH],int currentBlock,int blockRotate,int blockY,int blockX){
	// Block이 추가된 영역의 필드값을 바꾼다
	int scr, touched = 0;
	for(int i = 0; i<WIDTH; i++) f[HEIGHT][i] = 1; // 필드 맨 아래 테두리 부분의 필드값을 1로 채워준다
	for (int i = 0; i < BLOCK_HEIGHT; i++){
		for (int j = 0; j < BLOCK_WIDTH; j++){
			if (block[currentBlock][blockRotate][i][j] == 1){
				f[blockY + i][blockX + j] = 1;
				if(f[blockY + i + 1][blockX + j] == 1) touched++;
			}
		}
	}
	scr = touched * 10;

	return scr;
}

int DeleteLine(char f[HEIGHT][WIDTH]){
	// 1) 필드에 꽉 찬 구간이 있는지 탐색
	int flag, score = 0;
	for (int i = 0; i < HEIGHT; i++){ // HEIGHT = 22
		flag = 0;
		for (int j = 0; j < WIDTH; j++){ // WIDTH = 10
			if(f[i][j]==0) { // 현재 탐색중인 행에 빈 자리가 있음
				flag = 1;
				break;
			}
		}
		// 2) 꽉 찬 구간이 있으면(i번째 행) 해당 구간을 지운다(해당 구간으로 필드값을 한칸씩 내린다)
		if(flag == 0) {
			for(int k=i; k>=0; k--){
				for(int l=0; l<WIDTH; l++) {
					if(k==0) f[k][l] = 0;
					else f[k][l] = f[k-1][l];
				}
			}
			// 3) 점수 갱신 후 출력
			score += 100;
		}
	}
	return score;
}

///////////////////////// 2주차 실습에서 구현해야 할 함수 /////////////////////////
void createRankList(){
	// Input파일인 "rank.txt"에서 랭킹 정보를 읽어들여 랭킹 목록 생성
	FILE *fp;
	int this_score;
	char name[NAMELEN+1];

	//1. rank.txt 파일 열기
	fp = fopen("rank.txt", "r");

	// 2. 파일에서 랭킹정보 읽어 LinkedList로 저장
	fscanf(fp, "%d", &score_number);

	// 가장 첫번째 값 입력받기
	struct node *p = NULL;
	p = (struct node *)malloc(sizeof(struct node));
	fscanf(fp, "%s", name); strcpy(p->player, name);
	fscanf(fp, "%d", &this_score); p->scr = this_score;
	p->next = NULL;
	head = p;

	// 첫번째 값 다음부터 입력받기
	for(int i = 0; i<score_number-1; i++) {
		struct node *p;
		p = (struct node *)malloc(sizeof(struct node));
		fscanf(fp, "%s", name); strcpy(p->player, name);
		fscanf(fp, "%d", &this_score); p->scr = this_score;
		p->next = NULL;

		// 가장 큰 값일 경우 : 첫번째에 삽입
		if(this_score > head->scr) {
			p->next = head;
			head = p;
		}

		else {
			int flag = 0;
			struct node *temp = head;
			while(temp->next != NULL){
				// 중간 값일 경우 : 적절한 자리에 삽입
				if(temp->scr >= this_score && temp->next->scr < this_score) {
					p->next = temp->next;
					temp->next = p;
					flag = 1;
					break;
				}
				else temp = temp->next;
			}
			if (flag==0) temp->next = p; // 가장 작은 값일 경우 : 마지막에 삽입
		}

	}

	// 4. 파일닫기
	fclose(fp);
}

void rank(struct node *x){
	//목적: rank 메뉴를 출력하고 점수 순으로 X부터~Y까지 출력함
	//1. 문자열 초기화
	int X=1, Y=score_number, ch, i, j;
	clear();

	//2. printw()로 3개의 메뉴출력
	printw("1. list ranks from X to Y\n");
	printw("2. list ranks by a specific name\n");
	printw("3. delete a specific rank\n");

	//3. wgetch()를 사용하여 변수 ch에 입력받은 메뉴번호 저장
	ch = wgetch(stdscr);

	//4. 각 메뉴에 따라 입력받을 값을 변수에 저장
	//4-1. 메뉴1: X, Y를 입력받고 적절한 input인지 확인(X<=Y)
	if (ch == '1') {
		echo();
		printw("X: ");
		echo();
		scanw("%d", &X);
		printw("Y: ");
		scanw("%d", &Y);
		noecho();
		printw("        name        |        score        \n");
		printw("------------------------------------------\n");
		if(X > Y || score_number == 0 || X > score_number)
			mvprintw(8,0,"search failure: no rank in the list\n");
		else { // X와 Y사이의 rank 출력
			int index = 1;
			while(1){
				if(index == X){
					while(index != Y+1){
						printw("%11s%17d\n", x->player, x->scr);
						x = x->next; index++; }
					break; }
				x = x->next;
				index++;
			}
		}
	}

	//4-2. 메뉴2: 문자열을 받아 저장된 이름과 비교하고 이름에 해당하는 리스트를 출력 (7주차 과제)
	else if ( ch == '2') {
		char str[NAMELEN+1];
		int check = 0;
	}

	//4-3. 메뉴3: rank번호를 입력받아 리스트에서 삭제 (7주차 과제)
	else if ( ch == '3') {
		int num;
	}
	getch();

}

void newRank(int score){
	// 목적: GameOver시 호출되어 사용자 이름을 입력받고 score와 함께 리스트의 적절한 위치에 저장
	char str[NAMELEN+1];
	int i, j;
	clear();

	//1. 사용자 이름을 입력받음
	printw("your name: ");
	echo();
	scanw("%s", &str);
	score_number++;

	//2. 새로운 노드를 생성해 이름과 점수를 저장
	struct node *p;
	p = (struct node *)malloc(sizeof(struct node));
	strcpy(p->player, str);
	p->scr = score;
	p->next = NULL;

	//3. 연결리스트에 삽입
	if(score > head->scr) { // 가장 큰 값일 경우
		p->next = head;
		head = p;
	}

	else {
		int flag = 0;
		struct node *temp = head;
		while(temp->next != NULL){
			// 중간 값일 경우(적절한 자리에 삽입)
			if(temp->scr >= score && temp->next->scr < score) {
				p->next = temp->next;
				temp->next = p;
				flag = 1;
				break;
			}
			else temp = temp->next;
		}
		if (flag==0) temp->next = p; // 가장 작은 값일 경우
	}


	//4. txt 파일에 쓰기
	writeRankFile(head);
}

void writeRankFile(struct node* x){
	// 추가된 랭킹 정보를 rank.txt에 기록
	//1. "rank.txt" 연다
	FILE *fp = fopen("rank.txt", "wt");

	//2. 랭킹 정보들의 수를 "rank.txt"에 기록
	fprintf(fp, "%d\n", score_number);

	//3. 노드를 탐색하면서 "rank.txt"에 출력
	while(x->next != NULL){
		fprintf(fp, "%s %d\n", x->player, x->scr);
		x = x->next;
	}
	//4. 맨 마지막 노드 출력
	fprintf(fp, "%s %d\n", x->player, x->scr);
}

///////////////////////// 3주차 실습에서 구현해야 할 함수 /////////////////////////
void DrawRecommend(int y, int x, int blockID,int blockRotate){
	// user code
}

int recommend(RecNode *root){
	int max=0; // 미리 보이는 블럭의 추천 배치까지 고려했을 때 얻을 수 있는 최대 점수

	// user code

	return max;
}

void recommendedPlay(){
	// user code
}