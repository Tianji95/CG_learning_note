#pragma once
#ifndef WIDGET_CHANGED_MEDIATOR_H
#define WIDGET_CHANGED_MEDIATOR_H
#include <windows.h>

#include "Widget.h"
#include "Mediator.h"
#include "Button.h"
#include "ListBox.h"
#include "TextArea.h"
class WidgetChangedMediator: public Mediator {
public:
	WidgetChangedMediator(){};
	virtual ~WidgetChangedMediator() {};
	virtual void WidgetChanged(Widget* widget) {
		if (widget == _ok) {
			_text->ChangeText("click ok");
		}
		else if (widget == _cancel) {
			_text->ChangeText("click cancel");
			_fontList->SetList({ 1,2,3,4 });
		}
		else if (widget == _fontList) {
			_ok->ShowButton();
			_cancel->ShowButton();
		}
		else if (_text == widget) {
			_ok->ShowButton();
			_cancel->ShowButton();
		}
	}
	virtual void CreateWidgets() {
		_ok = new Button(this);
		_cancel = new Button(this);
		_fontList = new ListBox(this);
		_text = new TextArea(this);
	}
	virtual void Loop() {
		int i = 0;
		while (true) {
			if (i % 5 == 0) {
				_ok->Changed();
			}
			if (i % 13 == 0) {
				_cancel->Changed();
			}
			if (i % 11 == 0) {
				_fontList->Changed();
			}
			if (i % 3 == 0) {
				_text->Changed();
			}
			Sleep(1000); // 0.1s
			i++;
		}
	}

private:
	Button* _ok;
	Button* _cancel;
	ListBox* _fontList;
	TextArea* _text;
};
#endif // !WIDGET_CHANGED_MEDIATOR_H