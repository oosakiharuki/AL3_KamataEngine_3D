#include "Model.h"
#include "WorldTransform.h"
#include "Input.h"
#include "MyMath.h"
#include "PlayerBullet.h"

#ifdef _DEBUG
#include "imgui.h"
#endif

#include <list>

class RailCamera;

class Player {

public:
	~Player();
	void Initialize(Model* model,uint32_t textureHandle,ViewProjection* viewProjection,Vector3 poaition);

	void Update(RailCamera* railCamera);

	void Draw();

	void Rotate();
	void Attack(RailCamera* railCamera);

	Vector3 GetWorldPosition();

	void OnCollision();
	const std::list<PlayerBullet*>& GetBullets() const { return bullets_; }

	void SetParent(const WorldTransform* parent);

private:
	WorldTransform worldTransform_;

	Model* model_ = nullptr;

	uint32_t textureHandle_ = 0u;

	ViewProjection* viewProjection_ = nullptr;

	Input* input_ = nullptr;


	MyMath* myMath_ = nullptr;

	//PlayerBullet* bullet_ = nullptr;

	std::list<PlayerBullet*> bullets_;

	RailCamera* railCamera_ = nullptr;
};
