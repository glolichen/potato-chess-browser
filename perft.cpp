#include <iostream>
#include <vector>
#include <chrono>
#include <ctime>

#include "perft.h"
#include "moves.h"
#include "moveGen.h"
#include "board.h"

perft::ul perft::perft(int depth, bool first) {
    if (depth == 1)
        return moveGen::moveGen().size();

    std::vector<moves::Move> moves = moveGen::moveGen();

    ul old = 0;
    ul positions = 0;

    for (moves::Move m : moves) {
        if (first) {
            moves::printMove(m, false);
            std::cout << ": " << std::flush;
        }

        makeMove(m);
        positions += perft(depth - 1, false);
        unmakeMove(m);


        if (first) {
            std::cout << positions - old << std::endl;
            old = positions;
        }
    }

    return positions;
}
void perft::runPerft(int depth) {
    auto start = std::chrono::system_clock::now();
    ul result = perft::perft(depth, true);
    std::cout << "Total nodes: " << result << "\n";
    auto end = std::chrono::system_clock::now();
 
    std::chrono::duration<double> elapsedSeconds = end-start;
    std::cout << "Elapsed time: " << elapsedSeconds.count()*1000 << "ms\n";
    std::cout << "Nodes per Second: " << (int) (result / elapsedSeconds.count()) << "\n";
}