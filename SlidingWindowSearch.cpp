/*
 * SlidingWindowSearch.cpp
 *
 *  Created on: Oct 11, 2017
 *      Author: joe
 */

#include "SlidingWindowSearch.h"
#include <chrono>
#include <queue>

bool debugBFS = false;
bool debugDFS = false;
bool debugAStar = false;
std::vector<SlidingWindowPuzzle> dfsExplored;
std::vector<SlidingWindowPuzzle> aStarExplored;


bool comparePuzzles(const SlidingWindowPuzzle & l, const SlidingWindowPuzzle & r) //(2)
{
	int thisGScore = l.getDistanceOfMasterBrickToGoal();
	int thisHScore = (int) l.foundSolutionMoves.size();
	int thisFScore = thisGScore + thisHScore;

	int otherGScore = r.getDistanceOfMasterBrickToGoal();
	int otherHScore = (int) r.foundSolutionMoves.size();
	int otherFScore = otherGScore + otherHScore;

    return thisFScore < otherFScore;

}

SlidingWindowSearch::SlidingWindowSearch() {

}


SlidingWindowSearch::~SlidingWindowSearch() {
	// TODO Auto-generated destructor stub
}


GraphSearchResult SlidingWindowSearch::searchViaBFS(SlidingWindowPuzzle startState) {

	std::deque<SlidingWindowPuzzle> frontier;
	std::vector<SlidingWindowPuzzle> explored;

	if(startState.isComplete()) {
		if(debugBFS) {
			std::cout << "Start state has solution" << std::endl;
		}
		return GraphSearchResult(0,0.0f,startState.foundSolutionMoves);
	}

	frontier.push_back(startState);
	int i = 0;
	while(true) {
		if(debugBFS) {
			std::cout << "Starting iteration: " << i << std::endl;
		}
		i++;
		if(frontier.empty()) {
			return GraphSearchResult(i,0.0f,std::vector<Move>());
		}
		else {
			SlidingWindowPuzzle currentPuzzle = frontier.front();
			frontier.pop_front();

			if(debugBFS) {
				currentPuzzle.printGameBoardState();
			}
			explored.push_back(currentPuzzle.normalize());

					for(Move m : currentPuzzle.getAllLegalMoves()) {
						if(debugBFS) {
							std::cout << "Looking at move: " << m.pieceId << " to: " << m.direction << std::endl;
						}
						SlidingWindowPuzzle nextState = currentPuzzle.applyMoveCloning(m);
						bool inExplored = false;

						SlidingWindowPuzzle nextStateNormalized = nextState.normalize();


						for(SlidingWindowPuzzle exploredState : explored) {
							if(nextStateNormalized.isEqualTo(exploredState)) {
								if(debugBFS) {
									std::cout << "State in explored" << std::endl;
								}
								inExplored = true;
							}
						}

						bool inFrontier = false;

						for(SlidingWindowPuzzle frontierState : frontier) {
							if(nextState.isEqualTo(frontierState)) {
								if(debugBFS) {
									std::cout << "State in frontier" << std::endl;
								}
								inFrontier = true;
							}
						}


						if(!inExplored && !inFrontier) {
							if(nextState.isComplete()) {
								if(debugBFS) {
									std::cout << "Found a solution via BFS" << std::endl;
								}
								GraphSearchResult gsr(i,0.0f,nextState.foundSolutionMoves);
								return gsr;
							}
							frontier.push_back(nextState);
						}
			}
		}
	}
	return GraphSearchResult(i,0.0f,std::vector<Move>());
}

GraphSearchResult SlidingWindowSearch::searchViaDFS(SlidingWindowPuzzle swp) {

	if(swp.isComplete()) {
		GraphSearchResult gsr(dfsExplored.size(),0.0f,swp.foundSolutionMoves);
		return gsr;
	}
	else {

		for(SlidingWindowPuzzle explored : dfsExplored) {
			if(swp.isEqualTo(explored)) {
				if(debugDFS) {
					std::cout << "Already seen this state" << std::endl;
				}
				GraphSearchResult gsr(0,0.0f,std::vector<Move>());
				return gsr;
			}
			else {
				if(debugDFS) {
					std::cout << "Already saw: " << std::endl;
					explored.printGameBoardState();
					std::cout << "But this is: " << std::endl;
					swp.printGameBoardState();
				}

			}
		}

		// Add it to the explored list since we already checked if it's a solution
		dfsExplored.push_back(swp);

		for(Move m : swp.getAllLegalMoves()) {
			if(debugDFS) {
				std::cout << "Looking at moving: " << m.pieceId << " direction: " << m.direction << std::endl;
			}
			SlidingWindowPuzzle swpNew = swp.applyMoveCloning(m);
			GraphSearchResult gsr = searchViaDFS(swpNew);
			std::vector<Move> solutionMoves = gsr.foundSolutionMoves;
			if(solutionMoves.size() > 0) {
				GraphSearchResult gsr(dfsExplored.size(),0.0f,solutionMoves);
				return gsr;
			}
		}
	}
	GraphSearchResult gsr(0,0.0f,std::vector<Move>());
	return gsr;
}
std::vector<SlidingWindowPuzzle> dlsExplored;

