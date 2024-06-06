#pragma once
#include<WorldTransform.h>
#include<Model.h>
#include<cassert>
#include<numbers>
#include <Input.h>
#include<algorithm>
enum class LRDirection {
	kRight,
	kLeft,
};
class Player {
public:
	Player();
	~Player();
	void Initialize(Model* model,ViewProjection* viewProjection,const Vector3&position);
	void Update();
	void Draw();

private:
	WorldTransform worldTransform_;
	ViewProjection* viewProjection_ = nullptr;

	Model* model_;
	uint32_t textureHandle_ = 0u;
	Vector3 velocity_ = {};

	static inline const float kAcceleration = 0.05f;
	static inline const float kAttenuation = 0.01f;
	static inline const float kLimitRunSpeed = 0.5f;

	LRDirection lrDirection_ = LRDirection::kRight;

	//旋回開始時の角度
	float turnFirstRotationY_ = 0.0f;
	//旋回タイマー
	float turnTimer_ = 0.0f;
	//旋回時間<秒>
	static inline const float kTimeTurn = 0.3f;

	//atinationでok
};
