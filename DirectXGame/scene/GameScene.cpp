#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>

GameScene::GameScene() {}

GameScene::~GameScene() {
	delete model_;

	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			delete worldTransformBlock;
		}
	}
	worldTransformBlocks_.clear();
	delete debugCamera_;
	delete skydome_;
	delete modelSkydome_;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	textureHandle_ = TextureManager::Load("mario.jpg");
	model_ = Model::Create();

	viewProjection_.Initialize();

	// skydomeの初期化
	skydome_ = new Skydome();
	// 天球_skydome
	modelSkydome_ = Model::CreateFromOBJ("skydome", true);
	skydome_->Initialize(modelSkydome_, &viewProjection_);

	const uint32_t kNumBlockVirtial = 20;
	const uint32_t kNumBlockHorizontal = 20;
	// ブロック一個分の横幅
	const float kBlockWidth = 2.0f;
	const float kBlockHeigh = 2.0f;
	// 要素数を変更する
	// 列数を設定（縦方向のブロック数）
	worldTransformBlocks_.resize(kNumBlockVirtial);
	for (uint32_t i = 0; i < kNumBlockVirtial; ++i) {
		// 1列の要素数を設定（横方向のブロック数）
		worldTransformBlocks_[i].resize(kNumBlockHorizontal);
	}
	for (uint32_t i = 0; i < kNumBlockVirtial; ++i) {
		for (uint32_t j = 0; j < kNumBlockHorizontal; ++j) {
			worldTransformBlocks_[i][j] = new WorldTransform();
			worldTransformBlocks_[i][j]->Initialize();
			if (j % 2 == 0 && i % 2 == 0) {
				worldTransformBlocks_[i][j]->translation_.x = kBlockWidth * i;
				worldTransformBlocks_[i][j]->translation_.y = kBlockHeigh * j;
			}
			if (j % 2 != 0 && i % 2 != 0) {
				worldTransformBlocks_[i][j]->translation_.x = kBlockWidth * i;
				worldTransformBlocks_[i][j]->translation_.y = kBlockHeigh * j;
			}
		}
	}
	

	// デバッグカメラの生成
	debugCamera_ = new DebugCamera(1280, 720);
	
}

void GameScene::Update() {
	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			if (!worldTransformBlock)
				continue;

			worldTransformBlock->UpdateMatrix();
			worldTransformBlock->TransferMatrix();
		}
	}

	debugCamera_->Update();
#ifdef _DEBUG
	if (input_->TriggerKey(DIK_SPACE)) {
		isDebugCameraActive_ = true;
	}
	if (isDebugCameraActive_) {
		debugCamera_->Update();
		viewProjection_.matView = debugCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = debugCamera_->GetViewProjection().matProjection;
		// ビュープロジェクション行列の転送
		viewProjection_.TransferMatrix();
	} else {
		// ビュープロジェクション行列の更新と転送
		viewProjection_.UpdateMatrix();
	}
#endif
	skydome_->Update();
}
void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>
	
	skydome_->Draw();

	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			if (!worldTransformBlock)
				continue;
			//model_->Draw(*worldTransformBlock, viewProjection_);
		}
	}

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる

	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}
