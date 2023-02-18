#pragma once
#ifndef MAGIC_DOOR_H
#define MAGIC_DOOR_H
#include "Door.h"

class MagicDoor : public Door {
public:
	MagicDoor(const MagicDoor& other)
	{
		*this = other;
	}
	virtual Door* Clone() const {
		return new MagicDoor(*this);
	}
	void SetSize(int size) override {
		std::cout << "set MagicDoor size " << size << std::endl;
	}
};
#endif // !MAGIC_DOOR_H
