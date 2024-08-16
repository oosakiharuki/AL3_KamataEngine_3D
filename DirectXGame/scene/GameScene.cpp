#include "GameScene.h"
#include "TextureManager.h"
#include "AxisIndicator.h"
#include <cassert>
#include <fstream>

GameScene::GameScene() {}

GameScene::~GameScene() { 
	delete model_; 
	delete player_;
	for (Enemy* enemy_ : enemies_) {
		delete enemy_;
	}

	for (EnemyBullet* bullet : enemyBullets_) {
		delete bullet;
	} 

	delete skydomeModel_;
	delete skydome_;
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

	for (EnemyBullet* bullet : enemyBullets_) {
		AddEnemyBullet(bullet);
	}


	// スカイドーム
	skydomeModel_ = Model::CreateFromOBJ("skydome", true);
	textureHandleSkydome_ = TextureManager::Load("skydome/skydome.png");
	skydome_ = new Skydome();
	skydome_->Initialize(skydomeModel_, textureHandleSkydome_, &viewProjection_);




	debugCamera_ = new DebugCamera(1280, 720);
#ifdef _DEBUG
	AxisIndicator::GetInstance()->SetVisible(true);
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);
#endif
}


void GameScene::CheckAllCollisions() { 
	Vector3 posA, posB; 

	const std::list<PlayerBullet*>& playerBullets = player_->GetBullets();
	
	const std::list<EnemyBullet*>& enemyBullets = enemyBullets_;


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

	for (Enemy* enemy_ : enemies_) {
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

void GameScene::AddEnemyBullet(EnemyBullet* enemyBullet) { 

	enemyBullets_.push_back(enemyBullet);

}




void GameScene::LoadEnemyPopData() {

	std::ifstream file;
	file.open("Resources/EnemyPop.csv");
	assert(file.is_open());

	enemyPopCommands << file.rdbuf();

	file.close();
}

void GameScene::UpdateEnemyPopCommands() { 
	
	//if A
	if (WaitFlag) {
		waitTimer--;
		if (waitTimer <= 0) {
			WaitFlag = false;
		}
		return;
	}

	std::string line;

	while (getline(enemyPopCommands, line)) {
	
		std::istringstream line_stream(line);

		std::string word;

		getline(line_stream, word, ',');

		if (word.find("//") == 0) {
			continue;
		}

		if (word.find("POP") == 0) {
			getline(line_stream, word, ',');
			float x = (float)std::atof(word.c_str());

			getline(line_stream, word, ',');
			float y = (float)std::atof(word.c_str());

			getline(line_stream, word, ',');
			float z = (float)std::atof(word.c_str());


			EnemyBorn(Vector3(x, y, z));
			
		
		} else if (word.find("WAIT") == 0) {
			getline(line_stream, word, ',');

			int32_t waitTime = atoi(word.c_str());

			WaitFlag = true;
			waitTimer = waitTime;

			break;//待機時間にif Aを使うため一度while文から抜ける
		}

	}

}

void GameScene::Fire() {
	const float deltaTime = 1.0f / 60.0f;

	bulletTimer_ -= deltaTime;

	if (bulletTimer_ <= 0.0f) {

		assert(player_);

		const float kBulletSpeed = 0.5f;

		Vector3 playerPostion = player_->GetWorldPosition();

		Vector3 enemyPostion;

		for (Enemy* enemy_ : enemies_) {
			enemyPostion = enemy_->GetWorldPosition();
		}

		Vector3 subtract = myMath_->Subtract(playerPostion, enemyPostion);

		Vector3 normalize = myMath_->Normalize(subtract);

		normalize.x *= kBulletSpeed;
		normalize.y *= kBulletSpeed;
		normalize.z *= kBulletSpeed;

		Vector3 velocity(normalize);
		for (Enemy* enemy_ : enemies_) {
			velocity = myMath_->TransformNormal(velocity, enemy_->GetWorldTransform()->matWorld_);
		}
		EnemyBullet* newBullet = new EnemyBullet();
		newBullet->Initialize(model_, enemyPostion, velocity);

		enemyBullets_.push_back(newBullet);
		bulletTimer_ = 1.0f;
	}
}

void GameScene::EnemyBorn(Vector3 position) {
	// 敵生成
	for (int32_t i = 0; i < 5; i++) {
		Enemy* enemy_ = new Enemy();
		enemy_->Intialize(model_, EnemyTextureHandle_, &viewProjection_, position);
		enemy_->SetGameScene(this);



		enemy_->SetPlayer(player_);
		enemies_.push_back(enemy_);
	}
}



void GameScene::Update() { 
	// 自キャラ更新
	player_->Update();

	LoadEnemyPopData();
	UpdateEnemyPopCommands();

	Fire();//一つのみ反映される

	for (Enemy* enemy_ : enemies_) {
		enemy_->Update();
	}


	// Bullet Dead Timer
	enemies_.remove_if([](Enemy* enemy) {
		if (enemy->IsDead()) {

			delete enemy;
			return true;
		}
		return false;
	});


	// bullet_ != nullptr
	for (EnemyBullet* bullet_ : enemyBullets_) {
		bullet_->Update();
	}

	/// Bullet Dead Timer
	enemyBullets_.remove_if([](EnemyBullet* bullet) {
		if (bullet->IsDead()) {

			delete bullet;
			return true;
		}
		return false;
	});


	CheckAllCollisions();

	skydome_->Update();
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

	skydome_->Draw(viewProjection_);

	// 自キャラ描画
	player_->Draw();
	for (Enemy* enemy_ : enemies_) {
		enemy_->Draw();
	}

	for (EnemyBullet* bullet_ : enemyBullets_) {
		bullet_->Draw(viewProjection_);
	}



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
