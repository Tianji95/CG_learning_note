#pragma once
#ifndef MAGIC_DOOR_H
#define MAGIC_DOOR_H
#include "Door.h"

class MagicDoor : public Door {
public:
	void SetSize(int size) override {
		std::cout << "set MagicDoor size " << size << std::endl;
	}
};
#endif // !MAGIC_DOOR_H
