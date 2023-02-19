#pragma once
#ifndef SIMPLE_COMMON_COMMAND_H
#define SIMPLE_COMMON_COMMAND_H
#include <list>
#include "Command.h"

class SimpleCommonCommand{
public:
	SimpleCommonCommand() {}
	virtual ~SimpleCommonCommand() {}
	virtual void Execute() {
		for (auto cmd : _cmds) {
			cmd->Execute();
		}
	}
	virtual void AddCommand(Command* cmd) {
		_cmds.push_back(cmd);
	}
	virtual void Undo() {
		_cmds.back()->Undo();
		_undoCmds.push_back(_cmds.back());
		_cmds.pop_back();
	}
	virtual void Redo() {
		_undoCmds.back()->Redo();
		_cmds.push_back(_undoCmds.back());
		_undoCmds.pop_back();
	}
private:
	std::list<Command*> _cmds;
	std::list<Command*> _undoCmds;
};
#endif // !SIMPLE_COMMON_COMMAND_H