#include "Model.h"
#include "WorldTransform.h"
#include "DebugCamera.h"

class MapChipField;

enum class LRDirection {
	kRight,
	kLeft,
};

enum Corner {
	kRightBottom,
	kLeftBottom,
	kRightTop,
	kLeftTop,

	kNumCorner

};

class Player {

public:
	void Initialize(Model* model,ViewProjection* viewProjection,const Vector3& position);

	void Update();

	void Draw();
	void DebugDraw(DebugCamera* debug);

	const WorldTransform& GetWorldTransform() const { return worldTransform_; }
	const Vector3& Getvelocity() const { return velocity_; }

	void SetMapChipField(MapChipField* mapChipField) { mapChipField_ = mapChipField; }

	struct CollisionMapInfo {
		bool ceilingTachiFlag_ = false;
		bool landingFlag_ = false;
		bool wallTachiFlag_ = false;
		Vector3 moveCount;
	};

	void MapChipCollision(CollisionMapInfo& info);

	void MapChipCollisionUp(CollisionMapInfo& info);
	void MapChipCollisionDown(CollisionMapInfo& info);
	void MapChipCollisionRight(CollisionMapInfo& info);
	void MapChipCollisionLeft(CollisionMapInfo& info);

	void MapChipCollisionGround(CollisionMapInfo& info);
	
	static inline const float kWidth = 0.8f;
	static inline const float kHeight = 0.8f;

	Vector3 CornerPosition(const Vector3& center, Corner corner);

	void prosperity(const CollisionMapInfo& info);//hantei

	void CeilingTachi(const CollisionMapInfo& info);
	void CollisionWall(CollisionMapInfo& info);


private:
	MapChipField* mapChipField_ = nullptr;

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

	float positionNewSpace = 0.2f;

	static inline float kblank = 0.8f;

	static inline const float kAttenuationLanding = 0.8f;
	static inline const float kAttenuationWall = 0.3f;

	Vector3 Add(const Vector3& v1, const Vector3& v2) {
		Vector3 result{};
		result.x = v1.x + v2.x;
		result.y = v1.y + v2.y;
		result.z = v1.z + v2.z;
		return result;
	}
};
