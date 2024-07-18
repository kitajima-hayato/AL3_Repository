#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include"DebugCamera.h"
#include"Skydome.h"
#include"MapChipField.h"
#include<vector>
#include"Player.h"
#include"CameraController.h"
#include"Enemy.h"
#include"DeathParticles.h"
/// <summary>
/// ゲームシーン
/// </summary>

//ゲームのフェーズ(型)
enum class Phase {
	kPlay,	//ゲームプレイ
	kDeath,	//デス演出
};

class GameScene {

public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	void GenerateBlocks();
	//全ての当たり判定を行う
	void CheckAllCollisions();

	void ChangePhase();

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	uint32_t textureHandle_ = 0;
	
	Model* blockmodel_ = nullptr;
	
	ViewProjection viewProjection_;

	std::vector<std::vector<WorldTransform*>> worldTransformBlocks_;

	bool isDebugCameraActive_ = false;
	DebugCamera* debugCamera_ = nullptr;
	//天球skydome
	Skydome* skydome_ = nullptr;
	Model* modelSkydome_ = nullptr;

	//マップチップフィールド
	MapChipField* mapChipField_;

	Player* player_ = nullptr;
	Model* modelPlayer_ = nullptr;

	CameraController* cameraController_;

	std::list<Enemy*> enemies_;
	Model* enemyModel_ = nullptr;

	std::list<DeathParticles*> deathParticles_ ;
	Model* deathModel_ = nullptr;

	//ゲームの現在フェーズ(変数)
	Phase phase_;

	/// <summary>
	/// ゲームシーン用
	/// </summary>
};
