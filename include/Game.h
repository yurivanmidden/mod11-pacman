//==============================================================
// Filename : Game.h
// Authors : Brian Chang (s1387693), Yuri van Midden (s1496182)
// Version :
// License :
// Description : Header file for Game class
//==============================================================

#ifndef PACMAN_GAME_H
#define PACMAN_GAME_H

#include <vector>
#include <stdlib.h>//Required for random number generator
#include <time.h>//Required for random seed

#include "../include/GameObjectStruct.h"
#include "../include/Character.h"

class Game {
private:
	//Game state variables
    int score;
    int lives;
	std::vector<std::vector<int>> map;
	std::vector<std::vector<int>> mapSaved;
	
	//Characters
	Character pacman;
	Character blinky;
	Character pinky;
	Character inky;
	Character clyde;

	//Previous character position (for collision detection)
	std::vector<int> pacmanPreviousPosition;
	std::vector<int> blinkyPreviousPosition;
	std::vector<int> pinkyPreviousPosition;
	std::vector<int> inkyPreviousPosition;
	std::vector<int> clydePreviousPosition;

	//Timers for releasing ghosts
	int blinkyTimer;
	int pinkyTimer;
	int inkyTimer;
	int clydeTimer;

	//Scatter handling
	int scatterTime;
	int chaseTime;
	bool scatter;

	//Energizer handling
	int energizerTimer;
	int killCounter;

	//Fruit handling
	GameObjectStruct fruit;
	bool fruitInit;
	bool fruitActive;
	bool fruitEaten;
	int fruitCounter;

//---------------------------------------------------------------------------------------------------------------------------

//1. FUNCTIONS FOR INITIALIZATION

public:
	Game(std::vector<std::vector<int>> board);//Public: Constructor
	void initDeath();//Public: Restart after death (use life)
	void initNext();//Public: Start next level
	void initNew();//Public: Start new game

private:
	void configureMap(std::vector<std::vector<int>> &map);//Private: Configuration function for the map
	void resetPositions();//Private: Reset all characters to start positions

//---------------------------------------------------------------------------------------------------------------------------

//2. HELPER FUNCTIONS FOR CHECKING SQUARES ADJACENT TO A CHARACTER

private:
	bool checkAt(Character character, int value);//Private: Check if square at character is equal to value
	bool checkUp(Character character, int value);//Private: Check if square above character is equal to value
	bool checkNotUp(Character character, int value);//Private: Check if square above character is not equal to value
	bool checkDown(Character character, int value);//Private: Check if square below character is equal to value
	bool checkNotDown(Character character, int value);//Private: Check if square below character is not equal to value
	bool checkLeft(Character character, int value);//Private: Check if square left of character is equal to value
	bool checkNotLeft(Character character, int value);//Private: Check if square left character is not equal to value
	bool checkRight(Character character, int value);//Private: Check if square right of character is equal to value
	bool checkNotRight(Character character, int value);//Private: Check if square right of character is not equal to value

//---------------------------------------------------------------------------------------------------------------------------

//3. FUNCTIONS FOR SETTING CHARACTER DIRECTION

public:
	void inputDirection(Direction direction);//Public: Set direction of Pacman

private:
	std::vector<std::vector<int>> getPositions();//Private: Get positions of characters for AI
	std::vector<Direction> getDirections();//Private: Get directions of characters for AI
	std::vector<Direction> getFreeSquares(Character character);//Private: Get free squares around character for AI

//---------------------------------------------------------------------------------------------------------------------------

//4. FUNCTIONS FOR CHARACTER MOVEMENT

public:
	void movePacman() { updatePosition(pacman); }

	//Blinky Movement
	void moveBlinky() { moveGhost(blinky); }
	bool blinkyDead() { return ghostDead(blinky); }
	bool blinkyVulnerable() { return ghostVulnerable(blinky); }

	//Pinky Movement
	void movePinky() { moveGhost(pinky); }
	bool pinkyDead() { return ghostDead(pinky); }
	bool pinkyVulnerable() { return ghostVulnerable(pinky); }

	//Inky Movement
	void moveInky() { moveGhost(inky); }
	bool inkyDead() { return ghostDead(inky); }
	bool inkyVulnerable() { return ghostVulnerable(inky); }

	//Clyde Movement
	void moveClyde() { moveGhost(clyde); }
	bool clydeDead() { return ghostDead(clyde); }
	bool clydeVulnerable() { return ghostVulnerable(clyde); }

private:
	void updatePosition(Character &character);//Private: Update position of character
	void moveGhost(Character &ghost);//Private: Helper function for moving a ghost
	bool ghostDead(Character ghost);//Private: Helper function for checking if a ghost is dead (required for implementing ghost movement speed changes)
	bool ghostVulnerable(Character ghost);//Private: Helper function for checking if a ghost is vulnerable (required for implementing ghost movement speed changes)

//---------------------------------------------------------------------------------------------------------------------------

//5. FUNCTIONS FOR ACTIVATING GHOSTS

public:
	void blinkyCountdown() { ghostCountdown(blinky, blinkyTimer); }
	void pinkyCountdown() { ghostCountdown(pinky, pinkyTimer); }
	void inkyCountdown() { ghostCountdown(inky, inkyTimer); }
	void clydeCountdown() { ghostCountdown(clyde, clydeTimer); }

private:
	void ghostCountdown(Character &ghost, int &timer);//Private: Helper function for activating a ghost when timer expires

//---------------------------------------------------------------------------------------------------------------------------

//6. GENERAL GAME MANAGEMENT FUNCTIONS

public:
	void eatManager();//Public: Check if Pacman is on a dot or energizer
	bool checkEmpty();//Public: Check if the map is empty so the next level can be started
	void scatterToggle();//Public: Toggle between scatter and chase mode
	void vulnerableToggle();//Public: Toggle vulnerable off when energizer expires

//---------------------------------------------------------------------------------------------------------------------------

//7. FUNCTIONS FOR MANAGING GHOST COLLISIONS

public:
	bool ghostCollision();//Public: Check if Pacman has run into a ghost (also handles eating scared ghosts)

private:
	bool collisionCheck(Character ghost);//Private: Helper function to check if Pacman has collided with a specific ghost
	int getPointsGhostKill();//Private: Calculate points for killing ghosts

//---------------------------------------------------------------------------------------------------------------------------

//8. FRUIT HANDLING

public:
	void fruitManager();//Public: Manage fruit spawning, collision, and scoring

//---------------------------------------------------------------------------------------------------------------------------

//9. GETTERS FOR UI
public:
	int getScore() { return score; }
	int getLives() { return lives; }
	std::vector<GameObjectStruct> getObjects();//Public: Get objects to be rendered

};

#endif //PACMAN_GAME_H