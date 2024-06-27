#include <array>
#include "Model.h"
#include "WorldTransform.h"
//#include "ViewProjection.h"

class DeathParticles{
public:
	void Initialize(Model* model, ViewProjection* viewProjection, const Vector3& position);
	void Updata();
	void Draw();

private:

	static inline const uint32_t kNumParticles = 0;

	Model* model_ = nullptr;
	ViewProjection* viewProjection_ = nullptr;

	std::array<WorldTransform, kNumParticles> worldTransforms_;
	

};