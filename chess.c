#include "chess.h" // 체스 규칙 헤더 파일
#include <windows.h> // 윈도우 헤더 파일

// 보드 초기화 함수
void initializeBoard(ChessBoard* board) {
    // 보드 초기화
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            board->board[i][j].type = EMPTY;
        }
    }

    // 흰색 말 배치
    for (int j = 0; j < 8; j++) {
        board->board[1][j].type = PAWN;
        board->board[1][j].color = WHITE;
    }
    board->board[0][0].type = ROOK;
    board->board[0][1].type = KNIGHT;
    board->board[0][2].type = BISHOP;
    board->board[0][3].type = QUEEN;
    board->board[0][4].type = KING;
    board->board[0][5].type = BISHOP;
    board->board[0][6].type = KNIGHT;
    board->board[0][7].type = ROOK;
    for (int j = 0; j < 8; j++) {
        board->board[0][j].color = WHITE;
    }

    // 검은색 말 배치
    for (int j = 0; j < 8; j++) {
        board->board[6][j].type = PAWN;
        board->board[6][j].color = BLACK;
    }
    board->board[7][0].type = ROOK;
    board->board[7][1].type = KNIGHT;
    board->board[7][2].type = BISHOP;
    board->board[7][3].type = QUEEN;
    board->board[7][4].type = KING;
    board->board[7][5].type = BISHOP;
    board->board[7][6].type = KNIGHT;
    board->board[7][7].type = ROOK;
    for (int j = 0; j < 8; j++) {
        board->board[7][j].color = BLACK;
    }

    board->currentTurn = WHITE;
    board->gameOver = false;

    // 캐슬링 권한 초기화
    board->castlingRights.whiteKingSide = true;
    board->castlingRights.whiteQueenSide = true;
    board->castlingRights.blackKingSide = true;
    board->castlingRights.blackQueenSide = true;

    // 앙파상 타겟 초기화
    board->enPassantTarget.possible = false;
}

// 보드 출력 함수
void printBoard(const ChessBoard* board) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE); // 콘솔 핸들 가져오기
    CONSOLE_SCREEN_BUFFER_INFO consoleInfo; // 콘솔 정보 구조체
    WORD saved_attributes; // 저장된 속성
    GetConsoleScreenBufferInfo(hConsole, &consoleInfo); // 콘솔 정보 가져오기
    saved_attributes = consoleInfo.wAttributes; // 저장된 속성 저장

    printf("  a b c d e f g h\n"); // 열 라벨 출력
    for (int i = 7; i >= 0; i--) { // 행 라벨 출력 (역순)
        printf("%d ", i + 1); // 행 라벨 출력
        for (int j = 0; j < 8; j++) { // 열 라벨 출력
            Piece piece = board->board[i][j]; // 말 정보 가져오기
            char symbol; // 말 기호
            switch (piece.type) { // 말 기호 결정
                case EMPTY: symbol = '.'; break;
                case PAWN: symbol = 'P'; break;
                case KNIGHT: symbol = 'N'; break;
                case BISHOP: symbol = 'B'; break;
                case ROOK: symbol = 'R'; break;
                case QUEEN: symbol = 'Q'; break;
                case KING: symbol = 'K'; break;
            }
            if (piece.type != EMPTY) { // 말이 있는 경우
                if (piece.color == WHITE) { // 흰색 말인 경우
                    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY); // 밝은 흰색
                } else { // 검은색 말인 경우
                    SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY); // 밝은 회색
                }
                printf("%c ", symbol); // 말 기호 출력
                SetConsoleTextAttribute(hConsole, saved_attributes); // 원래 색상으로 복원
            } else { // 말이 없는 경우
                printf("%c ", symbol); // 말 기호 출력
            }
        }
        printf("\n"); // 행 라벨 출력 완료
    }
}

