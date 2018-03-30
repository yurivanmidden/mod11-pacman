#ifndef PACMAN_CHARACTERAI_H
#define PACMAN_CHARACTERAI_H

#include "GameObjectStruct.h"
#include <stdlib.h>//Random number generator

class CharacterAI {
private:
	Type myName;
	bool reversed;//Have you reversed yet
	bool scatter;//Disperse command
	bool vulnerable;
	bool entryAllowed;//Are you allowed to go home
	std::vector<int> home;//Home
	std::vector<int> entrance;//Entry point into map
	std::vector<int> corner;//Corner for scatter mode
//	State state;//State of character
	State state;

//	std::vector<int> targetDebug;//DEBUG TARGET

public:
	CharacterAI() {}//Default constructor for compiler
	CharacterAI(Type input, std::vector<int> inputHome, std::vector<int> inputEntrance, std::vector<int> inputCorner) {
		myName = input;
		reversed = false;//Unused at the moment
		scatter = false;
		vulnerable = false;
		entryAllowed = false;
		state = IDLE;
		home = inputHome;
		entrance = inputEntrance;
		corner = inputCorner;

//		targetDebug = { 0, 0 };//DEBUGGG
	}
	
	//GETTERS AND SETTERS
	std::vector<int> getHome() { return home; }
	void setHome(std::vector<int> inputHome) { home = inputHome; }

	bool getEntryAllowed() { return entryAllowed; }
	//void setEntryAllowed(bool inputEntryAllowed) { entryAllowed = inputEntryAllowed; }

	State getState() { return state; }
	void setState(State inputState) { state = inputState; }

	//bool getScatter() { return scatter; }
	void setScatter(bool inputScatter) { scatter = inputScatter; }

	bool getVulnerable() { return vulnerable; }
	void setVulnerable(bool inputVulernable) { vulnerable = inputVulernable; }

	//Reinitialize AI, set flags to starting values.
	void reinitAI() {
		reversed = false;
		scatter = false;
		vulnerable = false;
		entryAllowed = false;
		state = IDLE;
	}

