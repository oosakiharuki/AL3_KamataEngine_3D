#include "Model.h"
#include "WorldTransform.h"
#include"Input.h"

class Player {

public:
	void Initialize(Model* model,uint32_t textureHandle,ViewProjection* viewProjection);

	void Update();

	void Draw();

private:
	WorldTransform worldTransform_;

	Model* model_ = nullptr;

	uint32_t textureHandle_ = 0u;

	ViewProjection* viewProjection_ = nullptr;

	Input* input_ = nullptr;

	Vector3 move = {0, 0, 0};
	const float kPlayerSpeed = 2.0f;

};
