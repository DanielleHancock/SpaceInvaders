#pragma once
#include "EventHandler.h"
class ServerEventHandler : public EventHandler
{
public:
	ServerEventHandler(int GUID);
	void handleEvent(Event e);
};

