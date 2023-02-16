#pragma once
#ifndef COMPONENT_FACTORY_PRODUCER_H
#define COMPONENT_FACTORY_PRODUCER_H
#include <string>
#include "RoomFactory.h"
#include "DoorFactory.h"
class ComponentFactoryProducer {
public:
	ComponentFactoryProducer() {}
	virtual ~ComponentFactoryProducer() {}
	AbstructFactory* GetFactory(std::string& type) {
		if (type == "Door")
		{
			return new DoorFactory();
		}
		else if (type == "Room")
		{
			return new RoomFactory();
		}
	}
};
#endif // !COMPONENT_FACTORY_PRODUCER_H
