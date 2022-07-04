#include <fstream>
#include <string>
#include <vector>

#include "board.h"
#include "moveGen.h"

char board::board[144];
int board::king[2];
int board::enPassant = -1;

std::vector<int> board::pieces[2];

bool board::K = 0; // White Kingside
bool board::Q = 0; // White Queenside
bool board::k = 0; // Black Kingside
bool board::q = 0; // Black Queenside

bool board::turn = 0; // 0 = White, 1 = Black

int board::fiftyMoveClock = 0;

void board::decode(std::string fen) {   
    std::ofstream fout("file.txt");
    fout << fen;
    fout.close();

    std::vector<std::string> result = board::split(fen, ' ');

    if (!result[1].compare("w"))
        board::turn = false;
    else
        board::turn = true;

    board::fiftyMoveClock = stoi(result[4]);

    std::string castle = result[2];
    if (castle.find('K') != std::string::npos)
        board::K = true;
    if (castle.find('Q') != std::string::npos)
        board::Q = true;
    if (castle.find('k') != std::string::npos)
        board::k = true;
    if (castle.find('q') != std::string::npos)
        board::q = true;

    std::string ep = result[3];
    if (ep.compare("-")) {
        board::enPassant = board::SANToNotation(ep);
    }


    std::vector<std::string> line = board::split(result[0], '/');

    std::vector<int> empty;
    for (int i = 0; i < 8; i++) {
        int cur = 2;
        std::string curRank = line[i];
        for (int j = 0; j < curRank.size(); j++) {
            if (isdigit(curRank[j])) {
                int max = cur + (curRank[j] - '0');
                for (int k = cur; k < max; k++)
                    empty.push_back(i * 12 + k + 24);
                cur = max;
                continue;
            }

            board::board[i * 12 + cur + 24] = curRank[j];
            cur++;
        }
    }

    for (int square : empty)
        board::board[square] = 0;

    for (int i = 0; i < 24; i++)
        board::board[i] = -1;
    for (int i = 120; i < 144; i++)
        board::board[i] = -1;

    for (int i = 2; i < 10; i++) {
        board::board[i * 12] = -1;
        board::board[i * 12 + 1] = -1;
        board::board[i * 12 + 10] = -1;
        board::board[i * 12 + 11] = -1;
    }

    board::pieces[0].clear();
    board::pieces[1].clear();

    for (int i = 2; i < 10; i++) {
        for (int j = 2; j < 10; j++) {
            int index = i * 12 + j;
            if (board::board[index] == 0)
                continue;

            int color = (bool) islower(board::board[index]);

            if (board::board[index] == 'K')
                board::king[0] = index;
            if (board::board[index] == 'k')
                board::king[1] = index;

            board::pieces[color].push_back(index);
        }
    }
}
std::string board::encode() {
    std::string fen = "";
    for (int i = 2; i < 10; i++) {
        int empty = 0;
        for (int j = 2; j < 10; j++) {
            if (board::board[i * 12 + j] == 0)
                empty++;
            else {
                if (empty != 0)
                    fen += empty + '0';
                empty = 0;
                fen += (char) board::board[i * 12 + j];
            }
        }

        if (empty != 0)
            fen += empty + '0';

        if (i != 9)
            fen += "/";
    }
    fen += " ";

    fen += board::turn ? 'b' : 'w';
    fen += " ";

    std::string castleRights = "";
    if (board::K)
        castleRights += "K";
    if (board::Q)
        castleRights += "Q";
    if (board::k)
        castleRights += "k";
    if (board::q)
        castleRights += "q";
    if (castleRights == "")
        castleRights = "-";
    fen += castleRights;
    fen += " ";

    if (board::enPassant == -1)
        fen += "-";
    else
        fen += board::notationToSAN(board::enPassant);
        
    fen += " 0 ";
    fen += std::to_string(board::fiftyMoveClock + 1);

    return fen;
}

void board::printBoard() {
    std::cout << "+---+---+---+---+---+---+---+---+" << "\n";

    for (int i = 2; i < 10; i++) {
        for (int j = 2; j < 10; j++) {
            int index = i * 12 + j;
            char print = ' ';

            if (board::board[index] != 0)
                print = (char) (board::board[index]);

            std::cout << "| " << print << " ";
        }
        std::cout << "|" << "\n" << "+---+---+---+---+---+---+---+---+" << "\n";
    }

    std::cout << " ";
    std::cout << "\n" << "FEN: " << board::encode();
    std::cout << "\n" << "White Kingside: " << board::K << "      " << "White Queenside: " << board::Q;
    std::cout << "\n" << "Black Kingside: " << board::k << "      " << "Black Queenside: " << board::q;

    std::cout << "\n" << "En Passant Square: " << board::toXY(board::enPassant);
    std::cout << "\n" << "Turn: " << (board::turn ? "Black" : "White");

    std::cout << "\n";
}

std::string board::truncateFEN(std::string fen) {
    std::vector<std::string> result = board::split(fen, ' ');
    return result[0];
}

std::vector<std::string> board::split(std::string str, char splitOn) {
    std::vector<std::string> result;

    int left = 0;
    for (int i = 0; i < str.length(); i++) {
        if (str[i] == splitOn) {
            result.push_back(str.substr(left, i - left));
            left = i + 1;
        }
    }

    result.push_back(str.substr(left, str.length() - left));

    return result;
}

int board::SANToNotation(std::string san) {
    int y = (int) san[0] - 'a' + 2;
    int x = 8 - (san[1] - '0') + 2;
    return x * 12 + y;
}
std::string board::toXY(int coord) {
    if (board::board[coord] == -1)
        return "-1";
    if (coord == -1)
        return "-1";

    coord -= 26;
    coord -= (coord / 12) * 4;

    std::string x = std::to_string(coord / 8);
    std::string y = std::to_string(coord % 8);
    return x + y;
}
std::string board::notationToSAN(int coord) {
    std::string xy = board::toXY(coord);

    int x = 8 - (xy[0] - '0');
    char y = xy[1] - '0' + 'a';

    std::string out = "";
    out += y;
    out += x + '0';

    return out;
}
int board::toRegularBoard(int coord) {
    coord -= 26;
    coord -= (coord / 12) * 4;

    return coord;
}
int board::toNotation(int x, int y) {
    return (y + 2) + x * 12 + 24;
}