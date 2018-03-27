#ifndef PACMAN_CHARACTERAI_H
#define PACMAN_CHARACTERAI_H

#include "GameObjectStruct.h"
#include <stdlib.h>//Random number generator

class CharacterAI {
private:
	Type type;

public:
	CharacterAI() {}//Default constructor for compiler
	CharacterAI(Type input) { type = input; }
	
	//Choose Direction AI
	void setDirection(GameObjectStruct &object) {
		if (type != PACMAN) {//temp
			int nextDir = rand() % 4;
			switch (nextDir) {
			case 0: object.dir = UP;
				break;
			case 1: object.dir = DOWN;
				break;
			case 2: object.dir = LEFT;
				break;
			case 3: object.dir = RIGHT;
				break;
			}
		}
	}
};

#endif //PACMAN_CHARACTERAI_H