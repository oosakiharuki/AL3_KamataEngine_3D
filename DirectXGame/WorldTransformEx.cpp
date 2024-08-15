#include "WorldTransform.h"
#include "MyMath.h"
void WorldTransform::UpdateMatrix() {	
	
	static MyMath myMath_;
	matWorld_ = myMath_.MakeAffineMatrix(scale_, rotation_, translation_);

	if (parent_) {
		matWorld_.m[0][0] *= parent_->matWorld_.m[0][0];
		matWorld_.m[0][1] *= parent_->matWorld_.m[0][1];
		matWorld_.m[0][2] *= parent_->matWorld_.m[0][2];
		matWorld_.m[0][3] *= parent_->matWorld_.m[0][3];
		matWorld_.m[1][0] *= parent_->matWorld_.m[1][0];
		matWorld_.m[1][1] *= parent_->matWorld_.m[1][1];
		matWorld_.m[1][2] *= parent_->matWorld_.m[1][2];
		matWorld_.m[1][3] *= parent_->matWorld_.m[1][3];
		matWorld_.m[2][0] *= parent_->matWorld_.m[2][0];
		matWorld_.m[2][1] *= parent_->matWorld_.m[2][1];
		matWorld_.m[2][2] *= parent_->matWorld_.m[2][2];
		matWorld_.m[2][3] *= parent_->matWorld_.m[2][3];
		matWorld_.m[3][0] *= parent_->matWorld_.m[3][0];
		matWorld_.m[3][1] *= parent_->matWorld_.m[3][1];
		matWorld_.m[3][2] *= parent_->matWorld_.m[3][2];
		matWorld_.m[3][3] *= parent_->matWorld_.m[3][3];
	}

	TransferMatrix();
}