//============================================================================
// Name        : SlidingWindowProject.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include "SlidingWindowPuzzle.h"


// Represents internal state of the puzzle
/*
int** array;
int columnCount;
int rowCount;
*/

bool debug = false;
bool debugMoves = false;
bool debugParse = false;
bool debugRandomSearch = false;
bool debugRandomMoveGen = false;

using namespace std;

SlidingWindowPuzzle::SlidingWindowPuzzle() {
	solutionFound = false;
	array = new int*[10];
}

SlidingWindowPuzzle::SlidingWindowPuzzle(string fileName) {

	if(debugParse) {
		cout << "Opening: " << fileName << endl;

	}
	ifstream infile(fileName);


	std::string line;
	getline(infile, line);

	std::string tokenRow, tokenColumn;
	std::stringstream ss(line);

	std::getline(ss, tokenColumn, ',');
	std::getline(ss, tokenRow, ',');

	columnCount = stoi(tokenColumn);
	rowCount = stoi(tokenRow);

	array = new int*[rowCount];

	// Initialize array

	for(int i = 0; i < rowCount; i++) {
		array[i] = new int[columnCount];
	}

	for(int i = 0; i < rowCount; i++) {
		getline( infile, line );
		if(debugParse) {
			cout << "Processing line: " << line << endl;
		}
		std::stringstream linestream(line);

		for(int j = 0; j < columnCount; j++) {
			std::string token;
			std::getline(linestream, token, ',');
			if(debugParse) {
				cout << "Processing token: " << token << endl;
			}
			int value = stoi(token);
			array[i][j] = value;
		}
	}

	infile.close();

	solutionFound = false;

	if(debugParse) {
		printGameBoardState();
	}

	//normalize();

}


SlidingWindowPuzzle::SlidingWindowPuzzle(int cc, int rc, int** inputState, vector<Move> solns) {
	columnCount = cc;
	rowCount = rc;

	array = cloneState(inputState);

	solutionFound = false;
	for(Move m : solns) {
		foundSolutionMoves.push_back(m);
	}

	//normalize();

}



SlidingWindowPuzzle::~SlidingWindowPuzzle() {
	// TODO Auto-generated destructor stub
	//delete array;

	/*
	for(int i = 0; i < rowCount; i++) {
		delete[] array[i];
	}

	delete[] array;
	*/

}


int distanceOfMasterBrickToGoal(int** array) {

	return 0;
}

int** SlidingWindowPuzzle::cloneState(int** sourceArray) {
	int** returnArray = new int*[rowCount];

	for(int i = 0; i < rowCount; i++) {
		returnArray[i] = new int[columnCount];
	}

	for(int i = 0; i < rowCount; i++) {
		for(int j = 0; j < columnCount; j++) {
			returnArray[i][j] = (int) sourceArray[i][j];
		}
	}

	return returnArray;
}

void SlidingWindowPuzzle::printGameBoardState() {

	cout << columnCount << "," << rowCount << "," << endl;

	for(int i = 0; i < rowCount; i++) {
		for(int j = 0; j < columnCount; j++) {
			cout << array[i][j] << ",";
		}
		cout << endl;
	}
}


bool SlidingWindowPuzzle::isComplete() {
	for(int i = 0; i < rowCount; i++) {
		for(int j = 0; j < columnCount; j++) {
			if (array[i][j] == -1) {
				return false;
			}
		}
	}
	return true;
}





bool SlidingWindowPuzzle::isLegalArrayAccess(int rowIdx, int colIdx) {
	if ((rowIdx <= rowCount -1 && rowIdx >= 0) && (colIdx <= columnCount - 1 && colIdx >= 0)) {
		return true;
	}
	else {
		std::cout << "Illegal access, row: " << rowIdx << " col " << colIdx << std::endl;
	}
	return false;
}

bool SlidingWindowPuzzle::squareLeftIsFree(int pieceId, int rowId, int columnId) {
	if(!isLegalArrayAccess(rowId, columnId - 1)) {
		std::cout << "Illegal array access from squareLeftIsFree" << std::endl;
	}
	return array[rowId][columnId -1] == 0 || (pieceId == 2 && array[rowId][columnId -1] == -1);
}

