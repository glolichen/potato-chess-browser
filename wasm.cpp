#include <chrono>
#include <string>
#include <vector>

#include "attacked.h"
#include "board.h"
#include "checks.h"
#include "moveGen.h"
#include "moves.h"
#include "perft.h"
#include "search.h"

#include <emscripten/bind.h>
using namespace emscripten;

EMSCRIPTEN_BINDINGS(my_module) {
	function("decode", &board::decode);
	function("encode", &board::encode);
	function("printBoard", &board::printBoard);
	function("moveGen", &moveGen::moveGenForJS);
	function("runPerft", &perft::runPerft);
	function("getAttacked", &attacked::getAttackedForJS);
	function("search", &search::search);
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
	value_object<search::SearchResult>("SearchRersult")
		.field("move", &search::SearchResult::move)
		.field("depth", &search::SearchResult::depth)
		.field("eval", &search::SearchResult::eval)
		.field("mateFound", &search::SearchResult::mateFound);
	register_vector<moves::Move>("MoveVector");
	register_vector<checks::Check>("CheckVector");
	register_vector<perft::Entry>("EntryVector");
}
