#include "Enemy.h"
#include <cassert>

void Enemy::Intialize(Model* model, uint32_t textureHandle,ViewProjection* viewProjection) { 
	
	assert(model);

	model_ = model;
	textureHandle_ = textureHandle;

	worldTransform_.Initialize();
	worldTransform_.translation_ = Vector3(10.0f,3.0f,40.0f);

	viewProjection_ = viewProjection;
}

void Enemy::Update() {

	worldTransform_.TransferMatrix();

	const float kSpeed_ = 0.2f;

	worldTransform_.UpdateMatrix();

	worldTransform_.translation_.z -= kSpeed_;
}

void Enemy::Draw() {

	model_->Draw(worldTransform_, *viewProjection_, textureHandle_);

}
