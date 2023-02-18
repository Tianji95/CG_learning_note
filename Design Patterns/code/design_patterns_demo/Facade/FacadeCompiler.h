#pragma once
#ifndef FACADE_COMPILER_H
#define FACADE_COMPILER_H
#include "Parser.h"
class FacadeCompiler {
public:
	FacadeCompiler() {}
	~FacadeCompiler() {}
	virtual void Compile(std::istream& input, std::istream& output) {
		Scanner scanner(input);
		ProgramNodeBuilder builder;
		Parser parser;
		parser.Parse(scanner, builder);
		CodeGenerator generator(output);
		ProgramNode* parseTree = builder.GetRootNode();
		parseTree->Traverse(generator);
	}
};
#endif // !FACADE_COMPILER_H
