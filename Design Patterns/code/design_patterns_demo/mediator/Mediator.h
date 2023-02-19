#pragma once
#ifndef MEDIATOR_H
#define MEDIATOR_H
#include "Widget.h"
class Mediator {
public:
	Mediator() {};
	virtual ~Mediator() {};
	virtual void WidgetChanged(Widget* widget) = 0;
	virtual void CreateWidgets() = 0;
	virtual void Loop() = 0;
};
#endif // !MEDIATOR_H
