//=============================================================================
// インクルード
//=============================================================================
#include "WallClass.h"
#include "model.h"

//=============================================================================
// コンストラクタ
//=============================================================================
WallClass::WallClass()
{

}

//=============================================================================
// デストラクタ
//=============================================================================
WallClass::~WallClass()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT WallClass::InitWall(void) 
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();


	w_pD3DTextureModel = NULL;
	w_pD3DXMeshModel = NULL;
	w_pD3DXBuffMatModel = NULL;
	w_nNumMatModel = 0;

	// Xファイルの読み込み
	if (FAILED(D3DXLoadMeshFromX(MODEL_WALL,				// 読み込むモデルファイル名(Xファイル)　ここを書き換えれば他のも表示できる。
		D3DXMESH_SYSTEMMEM,		// メッシュの作成オプションを指定
		pDevice,				// IDirect3DDevice9インターフェイスへのポインタ
		NULL,					// 隣接性データを含むバッファへのポインタ
		&w_pD3DXBuffMatModel,	// マテリアルデータを含むバッファへのポインタ	マテリアル管理　	読み込んだXファイルモデルの情報がそれぞれ格納されている
		NULL,					// エフェクトインスタンスの配列を含むバッファへのポインタ							
		&w_nNumMatModel,		// D3DXMATERIAL構造体の数															//
		&w_pD3DXMeshModel)))	// ID3DXMeshインターフェイスへのポインタのアドレス									//
	{
		return E_FAIL;
	}

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,					// デバイスへのポインタ
		TEXTURE_WALLFILE,		// ファイルの名前
		&w_pD3DTextureModel);	// 読み込むメモリ

	for (int i = 0; i < RIGHT_WALL; i++)
	{
		// 位置・回転・スケールの初期設定
		WallWK[i].m_bUse = FALSE;				//エネミー構造体の使用フラグをOFFにする
		WallWK[i].m_posModel = D3DXVECTOR3(-500.0f, 50.0f, 200.0f*i);
		WallWK[i].m_rotModel = D3DXVECTOR3(0.0f, -PI / 2, PI / 2);
		WallWK[i].m_sclModel = D3DXVECTOR3(50.0f, 200.0f, 1.0f);
		WallWK[i].m_DirSpeed = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	}

	for (int i = RIGHT_WALL; i < NUM_WALL; i++)
	{
		// 位置・回転・スケールの初期設定
		WallWK[i].m_bUse = FALSE;				//エネミー構造体の使用フラグをOFFにする
		WallWK[i].m_posModel = D3DXVECTOR3(500.0f, 50.0f, 200.0f*(i - RIGHT_WALL));
		WallWK[i].m_rotModel = D3DXVECTOR3(0.0f, PI / 2, -PI / 2);
		WallWK[i].m_sclModel = D3DXVECTOR3(50.0f, 200.0f, 1.0f);
		WallWK[i].m_DirSpeed = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	}

	//DWORD VertexNum = e_pD3DXMeshModel->GetNumVertices();
	//DWORD VertexSize = e_pD3DXMeshModel->GetNumBytesPerVertex();	//１つの頂点のサイズ
	//BYTE *p;
	//
	//HRESULT Bhr = e_pD3DXMeshModel->LockVertexBuffer(D3DLOCK_READONLY, (LPVOID*)&p);
	//
	//D3DXComputeBoundingSphere((D3DXVECTOR3*)p, VertexNum, VertexSize,
	//	&enemyWK[i].EnemyCenter, &enemyWK[i].EnemyRadius);
	//
	//e_pD3DXMeshModel->UnlockVertexBuffer();
	//
	//D3DXCreateSphere(pDevice, enemyWK[i].EnemyRadius, 24, 24, &e_pD3DXMeshSphere, NULL);
	//
	//enemyWK[i].m_Radius = D3DXVECTOR3(enemyWK[i].EnemyRadius, 0.0f, 0.0f);

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void WallClass::UninitWall(void) 
{
	if (w_pD3DTextureModel != NULL)
	{// テクスチャの開放
		w_pD3DTextureModel->Release();
		w_pD3DTextureModel = NULL;
	}

	if (w_pD3DXMeshModel != NULL)
	{// メッシュの開放
		w_pD3DXMeshModel->Release();
		w_pD3DXMeshModel = NULL;
	}

	if (w_pD3DXBuffMatModel != NULL)
	{// マテリアルの開放
		w_pD3DXBuffMatModel->Release();
		w_pD3DXBuffMatModel = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void WallClass::UpdateWall(void) 
{
	pPlayerWall = GetPlayer();

	for (int i = 0; i < NUM_WALL; i++)
	{

		StractdistanceWall = WallWK[i].m_posModel.z - pPlayerWall->m_posModel.z;


		if (StractdistanceWall < -LIMIT_WALL_Z / 2)
		{
			WallWK[i].m_posModel.z += LIMIT_WALL_Z;
		}

		if (StractdistanceWall > LIMIT_WALL_Z / 2)
		{
			WallWK[i].m_posModel.z -= LIMIT_WALL_Z;
		}

		StractdistanceWall = 0;

	}
}

//=============================================================================
// 描画処理
//=============================================================================
void WallClass::DrawWall(void) 
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();


	D3DXMATERIAL *pD3DXMat_W;
	D3DMATERIAL9 matDef_W;		//マテリアル構造体のポインタ(D3DMATERIAL9構造体)

	// ラインティングを無効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);


	for (int i = 0; i < NUM_WALL; i++)
	{

		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&WallWK[i].m_mtxWorld);

		// スケールを反映
		D3DXMatrixScaling(&WallWK[i].m_mtxScl, WallWK[i].m_sclModel.x, WallWK[i].m_sclModel.y, WallWK[i].m_sclModel.z);
		D3DXMatrixMultiply(&WallWK[i].m_mtxWorld, &WallWK[i].m_mtxWorld, &WallWK[i].m_mtxScl);

		// 回転を反映
		D3DXMatrixRotationYawPitchRoll(&WallWK[i].m_mtxRot, WallWK[i].m_rotModel.y, WallWK[i].m_rotModel.x, WallWK[i].m_rotModel.z);
		D3DXMatrixMultiply(&WallWK[i].m_mtxWorld, &WallWK[i].m_mtxWorld, &WallWK[i].m_mtxRot);

		// 移動を反映
		D3DXMatrixTranslation(&WallWK[i].m_mtxTranslate, WallWK[i].m_posModel.x, WallWK[i].m_posModel.y, WallWK[i].m_posModel.z);
		D3DXMatrixMultiply(&WallWK[i].m_mtxWorld, &WallWK[i].m_mtxWorld, &WallWK[i].m_mtxTranslate);

		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &WallWK[i].m_mtxWorld);

		// 現在のマテリアルを取得
		pDevice->GetMaterial(&matDef_W);		//現在セットされているマテリアルを取得して保存しておく。

		// マテリアル情報に対するポインタを取得
		pD3DXMat_W = (D3DXMATERIAL*)w_pD3DXBuffMatModel->GetBufferPointer();		//Xファイルから取得したマテリアル構造体配列の先頭のポインタを取得する。
		for (int nCntMat = 0; nCntMat < (int)w_nNumMatModel; nCntMat++)		//取得したマテリアルの数だけループ
		{
			// マテリアルの設定　マテリアルをDirectXへセット
			pDevice->SetMaterial(&pD3DXMat_W[nCntMat].MatD3D);		//構造体配列のマテリアルデータをデバイスへセット

			// テクスチャの設定
			pDevice->SetTexture(0, w_pD3DTextureModel);

			// メッシュの描画
			w_pD3DXMeshModel->DrawSubset(nCntMat);
		}
		pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

		//e_pD3DXMeshSphere->DrawSubset(0);
		pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
		//}

		// マテリアルをデフォルトに戻す
		pDevice->SetMaterial(&matDef_W);


	}
	// ラインティングを有効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}
