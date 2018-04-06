//==============================================================
// Filename : Game.cpp
// Authors : Brian Chang (s1387693), Yuri van Midden (s1496182)
// Version :
// License :
// Description : Class definition of Game
//==============================================================

#include <vector>
#include <stdlib.h>//Required for random number generator
#include <time.h>//Required for random seed

#include "../include/GameObjectStruct.h"
#include "../include/Character.h"
#include "../include/Game.h"

//1. FUNCTIONS FOR INITIALIZATION

//Public: Constructor
Game::Game(std::vector<std::vector<int>> board) {
	//Game state variables
	score = 0;
	lives = 2;//2 extra lives
	map = board;
	configureMap(map);//Call function to configure map
	mapSaved = map;//Save a copy of map for resetting

	//Characters
	//Character constructor is explained in the class definition
	pacman = Character({ 14, 21, PACMAN, LEFT }, {}, {});
	blinky = Character({ 12, 13, BLINKY, RIGHT }, { 13, 11 }, { (int)map.at(0).size() - 1 , 0 });
	pinky = Character({ 13, 13, PINKY, UP }, { 13, 11 }, { 0, 0 });
	inky = Character({ 14, 13, INKY, UP }, { 14, 11 }, { 0, (int)map.size() - 1 });
	clyde = Character({ 15, 13, CLYDE, LEFT }, { 14, 11 }, { (int)map.at(0).size() - 1, (int)map.size() - 1 });

	//Previous character position (for collision detection)
	pacmanPreviousPosition = pacman.getPosition();
	blinkyPreviousPosition = blinky.getPosition();
	pinkyPreviousPosition = pinky.getPosition();
	inkyPreviousPosition = inky.getPosition();
	clydePreviousPosition = clyde.getPosition();

	//Timers for releasing ghosts
	blinkyTimer = 0;	//0s
	pinkyTimer = 20;	//1s
	inkyTimer = 200;	//10s
	clydeTimer = 300;	//15s

	//Scatter handling
	scatterTime = 0;	//0s
	chaseTime = 400;	//20s
	scatter = false;

	//Energizer handling
	energizerTimer = -1;//Set -1 because functions are executed when timer expires at 0
	killCounter = 0;

	//Fruit handling
	//Fruit object will be handled by fruitManager() so it is not done here
	fruitInit = false;
	fruitActive = false;
	fruitEaten = false;
	srand(time(NULL));//Initialize random seed
	fruitCounter = 1500 + rand() % 1001;//Set threshold to random value between 1500-2500
}

//Public: Restart after death (use life)
void Game::initDeath() {
	//Game state variables
	lives--;//Decrement number of lives

	//Characters
	resetPositions();

	//Previous character position (for collision detection)
	//Handled by resetPositions()

	//Timers for releasing ghosts
	blinkyTimer = 0;		//0s
	pinkyTimer = 20;		//1s
	if (inkyTimer < 40) {
		inkyTimer = 40;		//Don't change unless it is below 2s, in which case set it to 2s
	}
	if (clydeTimer < 100) {
		clydeTimer = 100;	//Don't change unless it is below 5s, in which case set it to 5s
	}

	//Scatter handling
	scatterTime = 0;	//0s
	chaseTime = 400;	//20s
	scatter = false;

	//Energizer handling
	energizerTimer = -1;//Set -1 because functions are executed when timer expires at 0
	killCounter = 0;

	//Fruit handling
	//Reset if fruit has not been eaten yet
	if (!fruitEaten) {
		fruitActive = false;
		if (fruitCounter - score < 200) {//Reset point threshold if it is less than 200 points away from current score
//			srand(time(NULL));//Don't do this, already called in Game constructor
			fruitCounter = score + 200 + rand() % 201;//Set threshold to random value between 200-400 above the current score
		}
	}
}

//Public: Start next level
void Game::initNext() {
	//Game state variables
	map = mapSaved;//Reset map

	//Characters
	resetPositions();

	//Previous character position (for collision detection)
	//Handled by resetPositions()

	//Timers for releasing ghosts
	//Fast timers from start from the second level
	blinkyTimer = 0;	//0s
	pinkyTimer = 20;	//1s
	inkyTimer = 40;		//2s
	clydeTimer = 100;	//5s

	//Scatter handling
	scatterTime = 0;
	chaseTime = 400;
	scatter = false;

	//Energizer handling
	energizerTimer = -1;//Set -1 because functions are executed when timer expires at 0
	killCounter = 0;

	//Fruit handling
	fruitInit = false;
	fruitActive = false;
	fruitEaten = false;
//	srand(time(NULL));//Don't do this, already called in Game constructor
	fruitCounter = score + 1500 + rand() % 1001;//Set threshold to random value between 1500-2500 above the current score
}

