#pragma once
#ifndef WINDOW_IMP_H
#define WINDOW_IMP_H
#include <string>
#include <iostream>
class WindowImp {
public:
	virtual void ImpSetExtent(float x, float y) = 0;
	virtual void ImpSetOrigin(float x, float y) = 0;
	virtual void DrawLine(float fromX, float fromY, float toX, float toY) = 0;
	virtual void DrawText(const std::string& s, float x, float y) = 0;
};
#endif // !WINDOW_IMP_H
