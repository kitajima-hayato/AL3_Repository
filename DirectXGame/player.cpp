#define NOMINMAX
#include "Player.h"
#include "MakeMatrix.h"
#include "MapChipField.h"
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

	Move();
	CollisionMapInfo collisionMapInfo;
	collisionMapInfo.movingDistance_ = velocity_;
	collision(collisionMapInfo);
	collisionDetection(collisionMapInfo); // 反映
	ceilingDecision(collisionMapInfo);
	rotationControl();
	calcMatrix();
};

void Player::Draw() { model_->Draw(worldTransform_, *viewProjection_); }

// 全体の流れを整理するやつ
//  移動入力
void Player::Move() {

	// 着地フラグ
	bool landing = false;
	// 地面との当たり判定
	// 下降中？
	if (velocity_.y < 0) {
		// Y座標が地面以下になったら接地
		if (worldTransform_.translation_.y <= 2.0f) {
			landing = true;
		}
	}

	// 移動入力
	// 接地状態
	if (onGround_) {

		// 左右移動操作
		if (Input::GetInstance()->PushKey(DIK_RIGHT) || Input::GetInstance()->PushKey(DIK_LEFT)) {
			// 左右加速
			Vector3 acceleration = {};
			if (Input::GetInstance()->PushKey(DIK_RIGHT)) {
				// 左入力中の右入力
				if (velocity_.x < 0.0f) {
					// 速度と逆方向に入力中は急ブレーキ
					velocity_.x *= (1.0f - kAttenuation);
				}
				acceleration.x += kAcceleration;

				if (lrDirection_ != LRDirection::kRight) {
					lrDirection_ = LRDirection::kRight;
					worldTransform_.rotation_.y = turnFirstRotationY_;
					turnTimer_ = kTimeTurn;
				}

			} else if (Input::GetInstance()->PushKey(DIK_LEFT)) {
				// 右入力中の左入力
				if (velocity_.x > 0.0f) {
					// 速度と逆方向に入力中は急ブレーキ
					velocity_.x *= (1.0f - kAttenuation);
				}
				acceleration.x -= kAcceleration;

				if (lrDirection_ != LRDirection::kLeft) {
					lrDirection_ = LRDirection::kLeft;
					worldTransform_.rotation_.y = turnFirstRotationY_;
					turnTimer_ = kTimeTurn;
				}
			}
			// 加速減速
			velocity_ += acceleration;

			// 最大速度制限
			velocity_.x = std::clamp(velocity_.x, -kLimitRunSpeed, kLimitRunSpeed);
		} else {
			// 非入力時は移動減衰をかける
			velocity_.x *= (1.0f - kAttenuation);
		}
		if (Input::GetInstance()->PushKey(DIK_UP)) {
			// ジャンプ初速
			velocity_.y += kJumpAcceleration;
		}
		// 接地判定
		//  ジャンプ開始
		if (velocity_.y > 0.0f) {
			// 空中状態に移行
			onGround_ = false;
		}

		// 空中
	} else {
		// 落下速度
		velocity_.y += -kGravityAcceleration;
		// 落下速度制限
		velocity_.y = std::max(velocity_.y, -kLimitFallSpeed);
		if (landing) {
			// めり込み排斥
			worldTransform_.translation_.y = 1.0f;
			// 摩擦で横方向速度が減衰する
			velocity_.x *= (1.0f - kAttenuation);
			// 下方向速度をリセット
			velocity_.y = 0.0f;
			// 設置状態に移行
			onGround_ = true;
		}
		// 地面との当たり判定
		// 下降中？
	}
};

// 衝突
void Player::collision(CollisionMapInfo& info) {

	// マップ衝突チェック
	collisionUp(info);
	collosopnDown(info);
	collisionLeft(info);
	collisionRight(info);
};