// 각 말의 이동 규칙을 확인하는 함수들
bool isValidPawnMove(const ChessBoard* board, int fromRow, int fromCol, int toRow, int toCol) {
    int direction = (board->board[fromRow][fromCol].color == WHITE) ? 1 : -1; // 방향 결정
    int startRow = (board->board[fromRow][fromCol].color == WHITE) ? 1 : 6; // 시작 행 결정
    
    // 기본 전진
    if (fromCol == toCol && toRow == fromRow + direction && // 기본 전진 조건
        board->board[toRow][toCol].type == EMPTY) { // 목적지에 말이 없는 경우
        return true; // 이동 가능
    }
    
    // 첫 이동시 2칸 전진
    if (fromRow == startRow && fromCol == toCol && // 첫 이동 조건
        toRow == fromRow + 2 * direction && // 2칸 전진 조건
        board->board[fromRow + direction][fromCol].type == EMPTY && // 중간 칸에 말이 없는 경우
        board->board[toRow][toCol].type == EMPTY) { // 목적지에 말이 없는 경우
        return true; // 이동 가능
    }
    
    // 대각선 공격
    if (toRow == fromRow + direction && // 대각선 공격 조건
        abs(toCol - fromCol) == 1 && // 대각선 공격 조건
        board->board[toRow][toCol].type != EMPTY && // 목적지에 말이 있는 경우
        board->board[toRow][toCol].color != board->board[fromRow][fromCol].color) { // 목적지에 같은 색상의 말이 있는 경우
        return true; // 이동 가능
    }
    
    return false; // 이동 불가
}

bool isValidKnightMove(int fromRow, int fromCol, int toRow, int toCol) { // 나이트 이동 규칙 확인 함수
    int rowDiff = abs(toRow - fromRow); // 행 차이
    int colDiff = abs(toCol - fromCol); // 열 차이
    return (rowDiff == 2 && colDiff == 1) || (rowDiff == 1 && colDiff == 2); // 나이트 이동 규칙 확인
}

bool isValidBishopMove(const ChessBoard* board, int fromRow, int fromCol, int toRow, int toCol) { // 비숍 이동 규칙 확인 함수
    if (abs(toRow - fromRow) != abs(toCol - fromCol)) return false; // 대각선 이동 조건
    
    int rowStep = (toRow > fromRow) ? 1 : -1; // 행 차이
    int colStep = (toCol > fromCol) ? 1 : -1; // 열 차이
    
    int currentRow = fromRow + rowStep; // 현재 행
    int currentCol = fromCol + colStep; // 현재 열
    
    while (currentRow != toRow && currentCol != toCol) { // 목적지에 도달할 때까지 반복
        if (board->board[currentRow][currentCol].type != EMPTY) return false; // 목적지에 말이 있는 경우
        currentRow += rowStep; // 현재 행 업데이트
        currentCol += colStep; // 현재 열 업데이트
    }
    
    return true; // 이동 가능
}

bool isValidRookMove(const ChessBoard* board, int fromRow, int fromCol, int toRow, int toCol) { // 룩 이동 규칙 확인 함수
    if (fromRow != toRow && fromCol != toCol) return false; // 룩 이동 조건
    
    int rowStep = (toRow > fromRow) ? 1 : (toRow < fromRow) ? -1 : 0; // 행 차이
    int colStep = (toCol > fromCol) ? 1 : (toCol < fromCol) ? -1 : 0; // 열 차이
    
    int currentRow = fromRow + rowStep; // 현재 행
    int currentCol = fromCol + colStep; // 현재 열
    
    while (currentRow != toRow || currentCol != toCol) { // 목적지에 도달할 때까지 반복
        if (board->board[currentRow][currentCol].type != EMPTY) return false; // 목적지에 말이 있는 경우
        currentRow += rowStep; // 현재 행 업데이트
        currentCol += colStep; // 현재 열 업데이트
    }
    
    return true;
}

bool isValidQueenMove(const ChessBoard* board, int fromRow, int fromCol, int toRow, int toCol) { // 퀸 이동 규칙 확인 함수
    return isValidBishopMove(board, fromRow, fromCol, toRow, toCol) || // 비숍 이동 규칙 확인
           isValidRookMove(board, fromRow, fromCol, toRow, toCol); // 룩 이동 규칙 확인
}

bool isValidKingMove(const ChessBoard* board, int fromRow, int fromCol, int toRow, int toCol) { // 킹 이동 규칙 확인 함수
    return abs(toRow - fromRow) <= 1 && abs(toCol - fromCol) <= 1; // 킹 이동 규칙 확인
}

