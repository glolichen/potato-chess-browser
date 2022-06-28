#ifndef BOOK_H
#define BOOK_H

#include <string>
#include <unordered_map>
#include <vector>
#include "hashing.h"
#include "moves.h"

namespace book {
    extern std::unordered_map<std::string, std::vector<moves::SimpleMove>> openingBook;
    void initOpeningBook(std::string text);
}

#endif