GraphSearchResult SlidingWindowSearch::searchViaDLS(SlidingWindowPuzzle swp, int depthLimit) {

	//std::cout << "Searching limit: " << depthLimit << std::endl;


	if(swp.isComplete()) {
		return GraphSearchResult(0,0.0f,swp.foundSolutionMoves);
	}


	for(SlidingWindowPuzzle explored : dlsExplored) {
		if(swp.isEqualTo(explored)) {
			GraphSearchResult gsr(0,0.0f,std::vector<Move>());
			return gsr;
		}
	}


	// Add it to the explored list since we already checked if it's a solution
	dlsExplored.push_back(swp);

	if(depthLimit == 0) {
		return GraphSearchResult(0,0.0f,std::vector<Move>());
	}
	else {
		int numNodesExplored = 0;
		for (Move m : swp.getAllLegalMoves()) {
			SlidingWindowPuzzle newState = swp.applyMoveCloning(m);
			numNodesExplored++;
			GraphSearchResult gsr = searchViaDLS(newState, depthLimit -1);

			std::vector<Move> foundMoves = gsr.foundSolutionMoves;
			numNodesExplored += gsr.nodesExpanded;
			if(foundMoves.size() > 0) {
				return GraphSearchResult(numNodesExplored,0.0f,foundMoves);
			}
		}
	}


	return GraphSearchResult(0,0.0f,std::vector<Move>());
}

// Searched via iterative deepening to a depth of 10
GraphSearchResult SlidingWindowSearch::searchViaIterativeDeepening(SlidingWindowPuzzle swp) {

	int numNodesExplored = 0;
	for(int i = 0; i < 1000; i++) {
		GraphSearchResult gsr = searchViaDLS(swp,i);
		dlsExplored.clear();
		std::vector<Move> foundSolution = gsr.foundSolutionMoves;
		numNodesExplored += gsr.nodesExpanded;
		if(foundSolution.size() > 0) {
			return gsr;
		}
	}

	return GraphSearchResult(0,0.0f,std::vector<Move>());

}

GraphSearchResult SlidingWindowSearch::searchViaAStar(SlidingWindowPuzzle swp) {

	if(swp.isComplete()) {
		GraphSearchResult gsr(aStarExplored.size(),0.0f,swp.foundSolutionMoves);
		return gsr;
	}
	else {

		for(SlidingWindowPuzzle explored : aStarExplored) {
			if(swp.isEqualTo(explored)) {
				if(debugAStar) {
					std::cout << "Already seen this state" << std::endl;
				}
				GraphSearchResult gsr(0,0.0f,std::vector<Move>());
				return gsr;
			}
			else {
				if(debugAStar) {
					std::cout << "Already saw: " << std::endl;
					explored.printGameBoardState();
					std::cout << "But this is: " << std::endl;
					swp.printGameBoardState();
				}

			}
		}

		// Add it to the explored list since we already checked if it's a solution
		aStarExplored.push_back(swp);


		std::vector<Move> moveList = swp.getAllLegalMoves();

		std::vector<SlidingWindowPuzzle> relatedPuzzles;

		for(Move m : moveList) {
			if(debugAStar) {
				std::cout << "Looking at moving: " << m.pieceId << " direction: " << m.direction << std::endl;
			}
			relatedPuzzles.push_back(swp.applyMoveCloning(m));
		}

	    std::sort(relatedPuzzles.begin(), relatedPuzzles.end(), comparePuzzles);

	    for(SlidingWindowPuzzle swpNew : relatedPuzzles) {
			GraphSearchResult gsr = searchViaAStar(swpNew);
			std::vector<Move> solutionMoves = gsr.foundSolutionMoves;
			if(solutionMoves.size() > 0) {
				GraphSearchResult gsr(aStarExplored.size(),0.0f,solutionMoves);
				return gsr;
			}
	    }

	}
	GraphSearchResult gsr(0,0.0f,std::vector<Move>());
	return gsr;
}



