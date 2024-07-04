#include "TitleScene.h"
#include "TextureManager.h"

void TitleScene::Initialize() { 

	textureHandle_ = TextureManager::Load("Title/Title.png");
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
	//modelTitle_->Draw(worldTransform_,viewProjection_,textureHandle_); //koko ga okashii
}
