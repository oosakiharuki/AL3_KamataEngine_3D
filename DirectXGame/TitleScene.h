#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "WorldTransform.h"
#include "ViewProjection.h"
#include "Fade.h"

class TitleScene {
public:

	enum class Phase {
		kFadeIn,
		kMain,
		kFadeOut,
	};

	~TitleScene();
	void Initialize();
	void Updata();
	void Draw();
	bool IsFinished() const { return finished_; }

private:
	Model* modelTitle_ = nullptr;

	WorldTransform worldTransform_;

	//uint32_t textureHandle_ = 0u;

	ViewProjection viewProjection_;

	bool finished_ = false;

	Fade* fade_ = nullptr;
	Phase phase_ = Phase::kFadeIn;
};