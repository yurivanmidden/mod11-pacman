//
// Created by yuri on 20-3-18.
//

#ifndef PACMAN_CHARACTER_H
#define PACMAN_CHARACTER_H


#include "GameObjectStruct.h"

class Character {
private:
    GameObjectStruct object;
    Type type;
    int x;
    int y;
    Direction dir;

public:
    Character(Type t) : type(t), x(1), y(1), dir(UP) {
        object.x = x;
        object.y = y;
        object.type = t;
        object.dir = dir;
    }

    GameObjectStruct getStruct();

    void setStruct(GameObjectStruct s);

    Type getType() const;

    void setType(Type type);

    int getX() const;

    void setX(int x);

    int getY() const;

    void setY(int y);

    Direction getDir() const;

    void setDir(Direction dir);

};


#endif //PACMAN_CHARACTER_H
