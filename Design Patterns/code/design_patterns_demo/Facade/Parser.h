#pragma once
#ifndef PARSER_H
#define PARSER_H
#include "Scanner.h"
#include "ProgramNodeBuilder.h"
class Parser {
public:
	Parser() {}
	virtual ~Parser() {}
	virtual void Parse(Scanner& scan, ProgramNodeBuilder&) {}
};
#endif // !PARSER_H
