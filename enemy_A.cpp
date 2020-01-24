//============================================================================
// エネミーA処理 [enemy_A.cpp]
// Author : 新里　将士
//
//=============================================================================
#include "enemy_A.h"	//修正予定
#include "model.h"
#include "input.h"
#include "bullet.h"
#include "particle.h" 
#include "enemyBullet.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	MODEL_ENEMY		"data/MODEL/EnemyMantis.x"				// 読み込むモデル名
#define	ENEMY_MOVE		(2.0f)									// 移動量
#define	ENEMY_ROTATE	(D3DX_PI * 0.02f)						// 回転量
#define NUM_ENEMY		(10)

#define ENEMY_A_WAVE_1	(5)
#define ENEMY_A_WAVE_2	(10)

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9	e_pD3DTextureModel;		// テクスチャへのポインタ
LPD3DXMESH			e_pD3DXMeshSphere;		// 当たり判定用のメッシュ情報
LPD3DXMESH			e_pD3DXMeshModel;		// メッシュ情報へのポインタ　読み込んだXファイルモデルを表す。
LPD3DXBUFFER		e_pD3DXBuffMatModel;	// マテリアル情報へのポインタ　読み込んだXファイルモデルのマテリアルを表す。
DWORD				e_nNumMatModel;			// マテリアル情報の数　読み込んだXファイルモデルのマテリアルの数。

OBJECT3D			enemyWK[NUM_ENEMY];		//エネミー情報

bool				E_wave1;				//Wave発生フラグ
int					cntWaveEnemy1;			//Wave１内のtrue状態のエネミーをカウント
bool				occurrenceWate1;		//Wavw1エネミー発生待機状態

bool				E_wave2;				//Wave2発生フラグ
int					cntWaveEnemy2;			//Wave2内のtrue状態のエネミーをカウント
bool				occurrenceWate2;		//Wavw2エネミー発生待機状態

D3DXVECTOR3			distancePlayer[NUM_ENEMY];			//プレイヤーとエネミーの距離
D3DXVECTOR3			enemyBulletSpeed[NUM_ENEMY];		//エネミーのバレット速度


OBJECT3D			*pEnemyModel0;			//プレイヤー情報の取得


//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitEnemy_A(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	pEnemyModel0 = GetPlayer();

	E_wave1 = false;
	cntWaveEnemy1 = 0;
	occurrenceWate1 = true;

	E_wave2 = false;
	occurrenceWate2 = true;

	e_pD3DTextureModel = NULL;
	e_pD3DXMeshModel = NULL;
	e_pD3DXBuffMatModel = NULL;
	e_nNumMatModel = 0;

	// Xファイルの読み込み
	if (FAILED(D3DXLoadMeshFromX(MODEL_ENEMY,		// 読み込むモデルファイル名(Xファイル)　ここを書き換えれば他のも表示できる。
		D3DXMESH_SYSTEMMEM,							// メッシュの作成オプションを指定
		pDevice,									// IDirect3DDevice9インターフェイスへのポインタ
		NULL,										// 隣接性データを含むバッファへのポインタ
		&e_pD3DXBuffMatModel,						// マテリアルデータを含むバッファへのポインタ	マテリアル管理　	読み込んだXファイルモデルの情報がそれぞれ格納されている
		NULL,										// エフェクトインスタンスの配列を含むバッファへのポインタ							
		&e_nNumMatModel,							// D3DXMATERIAL構造体の数
		&e_pD3DXMeshModel)))						// ID3DXMeshインターフェイスへのポインタのアドレス					
	{
		return E_FAIL;
	}

	
	D3DXVECTOR3 initPos[NUM_ENEMY] =
	{
		D3DXVECTOR3(-100,25,0),
		D3DXVECTOR3(-50,25,0),
		D3DXVECTOR3(0,25,0),
		D3DXVECTOR3(50,25,0),
		D3DXVECTOR3(100,25,0),
		//5
		D3DXVECTOR3(-100,25,0),
		D3DXVECTOR3(-150,25,0),
		D3DXVECTOR3(-200,25,0),
		D3DXVECTOR3(-250,25,0),
		D3DXVECTOR3(-300,25,0),
		//10
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

	E_wave1 = true;
	E_wave2 = true;

	for (int i = 0; i < NUM_ENEMY; i++)
	{
		// 位置・回転・スケールの初期設定
		enemyWK[i].m_bUse = false;				//エネミー構造体の使用フラグをOFFにする
		enemyWK[i].m_posModel = initPos[i];
		enemyWK[i].m_rotModel = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		enemyWK[i].m_sclModel = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		enemyWK[i].m_DirSpeed = D3DXVECTOR3(1.0f, 1.0f, 1.0f);


		DWORD VertexNum = e_pD3DXMeshModel->GetNumVertices();
		DWORD VertexSize = e_pD3DXMeshModel->GetNumBytesPerVertex();	//１つの頂点のサイズ
		BYTE *p;

		HRESULT Bhr = e_pD3DXMeshModel->LockVertexBuffer(D3DLOCK_READONLY, (LPVOID*)&p);

		D3DXComputeBoundingSphere((D3DXVECTOR3*)p, VertexNum, VertexSize,
									&enemyWK[i].EnemyCenter, &enemyWK[i].EnemyRadius);

		e_pD3DXMeshModel->UnlockVertexBuffer();

		D3DXCreateSphere(pDevice, enemyWK[i].EnemyRadius, 24, 24, &e_pD3DXMeshSphere, NULL);

		enemyWK[i].m_Radius = D3DXVECTOR3(enemyWK[i].EnemyRadius, 0.0f, 0.0f);
	}


	return S_OK;
}



