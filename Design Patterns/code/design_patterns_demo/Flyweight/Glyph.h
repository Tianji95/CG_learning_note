#pragma once
#ifndef GLYPH_H
#define GLYPH_H
#include <iostream>
#include "GlyphContext.h"
#include "Font.h"

class Glyph {
public:
	Glyph() {}
	Glyph(char c): _character(c), _ctx(nullptr){
		static int cnt = 0;
		cnt++;
		std::cout << "create glyph :" << cnt << std::endl;
	}
	virtual ~Glyph() {}
	virtual void Draw() {
		int x, y;
		_ctx->GetPosition(x, y);
		std::cout << "draw " << _character << " position: " << x << ", " << y << " color: " << _ctx->GetFont()->GetColor() << std::endl;
 	}
	virtual void SetContext(GlyphContext* context) {
		_ctx = context;
	}

private:
	GlyphContext* _ctx;
	char _character;
};
#endif