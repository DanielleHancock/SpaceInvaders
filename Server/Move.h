#pragma once
class GameObject;

class Move {
protected:
	int _movement_direction;

public:
	Move() {};
	virtual void update(GameObject* game_obj) {};
	virtual void moveLeft() {};
	virtual void moveRight() {};
	void setMovementDirection(int i) { _movement_direction = i; }
};

