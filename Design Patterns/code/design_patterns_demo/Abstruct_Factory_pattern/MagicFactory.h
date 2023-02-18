#pragma once
#ifndef MAGIC_FACTORY_H
#define MAGIC_FACTORY_H
#include "AbstructFactory.h"
#include "MagicDoor.h"
#include "MagicRoom.h"
class MagicFactory : public AbstructFactory {
public:
	MagicFactory() {}
	virtual ~MagicFactory() {}
	Door* CreateDoor() {
		return new MagicDoor();
	}
	Room* CreateRoom() {
		return new MagicRoom();
	}
};
#endif // !MAGIC_FACTORY_H
