#include "StandardHouseBuilder.h"
#include "CounterHouseBuilder.h"
int main() {
	CounterHouseBuilder builder;
	builder.CreateHouse();
	builder.CreateRoom(14);
	builder.CreateRoom(10);
	builder.CreateRoom(13);
	builder.CreateDoor(21);
	builder.CreateDoor(20);
	House* house = builder.GetHouse();
	int roomcnt = 0;
	int doorcnt = 0;
	builder.GetCount(roomcnt, doorcnt);
	std::cout << "roomcnt:" << roomcnt << ", doorcnt:" << doorcnt << std::endl;
}