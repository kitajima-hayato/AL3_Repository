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
#include"AABB.h"
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
class MapChipField;

class Player {
	// 自機の当たり判定の調整項目
	static inline const float kWidth = 0.8f;	//幅　X
	static inline const float kHeight = 0.8f;	//高さY
	static inline const float kBlank = 0.3f;

	//着地時の速度減衰率
	static inline const float kAttenuationLanding = 0.0f;
	static inline const float kAttenuationWall = 0.0f;
	static inline const float kDisplace = 0.002f;
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

	void collisionDown(CollisionMapInfo& info); // マップ衝突判定下方向
	void collisionLeft(CollisionMapInfo& info);	//マップ衝突判定左方向
	void collisionRight(CollisionMapInfo& info);//マップ衝突判定右方向


	// 判定結果を反映して移動させる
	void collisionDetection( CollisionMapInfo& info);
	// 天井に接触している場合の処理
	void ceilingDecision( CollisionMapInfo& info);
	// 壁に接触している場合の処理
	void wallDecision(const CollisionMapInfo& info);
	// 接地状態の切り替え
	void groundingSwitch(const CollisionMapInfo& info);
	// 旋回制御
	void rotationControl();
	// 行列計算
	void calcMatrix();

	const WorldTransform& GetWorldTransform() const { return worldTransform_; }
	const Vector3& GetVelocity() const { return velocity_; }
	const Vector3& GetPlayerPosition() const { return worldTransform_.translation_; }

	void SetMapChipField(MapChipField* mapChipField) { mapChipField_ = mapChipField; }

	// ワールド座標を取得、当たり判定を取るために必要
	Vector3 GetWorldPosition();
	//AABBを取得
	AABB GetAABB();

private:
	
	WorldTransform worldTransform_;
	ViewProjection* viewProjection_ = nullptr;

	Model* model_;
	uint32_t textureHandle_ = 0u;
	Vector3 velocity_ = {};

	static inline const float kAcceleration = 0.1f;
	static inline const float kAttenuation = 0.1f;
	static inline const float kLimitRunSpeed = 0.3f;

	LRDirection lrDirection_ = LRDirection::kRight;

	// 旋回開始時の角度
	float turnFirstRotationY_ = 0.0f;
	// 旋回タイマー
	float turnTimer_ = 0.0f;
	// 旋回時間<秒>
	static inline const float kTimeTurn = 0.3f;

	float easeInOutSine(float x) { return cosf(((float)M_PI * x) / 2); }
	float EaseOutSine(float x) { return cosf((x * (float)M_PI) / 2); }
	float easeInCubic(float x) { return x * x * x; }

	// 設置状態フラグ
	bool onGround_ = true;
	// 重力加速度(下)
	static inline const float kGravityAcceleration = 0.05f;
	// 最大落下速度
	static inline const float kLimitFallSpeed = 0.3f;
	// ジャンプ初速
	static inline const float kJumpAcceleration = 0.5f;

	MapChipField* mapChipField_ = nullptr;

	

};
