#include "Model.h"
#include "WorldTransform.h"
#include "DebugCamera.h"

enum class LRDirection {
	kRight,
	kLeft,
};
class Player {

public:
	void Initialize(Model* model,ViewProjection* viewProjection,const Vector3& position);

	void Update();

	void Draw(DebugCamera* debug);

	const WorldTransform& GetWorldTransform() const{ return worldTransform_; }
	const Vector3& Getvelocity() const { return velocity_; }

private:
	WorldTransform worldTransform_;

	Model* model_ = nullptr;

	uint32_t textureHandle_ = 0u;

	ViewProjection* viewProjection_ = nullptr;

	Vector3 velocity_ = {};
	static inline const float kAcceleration = 0.01f;
	static inline const float kAttenuation = 0.02f;
	static inline const float kLimitRunSpeed = 0.3f;

	LRDirection lrDirection_ = LRDirection::kRight;

	float turnFirstRotationY_ = 0.0f;

	float turnTimer_ = 0.0f;

	static inline float kTimeTurn = 0.3f;

	bool onGraund_ = true;

	static inline float kGravityAcceleration = 0.1f;
	static inline float kLimitFallSpeed = 0.4f; 
	static inline float kJumpAcceleration = 1.0f;

	bool landing = false;


	Vector3 Add(const Vector3& v1, const Vector3& v2) {
		Vector3 result;
		result.x = v1.x + v2.x;
		result.y = v1.y + v2.y;
		result.z = v1.z + v2.z;
		return result;
	}
};
