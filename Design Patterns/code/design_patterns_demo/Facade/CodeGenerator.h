#pragma once
#ifndef CODE_GENERATOR_H
#define CODE_GENERATOR_H
#include <iostream>
class ProgramNode;
class CodeGenerator {
public:
	virtual void visit(ProgramNode* node) {

	}
	CodeGenerator(std::istream& in) :_output(in) {}

protected:
private:
	std::istream& _output;

};
#endif // !CODE_GENERATOR_H
