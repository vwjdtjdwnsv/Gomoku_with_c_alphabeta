//
//  main.c
//  6AI_Ommmookkk
//
//  Created by 정성준 on 2020/04/9.
//  Copyright © 2020 정성준. All rights reserved.
//

#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include "header.h"
#include <math.h>//utility 위해서

//게임판 관련
//보드판 초기화 함수

void initBoard(){
    for (int i = 0; i < N; i++){
        for (int j = 0; j < N; j++){
            board[i][j] = '+';
        }
    }
    stage = 0;
}

//보드판 출력
void printBoard(){
    printf("   ");
    for (int i = 0 ; i < N; i++){
        printf("%d",i);
        if (i<10) {
            printf(" ");
        }
        printf(" ");
    }
    printf("\n");
    //여기까지 윗줄 열에 숫자 표시, 두자리 숫자는 한번만 띄어쓰기
    for (int i = 0; i < N; i++){
        printf("%d",i);
        if (i<10) {
            printf(" ");
        }
        printf(" ");
        for (int j = 0; j < N; j++){
            printf("%c  ",board[i][j]);
        }
        printf("\n\n");
    }
    //한줄씩 출력하고 개행 두번 내려옴
}


//둘 수 있는지 확인해보자 legal
//비었는지 확인

bool isEmpty(int x, int y){
    char c = board[x][y];
    if(c == '+')
        return true;
    else
        return false;
}

//유효한 좌표인지 확인
bool isValid(int x, int y){
    if((0<=x)&&( x<=18)&&(0<=y)&&(y<=18))
        return true;
    else
        return false;
}
//결과적으로 둘 수 있는 좌표인지 판단
bool isLegal(int x, int y){
    if (isEmpty(x, y)&&isValid(x, y)) {
        return true;
    }
    return false;
}

//33확인
bool isThree(int x, int y){
    int row = ((checkRow(x,y)==3)&&(closeRow(x,y)==2)) ? 1 : 0;
    int column = ((checkColumn(x,y)==3)&&(closeColumn(x,y)==2)) ? 1 : 0;
    int Rdig = ((checkRdig(x,y)==3)&&(closeRdig(x,y)==2)) ? 1 : 0;
    int Dig = ((checkDig(x,y)==3)&&(closeDig(x,y)==2)) ? 1 : 0;
        
    if(row+column+Rdig+Dig >= 2){
        return true;
    }
    return false;
}

//돌을 두는 함수
void setDol(int x, int y, char color){
    board[x][y]=color;
}


//----------------------------------평가함수---------
//열려 있는지를 확인, 양 끝이 열려 있으면 2 반환, 하나만 열려 있다면 1 반환, 다 막혔다면 0 반환
//열린 가로 확인
int closeRow(int x, int y){
    int i = 1;
    int close = 0;

    while ((y-i >= 0) && (board[x][y-i] == board[x][y]) )
        i++;
    //갈 수 있는데, 같은 색의 돌이면 계속 간다.
    //종료조건, 갈 수 없거나, 같은 색이 아니거나, 둘다...는 고려할 필요 없다.
    if(board[x][y-i]=='+')//근데 그 지점이 공백이냐? 그렇다면 열렸다. 막혔다면, 어차피 쓰레기값일테니... 신경쓰지말고
        close++;
     
    i=1;
    while ((y+i <= N-1) && (board[x][y+i] == board[x][y]))
        i++;
    if (board[x][y+i]=='+')
        close++;
    
    return close;
}

//열린세로확인
int closeColumn(int x, int y){
    int i = 1;
    int close = 0;

    while ((x-i >= 0) && (board[x-i][y] == board[x][y]) )
        i++;
    if(board[x-i][y]=='+')
        close++;
    
    i = 1;
    while ((x+i <= N-1) && (board[x+i][y] == board[x][y]))
        i++;
    if (board[x+i][y]=='+')
        close++;
    
    return close;
}

//열린'\'대각선확인
int closeRdig(int x, int y){
    int i = 1;
    int close = 0;

    while ((x+i <= N-1 && y+i <= N-1) && (board[x+i][y+i] == board[x][y]) )
        i++;
    if(board[x+i][y+i]=='+')
        close++;
     
    i=1;
    while ((x-i >= 0 && y-i >= 0) && (board[x-i][y-i] == board[x][y]) )
        i++;
    if(board[x-i][y-i]=='+')
        close++;
     
    return close;
}

