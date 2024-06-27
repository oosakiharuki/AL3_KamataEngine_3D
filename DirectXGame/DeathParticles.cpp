#include "DeathParticles.h"

void DeathParticles::Initialize(Model* model, ViewProjection* viewProjection, const Vector3& position) { 

	model_ = model;
	viewProjection_ = viewProjection;

	for (auto& worldTransform : worldTransforms_) {
		worldTransform.Initialize();
		worldTransform.translation_ = position;
	}
}

void DeathParticles::Updata() {
	for (auto& worldTransform : worldTransforms_) {
		worldTransform.UpdateMatrix();
	}
}

void DeathParticles::Draw() {
	for (auto& worldTransform : worldTransforms_) {
		model_->Draw(worldTransform, *viewProjection_);
	}
}