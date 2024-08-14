#include "Model.h"
#include "WorldTransform.h"
#include "TextureManager.h"
#include "EnemyBullet.h"
#include "MyMath.h"

class Enemy {
public:

	enum class Phase {
		Approach,
		Leave,
	};

	~Enemy();
	void Intialize(Model* model,uint32_t textureHandle, ViewProjection* viewProjection);
	void Update();
	void Draw();
	void Fire();

private:

	Model* model_ = nullptr;

	uint32_t textureHandle_ = 0u;

	WorldTransform worldTransform_;

	ViewProjection* viewProjection_ = nullptr;

	MyMath* myMath_ = nullptr;

	Phase phase_ = Phase::Approach;
	std::list<EnemyBullet*> bullets_;

	float bulletTimer_ = 1.0f;

};