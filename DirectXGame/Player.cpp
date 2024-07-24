#include "Player.h"
#include <cassert>

void Player::Initialize(Model* model, uint32_t textureHandle,ViewProjection* viewProjection) {

	assert(model);

	model_ = model;
	textureHandle_ = textureHandle;
	worldTransform_.Initialize();

	viewProjection_ = viewProjection;
	input_ = Input::GetInstance();
}

void Player::Update() {
	worldTransform_.TransferMatrix();


	if (input_->PushKey(DIK_LEFT)) {
		move.x -= kPlayerSpeed;
	} else if (input_->PushKey(DIK_RIGHT)) {
		move.x += kPlayerSpeed;
	}

	if (input_->PushKey(DIK_DOWN)) {
		move.y -= kPlayerSpeed;
	} else if (input_->PushKey(DIK_UP)) {
		move.y += kPlayerSpeed;
	}

	worldTransform_.translation_.x += move.x;
	worldTransform_.translation_.y += move.y;
	worldTransform_.translation_.z += move.z;
}

void Player::Draw() {
	model_->Draw(worldTransform_, *viewProjection_, textureHandle_);
}