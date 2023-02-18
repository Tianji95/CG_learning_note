#pragma once
#ifndef MAGIC_ROOM_H
#define MAGIC_ROOM_H
#include "Room.h"
class MagicRoom : public Room {
public:
	MagicRoom(const MagicRoom& other) {
		*this = other;
	}
	virtual Room* Clone() const {
		return new MagicRoom(*this);
	}
	void SetSize(int size) override {
		std::cout << "set MagicRoom size " << size << std::endl;
	}
};
#endif // !MAGIC_ROOM_H
