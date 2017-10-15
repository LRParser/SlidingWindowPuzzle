/*
 * Move.h
 *
 *  Created on: Oct 9, 2017
 *      Author: joe
 */

#ifndef MOVE_H_
#define MOVE_H_

#include <string>

class Move {
public:
	int pieceId;
	std::string direction;
	Move(int, std::string);
	virtual ~Move();

};

#endif /* MOVE_H_ */
