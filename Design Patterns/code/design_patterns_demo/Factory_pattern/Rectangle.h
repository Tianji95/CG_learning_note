#pragma once
#ifndef RECTANGLE_H
#define RECTANGLE_H
#include"Shape.h"

class Rectangle : public Shape {
public:
	Rectangle(){}
	virtual ~Rectangle()
	{

	}
	virtual void Draw() {
		std::cout << "Draw Rectangle" << std::endl;
	}
private:
	float width;
	float height;
};
#endif // !RECTANGLE_H