// 폰 프로모션 처리
void handlePawnPromotion(ChessBoard* board, int row, int col) {
    if ((row == 7 && board->board[row][col].color == WHITE) || // 폰이 흰색 행에 있는 경우
        (row == 0 && board->board[row][col].color == BLACK)) { // 폰이 검은색 행에 있는 경우
        printf("폰이 승급할 수 있습니다. 원하는 말을 선택하세요 (Q/R/B/N): "); // 폰 승급 메시지 출력
        char choice; // 선택 변수
        scanf(" %c", &choice);
        
        PieceType newType;
        switch (choice) {
            case 'Q': newType = QUEEN; break;
            case 'R': newType = ROOK; break;
            case 'B': newType = BISHOP; break;
            case 'N': newType = KNIGHT; break;
            default: newType = QUEEN; // 기본값은 퀸
        }
        
        board->board[row][col].type = newType; // 새로운 말 타입 저장
    }
}

// 앙파상 가능 여부 확인
bool isValidEnPassant(const ChessBoard* board, int fromRow, int fromCol, int toRow, int toCol) { // 앙파상 가능 여부 확인 함수
    if (!board->enPassantTarget.possible) return false; // 앙파상 가능 여부 확인
    
    Piece piece = board->board[fromRow][fromCol]; // 말 정보 가져오기
    if (piece.type != PAWN) return false; // 폰이 아닌 경우
    
    int direction = (piece.color == WHITE) ? 1 : -1; // 방향 결정
    
    return (fromRow + direction == toRow) && // 행 차이
           (abs(fromCol - toCol) == 1) && // 열 차이
           (toRow == board->enPassantTarget.row) && // 앙파상 타겟 행
           (toCol == board->enPassantTarget.col); // 앙파상 타겟 열
}

// 캐슬링 가능 여부 확인
bool isValidCastling(const ChessBoard* board, int fromRow, int fromCol, int toRow, int toCol) { // 캐슬링 가능 여부 확인 함수
    Piece piece = board->board[fromRow][fromCol]; // 말 정보 가져오기
    if (piece.type != KING) return false; // 킹이 아닌 경우
    
    // 킹이 움직였거나 체크 상태면 캐슬링 불가
    if (isCheck(board, piece.color)) return false; // 체크 상태면 캐슬링 불가
    
    bool isKingSide = (toCol > fromCol); // 킹 사이드 결정
    bool isWhite = (piece.color == WHITE); // 흰색 킹인지 검은색 킹인지 결정
    
    // 캐슬링 권한 확인
    if (isWhite) {
        if (isKingSide && !board->castlingRights.whiteKingSide) return false; // 킹 사이드 캐슬링 권한 확인
        if (!isKingSide && !board->castlingRights.whiteQueenSide) return false; // 퀸 사이드 캐슬링 권한 확인
    } else {
        if (isKingSide && !board->castlingRights.blackKingSide) return false; // 킹 사이드 캐슬링 권한 확인
        if (!isKingSide && !board->castlingRights.blackQueenSide) return false; // 퀸 사이드 캐슬링 권한 확인
    }
    
    // 킹과 룩 사이에 다른 말이 있는지 확인
    int rookCol = isKingSide ? 7 : 0;
    int step = isKingSide ? 1 : -1;
    for (int col = fromCol + step; col != rookCol; col += step) {
        if (board->board[fromRow][col].type != EMPTY) return false; // 킹과 룩 사이에 다른 말이 있는지 확인
    }
    
    // 킹이 이동하는 경로에 체크가 있는지 확인
    for (int col = fromCol + step; col != toCol + step; col += step) { // 킹이 이동하는 경로에 체크가 있는지 확인
        ChessBoard tempBoard = *board;
        tempBoard.board[fromRow][col] = tempBoard.board[fromRow][fromCol];
        tempBoard.board[fromRow][fromCol].type = EMPTY;
        if (isCheck(&tempBoard, piece.color)) return false;
    }
    
    return true;
}

