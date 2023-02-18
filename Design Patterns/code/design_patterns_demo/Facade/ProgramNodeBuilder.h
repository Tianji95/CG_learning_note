#pragma once
#ifndef PROGRAM_NODE_BUILDER_H
#define PROGRAM_NODE_BUILDER_H
#include "ProgramNode.h"
class ProgramNodeBuilder {
public:
	ProgramNodeBuilder() {}
	virtual ProgramNode* NewVariable(const char* variableName)const {
		return _node;
	}
	virtual ProgramNode* NewAssignment(ProgramNode* variable, ProgramNode* expression)const {
		return _node;

	}
	virtual ProgramNode* NewReturnStatement(ProgramNode* value)const {
		return _node;

	}
	virtual ProgramNode* NewCondition(ProgramNode* condition, ProgramNode* truePart, ProgramNode* falsePart)const {
		return _node;

	}
	ProgramNode* GetRootNode() {
		return _node;

	}
private:
	ProgramNode* _node = nullptr;
};
#endif // !PROGRAM_NODE_BUILDER_H
