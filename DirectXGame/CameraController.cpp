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
	const Vector3& targetPosition_ = target_->GetPlayerPosition();
	arrival_point.x = targetWorldTransform.translation_.x + targetOffset_.x + targetVelocity_.x * kVelocityBias;
	arrival_point.y = targetWorldTransform.translation_.y + targetOffset_.y + targetVelocity_.y * kVelocityBias;
	arrival_point.z = targetWorldTransform.translation_.z + targetOffset_.z + targetVelocity_.z * kVelocityBias;

	// 座標補間によりゆったり追従
	viewProjection_.translation_ = Lerp(viewProjection_.translation_, arrival_point, kInterpolationRate);

	// 追従対象が画面外に出ないように補正
	viewProjection_.translation_.x = (std::max)(viewProjection_.translation_.x, targetPosition_.x + margin.left);
	viewProjection_.translation_.x = (std::min)(viewProjection_.translation_.x, targetPosition_.x + margin.right);
	viewProjection_.translation_.y = (std::max)(viewProjection_.translation_.y, targetPosition_.y + margin.bottom);
	viewProjection_.translation_.y = (std::min)(viewProjection_.translation_.y, targetPosition_.y + margin.top);

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