// 캐슬링 권한 업데이트
void updateCastlingRights(ChessBoard* board, int fromRow, int fromCol, int toRow, int toCol) {
    Piece piece = board->board[fromRow][fromCol];
    
    // 킹이 움직이면 양쪽 캐슬링 모두 불가
    if (piece.type == KING) {
        if (piece.color == WHITE) {
            board->castlingRights.whiteKingSide = false;
            board->castlingRights.whiteQueenSide = false;
        } else {
            board->castlingRights.blackKingSide = false;
            board->castlingRights.blackQueenSide = false;
        }
    }
    
    // 룩이 움직이면 해당 방향의 캐슬링만 불가
    if (piece.type == ROOK) {
        if (piece.color == WHITE) {
            if (fromCol == 0) board->castlingRights.whiteQueenSide = false;
            if (fromCol == 7) board->castlingRights.whiteKingSide = false;
        } else {
            if (fromCol == 0) board->castlingRights.blackQueenSide = false;
            if (fromCol == 7) board->castlingRights.blackKingSide = false;
        }
    }
}

// 스테일메이트 확인
bool isStalemate(const ChessBoard* board, Color color) {
    if (isCheck(board, color)) return false;
    
    // 모든 가능한 이동을 시도
    for (int fromRow = 0; fromRow < 8; fromRow++) {
        for (int fromCol = 0; fromCol < 8; fromCol++) {
            if (board->board[fromRow][fromCol].type != EMPTY && 
                board->board[fromRow][fromCol].color == color) {
                for (int toRow = 0; toRow < 8; toRow++) {
                    for (int toCol = 0; toCol < 8; toCol++) {
                        if (isValidMove(board, fromRow, fromCol, toRow, toCol)) {
                            return false;
                        }
                    }
                }
            }
        }
    }
    
    return true;
}

// 이동 유효성 검사 함수 수정
bool isValidMove(const ChessBoard* board, int fromRow, int fromCol, int toRow, int toCol) {
    // 기본적인 경계 검사
    if (fromRow < 0 || fromRow > 7 || fromCol < 0 || fromCol > 7 ||
        toRow < 0 || toRow > 7 || toCol < 0 || toCol > 7) {
        return false;
    }

    // 시작 위치에 말이 있는지 확인
    if (board->board[fromRow][fromCol].type == EMPTY) {
        return false;
    }

    // 현재 턴의 색상과 말의 색상이 일치하는지 확인
    if (board->board[fromRow][fromCol].color != board->currentTurn) {
        return false;
    }

    // 목적지에 같은 색상의 말이 있는지 확인
    if (board->board[toRow][toCol].type != EMPTY &&
        board->board[toRow][toCol].color == board->board[fromRow][fromCol].color) {
        return false;
    }

    Piece piece = board->board[fromRow][fromCol];
    bool isValid = false;
    
    // 특수 규칙 검사
    if (piece.type == KING && abs(toCol - fromCol) == 2) {
        return isValidCastling(board, fromRow, fromCol, toRow, toCol);
    }
    
    if (piece.type == PAWN && isValidEnPassant(board, fromRow, fromCol, toRow, toCol)) {
        return true;
    }
    
    // 일반 이동 규칙 검사
    switch (piece.type) {
        case PAWN:
            isValid = isValidPawnMove(board, fromRow, fromCol, toRow, toCol);
            break;
        case KNIGHT:
            isValid = isValidKnightMove(fromRow, fromCol, toRow, toCol);
            break;
        case BISHOP:
            isValid = isValidBishopMove(board, fromRow, fromCol, toRow, toCol);
            break;
        case ROOK:
            isValid = isValidRookMove(board, fromRow, fromCol, toRow, toCol);
            break;
        case QUEEN:
            isValid = isValidQueenMove(board, fromRow, fromCol, toRow, toCol);
            break;
        case KING:
            isValid = isValidKingMove(board, fromRow, fromCol, toRow, toCol);
            break;
        default:
            return false;
    }
    
    if (!isValid) return false;
    
    // 이동 후 체크 상태가 되는지 확인
    ChessBoard tempBoard = *board;
    tempBoard.board[toRow][toCol] = tempBoard.board[fromRow][fromCol];
    tempBoard.board[fromRow][fromCol].type = EMPTY;
    
    return !isCheck(&tempBoard, piece.color);
}

