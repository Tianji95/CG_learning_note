#pragma once
#ifndef OPEN_COMMAND_H
#define OPEN_COMMAND_H
#include "Command.h"
class OpenCommand :public Command {
public:
	OpenCommand() {}
	virtual ~OpenCommand() {}
	virtual void Execute() {
		std::cout << "Execute OpenCommand" << std::endl;
	}
	virtual void Undo() {
		std::cout << "Undo OpenCommand" << std::endl;
	}
	virtual void Redo() {
		std::cout << "Redo OpenCommand" << std::endl;
	}
};
#endif // !OPEN_COMMAND_H