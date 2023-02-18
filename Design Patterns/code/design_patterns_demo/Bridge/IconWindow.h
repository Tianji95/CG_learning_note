#pragma once
#ifndef ICON_WINDOW_H
#define ICON_WINDOW_H
#include "Window.h"

class IconWindow : public Window {
public:
	IconWindow() {}
	IconWindow(WindowImp* img) :Window(img) {}
	virtual ~IconWindow(){}
	virtual void SetExtent(float x, float y) {
		std::cout << "IconWindow ImpSetExtent" << std::endl;
		WindowImp* imp = GetWindowImp();
		imp->ImpSetExtent(x, y);
	}
	virtual void SetOrigin(float x, float y) {
		std::cout << "IconWindow ImpSetOrigin" << std::endl;
		WindowImp* imp = GetWindowImp();
		imp->ImpSetOrigin(x, y);
	}
	virtual void DrawRect(float fromX, float fromY, float toX, float toY) {
		std::cout << "IconWindow DrawRect" << std::endl;
		WindowImp* imp = GetWindowImp();
		imp->DrawLine(fromX, fromY, fromX, toY);
		imp->DrawLine(fromX, fromY, toX, fromY);
		imp->DrawLine(toX, fromY, toX, toY);
		imp->DrawLine(fromX, toY, toX, toY);
	}
	virtual void DrawText(const std::string& s, float x, float y) {
		std::cout << "IconWindow DrawText" << std::endl;
		WindowImp* imp = GetWindowImp();
		imp->DrawText(s, x, y);
	}
    //�������������Ҫ�ģ�Ҳ��IconWindow���صĹ��ܣ�Bridgeģʽ��Ҫ�ǰѳ���ӿڣ�Window�ࣩ�͵ײ�ʵ�֣�WindowImp�ࣩ���ֿ������һ��Implementor�ӿ�ֻ�ṩ������������Abstraction�ṩ�˸߲�εĲ�����
	virtual void DrawIcon() {
		std::cout << "IconWindow DrawIcon" << std::endl;
		WindowImp* imp = GetWindowImp();
		imp->DrawText("drawIcon", 0, 0);
	}
private:
};

#endif // !ICON_WINDOW_H
