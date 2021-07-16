#pragma once
#include "GameObject.h"
#include "MoveByUserInput.h"
class Bullet;
#include "json.hpp"
using json = nlohmann::json;

class Player : public GameObject{

protected:
	float _prev_x;
	float _prev_y;
	bool _alive;
	Bullet* _bullet;
	float _start_x;
	float _start_y;

public:
	Player(float x, float y, int width, int height, MoveByUserInput* move_component, CollisionMediator* collision_component, Draw* draw_component, Bullet* bullet);
	void update();
	void revertPosition(); // move player back to previous position
	json toJSON();
	void loseFocus() { _active = false; }
	void gainFocus() { _active = true; }
	void die();
	void makeAlive() { _alive = true; }
	void draw();
	void reset();
	float xPos() { return _x_pos; }
	float yPos() { return _y_pos; }
	void setPos(float x, float y) { _x_pos = x; _y_pos = y; }
	Bullet* getBullet() { return _bullet; }
};

