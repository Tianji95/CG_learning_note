#pragma once
#include <iostream>
#include "MainLooper.h"

class GameLooper :public MainLooper {
protected:
	virtual void UpdateLogic() {
		std::cout << "GameLooper UpdateLogic" << std::endl;

	}
	virtual void UpdatePhysics() {
		std::cout << "GameLooper UpdatePhysics" << std::endl;
	}
	virtual void UpdateAnimation() {
		std::cout << "GameLooper UpdateAnimation" << std::endl;
	}
	virtual void UpdateRender() {
		std::cout << "GameLooper UpdateRender" << std::endl;
	}

};