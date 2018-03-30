//
// Created by yuri on 20-3-18.
//

#ifndef PACMAN_CHARACTER_H
#define PACMAN_CHARACTER_H

#include <vector>
#include "GameObjectStruct.h"
#include "CharacterAI.h"
//#include <list>

class Character {
private:
    GameObjectStruct object;
	CharacterAI AI;
//	std::vector<int> home;//Home point
//	State state;//Character state for ghosts, add to constructor, get+set
//	static std::list<GameObjectStruct*> objectList;//List to track characters

public:
	Character() {}//Default constructor for compiler
	Character(GameObjectStruct inputObject, std::vector<int> home, std::vector<int> entrance, std::vector<int> corner);


	void setDirectionAI(std::vector<Direction> freeSquares, std::vector<std::vector<int>> charPositions, std::vector<Direction> charDirections) { AI.setDirection(object, freeSquares, charPositions, charDirections); }//ADD STATE, POSITION,
	void reinitAI() { AI.reinitAI(); }

	//GET-SET AI PARAMETERS
	std::vector<int> getHome() { return AI.getHome(); }
	//void setHome(std::vector<int> home) { AI.setHome(home); }

	bool getEntryAllowed() { return AI.getEntryAllowed(); }
	//void setEntryAllowed(bool entryAllowed) { AI.setEntryAllowed(entryAllowed); }

	State getState() { return AI.getState(); }
	void setState(State state) { AI.setState(state); }

	//bool getScatter() { return AI.getScatter(); }
	void setScatter(bool scatter) { AI.setScatter(scatter); }

	bool getVulnerable() { return AI.getVulnerable(); }
	void setVulnerable(bool vulnerable) { AI.setVulnerable(vulnerable); }

//	std::vector<int> getTargetDebug() { return AI.getTargetDebug(); }


	//Character Getters
	Direction getDirection() { return object.dir; }
	int getX() { return object.x; }
	int getY() { return object.y; }
//	Type getType() { return object.type; } //Unused
	GameObjectStruct getStruct() { return object; }//Get the whole object
	//std::vector<int> getHome() { return home; }

	//Character Setters
	void setX(int X) { object.x = X; }//Used by game to move characters around
	void setY(int Y) { object.y = Y; }//Used by game to move characters around
	void setType(Type type) { object.type = type; }//Use for changing ghost
	void setDirection(Direction direction) { object.dir = direction; }//Used by Pacman

//	std::list<GameObjectStruct*> getList() { return objectList; } //Function to get list of packages

};


#endif //PACMAN_CHARACTER_H
