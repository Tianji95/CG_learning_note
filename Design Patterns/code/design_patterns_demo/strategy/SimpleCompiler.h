#pragma once
#include <iostream>
#include "Compiler.h"
class SimpleCompiler :public Compiler {
public:
	virtual void Compile() {
		std::cout << "SimpleCompiler Compile" << std::endl;
	}

	virtual void SaveResult() {
		std::cout << "SimpleCompiler SaveResult" << std::endl;
	}
};