//Public: Start new game
void Game::initNew() {
	//Game state variables
	score = 0;
	lives = 2;//2 extra lives.
	map = mapSaved;//Reset map

	//Characters
	resetPositions();

	//Previous character position (for collision detection)
	//Handled by resetPositions()

	//Timers for releasing ghosts
	blinkyTimer = 0;	//0s
	pinkyTimer = 20;	//1s
	inkyTimer = 200;	//10s
	clydeTimer = 300;	//15s

	//Scatter handling
	scatterTime = 0;	//0s
	chaseTime = 400;	//20s
	scatter = false;

	//Energizer handling
	energizerTimer = -1;//Set -1 because functions are executed when timer expires at 0
	killCounter = 0;

	//Fruit handling
	//Fruit object will be handled by fruitManager() so it is not done here
	fruitInit = false;
	fruitActive = false;
	fruitEaten = false;
//	srand(time(NULL));//Don't do this, already called in Game constructor
	fruitCounter = 1500 + rand() % 1001;//Set threshold to random value between 1500-2500
}

//Private: Configuration function for the map
void Game::configureMap(std::vector<std::vector<int>> &map) {
	//Function must be manually defined for a specific map

	//Indicate ghost house with -1
	map.at(13).at(12) = -1;//Blinky home
	map.at(13).at(13) = -1;//Pinky home
	map.at(13).at(14) = -1;//Inky home
	map.at(13).at(15) = -1;//Clyde home
	map.at(12).at(13) = -1;//Entrance 1
	map.at(12).at(14) = -1;//Entrance 2

	//Fill the map with dots (replace all instances of 0 with 2)
	for (int y = 0; y < map.size(); y++) {
		for (int x = 0; x < map.at(0).size(); x++) {
			if (map.at(y).at(x) == 0) {
				map.at(y).at(x) = 2;
			}
		}
	}

	//Remove dots from invalid locations
	map.at(21).at(14) = 0;//Pacman home

	//Add energizers
	map.at(21).at(1) = 3;
	map.at(21).at(26) = 3;
	map.at(5).at(1) = 3;
	map.at(5).at(26) = 3;
}

//Private: Reset all characters to start positions
void Game::resetPositions() {
	//Call reinitialization function for each character
	pacman.reinit();
	blinky.reinit();
	pinky.reinit();
	inky.reinit();
	clyde.reinit();

	//Reset previous position for each character
	pacmanPreviousPosition = pacman.getPosition();
	blinkyPreviousPosition = blinky.getPosition();
	pinkyPreviousPosition = pinky.getPosition();
	inkyPreviousPosition = inky.getPosition();
	clydePreviousPosition = clyde.getPosition();
}

//---------------------------------------------------------------------------------------------------------------------------

//2. HELPER FUNCTIONS FOR CHECKING SQUARES ADJACENT TO A CHARACTER

//Private: Check if square at character is equal to value
bool Game::checkAt(Character character, int value) {
	if (map.at(character.getY()).at(character.getX()) == value) {
		return true;
	}
	else {
		return false;
	}
}

//Private: Check if square above character is not equal to value
bool Game::checkNotUp(Character character, int value) {
	if (map.at(character.getY() - 1).at(character.getX()) != value) {
		return true;
	}
	else {
		return false;
	}
}

//Private: Check if square below character is not equal to value
bool Game::checkNotDown(Character character, int value) {
	if (map.at(character.getY() + 1).at(character.getX()) != value) {
		return true;
	}
	else {
		return false;
	}
}

//Private: Check if square left character is not equal to value
bool Game::checkNotLeft(Character character, int value) {
	if (map.at(character.getY()).at(character.getX() - 1) != value) {
		return true;
	}
	else {
		return false;
	}
}

//Private: Check if square right of character is not equal to value
bool Game::checkNotRight(Character character, int value) {
	if (map.at(character.getY()).at(character.getX() + 1) != value) {
		return true;
	}
	else {
		return false;
	}
}

//---------------------------------------------------------------------------------------------------------------------------

