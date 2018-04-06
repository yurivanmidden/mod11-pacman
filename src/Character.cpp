//==============================================================
// Filename : Character.cpp
// Authors : Brian Chang (s1387693), Yuri van Midden (s1496182)
// Version :
// License :
// Description : Class definition of Character
//==============================================================

#include <vector>

#include "../include/GameObjectStruct.h"
#include "../include/CharacterAI.h"
#include "../include/Character.h"

//Constructor
Character::Character(GameObjectStruct inputObject, std::vector<int> entrance, std::vector<int> corner) {
	object = inputObject;
	AI = CharacterAI(inputObject, entrance, corner);
}

//Reinitialize character
void Character::reinit() {
	//Reset character type (in case of ghosts that are scared/dead)
	object.type = AI.getName();
	//Reset to starting direction and position
	object.dir = AI.getStartDirection();
	object.x = AI.getHome().at(0);
	object.y = AI.getHome().at(1);
	//Reinitialize AI
	AI.reinit();
}