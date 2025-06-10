#ifndef CHESS_H
#define CHESS_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// 체스 말의 종류를 나타내는 열거형
typedef enum {
    EMPTY,
    PAWN,
    KNIGHT,
    BISHOP,
    ROOK,
    QUEEN,
    KING
} PieceType;

// 체스 말의 색상을 나타내는 열거형
typedef enum {
    WHITE,
    BLACK
} Color;

// 체스 말을 나타내는 구조체
typedef struct {
    PieceType type;
    Color color;
} Piece;

// 캐슬링 가능 여부를 저장하는 구조체
typedef struct {
    bool whiteKingSide;
    bool whiteQueenSide;
    bool blackKingSide;
    bool blackQueenSide;
} CastlingRights;

// 앙파상 가능한 폰의 위치를 저장하는 구조체
typedef struct {
    int row;
    int col;
    bool possible;
} EnPassantTarget;

// 체스 보드를 나타내는 구조체
typedef struct {
    Piece board[8][8];
    Color currentTurn;
    bool gameOver;
    CastlingRights castlingRights;
    EnPassantTarget enPassantTarget;
} ChessBoard;

// 함수 선언
void initializeBoard(ChessBoard* board);
void printBoard(const ChessBoard* board);
bool isValidMove(const ChessBoard* board, int fromRow, int fromCol, int toRow, int toCol);
void makeMove(ChessBoard* board, int fromRow, int fromCol, int toRow, int toCol);
bool isCheck(const ChessBoard* board, Color color);
bool isCheckmate(const ChessBoard* board, Color color);
bool isStalemate(const ChessBoard* board, Color color);

// 각 말의 이동 규칙 확인 함수 선언
bool isValidPawnMove(const ChessBoard* board, int fromRow, int fromCol, int toRow, int toCol);
bool isValidKnightMove(int fromRow, int fromCol, int toRow, int toCol);
bool isValidBishopMove(const ChessBoard* board, int fromRow, int fromCol, int toRow, int toCol);
bool isValidRookMove(const ChessBoard* board, int fromRow, int fromCol, int toRow, int toCol);
bool isValidQueenMove(const ChessBoard* board, int fromRow, int fromCol, int toRow, int toCol);
bool isValidKingMove(const ChessBoard* board, int fromRow, int fromCol, int toRow, int toCol);

// 특수 규칙 관련 함수 선언
bool isValidCastling(const ChessBoard* board, int fromRow, int fromCol, int toRow, int toCol);
void handlePawnPromotion(ChessBoard* board, int row, int col);
bool isValidEnPassant(const ChessBoard* board, int fromRow, int fromCol, int toRow, int toCol);
void updateCastlingRights(ChessBoard* board, int fromRow, int fromCol, int toRow, int toCol);

#endif // CHESS_H 