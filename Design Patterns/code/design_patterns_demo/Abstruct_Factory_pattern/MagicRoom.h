#pragma once
#ifndef MAGIC_ROOM_H
#define MAGIC_ROOM_H
#include "Room.h"
class MagicRoom : public Room {
public:
	void SetSize(int size) override {
		std::cout << "set MagicRoom size " << size << std::endl;
	}
};
#endif // !MAGIC_ROOM_H
