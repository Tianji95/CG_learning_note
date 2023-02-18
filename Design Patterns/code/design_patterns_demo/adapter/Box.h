#pragma once
#ifndef BOX_H
#define BOX_H
#include <array>
class Box { 
public:
	void SetPosition(const std::pair<float, float>& btleft, const std::pair<float, float>& toprt) {
		bottomleft = btleft;
		topright = toprt;
	}
	void GetPosition(float& obtleftX, float& obtleftY, float& otoprtX, float& otoprtY) {
		obtleftX = bottomleft.first;
		obtleftY = bottomleft.second;
		otoprtX = topright.first;
		otoprtY = topright.second;
	}
private:
	std::pair<float, float> bottomleft;
	std::pair<float, float> topright;
};
#endif // !BOX_H
