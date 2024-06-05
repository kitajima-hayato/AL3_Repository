#pragma once
#include<WorldTransform.h>
#include<Model.h>
#include<cassert>
#include<numbers>
#include <Input.h>

class Player {
public:
	Player();
	~Player();
	void Initialize(Model* model,ViewProjection* viewProjection,const Vector3&position);
	void Update();
	void Draw();

private:
	ViewProjection* viewProjection_ = nullptr;
	WorldTransform worldTransform_;
	Model* model_;
	uint32_t textureHandle_ = 0u;
	Vector3 velocity_ = {};
	static inline const float kAcceleration = 3;

};
