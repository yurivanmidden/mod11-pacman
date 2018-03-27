/// \file
/// main.cpp

/*
 *  Created on: Jan 29, 2015
 *      Author: frits
 *       Group: TA's
 */

#include "../include/GameObjectStruct.h"
#include "../include/UI.h"
#include "../include/Game.h"

void test() {}

/// Callback function to update the game state.
///
/// This function is called by an SDL timer at regular intervals.
Uint32 gameUpdate(Uint32 interval, void * /*param*/)
{
    // Do Game loop update here
    return interval;
}

Uint32 pacUpdate(Uint32 interval, void* game)
{
	Game* test = static_cast<Game*> (game);
	test->updatePacmanPosition();
	return interval;
}

/// Program entry point.
int main(int /*argc*/, char ** /*argv*/)
{
    std::vector<std::vector<int>> map = {{
        #include "../include/board.def"
    }};

    // Create a new ui object
    UI ui(map); // <-- use map from your Game objects.

    // Start timer for Game update, call this function every 100 ms.
    SDL_TimerID timer_id =
        SDL_AddTimer(100, gameUpdate, static_cast<void *>(nullptr));

    // Initialize game
    Game game(map);

	//Move Pacman every 150 ms
	SDL_TimerID timer_pac = SDL_AddTimer(150, pacUpdate, static_cast<void *>(&game));

    bool quit = false;
    while (!quit) {
        // set timeout to limit frame rate
        Uint32 timeout = SDL_GetTicks() + 20;

        // Handle the input
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            // Quit button.
            if (e.type == SDL_QUIT) {
                quit = true;
            }

            // All keydown events
            if (e.type == SDL_KEYDOWN) {
                switch (e.key.keysym.sym) {
                case SDLK_LEFT: // YOUR CODE HERE
					game.inputDirection(LEFT);
                    break;
                case SDLK_RIGHT: // YOUR CODE HERE
					game.inputDirection(RIGHT);
                    break;
                case SDLK_UP: // YOUR CODE HERE
					game.inputDirection(UP);
                    break;
                case SDLK_DOWN: // YOUR CODE HERE
					game.inputDirection(DOWN);
                    break;
                case SDLK_ESCAPE:
                    quit = true;
                    break;
                }
            }
        }

		game.updatePacman();
		
		game.eatCollision();//Make Pacman eat.
		

        // Set the score
        ui.setScore(game.getScore()); // <-- Pass correct value to the setter

        // Set the amount of lives
        ui.setLives(game.getLives()); // <-- Pass correct value to the setter

        // Render the scene
        std::vector<GameObjectStruct> objects = {game.getObjects()};
        ui.update(objects);

        while (!SDL_TICKS_PASSED(SDL_GetTicks(), timeout)) {			
            // ... do work until timeout has elapsed
        }
    }

    SDL_RemoveTimer(timer_id);
	SDL_RemoveTimer(timer_pac);

    return 0;
}
