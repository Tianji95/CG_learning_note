#pragma once
#ifndef ITEM_H
#define ITEM_H
#include <iostream>
class Item {
public:
	Item() {}
	Item(int v) : value(v){}
	~Item() {}
	void Print() {
		std::cout << "value: " << value << std::endl;
	}
private:
	int value;
};

#endif // !ITEM_H
