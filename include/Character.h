//==============================================================
// Filename : Character.h
// Authors : Brian Chang (s1387693), Yuri van Midden (s1496182)
// Version :
// License :
// Description : Header file for Character class
//==============================================================

#ifndef PACMAN_CHARACTER_H
#define PACMAN_CHARACTER_H

#include <vector>

#include "GameObjectStruct.h"
#include "CharacterAI.h"

class Character {
private:
    GameObjectStruct object;
	CharacterAI AI;

public:
	Character() {}//Default constructor for compiler
	Character::Character(GameObjectStruct inputObject, std::vector<int> entrance, std::vector<int> corner);//Constructor
	void reinit();//Reinitialize character

	//AI getters and setters
	Type getName() { return AI.getName(); }//Used for collision check

	State getState() { return AI.getState(); }
	void setState(State state) { AI.setState(state); }

	bool getEntryAllowed() { return AI.getEntryAllowed(); }

	void setScatter(bool scatter) { AI.setScatter(scatter); }

	bool getVulnerable() { return AI.getVulnerable(); }
	void setVulnerable(bool vulnerable) { AI.setVulnerable(vulnerable); }

	void setDirectionAI(std::vector<Direction> freeSquares, std::vector<std::vector<int>> charPositions, std::vector<Direction> charDirections) { AI.setDirection(object, freeSquares, charPositions, charDirections); }

	//Object getters and setters
	int getX() { return object.x; }
	void setX(int X) { object.x = X; }

	int getY() { return object.y; }
	void setY(int Y) { object.y = Y; }

	void setType(Type type) { object.type = type; }

	Direction getDirection() { return object.dir; }
	void setDirection(Direction direction) { object.dir = direction; }

	GameObjectStruct getStruct() { return object; }

	std::vector<int> getPosition() { return { object.x, object.y }; }//Get position as vector
};


#endif //PACMAN_CHARACTER_H
