#include "MoveByPattern.h"
#include "Variant.h"

MoveByPattern::MoveByPattern(float left, float length, ScriptManager* scripter) {
	_left = left;
	_right = left + length;
	_scripter = scripter;
	_movement_direction = 1;
}

void MoveByPattern::update(GameObject* game_obj) {
	variant var1(variant::TYPE_GAMEOBJECTPTR);
	var1._asGameObjectPtr = game_obj;
	variant var2(variant::TYPE_FLOAT);
	var2._asFloat = _movement_direction;
	variant var3(variant::TYPE_FLOAT);
	var3._asFloat = _left;
	variant var4(variant::TYPE_FLOAT);
	var4._asFloat = _right;

	_scripter->runScript("moveObject", 4, &var1, &var2, &var3, &var4);
}
