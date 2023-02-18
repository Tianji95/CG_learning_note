#pragma once
#ifndef TOY_DECORATOR_H
#define TOY_DECORATOR_H
#include <iostream>
#include"AbstractDecorator.h"
class ToyDecorator : public AbstractDecorator {
public:
	ToyDecorator(EquipmentComponent* comp) : AbstractDecorator(comp) {

	}
	virtual float GetPrice() {
		BeToy();
		return _comp->GetPrice();
	}
private:
	void BeToy() {
		std::cout << "to be a toy" << std::endl;
	}
};

#endif // !TOY_DECORATOR_H
