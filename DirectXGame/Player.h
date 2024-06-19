#pragma once
#include <Input.h>
#include <Model.h>
#include <WorldTransform.h>
#include <algorithm>
#include <cassert>
#include <numbers>

#include <corecrt_math.h>
#include <corecrt_math_defines.h>
#include <math.h>

#include <DebugText.h>

enum class LRDirection {
	kRight,
	kLeft,
};
struct CollisionMapInfo {
	bool ceilingHit_ = false;	//天井衝突フラグ
	bool onGround_ = false;		//着地フラグ
	bool wallHit_ = false;		//壁接触フラグ
	Vector3 movingDistance_;	//移動量
};
enum Corner {
	kRightBottom,	//右下
	kLeftBottom,	//左下
	kRightTop,		//右上
	kLeftTop,		//左上

	kNumCorner,		//要素数
};
//const Vector3 operator+(const Vector3& v1, const Vector3& v2);
class MapChipField;

class Player {
	// 自機の当たり判定の調整項目
	static inline const float kWidth = 1.8f;
	static inline const float kHeight = 1.8f;
	static inline const float kBlank = 0.3f;

public:
	Player();
	~Player();
	void Initialize(Model* model, ViewProjection* viewProjection, const Vector3& position);
	void Update();
	void Draw();

	Vector3 CornerPosition(const Vector3& center, Corner corner);

	// 全体の流れを整理するやつ
	//  移動入力
	void Move();
	// 移動量を加味して衝突判定する
	void collision(CollisionMapInfo &info);

	void collisionUp(CollisionMapInfo& info);	//マップ衝突判定上方向

	//void collisionLeft(CollisionMapInfo& info);	//マップ衝突判定左方向
	//void collosopnDown(CollisionMapInfo& info);	//マップ衝突判定下方向
	//void collisionRight(CollisionMapInfo& info);//マップ衝突判定右方向


	// 判定結果を反映して移動させる
	void collisionDetection( CollisionMapInfo& info);
	// 天上に接触している場合の処理
	void ceilingDecision( CollisionMapInfo& info);
	// 壁に接触している場合の処理
	void wallDecision();
	// 設置状態の切り替え
	void groundingSwitch();
	// 旋回制御
	void rotationControl();
	// 行列計算
	void calcMatrix();

	const WorldTransform& GetWorldTransform() const { return worldTransform_; }
	const Vector3& GetVelocity() const { return velocity_; }
	const Vector3& GetPlayerPosition() const { return worldTransform_.translation_; }

	void SetMapChipField(MapChipField* mapChipField) { mapChipField_ = mapChipField; }


	

private:
	
	WorldTransform worldTransform_;
	ViewProjection* viewProjection_ = nullptr;

	Model* model_;
	uint32_t textureHandle_ = 0u;
	Vector3 velocity_ = {};

	static inline const float kAcceleration = 0.1f;
	static inline const float kAttenuation = 0.05f;
	static inline const float kLimitRunSpeed = 0.5f;

	LRDirection lrDirection_ = LRDirection::kRight;

	// 旋回開始時の角度
	float turnFirstRotationY_ = 0.0f;
	// 旋回タイマー
	float turnTimer_ = 0.0f;
	// 旋回時間<秒>
	static inline const float kTimeTurn = 0.5f;

	float easeInOutSine(float x) { return cosf(((float)M_PI * x) / 2); }
	float EaseOutSine(float x) { return cosf((x * (float)M_PI) / 2); }
	float easeInCubic(float x) { return x * x * x; }

	// 設置状態フラグ
	bool onGround_ = true;
	// 重力加速度(下)
	static inline const float kGravityAcceleration = 0.2f;
	// 最大落下速度
	static inline const float kLimitFallSpeed = 0.4f;
	// ジャンプ初速
	static inline const float kJumpAcceleration = 0.8f;

	MapChipField* mapChipField_ = nullptr;
};
