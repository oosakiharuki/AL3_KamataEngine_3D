#include "Model.h"
#include "WorldTransform.h"
#include "TextureManager.h"

class Enemy {
public:

	enum class Phase {
		Approach,
		Leave,
	};


	void Intialize(Model* model,uint32_t textureHandle, ViewProjection* viewProjection);
	void Update();
	void Draw();

private:

	Model* model_ = nullptr;

	uint32_t textureHandle_ = 0u;

	WorldTransform worldTransform_;

	ViewProjection* viewProjection_ = nullptr;

	Phase phase_ = Phase::Approach;

};