#include "RailCamera.h"

void RailCamera::Initialize(ViewProjection* viewProjection) {

	worldTramsform_.Initialize();
	worldTramsform_.translation_ = viewProjection->translation_;
	worldTramsform_.rotation_ = viewProjection->rotation_;

	Vector3 Translation = {0.0f, 0.0f, 0.0f};

	worldTramsform_.translation_.x = Translation.x;
	worldTramsform_.translation_.y = Translation.y;
	worldTramsform_.translation_.z = Translation.z;

	Vector3 Rotate = {0.0f, 0.0f, 0.0f};

	worldTramsform_.rotation_.x = Rotate.x;
	worldTramsform_.rotation_.y = Rotate.y;
	worldTramsform_.rotation_.z = Rotate.z;

	viewProjection_ = viewProjection;

	viewProjection_->farZ = 400.0f;
	
}

Vector3 RailCamera::GetWorldTranslation() {

	Vector3 kCameraTranslation = {0.0f, 0.0f,-0.1f};

	worldTramsform_.translation_.x += kCameraTranslation.x;
	worldTramsform_.translation_.y += kCameraTranslation.y;
	worldTramsform_.translation_.z += kCameraTranslation.z;

	return worldTramsform_.translation_;
}

Vector3 RailCamera::GetWorldRotate() {

	Vector3 kCameraRotate = {0.0f, 0.0f, 0.0f};

	worldTramsform_.rotation_.x += kCameraRotate.x;
	worldTramsform_.rotation_.y += kCameraRotate.y;
	worldTramsform_.rotation_.z += kCameraRotate.z;

	return worldTramsform_.rotation_;
}

void RailCamera::Update() {

	////GetWorldTranslation();
	////GetWorldRotate();

	worldTramsform_.UpdateMatrix();

	viewProjection_->matView = myMath_->Inverse(worldTramsform_.matWorld_);


	ImGui::Begin("Camera");
	ImGui::DragFloat3("translation", &worldTramsform_.translation_.x, 0.1f, -100.0f,100.0f);
	ImGui::DragFloat3("rotate", &worldTramsform_.rotation_.x, 0.01f, -10.0f, 10.0f);
	ImGui::End();
}
