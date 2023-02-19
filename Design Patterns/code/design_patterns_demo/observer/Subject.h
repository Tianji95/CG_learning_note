#pragma once
#ifndef SUBJECT_H
#define SUBJECT_H
#include <unordered_set>
#include "Observer.h"
class Subject {
public:
	Subject() {}
	virtual ~Subject() {}
	virtual void AttachObserver(Observer* obs) {
		_observers.insert(obs);
	}
	virtual void DetachObserver(Observer* obs) {
		_observers.erase(obs);
	}
	virtual void Notify() {
		for (auto obj : _observers) {
			obj->Update();
		}
	}
private:
	std::unordered_set<Observer*> _observers;
};
#endif // !SUBJECT_H
