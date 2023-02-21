#pragma once
#include "ComponentVisitor.h"
class EquipmentVisitor : public ComponentVisitor {
public:
	virtual void Visit(DiskComponent* disk) {
		_price+=disk->GetPrice();
	}
	virtual void Visit(MemoryComponent* disk) {
		_price += disk->GetPrice();
	}
	int GetTotalPrice() {
		return _price;
	}
private:
	int _price = 0;
};