#pragma once
#ifndef GLYPH_CONTEXT_H
#define GLYPH_CONTEXT_H
#include "Font.h"
class GlyphContext {
public:
	virtual void SetFont(Font* font) {
		_font = font;
	}
	virtual void SetPosition(int x, int y) {
		posX = x;
		posY = y;
	}
	virtual void GetPosition(int& x, int& y) {
		x = posX;
		y = posY;
	}
	virtual Font* GetFont() {
		return _font;
	}
private:
	Font* _font;
	int posX = 0;
	int posY = 0;
};
#endif // !GLYPH_CONTEXT_H
