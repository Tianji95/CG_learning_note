#pragma once
#ifndef HEAL_DOOR_H
#define HEAL_DOOR_H
#include "Door.h"
class HealDoor : public Door
{
public:
	HealDoor() {}
	~HealDoor()
	{	}
	virtual void SetSize(int size) {
		std::cout << "set HealDoor size " << size << std::endl;
	}
};
#endif // !HEAL_DOOR_H