void Player::collisionUp(CollisionMapInfo& info) {
	// 上昇あり？
	if (info.movingDistance_.y <= 0) {
		return;
	}
	// 移動後の４つの角の座標
	std::array<Vector3, kNumCorner> positionNew;

	for (uint32_t i = 0; i < positionNew.size(); ++i) {
		positionNew[i] = CornerPosition(worldTransform_.translation_ + info.movingDistance_, static_cast<Corner>(i));
	}
	MapChipType mapChipType;
	// 真上の当たり判定
	bool hit = false;
	// 左上の判定
	IndexSet indexSet;
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionNew[kLeftTop]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}
	// 右上の判定
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionNew[kRightTop]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}

	// ブロックにヒット？
	if (hit) {
		// めり込みを排除する方向に移動量を設定する
		indexSet = mapChipField_->GetMapChipIndexSetByPosition(
		    {worldTransform_.translation_.x + 0, // Vector3(0, +kHeight / 2.0f, 0)
		     worldTransform_.translation_.y + (+kHeight / 2.0f), worldTransform_.translation_.z + 0});
		// めり込み先ブロックの範囲矩形
		Rect rect = mapChipField_->GetRectByIndex(indexSet.xIndex, indexSet.yIndex);
		info.movingDistance_.y = (std::max)(0.0f, rect.bottom - worldTransform_.translation_.y - (kHeight / 2.0f + kBlank));
		// 天井に当たったことを記録する
		info.ceilingHit_ = true;
	}
};

void Player::collosopnDown(CollisionMapInfo& info) {
	// 下降あり？
	if (info.movingDistance_.y >= 0) {
		return;
	}
	std::array<Vector3, kNumCorner> positionNew;
	for (uint32_t i = 0; i < positionNew.size(); ++i) {
		positionNew[i] = CornerPosition(worldTransform_.translation_ + info.movingDistance_, static_cast<Corner>(i));
	}
	MapChipType mapChipType;
	// 真下の当たり判定
	bool hit = false;
	// 左下点の判定
	IndexSet indexSet;
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionNew[kLeftBottom]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}
	// 右下点の判定
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionNew[kRightBottom]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}
	// ブロックにヒット？
	if (hit) {
		// めり込みを排除する方向に移動量を設定する
		indexSet = mapChipField_->GetMapChipIndexSetByPosition(worldTransform_.translation_ + Vector3(-kHeight / 2.0f, 0, 0));
		// めり込み先ブロックの範囲矩形
		Rect rect = mapChipField_->GetRectByIndex(indexSet.xIndex, indexSet.yIndex);
		info.movingDistance_.y = std::min(0.0f, rect.top - worldTransform_.translation_.y + (kHeight / 2.0f + kBlank));
		// 地面に当たったことを記録する
		info.onGround_ = true;
	}
};

void Player::collisionRight(CollisionMapInfo& info) {
	if (info.movingDistance_.x <= 0) {
		return;
	}
	std::array<Vector3, kNumCorner> positionNew;
	for (uint32_t i = 0; i < positionNew.size(); ++i) {
		positionNew[i] = CornerPosition(worldTransform_.translation_ + info.movingDistance_, static_cast<Corner>(i));
	}
	MapChipType mapChipType;
	MapChipType mapChipTypeNext;
	bool hit = false;
	IndexSet indexSet;
	// 右上の判定
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionNew[kRightTop]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	mapChipTypeNext = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex - 1, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock && mapChipTypeNext != MapChipType::kBlock) {
		hit = true;
	}
	// 右下判定
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionNew[kRightBottom]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	mapChipTypeNext = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex - 1, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock && mapChipTypeNext != MapChipType::kBlock) {
		hit = true;
	}
	if (hit) {
		IndexSet indexSetNow;
		indexSetNow = mapChipField_->GetMapChipIndexSetByPosition
		(worldTransform_.translation_ + Vector3(-kWidth / 2.0f, 0, 0));
		if (indexSetNow.xIndex != indexSet.xIndex) {
			// めり込みを排除する方向に移動量を設定する
			indexSet = mapChipField_->GetMapChipIndexSetByPosition(worldTransform_.translation_ - Vector3(-kWidth / 2.0f, 0, 0));
			// めり込み先ブロックの範囲矩形
			Rect rect = mapChipField_->GetRectByIndex(indexSet.xIndex, indexSet.yIndex);
			info.movingDistance_.x = std::min(rect.left - worldTransform_.translation_.y + (kWidth / 2.0f - kBlank), 0.0f);
			// 地面に当たったことを記録する
			info.wallHit_ = true;
		}
	}
}

