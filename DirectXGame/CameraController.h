#include "ViewProjection.h"
#include "WorldTransform.h"
#include"MyMath.h"

class Player;

class CameraController {
public:
	
	void Inyialize();
	void Update();
	void Reset();

	void SetTarget(Player* target) { target_ = target; }

private:
	
	ViewProjection viewProjection_;
	Player* target_ = nullptr;
	Vector3 targetOffset_ = {0, 0, -15.0f};
	MyMath* myMath;
};