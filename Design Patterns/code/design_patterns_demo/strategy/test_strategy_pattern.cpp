#include "ComplexCompiler.h"
#include "SimpleCompiler.h"
#include "Strategy.h"
int main() {
	Strategy* com = new Strategy(new ComplexCompiler());
	Strategy* com2 = new Strategy(new SimpleCompiler());
	com->Compile();
	com->SaveResult();
	com2->Compile();
	com2->SaveResult();
}