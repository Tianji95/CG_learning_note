#pragma once
#ifndef PROGRAM_NODE_H
#define PROGRAM_NODE_H
#include<list>
#include "CodeGenerator.h"
class ProgramNode {
public:
	virtual void GetSourcePosition(int& line, int& index) {

	}
	virtual void Add(ProgramNode*) {

	}
	virtual void Remove(ProgramNode*) {

	}
	virtual void Traverse(CodeGenerator& cg) {
		cg.visit(this);
		for (auto& node : _children) {
			node->Traverse(cg);
		}
	}
	std::list< ProgramNode*> _children;

protected:
	ProgramNode() {}
};
#endif // !PROGRAM_NODE_H
