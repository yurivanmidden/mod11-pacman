//
// Created by yuri on 20-3-18.
//

#ifndef PACMAN_CHARACTER_H
#define PACMAN_CHARACTER_H

#include "GameObjectStruct.h"
#include "CharacterAI.h"
//#include <list>

class Character {
private:
    GameObjectStruct object;
	CharacterAI AI;
//	static std::list<GameObjectStruct*> objectList;//List to track characters

public:
	Character() {}//Default constructor for compiler
	Character(GameObjectStruct input);

	//Character Getters
	Direction getDirection() { return object.dir; }
	int getX() { return object.x; }
	int getY() { return object.y; }
	Type getType() { return object.type; }
	GameObjectStruct getStruct() { return object; }//Get the whole object

	//Character Setters
	void setX(int X) { object.x = X; }
	void setY(int Y) { object.y = Y; }
	void setType(Type type) { object.type = type; }//Use for changing ghost
	void setDirection(Direction direction) { object.dir = direction; }

	void setDirectionAI() { AI.setDirection(object); }

	

//	std::list<GameObjectStruct*> getList() { return objectList; } //Function to get list of packages

};


#endif //PACMAN_CHARACTER_H
