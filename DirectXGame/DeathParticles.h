#include <array>
#include "Model.h"
#include "WorldTransform.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include "MyMath.h"

class DeathParticles{
public:
	void Initialize(Model* model, ViewProjection* viewProjection, const Vector3& position);
	Matrix4x4 MakeRotateZMatrix(float rotate_);
	bool Isfinfshed() { return isFinished_; }

	void Updata();
	void Draw();

private:

	static inline const uint32_t kNumParticles = 8;

	Model* model_ = nullptr;
	ViewProjection* viewProjection_ = nullptr;

	std::array<WorldTransform, kNumParticles> worldTransforms_;
	
	MyMath* myMath = new MyMath();

	static inline const float pi = 2 * float(M_PI);//2 pai

	static inline const float kDuration = 2.0f;
	static inline const float kSpeed = 0.05f;
	static inline const float kAngleUnit = pi / kNumParticles;

	bool isFinished_ = false;
	float counter_ = 0.0f;

	ObjectColor objectColor_;
	Vector4 color_;


};