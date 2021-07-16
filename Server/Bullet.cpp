#include "Bullet.h"
#include "DrawAsRectangle.h"
#include "CollisionMediator.h"
#include <string>

Bullet::Bullet(float x, float y, float speed, std::string collision_action, int move_direction, Draw* draw)
	:GameObject(x, y, 5, 10) {
	_speed = speed;
	_move_direction = move_direction;
	_draw_component = draw;
	_fired = false;
	_collision_action = collision_action;
}

void Bullet::update() {
	if (_fired) {
		_y_pos += (_speed * (float)_move_direction);
		if (_y_pos < 0 || _y_pos > 600) {
			this->reset();
		}
		if (_draw_component != nullptr) {
			_draw_component->update(*this);
		}
	}
}