//3. FUNCTIONS FOR SETTING CHARACTER DIRECTION

//Public: Set direction of Pacman
void Game::inputDirection(Direction direction) {
	//Set input direction if the square in that direction is not a wall (1) or ghost house (-1)
	//An additional check is performed first to make sure that direction is not out-of-bounds
	switch (direction) {
	case UP:
		if (pacman.getY() != 0) {
			if (checkNotUp(pacman, 1) && checkNotUp(pacman, -1)) {
				pacman.setDirection(UP);
			}
		}
		break;
	case DOWN:
		if (pacman.getY() != map.size() - 1) {
			if (checkNotDown(pacman, 1) && checkNotDown(pacman, -1)) {
				pacman.setDirection(DOWN);
			}
		}
		break;
	case LEFT:
		if (pacman.getX() != 0) {
			if (checkNotLeft(pacman, 1) && checkNotLeft(pacman, -1)) {
				pacman.setDirection(LEFT);
			}
		}
		break;
	case RIGHT:
		if (pacman.getX() != map.at(0).size() - 1) {
			if (checkNotRight(pacman, 1) && checkNotRight(pacman, -1)) {
				pacman.setDirection(RIGHT);
			}
		}
		break;
	}
}

//Private: Get positions of characters for AI
std::vector<std::vector<int>> Game::getPositions() {
	//Positions must be added to the vector in this order since this is the order the AI will assume
	std::vector<std::vector<int>> returnVector;
	returnVector.push_back(pacman.getPosition());
	returnVector.push_back(blinky.getPosition());
	returnVector.push_back(pinky.getPosition());
	returnVector.push_back(inky.getPosition());
	returnVector.push_back(clyde.getPosition());
	return returnVector;
}

//Private: Get directions of characters for AI
std::vector<Direction> Game::getDirections() {
	//Directions must be added to the vector in this order since this is the order the AI will assume
	std::vector<Direction> returnVector;
	returnVector.push_back(pacman.getDirection());
	returnVector.push_back(blinky.getDirection());
	returnVector.push_back(pinky.getDirection());
	returnVector.push_back(inky.getDirection());
	returnVector.push_back(clyde.getDirection());
	return returnVector;
}

//Private: Get free squares around character for AI
std::vector<Direction> Game::getFreeSquares(Character character) {
	std::vector<Direction> returnVector;
	//Check if up is free. If at edge, it is a portal so it must be free.
	if (character.getY() != 0) {
		//1 indicates a wall, so check that the square is not a 1.
		if (checkNotUp(character, 1)) {
			//-1 is only a valid square if the entryAllowed flag of the character is true (used in ghost initialization).
			if (checkNotUp(character, -1) || character.getEntryAllowed()) {
				//Push free square to returnVector.
				returnVector.push_back(UP);
			}
		}
	}
	else {
		//Push free square to returnVector.
		returnVector.push_back(UP);
	}
	//Check if down is free
	if (character.getY() != map.size() - 1) {
		if (checkNotDown(character, 1)) {
			if (checkNotDown(character, -1) || character.getEntryAllowed()) {
				returnVector.push_back(DOWN);
			}
		}
	}
	else {
		returnVector.push_back(DOWN);
	}
	//Check if left is free
	if (character.getX() != 0) {
		if (checkNotLeft(character, 1)) {
			if (checkNotLeft(character, -1) || character.getEntryAllowed()) {
				returnVector.push_back(LEFT);
			}
		}
	}
	else {
		returnVector.push_back(LEFT);
	}
	//Check if right is free
	if (character.getX() != map.at(0).size() - 1) {
		if (checkNotRight(character, 1)) {
			if (checkNotRight(character, -1) || character.getEntryAllowed()) {
				returnVector.push_back(RIGHT);
			}
		}
	}
	else {
		returnVector.push_back(RIGHT);
	}
	return returnVector;
}

//---------------------------------------------------------------------------------------------------------------------------

//4. FUNCTIONS FOR CHARACTER MOVEMENT

