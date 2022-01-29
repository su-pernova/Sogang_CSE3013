#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define MIN(a,b) ((a<b)? a:b)

int WIDTH, HEIGHT; //미로 전체의 가로, 세로 길이

typedef struct cell{
    int set_num; //소속된 집합 번호값
    //해당 방향으로 1 : 벽 있음, 0 : 벽 없음
    int up; int down;
    int left; int right;   
} cell;

int main(){
    // 1. WIDTH와 HEIGHT값을 입력받는다
    printf("WIDTH : ");
    scanf("%d", &WIDTH);
    printf("HEIGHT : ");
    scanf("%d", &HEIGHT); 

    // 2. WIDTH * HEIGHT 크기의 2차원 배열 생성
    int set_idx = 1;
    struct cell **maze;
    maze = (struct cell**)malloc(sizeof(struct cell*)*HEIGHT);
    for(int i=0; i<HEIGHT; i++){
        maze[i] = (struct cell*)malloc(sizeof(struct cell)*WIDTH);
        // 미로의 벽과 집합 값 초기화
        for(int j=0; j<WIDTH; j++){
            maze[i][j].set_num = set_idx++;
            maze[i][j].up = 1;
            maze[i][j].down = 1;
            maze[i][j].left = 1;
            maze[i][j].right = 1;
        }
    }

    // 3. 무작위의 미로를 생성한다
    int rand_num, current_set, new_set_flag = 0;
    for(int i=0; i<HEIGHT-1; i++){
        // 1) 오른쪽 벽을 남겨둘지 제거할지 임의로 선택
        // rand_num : 짝수면 벽을 지우고 아니면 벽을 남겨둔다
        for(int j=0; j<WIDTH-1; j++){
            if(maze[i][j].set_num != maze[i][j+1].set_num){
                srand(time(NULL));
                rand_num = rand()+i+j+2;
                if(rand_num % 3 == 0) {
                    if(maze[i][j].set_num < maze[i][j+1].set_num){
                        for(int k=0; k<HEIGHT; k++){
                            for(int l=0; l<WIDTH; l++){
                                if(maze[k][l].set_num == maze[i][j+1].set_num)
                                    maze[k][l].set_num = maze[i][j].set_num;
                            }
                        }
                    }
                    else if(maze[i][j].set_num > maze[i][j+1].set_num){
                        for(int k=0; k<HEIGHT; k++){
                            for(int l=0; l<WIDTH; l++){
                                if(maze[k][l].set_num == maze[i][j].set_num)
                                    maze[k][l].set_num = maze[i][j+1].set_num;
                            }
                        }
                    }
                    maze[i][j+1].left = 0;
                }
            }
        }

        // 2) 하단의 벽을 남겨둘지 제거할지 임의로 선택
        // rand_num : 3의 배수이면 벽을 지우고 아니면 벽을 남겨둔다
        // 각 집합마다 하나 이상이 선택되어야 함
        current_set = maze[i][0].set_num;
        for(int j=0; j<WIDTH; j++){
            srand(time(NULL));
            rand_num = rand()+i+j+3;
            if(rand_num % 2 == 0){
                new_set_flag = 1;
                maze[i+1][j].up = 0;
                maze[i+1][j].set_num = maze[i][j].set_num;
            }
            if((maze[i][j+1].set_num) != current_set){
                // 하나도 선택되지 않은 경우, 집합의 마지막 칸은 무조건 하단의 벽을 지워야 함
                if(new_set_flag == 0){
                    maze[i+1][j].up = 0;
                    maze[i+1][j].set_num = maze[i][j].set_num;
                    current_set = maze[i][j+1].set_num;
                }
                else {
                    current_set = maze[i][j+1].set_num;
                    new_set_flag = 0;
                }
            }
        }
    }

    // 마지막 줄 처리
    for(int j=0; j<WIDTH-1; j++){
        if(maze[HEIGHT-1][j].set_num != maze[HEIGHT-1][j+1].set_num){
            if(maze[HEIGHT-1][j+1].set_num < maze[HEIGHT-1][j].set_num){
                for(int k=0; k<HEIGHT; k++){
                    for(int l=0; l<WIDTH; l++){
                        if(maze[k][l].set_num == maze[HEIGHT-1][j].set_num)
                            maze[k][l].set_num = maze[HEIGHT-1][j+1].set_num;
                    }
                }
            }
            else if(maze[HEIGHT-1][j+1].set_num > maze[HEIGHT-1][j].set_num){
                for(int k=0; k<HEIGHT; k++){
                    for(int l=0; l<WIDTH; l++){
                        if(maze[k][l].set_num == maze[HEIGHT-1][j+1].set_num)
                            maze[k][l].set_num = maze[HEIGHT-1][j].set_num;
                    }
                }
            }
            maze[HEIGHT-1][j+1].left = 0;
        }
    }

    // 4. 저장된 미로 정보에 의거하여 미로 출력
    FILE *out;
    out = fopen("maze.maz", "wt");
    for(int i=0; i<HEIGHT; i++){
        // 미로 셀의 상단 벽 유무에 따른 출력
        for(int j=0; j<WIDTH; j++){
            if(maze[i][j].up == 1) fprintf(out, "+-");
            else fprintf(out, "+ ");
            if(j==WIDTH-1) fprintf(out, "+");
        } fprintf(out, "\n");
        // 미로 셀의 왼쪽 벽 유무에 따른 출력
        for(int j=0; j<WIDTH; j++){
            if(maze[i][j].left == 1) fprintf(out, "| ");
            else fprintf(out, "  ");
            //if(maze[i][j].left == 1) printf("|%d", maze[i][j].set_num);
            //else printf(" %d", maze[i][j].set_num);
            if(j==WIDTH-1) fprintf(out, "|\n");
        }
        // 마지막 행의 하단 벽 테두리 출력
        if(i==HEIGHT-1){
            for(int j=0; j<WIDTH; j++){
                fprintf(out, "+-");
                if(j==WIDTH-1) fprintf(out, "+\n");
            }
        }
    }

    printf("maze.maz created.\n");
    fclose(out);
    return 0;
}