void Player::collisionLeft(CollisionMapInfo& info) {
	if (info.movingDistance_.x >= 0) {
		return;
	}
	std::array<Vector3, kNumCorner> positionNew;
	for (uint32_t i = 0; i < positionNew.size(); ++i) {
		positionNew[i] = CornerPosition(worldTransform_.translation_ + info.movingDistance_, static_cast<Corner>(i));
	}
	MapChipType mapChipType;
	MapChipType mapChipTypeNext;
	bool hit = false;
	// 左上の判定
	IndexSet indexSet;
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionNew[kLeftTop]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	mapChipTypeNext = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex + 1, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock && mapChipTypeNext != MapChipType::kBlock) {
		hit = true;
	}
	// 左下判定
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionNew[kLeftBottom]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	mapChipTypeNext = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex + 1, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock && mapChipTypeNext != MapChipType::kBlock) {
		hit = true;
	}
	// ブロックにヒット？
	if (hit) {
		// 現在の座標が壁の外か判定
		IndexSet indexSetNow;
		indexSetNow = mapChipField_->GetMapChipIndexSetByPosition
		(worldTransform_.translation_ + Vector3(-kWidth / 2.0f, 0, 0));

		if (indexSetNow.xIndex != indexSet.xIndex) {
			// めり込みを排除する方向に移動量を設定する
			indexSet = mapChipField_->GetMapChipIndexSetByPosition
			(worldTransform_.translation_ + info.movingDistance_ + Vector3(-kWidth / 2.0f, 0, 0));

			// めり込み先ブロックの範囲矩形
			Rect rect = mapChipField_->GetRectByIndex
			(indexSet.xIndex, indexSet.yIndex);
			info.movingDistance_.x = 
				std::min(0.0f,rect.right - worldTransform_.translation_.x + (kWidth / 2.0f + kBlank));
			// 地面に当たったことを記録する
			info.wallHit_ = true;
		}
	}
}

// 判定結果の反映
void Player::collisionDetection(CollisionMapInfo& info) {
	// 移動
	worldTransform_.translation_ += info.movingDistance_;
};
// 天上に接触している場合の処理
void Player::ceilingDecision(CollisionMapInfo& info) {
	// 天井に当たった？
	if (info.ceilingHit_) {
		DebugText::GetInstance()->ConsolePrintf("hit ceiling\n");
		velocity_.y = 0;
	}
};
// 壁に接触している場合の処理
void Player::wallDecision(const CollisionMapInfo& info) {
	if (info.wallHit_) {

		velocity_.x *= (1.0f - kAttenuationWall);
	}
};
// 設置状態の切り替え
void Player::groundingSwitch(const CollisionMapInfo& info) {
	// 自キャラが接地状態？
	if (onGround_) {
		// 接地状態の処理
		if (velocity_.y > 0.0f) {
			onGround_ = false;
		} else {
			std::array<Vector3, kNumCorner> positionNew;
			for (uint32_t i = 0; i < positionNew.size(); ++i) {
				positionNew[i] = CornerPosition(worldTransform_.translation_ + info.movingDistance_, static_cast<Corner>(i));
			}
			// 落下判定
			MapChipType mapChipType;
			bool hit = false;
			// 左下点の判定
			IndexSet indexSet;
			indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionNew[kLeftBottom] + Vector3(0, -kDisplace, 0));
			mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
			if (mapChipType == MapChipType::kBlock) {
				hit = true;
			}
			// 右下点の判定
			indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionNew[kRightBottom] + Vector3(0, -kDisplace, 0));
			mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
			if (mapChipType == MapChipType::kBlock) {
				hit = true;
			}

			// 落下開始
			if (!hit) {
				// 落下なら空中状態に切り替え
				onGround_ = false;
			}
		}
	} else {
		// 空中状態の処理

		// 着地フラグ
		if (info.onGround_) {

			// 着地時にX速度を減衰
			velocity_.x *= (1.0f - kAttenuationLanding);
			// Y速度を０にする
			velocity_.y = 0.0f;
			// 着地状態に切り替える(落下を止める)
			onGround_ = true;
		}
	}
};
// 旋回制御
void Player::rotationControl() {

	if (turnTimer_ > 0.0f) {
		turnTimer_ -= 1.0f / 60.0f;
		// 左右の自キャラ角度テーブル
		float destinationRotationYTable[] = {std::numbers::pi_v<float> / 2.0f, std::numbers::pi_v<float> * 3.0f / 2.0f};
		// 状態に応じた角度を取得する
		float destinationRotationY = destinationRotationYTable[static_cast<uint32_t>(lrDirection_)];
		// 自キャラの角度を設定する
		worldTransform_.rotation_.y = destinationRotationY * easeInOutSine(turnTimer_); ////角度補完せえい！！！
	}
};
// 行列計算
void Player::calcMatrix() {
	// 行列計算
	worldTransform_.TransferMatrix();
	worldTransform_.UpdateMatrix();
};

