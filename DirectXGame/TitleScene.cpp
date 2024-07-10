#include "TitleScene.h"
#include "TextureManager.h"
#include "DirectXCommon.h"
#include <cassert>

void TitleScene::Initialize() { 
	assert(modelTitle_);

	//textureHandle_ = TextureManager::Load("Title/Title.png");
	modelTitle_ = Model::CreateFromOBJ("Title", true);
	worldTransform_.Initialize();
	viewProjection_.Initialize();
}

void TitleScene::Updata() {
	worldTransform_.TransferMatrix(); 
	if (Input::GetInstance()->PushKey(DIK_SPACE)) {
		finished_ = true;
	}

}

void TitleScene::Draw() {

	DirectXCommon* dxCommon_ = DirectXCommon::GetInstance();
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

	Model::PreDraw(commandList);
	modelTitle_->Draw(worldTransform_,viewProjection_); //koko ga okashii
}
