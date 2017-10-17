# SlidingWindowPuzzle
SlidingWindowPuzzle

## Overview

This program implements in C++11 a number of search heuristics. The major classes are:

SlidingWindowPuzzle - holds the 'state' for a puzzle, as well as functions for manipulating the pieces

SlidingWindowSearch - this holds the 'algorithms' for solving a puzzle, such as breadth-first search, depth-first search, etc

Move - a container holding move information (i.e., pieceId = 4, direction = 'up')

GraphSearchResult - a container to hold information about the number of nodes accessed and the found solution

## Building

A Makefile is provided with the project. Running the command:

```
make all
```

Will compile the solution to an executable in the same directory, 'hw1'

## Running


It can be run with:

./hw1

As per instructions it is hard-coded to first process SBP-level0.txt and then SBP-level1.txt

## Example output

Example output is provided in the output-hw1.txt file. It first shows a random walk on SBP-level0, then BFS, DFS, IDS, and AStar search on SBP-level1

This was run on tux at:
```
cd ~/jth82/SlidingWindowPuzzle
make all
./hw1 > output-hw1
```