#pragma once
#ifndef TIME_WIDGET_OBSERVER_H
#define TIME_WIDGET_OBSERVER_H
#include <iostream>
#include "Observer.h"
#include "Subject.h"
class TimeWidgetObserver : public Observer {
public:
	TimeWidgetObserver() {}
	virtual void Update() {
		std::cout << "update TimeWidget" << std::endl;
	}
};

#endif // !TIME_WIDGET_OBSERVER_H
