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
	virtual Door* Clone() const = 0;
private:
	float x;
	float y;
};
#endif // !DOOR_H
