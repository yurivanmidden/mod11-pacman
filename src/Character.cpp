//
// Created by yuri on 20-3-18.
//

#include "../include/Character.h"

//std::list<GameObjectStruct*> Character::objectList;

Character::Character(GameObjectStruct inputObject, std::vector<int> home, std::vector<int> entrance, std::vector<int> corner) {//Construct character and AI
	object = inputObject;
//	home = { inputObject.x, inputObject.y };
	AI = CharacterAI(object.type, home, entrance, corner);
//	Character::objectList.push_back(&object);
}