//열린'/'대각선확인
int closeDig(int x, int y){
    int i = 1;
    int close = 0;

    while ((x-i >= 0 && y+i <= N-1) && (board[x-i][y+i] == board[x][y]) )
        i++;
    if(board[x-i][y+i]=='+')
        close++;
     
    i=1;
    while ((x+i >= 0 && y-i >= 0) && (board[x+i][y-i] == board[x][y]) )
        i++;
    if(board[x+i][y-i]=='+')
        close++;
     
    return close;
}



//가로이웃 점수 확인
int checkRow(int x, int y){
    int value = 1;
    int i = 1;
    while ((y-i >= 0) && (board[x][y-i] == board[x][y]) ){
        value++;
        i++;
    }//왼쪽 얼마나 많은 이웃돌이 있는지
    
    i = 1;
    while ((y+i <= N-1) && (board[x][y+i] == board[x][y]) ){
        value++;
        i++;
    }//오른쪽 얼마나 많은 이웃돌이 있는지
    return value;
}
//세로
int checkColumn(int x, int y){
    int value = 1;
    int i = 1;
    
    while ((x-i >= 0) && (board[x-i][y] == board[x][y]) ){
        value++;
        i++;
    }//위에 얼마나 많은 이웃돌이 있는지
    i = 1;
    while ((x+i <= N-1) && (board[x+i][y] == board[x][y]) ){
        value++;
        i++;
    }//아래에 얼마나 많은 이웃돌이 있는지
    return value;
}
//역대각선 '\'방향
int checkRdig(int x, int y){
    int value = 1;
    int i = 1;
    
    while ((x+i <= N-1 && y+i <= N-1) && (board[x+i][y+i] == board[x][y]) ){
        value++;
        i++;
    }//동남에 얼마나 많은 이웃돌이 있는지
    i = 1;
    while ((x-i >= 0 && y-i >= 0) && (board[x-i][y-i] == board[x][y]) ){
        value++;
        i++;
    }//서북에 얼마나 많은 이웃돌이 있는지
    return value;
}
//대각선 '/'방향
int checkDig(int x, int y){
    int value = 1;
    int i = 1;
    
    while ((x+i <= N-1 && y-i >= 0) && (board[x+i][y-i] == board[x][y])){
        value++;
        i++;
    }//서남에 얼마나 많은 이웃돌이 있는지
    i = 1;
    while ((x-i >= 0 && y+i <= N-1) && (board[x-i][y+i] == board[x][y]) ){
        value++;
        i++;
    }//동북에 얼마나 많은 이웃돌이 있는지
    return value;
}

//승부가 결정났는지
bool isFinish(int x,int y){
    if(board[x][y]!='+')
        return (checkRow(x,y)>=5 || checkColumn(x,y)>=5 || checkDig(x,y)>=5 || checkRdig(x,y)>=5);
    else
        return false;
}


//----------------------AI 관련함수-------------------------------------------------------------------------------------

void DeepSearch(int x,int y,char color){
    status[stage].x=x;
    status[stage].y=y;
    setDol(x, y, color);
    stage++;
//    printf("deep \n");
//    printBoard();
};//깊이를 하나 내려가는 것

void Backtracking(void){
    int x = status[stage-1].x;
    int y = status[stage-1].y;
    board[x][y]='+';
    status[stage-1].x = -1;
    status[stage-1].y = -1;
    stage--;
//    printf("back \n");
//    printBoard();
};//깊이 하나 제거, 다시 올라오기

struct coordinate getNextMove(){
    struct coordinate nextMove;//여기에 알파베타 적용!!!!!!!!!!!!!!!!!!!
    return nextMove=alphaBetaSearch();
}

struct coordinate alphaBetaSearch(){
    int alpha = -300000;
    int beta = 300000;
    int depth = 0;//현재 깊이
    return MaxValue(alpha, beta, depth+1).move;
    //다 돌았다면, 최대값 찾기,그리고 반환
}


