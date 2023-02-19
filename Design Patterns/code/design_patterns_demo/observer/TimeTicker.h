#pragma once
#ifndef TIME_TICKER_H
#define TIME_TICKER_H
#include "Subject.h"
class TimeTicker :public Subject {
public:
	void Tick() {
		Notify();
	}
};
#endif // !TIME_TICKER_H
