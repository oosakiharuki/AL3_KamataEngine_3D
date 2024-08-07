#include "TitleScene.h"
#include "DirectXCommon.h"
#include <cassert>

TitleScene::~TitleScene() { 
	delete modelTitle_;
	delete fade_;
}

void TitleScene::Initialize() { 
	assert(modelTitle_);

	//textureHandle_ = TextureManager::Load("Title/Title.png");
	modelTitle_ = Model::CreateFromOBJ("Title", true);
	worldTransform_.Initialize();
	viewProjection_.Initialize();

	fade_ = new Fade();
	fade_->Intialize();
	fade_->Start(Fade::Status::FadeIn, 1);
}

void TitleScene::Updata() {
	switch (phase_) {
	case Phase::kFadeIn:
		if (fade_->IsFinished()) {
			phase_ = Phase::kMain;
		}
		break;
	case Phase::kMain:
		fade_->Stop();
		worldTransform_.TransferMatrix();
		if (Input::GetInstance()->PushKey(DIK_SPACE)) {
			//finished_ = true;
			fade_->Start(Fade::Status::FadeOut, 1);
			phase_ = Phase::kFadeOut;
		}
		break;
	case Phase::kFadeOut:
		
		if (fade_->IsFinished()) {
			finished_ = true;
		}
		break;
	}

	fade_->Update();
}

void TitleScene::Draw() {

	DirectXCommon* dxCommon_ = DirectXCommon::GetInstance();
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

	Model::PreDraw(commandList);
	modelTitle_->Draw(worldTransform_,viewProjection_); 

	fade_->Draw(commandList);
}
