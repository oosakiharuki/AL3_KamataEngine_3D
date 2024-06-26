#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>


GameScene::GameScene() {

}

GameScene::~GameScene() { 
	delete modelPlayer_; 
	delete modelBlock_;
	delete modelEnemy_; 
	for (std::vector<WorldTransform*> worldTransformLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformLine) {
			delete worldTransformBlock;
		}
		worldTransformBlocks_.clear();
	}

	delete player_;
	for (Enemy* enemy : enemies_) {
		delete enemy;
	}
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
	modelBlock_ = Model::CreateFromOBJ("block",true);
	modelEnemy_ = Model::CreateFromOBJ("enemy", true) ,


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


	//敵キャラ生成
	for (int32_t i = 0; i < 1; i++) {

		Enemy* enemy_ = new Enemy();

		Vector3 enemyPosition = mapChipField_->GetMapChipPositionByIndex(10 + i * 3, 18);
		enemy_->Initialize(modelEnemy_, &viewProjection_, enemyPosition);

		enemies_.push_back(enemy_);
	}

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


bool GameScene::IsCollision(const AABB& aabb1, const AABB& aabb2) {
	if ((aabb1.min.x <= aabb2.max.x && aabb1.max.x >= aabb2.min.x) &&
	    (aabb1.min.y <= aabb2.max.y && aabb1.max.y >= aabb2.min.y) &&
	    (aabb1.min.z <= aabb2.max.z && aabb1.max.z >= aabb2.min.z)) {
		return true;
	}
	return false;
}



void GameScene::CheckAllCollisions() {
	// player and enemy
	AABB aabb1, aabb2;

	aabb1 = player_->GetAABB();

	for (Enemy* enemy : enemies_) {
		aabb2 = enemy->GetAABB();

		if (IsCollision(aabb1,aabb2)) {
			player_->OnCollision(enemy);
			enemy->OnCollision(player_);
		}

	}

}

void GameScene::Update() {
	// 自キャラ更新
	player_->Update();
	for (Enemy* enemy : enemies_) {
		enemy->Updata();
	}
	for (std::vector<WorldTransform*> worldTransformLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformLine) {
			if (!worldTransformBlock) {
				continue;
			} else {	
				worldTransformBlock->UpdateMatrix();			
			}
		}
	}


	CheckAllCollisions();


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
	player_->Draw();
	for (Enemy* enemy : enemies_) {
		enemy->Draw();
	}
	for (std::vector<WorldTransform*> worldTransformLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformLine) {
			if (!worldTransformBlock) {
				continue;
			} 
			else if (isDebugCameraActive_ == true) {
				modelBlock_->Draw(*worldTransformBlock, debugCamera_->GetViewProjection());
				player_->DebugDraw(debugCamera_);
			}
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
