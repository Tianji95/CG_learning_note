#pragma once
#ifndef ROOM_H
#include <iostream>
class Room {
public:
	Room() {}
	virtual ~Room(){	}
	virtual void SetSize(int size) {
		std::cout << "set room size " << size << std::endl;
	}
private:
	float x;
	float y;
	float size;
};

#endif // !ROOM_H
