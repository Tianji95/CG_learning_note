#pragma once
#ifndef GLYPH_FACTORY_H
#define GLYPH_FACTORY_H
#include <unordered_map>
#include "Glyph.h"

class GlyphFactory {
public:
	GlyphFactory() {}
	virtual ~GlyphFactory() {}
	virtual Glyph* GetGlyph(char c) {
		if (glyphMap.find(c) == glyphMap.end()) {
			glyphMap[c] = new Glyph(c);
		}
		return glyphMap[c];
	}
private:
	std::unordered_map<char, Glyph*> glyphMap;
};
#endif // !GLYPH_FACTORY_H
