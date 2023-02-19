#pragma once
#ifndef MY_ITERATOR_H
#define MY_ITERATOR_H

template<typename T>
class MyIterator {
public:
	virtual void Begin() = 0;
	virtual void Next() = 0;
	virtual T Current() const = 0;
	virtual bool IsEnd() const = 0;
};
#endif // !MY_ITERATOR_H
