#include "ViewProjection.h"
#include "WorldTransform.h"

class Player;

class CameraController {
public:
	
	struct Rect {
		float left = 0.0f;
		float right = 1.0f;
		float bottom = 0.0f;
		float top = 1.0f;
	};

	void Inyialize();
	void Update();
	void Reset();

	void SetTarget(Player* target) { target_ = target; }
	const ViewProjection& GetViewProjection() const{ return viewProjection_; }


	Rect movebleArea_ = {0, 100, 0, 100};
	void SetMovebleArea(Rect& area) { movebleArea_ = area; }


private:
	
	ViewProjection viewProjection_;
	Player* target_ = nullptr;
	Vector3 targetOffset_ = {0, 0, -15.0f};
	
	static inline const float kvelocityBias = 10.0f;
	static inline const Rect margin = {-4.0f, 100 + 14.0f, -6.0f, 6.0f};

	Vector3 Add(const Vector3& v1, const Vector3& v2) {
		Vector3 result;
		result.x = v1.x + v2.x;
		result.y = v1.y + v2.y;
		result.z = v1.z + v2.z;
		return result;
	}
	Vector3 Calculate(const Vector3& v1, const float& v2) {
		Vector3 result;
		result.x = v1.x * v2;
		result.y = v1.y * v2;
		result.z = v1.z * v2;
		return result;
	}
};