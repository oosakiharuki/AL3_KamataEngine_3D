#define NOMINMAX
#include "Player.h"
#include "MapChipField.h"
#include "Input.h"
#include <cassert>
#include <numbers>
#include <algorithm>

#include "DebugText.h"

void Player::Initialize(Model* model, ViewProjection* viewProjection,const Vector3& position) {

	assert(model);

	model_ = model;
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;

	viewProjection_ = viewProjection;
	worldTransform_.rotation_.y = std::numbers::pi_v<float> / 2.0f;
}

Vector3 Player::CornerPosition(const Vector3& center, Corner corner) {

	Vector3 offsetTable[kNumCorner] = {

	    {+kWidth / 2.0f, -kHeight / 2.0f, 0.0f},
	    {-kWidth / 2.0f, -kHeight / 2.0f, 0.0f},
	    {+kWidth / 2.0f, +kHeight / 2.0f, 0.0f},
	    {-kWidth / 2.0f, +kHeight / 2.0f, 0.0f},
	};

	return Add(center , offsetTable[static_cast<uint32_t>(corner)]);
};

void Player::MapChipCollision(CollisionMapInfo& info) {

	MapChipCollisionUp(info);
	MapChipCollisionDown(info);
	MapChipCollisionRight(info);
	MapChipCollisionLeft(info);

}

void Player::MapChipCollisionGround(CollisionMapInfo& info) {
	
	if (onGraund_) {
		if (info.landingFlag_) {
			onGraund_ = true;

			velocity_.x += (1.0f - kAttenuationLanding);

			velocity_.y = 0.0f;
		}
	} 
	else {
		if (velocity_.y > 0.0f) {
			onGraund_ = false;
		} 
		else {
			worldTransform_.translation_.y = 1.0f;

			velocity_.x *= (1.0f - kAttenuation);

			velocity_.y = 0.0f;

			onGraund_ = true;
		}
	}
	
	std::array<Vector3, kNumCorner> positionNew;
	MapChipType mapChipType;

	bool hit = false;
	MapChipField::IndexSet indexSet;


	indexSet = mapChipField_->GetMapChipIntexSetByPosition(positionNew[kLeftBottom]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIntex, indexSet.yIntex);
	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}

	indexSet = mapChipField_->GetMapChipIntexSetByPosition(positionNew[kRightBottom]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIntex, indexSet.yIntex);
	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}

	indexSet = mapChipField_->GetMapChipIntexSetByPosition(
	    Add(worldTransform_.translation_, Vector3(0, -0.8f, 0)));

	if (!hit) {
		onGraund_ = false;
	}
}

void Player::prosperity(const CollisionMapInfo& info) {
	//worldTransform_.translation_.x += info.moveCount.x;
	//worldTransform_.translation_.y += info.moveCount.y;
	worldTransform_.translation_.z += info.moveCount.z;
}

void Player::CeilingTachi(const CollisionMapInfo& info) {
	if (info.ceilingTachiFlag_) {
		DebugText::GetInstance()->ConsolePrintf("hit celling");
		velocity_.y = 0;
	}
}

void Player::MapChipCollisionUp(CollisionMapInfo& info) {

	std::array<Vector3, kNumCorner> positionNew;

	for (uint32_t i = 0; i < positionNew.size(); ++i) {
		positionNew[i] = CornerPosition(Add(worldTransform_.translation_ , info.moveCount),static_cast<Corner>(i) );

	}
	//jimen kara tobutoki nomi ni suru
	if (info.moveCount.y <= 0) {
		return;
	}

	MapChipType mapChipType;
	bool hit = false;

	MapChipField::IndexSet indexSet;
	
	indexSet = mapChipField_->GetMapChipIntexSetByPosition(positionNew[kLeftTop]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIntex,indexSet.yIntex);
	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}

	indexSet = mapChipField_->GetMapChipIntexSetByPosition(positionNew[kRightTop]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIntex, indexSet.yIntex);
	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}


	if (hit) {
		indexSet = mapChipField_->GetMapChipIntexSetByPosition(Add(worldTransform_.translation_ , Vector3(0,kHeight / 2.0f,0)));
	
		MapChipField::Rect rect = mapChipField_->GetRectByIndex(indexSet.xIntex,indexSet.yIntex);
		info.moveCount.y = std::max(0.0f, rect.bottom - worldTransform_.translation_.y - (kHeight / 2.0f + kblank));

		info.ceilingTachiFlag_ = true;
	}
}

