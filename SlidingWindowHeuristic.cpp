#include "SlidingWindowHeuristic.h"

SlidingWindowHeuristic::SlidingWindowHeuristic() {
	fScore = 0;
	gScore = 0;
	hScore = 0;
}

SlidingWindowHeuristic::SlidingWindowHeuristic(SlidingWindowPuzzle sw) {
	fScore = 0;
	gScore = 0;
	hScore = 0;
	swp = sw;
}


SlidingWindowHeuristic::~SlidingWindowHeuristic() {
	// TODO Auto-generated destructor stub
}

void SlidingWindowHeuristic::computeHeuristic() {



}
