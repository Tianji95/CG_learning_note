#pragma once
#ifndef BOOM_ROOM_H
#define BOOM_ROOM_H
#include "Room.h"
class BoomRoom : public Room {
public:
	BoomRoom() {
	}
	BoomRoom(const BoomRoom& other) {
		*this = other;
	}
	virtual ~BoomRoom()
	{

	}
	virtual void SetSize(int size) {
		std::cout << "set BoomRoom size " << size << std::endl;
	}
	Room* Clone() const {
		return new BoomRoom(*this);
	}
private:
};
#endif // !BOOM_ROOM_H
