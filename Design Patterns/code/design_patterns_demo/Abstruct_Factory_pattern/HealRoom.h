#pragma once
#ifndef HEAL_ROOM_H
#define HEAL_ROOM_H
#include "Room.h"

class HealRoom : public Room {
public:
	HealRoom() {}
	~HealRoom()
	{

	}
	virtual void SetSize(int size) {
		std::cout << "set HealRoom size " << size << std::endl;
	}
private:
};
#endif // !HEAL_ROOM_H
