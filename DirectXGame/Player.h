#include "Model.h"
#include "WorldTransform.h"
#include"Input.h"
#include "MyMath.h"
#include "imgui.h"
#include "DebugCamera.h"

class Player {

public:
	void Initialize(Model* model,uint32_t textureHandle,ViewProjection* viewProjection);

	void Update();

	void Draw();
	void DebugDraw(DebugCamera* debug);
	private:
	WorldTransform worldTransform_;

	Model* model_ = nullptr;

	uint32_t textureHandle_ = 0u;

	ViewProjection* viewProjection_ = nullptr;

	Input* input_ = nullptr;


	MyMath* myMath_ = nullptr;





};
