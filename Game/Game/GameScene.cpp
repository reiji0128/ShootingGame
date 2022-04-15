#include "GameScene.h"
#include "Game.h"
#include <iostream>
#include "Math.h"
#include "Renderer.h"
#include "DebugGrid.h"
#include "AudioManager.h"
#include "BitmapText.h"
#include "Input.h"
#include "PhysicsWorld.h"
#include "Texture.h"
#include "StaticBGActor.h"
#include "BGCollisionSetter.h"
#include "PlayerActor.h"
#include "ThirdPersonCameraActor.h";
#include "Gun.h"


GameScene::GameScene()
	:mFont(nullptr)
	,mTex (nullptr)
	,mGrid(nullptr)
{
	printf("-----------------GameScene-----------------\n");
	// フォント初期化
	mFont = new BitmapText;
	mFont->SetFontImage(16, 6, "Assets/Font/font.png");
	mFont->ReMapText(" !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_\tabcdefghijklmnopqrstuvwxyz{|}~\\");

	// 行列初期化
	Matrix4 proj;
	proj = Matrix4::CreatePerspectiveFOV(Math::ToRadians(45.0f), GAMEINSTANCE.GetRenderer()->GetScreenWidth(), GAMEINSTANCE.GetRenderer()->GetScreenHeight(), 1.0, 10000.0f);
	GAMEINSTANCE.GetRenderer()->SetProjMatrix(proj);

	//デバッググリッド
	Vector3 color(0, 1, 0);
	mGrid = new DebugGrid(2000, 20, color);

	// ライティング設定
	GAMEINSTANCE.GetRenderer()->SetAmbientLight(Vector3(0.2f, 0.2f, 0.2f));
	DirectionalLight& dir = GAMEINSTANCE.GetRenderer()->GetDirectionalLight();
	dir.mDirection = Vector3(0.7f, 0.7f, -0.7f);
	dir.mDiffuseColor = Vector3(0.78f, 0.88f, 1.0f);
	dir.mSpecColor = Vector3(0.8f, 0.8f, 0.8f);


	// プレイヤーの生成
	PlayerActor* player = new PlayerActor
	                          (Vector3(-3070.0, 2450.0, 20.0),        // 座標
		                       1.0f,                                  // スケール
		                       "Assets/Player/SpecialForces.gpmesh",  // gpMeshのファイルパス
		                       "Assets/Player/SpecialForces.gpskel"); // gpSkelのファイルパス
	
	// 銃の生成
	Gun* gun = new Gun(Vector3(90, -40, 140), Vector3(-15, 130, 0),
		               "Assets/Gun/SK_KA47.gpmesh",
		                player->GetSkeltalMeshComp(),
		               "LeftHandIndex4");

    // カメラの生成
	ThirdPersonCameraActor* camera = new ThirdPersonCameraActor(player);
	camera->SetCameraLength(800.0f);

	// バックグラウンドの生成
	new StaticBGActor(Vector3(-3070.0, 2450.0, 20.0), "Assets/BackGround/LowPolyMap.gpmesh");
	
	// バックグラウンドの当たり判定の生成
	new BGCollisionSetter("Assets/BackGround/CollisionBox.json");

	
	// ゲームシステムに当たり判定リストを登録する
	GAMEINSTANCE.GetPhysics()->SetOneSideReactionCollisionPair(Tag::Enemy, Tag::Player);
	GAMEINSTANCE.GetPhysics()->SetOneSideReactionCollisionPair(Tag::BackGround, Tag::Player);
	GAMEINSTANCE.GetPhysics()->SetOneSideReactionCollisionPair(Tag::BackGround, Tag::Enemy);
	GAMEINSTANCE.GetPhysics()->SetOneSideReactionCollisionPair(Tag::PlayerBullet, Tag::Enemy);
	GAMEINSTANCE.GetPhysics()->SetOneSideReactionCollisionPair(Tag::EnemyBullet, Tag::Player);
}

GameScene::~GameScene()
{
	delete mGrid;
}

SceneBase* GameScene::Update()
{
	static float time = 0;
	time += 0.01f;

	// 当たり判定表示モードの切り替え
	if (INPUT_INSTANCE.IsKeyPushdown(KEY_START))
	{
		GAMEINSTANCE.GetPhysics()->ToggleDebugMode();
	}
	RENDERER->GetEffekseerManager()->Update();
	
	return this;
}

void GameScene::Draw()
{
	glClearColor(0.2f, 0.5f, 0.9f, 1.0f);
	// 画面クリア
	GAMEINSTANCE.GetRenderer()->WindowClear();

	// ゲームシステム関連の描画（コンポーネント系のものはここですべて描画される)
	GAMEINSTANCE.GetRenderer()->Draw();

	// 2D描画の開始処理
	RENDERER->SpriteDrawBegin();

	// 2D描画の終了処理
	RENDERER->SpriteDrawEnd();

// エフェクト関連の処理 //
	// エフェクト描画の開始処理
	RENDERER->GetEffekseerRenderer()->BeginRendering();
	RENDERER->GetEffekseerManager()->Draw();
	// エフェクト描画の終了処理
	RENDERER->GetEffekseerRenderer()->EndRendering();

	// 画面フリップ
	GAMEINSTANCE.GetRenderer()->WindowFlip();
}
