//============================================================================
// エネミーB処理 [enemy_B.cpp]
// Author : 新里　将士
//
//=============================================================================
#include "enemy_B.h"
#include "enemy_A.h"
#include "model.h"
#include "bullet.h"
#include "enemyBullet.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	MODEL_ENEMY_B		"data/MODEL/BeeEnemy.x"							// 読み込むモデル名
#define TEXTURE_ENEMYFILE_B "data/TEXTURE/side_arrow.png"
#define	ENEMY_MOVE		(2.0f)											// 移動量
#define	ENEMY_ROTATE	(D3DX_PI * 0.02f)								// 回転量
#define NUM_ENEMY_B		(30)

int					enemyB_wave1;

int					beforeWaveFlag;

OBJECT3D			B_enemyWK[NUM_ENEMY_B];

D3DXVECTOR3			BeeToDistancePlayer[NUM_ENEMY_B];
D3DXVECTOR3			BeeEnemyBulletSpeed[NUM_ENEMY_B];

OBJECT3D			*enemyMantis;

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9	eb_pD3DTextureModel;		// テクスチャへのポインタ
LPD3DXMESH			eb_pD3DXMeshSphere;
LPD3DXMESH			eb_pD3DXMeshModel;			// メッシュ情報へのポインタ　読み込んだXファイルモデルを表す。モデルを増やしたかったらこれを増やす。
LPD3DXBUFFER		eb_pD3DXBuffMatModel;		// マテリアル情報へのポインタ　読み込んだXファイルモデルのマテリアルを表す。
DWORD				eb_nNumMatModel;			// マテリアル情報の数　読み込んだXファイルモデルのマテリアルの数。


