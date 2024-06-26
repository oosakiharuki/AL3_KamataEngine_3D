#include "Model.h"
#include "WorldTransform.h"
#include "ViewProjection.h"

#include "MyMath.h"
class Player;

class Enemy {
public:
	void Initialize(Model* model, ViewProjection* viewProjection, const Vector3& position);
	void Updata();
	void Draw();

	Vector3 GetWorldPosition();
	AABB GetAABB();

	void OnCollision(const Player* player);

private:
	Model* model_;
	ViewProjection* viewProjection_;
	WorldTransform worldTransform_;
	
	static inline float kWalkSpeed = 0.01f;
	static inline float kWalkMotionAngleStart = 1.0f;
	static inline float kWalkMotionAngleEnd = -1.0f;
	static inline float kWalkMotionAngletime = 2.0f;

	float walkTimer_ = 0.0f;

	Vector3 velocity_ = {};

	static inline const float kWidth = 0.8f;
	static inline const float kHeight = 0.8f;
};