#pragma once
#ifndef EQUIPMENT_COMPONENT_H
#define EQUIPMENT_COMPONENT_H

#include "component.h"
class EquipmentComponent : public Component {
public:
	virtual Component* GetComponent() {
		return this;
	}
	virtual float GetPrice() {
		float price = 0;
		for (auto& com : _components) {
			price += com->GetPrice();
		}
		return price;
	}
	virtual float Power()
	{
		float power = 0;
		for (auto& com : _components) {
			power += com->Power();
		}
		return power;
	}
	virtual void Add(Component* com) {
		_components.push_back(com);
	}
	virtual void Remove(Component* com) {
		_components.remove(com);
	}
private:
	std::list<Component*> _components;
};
#endif // !EQUIPMENT_COMPONENT_H
