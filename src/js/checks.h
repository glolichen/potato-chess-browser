#ifndef CHECKS_H
#define CHECKS_H

namespace checks {
	struct Check {
		int coord; // Square of piece getting checked
		int original; // Square of piece checking the king
		int axis; // Axis of the check (offset from the piece to the king)
	};
	
	void getAttacked(std::vector<checks::Check>* attacked);
	std::vector<checks::Check> getAttackedForJS();
	void pchecks(int loc, std::vector<checks::Check>* attacked);
	void nchecks(int loc, std::vector<checks::Check>* attacked);
	void bchecks(int loc, std::vector<checks::Check>* attacked);
	void rchecks(int loc, std::vector<checks::Check>* attacked);
	void kchecks(int loc, std::vector<checks::Check>* attacked);

	bool enPassantLegal();

	void getPinned(std::vector<checks::Check>* pinned);
	void getBlocks(std::vector<int>* blocks, std::vector<checks::Check>* attacked);
}

#endif
