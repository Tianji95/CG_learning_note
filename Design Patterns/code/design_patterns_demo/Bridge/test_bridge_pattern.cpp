#include <iostream>
#include "IconWindow.h"
#include "XWindowImp.h"
int main() {
	IconWindow window(new XWindowImp());
	window.DrawIcon();
}