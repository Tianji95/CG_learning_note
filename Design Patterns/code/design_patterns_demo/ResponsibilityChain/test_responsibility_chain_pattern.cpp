#include "HelpMessageHandler.h"
#include "Button.h"
#include "Dialog.h"
int main() {
	Handler* button = new Button(nullptr);
	Handler* help = new HelpMessageHandler(button);
	Handler* dialog = new Dialog(help);
	Handler* handlerHead = new Handler(dialog);

	Request r;
	r.type = RequestType::BUTTON;
	handlerHead->HandleRequest(&r);

	r.type = RequestType::DIALOG;
	handlerHead->HandleRequest(&r);

	r.type = RequestType::HELP;
	handlerHead->HandleRequest(&r);

	r.type = RequestType::NONE;
	handlerHead->HandleRequest(&r);

}