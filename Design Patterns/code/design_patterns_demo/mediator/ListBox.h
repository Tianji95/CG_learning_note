#pragma once
#ifndef LIST_BOX_H
#define LIST_BOX_H
#include <list>
#include "Mediator.h"
#include "Widget.h"
class ListBox : public Widget {
public:
	ListBox(Mediator* mediator) :Widget(mediator) {}
	virtual void HandleMouse() {}
	virtual void SetList(std::list<int> list) {
		std::cout << "change list" << std::endl;

	}
};
#endif // !LIST_BOX_H