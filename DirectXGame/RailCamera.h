#include "WorldTransform.h"
#include "ViewProjection.h"
#include "MyMath.h"

#ifdef _DEBUG
#include "imgui.h"
#endif

class RailCamera {
public:
	void Initialize(ViewProjection* viewProjection);
	void Update();


	Vector3 GetWorldTranslation();
	Vector3 GetWorldRotate();
	//Matrix4x4 GetMatView();

	const WorldTransform& GetWorldTransform() const { return worldTramsform_; } 

private:
	WorldTransform worldTramsform_;
	ViewProjection* viewProjection_;
	MyMath* myMath_;

	//Matrix4x4 matView;
	//Vector3 viewTransform;
	//Vector3 viewRotate;
};