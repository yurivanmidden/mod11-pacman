//
// Created by yuri on 20-3-18.
//

#include "../include/Character.h"

GameObjectStruct Character::getStruct() {
    return object;
}

void Character::setStruct(GameObjectStruct s) {
    Character::object = s;
}

Type Character::getType() const {
    return type;
}

void Character::setType(Type type) {
    Character::type = type;
}

int Character::getX() const {
    return x;
}

void Character::setX(int x) {
    Character::x = x;
}

int Character::getY() const {
    return y;
}

void Character::setY(int y) {
    Character::y = y;
}

Direction Character::getDir() const {
    return dir;
}

void Character::setDir(Direction dir) {
    Character::dir = dir;
}