//Private: Update position of character.
void Game::updatePosition(Character &character) {
	//Save previous postion
	switch (character.getName()) {
	case PACMAN:
		pacmanPreviousPosition = character.getPosition();
		break;
	case BLINKY:
		blinkyPreviousPosition = character.getPosition();
		break;
	case PINKY:
		pinkyPreviousPosition = character.getPosition();
		break;
	case INKY:
		inkyPreviousPosition = character.getPosition();
		break;
	case CLYDE:
		clydePreviousPosition = character.getPosition();
		break;
	}

	//Update position if the square in the character's direction is valid.
	//The entryAllowed flag is used to determine whether -1 is a valid square for the character.
	//Map edges must be bounded by "1" except in the case of portals.
	//Portals musts be on the same line. Vertical portals are allowed.
	switch (character.getDirection()) {
	case UP:
		if (character.getY() == 0) {
			character.setY(map.size() - 1);
		}
		else if (checkNotUp(character, 1)) {
			if (checkNotUp(character, -1) || character.getEntryAllowed()) {
				character.setY(character.getY() - 1);
			}
		}
		break;
	case DOWN:
		if (character.getY() == map.size() - 1) {
			character.setY(0);
		}
		else if (checkNotDown(character, 1)) {
			if (checkNotDown(character, -1) || character.getEntryAllowed()) {
				character.setY(character.getY() + 1);
			}
		}
		break;
	case LEFT:
		if (character.getX() == 0) {
			character.setX(map.at(0).size() - 1);
		}
		else if (checkNotLeft(character, 1)) {
			if (checkNotLeft(character, -1) || character.getEntryAllowed()) {
				character.setX(character.getX() - 1);
			}
		}
		break;
	case RIGHT:
		if (character.getX() == map.at(0).size() - 1) {
			character.setX(0);
		}
		else if (checkNotRight(character, 1)) {
			if (checkNotRight(character, -1) || character.getEntryAllowed()) {
				character.setX(character.getX() + 1);
			}
		}
		break;
	}
}

//Private: Helper function for moving a ghost.
void Game::moveGhost(Character &ghost) {
	//Call the ghost's AI to set the direction
	ghost.setDirectionAI(getFreeSquares(ghost), getPositions(), getDirections());
	//Call update position to move ghost
	updatePosition(ghost);
}

//Private: Helper function for checking if a ghost is dead (required for implementing ghost movement speed changes)
bool Game::ghostDead(Character ghost) {
	return (ghost.getState() == DEAD) || (ghost.getState() == RESPAWN);
}

//Private: Helper function for checking if a ghost is vulnerable (required for implementing ghost movement speed changes)
bool Game::ghostVulnerable(Character ghost) {
	return ghost.getVulnerable();
}

//---------------------------------------------------------------------------------------------------------------------------

//5. FUNCTIONS FOR ACTIVATING GHOSTS

//Private: Helper function for activating a ghost when timer expires
void Game::ghostCountdown(Character &ghost, int &timer) {
	if (timer > 0) {
		timer--;
	}
	else if (timer == 0) {
		ghost.setState(START);
		timer--;//Decrement so it only counts down once
	}
}

//---------------------------------------------------------------------------------------------------------------------------

//6. GENERAL GAME MANAGEMENT FUNCTIONS

//Public: Check if Pacman is on a dot or energizer
void Game::eatManager() {
	//Eat dot
	if (checkAt(pacman, 2)) {
		map.at(pacman.getY()).at(pacman.getX()) = 0;
		score = score + 10;
	}

	//Eat energizers
	if (checkAt(pacman, 3)) {
		map.at(pacman.getY()).at(pacman.getX()) = 0;
		score = score + 50;
		energizerTimer = 200;//10s
		killCounter = 0;
		//Set ghosts to vulnerable if they are not dead or respawning
		if ((blinky.getState() != DEAD) && (blinky.getState() != RESPAWN)) {
			blinky.setVulnerable(true);
		}
		if ((pinky.getState() != DEAD) && (pinky.getState() != RESPAWN)) {
			pinky.setVulnerable(true);
		}
		if ((inky.getState() != DEAD) && (inky.getState() != RESPAWN)) {
			inky.setVulnerable(true);
		}
		if ((clyde.getState() != DEAD) && (clyde.getState() != RESPAWN)) {
			clyde.setVulnerable(true);
		}
	}
}

//Public: Check if the map is empty so the next level can be started
bool Game::checkEmpty() {
	for (int y = 0; y < map.size(); y++) {
		for (int x = 0; x < map.at(0).size(); x++) {
			if ((map.at(y).at(x) == 2) || (map.at(y).at(x) == 3)) {
				//If there are pellets or dots left, return false.
				return false;
			}
		}
	}
	return true;
}

