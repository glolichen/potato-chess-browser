#include <string>
#include <iostream>
#include <limits.h>

#include "board.h"
#include "moves.h"
#include "printing.h"

void printing::printEval(int eval) {
    if (eval > INT_MAX - 250)
        std::cout << "White has mate in " << INT_MAX - eval;
    else if (eval < INT_MIN + 250)
        std::cout << "Black has mate in " << -(INT_MIN - eval);
    else {
        if (eval >= 0)
            std::cout << "+";
        std::cout << eval;
    }

    std::cout << "\n";
}
