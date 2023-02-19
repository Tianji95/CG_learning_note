#include <vector>
#include "MyList.h"
#include "Item.h"
#include "MyIterator.h"

int main() {
	std::vector<int> test = {11, 12,13 };
	for (std::vector<int>::iterator iter = test.begin(); iter != test.end(); iter++) {
		std::cout << *iter << std::endl;
	}

	MyList<Item> items;
	items.Push_back(Item(3));
	items.Push_back(Item(4));
	items.Push_back(Item(5));
	ListIterator<Item> iterator(&items);
	for (iterator.Begin(); !iterator.IsEnd(); iterator.Next()) {
		iterator.Current().Print();
	}

}