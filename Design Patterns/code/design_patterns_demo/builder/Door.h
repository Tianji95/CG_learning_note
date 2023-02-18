#pragma once
#ifndef DOOR_H
#define DOOR_H
#include <iostream>
class Door
{
public:
	Door() {}
	virtual ~Door() {}
	virtual void SetSize(int size) {
		std::cout << "set door size " << size << std::endl;
	}
private:
	float x;
	float y;
};
#endif // !DOOR_H
