#pragma once
#ifndef PASTE_COMMAND_H
#define PASTE_COMMAND_H
#include "Command.h"
class PasteCommand :public Command {
public:
	PasteCommand() {}
	virtual ~PasteCommand() {}
	virtual void Execute() {
		std::cout << "Execute PasteCommand" << std::endl;
	}
	virtual void Undo() {
		std::cout << "Undo PasteCommand" << std::endl;
	}
	virtual void Redo() {
		std::cout << "Redo PasteCommand" << std::endl;
	}
};
#endif // !PASTE_COMMAND_H