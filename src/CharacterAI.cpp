//==============================================================
// Filename : CharacterAI.cpp
// Authors : Brian Chang (s1387693), Yuri van Midden (s1496182)
// Version :
// License :
// Description : Class definition of CharacterAI
//==============================================================

#include <vector>
#include <stdlib.h>//Required for random number generator
#include <time.h>//Required for random seed

#include "GameObjectStruct.h"
#include "Character.h"

//1. FUNCTIONS FOR INITIALIZATION

//Public: Constructor
CharacterAI::CharacterAI(GameObjectStruct inputObject, std::vector<int> inputEntrance, std::vector<int> inputCorner) {
	myName = inputObject.type;
	startDirection = inputObject.dir;//Object is initialized with starting direction
	exitDirection = LEFT;//Exit left when leaving ghost house
	enterDirection = DOWN;//Go down to enter ghost house
	home = { inputObject.x, inputObject.y };//Object is initialized at home position
	entrance = inputEntrance;
	corner = inputCorner;

	//Trackers
	state = IDLE;
	entryAllowed = false;
	reverse = false;
	scatter = false;
	vulnerable = false;
}

//Public: Reinitialize tracking variables
void CharacterAI::reinit() {
	state = IDLE;
	entryAllowed = false;
	reverse = false;
	scatter = false;
	vulnerable = false;
}

//---------------------------------------------------------------------------------------------------------------------------

//2. GETTERS AND SETTERS

//---------------------------------------------------------------------------------------------------------------------------

//3. TARGET SELECTION

//Private: Main set target function
std::vector<int> CharacterAI::setTarget(std::vector<std::vector<int>> charPositions, std::vector<Direction> charDirections) {
	switch (myName) {
	case BLINKY:
		return blinkyTargeting(charPositions, charDirections);
		break;
	case PINKY:
		return pinkyTargeting(charPositions, charDirections);
		break;
	case INKY:
		return inkyTargeting(charPositions, charDirections);//NEEDS TO BE IMPLEMENTED
		break;
	case CLYDE:
		return clydeTargeting(charPositions, charDirections);//NEEDS TO BE IMPLEMENTED
		break;
	}
}

//Private: Blinky targeting algorithm
std::vector<int> CharacterAI::blinkyTargeting(std::vector<std::vector<int>> charPositions, std::vector<Direction> charDirections) {
	//Target is Pacman's position
	return extractPosition(charPositions, PACMAN);
}

//Private: Pinky targeting algorithm
std::vector<int> CharacterAI::pinkyTargeting(std::vector<std::vector<int>> charPositions, std::vector<Direction> charDirections) {
	//Target is four squares in front of Pacman
	std::vector<int> temp = extractPosition(charPositions, PACMAN);
	switch (extractDirection(charDirections, PACMAN)) {
	case UP:
		temp.at(1) = temp.at(1) - 4;
		break;
	case DOWN:
		temp.at(1) = temp.at(1) + 4;
		break;
	case LEFT:
		temp.at(0) = temp.at(0) - 4;
		break;
	case RIGHT:
		temp.at(0) = temp.at(0) + 4;
		break;
	}
	return temp;
}

//Private: Inky targeting algorithm
std::vector<int> CharacterAI::inkyTargeting(std::vector<std::vector<int>> charPositions, std::vector<Direction> charDirections) {
	//Target is double the vector from Inky's position to the position two squares in front of Pacman
	//Get position two squares in front of Pacman
	std::vector<int> temp = extractPosition(charPositions, PACMAN);
	switch (extractDirection(charDirections, PACMAN)) {
	case UP:
		temp.at(1) = temp.at(1) - 2;
		break;
	case DOWN:
		temp.at(1) = temp.at(1) + 2;
		break;
	case LEFT:
		temp.at(0) = temp.at(0) - 2;
		break;
	case RIGHT:
		temp.at(0) = temp.at(0) + 2;
		break;
	}
	//Get current position
	std::vector<int> myPosition = extractPosition(charPositions, myName);
	//Double the difference between the positions and add it to the current position
	temp.at(0) = 2 * (temp.at(0) - myPosition.at(0)) + myPosition.at(0);
	temp.at(1) = 2 * (temp.at(1) - myPosition.at(1)) + myPosition.at(1);
	return temp;
}

