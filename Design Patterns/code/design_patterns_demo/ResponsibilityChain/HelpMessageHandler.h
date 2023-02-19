#pragma once
#ifndef HELP_MESSAGE_HANDLER_H
#define HELP_MESSAGE_HANDLER_H
#include "Handler.h"
class HelpMessageHandler :public Handler {
public:
	HelpMessageHandler() {}
	HelpMessageHandler(Handler* nxt) :Handler(nxt) {}

	virtual ~HelpMessageHandler() {}
	virtual void HandleRequest(Request* request) {
		if (request->type == RequestType::HELP)
		    std::cout << "HelpMessageHandler HandleRequest" << std::endl;

		if (next) {
			next->HandleRequest(request);
		}
	}
};
#endif // !HELP_MESSAGE_HANDLER_H