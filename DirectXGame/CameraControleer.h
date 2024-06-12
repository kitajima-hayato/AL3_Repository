#pragma once
#include "ViewProjection.h"

class Player;
class CameraControleer {

private:
	ViewProjection viewProjection_;
	Player* target_ = nullptr;
	//追従対象とカメラの座標の差(オフセット)
	Vector3 targetOffset_ = {0, 0, -15.0f};


public:
	void Initialize();
	void Update();
	void Reset();

	void SetTarget(Player* target) { target_ = target; }
};
