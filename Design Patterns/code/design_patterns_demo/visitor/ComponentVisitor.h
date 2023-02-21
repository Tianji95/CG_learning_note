#pragma once
#include "MemoryComponent.h"
#include "DiskComponent.h"
class ComponentVisitor {
public:
	virtual void Visit(DiskComponent* disk) = 0;
	virtual void Visit(MemoryComponent* disk) = 0;
};