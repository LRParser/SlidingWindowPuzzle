/*
 * SlidingWIndowPuzzle.h
 *
 *  Created on: Oct 11, 2017
 *      Author: joe
 */

#ifndef SLIDINGWINDOWPUZZLE_H_
#define SLIDINGWINDOWPUZZLE_H_
#include <stdlib.h>     /* srand, rand */
#include <time.h>

#include <iostream>
#include <fstream>
#include <set>
#include <vector>
#include <sstream>
#include <string>
#include <iterator>
#include <algorithm>
#include "Move.h"
#include "string.h"

class SlidingWindowPuzzle {
private:


	int** cloneState(int**);
	bool canMoveUp(int);
	bool canMoveDown(int);
	bool canMoveLeft(int);
	bool canMoveRight(int);
	std::vector<Move> getLegalMovesForPiece(int);
	std::set<int> getAllPieceNumbers();
	void applyMoveUp(Move);
	void applyMoveDown(Move);
	void applyMoveLeft(Move);
	void applyMoveRight(Move);

	void swapIdx(int,int);

public:
	int** array;
	int columnCount;
	int rowCount;
	std::vector<Move> foundSolutionMoves;
	bool solutionFound;
	SlidingWindowPuzzle();
	SlidingWindowPuzzle(std::string);
	SlidingWindowPuzzle(int, int, int**,std::vector<Move>);
	bool isEqualTo(SlidingWindowPuzzle swp2);
	SlidingWindowPuzzle applyMoveCloning(Move);
	virtual ~SlidingWindowPuzzle();
	Move getRandomMove();
	int** getState(void);
	bool isComplete(void);
	int indexOfBrick(int);
	SlidingWindowPuzzle normalize(void);
	void printGameBoardState(void);
	void randomMove(void);
	std::vector<Move> getAllLegalMoves();
	std::vector<Move> getSolutionMoves(void);
	void applyMove(Move);
	Move randomWalk(int);
	bool isLegalArrayAccess(int,int);
	bool squareLeftIsFree(int,int,int);
	bool squareRightIsFree(int,int,int);
	bool squareAboveIsFree(int,int,int);
	bool squareBelowIsFree(int,int,int);
	int getDistanceOfMasterBrickToGoal(void) const;
	/*
	bool operator<(const SlidingWindowPuzzle& otherPuzzle) {
		int thisGScore = getDistanceOfMasterBrickToGoal();
		int thisHScore = (int) foundSolutionMoves.size();
		int thisFScore = thisGScore + thisHScore;

		int otherGScore = otherPuzzle.getDistanceOfMasterBrickToGoal();
		int otherHScore = (int) otherPuzzle.foundSolutionMoves.size();
		int otherFScore = otherGScore + otherHScore;


	    return thisFScore < otherFScore;
	}

	bool operator>(const SlidingWindowPuzzle& otherPuzzle) {
		int thisGScore = getDistanceOfMasterBrickToGoal();
		int thisHScore = (int) foundSolutionMoves.size();
		int thisFScore = thisGScore + thisHScore;

		int otherGScore = otherPuzzle.getDistanceOfMasterBrickToGoal();
		int otherHScore = (int) otherPuzzle.foundSolutionMoves.size();
		int otherFScore = otherGScore + otherHScore;

	    return thisFScore > otherFScore;
	}
	*/

};


//bool comparePuzzles(const SlidingWindowPuzzle, const SlidingWindowPuzzle);




#endif /* SLIDINGWINDOWPUZZLE_H_ */
