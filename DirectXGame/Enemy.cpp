#include "Enemy.h"
void Enemy::Initialize(Model* model, ViewProjection* viewProjection, const Vector3& position) {
	assert(model);
	model_ = model;

	worldTransform_.Initialize();
	worldTransform_.translation_ = position;

	worldTransform_.rotation_.y = std::numbers::pi_v<float> * 3.0f / 2.0f;
	viewProjection_ = viewProjection;

	velocity_ = {-kWalkSpeed, 0, 0};
	WalkTimer_ = 0.0f;
}

void Enemy::Update() {

	// 移動
	worldTransform_.translation_ += velocity_;

	// タイマーを加算
	WalkTimer_ += 1.0f / 60.0f;

	// 回転アニメーション
	float param = std::sin((2.0f * std::numbers::pi_v<float>)*WalkTimer_ / kWalkMotionTime);
	float radian = kWalkMotionAngleStart + kWalkMotionAngleEnd * (param + 1.0f) / 2.0f;
	worldTransform_.rotation_.x = fLerp(kWalkMotionAngleStart, kWalkMotionAngleEnd, radian);
	worldTransform_.TransferMatrix();
	worldTransform_.UpdateMatrix();
}

void Enemy::Draw() { model_->Draw(worldTransform_, *viewProjection_); }

Vector3 Enemy::GetWorldPosition() { // ワールド座標を入れる変数
	Vector3 worldPos;
	// ワールド行列の平行移動成分を取得（ワールド座標）
	worldPos = worldTransform_.translation_;
	return worldPos;
}

AABB Enemy::GetAABB() {
	Vector3 worldPos = GetWorldPosition();
	AABB aabb;
	aabb.min = {worldPos.x - kWidthEnemy / 2.0f, worldPos.y - kHeightEnemy / 2.0f, worldPos.z - kWidthEnemy / 2.0f};
	aabb.max = {worldPos.x + kWidthEnemy / 2.0f, worldPos.y + kHeightEnemy / 2.0f, worldPos.z + kWidthEnemy / 2.0f};
	return aabb;
}

void Enemy::OnCollision(const Player* player) { 
	(void)player; 
}
