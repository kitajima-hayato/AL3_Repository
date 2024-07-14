#include "DeathParticles.h"
void DeathParticles::Initialize(Model* model, ViewProjection* viewProjection, const Vector3& position) { 
	assert(model);
	deathModel_ = model;

	viewProjection_ = viewProjection;
}

void DeathParticles::Update() {}

void DeathParticles::Draw() {}
