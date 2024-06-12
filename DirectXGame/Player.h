#pragma once
#include <Input.h>
#include <Model.h>
#include <WorldTransform.h>
#include <algorithm>
#include <cassert>
#include <numbers>

#include <corecrt_math.h>
#include <corecrt_math_defines.h>
#include <math.h>
enum class LRDirection {
	kRight,
	kLeft,
};
class Player {
public:
	Player();
	~Player();
	void Initialize(Model* model, ViewProjection* viewProjection, const Vector3& position);
	void Update();
	void Draw();

	

	const WorldTransform& GetWorldTransform() const{ return worldTransform_; }
	

private:
	WorldTransform worldTransform_;
	ViewProjection* viewProjection_ = nullptr;

	Model* model_;
	uint32_t textureHandle_ = 0u;
	Vector3 velocity_ = {};

	static inline const float kAcceleration = 0.05f;
	static inline const float kAttenuation = 0.05f;
	static inline const float kLimitRunSpeed = 0.3f;

	LRDirection lrDirection_ = LRDirection::kRight;

	// 旋回開始時の角度
	float turnFirstRotationY_ = 0.0f;
	// 旋回タイマー
	float turnTimer_ = 0.0f;
	// 旋回時間<秒>
	static inline const float kTimeTurn = 0.5f;

	float easeInOutSine(float x) { return cosf(((float)M_PI * x) / 2); }
	float EaseOutSine(float x) { return cosf((x * (float)M_PI) / 2); }
	float easeInCubic(float x) { return x * x * x; }

	// 設置状態フラグ
	bool onGround_ = true;
	// 重力加速度(下)
	static inline const float kGravityAcceleration = 0.5f;
	// 最大落下速度
	static inline const float kLimitFallSpeed = 0.2f;
	// ジャンプ初速
	static inline const float kJumpAcceleration = 0.5f;

	// atinationでok
};
