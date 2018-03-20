//
// Created by yuri on 20-3-18.
//

#ifndef PACMAN_GAME_H
#define PACMAN_GAME_H


#include <vector>
#include "GameObjectStruct.h"
#include "Character.h"

class Game {
private:
    int score;
    int lives;

public:
    Game() : score(0), lives(3) {

        Character pacman(PACMAN);
    }

    void init() {

    }

    std::vector getStructs() {

    }

    int getScore() const {
        return score;
    }

    void setScore(int score) {
        Game::score = score;
    }

    int getLives() const {
        return lives;
    }

    void setLives(int lives) {
        Game::lives = lives;
    }
};


#endif //PACMAN_GAME_H