bool SlidingWindowPuzzle::squareRightIsFree(int pieceId, int rowId, int columnId) {
	if(!isLegalArrayAccess(rowId, columnId + 1)) {
		std::cout << "Illegal array access from squareRightIsFree" << std::endl;
	}
	return array[rowId][columnId + 1] == 0 || (pieceId == 2 && array[rowId][columnId + 1] == -1);
}

bool SlidingWindowPuzzle::squareAboveIsFree(int pieceId, int rowId, int columnId) {
	if(!isLegalArrayAccess(rowId - 1,columnId)) {
		std::cout << "Illegal array access from squareAboveIsFree" << std::endl;
	}
	return array[rowId-1][columnId] == 0 || (pieceId == 2 && array[rowId-1][columnId] == -1);
}

bool SlidingWindowPuzzle::squareBelowIsFree(int pieceId, int rowId, int columnId) {
	if(!isLegalArrayAccess(rowId + 1,columnId)) {
		std::cout << "Illegal array access from squareBelowIsFree" << std::endl;
	}
	return array[rowId+1][columnId] == 0 || (pieceId == 2 && array[rowId+1][columnId] == -1);
}


bool SlidingWindowPuzzle::canMoveUp(int pieceId) {

	for(int i = 1; i < rowCount; i++) {

		for(int j = 0; j < columnCount; j++) {

			if(array[i][j] == pieceId) {

				if(!squareAboveIsFree(pieceId, i,j)) {
					return false;
				}

				// Now go left to right along top edge of piece, checking above
				for(int k = j + 1; k < columnCount; k++) {
					if(array[i][k] == pieceId && !squareAboveIsFree(pieceId,i,k)) {
						return false;
					}
				}

				return true;
			}
		}
	}
	return false;
}

bool SlidingWindowPuzzle::canMoveDown(int pieceId) {

	for(int i = rowCount - 1; i > 0 ; i--) {

		for(int j = 0; j < columnCount; j++) {

			if(array[i][j] == pieceId) {

				if(!squareBelowIsFree(pieceId, i,j)) {
					return false;
				}

				// Now go left to right along bottom edge of piece, checking below
				for(int k = j + 1; k < columnCount; k++) {
					if(array[i][k] == pieceId && !squareBelowIsFree(pieceId, i,k)) {
						return false;
					}
				}

				return true;
			}
		}
	}
	return false;
}

bool SlidingWindowPuzzle::canMoveLeft(int pieceId) {

	for(int i = 0; i < rowCount; i++) {

		for(int j = 1; j < columnCount; j++) {

			if(array[i][j] == pieceId) {

				if(!squareLeftIsFree(pieceId, i,j)) {
					return false;
				}

				// Now go down left edge of piece, row by row, checking left movement
				for(int k = i + 1; k < rowCount; k++) {
					if(!isLegalArrayAccess(k,j)) {
						std::cout << "Illegal array access from canMoveLeft" << std::endl;
					}
					if(array[k][j] == pieceId && !squareLeftIsFree(pieceId, k,j)) {
						return false;
					}
				}

				return true;
			}
		}
	}
	return false;
}

bool SlidingWindowPuzzle::canMoveRight(int pieceId) {

	for(int i = 0; i < rowCount; i++) {

		for(int j = columnCount - 1; j > 0; j--) {

			if(array[i][j] == pieceId) {

				if(!squareRightIsFree(pieceId, i,j)) {
					return false;
				}

				// Now go down right edge of piece, row by row, checking right movement
				for(int k = i + 1; k < rowCount; k++) {
					if(!isLegalArrayAccess(k,j)) {
						std::cout << "Illegal array access from canMoveRight" << std::endl;
					}
					if(array[k][j] == pieceId && !squareRightIsFree(pieceId, k,j)) {
						return false;
					}
				}

				return true;
			}
		}
	}
	return false;
}

vector<Move> SlidingWindowPuzzle::getLegalMovesForPiece(int pieceId) {
	vector<Move> legalMoves;

	bool moveUpPossible = canMoveUp(pieceId);
	bool moveDownPossible = canMoveDown(pieceId);
	bool moveLeftPossible = canMoveLeft(pieceId);
	bool moveRightPossible = canMoveRight(pieceId);

	if(moveUpPossible) {
		Move moveUp(pieceId,"up");
		legalMoves.push_back(moveUp);
	}

	if(moveDownPossible) {
		Move moveDown(pieceId,"down");
		legalMoves.push_back(moveDown);
	}

	if(moveLeftPossible) {
		Move moveLeft(pieceId,"left");
		legalMoves.push_back(moveLeft);
	}

	if(moveRightPossible) {
		Move moveRight(pieceId,"right");
		legalMoves.push_back(moveRight);
	}

	return legalMoves;
}

