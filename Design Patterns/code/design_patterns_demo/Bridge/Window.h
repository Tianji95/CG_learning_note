#pragma once
#ifndef WINDOW_H
#define WINDOW_H
#include "WindowImp.h"

class Window {
public:
	Window() {}
	Window(WindowImp* imp) :_imp(imp) {}
	virtual ~Window() {}
	virtual void SetExtent(float x, float y) = 0;
	virtual void SetOrigin(float x, float y) = 0;
	virtual void DrawRect(float fromX, float fromY, float toX, float toY) = 0;
	virtual void DrawText(const std::string& s, float x, float y) = 0;
protected:
	WindowImp* GetWindowImp() {
		return _imp;
	}
private:
	WindowImp* _imp = nullptr;
};
#endif // !WINDOW_H
