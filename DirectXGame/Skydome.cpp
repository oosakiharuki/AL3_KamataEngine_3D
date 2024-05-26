#include "Skydome.h"
#include <cassert>

void Skydome::Initialize(Model* model, uint32_t textureHandle, ViewProjection* viewProjection) {
	// モデル
	assert(model);

	model_ = model;
	textureHandle_ = textureHandle;
	worldTransform_.Initialize();

	viewProjection_ = viewProjection;
}

void Skydome::Update() { 
	worldTransform_.TransferMatrix(); 
}


void Skydome::Draw(const ViewProjection &viewProjection) { 
	model_->Draw(worldTransform_, viewProjection, textureHandle_); 


	//model_->Draw(worldTransform_, *viewProjection_, textureHandle_); 
}