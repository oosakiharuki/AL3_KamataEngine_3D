#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "Player.h"
#include "Enemy.h"

#include "DebugCamera.h"
#include<vector>

#include "MapChipField.h"
#include "CameraController.h"

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {

public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();



	//void GenerateBlocks();


private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	/// <summary>
	/// ゲームシーン用
	/// </summary>

	//テクスチャハンドル
	uint32_t textureHandle_ = 0;
	
	//3Dモデルデータ
	Model* modelPlayer_ = nullptr;
	Model* modelBlock_ = nullptr;
	Model* modelEnemy_ = nullptr;

	//WorldTransform worldTransform_;

	//ビュープロジェクション
	ViewProjection viewProjection_;

	std::vector<std::vector<WorldTransform*>> worldTransformBlocks_;

	//自分キャラ
	Player* player_ = nullptr;

	//敵キャラ
	//Enemy* enemy_ = nullptr;

	std::list<Enemy*> enemies_;

	bool IsCollision(const AABB& aabb1, const AABB& aabb2);
	void CheckAllCollisions();



	// カメラコントロール
	CameraController* cameraController_;

	//デバッグ
	bool isDebugCameraActive_ = false;

	DebugCamera* debugCamera_ = nullptr;

	MapChipField* mapChipField_;

};
