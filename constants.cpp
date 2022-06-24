#include "constants.h"

int constants::ROOK[4] = { 1, -1, 12, -12 };
int constants::BISHOP[4] = { 11, -11, 13, -13 };
int constants::QUEEN[8] = { 1, -1, 12, -12, 11, -11, 13, -13 };
int constants::KNIGHT[8] = { 10, -10, 14, -14, 23, -23, 25, -25 };
int constants::KING[8] = { 1, -1, 12, -12, 11, -11, 13, -13 };

int constants::PAWN_MOVE[2][1] = { { -12 } ,{ 12 } };
int constants::PAWN_CAPTURE[2][2] = { { -11, -13 }, { 11, 13 } };
int constants::PAWN_START[2][1] = { { -24 }, { 24 } };

bool constants::WHITE = false;
bool constants::BLACK = true;
