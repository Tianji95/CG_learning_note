#pragma once
#ifndef ROOM_FACTORY_H
#define ROOM_FACTORY_H
#include "AbstructFactory.h"

class RoomFactory : public AbstructFactory {
public:
	RoomFactory() {}
	~RoomFactory() {	}
	Room* CreateRoom() {}
private:

};

#endif