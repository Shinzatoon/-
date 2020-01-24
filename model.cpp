//============================================================================
// モデル処理 [model.cpp]
// Author : 新里　将士
//
//=============================================================================
#include "model.h"
#include "input.h"
#include "bullet.h"
#include "reticle.h"
#include "effect.h"
//#include "BulletClass.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEXTURE_FILENAME "data/TEXTURE/テクスチャ迷彩.jpg"
#define	VALUE_MOVE		(6.0f)											// 移動量
#define	VALUE_ROTATE	(D3DX_PI / 180.0f)								// 回転量
#define INVINCIBLE_TIME (2.0f)											//無敵時間
#define BLINK_INTERVAL	(0.5f)											//点滅間隔


#define PAR_INERTIA		(0.9f)						//慣性力
#define SPEED_LIMIT		(3.0f)						//限界速度



//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************


//*****************************************************************************
// グローバル変数
//*****************************************************************************

D3DXVECTOR3				CazrelativeVector;				//弾の発射位置の調整

D3DXVECTOR3				rot2 = D3DXVECTOR3(0, 0, 0);

D3DXVECTOR3				rot = D3DXVECTOR3(0, 0, 0);

D3DXVECTOR3				 ReticleVector = D3DXVECTOR3(0, 0, 0);
D3DXVECTOR3				 ReticleLength = D3DXVECTOR3(0, 0, 0);

D3DXVECTOR3				 EffectVector = D3DXVECTOR3(0, 0, 0);
D3DXVECTOR3				 EffectLength = D3DXVECTOR3(0, 0, 0);

D3DXVECTOR3				Temporary = D3DXVECTOR3(0, 0, 0);

//OBJECT3D				*ParentBulletWK;


bool renderFlag;
bool hit;
float blinkFrameTime;
float invincibleTime;
float blinkTime;

//*************キャラクターデータ変数の用意*****************************
#define		NUM_PLAYER	(3)
OBJECT3D	Player[NUM_PLAYER];			//プレイヤーキャラクタ構造体

//*************モデルーデータ変数の用意*****************************
#define		NUM_MODEL	(3)
MODEL3D		PlayerModel[NUM_MODEL];		//プレイヤーモデル構造体


float speedinertia;								//速度慣性保存
float rotationinertia;

//*********************モデルデータファイル名配列********************
const char* ModelData[] =
{
	"data/MODEL/戦車-改良2.2.x",
	"data/MODEL/戦車-砲台改良2.2.x",
	"data/MODEL/戦車-主砲改良2.2.x",

};



//float					g_Ofset = 1.0f;				// カメラの画角の変更

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitModel(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	//BazrelativeVector = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	//MODEL *model = &modelWK;

	//ParentBulletWK = GetParentBullet();
	speedinertia = 0.0f;
	rotationinertia = 0.0f;
	blinkFrameTime = 0.0f;
	renderFlag = true;
	hit = false;

	invincibleTime = 0.0f;
	blinkTime = 0.0f;
	
	// モデルデータの初期設定
	for (int i = 0; i < NUM_MODEL; i++)
	{
		PlayerModel[i].m_pD3DTextureModel = NULL;
		PlayerModel[i].m_pD3DXMeshModel = NULL;
		PlayerModel[i].m_pD3DXBuffMatModel = NULL;
		PlayerModel[i].m_nNumMatModel = 0;

		// Xファイルの読み込み
		if (D3D_OK != D3DXLoadMeshFromX(ModelData[i],					// 読み込むモデルファイル名(Xファイル)　ここを書き換えれば他のも表示できる。
								D3DXMESH_SYSTEMMEM,						// メッシュの作成オプションを指定
								pDevice,								// IDirect3DDevice9インターフェイスへのポインタ
								NULL,									// 隣接性データを含むバッファへのポインタ
								&PlayerModel[i].m_pD3DXBuffMatModel,	// マテリアルデータを含むバッファへのポインタ	マテリアル管理　	読み込んだXファイルモデルの情報がそれぞれ格納されている
								NULL,									// エフェクトインスタンスの配列を含むバッファへのポインタ							
								&PlayerModel[i].m_nNumMatModel,			// D3DXMATERIAL構造体の数
								&PlayerModel[i].m_pD3DXMeshModel))		// ID3DXMeshインターフェイスへのポインタのアドレス									
		{
			return E_FAIL;
		}

		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,					// デバイスへのポインタ
			TEXTURE_FILENAME,		// ファイルの名前
			&PlayerModel[i].m_pD3DTextureModel);	// 読み込むメモリ
		for (int i=0; i < NUM_PLAYER; i++)
		{
			// 位置・回転・スケールの初期設定
			Player[i].m_posModel = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			Player[i].m_rotModel = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			Player[i].m_sclModel = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

			Player[i].m_DirSpeed = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			Player[i].m_RotSpeed = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
			Player[i].m_Kakudo = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			Player[i].m_Radius = D3DXVECTOR3(300.0f, 1.0f, 385.0f);
			Player[i].m_ModelID = i;
			//Player[i].m_ShadowID = i;

			if (i != 0) 
			{
				SetRotParent(i, &Player[i - 1].m_mtxRot);
				SetParent(i, &Player[i - 1].m_mtxWorld);
				//SetParent(2, &ParentBulletWK->m_mtxWorld);
			}

		}

		Player[1].m_posModel = D3DXVECTOR3(0.0f, 20.0f, 0.0f);			//モデルの細かい調整
		Player[2].m_posModel = D3DXVECTOR3(0.0f, 0.0f, 10.0f);

	}


	
