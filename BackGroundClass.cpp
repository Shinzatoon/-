#include "BackGroundClass.h"
#include "model.h"
#include "model.h"


BackGroundClass::BackGroundClass()
{
}


BackGroundClass::~BackGroundClass()
{
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT BackGroundClass::InitBG(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();




	// 位置・回転・スケールの初期設定
	g_posBG = D3DXVECTOR3(0.0f, 180.0f, 0.0f);
	g_rotBG = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_sclBG = D3DXVECTOR3(10.0f, 10.0f, 10.0f);

	g_pD3DTextureBG = NULL;
	g_pD3DXMeshBG = NULL;
	g_pD3DXBuffMatBG = NULL;
	g_nNumMatBG = 0;

	// Xファイルの読み込み
	if (FAILED(D3DXLoadMeshFromX(MODEL_BG,				// 読み込むモデルファイル名(Xファイル)　ここを書き換えれば他のも表示できる。
		D3DXMESH_SYSTEMMEM,		// メッシュの作成オプションを指定
		pDevice,				// IDirect3DDevice9インターフェイスへのポインタ
		NULL,					// 隣接性データを含むバッファへのポインタ
		&g_pD3DXBuffMatBG,		// マテリアルデータを含むバッファへのポインタ	マテリアル管理　	読み込んだXファイルモデルの情報がそれぞれ格納されている
		NULL,					// エフェクトインスタンスの配列を含むバッファへのポインタ							
		&g_nNumMatBG,			// D3DXMATERIAL構造体の数															//
		&g_pD3DXMeshBG)))		// ID3DXMeshインターフェイスへのポインタのアドレス									//
	{
		return E_FAIL;
	}

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,					// デバイスへのポインタ
		TEXTURE_BG_FILENAME,	// ファイルの名前
		&g_pD3DTextureBG);		// 読み込むメモリ
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
void BackGroundClass::UninitBG(void)
{
	if (g_pD3DTextureBG != NULL)
	{// テクスチャの開放
		g_pD3DTextureBG->Release();
		g_pD3DTextureBG = NULL;
	}

	if (g_pD3DXMeshBG != NULL)
	{// メッシュの開放
		g_pD3DXMeshBG->Release();
		g_pD3DXMeshBG = NULL;
	}

	if (g_pD3DXBuffMatBG != NULL)
	{// マテリアルの開放
		g_pD3DXBuffMatBG->Release();
		g_pD3DXBuffMatBG = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void BackGroundClass::UpdateBG(void)
{

}

//=============================================================================
// 描画処理
//=============================================================================
void BackGroundClass::DrawBG(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	D3DXMATERIAL *pD3DXMat_BG;
	D3DMATERIAL9 matDef_BG;		//マテリアル構造体のポインタ(D3DMATERIAL9構造体)

	pBgModel0 = GetPlayer();


	// プロジェクションマトリックスの設定
	//	pDevice->SetTransform(D3DTS_PROJECTION, &g_mtxProjectionBG);

		//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&g_mtxWorldBG);

	//スケールを反映
	D3DXMatrixScaling(&mtxSclBG, g_sclBG.x, g_sclBG.y, g_sclBG.z);
	D3DXMatrixMultiply(&g_mtxWorldBG, &g_mtxWorldBG, &mtxSclBG);

	//回転を反映
	D3DXMatrixRotationYawPitchRoll(&mtxRotBG, g_rotBG.y, g_rotBG.x, g_rotBG.z);
	D3DXMatrixMultiply(&g_mtxWorldBG, &g_mtxWorldBG, &mtxRotBG);

	//移動を反映
	//モデル２の移動とのペアレント処理
	D3DXMatrixTranslation(&mtxTranslateBG, pBgModel0->m_posModel.x, pBgModel0->m_posModel.y, pBgModel0->m_posModel.z);
	D3DXMatrixMultiply(&g_mtxWorldBG, &g_mtxWorldBG, &mtxTranslateBG);


	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldBG);

	// ラインティングを無効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// 現在のマテリアルを取得
	pDevice->GetMaterial(&matDef_BG);		//現在セットされているマテリアルを取得して保存しておく。

	// マテリアル情報に対するポインタを取得
	pD3DXMat_BG = (D3DXMATERIAL*)g_pD3DXBuffMatBG->GetBufferPointer();		//Xファイルから取得したマテリアル構造体配列の先頭のポインタを取得する。


	for (int nCntMat = 0; nCntMat < (int)g_nNumMatBG; nCntMat++)		//取得したマテリアルの数だけループ
	{
		// マテリアルの設定　マテリアルをDirectXへセット
		pDevice->SetMaterial(&pD3DXMat_BG[nCntMat].MatD3D);		//構造体配列のマテリアルデータをデバイスへセット

		// テクスチャの設定
		pDevice->SetTexture(0, g_pD3DTextureBG);

		// メッシュの描画
		g_pD3DXMeshBG->DrawSubset(nCntMat);
	}

	// マテリアルをデフォルトに戻す
	pDevice->SetMaterial(&matDef_BG);

	// ラインティングを有効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}