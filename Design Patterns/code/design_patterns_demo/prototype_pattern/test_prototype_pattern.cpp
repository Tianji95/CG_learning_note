#include "PrototypeFactory.h"
#include "BoomDoor.h"
#include "BoomRoom.h"
int main() {
	ProtoTypeFactory proto(new BoomRoom(), new BoomDoor());

	auto d1 = proto.CreateDoor();
	auto d2 = proto.CreateDoor();
	auto d3 = proto.CreateDoor();
	auto r1 = proto.CreateRoom();
	auto r2 = proto.CreateRoom();
	auto r3 = proto.CreateRoom();
	d1->SetSize(45);
	d2->SetSize(45);
	d3->SetSize(45);
	r1->SetSize(45);
	r2->SetSize(45);
	r3->SetSize(45);


}