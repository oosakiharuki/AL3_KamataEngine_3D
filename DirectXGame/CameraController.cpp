#include "CameraController.h"
#include "Player.h"

void CameraController::Inyialize() { 
	viewProjection_.Initialize(); 	
}

void CameraController::Reset() {
	
	const WorldTransform& targetWorldTransform = target_->GetWorldTransform();
	viewProjection_.translation_ = myMath->Add(targetWorldTransform.translation_ , targetOffset_);

}

void CameraController::Update() {
	const WorldTransform& targetWorldTransform = target_->GetWorldTransform();
	viewProjection_.translation_ = myMath->Add(targetWorldTransform.translation_, targetOffset_);	

	viewProjection_.UpdateMatrix();
}