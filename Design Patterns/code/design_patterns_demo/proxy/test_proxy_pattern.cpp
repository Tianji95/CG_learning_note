#include "MeshProxy.h"
int main() {
	std::string filename = "city.obj";
	std::cout << "---------proxy mesh-------------" << std::endl;
	Graphics* meshproxy = new ProxyMesh(filename);
	meshproxy->SetPosition(3, 5);
	meshproxy->SetPosition(5,6);
	meshproxy->Draw();
	std::cout << "---------traditional mesh-------------" << std::endl;
	Graphics* mesh = new Mesh(filename);
	mesh->SetPosition(3, 5);
	mesh->SetPosition(5, 6);
	mesh->Draw();
}