//=============================================================================
// ゲーム画面処理 [game.cpp]
// Author : 新里　将士
//
//=============================================================================
#include "game.h"
#include "bullet.h"
#include "model.h"
#include "enemy_A.h"
#include "enemy_B.h"
#include "particle.h"
#include "reticle.h"
#include "effect.h"
#include "enemyBullet.h"
#include "input.h"
#include "WallClass.h"
#include "FieldClass.h"
#include "LightClass.h"
#include "BackGroundClass.h"
#include "FadeClass.h"
#include "CameraClass.h"
#include "ReticleClass.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

//*****************************************************************************
// グローバル変数
//*****************************************************************************
float gameProgressTime;		//frameTime計測変数

WallClass wall;
FieldClass field;
LightClass light;
BackGroundClass bg;
//BulletClass bullet;
CameraClass camera;
ReticleClass reticle;
//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitGame(void)
{
	
	FadeClass* fade = getFadeMain();
	
	//ゲーム内経過時間の初期化
	gameProgressTime = 0.0f;

	// ライトの初期化
	light.InitLight();

	// カメラの初期化
	camera.InitCamera();
	camera.setTargetPostion(&reticle.reticlePos);
	camera.setTargetDirection(&reticle.reticleRot);

	// 弾の初期化
	InitBullet();
	//bullet.InitBullet();

	//エネミーの弾
	InitEnemyBullet();

	// モデルの初期化
	InitModel();

	// フィールドの初期化
	//InitField();
	field.InitField();

	// 背景の初期化
	//InitBG();
	bg.InitBG();

	// エネミーの初期化
	InitEnemy_A();
	InitEnemy_B();

	// パーティクルの初期化
	InitParticle();

	//ウォールの初期化
	//InitWall();
	wall.InitWall();

	// レティクルの初期化
	//InitReticle();
	reticle.InitReticle();

	//エフェクト
	InitEffect();

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitGame(void)
{
	//ライトの終了処理
	//UninitLight();


	//カメラの終了処理
	//UninitCamera();


	//モデルの終了処理
	UninitModel();

	//フィールドの終了処理
	//UninitField();
	field.UninitField();

	//背景の終了処理
	//UninitBG();
	bg.UninitBG();

	//エネミーの終了処理
	//UninitEnemy();
	UninitEnemy_A();
	UninitEnemy_B();
	
	//弾の終了処理
	UninitBullet();
	//bullet.UninitBullet();
	UninitEnemyBullet();

	//パーティクルの終了処理
	UninitParticle();

	//ウォールの終了処理
	//UninitWall();
	wall.UninitWall();

	//レティクルの終了処理
	//UninitReticle();
	reticle.UninitReticle();

	//エフェクトの終了処理
	UninitEffect();

	
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateGame(void)
{
	FadeClass* fade = getFadeMain();

	//frameTimeを足しいれる
	gameProgressTime += GetFrameTime();	

	// カメラ更新
	//UpdateCamera();
	camera.UpdateCamera();

	// 弾処理の更新
	UpdateBullet();
	//bullet.UpdateBullet();
	UpdateEnemyBullet();

	// モデルの更新処理
	UpdateModel();

	// フィールドの更新処理
	//UpdateField();
	field.UpdateField();

	// 背景の更新処理
	//UpdateBG();
	bg.UpdateBG();

	// エネミーの更新処理
	UpdateEnemy_A();
	UpdateEnemy_B();

	//ウォールの更新処理
	//UpdateWall();
	wall.UpdateWall();

	// レティクルの更新処理
	//UpdateReticle();
	reticle.UpdateReticle();

	//パーティクルの更新処理
	UpdateParticle();

	//エフェクトの更新処理
	UpdateEffect();

	//ゲーム時間が100fを超えたらゲーム終了
	if (gameProgressTime > 100.0f)	
	{
		fade->SetFade(FADE::FADE_OUT);
	}

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawGame(void)
{
	
	//カメラの設定
	//DrawCamera();
	camera.DrawCamera();

	//モデルの描画処理
	DrawModel();
	
	//フィールドの描画処理
	//DrawField();
	field.DrawField();

	//背景の描画処理
	//DrawBG();
	bg.DrawBG();

	//エネミーの描画処理
	DrawEnemy_A();
	DrawEnemy_B();

	//弾処理の描画
	DrawBullet(camera.g_mtxView);
	//bullet.DrawBullet(camera.g_mtxView);
	DrawEnemyBullet(camera.g_mtxView);

	//パーティクルの描画処理
	DrawParticle(camera.g_mtxView);

	//ウォールの描画処理
	//DrawWall();
	wall.DrawWall();

	//レティクルの描画
	//DrawReticle();
	reticle.DrawReticle(camera.g_mtxView);

	//エフェクトの描画
	DrawEffect(camera.g_mtxView);

	//ゲームシーンタイムの描画
	DrawGameTime(gameProgressTime);

}


