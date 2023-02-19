#pragma once
#ifndef GRAPHICS_H
#define GRAPHICS_H
#include <iostream>
class Graphics {
public:
	Graphics() {}
	virtual ~Graphics() {}
	virtual void Draw() = 0;
	virtual void GetPosition(int& x, int& y) {
		x = this->x;
		y = this->y;
		std::cout << "GetPosition: " << x <<", " << y << std::endl;

	}
	virtual void SetPosition(int x, int y) {
		this->x = x;
		this->y = y;
		std::cout << "SetPosition: " << x << ", " << y << std::endl;
	}
private:
	int x = 0;
	int y = 0;
};
#endif // !GRAPHICS_H
