#include "Model.h"
#include "WorldTransform.h"

class Skydome {

public:
	void Initialize(Model* model,uint32_t textureHandle,ViewProjection* viewProjection );
	void Update();
	void Draw(const ViewProjection& viewProjection);

private:

	Model* model_ = nullptr;

	uint32_t textureHandle_ = 0u;

	WorldTransform worldTransform_;

	ViewProjection* viewProjection_;
};