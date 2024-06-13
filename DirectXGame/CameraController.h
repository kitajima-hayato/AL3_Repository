#pragma once
#include "ViewProjection.h"
#include"MakeMatrix.h"

//矩形

class Player;
class CameraController {
public:
	struct Rect {
		float left = 0.0f;   // 左端
		float right = 1.0f;  // 右端
		float bottom = 0.0f; // 下端
		float top = 1.0f;    // 上端
	};
	void Initialize();
	void Update();
	void Reset();

	void SetTarget(Player* target) { target_ = target; }
	void SetMovebleArea(Rect area) { movableArea_ = area; }
	ViewProjection& GetViewProjection() { return viewProjection_; }
	

private:
	ViewProjection viewProjection_;
	Player* target_ = nullptr;
	//追従対象とカメラの座標の差(オフセット)
	Vector3 targetOffset_ = {0, 0, -15.0f};
	//カメラの移動範囲
	Rect movableArea_ = {0, 100, 0, 100};

	//カメラの目標座標
	Vector3 

};
