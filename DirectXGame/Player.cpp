#define NOMINMAX
#include "Player.h"
#include "Input.h"
#include <cassert>
#include <numbers>
#include <algorithm>

void Player::Initialize(Model* model, ViewProjection* viewProjection,const Vector3& position) {

	assert(model);

	model_ = model;
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;

	viewProjection_ = viewProjection;
	worldTransform_.rotation_.y = std::numbers::pi_v<float> / 2.0f;
}

void Player::Update() {
	worldTransform_.TransferMatrix();

	if (onGraund_) {

		if (Input::GetInstance()->PushKey(DIK_RIGHT) || Input::GetInstance()->PushKey(DIK_LEFT)) {

			Vector3 acceleration = {};
			if (Input::GetInstance()->PushKey(DIK_RIGHT)) {

				if (velocity_.x < 0.0f) {
					acceleration.x *= (1.0f - kAttenuation);
				}
				acceleration.x += kAcceleration;

				if (lrDirection_ != LRDirection::kRight) {
					lrDirection_ = LRDirection::kRight;

					turnFirstRotationY_ = worldTransform_.rotation_.y;
					turnTimer_ = 4.0f;
				}

			} else if (Input::GetInstance()->PushKey(DIK_LEFT)) {

				if (velocity_.x > 0.0f) {
					acceleration.x *= (1.0f - kAttenuation);
				}
				acceleration.x -= kAcceleration;

				if (lrDirection_ != LRDirection::kLeft) {
					lrDirection_ = LRDirection::kLeft;

					turnFirstRotationY_ = worldTransform_.rotation_.y;
					turnTimer_ = 4.0f;
				}
			}
			velocity_.x += acceleration.x;
			velocity_.x = std::clamp(velocity_.x, -kLimitRunSpeed, kLimitRunSpeed);
		} else {
			velocity_.x *= (1.0f - kAttenuation);
		}

		if (Input::GetInstance()->PushKey(DIK_UP)) {

			velocity_ = Add(velocity_, Vector3(0, kJumpAcceleration, 0));
		}
		if (velocity_.y > 0.0f) {
			onGraund_ = false;
		}

	} else {
		velocity_ = Add(velocity_, Vector3(0, -kGravityAcceleration, 0));
		 velocity_.y = std::max(velocity_.y, -kLimitFallSpeed);
		if (landing) {
			worldTransform_.translation_.y = 2.0f;

			velocity_.x *= (1.0f - kAttenuation);

			velocity_.y = 0.0f;

			onGraund_ = true;
		}
	}

	if (velocity_.y < 0) {

		if (worldTransform_.translation_.y <= 2.0f) {
			landing = true;
		} 
	} else {
		landing = false;
	}

	worldTransform_.translation_.x += velocity_.x;
	worldTransform_.translation_.y += velocity_.y;
	worldTransform_.UpdateMatrix();

	if (turnTimer_ > 0.0f) {

		turnTimer_ -= 0.5f;

		float destinationRotationYTable[] = {
		    std::numbers::pi_v<float> / 2.0f, std::numbers::pi_v<float> * 3.0f / 2.0f};

		float destinationRotationY = destinationRotationYTable[static_cast<uint32_t>(lrDirection_)];

		worldTransform_.rotation_.y = destinationRotationY - turnTimer_;
	}
}

void Player::Draw(DebugCamera* debug) {
	model_->Draw(worldTransform_, *viewProjection_);
	model_->Draw(worldTransform_, debug->GetViewProjection());
}