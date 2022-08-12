// #include <vector>

// #include "perft.h"
// #include "moveGen.h"
// #include "search.h"

// #include <emscripten/bind.h>
// using namespace emscripten;

// EMSCRIPTEN_BINDINGS(my_module) {
// 	function("getAttacked", &moveGen::getChecksForJS);
// 	function("makeMove", &move::makeMoveForJS);
// 	function("moveGen", &moveGen::moveGenForJS);
// 	function("perft", &perft::test);
// 	function("search", &search::search);
// 	value_object<move::Move>("Move")
// 		.field("source", &move::Move::source)
// 		.field("dest", &move::Move::dest)
// 		.field("castle", &move::Move::castle)
// 		.field("promote", &move::Move::promote)
// 		.field("isEp", &move::Move::isEp);
// 	value_object<perft::PerftResult>("PerftResult")
// 		.field("answer", &perft::PerftResult::answer)
// 		.field("time", &perft::PerftResult::time);
// 	value_object<search::SearchResult>("SearchResult")
// 		.field("move", &search::SearchResult::move)
// 		.field("depth", &search::SearchResult::depth)
// 		.field("eval", &search::SearchResult::eval)
// 		.field("mateFound", &search::SearchResult::mateFound);
// 	register_vector<int>("IntVector");
// 	register_vector<move::Move>("MoveVector");
// }
