#pragma once
#ifndef X_WINDOW_IMP_H
#define X_WINDOW_IMP_H
#include "WindowImp.h"
class XWindowImp :public WindowImp {
public:
	virtual void ImpSetExtent(float x, float y)
	{
		std::cout << "XWindowImp ImpSetExtent" << std::endl;
	}
	virtual void ImpSetOrigin(float x, float y) {
		std::cout << "XWindowImp ImpSetOrigin" << std::endl;
	}
	virtual void DrawLine(float fromX, float fromY, float toX, float toY) {
		std::cout << "XWindowImp DrawLine" << std::endl;
	}
	virtual void DrawText(const std::string& s, float x, float y) {
		std::cout << "XWindowImp DrawText" << std::endl;
		XDrawText(s, x, y);
	}
private:
	// 这个也很重要，相当于imp将具体实现封装到自己内部，public的接口是为了保证和WindowImp一致。
	virtual void XDrawText(const std::string& s, float x, float y) {
		std::cout << "XWindowImp XDrawText" << std::endl;
	}
};
#endif // !X_WINDOW_IMP_H
