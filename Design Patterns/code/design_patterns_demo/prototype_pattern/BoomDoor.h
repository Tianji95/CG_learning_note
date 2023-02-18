#pragma once
#ifndef BOOM_DOOR_H
#define BOOM_DOOR_H
#include "Door.h"
class BoomDoor : public Door
{
public:
	BoomDoor() {}
	BoomDoor(const BoomDoor& other)
	{
		*this = other;
	}
	virtual ~BoomDoor() {	}
	virtual void SetSize(int size) {
		std::cout << "set BoomDoor size " << size << std::endl;
	}
	Door* Clone() const {
		return new BoomDoor(*this);
	}
private:
};

#endif // !BOOM_DOOR_H