set<int> SlidingWindowPuzzle::getAllPieceNumbers() {
	set<int> pieceNums;
	for(int i = 0; i < rowCount; i++) {

		for(int j = 0; j < columnCount; j++) {
			int cellVal = array[i][j];
			if(cellVal >= 2) {
				pieceNums.insert(cellVal);
			}
		}
	}
	return pieceNums;
}

vector<Move> SlidingWindowPuzzle::getAllLegalMoves() {

	// Find empty spaces on board, and see who can fit in

	vector<Move> legalMoves;
	set<int> pieceNums = getAllPieceNumbers();
	for(int pieceNum : pieceNums) {
		vector<Move> movesForPiece = getLegalMovesForPiece(pieceNum);
		for(Move m : movesForPiece) {
			legalMoves.push_back(m);
		}
	}

	return legalMoves;
}


void SlidingWindowPuzzle::applyMoveUp(Move m) {
	// Move the bottom-most row of the piece above the top of the piece

	int** originalState = cloneState(array);

	int pieceId = m.pieceId;

	// First, find the bottom-most row and clear it
	for(int i = rowCount - 1; i > 0; i--) {
		for(int j = 0; j < columnCount; j++) {
			if(array[i][j] == pieceId) {

				if(debugMoves) {
					cout << "Bottom row for: " << pieceId << " is: " << i << endl;
				}
				// We've found bottom row; now scan left to right, setting all entries to 0
				array[i][j] = 0;

				// Now go left to right across the columns
				for(int k = j + 1; k < columnCount; k++) {
					if(array[i][k] == pieceId) {
						array[i][k] = 0;
					}
				}
				goto findTopRow;

			}
		}
	}

	// Having cleared bottom row, find index of top-most row
	findTopRow:
		int topMostRow = -1;
		for(int i = 1; i < rowCount; i++) {
			for(int j = 0; j < columnCount; j++) {
				if(originalState[i][j] == pieceId) {
					topMostRow = i;
					goto writePieceRowAboveTopRow;
				}
			}
		}

	writePieceRowAboveTopRow:
		if(debugMoves) {
			cout << "Top row for piece: " << pieceId << " is: " << topMostRow << endl;
		}
		for(int j = 0; j < columnCount; j++) {
			if(originalState[topMostRow][j] == pieceId) {
				array[topMostRow-1][j] = pieceId;
			}
		}
		for(int i = 0; i < rowCount; i++) {
			delete[] originalState[i];
		}

		delete[] originalState;
}

void SlidingWindowPuzzle::applyMoveDown(Move m) {
	// Move the top-most row of the piece below the bottom row of the piece

	int** originalState = cloneState(array);

	int pieceId = m.pieceId;


	// First, find the top-most row and clear it
	for(int i = 0; i < rowCount; i++) {
		for(int j = 0; j < columnCount; j++) {
			if(array[i][j] == pieceId) {

				// We've found top row; set all cells with our piece ID to 0

				array[i][j] = 0;

				// Now go left to right across the columns
				for(int k = j + 1; k < columnCount; k++) {
					if(array[i][k] == pieceId) {
						array[i][k] = 0;
					}
				}
				goto findBottomRow;
			}
		}
	}

	// Next, find index of bottom-most row and write a piece row below it
	findBottomRow:
		int bottomMostRow = -1;
		for(int i = rowCount - 1; i > -1; i--) {
			for(int j = 0; j < columnCount; j++) {
				if(originalState[i][j] == pieceId) {
					bottomMostRow = i;
					goto writePieceRowBelowBottomMostRow;
				}
			}
		}

	writePieceRowBelowBottomMostRow:
		if(debugMoves) {
			cout << "Bottom row for piece: " << pieceId << " is: " << bottomMostRow << endl;
		}
		for(int j = 0; j < columnCount; j++) {
			if(!isLegalArrayAccess(bottomMostRow,j)) {
				std::cout << "Current puzzle state: " << std::endl;
				printGameBoardState();
				std::cout << "Trying to move: " << m.pieceId << " direction: " << m.direction << std::endl;
				std::cout << "applyMoveDown tried to access row: " << bottomMostRow << " and column: " << j << std::endl;
			}
			if(originalState[bottomMostRow][j] == pieceId) {
				array[bottomMostRow+1][j] = pieceId;
			}
		}
		for(int i = 0; i < rowCount; i++) {
			delete[] originalState[i];
		}

		delete[] originalState;

	}

