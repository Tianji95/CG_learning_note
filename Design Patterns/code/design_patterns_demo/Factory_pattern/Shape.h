#pragma once
#ifndef SHAPE_H
#define SHAPE_H
#include <iostream>

class Shape {
public:
	Shape() {

	}
	virtual ~Shape()
	{

	}
	virtual void Draw() = 0;
};

#endif // !SHAPE_H
