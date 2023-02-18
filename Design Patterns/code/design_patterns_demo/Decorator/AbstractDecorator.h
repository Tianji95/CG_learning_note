#pragma once
#ifndef ABSTRACT_DECORATOR_H
#define ABSTRACT_DECORATOR_H
#include "EquipmentComponent.h"
class AbstractDecorator : public EquipmentComponent {
public:
	AbstractDecorator(EquipmentComponent* comp) : _comp(comp) {

	}
	virtual float GetPrice() {
		return _comp->GetPrice();
	}
	virtual float Power() {
		return _comp->Power();
	}
	virtual void Add(EquipmentComponent* com) {
		_comp->Add(com);
	}
	virtual void Remove(EquipmentComponent* com) {
		_comp->Remove(com);
	}
protected:
	EquipmentComponent* _comp = nullptr;
};
#endif // !ABSTRACT_DECORATOR_H