//Private: Clyde targeting algorithm
std::vector<int> CharacterAI::clydeTargeting(std::vector<std::vector<int>> charPositions, std::vector<Direction> charDirections) {
	//Target is corner when distance to Pacman is less than 8 squares, otherwise target is Pacman's position
	std::vector<int> temp = extractPosition(charPositions, PACMAN);
	std::vector<int> myPosition = extractPosition(charPositions, myName);
	//Distance checking function can't be used here since it calculates distance using an adjacent tile
	//Just perform the check manually
	if (abs(temp.at(0) - myPosition.at(0)) + abs(temp.at(1) - myPosition.at(1)) < 8) {
		return corner;
	}
	else {
		return temp;
	}
}

//---------------------------------------------------------------------------------------------------------------------------

//4. GHOST AI

//Public: Main AI (sets direction as well as type)
void CharacterAI::setDirection(GameObjectStruct &object, std::vector<Direction> freeSquares, std::vector<std::vector<int>> charPositions, std::vector<Direction> charDirections) {
	std::vector<int> myPosition = extractPosition(charPositions, myName);//Get position from vector of character positions
	Direction myDirection = extractDirection(charDirections, myName);//Get direction from vector of character directions

	//Set object type depending on state
	if (state == DEAD) {
		object.type = SCAREDINV;
	}
	else {
		if (vulnerable) {
			object.type = SCARED;
		}
		else {
			object.type = myName;
		}
	}

	//Set direction
	if (state == IDLE) {
		//State IDLE: Ghost is at home and is not allowed to move, set random direction
		object.dir = randomize({ UP, DOWN, LEFT, RIGHT });
	}
	else if (state == START) {
		//State START: Ghost is allowed to enter map, target is entrance
		if (myPosition == home) {//When at home, first step should use startDirection
			entryAllowed = true;
			object.dir = followTarget(freeSquares, startDirection, entrance, myPosition);
		}
		else {
			if (myPosition != entrance) {
				object.dir = followTarget(freeSquares, myDirection, entrance, myPosition);
			}
			else {
				object.dir = exitDirection;//Set direction to exitDirection when entrance is reached
				entryAllowed = false;//Ghost is not allowed to enter ghost house anymore
				state = ACTIVE;//Go to state ACTIVE
			}
		}
	}
	else if (state == ACTIVE) {
		//State ACTIVE: Ghost hunts Pacman.
		//If reverse flag is set, reverse direction
		if (reverse) {
			object.dir = opposite(myDirection);//Or opposite(object.dir);
			reverse = false;
		}
		else {
			if (vulnerable) {
				//When vulnerable, ghost makes random turns
				object.dir = randomNoReverse(freeSquares, myDirection);
			}
			else if (scatter) {
				//When in scatter mode, target is the ghost's corner
				object.dir = followTarget(freeSquares, myDirection, corner, myPosition);
			}
			else {
				//Otherwise ghost sets target according to its targeting algorithm
				std::vector<int> target = setTarget(charPositions, charDirections);
				object.dir = followTarget(freeSquares, myDirection, target, myPosition);
			}
		}
	}
	else if (state == DEAD) {
		//State DEAD: Ghost returns to entrance
		if (myPosition != entrance) {
			object.dir = followTarget(freeSquares, myDirection, entrance, myPosition);
		}
		else {
			object.dir = enterDirection;//Set direction to enterDirection when entrance is reached
			entryAllowed = true;//Ghost is allowed to enter ghost house
			state = RESPAWN;//Go to state RESPAWN
		}
	}
	else if (state == RESPAWN) {
		//State RESPAWN: Ghost enters home
		if (myPosition != home) {
			object.dir = followTarget(freeSquares, myDirection, home, myPosition);
		}
		else {
			object.dir = startDirection;//Set direction to startDirection when home is reached
			vulnerable = false;//Ghost is no longer vulnerable
			object.type = myName;//Set ghost model back to normal
			state = START;//Go to state START
		}
	}
}

