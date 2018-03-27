//
// Created by yuri on 20-3-18.
//

#include "../include/Character.h"

//std::list<GameObjectStruct*> Character::objectList;

Character::Character(GameObjectStruct input) {//Construct character and AI
	object = input;
	AI = CharacterAI(object.type);
//	Character::objectList.push_back(&object);
}