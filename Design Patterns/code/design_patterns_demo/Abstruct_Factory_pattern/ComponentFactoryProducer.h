#pragma once
#ifndef COMPONENT_FACTORY_PRODUCER_H
#define COMPONENT_FACTORY_PRODUCER_H
#include <string>
#include "BoomFactory.h"
#include "HealFactory.h"
#include "MagicFactory.h"
class ComponentFactoryProducer {
public:
	ComponentFactoryProducer() {}
	virtual ~ComponentFactoryProducer() {}
	AbstructFactory* GetFactory(std::string type) {
		if (type == "Boom")
		{
			return new BoomFactory();
		}
		else if (type == "Heal")
		{
			return new HealFactory();
		}
		else if (type == "Magic") {
			return new MagicFactory();
		}
	}
};
#endif // !COMPONENT_FACTORY_PRODUCER_H
