#pragma once
#include <WorldTransform.h>
#include <Model.h>
#include <cassert>
#include <numbers>
#include<MapChipField.h>
#include"Vector3.h"
#include <math.h>
class Enemy {
	//歩行の速さ
	static inline const float kWalkSpeed = 0.05f;
	//最初の角度[度]
	static inline const float kWalkMotionAngleStart = -0.3f;
	//最後の角度[度]
	static inline const float kWalkMotionAngleEnd = 1.0f;
	//アニメーションの周期となる時間[秒]
	static inline const float kWalkMotionTime = 1.0f;

public:
	void Initialize(Model* model, ViewProjection* viewProjection, const Vector3& position);
	void Update();
	void Draw();

	void SetMapChipField(MapChipField* mapChipField) { mapChipField_ = mapChipField; }
	float fLerp(float start, float end, float t) { return (1.0f - t) * start + t * end; }

private:

	Model* model_;
	WorldTransform worldTransform_;
	ViewProjection* viewProjection_ = nullptr;
	MapChipField* mapChipField_ = nullptr;

	Vector3 velocity_;

	float WalkTimer_ = 0.0f;

};

