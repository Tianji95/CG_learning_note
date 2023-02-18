#pragma once
#ifndef ROOM_H
#define ROOM_H
#include <iostream>
class Room {
public:
	Room() {}
	virtual ~Room(){	}
	virtual void SetSize(int size) {
		std::cout << "set room size " << size << std::endl;
	}
	virtual Room* Clone() const = 0;
private:
	float x;
	float y;
	float size;
};

#endif // !ROOM_H
