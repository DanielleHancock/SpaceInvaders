#include "ServerEventHandler.h"
#include "Event.h"
#include "Alien.h"

extern std::vector<Alien*> aliens;

ServerEventHandler::ServerEventHandler(int GUID)
	:EventHandler(GUID) {

}

void ServerEventHandler::handleEvent(Event e) {
	if (e.getType() == "reset") {
        for (int i = 0; i < aliens.size(); i++) {
			aliens[i]->reset();
        }     
	}
}