struct action MaxValue(int alpha,int beta,int depth){//current를 알고 있다.
    struct action *MaxActions = getActions(ai_color, true);//ai가 둘 수 있는 수,value -무한 초기화
    struct action returnAction;
    returnAction.value = -300000;//반환할 값 초기화
    for(int i=0; MaxActions[i].move.x!=-1; i++) {
        DeepSearch(MaxActions[i].move.x, MaxActions[i].move.y ,ai_color);//일단 ai deep해보고
        printf("max x : %d, y : %d \n",MaxActions[i].move.x,MaxActions[i].move.y);
        if ((depth==depthLimit)||isFinish(x, y)){
            MaxActions[i].value = Utility(MaxActions[i].move.x, MaxActions[i].move.y,true);
            printf("max 유틸리티 : %d \n \n", Utility(MaxActions[i].move.x, MaxActions[i].move.y,true));
        }
        //최대깊이면, 바로 value에 utility값 넣어주고
        else
            MaxActions[i].value = MAX(MaxActions[i].value, MinValue(alpha, beta,depth+1).value);
        //아니라면 Min호출,반환한 action의 value값 넣어준다.
        alpha = MAX(alpha, MaxActions[i].value); //alpha = MAX(alpha,value)
        if (MaxActions[i].value >= beta){
            Backtracking();
            return MaxActions[i];//만약 beta보다 큰 값 발견했다면, 더 이상 탐색할 필요 없다. 어차피 선택 안될테니까.
        }
        returnAction = (returnAction.value > MaxActions[i].value) ? returnAction : MaxActions[i];//최대값 action 반환
        Backtracking();//다시 돌아와서,반복문 돌기
    }//반복 끝, 반환
    void free(void* MaxActions);
    return returnAction;
}

struct action MinValue(int alpha,int beta, int depth){
    struct action *MinActions = getActions(player_color, false);
    struct action returnAction;
    returnAction.value = 300000;//반환할 값 초기화
    for(int i=0; MinActions[i].move.x!=-1; i++) {
        DeepSearch(MinActions[i].move.x, MinActions[i].move.y,player_color);
        printf("min x : %d, y : %d \n",MinActions[i].move.x,MinActions[i].move.y);
        if ((depth==depthLimit)||isFinish(x, y)){
            MinActions[i].value = Utility(MinActions[i].move.x, MinActions[i].move.y,false);
            printf("min 유틸리티 : %d \n \n", Utility(MinActions[i].move.x, MinActions[i].move.y,false));
        }
        else
            MinActions[i].value = MIN(MinActions[i].value, MaxValue(alpha, beta,depth+1).value);
        beta = MIN(beta,MinActions[i].value); //beta = MIN(beta,value)
        if (MinActions[i].value <= alpha){
            Backtracking();
            return MinActions[i];
        }
        returnAction = (returnAction.value < MinActions[i].value) ? returnAction : MinActions[i];//최대값 action 반환
        Backtracking();//다시 돌아와서,반복문 돌
    }
    void free(void* MinActions);
    return returnAction;
}


int Utility(int x, int y,bool turn){
    int score = 0;
    if (isFinish(x, y)) {
        score+=30000;
    }
    //승리
    if(checkRow(x, y)==4&&closeRow(x, y)==2){
        score+=10000;
    }
    if(checkColumn(x, y)==4&&closeColumn(x, y)==2){
        score+=10000;
    }
    if(checkRdig(x, y)==4&&closeRdig(x, y)==2){
        score+=10000;
    }
    if(checkDig(x, y)==4&&closeDig(x, y)==2){
        score+=10000;
    }
    //다음 턴 무조건 승리
    
    if(checkRow(x, y)==4&&closeRow(x, y)==1){
        score+=4000;
    }
    if(checkColumn(x, y)==4&&closeColumn(x, y)==1){
        score+=4000;
    }
    if(checkRdig(x, y)==4&&closeRdig(x, y)==1){
        score+=4000;
    }
    if(checkDig(x, y)==4&&closeDig(x, y)==1){
        score+=4000;
    }
    //상대가 눈치 못 채면 승리 방어 여부 존재
    
    if(checkRow(x, y)==3 && closeRow(x, y)==2){
        score+=8000;
    }
    if(checkColumn(x, y)==3 && closeColumn(x, y)==2){
        score+=8000;
    }
    if(checkRdig(x, y)==3 && closeRdig(x, y)==2){
        score+=8000;
    }
    if(checkDig(x, y)==3 && closeDig(x, y)==2){
        score+=8000;
    }
    //가장 적극적으로 만들어야될 열린 33
    //단방향 4보다 더 적극적으로 만들어야함
    if(checkRow(x, y)==2&&closeRow(x, y)==2){
        score+=500;
    }
    if(checkColumn(x, y)==2&&closeColumn(x, y)==2){
        score+=500;
    }
    if(checkRdig(x, y)==2&&closeRdig(x, y)==2){
        score+=500;
    }
    if(checkDig(x, y)==2&&closeDig(x, y)==2){
        score+=500;
    }
    //열린 2, 언제든지 열린 3으로 발전 가능, 하지만 돌이 너무 뭉친다... 그게 더 좋은건가?적당히 뭉쳤으면
    else{
        score+=5;
    }
        
    //그 외 단방향 3, 단방향 2,,, 닫힌 경우는 뭘 해도 어지간하면 의미 없음
    if (turn)//AI차례
        return score;
    else
        return score*(-1);
}





