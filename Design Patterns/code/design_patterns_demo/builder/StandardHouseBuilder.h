#pragma once
#ifndef STANDARD_HOUSE_BUILDER_H
#define STANDARD_HOUSE_BUILDER_H
#include "HouseBuilder.h"
#include "Room.h"
#include "Door.h"
class StandardHouseBuilder : public HouseBuilder {
public:
	virtual void CreateHouse() {
		_house = new House();
	}
	virtual void CreateRoom(int roomid) {
		Room* room = new Room();
		if (_house) {
			_house->AttachRoom(room, roomid);
		}

	}
	virtual void CreateDoor(int doorid) {
		Door* door = new Door();
		if (_house) {
			_house->AttachDoor(door, doorid);
		}
	}
	virtual House* GetHouse() {
		return _house;
	}
private:
	House* _house = nullptr;

};
#endif // !STANDARD_HOUSE_BUILDER_H
