#pragma once
#ifndef BOOM_ROOM_H
#define BOOM_ROOM_H
#include "Room.h"
class BoomRoom : public Room {
public:
	BoomRoom() {
	}
	virtual ~BoomRoom()
	{

	}
	virtual void SetSize(int size) {
		std::cout << "set BoomRoom size " << size << std::endl;
	}
private:
};
#endif // !BOOM_ROOM_H
