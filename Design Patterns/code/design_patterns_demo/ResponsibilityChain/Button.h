#pragma once
#ifndef BUTTON_H
#define BUTTON_H
#include "Handler.h"
class Button : public Handler {
public:
	Button(){}
	Button(Handler* nxt) :Handler(nxt) {}

	virtual ~Button() {}
	virtual void HandleRequest(Request* request) {
		if (request->type == RequestType::BUTTON)
		    std::cout << "Button HandleRequest" << std::endl;
		if (next) {
			next->HandleRequest(request);
		}
	}
};
#endif // !BUTTON_H