// 指定した角の座標計算
Vector3 Player::CornerPosition(const Vector3& center, Corner corner) {

	Vector3 offsetTable[kNumCorner] = {
	    {+kWidth / 2.0f, -kHeight / 2.0f, 0.0f},
        {-kWidth / 2.0f, -kHeight / 2.0f, 0.0f},
        {+kWidth / 2.0f, +kHeight / 2.0f, 0.0f},
        {-kWidth / 2.0f, +kHeight / 2.0f, 0.0f}
    };

	return center + offsetTable[static_cast<uint32_t>(corner)];
}

#pragma region 前のUPDATE関数
// void Player::Update() {
//
//
//
//
//
//
//
//
//
//
//
//
//	// 移動入力
//	// 接地状態
//	if (onGround_) {
//
//		// 左右移動操作
//		if (Input::GetInstance()->PushKey(DIK_RIGHT) || Input::GetInstance()->PushKey(DIK_LEFT)) {
//			// 左右加速
//			Vector3 acceleration = {};
//			if (Input::GetInstance()->PushKey(DIK_RIGHT)) {
//				// 左入力中の右入力
//				if (velocity_.x < 0.0f) {
//					// 速度と逆方向に入力中は急ブレーキ
//					velocity_.x *= (1.0f - kAttenuation);
//				}
//				acceleration.x += kAcceleration;
//				if (lrDirection_ != LRDirection::kRight) {
//					lrDirection_ = LRDirection::kRight;
//					worldTransform_.rotation_.y = turnFirstRotationY_;
//					turnTimer_ = kTimeTurn;
//				}
//
//			} else if (Input::GetInstance()->PushKey(DIK_LEFT)) {
//				// 右入力中の左入力
//				if (velocity_.x > 0.0f) {
//					// 速度と逆方向に入力中は急ブレーキ
//					velocity_.x *= (1.0f - kAttenuation);
//				}
//				acceleration.x -= kAcceleration;
//				if (lrDirection_ != LRDirection::kLeft) {
//					lrDirection_ = LRDirection::kLeft;
//					worldTransform_.rotation_.y = turnFirstRotationY_;
//					turnTimer_ = kTimeTurn;
//				}
//			}
//			// 加速減速
//			velocity_.x += acceleration.x;
//			velocity_.y += acceleration.y;
//			velocity_.z += acceleration.z;
//
//			// 最大速度制限
//			velocity_.x = std::clamp(velocity_.x, -kLimitRunSpeed, kLimitRunSpeed);
//		} else {
//			// 非入力時は移動減衰をかける
//			velocity_.x *= (1.0f - kAttenuation);
//		}
//		if (Input::GetInstance()->PushKey(DIK_UP)) {
//			// ジャンプ初速
//			velocity_.y += kJumpAcceleration;
//		} // ジャンプ開始
//		if (velocity_.y > 0.0f) {
//			// 空中状態に移行
//			onGround_ = false;
//		}
//
//		// 空中
//	} else {
//		// 落下速度
//		velocity_.y += -kGravityAcceleration;
//		// 落下速度制限
//		velocity_.y = std::max(velocity_.y, -kLimitFallSpeed);
//		if (landing) {
//			// めり込み排斥
//			worldTransform_.translation_.y = 2.0f;
//			// 摩擦で横方向速度が減衰する
//			velocity_.x *= (1.0f - kAttenuation);
//			// 下方向速度をリセット
//			velocity_.y = 0.0f;
//			// 設置状態に移行
//			onGround_ = true;
//		}
//	}
//	// 旋回制御
//	if (turnTimer_ > 0.0f) {
//		turnTimer_ -= 1.0f / 60.0f;
//		// 左右の自キャラ角度テーブル
//		float destinationRotationYTable[] = {std::numbers::pi_v<float> / 2.0f, std::numbers::pi_v<float> * 3.0f / 2.0f};
//		// 状態に応じた角度を取得する
//		float destinationRotationY = destinationRotationYTable[static_cast<uint32_t>(lrDirection_)];
//		// 自キャラの角度を設定する
//		worldTransform_.rotation_.y = destinationRotationY * easeInOutSine(turnTimer_); ////角度補完せえい！！！
//	}
//
//	worldTransform_.translation_.x += velocity_.x;
//	worldTransform_.translation_.y += velocity_.y;
//	worldTransform_.translation_.z += velocity_.z;
//	// 行列計算
//	worldTransform_.TransferMatrix();
//	worldTransform_.UpdateMatrix();
// };
#pragma endregion