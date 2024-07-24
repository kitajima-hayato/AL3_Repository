#include "TitleScene.h"

TitleScene::TitleScene() {}

TitleScene::~TitleScene() { 
	delete model_; 
}

void TitleScene::Initialize() { 
	dxCommon_ = DirectXCommon::GetInstance();
	model_ = Model::CreateFromOBJ("Title", true);
	


	//assert(model_);
	viewProjection_.Initialize();
	worldTransform_.Initialize();
	


}

void TitleScene::Update() {
	
	//スペースを押したらタイトルからゲームシーンへ
	if (Input::GetInstance()->PushKey(DIK_SPACE)) {
		finished_ = true;
	}
	worldTransform_.scale_ = {0.5f, 0.5f, 0.5f};
	worldTransform_.TransferMatrix();
	worldTransform_.UpdateMatrix();
}

void TitleScene::Draw() { 
	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();
	Model::PreDraw(commandList);
	model_->Draw(worldTransform_, viewProjection_);
	Model::PostDraw();
}
