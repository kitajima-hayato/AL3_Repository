#include "Player.h"

Player::Player(){};
Player::~Player() {}

void Player::Initialize(Model* model, ViewProjection* viewProjection, const Vector3& position) {
	assert(model);
	model_ = model;

	worldTransform_.Initialize();
	worldTransform_.translation_ = position;

	worldTransform_.rotation_.y = std::numbers::pi_v<float> / 2.0f;
	viewProjection_ = viewProjection;
};
void Player::Update() {
	worldTransform_.TransferMatrix();
	// 移動入力
	// 左右移動操作
	if (Input::GetInstance()->PushKey(DIK_RIGHT) || Input::GetInstance()->PushKey(DIK_LEFT)) {
		// 左右加速
		Vector3 acceleration = {};
		if (Input::GetInstance()->PushKey(DIK_RIGHT)) {
			acceleration.x += kAcceleration;

		} else if (Input::GetInstance()->PushKey(DIK_LEFT)) {
			acceleration.x -= kAcceleration;
		}
		//加速減速
		velocity_.x += acceleration.x;
	}

	// 移動
	worldTransform_.translation_.x += velocity_.x;

	// 行列計算
	worldTransform_.UpdateMatrix();
};

void Player::Draw() { model_->Draw(worldTransform_, *viewProjection_, textureHandle_); };
