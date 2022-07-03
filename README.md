# Potato Chess

A chess engine written in C++ and compiled to Web Assembly

Features:
 * Written in C++ and compiled to web assembly using [Emscripten](https://github.com/emscripten-core/emscripten)
   * Compiled using the command `em++ -lembind -o engine.js -O3 *.cpp`
 * Can also be used as a desktop app (electron) for maximum performance
 * 144-long array board representation (because I don't understand bitboards)
 * 22 million nodes per second move generator
   * Keeps track of pinned pieces and the direction they are pinned
   * Keeps track of squares attacked by the opponent to prevent king from moving there
 * Tapered evaluation (uses game phase to "weigh" the importance of midgame score to endgame score) with piece tables
 * Minimax search with Alpha-Beta pruning
 * Move ordering is used to guess the strength of moves to speed up AB pruning

By the way, the name "potato chess" came from GothamChess' infamous video [1660 hung a piece on move 6](https://www.youtube.com/watch?v=7MRNWxCuIrI) where he calls the player a potato