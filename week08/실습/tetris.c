#include "tetris.h"

static struct sigaction act, oact;

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
		case MENU_RANK: rank(); break;
		case MENU_RECOMMEND: recommendedPlay(); break;
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
	//DrawShadow(blockY,blockX,nextBlock[0],blockRotate);
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
			score += 1;
		}
	}
	score = (score*score)*100;
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
	if(fp == NULL) {
		score_number = 0;
		fclose(fp);
		return;
	}

	//2. 파일에서 랭킹정보 읽어 LinkedList로 저장
	fscanf(fp, "%d", &score_number);

	struct node *p = NULL;
	p = (struct node *)malloc(sizeof(struct node));

	//3. 가장 첫번째 값 입력받기
	fscanf(fp, "%s", name); strcpy(p->player, name);
	fscanf(fp, "%d", &this_score); p->scr = this_score;
	p->next = NULL;
	head = p;

	//4. 첫번째 값 다음부터 입력받기
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
	//5. 파일닫기
	fclose(fp);
}

void rank(){ // rank 메뉴를 출력하고 점수 순으로 X부터~Y까지 출력함
	//1. 문자열 초기화
	int X=1, Y=score_number, ch, i, j;
	struct node *temp1 = head;
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
		printw("X: ");
		echo();
		scanw("%d", &X);
		printw("Y: ");
		scanw("%d", &Y);
		noecho();
		printw("        name        |        score        \n");
		printw("------------------------------------------\n");
		if(X > Y || score_number == 0 || X > score_number || Y > score_number)
			mvprintw(8,0,"search failure: no rank in the list\n");
		else { // X와 Y사이의 rank 출력
			int index = 1;
			while(1){
				if(index == X){
					while(index != Y+1){
						printw("%11s%17d\n", temp1->player, temp1->scr);
						temp1 = temp1->next; index++; }
					break; }
				temp1 = temp1->next;
				index++;
			}
		}
	}

	//4-2. 메뉴2: 문자열을 받아 저장된 이름과 비교하고 이름에 해당하는 리스트를 출력 (7주차 과제)
	else if ( ch == '2') {
		char str[NAMELEN+1];
		int check = 0;

		//1. 플레이어 이름을 입력받는다.
		printw("input the name: ");
		echo(); scanw("%s", &str); noecho();

		//2. 연결리스트를 탐색하면서 해당 플레이어 관련 정보 출력 
		printw("        name        |        score        \n");
		printw("------------------------------------------\n");
		temp1 = head;
		if(score_number == 0) {
			printw("\nsearch failure: no name in the list\n");
			getch();
			return;
		}
		while(temp1->next != NULL){
			if(strcmp(temp1->player, str)==0) {
				printw("%11s%17d\n", temp1->player, temp1->scr);
				check = 1;
			}
			temp1 = temp1->next;
		}
		if(strcmp(temp1->player, str)==0) {
			printw("%11s%17d\n", temp1->player, temp1->scr);
			check  = 1;
		}

		//3. 입력 받은 사용자 이름이 리스트에 없는 경우
		if(check == 0) printw("\nsearch failure: no name in the list\n");
	}

	//4-3. 메뉴3: rank번호를 입력받아 리스트에서 삭제 (7주차 과제)
	else if ( ch == '3') {
		int num; int index = 1;

		// 삭제할 랭킹 number를 입력받는다.
		printw("input the rank: ");
		echo(); scanw("%d", &num); noecho();

		// 입력받은 랭킹이 범위 밖의 랭킹이라 삭제가 불가능할 때
		if(num > score_number) printw("\nsearch failure: the rank not in the list\n");
		else { // 리스트를 탐색하면서 입력받은 순서의 랭킹을 삭제
			temp1 = head;
			if(num == 1) { // 가장 첫번째 원소를 삭제하는 경우
				head = temp1->next;
				free(temp1);
				score_number--;
			}
			else { // 첫번째가 아닌 원소를 삭제하는 경우
				while(1){
					if(index == num-1){
						struct node *temp2 = temp1->next;
						temp1->next = temp2->next;
						free(temp2);
						score_number--; // 랭킹 총 개수 감소
						break;
					}
					temp1 = temp1->next;
					index++;
				}
			}
			// 화면에 삭제 완료 메시지 출력
			printw("\nresult: the rank deleted\n");
			writeRankFile(head);
		}	
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
	if(score_number==1) head = p; // 현재 리스트가 비어있는 경우

	else if(score > head->scr) { // 가장 큰 값일 경우
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
	if(score_number != 0) fprintf(fp, "%d\n", score_number);

	// 3-1. 빈 노드인 경우 아무것도 출력하지 않고 종료한다.
	if(score_number == 0) {
		fclose(fp);
		return;
	}

	//3-2. 빈 노드가 아닌 경우 리스트를 탐색하면서 "rank.txt"에 출력
	while(x->next != NULL){
		fprintf(fp, "%s %d\n", x->player, x->scr);
		x = x->next; }
	
	//4. 맨 마지막 노드 출력
	fprintf(fp, "%s %d\n", x->player, x->scr);
	fclose(fp);
}

///////////////////////// 3주차 실습에서 구현해야 할 함수 /////////////////////////
void DrawRecommend(int y, int x, int blockID, int blockRotate){
	
}

int recommend(RecNode *root){
	// 1. 추천 시스템을 위한 트리 생성
	char this_field[HEIGHT][WIDTH];
	for(int j=0; j<HEIGHT; j++)
		for(int i=0; i<WIDTH; i++)
			this_field[j][i] = root->recField[j][i];

	int max = 0; // 미리 보이는 블럭의 추천 배치까지 고려했을 때 얻을 수 있는 최대 점수
	int score = root->acm_score; // 현재 누적 점수
	int rotation; // 회전 가능 횟수
	int blockY = -1; // 초기 Y 좌표
	int blockX = WIDTH/2 -2; // 초기 X 좌표
	int index = 0;

	// level = 첫번째 0, 두번째 1, 세번째 2
	switch(nextBlock[root->level]){ // [0](현재 블럭), [1](다음 블럭), [2](다다음 블럭)
	case 0: 
	case 5: 
	case 6: rotation = 2; break;
	case 1: 
	case 2: 
	case 3: rotation = 4; break;
	case 4: rotation = 1; break;
	default: break;
	}

	for (int i=0; i<rotation; i++){
		blockX = WIDTH/2 -2;
		while(CheckToMove(this_field, nextBlock[root->level], i, blockY, blockX-1)==1) blockX--; // 가장 왼쪽으로 블럭이동
		do {
			while (CheckToMove(this_field, nextBlock[root->level], i, blockY+1, blockX)==1) blockY++; // 가장 아래로 블럭이동
			score += AddBlockToField(this_field, nextBlock[root->level], i, blockY, blockX); // 필드 정보를 저장한 변수에 블럭정보 기입
			score += DeleteLine(this_field); // 필드 정보를 저장한 변수에 라인제거 정보 기입
			if(score >= max) {
				if(VISIBLE_BLOCKS == 1){
					if(score == max){
						if(rec_y < blockY){
							rec_x = blockX;
							rec_y = blockY;
							rec_rotation = i; }
					}
					else {
						rec_x = blockX;
						rec_y = blockY;
						rec_rotation = i;
					}
				}
				max = score;
			}
			struct RecNode *p; // 새로운 노드 생성
			p = (struct RecNode *)malloc(sizeof(struct RecNode));

			p->level = root->level + 1;
			p->acm_score = score;
			p->recField = this_field;
			root->child[index] = p; index ++; // 트리에 노드 삽입
			if(p->level<VISIBLE_BLOCKS) {
				if(recommend(p) >= max) {
					if(recommend(p) == max){
						if(root->level == 0 && rec_y < blockY){
							rec_x = blockX;
							rec_y = blockY;
							rec_rotation = i; } }
					else {
						max = recommend(p);
						if(root->level == 0){
							rec_x = blockX;
							rec_y = blockY;
							rec_rotation = i; } }
				}
			}
			blockX++; blockY = -1; score = root->acm_score;

			for(int j=0; j<HEIGHT; j++)
				for(int i=0; i<WIDTH; i++)
					this_field[j][i] = root->recField[j][i];

		} while(CheckToMove(this_field, nextBlock[root->level], i, blockY, blockX)==1);
	}

	return max;
}

// recommended_play 모드 실행을 위한 함수들
void DropToRecommended(int sig){
	timed_out = 0;
	int max = 0;

	struct RecNode *recRoot; // 전체 tree의 root노드
	recRoot = (struct RecNode *)malloc(sizeof(struct RecNode));

	recRoot->level = 0;
	recRoot->acm_score = 0;
	recRoot->recField = field;

	DrawBlock(blockY,blockX,nextBlock[0],blockRotate,' ');
	DrawShadow(blockY,blockX,nextBlock[0],blockRotate);
	max = recommend(recRoot); //printw(" %d", max);
	DrawBlock(rec_y,rec_x,nextBlock[0],rec_rotation,'R');

	if(rec_flag == 1){
		score += AddBlockToField(field,nextBlock[0],rec_rotation,rec_y,rec_x);
		if (rec_y == -1) gameOver = 1;
		score += DeleteLine(field);
		PrintScore(score);

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

		// rec_flag 값 초기화
		rec_flag = 0;
		return;
	}


	// rec_flag 값 set
	if(rec_flag == 0) rec_flag = 1;
}

void recommendedPlay(){
	int command;
	clear();

	act.sa_handler = DropToRecommended;
	sigaction(SIGALRM,&act,&oact);
	InitTetris();
	do{
		if(timed_out==0){
			//alarm(1);
			ualarm(500000,0);
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