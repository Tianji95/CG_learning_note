#pragma once
#ifndef ORIGINATOR_H
#define ORIGINATOR_H
#include <iostream>
#include "memento.h"
#include "Caretaker.h"
class Originator {
public:
	void SaveStateToMemento() {
		Memento* mmt = new Memento(_state);
		_caretaker.Add(mmt);
	}
	void SetStateFromMemento(int idx) {
		Memento* mmt = _caretaker.Get(idx);
		_state = mmt->GetState();
	}
	void setState(std::string state) {
		_state = state;
	}
	std::string getState() {
		std::cout << _state << std::endl;
		return _state;
	}
private:
	std::string _state;
	Caretaker _caretaker;
};
#endif // !ORIGINATOR_H
