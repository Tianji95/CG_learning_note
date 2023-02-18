#include "ComponentFactoryProducer.h"
int main() {
	ComponentFactoryProducer FactoryProducer;
	AbstructFactory* factory = FactoryProducer.GetFactory("Magic");
	Door* d1 = factory->CreateDoor();
	Door* d2 = factory->CreateDoor();

	Room* r1 = factory->CreateRoom();
	Room* r2 = factory->CreateRoom();
	d1->SetSize(3);
	d2->SetSize(5);
	r1->SetSize(6);
	r2->SetSize(7);	
}