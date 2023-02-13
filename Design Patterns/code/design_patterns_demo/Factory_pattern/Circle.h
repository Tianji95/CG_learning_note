#pragma once
#ifndef CIRCLE_H
#define CIRCLE_H
#include "Shape.h"

class Circle : public Shape {
public:
	Circle(){}
	virtual ~Circle()
	{

	}
	virtual void Draw() {
		std::cout << "Draw Circle" << std::endl;
	}
private:
	float x;
	float y;
	float r;
};

#endif // !CIRCLE_H

