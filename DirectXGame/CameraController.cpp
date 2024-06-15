#define NOMINMAX
#include "CameraController.h"
#include "Player.h"

void CameraController::Inyialize() { 
	viewProjection_.Initialize(); 	
	CameraController::Rect cameraArea = {10.0f, 100 - 10.0f, 6.0f, 6.0f};
	SetMovebleArea(cameraArea);
}

void CameraController::Reset() {
	
	const WorldTransform& targetWorldTransform = target_->GetWorldTransform();
	viewProjection_.translation_ = Add(targetWorldTransform.translation_ , targetOffset_);

}

void CameraController::Update() {
	const WorldTransform& targetWorldTransform = target_->GetWorldTransform();
	viewProjection_.translation_ = Add(Add(targetWorldTransform.translation_, targetOffset_),Calculate(target_->Getvelocity() , kvelocityBias));	

	viewProjection_.translation_.x = std::max(viewProjection_.translation_.x, movebleArea_.left);
	viewProjection_.translation_.x = std::min(viewProjection_.translation_.x, movebleArea_.right);
	viewProjection_.translation_.y = std::max(viewProjection_.translation_.y, movebleArea_.bottom);
	viewProjection_.translation_.y = std::min(viewProjection_.translation_.y, movebleArea_.top);

	viewProjection_.translation_.x = std::max(viewProjection_.translation_.x, target_->GetWorldTransform().translation_.x * margin.left);
	viewProjection_.translation_.x = std::min(viewProjection_.translation_.x, target_->GetWorldTransform().translation_.x * margin.right);
	viewProjection_.translation_.y = std::max(viewProjection_.translation_.y, target_->GetWorldTransform().translation_.y * margin.bottom);
	viewProjection_.translation_.y = std::min(viewProjection_.translation_.y, target_->GetWorldTransform().translation_.y * margin.top);


	viewProjection_.UpdateMatrix();
}