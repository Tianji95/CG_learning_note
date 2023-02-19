#pragma once
#ifndef WIDGET_H
#define WIDGET_H
#include <iostream>
class Mediator;
class Widget {
public:
	Widget(Mediator* mediator);
	virtual void Changed();
	virtual void HandleMouse() {}
private:
	Mediator* _mediator = nullptr;
};
#endif // !WIDGET_H
