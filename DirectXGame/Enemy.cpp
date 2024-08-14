#include "Enemy.h"
#include <cassert>

void Enemy::Intialize(Model* model, uint32_t textureHandle,ViewProjection* viewProjection) { 
	
	assert(model);

	model_ = model;
	textureHandle_ = textureHandle;

	worldTransform_.Initialize();
	worldTransform_.translation_ = Vector3(0.0f,1.0f,40.0f);

	viewProjection_ = viewProjection;
}

void Enemy::Update() {

	worldTransform_.TransferMatrix();

	const float kSpeed_ = 0.2f;

	worldTransform_.UpdateMatrix();

	switch (phase_) {
	case Phase::Approach:

		worldTransform_.translation_.z -= kSpeed_;

		if (worldTransform_.translation_.z < 0.0f) {
			phase_ = Phase::Leave;
		}

		break;
	case Phase::Leave:

		worldTransform_.translation_.x += -kSpeed_;
		worldTransform_.translation_.y += kSpeed_;

		break;
	default:
		break;
	}
}

void Enemy::Draw() {

	model_->Draw(worldTransform_, *viewProjection_, textureHandle_);

}
