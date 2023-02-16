#pragma once
#ifndef ROOM_H
class Room {
public:
	Room() {}
	virtual ~Room(){	}
private:
	float x;
	float y;
	float size;
};

#endif // !ROOM_H
