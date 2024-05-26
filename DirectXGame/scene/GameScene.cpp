#include "GameScene.h"
#include "TextureManager.h"
#include "MyMath.h"
#include <cassert>


GameScene::GameScene() {}

GameScene::~GameScene() { 
	delete model_; 
	//delete modelBlock_;
	for (std::vector<WorldTransform*> worldTransformLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformLine) {
			delete worldTransformBlock;
		}
		worldTransformBlocks_.clear();
	}

	delete player_;
	delete debugCamera_;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	//3Dモデル
	textureHandle_ = TextureManager::Load("Mario.jpg");
	
	model_ = Model::Create();
	modelBlock_ = Model::Create();

	worldTransform_.Initialize();

	viewProjection_.Initialize();

	//自キャラ生成
	player_ = new Player();

	player_->Initialize(model_, textureHandle_, &viewProjection_);

	//デバッグカメラ
	debugCamera_ = new DebugCamera(1280, 720);
	
	const uint32_t kNumBlockVirtical = 10;
	const uint32_t kNumBlockHorizontal = 20;

	const float kBlockWidth = 2.0f;
	const float kBlockHeight = 2.0f;

		
	worldTransformBlocks_.resize(kNumBlockVirtical);
	for (uint32_t i = 0; i < kNumBlockVirtical; ++i) {
		worldTransformBlocks_[i].resize(kNumBlockHorizontal);
	}

	for (uint32_t i = 0; i < kNumBlockVirtical; ++i) {
		for (uint32_t j = 0; j < kNumBlockHorizontal; ++j) {
			// uint32_t evenNum = (i + j) Mod 2;
			uint32_t evenNum = (i + j) - (2 * ((i + j) / 2)); // 偶数

			if (evenNum == 0) {
				worldTransformBlocks_[i][j] = new WorldTransform();
				worldTransformBlocks_[i][j]->Initialize();
				worldTransformBlocks_[i][j]->translation_.x = kBlockWidth * j;
				worldTransformBlocks_[i][j]->translation_.y = kBlockHeight * i;
			} 
			else {
				continue;
			}
		}
	}
}

void GameScene::Update() {
	// 自キャラ更新
	player_->Update();

	for (std::vector<WorldTransform*> worldTransformLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformLine) {
			if (!worldTransformBlock) {
				continue;
			} else {	
				worldTransformBlock->UpdateMatrix();			
			}
		}
	}
	debugCamera_->Update();

#ifdef _DEBUG 
	if (input_->TriggerKey(DIK_SPACE)) {
		isDebugCameraActive_ = true;
	}
#endif 


}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>

	// 自キャラ描画
	// player_->Draw();

	for (std::vector<WorldTransform*> worldTransformLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformLine) {
			if (!worldTransformBlock) {
				continue;
			} 
			else if (isDebugCameraActive_ == true) {
				modelBlock_->Draw(*worldTransformBlock, debugCamera_->GetViewProjection());
			}
			else {
				modelBlock_->Draw(*worldTransformBlock, viewProjection_);
			}
		}
	}


	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>
	//sprite_->Draw();

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}
