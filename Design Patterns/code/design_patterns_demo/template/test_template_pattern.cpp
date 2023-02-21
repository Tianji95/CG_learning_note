#include "gamelooper.h"
int main() {
	MainLooper* loop = new GameLooper();
	loop->Loop();
}