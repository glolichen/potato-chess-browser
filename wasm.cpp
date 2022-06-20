#include <vector>
#include <string>

#include "board.h"
#include "checks.h"
#include "moveGen.h"
#include "perft.h"
#include "moves.h"

#include <emscripten/bind.h>
using namespace emscripten;

// std::vector<std::vector<int>> wasmMoveGen(std::string fen) {
//     board::decode(fen);
// }

EMSCRIPTEN_BINDINGS(my_module) {
    function("decode", &board::decode);
    function("printBoard", &board::printBoard);
    function("moveGen", &moveGen::moveGen);
    function("runPerft", &perft::runPerft);
    function("getAttacked", &checks::getAttacked);
    value_object<moves::Move>("Move")
        .field("source", &moves::Move::source)
        .field("dest", &moves::Move::dest)
        .field("capture", &moves::Move::capture)
        .field("castle", &moves::Move::castle)
        .field("promote", &moves::Move::promote)
        .field("signal", &moves::Move::signal)
        .field("isEp", &moves::Move::isEp);
    value_object<perft::Entry>("Entry")
        .field("moves", &perft::Entry::moves)
        .field("possible", &perft::Entry::possible);
    value_object<checks::Check>("Check")
        .field("coord", &checks::Check::coord)
        .field("original", &checks::Check::original)
        .field("axis", &checks::Check::axis);
    register_vector<moves::Move>("MoveVector");
    register_vector<checks::Check>("CheckVector");
    register_vector<perft::Entry>("EntryVector");
}
