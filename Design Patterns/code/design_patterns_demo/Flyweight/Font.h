#pragma once
#ifndef FONT_H
#define FONT_H
#include <string>
class Font {
public:
	Font(std::string str) :color(str) {

	}
	virtual std::string GetColor() {
		return color;
	}
private:
	std::string color;
};
#endif // !FONT_H
