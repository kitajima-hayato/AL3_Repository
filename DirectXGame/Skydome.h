#pragma once
#include <cassert>
#include"Model.h"
#include"WorldTransform.h"
#include"ViewProjection.h"


class Skydome {


public:
	Skydome();
	~Skydome();
	void Initialize(Model* model, ViewProjection* viewprojection);
	void Update();
	void Draw();

private:
	//ワールド変換データ
	WorldTransform worldTransform_;
	ViewProjection* viewProjection_ = nullptr;
	//モデル
	Model* model_ = nullptr;
	uint32_t textureHandle_ = 0u;
};
