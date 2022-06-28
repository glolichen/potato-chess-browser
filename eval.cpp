#include <algorithm>
#include <iostream>
#include <map>

#include "board.h"
#include "eval.h"
#include "pieces.h"

const int PAWN[8][8] = {{0,  0,  0,   0,   0,   0,   0,  0},
                        {50, 50, 50,  50,  50,  50,  50, 50},
                        {10, 10, 20,  30,  30,  20,  10, 10},
                        {5,  5,  10,  25,  25,  10,  5,  5},
                        {0,  0,  0,   35,  37,  0,   0,  0},
                        {5,  -5, -10, 0,   0,   -10, -5, 5},
                        {5,  10, 10,  -40, -40, 10,  10, 5},
                        {0,  0,  0,   0,   0,   0,   0,  0}};
const int KNIGHT[8][8] = {{-50, -40, -30, -30, -30, -30, -40, -50},
                          {-40, -20, 0,   0,   0,   0,   -20, -40},
                          {-30, 0,   15,  15,  15,  15,  0,   -30},
                          {-30, 5,   15,  20,  20,  15,  5,   -30},
                          {-30, 0,   15,  20,  20,  15,  0,   -30},
                          {-50, 5,   15,  15,  15,  15,  5,   -50},
                          {-40, -20, 0,   5,   5,   0,   -20, -40},
                          {-50, -40, -30, -30, -30, -30, -40, -50}};
const int BISHOP[8][8] = {{-20, -10, -10, -10, -10, -10, -10, -20},
                          {-10, 0,   0,   0,   0,   0,   0,   -10},
                          {-10, 0,   5,   10,  10,  5,   0,   -10},
                          {-10, 5,   5,   10,  10,  5,   5,   -10},
                          {-10, 0,   10,  10,  10,  10,  0,   -10},
                          {-10, 10,  10,  10,  10,  10,  10,  -10},
                          {-10, 5,   0,   0,   0,   0,   5,   -10},
                          {-20, -10, -10, -10, -10, -10, -10, -20}};
const int ROOK[8][8] = {{0,  0,  0,  0,  0,  0,  0,  0},
                        {5,  10, 10, 10, 10, 10, 10, 5},
                        {-5, 0,  0,  0,  0,  0,  0,  -5},
                        {-5, 0,  0,  0,  0,  0,  0,  -5},
                        {-5, 0,  0,  0,  0,  0,  0,  -5},
                        {-5, 0,  0,  0,  0,  0,  0,  -5},
                        {-5, 0,  0,  0,  0,  0,  0,  -5},
                        {0,  0,  0,  5,  5,  0,  0,  0}};
const int QUEEN[8][8] = {{-20, -10, -10, -5, -5, -10, -10, -20},
                         {-10, 0,   0,   0,  0,  0,   0,   -10},
                         {-10, 0,   5,   5,  5,  5,   0,   -10},
                         {-5,  0,   5,   5,  5,  5,   0,   -5},
                         {0,   0,   5,   5,  5,  5,   0,   -5},
                         {-10, 5,   5,   5,  5,  5,   0,   -10},
                         {-10, 0,   5,   0,  0,  0,   0,   -10},
                         {-20, -10, -10, -5, -5, -10, -10, -20}};
const int KING_MID[8][8] = {{-30, -40, -40, -50, -50, -40, -40, -30},
                            {-30, -40, -40, -50, -50, -40, -40, -30},
                            {-30, -40, -40, -50, -50, -40, -40, -30},
                            {-30, -40, -40, -50, -50, -40, -40, -30},
                            {-20, -30, -30, -40, -40, -30, -30, -20},
                            {-10, -20, -20, -20, -20, -20, -20, -10},
                            {20,  20,  0,   0,   0,   0,   20,  20},
                            {20,  50,  10,  0,   0,   -20, 50,  20}};
