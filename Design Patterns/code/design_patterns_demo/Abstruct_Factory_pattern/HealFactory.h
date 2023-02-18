#pragma once
#ifndef HEAL_FACTORY_H
#define HEAL_FACTORY_H
#include "AbstructFactory.h"
#include "HealRoom.h"
#include "HealDoor.h"
class HealFactory : public AbstructFactory
{
public:
	HealFactory() {}
	~HealFactory() {}
	Door* CreateDoor() {
		return new HealDoor();
	}
	Room* CreateRoom() {
		return new HealRoom();
	}
};
#endif // !HEAL_FACTORY_H
