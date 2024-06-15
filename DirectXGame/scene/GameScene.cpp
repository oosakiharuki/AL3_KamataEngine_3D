#include "GameScene.h"
#include "TextureManager.h"
#include "MyMath.h"
#include <cassert>


GameScene::GameScene() {

}

GameScene::~GameScene() { 
	delete modelPlayer_; 
	delete modelBlock_;
	for (std::vector<WorldTransform*> worldTransformLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformLine) {
			delete worldTransformBlock;
		}
		worldTransformBlocks_.clear();
	}

	delete player_;
	delete debugCamera_;

	delete mapChipField_;
	delete cameraController_;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	//3Dモデル
	modelPlayer_ = Model::CreateFromOBJ("player",true);
	modelBlock_ = Model::Create();

	//worldTransform_.Initialize();

	viewProjection_.Initialize();

	//マップチップ
	mapChipField_ = new MapChipField;
	mapChipField_->LoadMapChipCsv("Resources/block.csv");	
	
	//自キャラ生成
	player_ = new Player();

	Vector3 playerPosition = mapChipField_->GetMapChipPositionByIndex(1,18);

	player_->Initialize(modelPlayer_, &viewProjection_,playerPosition);

	player_->SetMapChipField(mapChipField_);

	
	//カメラ
	cameraController_ = new CameraController;
	cameraController_->Inyialize();
	cameraController_->SetTarget(player_);
	cameraController_->Reset();

	//デバッグカメラ
	debugCamera_ = new DebugCamera(1280, 720);


	//GenerateBlocks();
		
	const uint32_t kNumBlockVirtical = mapChipField_->GetkNumBlockVertical();
	const uint32_t kNumBlockHorizontal = mapChipField_->GetkNumBlockHorizontal();

	//const float kBlockWidth = 2.0f;
	//const float kBlockHeight = 2.0f;


	worldTransformBlocks_.resize(kNumBlockVirtical);
	for (uint32_t i = 0; i < kNumBlockVirtical; ++i) {
		worldTransformBlocks_[i].resize(kNumBlockHorizontal);
	}

	for (uint32_t i = 0; i < kNumBlockVirtical; ++i) {
		for (uint32_t j = 0; j < kNumBlockHorizontal; ++j) {
			if (mapChipField_->GetMapChipTypeByIndex(j , i) == MapChipType::kBlock) {
				WorldTransform* worldTransform_ = new WorldTransform();
				worldTransform_->Initialize();
				worldTransformBlocks_[i][j] = worldTransform_;
				worldTransformBlocks_[i][j]->translation_ = mapChipField_->GetMapChipPositionByIndex(j,i);
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
	cameraController_->Update();

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
	player_->Draw(debugCamera_);

	for (std::vector<WorldTransform*> worldTransformLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformLine) {
			if (!worldTransformBlock) {
				continue;
			} 
			//else if (isDebugCameraActive_ == true) {
			//	modelBlock_->Draw(*worldTransformBlock, debugCamera_->GetViewProjection());
			//}
			else {
				modelBlock_->Draw(*worldTransformBlock, viewProjection_);
				viewProjection_.matView = cameraController_->GetViewProjection().matView;
				viewProjection_.matProjection = cameraController_->GetViewProjection().matProjection;
				// ビュープロジェクション行列の転送
				viewProjection_.TransferMatrix();
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
