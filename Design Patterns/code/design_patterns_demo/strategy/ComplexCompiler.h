#pragma once
#include <iostream>
#include "Compiler.h"
class ComplexCompiler :public Compiler {
public:
	virtual void Compile() {
		std::cout << "ComplexCompiler Compile" << std::endl;
	}

	virtual void SaveResult() {
		std::cout << "ComplexCompiler SaveResult" << std::endl;
	}
};