#include "InputHandler.h"
#include "Event.h"
#include "Player.h"
#include "MoveByUserInput.h"
#include "Bullet.h"
#include "Alien.h"

extern std::vector<Alien*> aliens;

InputHandler::InputHandler(int GUID) 
	:EventHandler(GUID){
}

void InputHandler::handleEvent(Event e) {
	if (e.getType() == "left arrow press") {
		Player* player = e.getPlayerArg("player");
		player->getMoveComponent()->moveLeft();
	}
	else if (e.getType() == "right arrow press") {
		Player* player = e.getPlayerArg("player");
		player->getMoveComponent()->moveRight();
	}
	else if (e.getType() == "space press") {
		Player* player = e.getPlayerArg("player");
		player->getBullet()->fire();
	}
	else if (e.getType() == "reset") {
		for (int i = 0; i < aliens.size(); i++) {
			aliens[i]->reset();
		}
		Player* player = e.getPlayerArg("player");
		player->reset();
	}
}


