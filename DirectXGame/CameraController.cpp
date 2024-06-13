#include "CameraController.h"
#include "Player.h"
void CameraController::Initialize() {
	// ビュープロジェクションの初期化
	viewProjection_.Initialize();
	
}

void CameraController::Update() {
	// 追従対象のワールドトランスフォームを参照
	const WorldTransform& targetWorldTransform = target_->GetWorldTransform();
	// 追従対象とオフセットと追従対象の速度からカメラの目標座標を計算
	const Vector3& targetVelocity_ = target_->GetVelocity();
	target_coordinate_.x = targetWorldTransform.translation_.x + targetOffset_.x + targetVelocity_.x * kVelocityBias;
	target_coordinate_.y = targetWorldTransform.translation_.y + targetOffset_.y + targetVelocity_.y * kVelocityBias;
	target_coordinate_.z = targetWorldTransform.translation_.z + targetOffset_.z + targetVelocity_.z * kVelocityBias;

	// 座標補間によりゆったり追従
	viewProjection_.translation_ = Lerp(viewProjection_.translation_, target_coordinate_, kInterpolationRate);

	// 移動範囲制限
	viewProjection_.translation_.x = (std::max)(viewProjection_.translation_.x, movableArea_.left);
	viewProjection_.translation_.x = (std::min)(viewProjection_.translation_.x, movableArea_.right);
	viewProjection_.translation_.y = (std::max)(viewProjection_.translation_.y, movableArea_.bottom);
	viewProjection_.translation_.y = (std::min)(viewProjection_.translation_.y, movableArea_.top);

	// 行列を更新する
	viewProjection_.UpdateMatrix();
}

void CameraController::Reset() {
	// 追従対象のワールドトランスフォームを参照
	const WorldTransform& targetWorldTransform = target_->GetWorldTransform();
	// 追従対象とオフセットからカメラの座標を計算
	viewProjection_.translation_.x = targetWorldTransform.translation_.x + targetOffset_.x;
	viewProjection_.translation_.y = targetWorldTransform.translation_.y + targetOffset_.y;
	viewProjection_.translation_.z = targetWorldTransform.translation_.z + targetOffset_.z;
}