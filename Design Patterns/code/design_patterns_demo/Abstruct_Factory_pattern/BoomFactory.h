#pragma once
#ifndef BOOM_FACTORY_H
#define BOOM_FACTORY_H
#include "AbstructFactory.h"
#include "BoomRoom.h"
#include "BoomDoor.h"
class BoomFactory : public AbstructFactory {
public:
	BoomFactory() {}
	~BoomFactory() {	}
	Room* CreateRoom() {
		return new BoomRoom();
	}
	Door* CreateDoor() {
		return new BoomDoor();
	}
private:

};

#endif