	//Set Target
	std::vector<int> setTarget(std::vector<std::vector<int>> charPositions, std::vector<Direction> charDirections) {
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

	//Blinky Targeting Algorithm: Target is Pacman
	std::vector<int> blinkyTargeting(std::vector<std::vector<int>> charPositions, std::vector<Direction> charDirections) {
		return extractPosition(charPositions, PACMAN);
	}

	//Pinky Targeting Algorithm: Target is four squares in front of Pacman
	std::vector<int> pinkyTargeting(std::vector<std::vector<int>> charPositions, std::vector<Direction> charDirections) {
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

	//Inky Targeting Algorithm: PLACEHOLDER, CALLS BLINKY
	std::vector<int> inkyTargeting(std::vector<std::vector<int>> charPositions, std::vector<Direction> charDirections) {
		return blinkyTargeting(charPositions, charDirections);
	}

	//Clyde Targeting Algorithm: PLACEHOLDER, CALLS PINKY
	std::vector<int> clydeTargeting(std::vector<std::vector<int>> charPositions, std::vector<Direction> charDirections) {
		return pinkyTargeting(charPositions, charDirections);
	}

	//Get target for printing
//	std::vector<int> getTargetDebug() { return targetDebug; }

	//MAIN AI
	void setDirection(GameObjectStruct &object, std::vector<Direction> freeSquares, std::vector<std::vector<int>> charPositions, std::vector<Direction> charDirections) {
		Direction myDirection = extractDirection(charDirections, myName);
		std::vector<int> myPosition = extractPosition(charPositions, myName);

		if (state == IDLE) {
			if (vulnerable) {
				object.type = SCARED;
			}
			else {
				object.type = myName;
			}

			object.dir = randomize({UP, DOWN, LEFT, RIGHT});//Random direction
//			object.dir = UP;
		}
		else if (state == START) {//Allow + set up || Manual Dir Up // Change state2
			if (vulnerable) {
				object.type = SCARED;
			}
			else {
				object.type = myName;
			}

			if (myPosition != entrance) {
				entryAllowed = true;//Test
				object.dir = followTarget(freeSquares, myDirection, entrance, myPosition);
//				targetDebug = entrance;//DEBUG
			}
			else {
				object.dir = LEFT;
				state = ACTIVE;
				entryAllowed = false;
			}
			//follow entrance until position is at entrance, then direction left and state = active
		}
		else if (state == ACTIVE) {
			if (vulnerable) {
				object.type = SCARED;
			}
			else {
				object.type = myName;
			}

			if (vulnerable) {
				object.dir = randomNoReverse(freeSquares, myDirection);
				//Change dir to random no reverse
				//Target debug something
			}
			else if (scatter) {
				object.dir = followTarget(freeSquares, myDirection, corner, myPosition);
//				targetDebug = corner;//DEBUG
			}
			else {
				std::vector<int> target = setTarget(charPositions, charDirections);
				object.dir = followTarget(freeSquares, myDirection, target, myPosition);
//				targetDebug = target;//DEBUG
			}
		}
		else if (state == DEAD) {
			object.type = SCAREDINV;
			entryAllowed = true;//Maybe handle elsewhere
//			std::vector<int> test = { 13, 13 };
			if (myPosition != entrance) {
//				object.dir = followTarget(freeSquares, myDir, home, myPos);
//				targetDebug = home;
				object.dir = followTarget(freeSquares, myDirection, entrance, myPosition);
//				targetDebug = entrance;
			}
			else {
				object.dir = DOWN;
				state = RESPAWN;
			}
		}
		//Respawn: go home, then come alive again
		else if (state == RESPAWN) {
			if (myPosition != home) {
				object.dir = followTarget(freeSquares, myDirection, home, myPosition);
//				targetDebug = home;
			}
			else {
				object.dir = UP;
				state = START;
				vulnerable = false;
				object.type = myName;
			}
		}

	}

	//Find the opposite of a direction
	Direction opposite(Direction direction) {
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

	//extractPos
	std::vector<int> extractPosition(std::vector<std::vector<int>> charPositions, Type name) {
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

	//extract direction of a character from charDirections
	Direction extractDirection(std::vector<Direction> charDirections, Type name) {
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

	//Pick a random direction that is not backwards
	Direction randomNoReverse(std::vector<Direction> freeSquares, Direction myDirection) {
		if (freeSquares.size() == 0) {//Just in case
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
			std::vector<Direction> check;//Save directions that aren't backwards
			for (auto i : freeSquares) {
				if (i != opposite(myDirection)) {
					check.push_back(i);
				}
			}
			return randomize(check);//Pick a random one that
		}
	}

	//Pathing
	Direction followTarget(std::vector<Direction> freeSquares, Direction myDirection, std::vector<int> target, std::vector<int> myPosition) {
		if (freeSquares.size() == 0) {//Just in case
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
			std::vector<Direction> check;//Save directions that aren't backwards
			for (auto i : freeSquares) {
				if (i != opposite(myDirection)) {
					check.push_back(i);
				}
			}
			std::vector<int> distances;//Calculate distance to each one
			for (auto i : check) {
				distances.push_back(distanceToTarget(target, myPosition, i));
			}
			int min = distances.at(0);//Find min distance
			for (auto i : distances) {
				if (i < min) {
					min = i;
				}
			}
			std::vector<Direction> outputs;//Save possibilities
			for (int i = 0; i < distances.size(); i++) {
				if (distances.at(i) == min) {
					outputs.push_back(check.at(i));
				}
			}
			return randomize(outputs);//Pick a random one in case of multiple.
		}
	}

	//Pick a random direction out of a vector of directions
	Direction randomize(std::vector<Direction> directions) {
		int random = rand() % directions.size();
		return directions.at(random);
	}

	//Find the distance to a target from a square in a certain direction
	int distanceToTarget(std::vector<int> target, std::vector<int> myPosition, Direction direction) {
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

};

#endif //PACMAN_CHARACTERAI_H