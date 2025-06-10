#include "chess.h"
#include <windows.h>

int main() {
    // 콘솔 인코딩을 UTF-8로 설정
    SetConsoleOutputCP(CP_UTF8);
    
    ChessBoard board; // 체스 보드 구조체 선언
    initializeBoard(&board); // 체스 보드 초기화
    
    printf("체스 게임을 시작합니다!\n"); // 게임 시작 메시지 출력
    printf("이동은 '시작위치 목적지' 형식으로 입력하세요 (예: e2 e4)\n"); // 이동 방법 설명
    printf("캐슬링은 'e1 g1'(킹사이드) 또는 'e1 c1'(퀸사이드)로 입력하세요\n"); // 캐슬링 방법 설명
    
    while (!board.gameOver) { // 게임 종료 전까지 반복
        printBoard(&board); // 체스 보드 출력
        
        printf("\n%s의 차례입니다.\n", board.currentTurn == WHITE ? "흰색" : "검은색"); // 현재 차례 출력
        printf("이동할 말의 위치를 입력하세요 (예: e2 e4): "); // 이동 방법 설명
        
        char from[3], to[3]; // 시작 위치와 목적지 위치 선언
        if (scanf("%2s %2s", from, to) != 2) { // 입력 받기
            printf("잘못된 입력입니다. 다시 시도하세요.\n"); // 잘못된 입력 메시지 출력
            continue;
        }
        
        // 입력된 위치를 배열 인덱스로 변환
        int fromCol = from[0] - 'a';
        int fromRow = from[1] - '1';
        int toCol = to[0] - 'a';
        int toRow = to[1] - '1';
        
        if (isValidMove(&board, fromRow, fromCol, toRow, toCol)) { // 이동 가능 여부 확인
            makeMove(&board, fromRow, fromCol, toRow, toCol); // 이동 실행 
            
            // 체크메이트 확인
            if (isCheckmate(&board, board.currentTurn)) {
                printf("\n체크메이트! %s의 승리입니다!\n", 
                       board.currentTurn == WHITE ? "검은색" : "흰색");
                board.gameOver = true;
            }
            // 스테일메이트 확인
            else if (isStalemate(&board, board.currentTurn)) {
                printf("\n스테일메이트! 무승부입니다.\n");
                board.gameOver = true;
            }
        } else {
            printf("잘못된 이동입니다. 다시 시도하세요.\n");
        }
    }
    
    return 0;
} 