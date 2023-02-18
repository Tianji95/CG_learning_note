#include <iostream>
#include "TextViewBoxAdapter.h"

int main() {
	Box b;
	std::pair<float, float> btleft = std::make_pair(1, 0);
	std::pair<float, float> topright = std::make_pair(3, 4);
	b.SetPosition(btleft, topright);
	TextViewBoxAdapter adapter(&b);
	float area = adapter.CalculateArea();
	std::cout << "area:" << area << std::endl;
}