//Private: Extract position of a character from charPositions
std::vector<int> CharacterAI::extractPosition(std::vector<std::vector<int>> charPositions, Type name) {
	switch (name) {
	case PACMAN:
		return charPositions.at(0);
		break;
	case BLINKY:
		return charPositions.at(1);
		break;
	case PINKY:
		return charPositions.at(2);
		break;
	case INKY:
		return charPositions.at(3);
		break;
	case CLYDE:
		return charPositions.at(4);
		break;
	}
}

//Private: Extract direction of a character from charDirections
Direction CharacterAI::extractDirection(std::vector<Direction> charDirections, Type name) {
	switch (name) {
	case PACMAN:
		return charDirections.at(0);
		break;
	case BLINKY:
		return charDirections.at(1);
		break;
	case PINKY:
		return charDirections.at(2);
		break;
	case INKY:
		return charDirections.at(3);
		break;
	case CLYDE:
		return charDirections.at(4);
		break;
	}
}

//Private: Logic for following a target
Direction CharacterAI::followTarget(std::vector<Direction> freeSquares, Direction myDirection, std::vector<int> target, std::vector<int> myPosition) {
	if (freeSquares.size() == 0) {//Safeguard
		return myDirection;
	}
	else if (freeSquares.size() == 1) {//One path, take it
		return freeSquares.at(0);
	}
	else if (freeSquares.size() == 2) {//Two ways, don't go backwards.
		for (auto i : freeSquares) {
			if (i != opposite(myDirection)) {
				return i;
			}
		}
	}
	else if (freeSquares.size() > 2) {
		std::vector<Direction> checkDirections;
		std::vector<int> checkDistances;

		//Loop thorough freeSquares and calculate the distance for each direction that is not backwards
		for (auto i : freeSquares) {
			if (i != opposite(myDirection)) {
				checkDirections.push_back(i);
				checkDistances.push_back(distanceToTarget(target, myPosition, i));
			}
		}

		//Find the minimum distance
		int min = checkDistances.at(0);
		for (auto i : checkDistances) {
			if (i < min) {
				min = i;
			}
		}

		//Check which directions match the minimum distance (in case of multiple)
		std::vector<Direction> outputs;
		for (int i = 0; i < checkDistances.size(); i++) {
			if (checkDistances.at(i) == min) {
				outputs.push_back(checkDirections.at(i));
			}
		}

		//Pick a random one (in case of multiple)
		return randomize(outputs);
	}
}

//Private: Logic for making random turns
Direction CharacterAI::randomNoReverse(std::vector<Direction> freeSquares, Direction myDirection) {
	if (freeSquares.size() == 0) {//Safeguard
		return myDirection;
	}
	else if (freeSquares.size() == 1) {//One way, take it.
		return freeSquares.at(0);
	}
	else if (freeSquares.size() == 2) {//Two ways, don't go backwards.
		for (auto i : freeSquares) {
			if (i != opposite(myDirection)) {
				return i;
			}
		}
	}
	else if (freeSquares.size() > 2) {
		std::vector<Direction> checkDirections;//Save directions that aren't backwards
		for (auto i : freeSquares) {
			if (i != opposite(myDirection)) {
				checkDirections.push_back(i);
			}
		}
		return randomize(checkDirections);//Pick a random one
	}
}

//Private: Helper function to find the opposite of a direction.
Direction CharacterAI::opposite(Direction direction) {
	switch (direction) {
	case UP:
		return DOWN;
		break;
	case DOWN:
		return UP;
		break;
	case LEFT:
		return RIGHT;
		break;
	case RIGHT:
		return LEFT;
		break;
	}
}

//Private: Helper function to pick a random direction out of a vector of directions
Direction CharacterAI::randomize(std::vector<Direction> directions) {
//	srand(time(NULL));//Don't do this, already called in Game constructor
	int random = rand() % directions.size();
	return directions.at(random);
}

//Private: Helper function to find the distance to a target from a square in a given direction
int CharacterAI::distanceToTarget(std::vector<int> target, std::vector<int> myPosition, Direction direction) {
	switch (direction) {
	case UP:
		myPosition.at(1)--;
		break;
	case DOWN:
		myPosition.at(1)++;
		break;
	case LEFT:
		myPosition.at(0)--;
		break;
	case RIGHT:
		myPosition.at(0)++;
		break;
	}
	return abs(target.at(0) - myPosition.at(0)) + abs(target.at(1) - myPosition.at(1));
}