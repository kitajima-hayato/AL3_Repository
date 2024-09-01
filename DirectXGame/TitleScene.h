#pragma once
#include <Input.h>
#include <cassert>
#include <DirectXCommon.h>
#include"Model.h"
#include"ViewProjection.h"
#include"WorldTransform.h"
#include "Skydome.h"
#include "Sprite.h"
class TitleScene {


public:
	TitleScene();
	~TitleScene();
	void Initialize();
	void Update();
	void Draw();
	// デスフラグ
	bool IsFinished() const { return finished_; }

private:
	DirectXCommon* dxCommon_ = nullptr;
	Model* model_;
	ViewProjection viewProjection_;
	WorldTransform worldTransform_;
	Skydome* skydome_ = nullptr;
	Model* skyModel_ = nullptr;

	Sprite* spriteTitle_ = nullptr;
	uint32_t textureHandleTitle_ = 0;
	//終了フラグ
	bool finished_ = false;
	
	

};
