#pragma once
#include "GameObject.h"
class Bullet;
#include "json.hpp" // https://github.com/nlohmann/json
using json = nlohmann::json;

class Alien : public GameObject
{
private:
	bool _alive;
	Bullet* _bullet;
	float _start_x;
	float _start_y;

public:
	Alien(float x, float y, int width, int height, Move* move_component, CollisionMediator* collision, 
		Draw* draw_component, Bullet* bullet);
	void update();
	json toJSON();
	float xPos() { return _x_pos; }
	float yPos() { return _y_pos; }
	void setPos(float x, float y) { _x_pos = x; _y_pos = y; }
	void setX(float x) { _x_pos = x; }
	void setY(float y) { _y_pos = y; }
	void addToX(float i) { _x_pos += i; }
	void addToY(float i) { _y_pos += i; }
	Move* getMoveComponent() { return _move_component; }
	void die() { _alive = false; }
	void reset();
	Bullet* getBullet() { return _bullet; }
};

