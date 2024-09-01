#include "TitleScene.h"
#include <TextureManager.h>

TitleScene::TitleScene() { 
	

}

TitleScene::~TitleScene() { 
	delete model_; 
	delete skydome_;
	delete spriteTitle_;
}

void TitleScene::Initialize() { 
	dxCommon_ = DirectXCommon::GetInstance();
	model_ = Model::CreateFromOBJ("Title", true);
	


	//assert(model_);
	viewProjection_.Initialize();
	worldTransform_.Initialize();
	
	skydome_ = new Skydome();
	skyModel_->Model::CreateFromOBJ("skydome", true);
	skydome_->Initialize(skyModel_,&viewProjection_);

	spriteTitle_ = new Sprite();
	textureHandleTitle_ = TextureManager::Load("Title.png");
	spriteTitle_ = Sprite::Create(textureHandleTitle_, {0, 0});
}

void TitleScene::Update() {
	//skydome_->Update();
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
	Sprite::PreDraw(commandList);
	spriteTitle_->Draw();
	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
	Model::PreDraw(commandList);
	
	model_->Draw(worldTransform_, viewProjection_);
	Model::PostDraw();
}
