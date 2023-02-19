#pragma once
#ifndef CARE_TAKER_H
#define CARE_TAKER_H
#include <vector>
#include "memento.h"
class Caretaker {
public:
	void Add(Memento* mmt) {
		_mementos.push_back(mmt);
	}
	Memento* Get(int idx) {
		if (idx >= _mementos.size()) {
			return nullptr;
		}
		return _mementos[idx];
	}
private:
	std::vector<Memento*> _mementos;
};
#endif // !CARE_TAKER_H
