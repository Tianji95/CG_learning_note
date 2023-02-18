#pragma once
#ifndef PROTOTYPE_FACTORY_H
#define PROTOTYPE_FACTORY_H
#include "Room.h"
#include "Door.h"
#include "AbstructFactory.h"
class ProtoTypeFactory : public AbstructFactory{
public:
	ProtoTypeFactory(Room* r, Door* d){
		_room = r;
		_door = d;
	}
	virtual ~ProtoTypeFactory() {}
	Door* CreateDoor() {
		return _door->Clone();
	}
	Room* CreateRoom() {
		return _room->Clone();
	}
private:
	Room* _room = nullptr;
	Door* _door = nullptr;
};
#endif // !PROTOTYPE_FACTORY_H