//=============================================================================
// 描画処理
//=============================================================================
void DrawEnemy_A(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();


	D3DXMATERIAL *pD3DXMat_E;
	D3DMATERIAL9 matDef_E;		//マテリアル構造体のポインタ(D3DMATERIAL9構造体)

	
	pEnemyModel0 = GetPlayer();

	for (int i = 0; i < ENEMY_A_WAVE_1; i++)
	{
		
		if (enemyWK[i].m_bUse)
		{

			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&enemyWK[i].m_mtxWorld);

			// スケールを反映
			D3DXMatrixScaling(&enemyWK[i].m_mtxScl, enemyWK[i].m_sclModel.x, enemyWK[i].m_sclModel.y, enemyWK[i].m_sclModel.z);
			D3DXMatrixMultiply(&enemyWK[i].m_mtxWorld, &enemyWK[i].m_mtxWorld, &enemyWK[i].m_mtxScl);

			// 回転を反映
			D3DXMatrixRotationYawPitchRoll(&enemyWK[i].m_mtxRot, enemyWK[i].m_rotModel.y, enemyWK[i].m_rotModel.x, enemyWK[i].m_rotModel.z);
			CalcLookAtMatrixAxisFix(
				&enemyWK[i].m_mtxRot,
				&enemyWK[i].m_posModel,
				&pEnemyModel0->m_posModel,
				&D3DXVECTOR3(0.0f, 1.0f, 0.0f)
			);
			D3DXMatrixMultiply(&enemyWK[i].m_mtxWorld, &enemyWK[i].m_mtxWorld, &enemyWK[i].m_mtxRot);

			
			// 移動を反映
			D3DXMatrixTranslation(&enemyWK[i].m_mtxTranslate, enemyWK[i].m_posModel.x, enemyWK[i].m_posModel.y, enemyWK[i].m_posModel.z);
			D3DXMatrixMultiply(&enemyWK[i].m_mtxWorld, &enemyWK[i].m_mtxWorld, &enemyWK[i].m_mtxTranslate);

			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &enemyWK[i].m_mtxWorld);

			// 現在のマテリアルを取得
			pDevice->GetMaterial(&matDef_E);		//現在セットされているマテリアルを取得して保存しておく。

			// マテリアル情報に対するポインタを取得
			pD3DXMat_E = (D3DXMATERIAL*)e_pD3DXBuffMatModel->GetBufferPointer();		//Xファイルから取得したマテリアル構造体配列の先頭のポインタを取得する。
			for (int nCntMat = 0; nCntMat < (int)e_nNumMatModel; nCntMat++)		//取得したマテリアルの数だけループ
			{
				// マテリアルの設定　マテリアルをDirectXへセット
				pDevice->SetMaterial(&pD3DXMat_E[nCntMat].MatD3D);		//構造体配列のマテリアルデータをデバイスへセット

				// テクスチャの設定
				pDevice->SetTexture(0, e_pD3DTextureModel);

				// メッシュの描画
				e_pD3DXMeshModel->DrawSubset(nCntMat);
			}
			pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

			pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	
			// マテリアルをデフォルトに戻す
			pDevice->SetMaterial(&matDef_E);
		}	
		
	}
	
	
	
	for (int i = ENEMY_A_WAVE_1; i < ENEMY_A_WAVE_2; i++)
	{
		
		if (enemyWK[i].m_bUse)
		{

			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&enemyWK[i].m_mtxWorld);

			// スケールを反映
			D3DXMatrixScaling(&enemyWK[i].m_mtxScl, enemyWK[i].m_sclModel.x, enemyWK[i].m_sclModel.y, enemyWK[i].m_sclModel.z);
			D3DXMatrixMultiply(&enemyWK[i].m_mtxWorld, &enemyWK[i].m_mtxWorld, &enemyWK[i].m_mtxScl);

			// 回転を反映
			D3DXMatrixRotationYawPitchRoll(&enemyWK[i].m_mtxRot, enemyWK[i].m_rotModel.y, enemyWK[i].m_rotModel.x, enemyWK[i].m_rotModel.z);
			CalcLookAtMatrixAxisFix(
				&enemyWK[i].m_mtxRot,
				&enemyWK[i].m_posModel,
				&pEnemyModel0->m_posModel,
				&D3DXVECTOR3(0.0f, 1.0f, 0.0f));
			D3DXMatrixMultiply(&enemyWK[i].m_mtxWorld, &enemyWK[i].m_mtxWorld, &enemyWK[i].m_mtxRot);

			// 移動を反映
			D3DXMatrixTranslation(&enemyWK[i].m_mtxTranslate, enemyWK[i].m_posModel.x, enemyWK[i].m_posModel.y, enemyWK[i].m_posModel.z);
			D3DXMatrixMultiply(&enemyWK[i].m_mtxWorld, &enemyWK[i].m_mtxWorld, &enemyWK[i].m_mtxTranslate);

			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &enemyWK[i].m_mtxWorld);

			// 現在のマテリアルを取得
			pDevice->GetMaterial(&matDef_E);		//現在セットされているマテリアルを取得して保存しておく。

			// マテリアル情報に対するポインタを取得
			pD3DXMat_E = (D3DXMATERIAL*)e_pD3DXBuffMatModel->GetBufferPointer();		//Xファイルから取得したマテリアル構造体配列の先頭のポインタを取得する。
			for (int nCntMat = 0; nCntMat < (int)e_nNumMatModel; nCntMat++)		//取得したマテリアルの数だけループ
			{
				// マテリアルの設定　マテリアルをDirectXへセット
				pDevice->SetMaterial(&pD3DXMat_E[nCntMat].MatD3D);		//構造体配列のマテリアルデータをデバイスへセット

				// テクスチャの設定
				pDevice->SetTexture(0, e_pD3DTextureModel);

				// メッシュの描画
				e_pD3DXMeshModel->DrawSubset(nCntMat);
			}
			pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

			
			pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

			// マテリアルをデフォルトに戻す
			pDevice->SetMaterial(&matDef_E);

		}
	}

}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateEnemy_A(void)
{
	pEnemyModel0 = GetPlayer();
	
	//WAVE1　
	//エネミーの発生
	if (pEnemyModel0->m_posModel.z >= 100)
	{
		E_wave1 = true;
		if (occurrenceWate1)
		{
			for (int i = 0; i < ENEMY_A_WAVE_1; i++)
			{
				enemyWK[i].m_bUse = true;
			}
			occurrenceWate1 = false;
		}
	}
	//WAVE内のエネミーがすべて倒されたらWAVEをfalseにする。。
	cntWaveEnemy1 = ENEMY_A_WAVE_1;
	for (int i = 0; i < ENEMY_A_WAVE_1; i++)
	{
		if (enemyWK[i].m_bUse == false)
		{
			cntWaveEnemy1--;
		}
		if (cntWaveEnemy1 <= 0)
		{
			E_wave1 = false;
		}
	}
	

	//WAVE1エネミーの攻撃パターン
	for (int i = 0; i < ENEMY_A_WAVE_1; i++)
	{
		distancePlayer[i] = pEnemyModel0->m_posModel - enemyWK[i].m_posModel;		//プレイヤーとの距離
		float area = D3DXVec3Length(&distancePlayer[i]);							//攻撃開始範囲

		//攻撃の方向をエネミーの向いている方向に合わせ、スピードも決める
		enemyBulletSpeed[i].x = enemyWK[i].m_mtxRot._31;
		enemyBulletSpeed[i].y = enemyWK[i].m_mtxRot._32;
		enemyBulletSpeed[i].z = enemyWK[i].m_mtxRot._33;
		enemyBulletSpeed[i] *= -1.0f;

		if (enemyWK[i].m_bUse)
		{
			//出現位置から動かない
			enemyWK[i].m_posModel.z = 100.0f;			

			//攻撃範囲内になったら
			if (area < 200.0f)
			{
				CreateEnemyBullet(enemyWK[i].m_posModel, enemyBulletSpeed[i]);
			}
		}
	}
	
	
	//WAVE2
	//エネミーの発生
	if (pEnemyModel0->m_posModel.z >= 800 && E_wave1 == false)
	{
		E_wave2 = true;
		if (occurrenceWate2)
		{
			for (int i = ENEMY_A_WAVE_1; i < ENEMY_A_WAVE_2; i++)
			{
				enemyWK[i].m_bUse = true;
			}
			occurrenceWate2 = false;
		}
	}

	//WAVE内のエネミーがすべて倒されたらWAVEをfalseにする。。
	cntWaveEnemy2 = ENEMY_A_WAVE_2;
	for (int i = ENEMY_A_WAVE_1; i < ENEMY_A_WAVE_2; i++)
	{
		if (enemyWK[i].m_bUse == false)
		{
			cntWaveEnemy2--;
		}
		if (cntWaveEnemy2 <= ENEMY_A_WAVE_1)
		{
			E_wave2 = false;
		}
	}

	//WAVE2エネミーの行動パターン
	for (int i = ENEMY_A_WAVE_1; i < ENEMY_A_WAVE_2; i++)
	{
		if (enemyWK[i].m_bUse)
		{
			
			enemyWK[i].m_posModel.z = pEnemyModel0->m_posModel.z + 300.0f;
			
			if (enemyWK[i].m_posModel.x > 300.0f || enemyWK[i].m_posModel.x < -300.0f)
			{
				enemyWK[i].m_DirSpeed.x *= -1.0f;
			}
			enemyWK[i].m_posModel.x += enemyWK[i].m_DirSpeed.x;
		}
	}

	//弾との当たり判定処理
	for (int i = 0; i < NUM_ENEMY; i++)
	{
		if (enemyWK[i].m_bUse)
		{

			OBJECT3D* ebullet = GetBullet();
			for (int n = 0; n < 10; n++)
			{	
				if (ebullet[n].m_bUse)
				{
					//チェック関数を呼ぶ
					if (HitOBJECT3D(&ebullet[n], &enemyWK[i]) == TRUE)
					{

						ebullet[n].m_bUse = false;
						enemyWK[i].m_bUse = false;
					}
					
				}
			}
		}
	}
	

	for (int i = 0; i < NUM_ENEMY; i++)
	{
		if (enemyWK[i].m_bUse)
		{
			D3DXVECTOR3 move;
			float fAngle, fLength;
			int nLife;
			float fSize;

			fAngle = (float)(rand() % 628 - 314) / 100.0f;
			fLength = rand() % (int)(5.0f * 20) / 10.0f - 5.0f;			////散らばる範囲（距離）
			move.x = sinf(fAngle) * fLength;
			move.y = rand() % 10 / 10.0f + 5.0f;
			move.z = cosf(fAngle) * fLength;
			nLife = rand() % 90 + 15;
			fSize = 5 + (float)(rand() * (10 - 5 + 1.0) / (1.0 + RAND_MAX));

			SetParticle(enemyWK[i].m_posModel, move, D3DXCOLOR(0.0f, 1.00f, 0.0f, 10.0f), fSize, fSize, nLife);
		}
	}
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitEnemy_A(void)
{
	if (e_pD3DTextureModel != NULL)
	{// テクスチャの開放
		e_pD3DTextureModel->Release();
		e_pD3DTextureModel = NULL;
	}
	
	if (e_pD3DXMeshModel != NULL)
	{// メッシュの開放
		e_pD3DXMeshModel->Release();
		e_pD3DXMeshModel = NULL;
	}
	
	if (e_pD3DXBuffMatModel != NULL)
	{// マテリアルの開放
		e_pD3DXBuffMatModel->Release();
		e_pD3DXBuffMatModel = NULL;
	}
}

int GetEnemyWave2(void)
{
	return E_wave2;
}