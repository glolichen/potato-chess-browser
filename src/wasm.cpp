#include <vector>

#include "js/board.h"
#include "js/checks.h"
#include "js/move.h"
#include "js/moveGen.h"

#include "bitboard/perft.h"
#include "bitboard/search.h"

#include <emscripten/bind.h>
using namespace emscripten;

EMSCRIPTEN_BINDINGS(my_module) {
	function("decode", &board::decode);
	function("encode", &board::encode);
	function("printBoard", &board::printBoard);
	function("moveGen", &moveGen::moveGenForJS);
	function("getAttacked", &checks::getAttackedForJS);
	function("perft", &perft::test);
	function("search", &search::search);
	value_object<move::Move>("Move")
		.field("source", &move::Move::source)
		.field("dest", &move::Move::dest)
		.field("capture", &move::Move::capture)
		.field("castle", &move::Move::castle)
		.field("promote", &move::Move::promote)
		.field("signal", &move::Move::signal)
		.field("isEp", &move::Move::isEp);
	value_object<perft::PerftResult>("PerftResult")
		.field("answer", &perft::PerftResult::answer)
		.field("time", &perft::PerftResult::time);
	value_object<search::SearchResult>("SearchResult")
		.field("move", &search::SearchResult::move)
		.field("depth", &search::SearchResult::depth)
		.field("eval", &search::SearchResult::eval)
		.field("mateFound", &search::SearchResult::mateFound);
	value_object<checks::Check>("Check")
		.field("coord", &checks::Check::coord)
		.field("original", &checks::Check::original)
		.field("axis", &checks::Check::axis);
	register_vector<move::Move>("MoveVector");
	register_vector<checks::Check>("CheckVector");
}