void SlidingWindowPuzzle::applyMoveLeft(Move m) {
	// Move the right-most column of the piece to the left of the left-most column of the piece

	int** originalState = cloneState(array);

	int pieceId = m.pieceId;


	// First, find the right-most column and clear it
	for(int i = 0; i < rowCount; i++) {

		for(int j = columnCount; j > 0; j--) {
			if(array[i][j] == pieceId) {

				if(debugMoves) {
					cout << "Clearing first index: " << i << ", " << j << endl;
				}
				array[i][j] = 0;
				// Now go down the rows
				for(int k = i + 1; k < rowCount; k++) {
					if(array[k][j] == pieceId) {
						if(debugMoves) {
							cout << "Clearing index: " << k << ", " << j << endl;
						}
						array[k][j] = 0;
					}
				}

				// We've cleared right-most column; now find left-most column
				goto findLeftMostColumn;

			}
		}
	}

	findLeftMostColumn:

	// Next, find index of left-most column and write a piece row left of it

	int leftMostColumn = -1;
	for(int i = 0; i < rowCount; i++) {
		for(int j = 0; j < columnCount; j++) {
			if(originalState[i][j] == pieceId) {
				leftMostColumn = j;
				goto writePieceRowLeftOfLeftMostColumn;
			}
		}
	}

	writePieceRowLeftOfLeftMostColumn:
	if(leftMostColumn == -1) {
		cout << "Couldn't find left-most column" << endl;
	}
	if(debugMoves) {
		cout << "Left-most column for piece: " << pieceId << " is: " << leftMostColumn << endl;
	}
	for(int i = 0; i < rowCount; i++) {
		if(originalState[i][leftMostColumn] == pieceId) {
			array[i][leftMostColumn - 1] = pieceId;
		}
	}
	for(int i = 0; i < rowCount; i++) {
		delete[] originalState[i];
	}

	delete[] originalState;

}

void SlidingWindowPuzzle::applyMoveRight(Move m) {
	// Move the left-most column of the piece to the right of the right-most column of the piece

	int** originalState = cloneState(array);

	int pieceId = m.pieceId;


	// First, find the left-most column and clear it
	for(int i = 0; i < rowCount; i++) {

		for(int j = 0; j < columnCount; j++) {
			if(array[i][j] == pieceId) {

				array[i][j] = 0;

				// Now scan down the rows
				for(int k = i + 1; k < rowCount; k++) {
					if(array[k][j] == pieceId) {
						array[k][j] = 0;
					}
				}

				// We've cleared left-most column; now find right-most column
				goto findRightMostColumn;

			}
		}
	}

	findRightMostColumn:

	// Next, find index of right-most column and write a piece row right of it
	int rightMostColumn = -1;
	for(int i = 1; i < rowCount; i++) {

		for(int j = columnCount; j > -1; j--) {
			if(originalState[i][j] == pieceId) {
				rightMostColumn = j;
				goto writePieceRowRightOfRightMostColumn;
			}
		}
	}

	writePieceRowRightOfRightMostColumn:

	if(debugMoves) {
		cout << "Right-most column for piece: " << pieceId << " is: " << rightMostColumn << endl;
	}
	for(int i = 0; i < rowCount; i++) {
		if(originalState[i][rightMostColumn] == pieceId) {
			array[i][rightMostColumn + 1] = pieceId;
		}
	}
	for(int i = 0; i < rowCount; i++) {
		delete[] originalState[i];
	}

	delete[] originalState;

}

void SlidingWindowPuzzle::applyMove(Move m) {

	foundSolutionMoves.push_back(m);

	if(debugMoves) {
		std::cout << "Making move of: " << m.pieceId << " to: " << m.direction << std::endl;
	}

	if(m.direction.compare("up") == 0) {
		applyMoveUp(m);
	}
	else if(m.direction.compare("down") == 0) {
		applyMoveDown(m);
	}
	else if(m.direction.compare("left") == 0) {
		applyMoveLeft(m);
	}
	else if(m.direction.compare("right") == 0) {
		applyMoveRight(m);
	}

}



