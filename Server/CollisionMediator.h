#pragma once
class GameObject;
class Alien;
class Bullet;
class Player;
#include <vector>

class CollisionMediator {
private:
	std::vector<std::reference_wrapper<Bullet>>  _bullets;
	std::vector<std::reference_wrapper<Alien>> _aliens;

public:
	CollisionMediator();
	void update(Alien* alien);
	void update(Player* player);
	void addGameObject(Bullet& game_obj);
	void addGameObject(Alien& alien);
	int getBulletsSize() { return _bullets.size(); }
};
