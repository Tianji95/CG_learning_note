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
	// ���Ҳ����Ҫ���൱��imp������ʵ�ַ�װ���Լ��ڲ���public�Ľӿ���Ϊ�˱�֤��WindowImpһ�¡�
	virtual void XDrawText(const std::string& s, float x, float y) {
		std::cout << "XWindowImp XDrawText" << std::endl;
	}
};
#endif // !X_WINDOW_IMP_H
