#include "Enemy.h"
#include <cassert>
#include <numbers>
#define _USE_MATH_DEFINES
#include <math.h>

void Enemy::Initialize(Model* model, ViewProjection* viewProjection, const Vector3& position) {
	assert(model);
	
	model_ = model;
	worldTransform_.Initialize();
	viewProjection_ = viewProjection;

	worldTransform_.translation_ = position;

	worldTransform_.rotation_.y = std::numbers::pi_v<float> / -2.0f;
	
	velocity_ = {-kWalkSpeed, 0, 0};

	walkTimer_ = 0.0f;
}

Vector3 Enemy::GetWorldPosition() {
	Vector3 worldPos_;
	worldPos_.x = worldTransform_.matWorld_.m[3][0];
	worldPos_.y = worldTransform_.matWorld_.m[3][1];
	worldPos_.z = worldTransform_.matWorld_.m[3][2];
	return worldPos_;
}

AABB Enemy::GetAABB() {
	Vector3 worldPos_ = GetWorldPosition();

	AABB aabb;
	aabb.min = {
	    worldPos_.x - kWidth / 2.0f, worldPos_.y - kHeight / 2.0f, worldPos_.z - kWidth / 2.0f};
	aabb.max = {
	    worldPos_.x + kWidth / 2.0f, worldPos_.y + kHeight / 2.0f, worldPos_.z + kWidth / 2.0f};

	return aabb;
};

void Enemy::OnCollision(const Player* player) { 
	(void)player;
}


void Enemy::Updata() { 
	worldTransform_.translation_.x += velocity_.x;
	worldTransform_.translation_.y += velocity_.y;
	worldTransform_.translation_.z += velocity_.z;

	walkTimer_ += 1.0f / 60.0f;

	float a = 2 * float(M_PI);

	//worldTransform_.rotation_.x = std::sin(a * walkTimer_/ kWalkMotionAngletime);

	float param = std::sin(a * walkTimer_ / kWalkMotionAngletime);	
	float radian = kWalkMotionAngleStart + kWalkMotionAngleEnd * (param + 1.0f) / 2.0f;
	
	worldTransform_.rotation_.x = std::sin(radian);


	worldTransform_.UpdateMatrix();
}

void Enemy::Draw() { 
	model_->Draw(worldTransform_, *viewProjection_);
}