//Public: Toggle between scatter and chase mode
void Game::scatterToggle() {
	//When in scatter mode, decrement scatterTime until it reaches zero then switch to chase mode.
	//When in chase mode, decrement chaseTime until it reaches zero then switch to scatter mode.
	if (scatter) {
		if (scatterTime > 0) {
			scatterTime--;
		}
		else {
			chaseTime = 400;//20s
			scatter = false;
			blinky.setScatter(false);
			pinky.setScatter(false);
			inky.setScatter(false);
			clyde.setScatter(false);
		}
	}
	else {
		if (chaseTime > 0) {
			chaseTime--;
		}
		else {
			scatterTime = 100;//5s
			scatter = true;
			blinky.setScatter(true);
			pinky.setScatter(true);
			inky.setScatter(true);
			clyde.setScatter(true);
		}
	}
}

//Public: Toggle vulnerable off when energizer expires
void Game::vulnerableToggle() {
	if (energizerTimer > 0) {
		//Decrement energizer timer
		if (!blinky.getVulnerable() && !pinky.getVulnerable() && !inky.getVulnerable() && !clyde.getVulnerable()) {
			energizerTimer = 0;//Skip rest of timer if no ghosts are vulnerable
		}
		else {
			energizerTimer--;
		}
	}
	else if (energizerTimer == 0) {
		killCounter = 0;
		//Reset vulnerable for ghosts that are not dead or respawning
		if ((blinky.getState() != DEAD) && (blinky.getState() != RESPAWN)) {
			blinky.setVulnerable(false);
		}
		if ((pinky.getState() != DEAD) && (pinky.getState() != RESPAWN)) {
			pinky.setVulnerable(false);
		}
		if ((inky.getState() != DEAD) && (inky.getState() != RESPAWN)) {
			inky.setVulnerable(false);
		}
		if ((clyde.getState() != DEAD) && (clyde.getState() != RESPAWN)) {
			clyde.setVulnerable(false);
		}
		energizerTimer--;//Decrement to -1 so it only executes once
	}
}

//---------------------------------------------------------------------------------------------------------------------------

//7. FUNCTIONS FOR MANAGING GHOST COLLISIONS

//Public: Check if Pacman has run into a ghost (also handles eating scared ghosts)
bool Game::ghostCollision() {
	//Call collisionCheck(blinky) to see if there was a collision with Blinky.
	if (collisionCheck(blinky)) {
		if (blinky.getVulnerable()) {
			if (blinky.getState() != DEAD) {
				//If there was a collision but Blinky was vulnerable and not dead, kill Blinky and increase the score.
				blinky.setState(DEAD);
				killCounter++;
				score = score + getPointsGhostKill();//getPointsGhostKill() returns score for ghost kill based on killCounter.
			}
		}
		else {
			return true;
		}
	}
	//Check for collision with Pinky
	if (collisionCheck(pinky)) {
		if (pinky.getVulnerable()) {
			if (pinky.getState() != DEAD) {
				pinky.setState(DEAD);
				killCounter++;
				score = score + getPointsGhostKill();
			}
		}
		else {
			return true;
		}
	}
	//Check for collision with Inky
	if (collisionCheck(inky)) {
		if (inky.getVulnerable()) {
			if (inky.getState() != DEAD) {
				inky.setState(DEAD);
				killCounter++;
				score = score + getPointsGhostKill();
			}
		}
		else {
			return true;
		}
	}
	//Check for collision with Clyde
	if (collisionCheck(clyde)) {
		if (clyde.getVulnerable()) {
			if (clyde.getState() != DEAD) {
				clyde.setState(DEAD);
				killCounter++;
				score = score + getPointsGhostKill();
			}
		}
		else {
			return true;
		}
	}
	//Check all ghosts before returning false
	return false;
}

//Private: Helper function to check if Pacman has collided with a specific ghost
bool Game::collisionCheck(Character ghost) {
	//Check if Pacman is on same square as ghost
	if (pacman.getPosition() == ghost.getPosition()) {
		return true;
	}

	//Check if Pacman and ghost have passed through each other without landing on the same square (also a collision)
	//Get previous position of ghost
	std::vector<int> ghostPreviousPosition;

	switch (ghost.getName()) {
	case BLINKY:
		ghostPreviousPosition = blinkyPreviousPosition;
		break;
	case PINKY:
		ghostPreviousPosition = pinkyPreviousPosition;
		break;
	case INKY:
		ghostPreviousPosition = inkyPreviousPosition;
		break;
	case CLYDE:
		ghostPreviousPosition = clydePreviousPosition;
		break;
	}

	//If Pacman's current position is the ghost's previous position and Pacman's previous position is the ghost's current position, return true.
	if ((pacman.getPosition() == ghostPreviousPosition) && (pacmanPreviousPosition == ghost.getPosition())) {
		return true;
	}

	//If no collision, return false.
	return false;
}

