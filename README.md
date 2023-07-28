# Potato Chess

## Fork of https://github.com/glolichen/potato-chess-uci

A chess engine written in C++ and compiled to Web Assembly

Features:
 * Written in C++ and compiled to web assembly using [Emscripten](https://github.com/emscripten-core/emscripten)
   * Compiled using the command `em++ src/*.cpp -o js/engine.js -lembind -O3 -sALLOW_MEMORY_GROWTH`
   * Memory growth must be allowed as memory usage usually exceeds 16MB
 * Can also be used as a desktop app (electron) for maximum performance
 * Bitboard-based move generator
 * 30-100 million nodes per second move generator (depends on platform and position)
   * Keeps track of pinned pieces and the direction they are pinned
   * Keeps track of squares attacked by the opponent to prevent king from moving there
 * Tapered evaluation (uses game phase to "weigh" the importance of midgame score to endgame score) with piece tables
 * Minimax search with Alpha-Beta pruning
 * Transposition table with Zobrist Hashing
 * Move ordering is used to guess the strength of moves to speed up AB pruning
 * Syzygy endgame tablebase (powered by [Lichess](https://github.com/lichess-org/lila-tablebase))
