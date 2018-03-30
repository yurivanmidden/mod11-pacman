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
	std::vector<std::vector<int>> mapSaved;
	Character pacman;
	Character blinky;
	Character pinky;
	Character inky;
	Character clyde;

	//Scatter Toggle
	int scatterTime;
	int chaseTime;
	bool scatter;

	//Test
	int energizerTimer;
	int killCounter;

	//
	int blinkyTimer;
	int pinkyTimer;
	int inkyTimer;
	int clydeTimer;

//	std::vector<std::vector<int>> crossroads;
//	static std::list<GameObjectStruct*> objectList;//List to track objects

public:
	//Blinky Movement
	bool blinkyDead() {	return ghostDead(blinky); }
	bool blinkyScared() { return ghostScared(blinky); }
	void moveBlinky() { moveGhost(blinky); }

	//Pinky Movement
	bool pinkyDead() { return ghostDead(pinky); }
	bool pinkyScared() { return ghostScared(pinky); }
	void movePinky() { moveGhost(pinky); }

	//Inky Movement
	bool inkyDead() { return ghostDead(inky); }
	bool inkyScared() { return ghostScared(inky); }
	void moveInky() { moveGhost(inky); }

	//Clyde Movement
	bool clydeDead() { return ghostDead(clyde); }
	bool clydeScared() { return ghostScared(clyde); }
	void moveClyde() { moveGhost(clyde); }

	//Movement Helpers
	bool ghostDead(Character ghost) {
		return (ghost.getState() == DEAD) || (ghost.getState() == RESPAWN);
	}
	bool ghostScared(Character ghost) {
		return ghost.getVulnerable();
	}
	//Pass by refernce to modify
	void moveGhost(Character &ghost) {
		ghost.setDirectionAI(getFreeSquares(ghost), getPositions(), getDirections());
		updatePosition(ghost);
	}

	void blinkyCountdown() {
		ghostCountdown(blinky, blinkyTimer);
	}

	void pinkyCountdown() {
		ghostCountdown(pinky, pinkyTimer);
	}

	void inkyCountdown() {
		ghostCountdown(inky, inkyTimer);
	}

	void clydeCountdown() {
		ghostCountdown(clyde, clydeTimer);
	}

	//Ghost Timer
	void ghostCountdown(Character &ghost, int &timer) {
		if (timer > 0) {
			timer--;
		}
		else if (timer == 0) {
			ghost.setState(START);
			timer--;
		}
	}

	Game(std::vector<std::vector<int>> board) {
		//Set chase to 1 so that scatter toggles.
		scatterTime = 0;
		chaseTime = 400;
		scatter = false;

		//No energizer at the start
		energizerTimer = 0;
		killCounter = 0;

		//Set timers to for releasing ghosts
		blinkyTimer = 0;
		pinkyTimer = 20;//1s
		inkyTimer = 200;//10s
		clydeTimer = 300;//15s

		// Score, lives, import map
		score = 0;
		lives = 3;
		map = board;
		configureMap(map);//Configure map
		mapSaved = map;//Save a copy of map for resetting

		//Make characters
		pacman = Character({ 14, 21, PACMAN, LEFT }, { 14, 21 }, {}, {});//Pacman has no entrance/corner.
		blinky = Character({ 12, 13, BLINKY, UP }, { 12, 13 }, { 13, 11 }, { (int) map.at(0).size() - 1 , 0 });
		pinky = Character({ 13, 13, PINKY, UP }, { 13, 13 }, { 13, 11 }, { 0, 0 });
		inky = Character({ 14, 13, INKY, UP }, { 14, 13 }, { 14, 11 }, { 0, (int) map.size() - 1 });
		clyde = Character({ 15, 13, CLYDE, UP }, { 15, 13 }, { 14, 11 }, { (int)map.at(0).size() - 1, (int)map.size() - 1 });
	}

	//Set up map configuration manually.
	void configureMap(std::vector<std::vector<int>> &map) {
		//Put -1 where the ghost door/home is
		map.at(13).at(12) = -1;//Ghost Home 1
		map.at(13).at(13) = -1;//Ghost Home 2
		map.at(13).at(14) = -1;//Ghost Home 3
		map.at(13).at(15) = -1;//Ghost Home 4
		map.at(12).at(13) = -1;//Door 1
		map.at(12).at(14) = -1;//Door 2

		//Add dots to map "2"
		for (int y = 0; y < map.size(); y++) {
			for (int x = 0; x < map.at(0).size(); x++) {
				if (map.at(y).at(x) == 0) {
					map.at(y).at(x) = 2;
				}
			}
		}

		//Remove dots from invalid locations (Pacman start)
		map.at(21).at(14) = 0;//Pacman Home

		//Add power pellets
		map.at(21).at(1) = 3;
		map.at(21).at(26) = 3;
		map.at(5).at(1) = 3;
		map.at(5).at(26) = 3;
	}

	//Timer to toggle vulnerable off when energizer expires
	void vulnerableToggle() {
		if (energizerTimer > 0) {
			if (!blinky.getVulnerable() && !pinky.getVulnerable() && !inky.getVulnerable() && !clyde.getVulnerable()) {
				energizerTimer = 0;//Expire timer if nobody is vulnerable
				killCounter = 0;//Reset kill counter
			}
			else {
				energizerTimer--;
			}
		}
		else {
			killCounter = 0;
			blinky.setVulnerable(false);
			pinky.setVulnerable(false);
			inky.setVulnerable(false);
			clyde.setVulnerable(false);
		}
	}

	//Toggle between scatter and chase mode
	void scatterToggle() {
		if (scatter) {
			if (scatterTime > 0) {
				scatterTime--;
			}
			else {
				chaseTime = 400;
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
				scatterTime = 100;
				scatter = true;
				blinky.setScatter(true);
				pinky.setScatter(true);
				inky.setScatter(true);
				clyde.setScatter(true);
			}
		}
	}

	//Reset all characters to start positions
	void resetPositions() {
		pacman.setX(pacman.getHome().at(0));
		pacman.setY(pacman.getHome().at(1));
		pacman.setDirection(LEFT);

		blinky.setX(blinky.getHome().at(0));
		blinky.setY(blinky.getHome().at(1));
		blinky.setDirection(UP);
		blinky.reinitAI();

		pinky.setX(pinky.getHome().at(0));
		pinky.setY(pinky.getHome().at(1));
		pinky.setDirection(UP);
		pinky.reinitAI();

		inky.setX(inky.getHome().at(0));
		inky.setY(inky.getHome().at(1));
		inky.setDirection(UP);
		inky.reinitAI();

		clyde.setX(clyde.getHome().at(0));
		clyde.setY(clyde.getHome().at(1));
		clyde.setDirection(UP);
		clyde.reinitAI();

	}

	//Restart after death
	void initDeath() {
		lives--;

		resetPositions();
		//Set chase to 1 so that scatter toggles.
		scatterTime = 0;
		chaseTime = 400;
		scatter = false;

		//No energizer at the start
		energizerTimer = 0;
		killCounter = 0;

		//Set timers for releasing ghosts
		blinkyTimer = 0;
		pinkyTimer = 20;//1s
		if (inkyTimer < 40) {
			inkyTimer = 40;//Min 2s
		}
		if (clydeTimer < 100) {
			clydeTimer = 100;//Min 5s
		}
	}

	
	//Void new
	void initNew() {
		resetPositions();
		map = mapSaved;
		//Slow timers
		//Reset score

		//Set chase to 1 so that scatter toggles.
		scatterTime = 0;
		chaseTime = 400;
		scatter = false;

		//No energizer at the start
		energizerTimer = 0;
		killCounter = 0;

		blinkyTimer = 0;
		pinkyTimer = 20;//1s
		inkyTimer = 200;//10s
		clydeTimer = 300;//15s

		// Score, lives, import map
		score = 0;
		lives = 3;
	}

	//Next level
	void initNext() {
		resetPositions();
		map = mapSaved;
		//Reset positions
		//Reset board

		//Set chase to 1 so that scatter toggles.
		scatterTime = 0;
		chaseTime = 400;
		scatter = false;
		
		//No energizer at the start
		energizerTimer = 0;
		killCounter = 0;
		
		//Fast timers
		blinkyTimer = 0;
		pinkyTimer = 20;
		inkyTimer = 40;
		clydeTimer = 100;
	}

	//Check if it is time to start next level
	bool checkEmpty() {
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

	//void updatePacman() { pacman.setDirectionAI(); }
	void movePacman() { updatePosition(pacman); }//DEBUG LET PACMAN GO HOME

/*	//Lets test blinky
	void testBlinky() {
//		blinky.setDirectionAI(getFreeSquares(blinky), getPositions(), getDirections());
//		updatePosition(blinky);

		pinky.setDirectionAI(getFreeSquares(pinky), getPositions(), getDirections());
		updatePosition(pinky);

		inky.setDirectionAI(getFreeSquares(inky), getPositions(), getDirections());
		updatePosition(inky);

		clyde.setDirectionAI(getFreeSquares(clyde), getPositions(), getDirections());
		updatePosition(clyde);

	}*/

	//Get positions of characters for AI
	std::vector<std::vector<int>> getPositions() {
		std::vector<std::vector<int>> returnVector;
		returnVector.push_back({ pacman.getX(), pacman.getY() });
		returnVector.push_back({ blinky.getX(), blinky.getY() });
		returnVector.push_back({ pinky.getX(), pinky.getY() });
		returnVector.push_back({ inky.getX(), inky.getY() });
		returnVector.push_back({ clyde.getX(), clyde.getY() });
		return returnVector;
	}

	//Get directions of characters for AI
	std::vector<Direction> getDirections() {
		std::vector<Direction> returnVector;
		returnVector.push_back(pacman.getDirection());
		returnVector.push_back(blinky.getDirection());
		returnVector.push_back(pinky.getDirection());
		returnVector.push_back(inky.getDirection());
		returnVector.push_back(clyde.getDirection());
		return returnVector;
	}

	//Get free squares around character for AI. Uses entryAllowed to determine whether -1 is a valid square.
	std::vector<Direction> getFreeSquares(Character character) {
		std::vector<Direction> returnVector;
		//Check if up is free (if at edge, it is a portal so it must be free)
		if (character.getY() != 0) {
			if (checkNotUp(character, 1)) {
				if (checkNotUp(character, -1) || character.getEntryAllowed()) {
					returnVector.push_back(UP);
				}
			}
		}
		else {
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

	//Update position of character. Map edges must be bounded by "1" except in the case of portals.
	//Portals musts be on the same line. Vertical portals are allowed.
	void updatePosition(Character &character) {
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

	//Update position of character. Map edges must be bounded by "1" except in the case of portals.
	//Portals musts be on the same line. Vertical portals are allowed.
	//Debug version, go anywhere except walls
	void updatePositionDebug(Character &character) {
		switch (character.getDirection()) {
		case UP:
			if (character.getY() == 0) {
				character.setY(map.size() - 1);
			}
			else if (checkNotUp(character, 1)) {
				character.setY(character.getY() - 1);
			}
			break;
		case DOWN:
			if (character.getY() == map.size() - 1) {
				character.setY(0);
			}
			else if (checkNotDown(character, 1)) {
				character.setY(character.getY() + 1);
			}
			break;
		case LEFT:
			if (character.getX() == 0) {
				character.setX(map.at(0).size() - 1);
			}
			else if (checkNotLeft(character, 1)) {
				character.setX(character.getX() - 1);
			}
			break;
		case RIGHT:
			if (character.getX() == map.at(0).size() - 1) {
				character.setX(0);
			}
			else if (checkNotRight(character, 1)) {
				character.setX(character.getX() + 1);
			}
			break;
		}
	}

	//Switch direction of Pacman
	//Perform some checks to protect against edge cases and disallow turning into walls
	//DEBUG: CHECKS REMOVED
	void inputDirectionDebug(Direction direction) {
		switch (direction) {
		case UP:
			pacman.setDirection(UP);
			break;
		case DOWN:
			pacman.setDirection(DOWN);
			break;
		case LEFT:
			pacman.setDirection(LEFT);
			break;
		case RIGHT:
			pacman.setDirection(RIGHT);
			break;
		}
	}
	//Original input direction (not debug)
	void inputDirection(Direction direction) {
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

	int getPointsGhostKill() {
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
		if (killCounter == 4) {
			energizerTimer = 0;//Expire timer if nobody is vulnerable
			killCounter = 0;
		}
	}

	//Collision check between character 1 (pacman) and character 2 (ghost). Will also check if next iteration should cause collision to prevent objects from passing through each other.
	bool collisionCheckNext(Character character1, Character character2) {
		if ((character1.getX() == character2.getX()) && (character1.getY() == character2.getY())) {
			return true;
		}

		switch (character1.getDirection()) {
		case UP:
			if (character1.getY() != 0) {
				if ((character2.getX() == character1.getX()) && (character2.getY() == character1.getY() - 1) && (character2.getDirection() == DOWN)) {
					return true;
				}
			}
			else {
				if ((character2.getX() == character1.getX()) && (character2.getY() == map.size() - 1) && (character2.getDirection() == DOWN)) {
					return true;
				}
			}
			break;
		case DOWN:
			if (character1.getY() != map.size() - 1) {
				if ((character2.getX() == character1.getX()) && (character2.getY() == character1.getY() + 1) && (character2.getDirection() == UP)) {
					return true;
				}
			}
			else {
				if ((character2.getX() == character1.getX()) && (character2.getY() == 0) && (character2.getDirection() == UP)) {
					return true;
				}
			}
			break;
		case LEFT:
			if (character1.getX() != 0) {
				if ((character2.getX() == character1.getX() - 1) && (character2.getY() == character1.getY()) && (character2.getDirection() == RIGHT)) {
					return true;
				}
			}
			else {
				if ((character2.getX() == map.at(0).size() - 1) && (character2.getY() == character1.getY()) && (character2.getDirection() == RIGHT)) {
					return true;
				}
			}
			break;
		case RIGHT:
			if (character1.getX() != map.at(0).size() - 1) {
				if ((character2.getX() == character1.getX() + 1) && (character2.getY() == character1.getY()) && (character2.getDirection() == LEFT)) {
					return true;
				}
			}
			else {
				if ((character2.getX() == 0) && (character2.getY() == character1.getY()) && (character2.getDirection() == LEFT)) {
					return true;
				}
			}
			break;
		}
		return false;
	}

	//Check if pacman has run into a ghost CHECK SCARED
	bool ghostCollision() {
//		if ((pacman.getX() == blinky.getX()) && (pacman.getY() == blinky.getY())) {
		if (collisionCheckNext(pacman, blinky)) {
			if (blinky.getVulnerable()) {
				if (blinky.getState() != DEAD) {
					blinky.setState(DEAD);
					killCounter++;
					score = score + getPointsGhostKill();
				}
			}
			else {
				return true;
			}
		}
//		if ((pacman.getX() == pinky.getX()) && (pacman.getY() == pinky.getY())) {
		if (collisionCheckNext(pacman, pinky)) {
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
//		if ((pacman.getX() == inky.getX()) && (pacman.getY() == inky.getY())) {
		if (collisionCheckNext(pacman, inky)) {
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
//		if ((pacman.getX() == clyde.getX()) && (pacman.getY() == clyde.getY())) {
		if (collisionCheckNext(pacman, clyde)) {
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
		//Return false at the end because it is possible two ghosts occupy the same square
		return false;
	}

	//Check if Pacman is eating something
	void eatCollision() {
		//Eat dots
		if (map.at(pacman.getY()).at(pacman.getX()) == 2) {
			map.at(pacman.getY()).at(pacman.getX()) = 0;
			score = score + 10;
		}
		//Eat energizers
		if (map.at(pacman.getY()).at(pacman.getX()) == 3) {
			killCounter = 0;

			map.at(pacman.getY()).at(pacman.getX()) = 0;
			score = score + 50;

			energizerTimer = energizerTimer + 300;//15 seconds

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
				if (map.at(y).at(x) == 3) {
					returnVector.push_back({ x, y, ENERGIZER, UP });
				}
			}
		}

		//Test Crossroads
		//for (auto i : crossroads) {
		//	returnVector.push_back({ i.at(0), i.at(1), APPEL, UP });
		//}

		//Add characters
/*		GameObjectStruct temp = blinky.getStruct();
//		returnVector.push_back(blinky.getStruct());
		if ((blinky.getState() == DEAD) || (blinky.getState() == RESPAWN)) {
			temp.type = SCAREDINV;
		}
		else if (blinky.getVulnerable()) {
			temp.type = SCARED;
		}
		returnVector.push_back(temp);

		temp = pinky.getStruct();
		if (pinky.getState() == DEAD) {
			temp.type = SCAREDINV;
		}
		else if (pinky.getVulnerable()) {
			temp.type = SCARED;
		}
		returnVector.push_back(temp);*/

		returnVector.push_back(blinky.getStruct());
		returnVector.push_back(pinky.getStruct());
		returnVector.push_back(inky.getStruct());
		returnVector.push_back(clyde.getStruct());
		returnVector.push_back(pacman.getStruct());

//		returnVector.push_back({ blinky.getTargetDebug().at(0), blinky.getTargetDebug().at(1), APPEL, UP });

		return returnVector;
	}



	int getScore() const { return score; }
	int getLives() const { return lives; }



	//Check if square at character is equal to value
	bool checkAt(Character character, int value) {
		if (map.at(character.getY()).at(character.getX()) == value) {
			return true;
		}
		else {
			return false;
		}
	}

	//Check if square above character is equal to value
	bool checkUp(Character character, int value) {
		if (map.at(character.getY() - 1).at(character.getX()) == value) {
			return true;
		}
		else {
			return false;
		}
	}

	//Check if square above character is not equal to value
	bool checkNotUp(Character character, int value) {
		if (map.at(character.getY() - 1).at(character.getX()) != value) {
			return true;
		}
		else {
			return false;
		}
	}

	//Check if square below character is equal to value
	bool checkDown(Character character, int value) {
		if (map.at(character.getY() + 1).at(character.getX()) == value) {
			return true;
		}
		else {
			return false;
		}
	}

	//Check if square below character is not equal to value
	bool checkNotDown(Character character, int value) {
		if (map.at(character.getY() + 1).at(character.getX()) != value) {
			return true;
		}
		else {
			return false;
		}
	}

	//Check if square left of character is equal to value
	bool checkLeft(Character character, int value) {
		if (map.at(character.getY()).at(character.getX() - 1) == value) {
			return true;
		}
		else {
			return false;
		}
	}

	//Check if square left character is not equal to value
	bool checkNotLeft(Character character, int value) {
		if (map.at(character.getY()).at(character.getX() - 1) != value) {
			return true;
		}
		else {
			return false;
		}
	}

	//Check if square right of character is equal to value
	bool checkRight(Character character, int value) {
		if (map.at(character.getY()).at(character.getX() + 1) == value) {
			return true;
		}
		else {
			return false;
		}
	}

	//Check if square right of character is not equal to value
	bool checkNotRight(Character character, int value) {
		if (map.at(character.getY()).at(character.getX() + 1) != value) {
			return true;
		}
		else {
			return false;
		}
	}


};

#endif //PACMAN_GAME_H