//==============================================================
// Filename : CharacterAI.h
// Authors : Brian Chang (s1387693), Yuri van Midden (s1496182)
// Version :
// License :
// Description : Header file for CharacterAI class
//==============================================================

#ifndef PACMAN_CHARACTERAI_H
#define PACMAN_CHARACTERAI_H

#include <vector>
#include <stdlib.h>//Required for random number generator
#include <time.h>//Required for random seed

#include "GameObjectStruct.h"

//Ghost states
enum State { IDLE, START, ACTIVE, DEAD, RESPAWN };

class CharacterAI {
private:
	//Variables
	Type myName;//Save name because the GameStructObject will be modified when ghosts change modes
	Direction startDirection;//Starting direction
	Direction exitDirection;//Direction to face after leaving ghost house
	Direction enterDirection;//Direction to face to enter ghost hosue
	std::vector<int> home;//Home (starting square)
	std::vector<int> entrance;//Entry point into map
	std::vector<int> corner;//Corner coordinate to target in scatter mode

	//Trackers
	State state;
	bool entryAllowed;//Entry allowed flag
	bool reverse;//Reverse direction flag
	bool scatter;//Scatter flag
	bool vulnerable;//Vulnerable flag

//---------------------------------------------------------------------------------------------------------------------------

//1. FUNCTIONS FOR INITIALIZATION

public:
	CharacterAI() {}//Default constructor for compiler
	CharacterAI(GameObjectStruct inputObject, std::vector<int> inputEntrance, std::vector<int> inputCorner);//Public: Constructor
	void reinit();//Public: Reinitialize tracking variables

//---------------------------------------------------------------------------------------------------------------------------

//2. GETTERS AND SETTERS

public:
	Type getName() { return myName; }

	Direction getStartDirection() { return startDirection; }

	std::vector<int> getHome() { return home; }

	State getState() { return state; }
	void setState(State inputState) { state = inputState; }

	bool getEntryAllowed() { return entryAllowed; }

	void setScatter(bool inputScatter) { scatter = inputScatter; if (!vulnerable) { reverse = true; } }	//When not vulnerable, reverse direction when toggling scatter

	bool getVulnerable() { return vulnerable; }
	void setVulnerable(bool inputVulernable) { vulnerable = inputVulernable; if (vulnerable) { reverse = true; } }//When setting vulernerable true, reverse direction

//---------------------------------------------------------------------------------------------------------------------------

//3. TARGET SELECTION

private:
	//Private: Main set target function
	std::vector<int> setTarget(std::vector<std::vector<int>> charPositions, std::vector<Direction> charDirections);
	
	//Private: Blinky targeting algorithm
	std::vector<int> blinkyTargeting(std::vector<std::vector<int>> charPositions, std::vector<Direction> charDirections);
	
	//Private: Pinky targeting algorithm
	std::vector<int> pinkyTargeting(std::vector<std::vector<int>> charPositions, std::vector<Direction> charDirections);
	
	//Private: Inky targeting algorithm
	std::vector<int> inkyTargeting(std::vector<std::vector<int>> charPositions, std::vector<Direction> charDirections);
	
	//Private: Clyde targeting algorithm
	std::vector<int> clydeTargeting(std::vector<std::vector<int>> charPositions, std::vector<Direction> charDirections);

//---------------------------------------------------------------------------------------------------------------------------

//4. GHOST AI

public:
	//Public: Main AI (sets direction as well as type)
	void setDirection(GameObjectStruct &object, std::vector<Direction> freeSquares, std::vector<std::vector<int>> charPositions, std::vector<Direction> charDirections);

private:
	//Private: Extract position of a character from charPositions
	std::vector<int> extractPosition(std::vector<std::vector<int>> charPositions, Type name);
	
	//Private: Extract direction of a character from charDirections
	Direction extractDirection(std::vector<Direction> charDirections, Type name);
	
	//Private: Logic for following a target
	Direction followTarget(std::vector<Direction> freeSquares, Direction myDirection, std::vector<int> target, std::vector<int> myPosition);
	
	//Private: Logic for making random turns
	Direction randomNoReverse(std::vector<Direction> freeSquares, Direction myDirection);
	
	//Private: Helper function to find the opposite of a direction.
	Direction opposite(Direction direction);
	
	//Private: Helper function to pick a random direction out of a vector of directions
	Direction randomize(std::vector<Direction> directions);
	
	//Private: Helper function to find the distance to a target from a square in a given direction
	int distanceToTarget(std::vector<int> target, std::vector<int> myPosition, Direction direction);
};

#endif //PACMAN_CHARACTERAI_H