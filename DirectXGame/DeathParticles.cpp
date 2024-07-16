#include "DeathParticles.h"
#include"WorldTransform.h"


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
	for (auto& worldTransform : worldTransforms_) {
		worldTransform.TransferMatrix();
		worldTransform.UpdateMatrix();
	}
}

void DeathParticles::Draw() {
	for (auto& worldTransform : worldTransforms_) {
		deathModel_->Draw(worldTransform, *viewProjection_);
	}

}
