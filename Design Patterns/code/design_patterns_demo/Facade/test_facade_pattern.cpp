
#include <fstream>
#include "FacadeCompiler.h"
int main() {
	FacadeCompiler compiler;

	std::filebuf in;
	std::filebuf out;
	std::istream input(&in);
	std::istream outs(&out);
	compiler.Compile(input, outs);
}