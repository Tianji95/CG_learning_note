#pragma once
#ifndef DISK_COMPONENT_H
#define DISK_COMPONENT_H
#include "EquipmentComponent.h"
class DiskComponent : public EquipmentComponent {
public:
	virtual float GetPrice() {
		return 30;
	}
	virtual float Power()
	{
		return 30;
	}
	virtual void AcceptVisitor(ComponentVisitor* visitor) {
		visitor->Visit(this);
	}

};
#endif // !DISK_COMPONENT_H
