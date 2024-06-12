#pragma once
#include "ViewProjection.h"

class Player;
class CameraControleer {

private:
	ViewProjection viewProjection_;
	Player* target_ = nullptr;

public:
	void Initialize(ViewProjection viewProjection_);
	void Update();

	void SetTarget(Player* target) { target_ = target; }
};