int main() {

	SlidingWindowPuzzle swp;
	SlidingWindowSearch sws;
	GraphSearchResult gsr;
	std::string level0FileName = "SBP-level0.txt";
	std::string level1FileName = "SBP-level1.txt";



	// PART 1 - Random Walk
	swp = SlidingWindowPuzzle(level0FileName);

	swp.randomWalk(3);
	std::vector<Move> solutionMoves = swp.foundSolutionMoves;

	// Show evolution of board thru time

	swp = SlidingWindowPuzzle(level0FileName);
	swp.printGameBoardState();
	for(Move m : solutionMoves) {
		std::cout << std::endl;
		std::cout << "(" << m.pieceId << "," << m.direction << ")" << std::endl;
		std::cout << std::endl;
		swp.applyMove(m);
		swp.printGameBoardState();
	}

	std::cout << "End of found Random Walk solution" << std::endl;

	// PART 2 - Breadth-First Search


	swp = SlidingWindowPuzzle(level1FileName);
	sws = SlidingWindowSearch();
	auto start = std::chrono::high_resolution_clock::now();
	gsr = sws.searchViaBFS(swp);
	auto stop = std::chrono::high_resolution_clock::now();
	std::vector<Move> bfsSol = gsr.foundSolutionMoves;
	swp = SlidingWindowPuzzle(level1FileName);

	for (Move m : bfsSol) {
		std::cout << "(" << m.pieceId << "," << m.direction << ")" << std::endl;
		swp.applyMove(m);
	}

	swp.printGameBoardState();
	std::chrono::duration<double> elapsed = stop - start;

	std::cout << "#" << gsr.nodesExpanded << " " << elapsed.count() << " " << bfsSol.size() << std::endl;

	std::cout << "End of found BFS solution" << std::endl;

	swp = SlidingWindowPuzzle(level1FileName);

	sws = SlidingWindowSearch();
	start = std::chrono::high_resolution_clock::now();
	gsr = sws.searchViaDFS(swp);
	stop = std::chrono::high_resolution_clock::now();

	std::vector<Move> dfsSol = gsr.foundSolutionMoves;

	for (Move m : dfsSol) {
		std::cout << "(" << m.pieceId << "," << m.direction << ")" << std::endl;
		swp.applyMove(m);
	}

	swp.printGameBoardState();

	elapsed = stop - start;
	std::cout << "#" << gsr.nodesExpanded << " " << elapsed.count() << " " << dfsSol.size() << std::endl;
	std::cout << "End of found DFS solution" << std::endl;
	dfsExplored.clear();

	// Now let's try IDS
	swp = SlidingWindowPuzzle(level1FileName);
	sws = SlidingWindowSearch();

	start = std::chrono::high_resolution_clock::now();
	gsr = sws.searchViaIterativeDeepening(swp);
	stop = std::chrono::high_resolution_clock::now();
	std::vector<Move> idsSol = gsr.foundSolutionMoves;

	for (Move m : idsSol) {
		std::cout << "(" << m.pieceId << "," << m.direction << ")" << std::endl;
		swp.applyMove(m);
	}

	swp.printGameBoardState();
	elapsed = stop - start;
	std::cout << "#" << gsr.nodesExpanded << " " << elapsed.count() << " " << idsSol.size() << std::endl;
	std::cout << "End of found IDS solution" << std::endl;

	// Now let's try AStar
	swp = SlidingWindowPuzzle(level1FileName);
	sws = SlidingWindowSearch();

	start = std::chrono::high_resolution_clock::now();
	gsr = sws.searchViaAStar(swp);
	stop = std::chrono::high_resolution_clock::now();
	std::vector<Move> aStarSol = gsr.foundSolutionMoves;

	for (Move m : aStarSol) {
		std::cout << "(" << m.pieceId << "," << m.direction << ")" << std::endl;
		swp.applyMove(m);
	}

	swp.printGameBoardState();
	elapsed = stop - start;
	std::cout << "#" << gsr.nodesExpanded << " " << elapsed.count() << " " << aStarSol.size() << std::endl;
	std::cout << "End of found AStar solution" << std::endl;

	return 0;

}
