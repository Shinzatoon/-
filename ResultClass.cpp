//=============================================================================
// リザルト画面処理 [result.cpp]
// Author : 新里　将士
//
//=============================================================================

//*****************************************************************************
// インクルード
//*****************************************************************************
#include "ResultClass.h"

#include "input.h"
//#include "fade.h"

#include "FadeClass.h"

ResultClass::ResultClass()
{
}


ResultClass::~ResultClass()
{
}

//
//FadeClass fadeResult;

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT ResultClass::InitResult(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//FadeClass* fade = getFadeMain();

	g_nCountAppearResult = 0;
	g_fAlphaResult = 0.0f;
	g_nCountWaitResult = 0;

	// 頂点情報の作成
	MakeVertexResult(pDevice);

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,						// デバイスへのポインタ
		TEXTURE_RESULT,				// ファイルの名前
		&g_pD3DTextureResult);		// 読み込むメモリー

// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,						// デバイスへのポインタ
		TEXTURE_RESULT_LOGO,		// ファイルの名前
		&g_pD3DTextureResultLogo);	// 読み込むメモリー

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void ResultClass::UninitResult(void)
{
	if (g_pD3DTextureResult != NULL)
	{// テクスチャの開放
		g_pD3DTextureResult->Release();
		g_pD3DTextureResult = NULL;
	}

	if (g_pD3DVtxBuffResult != NULL)
	{// 頂点バッファの開放
		g_pD3DVtxBuffResult->Release();
		g_pD3DVtxBuffResult = NULL;
	}

	if (g_pD3DTextureResultLogo != NULL)
	{// テクスチャの開放
		g_pD3DTextureResultLogo->Release();
		g_pD3DTextureResultLogo = NULL;
	}

	if (g_pD3DVtxBuffResultLogo != NULL)
	{// 頂点バッファの開放
		g_pD3DVtxBuffResultLogo->Release();
		g_pD3DVtxBuffResultLogo = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void ResultClass::UpdateResult(void)
{
	FadeClass* fade = getFadeMain();
	g_nCountAppearResult++;
	if (g_nCountAppearResult >= COUNT_APPERA_RESULT)
	{
		if (g_fAlphaResult < 1.0f)
		{
			g_fAlphaResult += 0.05f;
			if (g_fAlphaResult >= 1.0f)
			{
				g_fAlphaResult = 1.0f;
			}

			SetColorResultLogo();
		}
	}

	if (GetKeyboardTrigger(DIK_RETURN))
	{// Enter押したら、フェードアウトしてモードを切り替えいく
		fade->SetFade(FADE::FADE_OUT);
	}

	g_nCountWaitResult++;
	if (g_nCountWaitResult > LIMIT_COUNT_WAIT)
	{
		fade->SetFade(FADE::FADE_OUT);
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void ResultClass::DrawResult(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, g_pD3DVtxBuffResult, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, g_pD3DTextureResult);

	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);

	// 頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, g_pD3DVtxBuffResultLogo, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, g_pD3DTextureResultLogo);

	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);
}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT ResultClass::MakeVertexResult(LPDIRECT3DDEVICE9 pDevice)
{
	// オブジェクトの頂点バッファを生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX,	// 頂点データ用に確保するバッファサイズ(バイト単位)
		D3DUSAGE_WRITEONLY,			// 頂点バッファの使用法　
		FVF_VERTEX_2D,				// 使用する頂点フォーマット
		D3DPOOL_MANAGED,			// リソースのバッファを保持するメモリクラスを指定
		&g_pD3DVtxBuffResult,		// 頂点バッファインターフェースへのポインタ
		NULL)))						// NULLに設定
	{
		return E_FAIL;
	}

	{//頂点バッファの中身を埋める
		VERTEX_2D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pD3DVtxBuffResult->Lock(0, 0, (void**)&pVtx, 0);

		// 頂点座標の設定
		pVtx[0].vtx = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[1].vtx = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
		pVtx[2].vtx = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
		pVtx[3].vtx = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

		// テクスチャのパースペクティブコレクト用
		pVtx[0].rhw =
			pVtx[1].rhw =
			pVtx[2].rhw =
			pVtx[3].rhw = 1.0f;

		// 反射光の設定
		pVtx[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

		// テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		// 頂点データをアンロックする
		g_pD3DVtxBuffResult->Unlock();
	}

	// オブジェクトの頂点バッファを生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX,	// 頂点データ用に確保するバッファサイズ(バイト単位)
		D3DUSAGE_WRITEONLY,			// 頂点バッファの使用法　
		FVF_VERTEX_2D,				// 使用する頂点フォーマット
		D3DPOOL_MANAGED,			// リソースのバッファを保持するメモリクラスを指定
		&g_pD3DVtxBuffResultLogo,	// 頂点バッファインターフェースへのポインタ
		NULL)))						// NULLに設定
	{
		return E_FAIL;
	}

	{//頂点バッファの中身を埋める
		VERTEX_2D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pD3DVtxBuffResultLogo->Lock(0, 0, (void**)&pVtx, 0);

		// 頂点座標の設定
		pVtx[0].vtx = D3DXVECTOR3(RESULT_LOGO_POS_X, RESULT_LOGO_POS_Y, 0.0f);
		pVtx[1].vtx = D3DXVECTOR3(RESULT_LOGO_POS_X + RESULT_LOGO_WIDTH, RESULT_LOGO_POS_Y, 0.0f);
		pVtx[2].vtx = D3DXVECTOR3(RESULT_LOGO_POS_X, RESULT_LOGO_POS_Y + RESULT_LOGO_HEIGHT, 0.0f);
		pVtx[3].vtx = D3DXVECTOR3(RESULT_LOGO_POS_X + RESULT_LOGO_WIDTH, RESULT_LOGO_POS_Y + RESULT_LOGO_HEIGHT, 0.0f);

		// テクスチャのパースペクティブコレクト用
		pVtx[0].rhw =
			pVtx[1].rhw =
			pVtx[2].rhw =
			pVtx[3].rhw = 1.0f;

		// 反射光の設定
		pVtx[0].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_fAlphaResult);
		pVtx[1].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_fAlphaResult);
		pVtx[2].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_fAlphaResult);
		pVtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_fAlphaResult);

		// テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		// 頂点データをアンロックする
		g_pD3DVtxBuffResultLogo->Unlock();
	}

	return S_OK;
}

//=============================================================================
// 頂点カラーの設定
//=============================================================================
void ResultClass::SetColorResultLogo(void)
{
	{//頂点バッファの中身を埋める
		VERTEX_2D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pD3DVtxBuffResultLogo->Lock(0, 0, (void**)&pVtx, 0);

		// 反射光の設定
		pVtx[0].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_fAlphaResult);
		pVtx[1].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_fAlphaResult);
		pVtx[2].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_fAlphaResult);
		pVtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_fAlphaResult);

		// 頂点データをアンロックする
		g_pD3DVtxBuffResultLogo->Unlock();
	}
}