//좌표를 입력받아서 가능한 액션을 뱉는 함수
//call by reference, call by v 신경쓰면서 함수 선언합시다

//게임 시작
int main() {
    
    initBoard();
    printf("흑으로 플레이하려면 1, 백으로 플레이하려면 2를 누르세요, 흑이 먼저 둡니다. \n");
    scanf("%d",&player);
    ai=3-player;//ai는 자동으로 반대편
    //흑백 선택, 1과2를 제외한 input오류는 고려하지 않겠습니다~
    if (player==1){
        player_color='B';
        ai_color='W';
    }
    else{
        player_color='W';
        ai_color='B';
    }
    //색 배정
    
    printBoard();
    
    
    //게임 진행
    while (win){
        //플레이어 차례
        if (now == player){//1부터 시작해서 2, 1 계속 반복, nowplaying을 now로 하자.
            threeflag = false;//33판단 flag 꺼두기
            printf("플레이어 %c 차례입니다.\n",player_color);
            start = clock();//실행시간 시작
            
            printf("x좌표를 입력하세요 \n");
            scanf("%d", &x);
            printf("y좌표를 입력하세요 \n");
            scanf("%d", &y);
            
            while (!threeflag) {
                threeflag=true;//
                while (!isLegal(x, y)) {//입력이 잘못됐다면
                    printf("유효한 위치가 아닙니다. 다시 입력하세요 \n");
                    printf("x좌표를 입력하세요 \n");
                    scanf("%d", &x);
                    printf("y좌표를 입력하세요 \n");
                    scanf("%d", &y);
                }
            //유효한 위치까지는 성공 이제 33 판단
                setDol(x, y, player_color);//일단 둬보자
                
                if (isThree(x, y)) {
                    board[x][y]='+';
                    printf("33 위치에 해당합니다. 다시 입력하세요 \n");
                    printf("x좌표를 입력하세요 \n");
                    scanf("%d", &x);
                    printf("y좌표를 입력하세요 \n");
                    scanf("%d", &y);
                    threeflag=false;
                }
            }
            //시간 조건 확인 일단 플레이어는 시간제한 없도록 하자
            if( (clock()-start)/CLOCKS_PER_SEC > timelimit ){
                printf("제한시간 초과, 플레이어 패배 쿠쿠루삥빵뽕뿡 \n");
                win = 0;
                break;
            }
            status[stage].x=x;
            status[stage].y=y;
//            printf("\n stage : %d, coord x: %d,coord y : %d \n",stage,status[stage].x,status[stage].y);
//            printf("%d,%d\n",x,y);
            printBoard();
        }
        
 
        //AI
        else { // get computer move
            if (stage==0) {
                status[0].x=9;
                status[0].y=9;
            }//AI가 먼저라면 그냥 정중앙에 두자
            else{
                status[stage] = getNextMove();//보드,현재 돌이 있는 좌표,색, stage정보 전달(얘는 메모리 설정 위해서 전달)
            }
            x = status[stage].x;
            y = status[stage].y;
            setDol(x,y, ai_color);
            printf("\n AI가 %d,%d 에 뒀습니다. \n\n", status[stage].x, status[stage].y);
            printBoard();
        }//AI는 알아서 좌표에 넣도록 할까...?

        
        //여기까지 돌을 두는 과정
        //돌을 두고 난 다음의 처리, 정상적으로 돌을 뒀다고 가정하면, 출력, 승리(무승부)조건 확인, 승리 안 났으면 턴 바꿔 줌
        
        if(isFinish(x, y)){
            if(now==player){
                printf("플레이어 승리! \n");
                win = 0;
            }
            else{
                printf("플레이어 패배! 쿠쿠루삥빵뽕 \n");
                win = 0;
            }
        }
        if(stage==360){
            win=0;
            printf("무승부!\n");
        }
        //종료 조건 확인
        
        stage++;
        now = 3 - now;
        //턴 바꿔서 다시 시도
    }
    return 0;
}
