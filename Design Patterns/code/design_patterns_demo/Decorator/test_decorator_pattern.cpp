#include <iostream>
#include "DiskComponent.h"
#include "MemoryComponent.h"
#include "ToyDecorator.h"
int main() {
	EquipmentComponent* computer = new EquipmentComponent();
	EquipmentComponent* diskToy = new ToyDecorator(new DiskComponent());
	EquipmentComponent* memory = new MemoryComponent();
	computer->Add(diskToy);
	computer->Add(memory);
	float price = computer->GetPrice();
	float power = computer->Power();
	std::cout << "price: " << price << std::endl;
	std::cout << "power: " << power << std::endl;
}