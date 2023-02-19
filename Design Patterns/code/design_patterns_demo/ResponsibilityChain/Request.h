#pragma once
#ifndef REQUEST_H
#define REQUEST_H
enum class RequestType {
	HELP = 1,
	BUTTON = 2,
	DIALOG = 3,
	NONE = 4
};
class Request {
public:
	RequestType type;
};
#endif // !REQUEST_H