void Player::MapChipCollisionDown(CollisionMapInfo& info) {
	
	if (info.moveCount.y >= 0) {
		return;
	}
	
	std::array<Vector3, kNumCorner> positionNew;

	for (uint32_t i = 0; i < positionNew.size(); ++i) {
		positionNew[i] = CornerPosition(
		    Add(worldTransform_.translation_, info.moveCount), static_cast<Corner>(i));
	}

	MapChipType mapChipType;
	bool hit = false;

	MapChipField::IndexSet indexSet;

	indexSet = mapChipField_->GetMapChipIntexSetByPosition(positionNew[kLeftBottom]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIntex, indexSet.yIntex);
	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}

	indexSet = mapChipField_->GetMapChipIntexSetByPosition(positionNew[kRightBottom]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIntex, indexSet.yIntex);
	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}

	if (hit) {
		indexSet = mapChipField_->GetMapChipIntexSetByPosition(
		    Add(worldTransform_.translation_, Vector3(0, -kHeight / 2.0f, 0)));

		MapChipField::Rect rect = mapChipField_->GetRectByIndex(indexSet.xIntex, indexSet.yIntex);
		info.moveCount.y = std::max(
		    0.0f, rect.bottom - worldTransform_.translation_.y - (-kHeight / 2.0f + kblank));

		info.ceilingTachiFlag_ = true;
	}



}

void Player::MapChipCollisionRight(CollisionMapInfo& info) {
	if (info.moveCount.x >= 0) {
		return;
	}
	std::array<Vector3, kNumCorner> positionNew;

	for (uint32_t i = 0; i < positionNew.size(); ++i) {
		positionNew[i] = CornerPosition(
		    Add(worldTransform_.translation_, info.moveCount), static_cast<Corner>(i));
	}

	MapChipType mapChipType;
	bool hit = false;

	MapChipField::IndexSet indexSet;

	indexSet = mapChipField_->GetMapChipIntexSetByPosition(positionNew[kRightTop]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIntex, indexSet.yIntex);
	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}

	indexSet = mapChipField_->GetMapChipIntexSetByPosition(positionNew[kRightBottom]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIntex, indexSet.yIntex);
	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}

	if (hit) {
		indexSet = mapChipField_->GetMapChipIntexSetByPosition(
		    Add(worldTransform_.translation_, Vector3(-kWidth / 2.0f,0, 0)));

		MapChipField::Rect rect = mapChipField_->GetRectByIndex(indexSet.xIntex, indexSet.yIntex);
		info.moveCount.y = std::max(
		    0.0f, rect.bottom - worldTransform_.translation_.x - (-kWidth / 2.0f + kblank));

		info.wallTachiFlag_ = true;
	}
}

void Player::MapChipCollisionLeft(CollisionMapInfo& info) {
	if (info.moveCount.x <= 0) {
		return;
	}
	std::array<Vector3, kNumCorner> positionNew;

	for (uint32_t i = 0; i < positionNew.size(); ++i) {
		positionNew[i] = CornerPosition(
		    Add(worldTransform_.translation_, info.moveCount), static_cast<Corner>(i));
	}

	MapChipType mapChipType;
	bool hit = false;

	MapChipField::IndexSet indexSet;

	indexSet = mapChipField_->GetMapChipIntexSetByPosition(positionNew[kLeftTop]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIntex, indexSet.yIntex);
	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}

	indexSet = mapChipField_->GetMapChipIntexSetByPosition(positionNew[kLeftBottom]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIntex, indexSet.yIntex);
	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}

	if (hit) {
		indexSet = mapChipField_->GetMapChipIntexSetByPosition(
		    Add(worldTransform_.translation_, Vector3(kWidth / 2.0f, 0, 0)));

		MapChipField::Rect rect = mapChipField_->GetRectByIndex(indexSet.xIntex, indexSet.yIntex);
		info.moveCount.y = std::max(
		    0.0f, rect.bottom - worldTransform_.translation_.x - (kWidth / 2.0f + kblank));

		info.wallTachiFlag_ = true;
	}
}

void Player::MapChipCollisionWall(CollisionMapInfo& info) { 
	if (info.wallTachiFlag_) {
	
	}


}




void Player::Update() {
	worldTransform_.TransferMatrix();

	if (onGraund_) {

		//1
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
			worldTransform_.translation_.y = 1.0f;

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

		//2
		//collision
		CollisionMapInfo collisionMapInfo_;
		collisionMapInfo_.moveCount = velocity_;

		MapChipCollision(collisionMapInfo_);
		//MapChipCollision(collisionMapInfo_);
		//MapChipCollision(collisionMapInfo_);
		//MapChipCollision(collisionMapInfo_);

		//3
		prosperity(collisionMapInfo_);
		//4
		CeilingTachi(collisionMapInfo_);





}

void Player::Draw() {
	model_->Draw(worldTransform_, *viewProjection_);
}

void Player::DebugDraw(DebugCamera* debug) {
	model_->Draw(worldTransform_, debug->GetViewProjection());
}