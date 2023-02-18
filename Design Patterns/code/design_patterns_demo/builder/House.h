#pragma once
#ifndef HOUSE_H
#define HOUSE_H
#include <unordered_map>
#include <iostream>
#include "Room.h"
#include "Door.h"
class House {
public:
	void AttachRoom(Room* room, int roomid) {
		std::cout << "room attach to Room id: " << roomid << std::endl;
		_roomMap[roomid] = room;
	}
	void AttachDoor(Door* door, int doorid) {
		std::cout << "door attach to Door id: " << doorid << std::endl;
		_doorMap[doorid] = door;
	}
	
private:
	std::unordered_map<int, Room*> _roomMap;
	std::unordered_map<int, Door*> _doorMap;
};
#endif // !HOUSE_H
