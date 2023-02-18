#pragma once
#ifndef ABSTRUCT_FACTORY_H
#define ABSTRUCT_FACTORY_H
#include "Door.h"
#include "Room.h"
class AbstructFactory {
public:
	AbstructFactory(){}
	virtual ~AbstructFactory()
	{
	}
	virtual Door* CreateDoor() = 0;
	virtual Room* CreateRoom() = 0;
private:

};

#endif // !ABSTRUCT_FACTORY_H
