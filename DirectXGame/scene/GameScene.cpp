#include "GameScene.h"
#include "TextureManager.h"
#include "AxisIndicator.h"
#include <cassert>


GameScene::GameScene() {}

GameScene::~GameScene() { 
	delete model_; 
	delete player_;
	delete enemy_;

	delete debugCamera_;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	//3Dモデル
	textureHandle_ = TextureManager::Load("Mario.jpg");
	EnemyTextureHandle_ = TextureManager::Load("uvChecker.png");

	model_ = Model::Create();

	viewProjection_.Initialize();

	//自キャラ生成
	player_ = new Player();

	player_->Initialize(model_, textureHandle_, &viewProjection_);


	enemy_ = new Enemy(); 

	enemy_->Intialize(model_, EnemyTextureHandle_, &viewProjection_);

	enemy_->SetPlayer(player_);

	debugCamera_ = new DebugCamera(1280, 720);
#ifdef _DEBUG
	AxisIndicator::GetInstance()->SetVisible(true);
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);
#endif
}


void GameScene::CheckAllCollisions() { 
	Vector3 posA, posB; 

	const std::list<PlayerBullet*>& playerBullets = player_->GetBullets();
	
	const std::list<EnemyBullet*>& enemyBullets = enemy_->GetBullets();


	//case radius
	const float plaeyrRadius = 1.0f;

	const float EnemyRadius = 1.0f;


	for (EnemyBullet* bullet : enemyBullets) {

		posA = player_->GetWorldPosition();
		posB = bullet->GetWorldPosition();

		Vector3 distance{};

		distance.x = (posB.x - posA.x) * (posB.x - posA.x); 	
		distance.y = (posB.y - posA.y) * (posB.y - posA.y);	    
		distance.z = (posB.z - posA.z) * (posB.z - posA.z);

		float L;

		L = (plaeyrRadius + EnemyRadius) * (plaeyrRadius + EnemyRadius);


		if (distance.x + distance.y + distance.z <= L) {

			player_->OnCollision();
			bullet->OnCollision();
		}
	}


	for (PlayerBullet* bullet : playerBullets) {

		posA = enemy_->GetWorldPosition();
		posB = bullet->GetWorldPosition();

		Vector3 distance{};

		distance.x = (posB.x - posA.x) * (posB.x - posA.x);
		distance.y = (posB.y - posA.y) * (posB.y - posA.y);
		distance.z = (posB.z - posA.z) * (posB.z - posA.z);

		float L;

		L = (plaeyrRadius + EnemyRadius) * (plaeyrRadius + EnemyRadius);

		if (distance.x + distance.y + distance.z <= L) {

			enemy_->OnCollision();
			bullet->OnCollision();
		}
	}


	for (PlayerBullet* bulletP : playerBullets) {
		for (EnemyBullet* bulletE : enemyBullets) {

			posA = bulletP->GetWorldPosition();
			posB = bulletE->GetWorldPosition();

			Vector3 distance{};

			distance.x = (posB.x - posA.x) * (posB.x - posA.x);
			distance.y = (posB.y - posA.y) * (posB.y - posA.y);
			distance.z = (posB.z - posA.z) * (posB.z - posA.z);

			float L;

			L = (plaeyrRadius + EnemyRadius) * (plaeyrRadius + EnemyRadius);

			if (distance.x + distance.y + distance.z <= L) {

				bulletP->OnCollision();
				bulletE->OnCollision();
			}
		}
	}
}



void GameScene::Update() { 
	// 自キャラ更新
	player_->Update();

	enemy_->Update();

	CheckAllCollisions();

#ifdef _DEBUG
	debugCamera_->Update();

	if (input_->TriggerKey(DIK_0)) {
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


	//自キャラ描画
	player_->Draw();

	enemy_->Draw();

	 if (isDebugCameraActive_ == true) {
		viewProjection_.matView = debugCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = debugCamera_->GetViewProjection().matProjection;
		viewProjection_.TransferMatrix();		
	} else {
		viewProjection_.UpdateMatrix();
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
