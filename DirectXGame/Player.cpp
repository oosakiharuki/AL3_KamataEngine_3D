#include "Player.h"
#include <cassert>

#include "RailCamera.h"

Player::~Player() {
	for (PlayerBullet* bullet_ : bullets_) {
		delete bullet_;
	}
}

void Player::Initialize(Model* model, uint32_t textureHandle, ViewProjection* viewProjection, Vector3 position) {

	assert(model);

	model_ = model;
	textureHandle_ = textureHandle;
	worldTransform_.Initialize();

	worldTransform_.translation_ = position;
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

void Player::Attack(RailCamera* railCamera) {

	if (input_->TriggerKey(DIK_SPACE)) {

		const float kBulletSpeed = 1.0f;
		Vector3 velocity(0, 0, kBulletSpeed);

		velocity = myMath_->TransformNormal(velocity,worldTransform_.matWorld_);

		PlayerBullet* newBullet = new PlayerBullet();
		newBullet->Initialize(model_, worldTransform_.translation_,velocity);

		newBullet->SetParent(&railCamera->GetWorldTransform());

		bullets_.push_back(newBullet);
	}
}

Vector3 Player::GetWorldPosition() { 
	Vector3 worldPos{};

	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}

void Player::OnCollision() {

}

void Player::SetParent(const WorldTransform* parent) {
	worldTransform_.parent_ = parent;
}

void Player::Update(RailCamera* railCamera) {
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
	Attack(railCamera);

	//bullet_ != nullptr
	for (PlayerBullet* bullet_ : bullets_) { 
		bullet_->Update();
	}

	///Bullet Dead Timer
	bullets_.remove_if([](PlayerBullet* bullet) {
		if (bullet->IsDead()) {

			delete bullet;
			return true;
		}
		return false;
	});


#ifdef _DEBUG
	ImGui::Begin("ImGui");
	ImGui::DragFloat3("Player", &worldTransform_.translation_.x, 0.1f);
	ImGui::Text("trigger 0 key debugCamera");
	ImGui::End();
#endif

}

void Player::Draw() {
	model_->Draw(worldTransform_, *viewProjection_, textureHandle_);

	for (PlayerBullet* bullet_ : bullets_) { 
		bullet_->Draw(*viewProjection_);
	}
}