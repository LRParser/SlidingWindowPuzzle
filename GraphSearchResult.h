/*
 * GraphSearchResult.h
 *
 *  Created on: Oct 14, 2017
 *      Author: joe
 */

#ifndef GRAPHSEARCHRESULT_H_
#define GRAPHSEARCHRESULT_H_
#include <vector>
#include "Move.h"

class GraphSearchResult {
public:
	GraphSearchResult();
	GraphSearchResult(int,float,std::vector<Move>);
	virtual ~GraphSearchResult();
	int nodesExpanded;
	float timeElapsed;
	std::vector<Move> foundSolutionMoves;

};

#endif /* GRAPHSEARCHRESULT_H_ */
