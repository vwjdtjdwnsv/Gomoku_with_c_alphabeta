//
//  AI.h
//  6AI_Ommmookkk
//
//  Created by 정성준 on 2020/04/15.
//  Copyright © 2020 정성준. All rights reserved.
//

#ifndef AI_h
#define AI_h
#define _CRT_SECURE_NO_WARNINGS    // strcpy 보안 경고로 인한 컴파일 에러 방지
#include <stdio.h>
#include <string.h>    // strcpy 함수가 선언된 헤더 파일
#include <stdlib.h>    // malloc, free 함수가 선언된 헤더 파일

#define MIN(X,Y) ((X) < (Y) ? (X) : (Y))
#define MAX(X,Y) ((X) > (Y) ? (X) : (Y))

const int N = 19; // 19 * 19 오목
char board[N][N]; //보드판 선언
int stage = 0; //진행한 stage, 얘를 쓸까?
int timelimit = 10;//시간제한기본10초로 두자

int depthLimit = 3;


int player; //플레이어 선택 1or2
int ai;
char player_color;//색, B or W
char ai_color;

int now=1;//어차피 1이 먼저 시작하니까.
int win=1;//종료를 win이 0이 되는 걸로 하자.

clock_t start;//시간 측정을 위해

int x,y;//좌표 입력변수

struct coordinate{
    int x;
    int y;
};

struct action{
    struct coordinate move;
    int value;
};

struct coordinate status[361];
//현재 돌이 놓여 있는 좌표 상태, 그 돌들의 색은 중요한가? 모르겠다.


bool isEmpty(int x,int y);
bool isValid(int x,int y);
bool isThree(int x,int y);//33확인함수
bool isLegal(int x,int y);


void setDol(int x,int y,char color);//돌을 두는 함수

int checkRow(int x,int y);
int checkColumn(int x,int y);
int checkDig(int x,int y);
int checkRdig(int x,int y);

int closeRow(int x,int y);
int closeColumn(int x,int y);
int closeDig(int x,int y);
int closeRdig(int x,int y);

bool threeflag;




//AI_search
struct coordinate getNextMove(void);//보드, 현재 좌표정보 그냥 call by ref로 전달됨!이 아니라 어차피 전역이다 백트랙킹을 쓸 수밖에 없다.
//다음 좌표를 구한다.
struct coordinate alphaBetaSearch(void);

int MaxValue(int alpha,int beta,int depth);
int MinValue(int alpha,int beta,int depth);

void DeepSearch(int x,int y,char color);
void Backtracking(void);//search를 위해


int Utility(int x, int y,bool turn);//terminate시 평가 노드, 지금 둔 노드의 값어치,그 때 둔 사람이 누구인지.

bool isOverlap(int index,int x,int y,struct action actions[]){
    for (int i=0; i<index+1; i++) {
        if (actions[i].move.x==x && actions[i].move.y==y)
            return true;
    }
    return false;
}//중복 검사하는 함수

struct action *getActions(char color, bool MaxTurn){//MAX면 v=-10000으로 초기값 설정
    
    struct action *posActions = malloc(8 * stage * sizeof(struct action));    // 구조체 포인터에 동적 메모리 할당;
    
    char c=color;//액션에서 둘 돌의 색
    int index=0;//action을 넣기 위한 index
    int init_value;//action의 초기값
    if(MaxTurn)
        init_value = -300000;
    else
        init_value = 300000;
    //Maxturn일 경우 마이너스 무한대, 아닐 경우 양의 무한대
    for (int i=0; i<stage; i++) {
//        printf("\n 현재는 %d 스테이지입니다.\n\n", stage);
        
        int x=status[i].x;
        int y=status[i].y;
        
        //8방향에 대해 각각 유효 위치인지, 33은 아닌지 파악, 가능한 좌표라면 저장
        //위 유효엑션확인,맞으면 action 저장, value 초기화
        if(isLegal(x-1, y)){
            setDol(x-1, y, c);
            
            if(!isThree(x-1, y)){
                while (!isOverlap(index, x-1, y, posActions)) {
                    posActions[index].move.x = x-1;
                    posActions[index].move.y = y;
                    posActions[index].value = init_value;
                    index++;
                }
            }
            
            board[x-1][y]='+';
        }
        
        //아래 유효액션확인
        if(isLegal(x+1, y)){
            setDol(x+1, y, c);
            if(!isThree(x+1, y)){
                while (!isOverlap(index, x+1, y, posActions)) {
                    posActions[index].move.x = x+1;
                    posActions[index].move.y = y;
                    posActions[index].value = init_value;
                    index++;
                }
            }
            board[x+1][y]='+';
        }
        
        //왼쪽 유효액션확인
        if(isLegal(x, y-1)){
            setDol(x, y-1, c);
            if(!isThree(x, y-1)&&!isOverlap(index, x, y-1, posActions)){
                posActions[index].move.x = x;
                posActions[index].move.y = y-1;
                posActions[index].value = init_value;
                index++;
            }
            board[x][y-1]='+';
        }
        //오른 유효액션확인
        if(isLegal(x, y+1)){
            setDol(x, y+1, c);
            if(!isThree(x, y+1)&&!isOverlap(index, x, y+1, posActions)){
                posActions[index].move.x = x;
                posActions[index].move.y = y+1;
                posActions[index].value = init_value;
                index++;
            }
            board[x][y+1]='+';
        }
        
        
        //동북 유효액션확인
        if(isLegal(x-1, y+1)){
            setDol(x-1, y+1, c);
            if(!isThree(x-1, y+1)&&!isOverlap(index, x-1, y+1, posActions)){
                posActions[index].move.x = x-1;
                posActions[index].move.y = y+1;
                posActions[index].value = init_value;
                index++;
            }
            board[x-1][y+1]='+';
        }
        
        //서북 유효액션확인
        if(isLegal(x-1, y-1)){
            setDol(x-1, y-1, c);
            if(!isThree(x-1, y-1)&&!isOverlap(index, x-1, y-1, posActions)){
                posActions[index].move.x = x-1;
                posActions[index].move.y = y-1;
                posActions[index].value = init_value;
                index++;
            }
            board[x-1][y-1]='+';
        }
        
        //서남 유효액션확인
        if(isLegal(x+1, y-1)){
            setDol(x+1, y-1, c);
            if(!isThree(x+1, y-1)&&!isOverlap(index, x+1, y-1, posActions)){
                posActions[index].move.x = x+1;
                posActions[index].move.y = y-1;
                posActions[index].value = init_value;
                index++;
            }
            board[x+1][y-1]='+';
        }
        //동남 유효액션확인
        if(isLegal(x+1, y+1)){
            setDol(x+1, y+1, c);
            if(!isThree(x+1, y+1)&&!isOverlap(index, x+1, y+1, posActions)){
                posActions[index].move.x = x+1;
                posActions[index].move.y = y+1;
                posActions[index].value = init_value;
                index++;
            }
            board[x+1][y+1]='+';
        }
    }//가능한 액션 모두 정리,
//    printf("index : %d \n",index);
    posActions[index].move.x = -1;
    posActions[index].move.y = -1;//종료 지점 알기 위해 표시
    return posActions;
}
#endif /* AI_h */
