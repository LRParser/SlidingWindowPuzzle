/*
 * GraphSearchResult.cpp
 *
 *  Created on: Oct 14, 2017
 *      Author: joe
 */

#include "GraphSearchResult.h"

GraphSearchResult::GraphSearchResult() {
	nodesExpanded = 0;
	timeElapsed = 0.0f;
	foundSolutionMoves = std::vector<Move>();
	// TODO Auto-generated constructor stub

}

GraphSearchResult::GraphSearchResult(int nodesExpanded_, float timeElapsed_, std::vector<Move> foundSolutionMoves_) {
	nodesExpanded = nodesExpanded_;
	timeElapsed = timeElapsed_;
	foundSolutionMoves = foundSolutionMoves_;
	// TODO Auto-generated constructor stub

}

GraphSearchResult::~GraphSearchResult() {
	// TODO Auto-generated destructor stub
}

