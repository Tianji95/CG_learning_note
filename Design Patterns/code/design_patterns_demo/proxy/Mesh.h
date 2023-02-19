#pragma once
#ifndef MESH_H
#define MESH_H
#include <string>
#include "Graphics.h"
class Mesh :public Graphics {
public:
	Mesh(std::string filename) {
		std::cout << "A Very Heavy Mesh Create: "<< filename << std::endl;
	}
	virtual void Draw() {
		std::cout << "Draw Mesh" << std::endl;
	}
};
#endif // !MESH_H
