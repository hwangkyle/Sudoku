#pragma once
#pragma once

using namespace std;

class Board {
public:
	Board(unsigned diff);
	void displayPuzzle();
	int** getPuzzle();
	void insert(unsigned r, unsigned c, int move);
	bool solve();
	bool isFinished();

private:
	int base[9][9] = { 0 };
	int puzzle[9][9] = { 0 };
	void createBase(int b[9][9]);
	void puzzlify(int b[9][9], unsigned diff);
	bool validMove(int b[9][9], unsigned r, unsigned c, int move);
	bool checkAmountFilled(int b[9][9], unsigned diff);
};