#pragma once
#ifndef MEMORY_COMPONENT_H
#define MEMORY_COMPONENT_H
#include "EquipmentComponent.h"
class MemoryComponent : public EquipmentComponent {
public:
	virtual float GetPrice() {
		return 40;
	}
	virtual float Power()
	{
		return 40;
	}
	virtual void AcceptVisitor(ComponentVisitor* visitor) {
		visitor->Visit(this);
	}
};

#endif // !MEMORY_COMPONENT_H
