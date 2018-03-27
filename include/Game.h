//
// Created by yuri on 20-3-18.
//

#ifndef PACMAN_GAME_H
#define PACMAN_GAME_H


#include <vector>
#include <list>
#include "GameObjectStruct.h"
#include "Character.h"

class Game {
private:
    int score;
    int lives;
	std::vector<std::vector<int>> map;
	Character pacman;
	Character blinky;
	Character pinky;
	Character inky;
	Character clyde;
	std::vector<std::vector<int>> crossroads;

	static std::list<GameObjectStruct*> objectList;//List to track objects

public:
	Game(std::vector<std::vector<int>> board) {
		score = 0;
		lives = 3;
		map = board;
		//Save squares where AI will need to make decisions
		for (int y = 1; y < map.size() - 1; y++) {
			for (int x = 1; x < map.at(0).size() - 1; x++) {
				int count = 0;
				if (map.at(y - 1).at(x) != 1) {
					count++;
				}
				if (map.at(y + 1).at(x) != 1) {
					count++;
				}
				if (map.at(y).at(x - 1) != 1) {
					count++;
				}
				if (map.at(y).at(x + 1) != 1) {
					count++;
				}
				if (map.at(y).at(x) != 1) {
					count++;
				}
				if (count > 2) {
					crossroads.push_back({ x, y });
				}
			}
		}
		//Dots are 2s
		for (int y = 0; y < map.size(); y++) {
			for (int x = 0; x < map.at(0).size(); x++) {
				if (map.at(y).at(x) != 1) {
					map.at(y).at(x) = 2;
				}
			}
		}
		//Remove dots from invalid locations
		map.at(21).at(14) = 0;//Pacman start
		map.at(13).at(12) = 0;//Ghost 1
		map.at(13).at(13) = 0;//Ghost 2
		map.at(13).at(14) = 0;//Ghost 3
		map.at(13).at(15) = 0;//Ghost 4
		map.at(12).at(13) = 0;//Door 1
		map.at(12).at(14) = 0;//Door 2

		//Make characters
		pacman = Character({ 14, 21, PACMAN, LEFT });
		blinky = Character({ 12, 13, BLINKY, UP });
		pinky = Character({ 13, 13, PINKY, UP });
		inky = Character({ 14, 13, INKY, UP });
		clyde = Character({ 15, 13, CLYDE, UP });
	}

	void updatePacman() { pacman.setDirectionAI(); }
	void updatePacmanPosition() { updatePosition(pacman); }


	//Update position of character. Map edges must be bounded by "1" except in the case of portals.
	//Portals musts be on the same line. Vertical portals are allowed.
	void updatePosition(Character &character) {
		switch (character.getDirection()) {
		case UP:
			if (character.getY() == 0) {
				character.setY(map.size() - 1);
			}
			else if (map.at(character.getY() - 1).at(character.getX()) != 1) {
				character.setY(character.getY() - 1);
			}
			break;
		case DOWN:
			if (character.getY() == map.size() - 1) {
				character.setY(0);
			}
			else if (map.at(character.getY() + 1).at(character.getX()) != 1) {
				character.setY(character.getY() + 1);
			}
			break;
		case LEFT:
			if (character.getX() == 0) {
				character.setX(map.at(0).size() - 1);
			}
			else if (map.at(character.getY()).at(character.getX() - 1) != 1) {
				character.setX(character.getX() - 1);
			}
			break;
		case RIGHT:
			if (character.getX() == map.at(0).size() - 1) {
				character.setX(0);
			}
			else if (map.at(character.getY()).at(character.getX() + 1) != 1) {
				character.setX(character.getX() + 1);
			}
			break;
		}
	}

	//Switch direction of Pacman
	//Perform some checks to protect against edge cases and disallow turning into walls
	void inputDirection(Direction direction) {
		switch (direction) {
		case UP:
			if (pacman.getY() != 0) {
				if (map.at(pacman.getY() - 1).at(pacman.getX()) != 1) {
					pacman.setDirection(UP);
				}
			}
			break;
		case DOWN:
			if (pacman.getY() != map.size() - 1) {
				if (map.at(pacman.getY() + 1).at(pacman.getX()) != 1) {
					pacman.setDirection(DOWN);
				}
			}
			break;
		case LEFT:
			if (pacman.getX() != 0) {
				if (map.at(pacman.getY()).at(pacman.getX() - 1) != 1) {
					pacman.setDirection(LEFT);
				}
			}
			break;
		case RIGHT:
			if (pacman.getX() != map.at(0).size() - 1) {
				if (map.at(pacman.getY()).at(pacman.getX() + 1) != 1) {
					pacman.setDirection(RIGHT);
				}
			}
			break;
		}
	}

    void init() {

    }

	//Check if a ghost has run into a wall
	bool wallCollision(Character character) {
		switch (character.getDirection()) {
		case UP:
			break;
		case DOWN:
			break;
		case LEFT:
			break;
		case RIGHT:
			break;
		}
	}

	bool ghostCollision();

	//Check if Pacman is eating something
	void eatCollision() {
		//Eat dots
		if (map.at(pacman.getY()).at(pacman.getX()) == 2) {
			map.at(pacman.getY()).at(pacman.getX()) = 0;
			score = score + 10;
		}
		//Eat energizers
		//Eat fruit
	}

	//Function to get objects
	std::vector<GameObjectStruct> getObjects() {
		std::vector<GameObjectStruct> returnVector;
		//Add pellets
		for (int y = 0; y < map.size(); y++) {//Fill pellets
			for (int x = 0; x < map.at(0).size(); x++) {
				if (map.at(y).at(x) == 2) {
					returnVector.push_back({ x, y, DOT, UP });
				}
			}
		}
		//Add characters
		returnVector.push_back(blinky.getStruct());
		returnVector.push_back(pinky.getStruct());
		returnVector.push_back(inky.getStruct());
		returnVector.push_back(clyde.getStruct());
		returnVector.push_back(pacman.getStruct());
		return returnVector;
	}

	int getScore() const { return score; }
	int getLives() const { return lives; }
};

#endif //PACMAN_GAME_H