#if 0
	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,					// デバイスへのポインタ
								TEXTURE_FILENAME,		// ファイルの名前
								&g_pD3DTextureModel);	// 読み込むメモリー
#endif

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitModel(void)
{
	for (int i = 0; i < NUM_MODEL; i++)
	{
		if(PlayerModel[i].m_pD3DTextureModel != NULL)
		{
			// テクスチャの開放
			PlayerModel[i].m_pD3DTextureModel->Release();
			PlayerModel[i].m_pD3DTextureModel = NULL;
		}

		if(PlayerModel[i].m_pD3DXMeshModel != NULL)
		{
			// メッシュの開放
			PlayerModel[i].m_pD3DXMeshModel->Release();
			PlayerModel[i].m_pD3DXMeshModel = NULL;
		}

		if(PlayerModel[i].m_pD3DXBuffMatModel != NULL)
		{
			// マテリアルの開放
			PlayerModel[i].m_pD3DXBuffMatModel->Release();
			PlayerModel[i].m_pD3DXBuffMatModel = NULL;
		}

		}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateModel(void)
{

	
	D3DXVECTOR3 speed;
	speed.x = 0.0f;
	speed.y = 0.0f;
	speed.z = 0.0f;

	D3DXVECTOR3 rot1;
	rot1.x = 0.0f;
	rot1.y = 0.0f;
	rot1.z = 0.0f;

	D3DXVECTOR3 Bulletpos;
	Bulletpos.x = 0.0f;
	Bulletpos.y = 0.0f;
	Bulletpos.z = 0.0f;

	D3DXVECTOR3 Bulletspeed;
	Bulletspeed.x = 0.0f;
	Bulletspeed.y = 0.0f;
	Bulletspeed.z = 0.0f;

	D3DXVECTOR3 ModelWorldmtxZ;
	ModelWorldmtxZ.x = 0.0f;
	ModelWorldmtxZ.y = 0.0f;
	ModelWorldmtxZ.z = 0.0f;

	

	rot.x = 0.0f;
	rot.y = 0.0f;
	rot.z = 0.0f;

	ReticleVector.x = 0.0f;
	ReticleVector.y = 0.0f;
	ReticleVector.z = 0.0f;

	ReticleLength.x = 0.0f;
	ReticleLength.y = 0.0f;
	ReticleLength.z = 0.0f;
	
	EffectVector.x = 0.0f;
	EffectVector.y = 0.0f;
	EffectVector.z = 0.0f;

	EffectLength.x = 0.0f;
	EffectLength.y = 0.0f;
	EffectLength.z = 0.0f;

	//エフェクトをセット, エフェクトへのポインタ変数
	EffectVector.x = Player[0].m_virtualMtxRot._11;
	EffectVector.y = Player[0].m_virtualMtxRot._12;
	EffectVector.z = Player[0].m_virtualMtxRot._13;

	ModelWorldmtxZ.x = Player[0].m_virtualMtxRot._31;
	ModelWorldmtxZ.y = Player[0].m_virtualMtxRot._32;
	ModelWorldmtxZ.z = Player[0].m_virtualMtxRot._33;

	EffectLength.x = Player[0].m_mtxWorld._41;
	EffectLength.y = Player[0].m_mtxWorld._42;
	EffectLength.z = Player[0].m_mtxWorld._43;

	//エフェクト（土埃）の設定
	D3DXVECTOR3 move;
	move.x = Player[0].m_virtualMtxRot._31;
	move.y = Player[0].m_virtualMtxRot._32;
	move.z = Player[0].m_virtualMtxRot._33;
	float fAngle, fLength;
	int nLife;
	float fSize;

				
	fAngle = (float)(rand() % 628 - 314) / 100.0f;			//散らばる範囲（角度）
	fLength = rand() % (int)(5.0f * 20) / 10.0f - 5.0f;		//散らばる範囲（距離）
	move.x = sinf(fAngle) * 0.0f;							//
	move.y = rand() % 10 / 10.0f + 5.0f * 0.0f;
	move.z = 0.0f;
	nLife = rand() % 90 + 15;
	fSize = 5 + (float)(rand() * (10 - 5 + 1.0) / (1.0 + RAND_MAX));


	
	speed.x = Player[0].m_mtxRot._31;
	speed.y = Player[0].m_mtxRot._32;
	speed.z = Player[0].m_mtxRot._33;
	speed *= speedinertia;

	//車体の移動
	if (GetKeyboardPress(DIK_S) || IsButtonPressed(0, STICK_RIGHT) || IsButtonPressed(0, STICK_RIGHTUP) || IsButtonPressed(0, STICK_RIGHTDOWN))
	{
		speedinertia = speedinertia * PAR_INERTIA - SPEED_LIMIT * (1.0f - PAR_INERTIA);
	
		//移動エフェクト（土煙）
		SetEffect(Player[0].m_posModel + (EffectVector * 10) + (ModelWorldmtxZ * 20),
			move, D3DXCOLOR(0.45f, 0.30f, 0.18f, 1.0f), fSize, fSize, nLife);
		SetEffect(Player[0].m_posModel + (EffectVector * -10) + (ModelWorldmtxZ * 20),
			move, D3DXCOLOR(0.45f, 0.30f, 0.18f, 1.0f), fSize, fSize, nLife);
	}
	else if (GetKeyboardPress(DIK_W) || IsButtonPressed(0, STICK_LEFT) || IsButtonPressed(0, STICK_LEFTUP) || IsButtonPressed(0, STICK_LEFTDOWN))
	{

		speedinertia = speedinertia * PAR_INERTIA + SPEED_LIMIT * (1.0f - PAR_INERTIA);
		
		//移動エフェクト（土煙）
		SetEffect(Player[0].m_posModel + (EffectVector * 10) + (ModelWorldmtxZ * -20),
			move, D3DXCOLOR(0.45f, 0.30f, 0.18f, 1.0f), fSize, fSize, nLife);
		SetEffect(Player[0].m_posModel + (EffectVector * -10) + (ModelWorldmtxZ * -20),
			move, D3DXCOLOR(0.45f, 0.30f, 0.18f, 1.0f), fSize, fSize, nLife);
		//speed.x = Player[0].m_mtxRot._31;
		//speed.y = Player[0].m_mtxRot._32;
		//speed.z = Player[0].m_mtxRot._33;
	}
	else
	{
		speedinertia = speedinertia * PAR_INERTIA;
	}


	//モデルのx方向への限界移動処理
	if (Player[0].m_posModel.x >= 470)
	{
		Player[0].m_posModel.x = 470;
	}
	if (Player[0].m_posModel.x <= -470)
	{
		Player[0].m_posModel.x = -470;
	}


	//車体の回転　Y軸
	if (!GetKeyboardPress(DIK_A) || !IsButtonPressed(0, STICK_DOWN))
	{
		if (GetKeyboardPress(DIK_D) || IsButtonPressed(0, STICK_UP) || IsButtonPressed(0, STICK_RIGHTUP) || IsButtonPressed(0, STICK_LEFTUP))
		{
			//rot.x = Player[0].m_mtxRot._21;
			rot.y += VALUE_ROTATE;
			//rot.z = Player[0].m_mtxRot._23;
			//rot *= +VALUE_ROTATE;
			if (rot.y >= D3DXToRadian(10.0f))
			{
				rot.y = D3DXToRadian(10.0f);
			}
			//移動エフェクト（土煙）
			SetEffect(Player[0].m_posModel + (EffectVector * 10) + (ModelWorldmtxZ * 20),
				move, D3DXCOLOR(0.45f, 0.30f, 0.18f, 1.0f), fSize, fSize, nLife);
			SetEffect(Player[0].m_posModel + (EffectVector * -10) + (ModelWorldmtxZ * 20),
				move, D3DXCOLOR(0.45f, 0.30f, 0.18f, 1.0f), fSize, fSize, nLife);
			//移動エフェクト（土煙）
			SetEffect(Player[0].m_posModel + (EffectVector * 10) + (ModelWorldmtxZ * -20),
				move, D3DXCOLOR(0.45f, 0.30f, 0.18f, 1.0f), fSize, fSize, nLife);
			SetEffect(Player[0].m_posModel + (EffectVector * -10) + (ModelWorldmtxZ * -20),
				move, D3DXCOLOR(0.45f, 0.30f, 0.18f, 1.0f), fSize, fSize, nLife);
		}
	}
	
	if (!GetKeyboardPress(DIK_D) || !IsButtonPressed(0, STICK_UP))
	{
		if (GetKeyboardPress(DIK_A) || IsButtonPressed(0, STICK_DOWN) || IsButtonPressed(0, STICK_RIGHTDOWN) || IsButtonPressed(0, STICK_LEFTDOWN))
		{
			//rot.x = Player[0].m_mtxRot._21;
			rot.y += -VALUE_ROTATE;
			//rot.z = Player[0].m_mtxRot._23;
			//rot *= -VALUE_ROTATE;
			if (rot.y <= D3DXToRadian(-10.0f))
			{
				rot.y = D3DXToRadian(-10.0f);
			}
			//移動エフェクト（土煙）
			SetEffect(Player[0].m_posModel + (EffectVector * 10) + (ModelWorldmtxZ * 20),
				move, D3DXCOLOR(0.45f, 0.30f, 0.18f, 1.0f), fSize, fSize, nLife);
			SetEffect(Player[0].m_posModel + (EffectVector * -10) + (ModelWorldmtxZ * 20),
				move, D3DXCOLOR(0.45f, 0.30f, 0.18f, 1.0f), fSize, fSize, nLife);
			//移動エフェクト（土煙）
			SetEffect(Player[0].m_posModel + (EffectVector * 10) + (ModelWorldmtxZ * -20),
				move, D3DXCOLOR(0.45f, 0.30f, 0.18f, 1.0f), fSize, fSize, nLife);
			SetEffect(Player[0].m_posModel + (EffectVector * -10) + (ModelWorldmtxZ * -20),
				move, D3DXCOLOR(0.45f, 0.30f, 0.18f, 1.0f), fSize, fSize, nLife);
		}
	}
	
	
	// 砲台の回転
	if (!GetKeyboardPress(DIK_LEFT) || !IsButtonPressed(1, STICK_UP))
	{
		if (GetKeyboardPress(DIK_RIGHT) || IsButtonPressed(1, STICK_DOWN) || IsButtonPressed(1, STICK_RIGHTDOWN) || IsButtonPressed(1, STICK_LEFTDOWN))
		{

			//rot1.x = Player[1].m_mtxRot._21;
			rot1.y += VALUE_ROTATE;
			//rot1.z = Player[1].m_mtxRot._23;
			//rot1 *= +VALUE_ROTATE;

		}
	}
	
	if (!GetKeyboardPress(DIK_RIGHT) || !IsButtonPressed(1, STICK_DOWN))
	{
		if (GetKeyboardPress(DIK_LEFT) || IsButtonPressed(1, STICK_UP) || IsButtonPressed(1, STICK_RIGHTUP) || IsButtonPressed(1, STICK_LEFTUP))
		{

			//rot1.x = Player[1].m_mtxRot._21;
			rot1.y += -VALUE_ROTATE;
			//rot1.z = Player[1].m_mtxRot._23;
			//rot1 *= -VALUE_ROTATE;

		}
	}
	
	//主砲の上下運動
	if (GetKeyboardPress(DIK_UP) || IsButtonPressed(1, STICK_RIGHT) || IsButtonPressed(1, STICK_RIGHTDOWN) || IsButtonPressed(1, STICK_RIGHTUP))
	{
		
		rot2.x += -VALUE_ROTATE;
		//rot2.y = Player[2].m_mtxRot._12;
		//rot2.z = Player[2].m_mtxRot._13;
		//rot2 *= -VALUE_ROTATE;
		if (rot2.x <= D3DXToRadian(-30.0f))
		{
			rot2.x = D3DXToRadian(-30.0f);
		}

	}
	
	if (GetKeyboardPress(DIK_DOWN) || IsButtonPressed(1, STICK_LEFT) || IsButtonPressed(1, STICK_LEFTDOWN) || IsButtonPressed(1, STICK_LEFTUP))
	{
		rot2.x += VALUE_ROTATE;
		if (rot2.x >= D3DXToRadian(0.0f))
		{
			rot2.x = D3DXToRadian(0.0f);
		}
	}
	
	//モデルの位置
	Player[0].m_posModel += speed;					
	//モデルの向き(回転)g_posCameraP
	Player[0].m_rotModel += rot;					
	// モデルの向き(回転)g_posCameraP
	Player[1].m_rotModel += rot1;					
	Player[2].m_rotModel = rot2;


	// 弾発射
	if (GetKeyboardTrigger(DIK_SPACE) || IsButtonTriggered(1, BUTTON_ZR))
	{
		//弾の発射向きを主砲の向きと同機する
		Bulletspeed.x = Player[2].m_virtualMtxRot._31;
		Bulletspeed.y = Player[2].m_virtualMtxRot._32;
		Bulletspeed.z = Player[2].m_virtualMtxRot._33;
		//弾の発射位置を主砲の位置と合わせるための同機と調整
		CazrelativeVector.x = Player[2].m_mtxWorld._41;
		CazrelativeVector.y = Player[2].m_mtxWorld._42;
		CazrelativeVector.z = Player[2].m_mtxWorld._43;
		Bulletpos += CazrelativeVector + Bulletspeed * 30.0f;
		//弾のスピード
		Bulletspeed *= 30.0f;		

		//プレイヤーの位置と弾のスピードを与えて作成
		CreateBullet(Bulletpos, Bulletspeed);
		
	}
	
	//レティクル（照準）をセット, レティクルへのポインタ変数
	ReticleVector.x = Player[2].m_virtualMtxRot._31;
	ReticleVector.y = Player[2].m_virtualMtxRot._32;
	ReticleVector.z = Player[2].m_virtualMtxRot._33;
	//レティクル（照準）をセット, レティクルへのポインタ変数
	ReticleLength.x = Player[2].m_mtxWorld._41;
	ReticleLength.y = Player[2].m_mtxWorld._42;
	ReticleLength.z = Player[2].m_mtxWorld._43;




	

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawModel(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	
	
	D3DXMATERIAL *pD3DXMat;
	D3DMATERIAL9 matDef;		//マテリアル構造体のポインタ(D3DMATERIAL9構造体)

	// 現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);		//現在セットされているマテリアルを取得して保存しておく。


	////被弾時の無敵エフェクト（点滅）
	//if (GetKeyboardTrigger(DIK_SPACE))
	//{
	//	hit = true;
	//}
	//if (hit == true)
	//{
	//	invincibleTime = INVINCIBLE_TIME; 
	//	renderFlag = !renderFlag;
	//	hit = false;
	//}
	//if (invincibleTime > 0)
	//{
	//	invincibleTime -= GetFrameTime();
	//	blinkTime += GetFrameTime();
	//	if (blinkTime > BLINK_INTERVAL)
	//	{
	//		blinkTime = 0;
	//		renderFlag = !renderFlag;
	//	}
	//}
	//else
	//{
	//	renderFlag = true;
	//}
	

		//*************************全てのキャラクターを表示************************
		for (int i = 0; i < NUM_PLAYER; i++)
		{
			D3DXVECTOR3 temp;
			
			


			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&Player[i].m_mtxWorld);
			D3DXMatrixIdentity(&Player[i].m_virtualMtxRot);


			// スケールを反映(スケールを行列にしてワールド行列(はじめは単位行列)と乗算)
			temp = Player[i].m_sclModel;
			D3DXMatrixScaling(&Player[i].m_mtxScl, temp.x, temp.y, temp.z);
			D3DXMatrixMultiply(&Player[i].m_mtxWorld, &Player[i].m_mtxWorld, &Player[i].m_mtxScl);

			// 回転を反映(回転を行列にしてワールド行列と乗算)
			temp = Player[i].m_rotModel;
			D3DXMatrixRotationYawPitchRoll(&Player[i].m_mtxRot, temp.y, temp.x, temp.z);
			D3DXMatrixMultiply(&Player[i].m_mtxWorld, &Player[i].m_mtxWorld, &Player[i].m_mtxRot);

			// 移動を反映(移動を行列にしてワールド行列と乗算)
			temp = Player[i].m_posModel;
			D3DXMatrixTranslation(&Player[i].m_mtxTranslate, temp.x, temp.y, temp.z);
			D3DXMatrixMultiply(&Player[i].m_mtxWorld, &Player[i].m_mtxWorld, &Player[i].m_mtxTranslate);

			//MAINTANKを親設定にする
			D3DXMatrixMultiply(&Player[i].m_virtualMtxRot, &Player[i].m_virtualMtxRot, &Player[i].m_mtxRot);
			for (int j = i; j >= 0; j--)
			{
				if (Player[j].m_mtxRotParent != NULL)
				{
					D3DXMatrixMultiply(&Player[i].m_virtualMtxRot, &Player[i].m_virtualMtxRot, Player[j].m_mtxRotParent); //仮想回転行列に子と親の回転行列を乗算した結果を格納する
				}
			}
			if (Player[i].m_mtxParent != NULL)
			{
				D3DXMatrixMultiply(&Player[i].m_mtxWorld, &Player[i].m_mtxWorld, Player[i].m_mtxParent);
			}
			// ワールドマトリックスの設定(完成したワールドマトリックスをDirectXへセット)
			pDevice->SetTransform(D3DTS_WORLD, &Player[i].m_mtxWorld);

			// このキャラクターが使うモデルのID
			int modelno = Player[i].m_ModelID;


			// モデルのマテリアル情報に対するポインタを取得
			pD3DXMat = (D3DXMATERIAL*)PlayerModel[modelno].m_pD3DXBuffMatModel->GetBufferPointer();		//Xファイルから取得したマテリアル構造体配列の先頭のポインタを取得する。

			// モデルの情報に従ってモデルを構成するオブジェクトの数だけ繰り返す
			for (int nCntMat = 0; nCntMat < (int)PlayerModel[modelno].m_nNumMatModel; nCntMat++)		//取得したマテリアルの数だけループ
			{
				// オブジェクトマテリアルの設定　マテリアルをDirectXへセット
				pDevice->SetMaterial(&pD3DXMat[nCntMat].MatD3D);		//構造体配列のマテリアルデータをデバイスへセット

				// オブジェクトテクスチャの設定
				pDevice->SetTexture(0, PlayerModel[modelno].m_pD3DTextureModel);

				if (renderFlag)
				{
						// オブジェクトメッシュの描画
						PlayerModel[modelno].m_pD3DXMeshModel->DrawSubset(nCntMat);
				}
			}

		}
	

	// マテリアルをデフォルトに戻す
	pDevice->SetMaterial(&matDef);

}

void SetParent(int i, D3DXMATRIX* newParent)
{
	Player[i].m_mtxParent = newParent;
}

void SetRotParent(int i, D3DXMATRIX* newParent)
{
	Player[i].m_mtxRotParent = newParent;
}




OBJECT3D *GetPlayer(void)
{
	return(&Player[0]);
}

MODEL3D *GetModel(void)
{
	return(&PlayerModel[0]);
}

OBJECT3D *GetPlayer1(void)
{
	return(&Player[1]);
}

OBJECT3D *GetPlayer2(void)
{
	return(&Player[2]);
}

D3DXVECTOR3 *GetReticleVector(void)
{
	return(&ReticleVector);
}

D3DXVECTOR3 *GetReticleLength(void)
{
	return(&ReticleLength);
}

D3DXVECTOR3 *GetEffectVector(void)
{
	return(&EffectVector);
}

D3DXVECTOR3 *GetEffectLength(void)
{
	return(&EffectLength);
}