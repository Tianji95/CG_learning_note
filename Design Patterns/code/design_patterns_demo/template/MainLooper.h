#pragma once
class MainLooper {
public:
	MainLooper() {}
	~MainLooper() {}
	virtual void Loop() {
		UpdateLogic();
		UpdatePhysics();
		UpdateAnimation();
		UpdateRender();
	}
protected:
	virtual void UpdateLogic() = 0;
	virtual void UpdatePhysics() = 0;
	virtual void UpdateAnimation() = 0;
	virtual void UpdateRender() = 0;
};