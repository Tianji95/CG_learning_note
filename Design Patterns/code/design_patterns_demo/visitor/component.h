#pragma once
#ifndef COMPONENT_H
#define COMPONENT_H
#include <list>
#include "ComponentVisitor.h"
class Component {
public:
	Component() {}
	virtual ~Component() {}
	virtual Component* GetComponent() {
		return nullptr;
	}
	virtual float GetPrice() = 0;
	virtual float Power() = 0;
	virtual void Add(Component* com) = 0;
	virtual void Remove(Component* com) = 0;
	virtual void AcceptVisitor(ComponentVisitor* visitor) = 0;
};

#endif // !COMPONENT_H