//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitEnemy_B(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	enemyMantis = GetPlayer();

	eb_pD3DTextureModel = NULL;
	eb_pD3DXMeshModel = NULL;
	eb_pD3DXBuffMatModel = NULL;
	eb_nNumMatModel = 0;

	// Xファイルの読み込み
	if (FAILED(D3DXLoadMeshFromX(MODEL_ENEMY_B,				// 読み込むモデルファイル名(Xファイル)　ここを書き換えれば他のも表示できる。
		D3DXMESH_SYSTEMMEM,		// メッシュの作成オプションを指定
		pDevice,				// IDirect3DDevice9インターフェイスへのポインタ
		NULL,					// 隣接性データを含むバッファへのポインタ
		&eb_pD3DXBuffMatModel,	// マテリアルデータを含むバッファへのポインタ	マテリアル管理　	読み込んだXファイルモデルの情報がそれぞれ格納されている
		NULL,					// エフェクトインスタンスの配列を含むバッファへのポインタ							
		&eb_nNumMatModel,		// D3DXMATERIAL構造体の数															//
		&eb_pD3DXMeshModel)))	// ID3DXMeshインターフェイスへのポインタのアドレス									//
	{
		return E_FAIL;
	}

	

	D3DXVECTOR3 initPos[NUM_ENEMY_B] =
	{
		D3DXVECTOR3(100,50,0),
		D3DXVECTOR3(100,50,0),
		D3DXVECTOR3(0,50,0),
		D3DXVECTOR3(-100,50,0),
		D3DXVECTOR3(-100,50,0),
		//5
		//D3DXVECTOR3(-100,50,0),
		//D3DXVECTOR3(-150,50,0),
		//D3DXVECTOR3(-200,50,0),
		//D3DXVECTOR3(-250,50,0),
		//D3DXVECTOR3(-300,50,0),
		////10
		//D3DXVECTOR3(-100,20,0),
		//D3DXVECTOR3(-50,20,0),
		//D3DXVECTOR3(0,20,0),
		//D3DXVECTOR3(50,20,0),
		//D3DXVECTOR3(100,20,0),
		////15
		//D3DXVECTOR3(-100,20,0),
		//D3DXVECTOR3(50,20,0),
		//D3DXVECTOR3(0,20,0),
		//D3DXVECTOR3(50,20,0),
		//D3DXVECTOR3(100,20,0),
		////20
		//D3DXVECTOR3(-100,20,0),
		//D3DXVECTOR3(-50,20,0),
		//D3DXVECTOR3(0,20,0),
		//D3DXVECTOR3(50,20,0),
		//D3DXVECTOR3(100,20,0),
		////25
		//D3DXVECTOR3(-100,20,0),
		//D3DXVECTOR3(-50,20,0),
		//D3DXVECTOR3(0,20,0),
		//D3DXVECTOR3(50,20,0),
		//D3DXVECTOR3(100,20,0)
		//30
	};

	enemyB_wave1 = TRUE;
	

	for (int i = 0; i < NUM_ENEMY_B; i++)
	{
		// 位置・回転・スケールの初期設定
		B_enemyWK[i].m_bUse = FALSE;				//エネミー構造体の使用フラグをOFFにする
		B_enemyWK[i].m_posModel = initPos[i];
		B_enemyWK[i].m_rotModel = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		B_enemyWK[i].m_sclModel = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		B_enemyWK[i].m_DirSpeed = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

		DWORD VertexNum = eb_pD3DXMeshModel->GetNumVertices();
		DWORD VertexSize = eb_pD3DXMeshModel->GetNumBytesPerVertex();	//１つの頂点のサイズ
		BYTE *p;

		HRESULT Bhr = eb_pD3DXMeshModel->LockVertexBuffer(D3DLOCK_READONLY, (LPVOID*)&p);

		D3DXComputeBoundingSphere((D3DXVECTOR3*)p, VertexNum, VertexSize,
			&B_enemyWK[i].EnemyCenter, &B_enemyWK[i].EnemyRadius);

		eb_pD3DXMeshModel->UnlockVertexBuffer();

		D3DXCreateSphere(pDevice, B_enemyWK[i].EnemyRadius, 24, 24, &eb_pD3DXMeshSphere, NULL);

		B_enemyWK[i].m_Radius = D3DXVECTOR3(B_enemyWK[i].EnemyRadius, 0.0f, 0.0f);
	}


	return S_OK;
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawEnemy_B(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();


	D3DXMATERIAL *pD3DXMat_BE;
	D3DMATERIAL9 matDef_BE;		//マテリアル構造体のポインタ(D3DMATERIAL9構造体)


	//enemyMantis = GetPlayer();

	

	for (int i = 0; i < 5; i++)
	{
		
		if (B_enemyWK[i].m_bUse)
		{

			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&B_enemyWK[i].m_mtxWorld);

			// スケールを反映
			D3DXMatrixScaling(&B_enemyWK[i].m_mtxScl, B_enemyWK[i].m_sclModel.x, B_enemyWK[i].m_sclModel.y, B_enemyWK[i].m_sclModel.z);
			D3DXMatrixMultiply(&B_enemyWK[i].m_mtxWorld, &B_enemyWK[i].m_mtxWorld, &B_enemyWK[i].m_mtxScl);

			// 回転を反映
			D3DXMatrixRotationYawPitchRoll(&B_enemyWK[i].m_mtxRot, B_enemyWK[i].m_rotModel.y, B_enemyWK[i].m_rotModel.x, B_enemyWK[i].m_rotModel.z);
			CalcLookAtMatrix(
				&B_enemyWK[i].m_mtxRot,
				&B_enemyWK[i].m_posModel,
				&enemyMantis->m_posModel,
				&D3DXVECTOR3(0.0f, 1.0f, 0.0f));
			D3DXMatrixMultiply(&B_enemyWK[i].m_mtxWorld, &B_enemyWK[i].m_mtxWorld, &B_enemyWK[i].m_mtxRot);

			// 移動を反映
			D3DXMatrixTranslation(&B_enemyWK[i].m_mtxTranslate, B_enemyWK[i].m_posModel.x, B_enemyWK[i].m_posModel.y, B_enemyWK[i].m_posModel.z);
			D3DXMatrixMultiply(&B_enemyWK[i].m_mtxWorld, &B_enemyWK[i].m_mtxWorld, &B_enemyWK[i].m_mtxTranslate);

			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &B_enemyWK[i].m_mtxWorld);

			// 現在のマテリアルを取得
			pDevice->GetMaterial(&matDef_BE);		//現在セットされているマテリアルを取得して保存しておく。

			// マテリアル情報に対するポインタを取得
			pD3DXMat_BE = (D3DXMATERIAL*)eb_pD3DXBuffMatModel->GetBufferPointer();		//Xファイルから取得したマテリアル構造体配列の先頭のポインタを取得する。
			for (int nCntMat = 0; nCntMat < (int)eb_nNumMatModel; nCntMat++)		//取得したマテリアルの数だけループ
			{
				// マテリアルの設定　マテリアルをDirectXへセット
				pDevice->SetMaterial(&pD3DXMat_BE[nCntMat].MatD3D);		//構造体配列のマテリアルデータをデバイスへセット

				// テクスチャの設定
				pDevice->SetTexture(0, eb_pD3DTextureModel);

				// メッシュの描画
				eb_pD3DXMeshModel->DrawSubset(nCntMat);
			}
			pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

			//e_pD3DXMeshSphere->DrawSubset(0);
			pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
			//}

			// マテリアルをデフォルトに戻す
			pDevice->SetMaterial(&matDef_BE);
		}

	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateEnemy_B(void)
{
	//enemyMantis = GetPlayer();
	beforeWaveFlag = GetEnemyWave2();


	if (enemyMantis->m_posModel.z >= 2000 && beforeWaveFlag == false)
	{
		//E_wave1 = TRUE;
		if (enemyB_wave1)
		{
			for (int i = 0; i < 5; i++)
			{
				B_enemyWK[i].m_bUse = TRUE;
			}
			enemyB_wave1 = FALSE;
		}
	}

	for (int i = 0; i < 5; i++)
	{
		BeeToDistancePlayer[i] = enemyMantis->m_posModel - B_enemyWK[i].m_posModel;
		float BeeArea = D3DXVec3Length(&BeeToDistancePlayer[i]);

		BeeEnemyBulletSpeed[i].x = B_enemyWK[i].m_mtxRot._31;
		BeeEnemyBulletSpeed[i].y = B_enemyWK[i].m_mtxRot._32;
		BeeEnemyBulletSpeed[i].z = B_enemyWK[i].m_mtxRot._33;
		BeeEnemyBulletSpeed[i] *= -10.0f;

		if (B_enemyWK[i].m_bUse)
		{
			B_enemyWK[i].m_posModel.z = enemyMantis->m_posModel.z + (50.0f*i) - 50.0f *2 ;

			//float radian = PI * 2 / 180.0f * actionTime;
			//float moveLength = 100.0f * sinf(radian);
			//B_enemyWK[i].m_posModel.z += moveLength;

			if (BeeArea < 200)
			{
				CreateEnemyBullet(B_enemyWK[i].m_posModel, BeeEnemyBulletSpeed[i]);
			}


			//OBJECT3D* ebullet = GetBullet();
			//for (int n = 0; n < NUM_BULLET; n++)
			//{	//チェック関数を呼ぶ
			//	HitOBJECT3D(&ebullet[n], &enemyWK[i]);
			//}
		}
	}
	//EU_use1[i] = TRUE;


	for (int i = 0; i < 5; i++)
	{
		if (B_enemyWK[i].m_bUse)
		{

			OBJECT3D* ebullet = GetBullet();
			for (int n = 0; n < 10; n++)
			{
				if (ebullet[n].m_bUse)
				{
					//チェック関数を呼ぶ

					if (HitOBJECT3D(&ebullet[n], &B_enemyWK[i]) == TRUE)
					{
						ebullet[n].m_bUse = FALSE;
						B_enemyWK[i].m_bUse = FALSE;
					}

				}
			}
		}
	}
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitEnemy_B(void)
{
	if (eb_pD3DTextureModel != NULL)
	{// テクスチャの開放
		eb_pD3DTextureModel->Release();
		eb_pD3DTextureModel = NULL;
	}

	if (eb_pD3DXMeshModel != NULL)
	{// メッシュの開放
		eb_pD3DXMeshModel->Release();
		eb_pD3DXMeshModel = NULL;
	}

	if (eb_pD3DXBuffMatModel != NULL)
	{// マテリアルの開放
		eb_pD3DXBuffMatModel->Release();
		eb_pD3DXBuffMatModel = NULL;
	}
}