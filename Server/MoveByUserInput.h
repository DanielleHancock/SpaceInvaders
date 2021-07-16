#pragma once
#include "Move.h"
class GameObject;

class MoveByUserInput : public Move
{
private:
	float _move_increment; // how far the object moves left or right per tic
	int _increments_to_move_sideways;

public:
	MoveByUserInput(float move_increment);
	void update(GameObject* player);
	void moveRight();
	void moveLeft();
};

