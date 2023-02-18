#pragma once
#ifndef TEXT_VIEW_BOX_ADAPTER_H
#define TEXT_VIEW_BOX_ADAPTER_H
#include "TextView.h"
#include "Box.h"

class TextViewBoxAdapter : public TextView {
public:
	TextViewBoxAdapter() {}
	TextViewBoxAdapter(Box* b) {
		box = b;
	}
	virtual void SetOrigin(Box* b) {
		box = b;
	}
	virtual void SetSize(Box* b) {
		box = b;
	}
	virtual void GetSize(float& width, float& height) {
		float obtleftX;
		float obtleftY;
		float otoprtX;
		float otoprtY;
		box->GetPosition(obtleftX, obtleftY, otoprtX, otoprtY);
		width = abs(otoprtX - obtleftX);
		height = abs(obtleftY - otoprtY);
	}
	virtual void GetOrigin(float& x, float& y) {
		float obtleftX;
		float obtleftY;
		float otoprtX;
		float otoprtY;
		box->GetPosition(obtleftX, obtleftY, otoprtX, otoprtY);
		x = (obtleftX + otoprtX) / 2;
		y = (obtleftY + otoprtY) / 2;
	}
	virtual float CalculateArea() {
		float obtleftX;
		float obtleftY;
		float otoprtX;
		float otoprtY;
		box->GetPosition(obtleftX, obtleftY, otoprtX, otoprtY);
		float width = abs(otoprtX - obtleftX);
		float height = abs(obtleftY - otoprtY);
		return width * height;
	}
private:
	Box* box;
};


#endif // !TEXT_VIEW_BOX_ADAPTER_H