// 체크 상태 확인 함수 구현
bool isCheck(const ChessBoard* board, Color color) {
    // 킹의 위치 찾기
    int kingRow = -1, kingCol = -1;
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (board->board[i][j].type == KING && board->board[i][j].color == color) {
                kingRow = i;
                kingCol = j;
                break;
            }
        }
        if (kingRow != -1) break;
    }
    
    // 상대방의 모든 말이 킹을 공격할 수 있는지 확인
    Color opponentColor = (color == WHITE) ? BLACK : WHITE;
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (board->board[i][j].type != EMPTY && board->board[i][j].color == opponentColor) {
                if (isValidMove(board, i, j, kingRow, kingCol)) {
                    return true;
                }
            }
        }
    }
    
    return false;
}

// 체크메이트 상태 확인 함수 구현
bool isCheckmate(const ChessBoard* board, Color color) {
    if (!isCheck(board, color)) return false;
    
    // 모든 가능한 이동을 시도
    for (int fromRow = 0; fromRow < 8; fromRow++) {
        for (int fromCol = 0; fromCol < 8; fromCol++) {
            if (board->board[fromRow][fromCol].type != EMPTY && 
                board->board[fromRow][fromCol].color == color) {
                for (int toRow = 0; toRow < 8; toRow++) {
                    for (int toCol = 0; toCol < 8; toCol++) {
                        if (isValidMove(board, fromRow, fromCol, toRow, toCol)) {
                            // 이동 후 체크 상태가 해소되는지 확인
                            ChessBoard tempBoard = *board;
                            tempBoard.board[toRow][toCol] = tempBoard.board[fromRow][fromCol];
                            tempBoard.board[fromRow][fromCol].type = EMPTY;
                            
                            if (!isCheck(&tempBoard, color)) {
                                return false;
                            }
                        }
                    }
                }
            }
        }
    }
    
    return true;
}

// 말 이동 함수
void makeMove(ChessBoard* board, int fromRow, int fromCol, int toRow, int toCol) {
    if (isValidMove(board, fromRow, fromCol, toRow, toCol)) {
        Piece piece = board->board[fromRow][fromCol];
        
        // 킹이 잡혔는지 확인
        if (board->board[toRow][toCol].type == KING) {
            board->gameOver = true;
            printf("\n킹이 잡혔습니다! %s의 승리입니다!\n", 
                   board->currentTurn == WHITE ? "흰색" : "검은색");
        }
        
        // 캐슬링 처리
        if (piece.type == KING && abs(toCol - fromCol) == 2) {
            bool isKingSide = (toCol > fromCol);
            int rookFromCol = isKingSide ? 7 : 0;
            int rookToCol = isKingSide ? toCol - 1 : toCol + 1;
            
            // 룩 이동
            board->board[toRow][rookToCol] = board->board[fromRow][rookFromCol];
            board->board[fromRow][rookFromCol].type = EMPTY;
        }
        
        // 앙파상 처리
        if (piece.type == PAWN && isValidEnPassant(board, fromRow, fromCol, toRow, toCol)) {
            board->board[fromRow][toCol].type = EMPTY; // 잡힌 폰 제거
        }
        
        // 일반 이동
        board->board[toRow][toCol] = board->board[fromRow][fromCol];
        board->board[fromRow][fromCol].type = EMPTY;
        
        // 폰 프로모션 처리
        handlePawnPromotion(board, toRow, toCol);
        
        // 캐슬링 권한 업데이트
        updateCastlingRights(board, fromRow, fromCol, toRow, toCol);
        
        // 앙파상 타겟 업데이트
        if (piece.type == PAWN && abs(toRow - fromRow) == 2) {
            board->enPassantTarget.row = (fromRow + toRow) / 2;
            board->enPassantTarget.col = toCol;
            board->enPassantTarget.possible = true;
        } else {
            board->enPassantTarget.possible = false;
        }
        
        // 게임이 종료되지 않았다면 턴 변경
        if (!board->gameOver) {
            board->currentTurn = (board->currentTurn == WHITE) ? BLACK : WHITE;
        }
    }
} 