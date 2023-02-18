#pragma once
#ifndef APPLICATION_WINDOW_H
#define APPLICATION_WINDOW_H
#include "Window.h"
class ApplicationWindow : public Window {
public:
	virtual void SetExtent(float x, float y) {
		std::cout << "ApplicationWindow ImpSetExtent" << std::endl;
		WindowImp* imp = GetWindowImp();
		imp->ImpSetExtent(x, y);
	}
	virtual void SetOrigin(float x, float y) {
		std::cout << "ApplicationWindow ImpSetOrigin" << std::endl;
		WindowImp* imp = GetWindowImp();
		imp->ImpSetOrigin(x, y);
	}
	virtual void DrawRect(float fromX, float fromY, float toX, float toY) {
		std::cout << "ApplicationWindow DrawRect" << std::endl;
		WindowImp* imp = GetWindowImp();
		imp->DrawLine(fromX, fromY, fromX, toY);
		imp->DrawLine(fromX, fromY, toX, fromY);
		imp->DrawLine(toX, fromY, toX, toY);
		imp->DrawLine(fromX, toY, toX, toY);
	}
	virtual void DrawText(const std::string& s, float x, float y) {
		std::cout << "ApplicationWindow DrawText" << std::endl;
		WindowImp* imp = GetWindowImp();
		imp->DrawText(s, x, y);
	}
private:
};
#endif // !APPLICATION_WINDOW_H
