#include "Widget.h"
#include "Mediator.h"
void Widget::Changed() {
	_mediator->WidgetChanged(this);
}
Widget::Widget(Mediator* mediator) {
	_mediator = mediator;
}