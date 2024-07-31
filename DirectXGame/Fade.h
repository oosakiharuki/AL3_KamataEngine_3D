#include <Sprite.h>




class Fade{
public:
	enum class Status {
	None,
	FadeIn,
	FadeOut,
};
	void Intialize();
	void Update();
	void Draw(ID3D12GraphicsCommandList* commandList);
	void Start(Status status, float duration);

private:
	Sprite* sprite_ = nullptr;
	uint32_t textureHandle_ = 0u;

	Status status_ = Status::FadeIn;
	float duration_ = 0.0f;
	float countTimer_ = 0.0f;
};