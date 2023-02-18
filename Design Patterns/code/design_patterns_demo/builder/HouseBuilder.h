#pragma once
#ifndef HOUSE_BUILDER_H
#define HOUSE_BUILDER_H
#include "House.h"
class HouseBuilder {
public:
	virtual void CreateHouse() = 0;
	virtual void CreateRoom(int roomid) = 0;
	virtual void CreateDoor(int doorid) = 0;
	virtual House* GetHouse() = 0;
private:
};
#endif // !HOUSE_BUILDER_H
