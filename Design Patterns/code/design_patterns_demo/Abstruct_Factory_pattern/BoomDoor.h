#pragma once
#ifndef BOOM_DOOR_H
#define BOOM_DOOR_H
#include "Door.h"
class BoomDoor : public Door
{
public:
	BoomDoor() {}
	virtual ~BoomDoor() {	}
	virtual void SetSize(int size) {
		std::cout << "set BoomDoor size " << size << std::endl;
	}
private:
};

#endif // !BOOM_DOOR_H
