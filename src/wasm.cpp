#include <vector>

#include "perft.h"
#include "moveGen.h"
#include "search.h"

#include <emscripten/bind.h>
using namespace emscripten;

EMSCRIPTEN_BINDINGS(module) {
	function("getAttacked", &moveGen::get_checks_for_JS);
	function("makeMove", &move::make_move_for_JS);
	function("moveGen", &moveGen::move_gen_for_JS);
	function("perft", &perft::test);
	function("search", &search::search);
	value_object<move::Move>("Move")
		.field("source", &move::Move::source)
		.field("dest", &move::Move::dest)
		.field("castle", &move::Move::castle)
		.field("promote", &move::Move::promote)
		.field("isEp", &move::Move::isEp);
	value_object<perft::PerftResult>("PerftResult")
		.field("answer", &perft::PerftResult::answer)
		.field("time", &perft::PerftResult::time);
	value_object<search::SearchResult>("SearchResult")
		.field("move", &search::SearchResult::move)
		.field("depth", &search::SearchResult::depth)
		.field("eval", &search::SearchResult::eval)
		.field("mateFound", &search::SearchResult::mate_found);
	register_vector<int>("IntVector");
	register_vector<move::Move>("MoveVector");
}
