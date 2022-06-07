#ifndef BOOK_H
#define BOOK_H

#include <map>
#include <vector>
#include <string>
#include "moves.h"

namespace book {
    extern std::map<std::string, std::vector<moves::SimpleMove>> openingBook;
    void initOpeningBook();
}

#endif