SlidingWindowPuzzle SlidingWindowPuzzle::applyMoveCloning(Move m) {

	SlidingWindowPuzzle clone(columnCount,rowCount,array,foundSolutionMoves);
	clone.applyMove(m);

	return clone;

}

bool SlidingWindowPuzzle::isEqualTo(SlidingWindowPuzzle swp2) {

	// Check both puzzles normalized

	for(int i = 0; i < rowCount; i++) {
		for(int j = 0; j < columnCount; j++) {
			if(array[i][j] != swp2.array[i][j]) {
				return false;
			}
		}
	}
	return true;
}

int SlidingWindowPuzzle::indexOfBrick(int brickNum) {
	// First,

	int totalSquaresInGrid = rowCount * columnCount;
	int* lowestCellForBrickNum = new int[totalSquaresInGrid];

	for(int i = 0; i < rowCount; i++) {
		for(int j = 0; j < columnCount; j++) {
			int cellNum = i * columnCount + j;
			lowestCellForBrickNum[cellNum] = 2 * totalSquaresInGrid;
		}
	}


	for(int i = 0; i < rowCount; i++) {
		for(int j = 0; j < columnCount; j++) {
			int cellNum = i * columnCount + j;
			int brickNum = array[i][j];
			if (cellNum < lowestCellForBrickNum[brickNum]) {
				lowestCellForBrickNum[brickNum] = cellNum;
			}

			cout << "Cellnum is:" << cellNum << endl;
			cout << "At that location we find: " << array[i][j] << endl;
		}
	}

	return lowestCellForBrickNum[brickNum];
}

void SlidingWindowPuzzle::swapIdx(int oldIdx, int newIdx) {
	for(int i = 0; i < rowCount; i++) {
		for(int j = 0; j < columnCount; j++) {
			if(array[i][j] == oldIdx) {
				array[i][j] = newIdx;
			}
			else if(array[i][j] == newIdx) {
				array[i][j] = oldIdx;
			}
		}
	}
}

SlidingWindowPuzzle SlidingWindowPuzzle::normalize() {

	int** clonedState = cloneState(array);

	int nextIdx = 3;
	for(int i = 0; i < rowCount; i++) {
		for(int j = 0; j < columnCount; j++) {
			if(clonedState[i][j] == nextIdx) {
				nextIdx++;
			}
			else if(clonedState[i][j] > nextIdx) {
				swapIdx(clonedState[i][j],nextIdx);
				nextIdx++;
			}
		}
	}
	return SlidingWindowPuzzle(columnCount, rowCount, clonedState, foundSolutionMoves);
}

Move SlidingWindowPuzzle::getRandomMove() {
	vector<Move> allMoves = getAllLegalMoves();
		// Moves size
		int movesSize = (int)allMoves.size();
		if(movesSize == 0) {
			return Move(-2,"down");
		}

		int randomIdx = rand() % movesSize;
		if(debugRandomMoveGen) {
			cout << "Rand idx: " << randomIdx << " out of: " << movesSize << " options " << endl;
		}

		Move randomMove = allMoves.at(randomIdx);
		return randomMove;
}

Move SlidingWindowPuzzle::randomWalk(int N) {


	if(N == 0) {
		if(debugRandomSearch) {
			cout << "Tried all moves, final state: " << endl;
			printGameBoardState();
		}

		Move noMove(-2,"down");
		return noMove;
	}

	Move randomMove = getRandomMove();

	if(debugRandomSearch) {
		cout << "Making move: " << randomMove.pieceId << " to: " << randomMove.direction << endl;
	}

	applyMove(randomMove);

	if(isComplete()) {
		solutionFound = true;
		if(debugRandomSearch) {
			cout << "SOLVED!" << " and length is: " << foundSolutionMoves.size() << endl;
			cout << "(" << randomMove.pieceId << "," << randomMove.direction << ")" << endl;
		}
		return randomMove;
	}
	else {
		Move retMove = randomWalk(N - 1);
		if(retMove.pieceId != -2) {
			if(debugRandomSearch) {
				cout << "(" << randomMove.pieceId << "," << randomMove.direction << ")" << endl;
			}
		}
		return retMove;
	}

}

