/*
 * SlidingWindowSearch.h
 *
 *  Created on: Oct 11, 2017
 *      Author: joe
 */

#ifndef SLIDINGWINDOWSEARCH_H_
#define SLIDINGWINDOWSEARCH_H_

#include "SlidingWindowPuzzle.h"
#include "GraphSearchResult.h"
#include <deque>


class SlidingWindowSearch {
public:
	SlidingWindowPuzzle startState;
	std::vector<SlidingWindowPuzzle> dfsExplored;
	SlidingWindowSearch();
	SlidingWindowSearch(SlidingWindowPuzzle);
	virtual ~SlidingWindowSearch();
	GraphSearchResult searchViaBFS(SlidingWindowPuzzle);
	GraphSearchResult searchViaDFS(SlidingWindowPuzzle);
	GraphSearchResult searchViaDLS(SlidingWindowPuzzle, int);
	GraphSearchResult searchViaIterativeDeepening(SlidingWindowPuzzle);


};


#endif /* SLIDINGWINDOWSEARCH_H_ */
