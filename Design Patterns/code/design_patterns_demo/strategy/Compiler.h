#pragma once
#ifndef COMPILER_H
#define COMPILER_H

class Compiler {
public:
	virtual void Compile() = 0;
	virtual void SaveResult() = 0;
};
#endif // !COMPILER_H
