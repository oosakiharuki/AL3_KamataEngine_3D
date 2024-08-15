#include "Enemy.h"
#include <cassert>

#include "Player.h"

Enemy::~Enemy() { 

	for (EnemyBullet* bullet : bullets_) {
		delete bullet;
	} 

}

void Enemy::Intialize(Model* model, uint32_t textureHandle,ViewProjection* viewProjection) { 
	
	assert(model);

	model_ = model;
	textureHandle_ = textureHandle;

	worldTransform_.Initialize();
	worldTransform_.translation_ = Vector3(20.0f,1.0f,80.0f);

	viewProjection_ = viewProjection;
}

void Enemy::Fire() {
	const float deltaTime = 1.0f / 60.0f;

	bulletTimer_ -= deltaTime;

	if (bulletTimer_ <= 0.0f) {

		assert(player_);

		const float kBulletSpeed = 0.5f;

		Vector3 playerPostion = player_->GetWorldPosition();
		Vector3 enemyPostion = GetWorldPosition();

		Vector3 subtract = myMath_->Subtract(playerPostion, enemyPostion);

		Vector3 normalize = myMath_->Normalize(subtract);

		normalize.x *= kBulletSpeed;
		normalize.y *= kBulletSpeed;
		normalize.z *= kBulletSpeed;


		Vector3 velocity(normalize);



		velocity = myMath_->TransformNormal(velocity, worldTransform_.matWorld_);


		EnemyBullet* newBullet = new EnemyBullet();
		newBullet->Initialize(model_, worldTransform_.translation_, velocity);

		bullets_.push_back(newBullet);
		bulletTimer_ = 1.0f;
	}
}

Vector3 Enemy::GetWorldPosition() { 
	Vector3 worldPos{};

	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}

void Enemy::OnCollision() {

}

void Enemy::Update() {

	worldTransform_.TransferMatrix();

	const float kSpeed_ = 0.2f;

	worldTransform_.UpdateMatrix();

	switch (phase_) {
	case Phase::Approach:

		worldTransform_.translation_.z += -kSpeed_;

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

	Fire();

	// bullet_ != nullptr
	for (EnemyBullet* bullet_ : bullets_) {
		bullet_->Update();
	}

	/// Bullet Dead Timer
	bullets_.remove_if([](EnemyBullet* bullet) {
		if (bullet->IsDead()) {

			delete bullet;
			return true;
		}
		return false;
	});
}

void Enemy::Draw() {

	model_->Draw(worldTransform_, *viewProjection_, textureHandle_);
	
	for (EnemyBullet* bullet_ : bullets_) {
		bullet_->Draw(*viewProjection_);
	}
}
