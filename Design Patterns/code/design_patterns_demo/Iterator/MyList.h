#pragma once
#ifndef MY_LIST_H
#define MY_LIST_H
#include <list>
#include "MyIterator.h"
template<typename T>
class MyList {
public:
	T Get(unsigned int index){
		return _list[index];
	}
	void Push_back(T value) {
		_list.push_back(value);
	}
	bool IsEnd(unsigned int index) {
		return index >= _list.size();
	}
private:
	std::vector<T> _list;
};
template<typename T>
class ListIterator : public MyIterator<T> {
public:
	ListIterator(MyList<T>* aList) : _list(aList) {

	} 
	virtual void Begin()
	{
		_currentidx = 0;
	}
	virtual void Next() {
		_currentidx++;
	}
	virtual T Current() const {
		return _list->Get(_currentidx);
	}
	virtual bool IsEnd() const {
		return _list->IsEnd(_currentidx);
	}

private:
	MyList<T>* _list;
	int _currentidx = 0;
};



#endif // !MY_LIST_H
