#include "CollisionMediator.h"
#include "GameTimeline.h"
#include "Alien.h"
#include "Bullet.h"
#include "Player.h"
#include "Event.h"
#include "EventManager.h"

extern GameTimeline timeline;

CollisionMediator::CollisionMediator() {

}

void CollisionMediator::update(Alien* obj) {
	sf::RectangleShape obj_shape(sf::Vector2f(obj->width(), obj->height()));
	obj_shape.setPosition(obj->xPos(), obj->yPos());
	sf::Rect<float> obj_bounds = obj_shape.getGlobalBounds();

	// Check if bullet has hit alien
	for (int i = 0; i < _bullets.size(); i++) { // check for intersections between parameter & all objects in vector
		Bullet* bullet = &_bullets[i].get();
		sf::Rect<float> bullet_bounds(bullet->xPos(), bullet->yPos(), bullet->width(), bullet->height());
		if (obj_bounds.intersects(bullet_bounds)) {
			if (bullet->getCollisionAction() == "kill alien") {
				bullet->reset();
				obj->die();
			}
		}
	}
}

void CollisionMediator::update(Player* obj) {
	sf::RectangleShape obj_shape(sf::Vector2f(obj->width(), obj->height()));
	obj_shape.setPosition(obj->xPos(), obj->yPos());
	sf::Rect<float> obj_bounds = obj_shape.getGlobalBounds();

	// Check if bullet has hit player
	for (int i = 0; i < _bullets.size(); i++) { // check for intersections between parameter & all objects in vector
		Bullet* bullet = &_bullets[i].get();
		sf::Rect<float> bullet_bounds(bullet->xPos(), bullet->yPos(), bullet->width(), bullet->height());
		if (obj_bounds.intersects(bullet_bounds)) {
				bullet->reset();
				obj->die();
				Event e("reset", timeline.getTime(), timeline.getTime());
				EventManager::raiseOnServer(e);
				std::map<std::string, variant> args;
				variant var(variant::Type::TYPE_PLAYERPTR);
				var._asPlayerPtr = obj;
				args["player"] = var;
				Event e2("reset", args, timeline.getTime(), timeline.getTime());
				EventManager::raiseAndHandleNow(e2);
				return;
		}
	}

	// Check if alien has hit player
	for (int i = 0; i < _aliens.size(); i++) { // check for intersections between parameter & all objects in vector
		Alien* alien = &_aliens[i].get();
		sf::Rect<float> bullet_bounds(alien->xPos(), alien->yPos(), alien->width(), alien->height());
		if (obj_bounds.intersects(bullet_bounds)) {
			obj->die();
			Event e("reset", timeline.getTime(), timeline.getTime());
			EventManager::raiseOnServer(e);
			std::map<std::string, variant> args;
			variant var(variant::Type::TYPE_PLAYERPTR);
			var._asPlayerPtr = obj;
			args["player"] = var;
			Event e2("reset", args, timeline.getTime(), timeline.getTime());
			EventManager::raiseAndHandleNow(e2);
			return;
		}
	}
}

void CollisionMediator::addGameObject(Bullet& game_obj) {
	_bullets.push_back(std::reference_wrapper<Bullet>(game_obj));
}

void CollisionMediator::addGameObject(Alien& alien) {
	_aliens.push_back(std::reference_wrapper<Alien>(alien));
}