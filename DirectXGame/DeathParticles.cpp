#include "DeathParticles.h"


void DeathParticles::Initialize(Model* model, ViewProjection* viewProjection, const Vector3& position) { 
	assert(model);
	deathModel_ = model;

	viewProjection_ = viewProjection;
	for (auto& worldTransform : worldTransforms_) {
		worldTransform.Initialize();
		worldTransform.translation_ = position;
	}
	
}

void DeathParticles::Update() {
	// 終了なら何もしないで
	if (isFinished_) {
		return;
	}
	for (auto& worldTransform : worldTransforms_) {
		worldTransform.TransferMatrix();
		worldTransform.UpdateMatrix();
	}
	for (uint32_t i = 0; i < kNumParticles; ++i) {
		//基本となる速度ベクトル
		Vector3 velocity = {kSpeed, 0, 0};
		//回転角を計算する
		float angle = kAngleUnit * i;
		//Z軸周りの回転行列
		Matrix4x4 matrixRotation = MakeRotateZMatrix(angle);
		//基本ベクトルを回転させて速度ベクトルを得る
		velocity = Transform(velocity, matrixRotation);
		//移動処理
		worldTransforms_[i].translation_ += velocity;
	}
	//カウンターフレームを１フレーム分の秒数進める
	counter_ += 1.0f / 60.0f;
	//存続時間の上限に達したら
	if (counter_ >= kDuration) {
		counter_ = kDuration;
		//終了扱いにする
		isFinished_ = true;
	}
}

void DeathParticles::Draw() {
	//終了なら何もしないで
	if (isFinished_) {
		return;
	}
	for (auto& worldTransform : worldTransforms_) {
		deathModel_->Draw(worldTransform, *viewProjection_);
	}

}
