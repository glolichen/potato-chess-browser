#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "board.h"
#include "book.h"
#include "moves.h"

std::map<std::string, std::vector<moves::SimpleMove>> book::openingBook;

void book::initOpeningBook(std::string text) {
    std::vector<std::string> split = board::split(text, '\n');
    for (std::string line : split) {
        std::vector<std::string> result;
        int left = 0;
        for (int i = 0; i < line.length(); i++) {
            if (line[i] == '|') {
                result.push_back(line.substr(left, i - left));
                left = i + 1;
            }
        }
        result.push_back(line.substr(left, line.length() - left));
        result[0].pop_back();

        std::string truncatedFEN = board::truncateFEN(result[0]);

        std::vector<std::string> movesAsString;
        left = 0;
        for (int i = 0; i < result[1].length(); i++) {
            if (result[1][i] == ' ') {
                movesAsString.push_back(result[1].substr(left, i - left));
                left = i + 1;
            }
        }
        movesAsString.push_back(result[1].substr(left, result[1].length() - left));

        std::vector<moves::SimpleMove> bookMoves;
        for (std::string &move: movesAsString) {
            if (move != "")
                bookMoves.push_back(moves::convertUCI(move));
        }

        openingBook.insert({ truncatedFEN, bookMoves });
    }
}
