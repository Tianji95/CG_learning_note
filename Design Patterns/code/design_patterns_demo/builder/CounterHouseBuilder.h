#pragma once
#ifndef COUNTER_HOUSE_BUILDER_H
#define COUNTER_HOUSE_BUILDER_H
#include "HouseBuilder.h"
class CounterHouseBuilder : public HouseBuilder {
public:
	virtual void CreateHouse() {
		_house = new House();
	}
	virtual void CreateRoom(int roomid) {
		roomcount += 1;
	}
	virtual void CreateDoor(int doorid) {
		doorcount += 1;
	}
	virtual House* GetHouse() {
		return _house; 
	}
	void GetCount(int& roomcnt, int& doorcnt) {
		roomcnt = roomcount;
		doorcnt = doorcount;
	}
private:
	House* _house = nullptr;
	int roomcount = 0;
	int doorcount = 0;
};
#endif // !COUNTER_HOUSE_BUILDER_H
