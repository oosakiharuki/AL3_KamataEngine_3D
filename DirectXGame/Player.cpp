#include "Player.h"
#include <cassert>

Player::~Player() {
	delete bullet_;
}

void Player::Initialize(Model* model, uint32_t textureHandle,ViewProjection* viewProjection) {

	assert(model);

	model_ = model;
	textureHandle_ = textureHandle;
	worldTransform_.Initialize();

	viewProjection_ = viewProjection;
	input_ = Input::GetInstance();
}

void Player::Rotate() {
	// Rotate Speed
	const float kRotSpeed = 0.02f;

	// Bectol henkou
	if (input_->PushKey(DIK_A)) {
		worldTransform_.rotation_.y -= kRotSpeed;
	} else if(input_->PushKey(DIK_D)) {
		worldTransform_.rotation_.y += kRotSpeed;
	}
}

void Player::Attack() {

	if (input_->TriggerKey(DIK_SPACE)) {

		if (bullet_) {
			delete bullet_;
			bullet_ = nullptr;
		}

		PlayerBullet* newBullet = new PlayerBullet();
		newBullet->Initialize(model_, worldTransform_.translation_);

		bullet_ = newBullet;
	}

	

}

void Player::Update() {
	worldTransform_.TransferMatrix();

	Vector3 move = {0, 0, 0};
	const float kPlayerSpeed = 0.2f;

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

	worldTransform_.UpdateMatrix();

	const float kMoveLimitX = 35.0f;
	const float kMoveLimitY = 20.0f;

	worldTransform_.translation_.x = max(worldTransform_.translation_.x, -kMoveLimitX);
	worldTransform_.translation_.x = min(worldTransform_.translation_.x, +kMoveLimitX);

	worldTransform_.translation_.y = max(worldTransform_.translation_.y, -kMoveLimitY);
	worldTransform_.translation_.y = min(worldTransform_.translation_.y, +kMoveLimitY);

	Rotate();
	Attack();

	//bullet_ != nullptr
	if (bullet_) { 
		bullet_->Update();
	}


#ifdef _DEBUG
	ImGui::Begin("ImGui");
	ImGui::DragFloat3("Player", &worldTransform_.translation_.x, 0.1f);
	ImGui::End();
#endif

}

void Player::Draw() {
	model_->Draw(worldTransform_, *viewProjection_, textureHandle_);

	if (bullet_) {
		bullet_->Draw(*viewProjection_);
	}
}