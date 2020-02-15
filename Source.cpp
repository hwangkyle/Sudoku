#include "Header.h"
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <vector>
#include <string>

Board::Board(unsigned diff) {
	srand(time(NULL));
	createBase(base); // defines 'base'
	do {
		puzzlify(base, diff); // defines 'puzzle'
	} while (!checkAmountFilled(puzzle, diff));

	// defines 'originalPuzzle'
	for (unsigned i = 0; i < 9; i++)
		for (unsigned j = 0; j < 9; j++)
			originalPuzzle[i][j] = puzzle[i][j];
}


void Board::displayPuzzle() {
	/*
		Self-explanatory
	*/

	for (unsigned i = 0; i < 9; i++) {
		for (unsigned j = 0; j < 9; j++) {
			if (puzzle[i][j] == 0)
				cout << " ";
			else
				cout << puzzle[i][j];
			cout << " ";
		}
		cout << endl;
	}
}

int** Board::getPuzzle() {
	int** arr = new int*[9];
	for (unsigned i = 0; i < 9; i++) {
		arr[i] = new int[9];
		for (unsigned j = 0; j < 9; j++) {
			arr[i][j] = puzzle[i][j];
		}
	}
	return arr;
}

void Board::insert(unsigned r, unsigned c, int move) {
	/*
		Inserts 'move' into the board at row 'r' and col 'c' if possible.
		'r' and 'c' start from 1
		If 'move' does not work, it will tell you.
	*/

	r--; c--;
	if (validMove(puzzle, r, c, move))
		puzzle[r][c] = move;
	else
		cout << "Invalid Move" << endl;
}

void Board::remove(unsigned r, unsigned c) {
	/*
		Removes a move in a square.
		If it is part of the original, it will not remove.
	*/

	if (originalPuzzle[r][c] == 0) // meaning nothing was there originally
		puzzle[r][c] == 0;
	else
		cout << "Can't remove!" << endl;
}

bool Board::isFinished() {
	for (unsigned i = 0; i < 9; i++)
		for (unsigned j = 0; j < 9; j++)
			if (puzzle[i][j] == 0)
				return false;
	return true;
}

bool Board::solve() {
	/*
		Self-explanatory
	*/

	// finds next blank space

	unsigned r = 10, c = 10;
	bool done = false;
	for (unsigned i = 0; i < 9; i++) {
		for (unsigned j = 0; j < 9; j++) {
			if (puzzle[i][j] == 0) {
				r = i;
				c = j;
				done = true;
				break;
			}
		}
		if (done) break;
	}

	// end of the board

	if (r == 10 && c == 10)
		return true;

	// solves

	for (int n = 1; n < 10; n++) {
		if (validMove(puzzle, r, c, n)) {
			puzzle[r][c] = n;
			if (solve())
				return true;
			else
				puzzle[r][c] = 0;
		}
	}
	return false;
}

void Board::createBase(int b[9][9]) {
	/*
		Makes the sudoku board; fills 'base', i.e. no empty (0) spots
	*/

	for (unsigned i = 0; i < 9; i++) {
		vector<int> nums{ 1, 2, 3, 4, 5, 6, 7, 8, 9 };
		int num = nums[rand() % nums.size()];
		for (unsigned j = 0; j < 9; j++) {
			int check = 0;
			while (!validMove(b, i, j, num)) {
				check++;
				if (check == 69) {
					for (unsigned i = 0; i < 9; i++) {
						for (unsigned j = 0; j < 9; j++) {
							b[i][j] = 0;
						}
					}
					i = 0;
					j = 0;
				}
				num = nums[rand() % nums.size()];
			}
			b[i][j] = num;
			num = nums[rand() % nums.size()];
		}
	}
}

void Board::puzzlify(int b[9][9], unsigned diff) {
	/*
		Takes 'base' and randomly puts in empty spots and places that into 'puzzle'
		'diff' (difficulty) is a number from 0 to 5, where 0 is easiest
	*/

	// how manyish squares should be filled; 17 is supposedly the minimum required to solve a sudoku board
	//     0 -> 67, 1 -> 57, ...
	int prob = 17 + ((5 - diff) * 10);

	// randomly fills in 'puzzle'

	for (unsigned i = 0; i < 9; i++) {
		for (unsigned j = 0; j < 9; j++) {
			if (rand() % 81 < prob)
				puzzle[i][j] = base[i][j];
		}
	}
}

bool Board::validMove(int b[9][9], unsigned r, unsigned c, int move) {
	/*
		Used in to see if the insertion 'move' in subsquare of row 'r' and column 'c' works
	*/

	int br[9] = { 0 }, bc[9] = { 0 }, bs[9] = { 0 };
	for (unsigned i = 0; i < 9; i++) {
		br[i] = b[r][i];
		bc[i] = b[i][c];
	}
	int s = (r / 3) + ((c / 3) * 3);
	int m = (s % 3) * 3;
	int n = (s / 3) * 3;
	for (unsigned i = 0; i < 3; i++) {
		for (unsigned j = 0; j < 3; j++) {
			bs[(i * 3) + j] = b[i + m][j + n];
		}
	}
	for (unsigned i = 0; i < 9; i++) {
		if (br[i] == move || bc[i] == move || bs[i] == move)
			return false;
	}
	return true;
}

bool Board::checkAmountFilled(int b[9][9], unsigned diff) {
	/*
		used for .puzzlify to check if board has minimum/maximum requirement for its difficulty
	*/
	int prob = 17 + ((5 - diff) * 10);
	int count = 0;
	bool done = false;

	for (unsigned i = 0; i < 9; i++) {
		for (unsigned j = 0; j < 9; j++) {
			if (b[i][j] != 0)
				count++;
			if (count >= prob + 10) {
				done = true;
				break;
			}
		}
		if (done) break;
	}
	
	if (count < prob || done) {
		for (unsigned i = 0; i < 9; i++)
			for (unsigned j = 0; j < 9; j++)
				puzzle[i][j] = 0;
		return false;
	}
	return true;
}