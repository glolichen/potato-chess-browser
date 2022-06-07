#include <map>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>

#include "moves.h"
#include "hashing.h"
#include "board.h"
#include "printing.h"
#include "hashing.h"
#include "book.h"

std::map<std::string, std::vector<moves::SimpleMove>> book::openingBook;

void book::initOpeningBook() {
    std::ifstream fin;
    fin.open("Assets/bookFEN.txt");

    std::string line;

    if (fin.is_open()) {
        while (std::getline(fin, line)) {
            if (line == "")
                break;

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

            openingBook.insert(std::pair<std::string, std::vector<moves::SimpleMove>>{truncatedFEN, bookMoves});
        }
        fin.close();
    }
}