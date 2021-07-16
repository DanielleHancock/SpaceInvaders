#include "Player.h"
#include "Bullet.h"
#include "GameObject.h"
#include "json.hpp"
#include "CollisionMediator.h"
using json = nlohmann::json;

Player::Player(float x, float y, int width, int height, MoveByUserInput* move_component, CollisionMediator* collision_component, Draw* draw_component, Bullet* bullet)
	:GameObject(x, y, width, height) {
	_prev_x = x;
	_prev_y = y;
	_move_component = move_component;
	_draw_component = draw_component;
	_collision_component = collision_component;
	_active = true;
	_alive = true;
	_bullet = bullet;
	_start_x = x;
	_start_y = y;
}


void Player::update() {
	if (_alive) {
		if (_move_component != nullptr) {
			_prev_x = this->xPos();
			_prev_y = this->yPos();
			_move_component->update(this);
			if (!_bullet->isFired()) {
				_bullet->setPos(_x_pos + _width/2, _y_pos + _width/2);
			}
			else {
				_bullet->update();
			}
		}
		if (_collision_component != nullptr)
			_collision_component->update(this);
		_draw_component->update(*this);
	}
}

void Player::revertPosition() {
	this->setPos(_prev_x, _prev_y);
}

void Player::draw() {
	_draw_component->update(*this);
}

json Player::toJSON() {
	json j;
	j["player"];
	j["player"]["x"] = _x_pos;
	j["player"]["y"] = _y_pos;
	j["player"]["width"] = _width;
	j["player"]["height"] = _height;
	j["player"]["alive"] = _alive;
	j["player"]["color"]["r"] = this->getDrawComponent()->getColor().r;
	j["player"]["color"]["g"] = this->getDrawComponent()->getColor().g;
	j["player"]["color"]["b"] = this->getDrawComponent()->getColor().b;
	return j;
}

void Player::die() {
	std::cout << "Game Over!" << std::endl;
	_alive = false;

}

void Player::reset() {
	_alive = true;
	_x_pos = _start_x;
	_y_pos = _start_y;
	_bullet->reset();
}