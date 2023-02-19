#include "TimeTicker.h"
#include "TimeWidgetObserver.h"
int main() {
	Subject* ticker = new TimeTicker();
	Observer* widget = new TimeWidgetObserver();
	ticker->AttachObserver(widget);
	ticker->Notify();
}