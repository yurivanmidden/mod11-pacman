//==============================================================
// Filename : main.cpp
// Authors : Brian Chang (s1387693), Yuri van Midden (s1496182)
// Version :
// License :
// Description : Main program
//==============================================================

#include "../include/GameObjectStruct.h"
#include "../include/UI.h"
#include "../include/Game.h"

//Time-based game mechanics.
Uint32 events(Uint32 interval, void* game)
{
	Game* temp = static_cast<Game*> (game);
	temp->scatterToggle();		//Switch between scatter and chase modes.
	temp->vulnerableToggle();	//Toggle vulnerable state of ghosts when energizer expires.
	temp->blinkyCountdown();	//Countdown for releasing Blinky.
	temp->pinkyCountdown();		//Countdown for releasing Pinky.
	temp->inkyCountdown();		//Countdown for releasing Inky.
	temp->clydeCountdown();		//Countdown for releasing Clyde.
	return interval;
}

//Slow movement speed for scared ghosts.
Uint32 speedSlow(Uint32 interval, void* game)
{
	Game* temp = static_cast<Game*> (game);
	//Check if ghosts are vulnerable and alive (not dead).
	if (!temp->blinkyDead() && temp->blinkyVulnerable()) {
		temp->moveBlinky();
	}
	if (!temp->pinkyDead() && temp->pinkyVulnerable()) {
		temp->movePinky();
	}
	if (!temp->inkyDead() && temp->inkyVulnerable()) {
		temp->moveInky();
	}
	if (!temp->clydeDead() && temp->clydeVulnerable()) {
		temp->moveClyde();
	}
	return interval;
}

//Normal movement speed for Pacman and ghosts.
Uint32 speedNormal(Uint32 interval, void* game)
{
	Game* temp = static_cast<Game*> (game);
	//Pacman always moves at normal speed.
	temp->movePacman();
	//Check if ghosts are alive (not dead) and not vulnerable.
	if (!temp->blinkyDead() && !temp->blinkyVulnerable()) {
		temp->moveBlinky();
	}
	if (!temp->pinkyDead() && !temp->pinkyVulnerable()) {
		temp->movePinky();
	}
	if (!temp->inkyDead() && !temp->inkyVulnerable()) {
		temp->moveInky();
	}
	if (!temp->clydeDead() && !temp->clydeVulnerable()) {
		temp->moveClyde();
	}
	return interval;
}

//Fast movement speed for dead ghosts.
Uint32 speedFast(Uint32 interval, void* game)
{
	Game* temp = static_cast<Game*> (game);
	//Check if ghosts are dead.
	if (temp->blinkyDead()) {
		temp->moveBlinky();
	}
	if (temp->pinkyDead()) {
		temp->movePinky();
	}
	if (temp->inkyDead()) {
		temp->moveInky();
	}
	if (temp->clydeDead()) {
		temp->moveClyde();
	}
	return interval;
}

//Program entry point.
int main(int /*argc*/, char ** /*argv*/)
{
	//Import board.
	std::vector<std::vector<int>> map = {{
		#include "../include/board.def"
	}};
	
	//Create a new UI object.
	UI ui(map);
	
	//Initialize game.
    Game game(map);

	//50 ms timer for time-dependent game mechanics.
	SDL_TimerID timerEvents = SDL_AddTimer(50, events, static_cast<void *>(&game));

	//300 ms, 150 ms, and 75 ms timers for different movement speeds.
	SDL_TimerID moveSlow = SDL_AddTimer(300, speedSlow, static_cast<void *>(&game));
	SDL_TimerID moveNormal = SDL_AddTimer(150, speedNormal, static_cast<void *>(&game));
	SDL_TimerID moveFast = SDL_AddTimer(75, speedFast, static_cast<void *>(&game));

    bool quit = false;
    while (!quit) {
        //Set timeout to limit frame rate.
		Uint32 timeout = SDL_GetTicks() +20;

        //Handle the input.
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            //Quit button.
            if (e.type == SDL_QUIT) {
                quit = true;
            }
            //All keydown events.
            if (e.type == SDL_KEYDOWN) {
                switch (e.key.keysym.sym) {
                case SDLK_LEFT:
					game.inputDirection(LEFT);
                    break;
                case SDLK_RIGHT:
					game.inputDirection(RIGHT);
                    break;
                case SDLK_UP:
					game.inputDirection(UP);
                    break;
                case SDLK_DOWN:
					game.inputDirection(DOWN);
                    break;
                case SDLK_ESCAPE:
                    quit = true;
                    break;
                }
            }
        }

		game.eatManager();//Make Pacman eat.
		game.fruitManager();//Spawn fruit.

		//Start the next level if the board is empty (no more pellets or energizers).
		if (game.checkEmpty()) {
			game.initNext();
		}

		//Check if Pacman has hit a ghost.
		//If the ghost is vulnerable, ghostCollision() handles the eating of the ghost and returns false.
		//If the ghost is not vulnerable, ghostCollision() returns true and the level will be reinitialized.
		if (game.ghostCollision()) {
			if (game.getLives() > 0) {
				game.initDeath();//Restart without resetting the board if the player has lives left.
			}
			else {
				game.initNew();//Start a new game if the player is out of lives.
			}
		}

		//Update UI
        ui.setScore(game.getScore());//Set the score.
		ui.setLives(game.getLives());//Set the amount of lives.
        std::vector<GameObjectStruct> objects = {game.getObjects()};
        ui.update(objects);//Render objects
						   
		//Add keydown events here to improve responsiveness.
		bool keyPressed = false;//Only accept one keypress to prevent errors.
        while (!SDL_TICKS_PASSED(SDL_GetTicks(), timeout)) {
			if (!keyPressed) {
				if (e.type == SDL_KEYDOWN) {
					switch (e.key.keysym.sym) {
					case SDLK_LEFT:
						game.inputDirection(LEFT);
						break;
					case SDLK_RIGHT:
						game.inputDirection(RIGHT);
						break;
					case SDLK_UP:
						game.inputDirection(UP);
						break;
					case SDLK_DOWN:
						game.inputDirection(DOWN);
						break;
					case SDLK_ESCAPE:
						quit = true;
						break;
					}
				}
				keyPressed = true;
			}
        }
    }

	SDL_RemoveTimer(timerEvents);
	SDL_RemoveTimer(moveSlow);
	SDL_RemoveTimer(moveNormal);
	SDL_RemoveTimer(moveFast);

    return 0;
}
