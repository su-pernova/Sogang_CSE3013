#include "tetris.h"

static struct sigaction act, oact;

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
	int i,j;

	for(j=0;j<HEIGHT;j++)
		for(i=0;i<WIDTH;i++)
			field[j][i]=0;

	nextBlock[0]=rand()%7;
	nextBlock[1]=rand()%7;

	blockRotate=0;
	blockY=-1;
	blockX=WIDTH/2-2;
	score=0;	
	gameOver=0;
	timed_out=0;

	DrawOutline();
	DrawField();
	DrawBlock(blockY,blockX,nextBlock[0],blockRotate,' ');
	DrawNextBlock(nextBlock);
	PrintScore(score);
}

void DrawOutline(){	
	int i,j;
	/* 블럭이 떨어지는 공간의 테두리를 그린다 */
	DrawBox(0,0,HEIGHT,WIDTH);

	/* next block을 보여주는 공간의 테두리를 그린다 */
	/* move = ncurses lib 함수. 커서를 (y,x)로 이동시킨다 */
	move(2,WIDTH+10);
	printw("NEXT BLOCK");
	DrawBox(3,WIDTH+10,4,8);

	/* score를 보여주는 공간의 테두리를 그린다 */
	move(9,WIDTH+10);
	printw("SCORE");
	DrawBox(10,WIDTH+10,1,8);
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
}


void PrintScore(int score){
	move(11,WIDTH+11);
	printw("%8d",score);
}

void DrawNextBlock(int *nextBlock){
	int i, j;
	for( i = 0; i < 4; i++ ){
		move(4+i,WIDTH+13);
		for( j = 0; j < 4; j++ ){
			if( block[nextBlock[1]][0][i][j] == 1 ){
				attron(A_REVERSE);
				printw(" ");
				attroff(A_REVERSE);
			}
			else printw(" ");
		}
	}
}

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

///////////////////////// 첫주차 실습에서 구현해야 할 함수 /////////////////////////

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
	// 2) 이전 블록 정보를 지운다. DrawBlock함수 참조할 것
	for (int i = 0; i < BLOCK_HEIGHT; i++){
		for (int j = 0; j < BLOCK_WIDTH; j++){
			if (block[currentBlock][tempR][i][j] == 1 && tempY+i >= 0){
				move(i+tempY+1, j+tempX+1); // (DrawBlock에서 가져온 것) 왜 +1을 해주지? 
				printw(".");
			}
		}
	}
	// 3) 새로운 블록 정보를 그린다.
	DrawBlock(blockY, blockX, currentBlock, blockRotate, ' ');
}

void BlockDown(int sig){	
	// 강의자료 p26-27의 flowchart 참고
	// 매개변수 int sig 는 신호를 받는 것이므로 함수 내부에서 사용하지 않아도 됨
	timed_out = 0;
	
	if (CheckToMove(field, nextBlock[0], blockRotate, blockY+1, blockX)==1){
		DrawChange(field, KEY_DOWN, nextBlock[0], blockRotate, ++blockY, blockX);
		return;
	}
	else {
		if (blockY == -1) gameOver = 1;
		AddBlockToField(field,nextBlock[0],blockRotate,blockY,blockX);
		score += 10; // 블럭을 필드에 쌓을때마다 +10점
	}
	
	score += DeleteLine(field); // 한 줄을 가득 채워 없애는 경우 +100점
	PrintScore(score);

	// 현재 블럭을 다음 블럭으로 바꾸고, 다음 블럭 업데이트
	nextBlock[0] = nextBlock[1];
	nextBlock[1]=rand()%7;
	DrawNextBlock(nextBlock);

	// 현재 블록의 위치를 초기화
	blockX = WIDTH/2 -2;
	blockY = -1;
	blockRotate = 0;
	DrawField();

}

void AddBlockToField(char f[HEIGHT][WIDTH],int currentBlock,int blockRotate,int blockY,int blockX){
	// Block이 추가된 영역의 필드값을 바꾼다.
	int i, j;
	for (i = 0; i < BLOCK_HEIGHT; i++){
		for (j = 0; j < BLOCK_WIDTH; j++){
			if (block[currentBlock][blockRotate][i][j] == 1)
				f[blockY + i][blockX + j] = 1;
		}
	}
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

///////////////////////////////////////////////////////////////////////////

void DrawShadow(int y, int x, int blockID,int blockRotate){
	// user code
}

void createRankList(){
	// user code
}

void rank(){
	// user code
}

void writeRankFile(){
	// user code
}

void newRank(int score){
	// user code
}

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
