#include <vector>
#include "GlyphFactory.h"
#include "GlyphContext.h"


int main() {
	std::string str = "this is a test string, we use flyweight pattern store glyph, font and position are easy to change, while there are only 26 characters";
	GlyphContext context;
	GlyphFactory factory;
	Font* red = new Font("red");
	Font* blue = new Font("blue");
	Font* yellow = new Font("yellow");
	Font* green = new Font("green");
	std::vector<Font*> fontList = { red, blue , yellow ,green };
	int fontidx = 0;
	for (int i = 0; i < str.size(); i++) {
		if (i % 5 == 0) {
			fontidx = (fontidx + 1) % fontList.size();
			context.SetFont(fontList[fontidx]);
		}
		if (i % 3 == 0) {
			context.SetPosition(i, i);
		}
		Glyph* gly = factory.GetGlyph(str[i]);
		gly->SetContext(&context);
		gly->Draw();
	}
}