#include <iostream>
#include "DiskComponent.h"
#include "MemoryComponent.h"

int main() {
	EquipmentComponent* computer = new EquipmentComponent();
	EquipmentComponent* disk = new DiskComponent();
	EquipmentComponent* memory = new MemoryComponent();
	computer->Add(disk);
	computer->Add(memory);
	float price = computer->GetPrice();
	float power = computer->Power();
	std::cout << "price: " << price << std::endl;
	std::cout << "power: " << power << std::endl;
}