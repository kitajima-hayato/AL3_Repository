#include "Skydome.h"

Skydome::Skydome() {}

Skydome::~Skydome() { 
	
}

void Skydome::Initialize(Model *model,ViewProjection* viewprojection) {
	model_ = model;
	assert(model);
	worldTransform_.Initialize();
	viewProjection_ = viewprojection;
}

void Skydome::Update() {

}

void Skydome::Draw() { 
	model_->Draw(worldTransform_, *viewProjection_,textureHandle_);
}
