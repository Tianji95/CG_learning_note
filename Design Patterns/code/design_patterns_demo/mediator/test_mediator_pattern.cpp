#include "WidgetChangedMediator.h"
int main() {
	WidgetChangedMediator mediator;
	mediator.CreateWidgets();
	mediator.Loop();
}