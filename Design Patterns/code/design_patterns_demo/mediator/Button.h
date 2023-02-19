#pragma once
#ifndef BUTTON_H
#define BUTTON_H
#include "Mediator.h"
#include "Widget.h"
class Button : public Widget {
public:
	Button(Mediator* mediator) :Widget(mediator) {}
	virtual void HandleMouse() {}
	void ShowButton() {
		std::cout << "show Button" << std::endl;
	}
};
#endif // !BUTTON_H