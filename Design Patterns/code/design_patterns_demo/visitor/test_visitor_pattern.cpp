#include "EquipmentVisitor.h"
int main() {
	EquipmentVisitor* visiter = new EquipmentVisitor();
	Component* disk = new DiskComponent();
	Component* mem = new MemoryComponent();
	disk->AcceptVisitor(visiter);
	mem->AcceptVisitor(visiter);
	visiter->GetTotalPrice();

}