const int KING_END[8][8] = {{-50, -40, -30, -20, -20, -30, -40, -50},
                            {-30, -20, -10, 0,   0,   -10, -20, -30},
                            {-30, -10, 20,  30,  30,  20,  -10, -30},
                            {-30, -10, 30,  40,  40,  30,  -10, -30},
                            {-30, -10, 30,  40,  40,  30,  -10, -30},
                            {-30, -10, 20,  30,  30,  20,  -10, -30},
                            {-30, -30, 0,   0,   0,   0,   -30, -30},
                            {-50, -30, -30, -30, -30, -30, -30, -50}};

const int VALUES[5] = {100, 305, 333, 563, 950};
eval::Table ALL_TABLES[2][5];

eval::Table flip(const int oldTable[8][8]) {
    eval::Table newTable;
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++)
            newTable.table[i][j] = oldTable[7 - i][j];
    }
    return newTable;
}

void eval::initPieceTables() {
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++)
            ALL_TABLES[0][0].table[i][j] = PAWN[i][j];
    }
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++)
            ALL_TABLES[0][1].table[i][j] = KNIGHT[i][j];
    }
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++)
            ALL_TABLES[0][2].table[i][j] = BISHOP[i][j];
    }
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++)
            ALL_TABLES[0][3].table[i][j] = ROOK[i][j];
    }
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++)
            ALL_TABLES[0][4].table[i][j] = QUEEN[i][j];
    }

    ALL_TABLES[1][0] = flip(PAWN);
    ALL_TABLES[1][1] = flip(KNIGHT);
    ALL_TABLES[1][2] = flip(BISHOP);
    ALL_TABLES[1][3] = flip(ROOK);
    ALL_TABLES[1][4] = flip(QUEEN);
}

int eval::evaluate() {
    int white = 0;
    int black = 0;

    int whitePawn = 0;
    int blackPawn = 0;

    int pieceCount = 0;

    for (int i = 2; i < 10; i++) {
        for (int j = 2; j < 10; j++) {
            int index = i * 12 + j;
            if (board::board[index] == 0 || tolower(board::board[index]) == 'k')
                continue;

            int pieceNumber = pieces::PIECE_TABLE.at(tolower(board::board[index]));
            int value = VALUES[pieceNumber];
            pieceCount++;
            if ((bool) islower(board::board[index])) {
                black += value;
                if (board::board[index] == 'p')
                    blackPawn += 100;
                black += ALL_TABLES[1][pieceNumber].table[i - 2][j - 2];
            }
            else {
                white += value;
                if (board::board[index] == 'P')
                    whitePawn += 100;
                white += ALL_TABLES[0][pieceNumber].table[i - 2][j - 2];
            }
        }
    }

    bool endgame = (white + black) < 3526;
    int evaluation = white - black;

    if (endgame) {
        int egWeight = board::turn ? ((black - blackPawn) / 10) : ((white - whitePawn) / 10);
        if (pieceCount <= 4)
            egWeight = 2;
        evaluation += (board::turn ? -1 : 1) * eval::kingLocationEval(egWeight);
    }

    return evaluation;
}

int eval::kingSquareEval(bool endgame) {
    std::string ownKing = board::toXY(board::king[board::turn]);
    int x = ownKing[0]-'0';
    int y = ownKing[1]-'0';

    return endgame ? (KING_END[x][y]) : (KING_MID[x][y]);
}
int eval::kingLocationEval(int EGWeight) {
//    string ownKing = toXY(king[turn]);
    std::string enemyKing = board::toXY(board::king[!board::turn]);

//    int ownKingX = ownKing[0]-'0';
//    int ownKingY = ownKing[1]-'0';

    int enemyKingX = enemyKing[0] - '0';
    int enemyKingY = enemyKing[1] - '0';

    int enemyKingDstX = 3 - std::min(abs(3 - enemyKingX), abs(enemyKingX - 4));
    int enemyKingDstY = 3 - std::min(abs(3 - enemyKingY), abs(enemyKingY - 4));

    return (enemyKingDstX + enemyKingDstY) * EGWeight;
}