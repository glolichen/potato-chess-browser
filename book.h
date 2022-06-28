#ifndef BOOK_H
#define BOOK_H

#include <map>
#include <string>
#include <vector>
#include "hashing.h"
#include "moves.h"

namespace book {
    extern std::map<std::string, std::vector<moves::SimpleMove>> openingBook;
    void initOpeningBook(std::string text);
}

#endif
