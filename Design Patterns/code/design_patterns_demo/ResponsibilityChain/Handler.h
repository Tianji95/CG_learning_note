#pragma once
#ifndef HANDLER_H
#define HANDLER_H
#include<iostream>
#include "Request.h"
class Handler {
public:
	Handler() {}
	Handler(Handler* nxt) : next(nxt) {}
	virtual ~Handler() {}
	virtual void HandleRequest(Request* request) {
		std::cout << "Handler HandleRequest" << std::endl;

		if (next) {
			next->HandleRequest(request);
		}
	}

protected:
	Handler* next = nullptr;
};
#endif // !HANDLER_H
