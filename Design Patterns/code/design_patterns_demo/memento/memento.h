#pragma once
#ifndef MEMENTO_H
#define MEMENTO_H
#include <string>
class Memento {
public:
	Memento(std::string state) : _state(state) {}
	std::string GetState() {
		return _state;
	}
private:
	std::string _state;
};

#endif // !MEMENTO_H
