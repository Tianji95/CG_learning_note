#pragma once
#ifndef STRATEGY_H
#define STRATEGY_H
#include "Compiler.h"
class Strategy {
public:
	Strategy(Compiler* comp) :_comp(comp) {}
	virtual void Compile() {
		_comp->Compile();
	}
	virtual void SaveResult() {
		_comp->SaveResult();
	}
private:
	Compiler* _comp;
};
#endif // !STRATEGY_H
