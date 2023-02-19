#include "SimpleCommonCommand.h"
#include "OpenCommand.h"
#include "PasteCommand.h"
int main() {
	SimpleCommonCommand cmds;
	Command* pcmd = new PasteCommand();
	Command* ocmd = new OpenCommand();
	cmds.AddCommand(pcmd);
	cmds.AddCommand(ocmd);
	cmds.Execute();
	cmds.Undo();
	cmds.Redo();
	cmds.Undo();
	cmds.Undo();
	cmds.Redo();
}