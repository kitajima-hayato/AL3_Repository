#pragma once
#include <cassert>
#include<array>
#include"Model.h"
#include"ViewProjection.h"

class DeathParticles {
	//パーティクルの個数
	static inline const uint32_t kNumParticles = 8;

public:
	void Initialize(Model* model, ViewProjection* viewProjection, const Vector3& position);
	void Update();
	void Draw();

private:


	Model* deathModel_ = nullptr;
	ViewProjection* viewProjection_ = nullptr;

	std::array<WorldTransform, kNumParticles> worldTransforms_;





};

