#pragma once
#ifndef TEXT_AREA_H
#define TEXT_AREA_H
#include <string>
#include "Mediator.h"
#include "Widget.h"
class TextArea : public Widget {
public:
	TextArea(Mediator* mediator) :Widget(mediator) {}
	virtual void HandleMouse() {}
	virtual void ChangeText(std::string s) {
		std::cout << s << std::endl;
	}
};
#endif // !TEXT_AREA_H