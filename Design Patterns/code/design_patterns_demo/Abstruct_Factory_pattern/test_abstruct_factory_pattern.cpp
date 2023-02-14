#include "AbstructShapeFactory.h"
int main() {
	ShapeFactory factory;

	Shape* rect = factory.CreateShape("rectangle");
	rect->Draw();
	Shape* circle = factory.CreateShape("circle");
	circle->Draw();
	return 0;
}