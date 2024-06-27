#include "DeathParticles.h"
#include <algorithm>

void DeathParticles::Initialize(Model* model, ViewProjection* viewProjection, const Vector3& position) { 

	model_ = model;
	viewProjection_ = viewProjection;

	for (auto& worldTransform : worldTransforms_) {
		worldTransform.Initialize();
		worldTransform.translation_ = position;
	}

	objectColor_.Initialize();
	color_ = {1, 1, 1, 1};
}

Matrix4x4 DeathParticles::MakeRotateZMatrix(float rotate_) {
	Matrix4x4 resultZ{};

	resultZ.m[0][0] = std::cos(rotate_);
	resultZ.m[0][1] = std::sin(rotate_);
	resultZ.m[0][2] = 0.0f;
	resultZ.m[0][3] = 0.0f;
	resultZ.m[1][0] = -(std::sin(rotate_));
	resultZ.m[1][1] = std::cos(rotate_);
	resultZ.m[1][2] = 0.0f;
	resultZ.m[1][3] = 0.0f;
	resultZ.m[2][0] = 0.0f;
	resultZ.m[2][1] = 0.0f;
	resultZ.m[2][2] = 1.0f;
	resultZ.m[2][3] = 0.0f;
	resultZ.m[3][0] = 0.0f;
	resultZ.m[3][1] = 0.0f;
	resultZ.m[3][2] = 0.0f;
	resultZ.m[3][3] = 1.0f;

	return resultZ;
}

void DeathParticles::Updata() {
	if (isFinished_) {
		return;
	}

	for (auto& worldTransform : worldTransforms_) {
		worldTransform.UpdateMatrix();
	}
	for (uint32_t i = 0; i < kNumParticles; i++) {
		Vector3 velocity_ = {kSpeed, 0, 0};

		float angle = kAngleUnit * i;
		
		Matrix4x4 matrixRotation = MakeRotateZMatrix(angle);

		velocity_ = myMath->Transform(velocity_, matrixRotation);

		worldTransforms_[i].translation_.x += velocity_.x;
		worldTransforms_[i].translation_.y += velocity_.y;
		worldTransforms_[i].translation_.z += velocity_.z;

	}

	counter_ += 1.0f / 60.0f;
	if (counter_ >= kDuration) {
		counter_ = kDuration;
		isFinished_ = true;
	}

	color_.w = std::clamp(kDuration - counter_, 0.0f, 1.0f);
	objectColor_.SetColor(color_);
	objectColor_.TransferMatrix();
}

void DeathParticles::Draw() {
	if (isFinished_) {
		return;
	}
	for (auto& worldTransform : worldTransforms_) {
		model_->Draw(worldTransform, *viewProjection_,&objectColor_);
	}
}