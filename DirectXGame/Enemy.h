#include "Model.h"
#include "WorldTransform.h"
#include "ViewProjection.h"

class Enemy {
public:
	void Initialize(Model* model, ViewProjection* viewProjection, const Vector3& position);
	void Updata();
	void Draw();

private:
	Model* model_;
	ViewProjection* viewProjection_;
	WorldTransform worldTransform_;
	
	static inline float kWalkSpeed = 1.0f;
	Vector3 velocity_ = {};
};