//Private: Calculate points for killing ghosts
int Game::getPointsGhostKill() {
	//Each subsequent ghost kill is worth more than the previous. killCounter tracks total kills (handled elsewhere).
	switch (killCounter) {
	case 1:
		return 200;
		break;
	case 2:
		return 400;
		break;
	case 3:
		return 800;
		break;
	case 4:
		return 1600;
		break;
	}
}

//---------------------------------------------------------------------------------------------------------------------------

//8. FRUIT HANDLING

//Public: Manage fruit spawning, collision, and scoring
void Game::fruitManager() {
	//Check if score threshold has been reached if fruit has not been eaten yet
	if (!fruitEaten) {
		if ((score > fruitCounter) && !fruitActive) {
			//Loop over map to find zero squares
			std::vector<int> currentSpace;
			std::vector<std::vector<int>> emptySpaces;
			for (int y = 0; y < map.size(); y++) {//Fill pellets
				for (int x = 0; x < map.at(0).size(); x++) {
					if (map.at(y).at(x) == 0) {
						currentSpace = { x, y };
						//Don't spawn fruit on Pacman
						if (pacman.getPosition() != currentSpace) {
							emptySpaces.push_back(currentSpace);
						}
					}
				}
			}
//			srand(time(NULL));//Don't do this, already called in Game constructor
			currentSpace = emptySpaces.at(rand() % emptySpaces.size());//Pick a random square

			//Pick a random fruit type and intialize it. If a fruit was already initialized, just set position.
			if (!fruitInit) {
				std::vector<Type> fruitTypes = { CHERRY, STRAWBERRY, ORANGE, BELL, APPLE, GRAPES };
//				srand(time(NULL));//Don't do this, already called in Game constructor
				Type fruitType = fruitTypes.at(rand() % fruitTypes.size());
				fruit = { currentSpace.at(0), currentSpace.at(1), fruitType, UP };
				fruitInit = true;
			}
			else {
				fruit.x = currentSpace.at(0);
				fruit.y = currentSpace.at(1);
			}
			fruitActive = true;//Flag for drawing fruit and performing collision checks
		}

		//Check for a collision if fruit is active
		if (fruitActive) {
			if ((pacman.getX() == fruit.x) && (pacman.getY() == fruit.y)) {
				switch (fruit.type) {
				case CHERRY:
					score = score + 100;
					break;
				case STRAWBERRY:
					score = score + 300;
					break;
				case ORANGE:
					score = score + 500;
					break;
				case BELL://Use score for Galaxian Boss (next step)
					score = score + 2000;
					break;
				case APPLE:
					score = score + 700;
					break;
				case GRAPES://Replaces melon
					score = score + 1000;
					break;
				}
				fruitEaten = true;//Fruit is eaten so no more fruit will be spawned during this level
				fruitActive = false;//Set inactive so fruit is not drawn on map anymore
			}
		}
	}
}

//---------------------------------------------------------------------------------------------------------------------------

//9. GETTERS FOR UI

//Public: Get objects to be rendered
std::vector<GameObjectStruct> Game::getObjects() {
	std::vector<GameObjectStruct> returnVector;

	//Push pellets and energizers
	for (int y = 0; y < map.size(); y++) {
		for (int x = 0; x < map.at(0).size(); x++) {
			if (map.at(y).at(x) == 2) {
				returnVector.push_back({ x, y, DOT, UP });
			}
			if (map.at(y).at(x) == 3) {
				returnVector.push_back({ x, y, ENERGIZER, UP });
			}
		}
	}

	//Push fruit if active
	if (fruitActive) {
		returnVector.push_back(fruit);
	}

	//Push characters
	returnVector.push_back(clyde.getStruct());
	returnVector.push_back(inky.getStruct());
	returnVector.push_back(pinky.getStruct());
	returnVector.push_back(blinky.getStruct());
	returnVector.push_back(pacman.getStruct());

	return returnVector;
}
