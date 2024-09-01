#pragma once
#include <cassert>
#include<array>
#include"Model.h"
#include"ViewProjection.h"
#include"WorldTransform.h"
#include"MakeMatrix.h"
#include <numbers>

class DeathParticles {
	//パーティクルの個数
	static inline const uint32_t kNumParticles = 8;

public:
	void Initialize(Model* model, ViewProjection* viewProjection, const Vector3& position);
	void Update();
	void Draw();
	bool IsFinished() const { return isFinished_; }

	bool Finish(bool finish);


private:
	Model* deathModel_ = nullptr;
	ViewProjection* viewProjection_ = nullptr;

	std::array<WorldTransform, kNumParticles> worldTransforms_;

	//存続時間(消滅までの時間)<秒>
	static inline const float kDuration = 1.5f;
	//移動の速さ
	static inline const float kSpeed = 0.05f;
	//分割した１個分の角度
	static inline const float kAngleUnit = 2 * std::numbers::pi_v<float> / kNumParticles;

	//終了フラグ
	bool isFinished_ = false;
	//経過時間カウント
	float counter_ = 0.0f;

	//色変更オブジェクト
	ObjectColor objectColor_;
	//色の数値
	Vector4 color_;
};

