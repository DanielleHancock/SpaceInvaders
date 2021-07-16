#pragma once
#include "GameObject.h"
#include <iostream>

class Bullet : public GameObject {
private:
	bool _fired;
	float _speed;
	int _move_direction;

public:
	Bullet(float x, float y, float speed, std::string collision_action, int move_direction, Draw* draw);
	void update();
	void fire() { _fired = true; }
	bool isFired() { return _fired; }
	void reset() { _fired = false; }
};

