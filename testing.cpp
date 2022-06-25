// #include <iostream>
// #include <chrono>

// #include "board.h"
// #include "moveGen.h"
// #include "perft.h"

// int main() {    
//     int start = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
//     auto all = perft::runPerft("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1", 6);
//     int end = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    
//     std::cout << end - start << "\n";

//     return 0;
// }