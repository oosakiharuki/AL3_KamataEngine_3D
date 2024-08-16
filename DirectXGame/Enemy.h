#include "Model.h"
#include "WorldTransform.h"
#include "TextureManager.h"
#include "EnemyBullet.h"
#include "MyMath.h"

class Player;
class GameScene;

class Enemy {
public:

	enum class Phase {
		Approach,
		Leave,
	};

	~Enemy();
	void Intialize(Model* model,uint32_t textureHandle, ViewProjection* viewProjection,Vector3 position);
	void Update();
	void Draw();
	//void Fire();

	void SetPlayer(Player* player) { player_ = player; }

	Vector3 GetWorldPosition();

	void OnCollision();

	void SetGameScene(GameScene* gameScene) { gameScene_ = gameScene; }
	const WorldTransform* GetWorldTransform()const  { return &worldTransform_; }

	bool IsDead() const { return isDead_; }

private:

	Model* model_ = nullptr;

	uint32_t textureHandle_ = 0u;

	WorldTransform worldTransform_;

	ViewProjection* viewProjection_ = nullptr;

	MyMath* myMath_ = nullptr;


	Phase phase_ = Phase::Approach;
	std::list<EnemyBullet*> bullets_;

	//float bulletTimer_ = 1.0f;

	Player* player_ = nullptr;
	GameScene* gameScene_ = nullptr;

	bool isDead_ = false;
};