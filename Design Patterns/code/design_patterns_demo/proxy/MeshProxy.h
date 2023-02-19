#pragma once
#include "Mesh.h"

class ProxyMesh : public Graphics {
public:
	ProxyMesh() {}
	ProxyMesh(std::string filename) : _filename(filename),_mesh(nullptr){
		std::cout << "Light Mesh Create: " << filename << std::endl;
	}
	virtual void Draw() {
		if (_mesh == nullptr) {
			_mesh = new Mesh(_filename);
		}
		_mesh->Draw();
	}
	virtual Mesh* GetMesh() {
		if (_mesh == nullptr) {
			_mesh = new Mesh(_filename);
		}
		return _mesh;
	}
private:
    // ������mesh�����ı�Ҫ��Ϣ��״̬����������
	Mesh* _mesh;
	std::string _filename;

};