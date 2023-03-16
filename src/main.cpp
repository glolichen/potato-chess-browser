#include <iostream>
#include <tuple>

#include "bitboard.h"
#include "eval.h"
#include "maps.h"
#include "move.h"
#include "moveGen.h"
#include "perft.h"
#include "search.h"

#ifdef __EMSCRIPTEN__
	#include <vector>
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
#else
	int main() {
		maps::init();
		eval::init();

		bitboard::decode("r1bqkb1r/pppppppp/2n5/4P3/3Pn3/5Q2/PPP2PPP/RNB1KBNR b KQkq - 0 5");

		// move::print_move(moves[7], 1);
		// std::cout << moves[7] << "\n";

		move::make_move(bitboard::board, 215296);
		bitboard::print_board(bitboard::board);

		std::vector<int> moves;
		moveGen::move_gen(bitboard::board, moves);

		for (int move : moves)
			move::print_move(move, 1);

		// auto search = search::search("rnbqkbnr/pppppppp/8/8/8/4P3/PPPP1PPP/RNBQKBNR b KQkq - 0 1", 3000);
		// move::print_move(search.move, true);
		// std::cout << search.depth;

		// bitboard::decode("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
		// auto hashes1 = hashing::hash(&bitboard::board);

		// bitboard::decode("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
		// auto hashes2 = hashing::hash(&bitboard::board);

		// std::cout << (hashes1 == hashes2) << "\n";

		// move::printMove(211648, true);

		// perft::test("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1", 5);

		return 0;
	}
#endif
