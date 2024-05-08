#include "Model.h"
#include "WorldTransform.h"

class Player {

public:
	void Initialize();

	void Update();

	void Draw();

private:
	WorldTransform worldTransform_;

	Model* model = nullptr;

	uint32_t textureHandle_ = 0u;
};
