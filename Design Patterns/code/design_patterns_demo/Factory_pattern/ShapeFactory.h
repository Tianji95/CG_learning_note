#pragma once
#ifndef SHAPE_FACTORY_H
#define SHAPE_FACTORY_H
#include <string>
#include "Shape.h"
#include "Circle.h"
#include "Rectangle.h"

class ShapeFactory {
public:
	ShapeFactory()
	{

	}
	virtual ~ShapeFactory()
	{

	}
	Shape* CreateShape(std::string type) {
		if (type == "circle") {
			return new Circle();
		}
		else if (type == "rectangle") {
			return new Rectangle();
		}
		else {
			return nullptr;
		}
	}
};
#endif // !SHAPE_FACTORY_H
