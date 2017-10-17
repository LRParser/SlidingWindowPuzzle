#ifndef SLIDINGWINDOWHEURISTIC_H_
#define SLIDINGWINDOWHEURISTIC_H_

#include "SlidingWindowPuzzle.h"

class SlidingWindowHeuristic {
public:
	int fScore;
	int gScore;
	int hScore;
	SlidingWindowPuzzle swp;
	SlidingWindowHeuristic();
	SlidingWindowHeuristic(SlidingWindowPuzzle);
	virtual ~SlidingWindowHeuristic();
	void computeHeuristic();

};


#endif /* SLIDINGWINDOWHEURISTIC_H_ */
