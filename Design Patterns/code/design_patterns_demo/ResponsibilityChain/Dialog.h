#pragma once
#ifndef DIALOG_H
#define DIALOG_H
#include "Handler.h"
class Dialog :public Handler{
public:
	Dialog(Handler* nxt) :Handler(nxt){}
	virtual ~Dialog() {}
	virtual void HandleRequest(Request* request) {
		if (request->type == RequestType::DIALOG)
			std::cout << "Dialog HandleRequest" << std::endl;

		if (next) {
			next->HandleRequest(request);
		}
	}
};
#endif // !DIALOG_H