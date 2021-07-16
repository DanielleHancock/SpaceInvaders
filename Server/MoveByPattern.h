#pragma once
#include "Move.h"
#include "ScriptManager.h"
#include "dukglue/dukglue.h"
class GameObject;

class MoveByPattern : public Move
{
private:
	ScriptManager* _scripter;
	float _left; // leftmost point
	float _right; // rightmost point

public:
	MoveByPattern(float left, float length, ScriptManager* scripter);
	virtual void update(GameObject* game_obj);
	void setMovementDirection(int i) { _movement_direction = i; }
};

