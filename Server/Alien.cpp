#include "Alien.h"
#include "Bullet.h"
#include <random>
#include <string>
std::random_device rd;
std::uniform_int_distribution<int> distribution(1, 1000);
std::mt19937 engine(rd()); // Mersenne twister MT19937

Alien::Alien(float x, float y, int width, int height, Move* move_component, CollisionMediator* collision, Draw* draw_component, Bullet* bullet)
	:GameObject(x,y,width,height) {
	_move_component = move_component;
	_collision_component = collision;
	_draw_component = draw_component;
	_alive = true;
	_bullet = bullet;
	_start_x = x;
	_start_y = y;
}

void Alien::update() {
	if (_alive) {
		if (_move_component != nullptr)
			_move_component->update(this);
		if (_collision_component != nullptr)
			_collision_component->update(this);
		if (_bullet != nullptr) {
			if (!_bullet->isFired()) {
				_bullet->setPos(_x_pos + _width / 2, _y_pos + _width / 2);
				int value = distribution(engine);
				if (value < 5) {
					_bullet->fire();
				}
			}
			if(_bullet->isFired()){
				_bullet->update();
			}
		}
		if (_draw_component != nullptr)
			_draw_component->update(*this);
	}
}

json Alien::toJSON() {
	json j;
	j["alien"]["x"] = _x_pos;
	j["alien"]["y"] = _y_pos;
	return j;
}

void Alien::reset() {
	_x_pos = _start_x;
	_y_pos = _start_y;
	_alive = true;
	if(_bullet != nullptr)
		_bullet->reset();
}