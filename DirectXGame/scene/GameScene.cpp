#include <cassert>
#include "GameScene.h"
#include "TextureManager.h"

GameScene::GameScene() {}

GameScene::~GameScene() {
	delete blockmodel_;

	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			delete worldTransformBlock;
		}
	}
	worldTransformBlocks_.clear();
	delete debugCamera_;
	delete skydome_;
	delete modelSkydome_;
	delete mapChipField_;
	delete player_;
	delete modelPlayer_;
	for (Enemy*newEnemy:enemies_) {
		delete newEnemy;
	}
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	
	blockmodel_ = Model::CreateFromOBJ("block",true);

	viewProjection_.Initialize();

	//// skydomeの初期化
	//skydome_ = new Skydome();
	//// 天球_skydome
	//modelSkydome_ = Model::CreateFromOBJ("skydome", true);
	//skydome_->Initialize(modelSkydome_, &viewProjection_);

	// デバッグカメラの生成
	debugCamera_ = new DebugCamera(1280, 720);

	// マップチップ
	mapChipField_ = new MapChipField;
	mapChipField_->LoadMapChipCsv("MapData/map.csv");

	GenerateBlocks();

	//Player関連
	player_ = new Player;
	modelPlayer_ = Model::CreateFromOBJ("player", true);
	Vector3 playerPosition = mapChipField_->GetMapChipPositionByIndex(3, 18);
	player_->Initialize(modelPlayer_, &viewProjection_, playerPosition);
	player_->SetMapChipField(mapChipField_);
	//カメラコントロールの初期化
	cameraController_ = new CameraController;
	cameraController_->Initialize();
	cameraController_->SetTarget(player_);
	cameraController_->Reset();

	CameraController::Rect cameraArea = {11.0f, 100 - 12.0f, 6.0f, 6.0f};
	cameraController_->SetMovebleArea(cameraArea);
	
	#pragma region エネミー関連
	//エネミー関連
	enemyModel_ = Model::CreateFromOBJ("enemy", true);

	for (int32_t i = 0; i < 3; ++i) {
		Enemy* newEnemy = new Enemy();
		Vector3 enemyPosition = mapChipField_->GetMapChipPositionByIndex(20 + i + i, 18);
		newEnemy->Initialize(enemyModel_,&viewProjection_,enemyPosition);
		enemies_.push_back(newEnemy);
	}
	#pragma endregion
	//仮の生成処理
	deathParticles_ = new DeathParticles;
	deathModel_ = Model::CreateFromOBJ("deathParticle");
	deathParticles_->Initialize(deathModel_, &viewProjection_, playerPosition);
}

void GameScene::GenerateBlocks() {
	uint32_t NumBlockVirtial = mapChipField_->GetNumBlockVirtial();
	uint32_t NumBlockHorizontal = mapChipField_->GetNumBlockHorizontal();

	
	// 要素数を変更する
	// 列数を設定（縦方向のブロック数）
	worldTransformBlocks_.resize(NumBlockVirtial);
	for (uint32_t i = 0; i < NumBlockVirtial; ++i) {
		// 1列の要素数を設定（横方向のブロック数）
		worldTransformBlocks_[i].resize(NumBlockHorizontal);
	}
	for (uint32_t i = 0; i < NumBlockVirtial; ++i) {
		for (uint32_t j = 0; j < NumBlockHorizontal; ++j) {
			if (mapChipField_->GetMapChipTypeByIndex(j, i) == MapChipType::kBlock) {
				WorldTransform* worldTransform = new WorldTransform();
				worldTransform->Initialize();
				worldTransformBlocks_[i][j] = worldTransform;
				worldTransformBlocks_[i][j]->translation_ = mapChipField_->GetMapChipPositionByIndex(j, i);
				
			}
		}
	}
}
void GameScene::CheckAllCollisions() {

	#pragma region 自キャラと敵キャラの当たり判定
	//判定対象1と2の座標
	AABB aabb1, aabb2;
	//自キャラの座標
	aabb1 = player_->GetAABB();
	//自キャラと敵弾すべての当たり判定
	for (Enemy* enemy : enemies_) {
		//敵弾の座標
		aabb2 = enemy->GetAABB();
		//AABB同士の交差判定
		if (IsCollision(aabb1, aabb2)) {
			//自キャラの衝突時コールバックを呼び出す
			player_->OnCollision(enemy);
			//敵弾の衝突時コールバックを呼び出す
			enemy->OnCollision(player_);
		}
	}
	#pragma endregion

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
		viewProjection_.matView = cameraController_->GetViewProjection().matView;
		viewProjection_.matProjection = cameraController_->GetViewProjection().matProjection;
		viewProjection_.TransferMatrix();
		//viewProjection_.UpdateMatrix();
	}
#endif
	skydome_->Update();
	player_->Update();
	for (Enemy* newEnemy : enemies_) {
		newEnemy->Update();
	}
	cameraController_->Update();
	//全ての当たり判定を行う
	//一番最後で行うワールド座標で判定を取るため、各オブジェクトのワールド行列計算が終わった後である必要があるため
	CheckAllCollisions();
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

	// skydome_->Draw();

	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			if (!worldTransformBlock)
				continue;
			blockmodel_->Draw(*worldTransformBlock, viewProjection_);
		}
	}
	player_->Draw();
	for (Enemy* newEnemy : enemies_) {
		newEnemy->Draw();
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
