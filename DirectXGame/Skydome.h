#include "Model.h"
#include "WorldTransform.h"

class Skydome {
public:
	void Initialize(Model* model, uint32_t textureHandle, ViewProjection* viewProjection);

	void Update();

	void Draw();

private:
	//ワールド返還データ
	WorldTransform worldTransform_;


	uint32_t textureHandle_ = 0u;

	//モデル
	Model* model_;
	
	ViewProjection* viewProjection_;
};