#include "WorldTransform.h"
#include "MyMath.h"

void WorldTransform::UpdateMatrix() { 

	rotate.x = MaMakeRotateXMatrix(rotation_.x);
	rotate.y = MakeRotateXMatrix(rotation_.y);
	rotate.z = MakeRotateXMatrix(rotation_.z);

	rotateXYZ = Multiply(rotate.x,Multiply(rotate_.y,rotate_.z));

	matWorld_ = MakeAffineMatrix(scale, rotation_, translation_);

	TransferMatrix();
}