#pragma once
#include<WorldTransform.h>
#include<Model.h>
#include<cassert>

class Player {
public:
	Player();
	~Player();
	void Initialize(Model* model, uint32_t textureHandle,ViewProjection* viewProjection);
	void Update();
	void Draw();

private:
	ViewProjection* viewProjection_ = nullptr;
	WorldTransform worldTransform_;
	Model* model_;
	uint32_t textureHandle_ = 0u;


};
