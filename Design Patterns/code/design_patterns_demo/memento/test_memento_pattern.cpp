#include "Originator.h"
#include "Caretaker.h"

int main() {
	Originator ori;
	ori.setState("state 1"); ori.getState();
	ori.SaveStateToMemento(); ori.getState();
	ori.setState("state 2"); ori.getState();
	ori.setState("state 3"); ori.getState();
	ori.SaveStateToMemento(); ori.getState();
	ori.setState("state 4"); ori.getState();
	ori.SetStateFromMemento(0); ori.getState();
	ori.SetStateFromMemento(1); ori.getState();


}