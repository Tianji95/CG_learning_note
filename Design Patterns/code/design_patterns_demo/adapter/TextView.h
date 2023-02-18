#pragma once
#ifndef TEXTVIEW_H
#define TEXTVIEW_H
class TextView {
public:
	virtual void SetOrigin(float inputx, float inputy) {
		centerX = inputx;
		centerY = inputy;
	}
	virtual void SetSize(float width, float height) {
		this->width = width;
		this->height = height;
	}
	virtual void GetSize(float& width, float& height) {
		width = this->width;
		height = this->height;
	}
	virtual void GetOrigin(float& x, float& y) {
		x = this->centerX;
		y = this->centerY;
	}
	virtual float CalculateArea() {
		return width * height;
	}
	TextView(){}
	virtual ~TextView(){}
private:
	float centerX = 0;
	float centerY = 0;
	float width = 0;
	float height = 0;
};
#endif // !TEXTVIEW_H
