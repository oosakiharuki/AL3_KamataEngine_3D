#include "WorldTransform.h"

void WorldTransform::UpdateMatrix() {
	matWorld_ = myMath_->MakeAffineMatrix(scale_, rotation_, translation_);

	TransferMatrix();
}

void WorldTransform::Initialize() {}


void WorldTransform::CreateConstBuffer() {}


void WorldTransform::Map() {}


void WorldTransform::TransferMatrix() {}