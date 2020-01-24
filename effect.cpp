//=============================================================================
// エフェクト処理 [effect.cpp]
// Author : 新里　将士
//
//=============================================================================
#include "effect.h"
#include "particle.h"
#include "input.h"
#include "CameraClass.h"
#include "model.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	TEXTURE_EFFECT	"data/TEXTURE/tutibokori.png"	// 読み込むテクスチャファイル名
#define	EFFECT_SIZE_X		(20.0f)							// ビルボードの幅
#define	EFFECT_SIZE_Y		(20.0f)							// ビルボードの高さ
#define	VALUE_MOVE_EFFECT	(2.0f)							// 移動速度

#define	MAX_EFFECT			(1080)						// ビルボード最大数

//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;		// 位置
	D3DXVECTOR3 rot;		// 回転
	D3DXVECTOR3 scale;		// スケール
	D3DXVECTOR3 move;		// 移動量
	D3DXCOLOR col;			// 色
	float fSizeX;			// 幅
	float fSizeY;			// 高さ
	int	nTimer;
	float nDecAlpha;
	int nIdxShadow;			// 影ID
	int nLife;				// 寿命
	bool bUse;				// 使用しているかどうか



} EFFECT;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexEffect(LPDIRECT3DDEVICE9 pDevice);
void SetVertexEffect(int nIdxEffect, float fSizeX, float fSizeY);
void SetColorEffect(int nIdxEffect, D3DXCOLOR col);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pD3DTextureEffect= NULL;	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuffEffect = NULL;	// 頂点バッファインターフェースへのポインタ

D3DXMATRIX				g_mtxWorldEffect;				// ワールドマトリックス

EFFECT					g_aEffect[MAX_EFFECT];		// パーティクルワーク
D3DXVECTOR3				effect_posBase;						// ビルボード発生位置
float					effect_fWidthBase = 5.0f;			// 基準の幅
float					effect_fHeightBase = 5.0f;			// 基準の高さ
bool					effect_bPause = false;				// ポーズON/OFF

bool					effect_EAlpaTest;					// アルファテストON/OFF
int						effect_EnAlpha;						// アルファテストの閾値

//CAMERA					*effectCamera;

D3DXVECTOR3				*PlayerVector0;
D3DXVECTOR3				*PlayerLength0;

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitEffect(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点情報の作成
	MakeVertexEffect(pDevice);

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,						// デバイスへのポインタ
		TEXTURE_EFFECT,			// ファイルの名前
		&g_pD3DTextureEffect);	// 読み込むメモリー

	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		g_aEffect[nCntEffect].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEffect[nCntEffect].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEffect[nCntEffect].scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		g_aEffect[nCntEffect].move = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		g_aEffect[nCntEffect].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		g_aEffect[nCntEffect].fSizeX = EFFECT_SIZE_X;
		g_aEffect[nCntEffect].fSizeY = EFFECT_SIZE_Y;
		g_aEffect[nCntEffect].nIdxShadow = -1;
		g_aEffect[nCntEffect].nLife = 0;
		g_aEffect[nCntEffect].bUse = false;
	}

	effect_posBase = D3DXVECTOR3(0.0f, 0.0f, 500.0f);

	//g_PnAlpha = 0x66;
	effect_EAlpaTest = true;

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitEffect(void)
{
	if (g_pD3DTextureEffect != NULL)
	{// テクスチャの開放
		g_pD3DTextureEffect->Release();
		g_pD3DTextureEffect = NULL;
	}

	if (g_pD3DVtxBuffEffect != NULL)
	{// 頂点バッファの開放
		g_pD3DVtxBuffEffect->Release();
		g_pD3DVtxBuffEffect = NULL;
	}
}


//=============================================================================
// 更新処理
//=============================================================================
void UpdateEffect(void)
{
	//CameraClass camera;
	//D3DXVECTOR3 EffectrotCamera;

	//effectCamera = GetMtxView();

	//戦車本体の向きと長さにつなげる
	PlayerVector0 = GetEffectVector();
	PlayerLength0 = GetEffectLength();


	// カメラの回転を取得
	//EffectrotCamera = effectCamera->relativeVector;

	//エフェクトパーティクルの動き
	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		

		if (g_aEffect[nCntEffect].bUse)
		{

			g_aEffect[nCntEffect].pos.x += g_aEffect[nCntEffect].move.x;
			g_aEffect[nCntEffect].pos.z += g_aEffect[nCntEffect].move.z;

			g_aEffect[nCntEffect].col.a -= g_aEffect[nCntEffect].nDecAlpha;
			if (g_aEffect[nCntEffect].col.a <= 0.0f)
			{
				g_aEffect[nCntEffect].col.a = 0.0f;
			}
			SetColorEffect(nCntEffect,
				D3DXCOLOR(g_aEffect[nCntEffect].col.r, g_aEffect[nCntEffect].col.b,
					g_aEffect[nCntEffect].col.b, g_aEffect[nCntEffect].col.a));

			g_aEffect[nCntEffect].nTimer--;
			if (g_aEffect[nCntEffect].nTimer <= 0)
			{
				g_aEffect[nCntEffect].bUse = false;
			}
		}
	}
	

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawEffect(D3DXMATRIX cameraView)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxView, mtxScale, mtxTranslate;

	//effectCamera = GetMtxView();
	//CameraClass camera;

	// αテストを有効に

	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);		//FALSEでOFF

	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);

	pDevice->SetRenderState(D3DRS_ALPHAREF, 0x0);



	// αテストを無効に
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	// ライティングを無効に
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// パーティクルっぽく設定をする

	pDevice->SetRenderState(D3DRS_POINTSPRITEENABLE, TRUE);

	// ブレンドを表現したい設定になるようにする
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);		// αソースカラーの指定
	//pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);		// αデスティネーションカラーの指定
	//pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);	// αソースカラーの指定
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);	// αソースカラーの指定
	//pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);		// αデスティネーションカラーの指定
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);//奥行きを考えない


	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		if (g_aEffect[nCntEffect].bUse)
		{
			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_mtxWorldEffect);

			// ビューマトリックスを取得
			mtxView = cameraView;

			g_mtxWorldEffect._11 = mtxView._11;
			g_mtxWorldEffect._12 = mtxView._21;
			g_mtxWorldEffect._13 = mtxView._31;
			g_mtxWorldEffect._21 = mtxView._12;
			g_mtxWorldEffect._22 = mtxView._22;
			g_mtxWorldEffect._23 = mtxView._32;
			g_mtxWorldEffect._31 = mtxView._13;
			g_mtxWorldEffect._32 = mtxView._23;
			g_mtxWorldEffect._33 = mtxView._33;

			// スケールを反映
			D3DXMatrixScaling(&mtxScale, g_aEffect[nCntEffect].scale.x, g_aEffect[nCntEffect].scale.y, g_aEffect[nCntEffect].scale.z);
			D3DXMatrixMultiply(&g_mtxWorldEffect, &g_mtxWorldEffect, &mtxScale);

			// 移動を反映
			D3DXMatrixTranslation(&mtxTranslate, g_aEffect[nCntEffect].pos.x, g_aEffect[nCntEffect].pos.y, g_aEffect[nCntEffect].pos.z);
			D3DXMatrixMultiply(&g_mtxWorldEffect, &g_mtxWorldEffect, &mtxTranslate);

			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldEffect);

			// 頂点バッファをデバイスのデータストリームにバインド
			pDevice->SetStreamSource(0, g_pD3DVtxBuffEffect, 0, sizeof(VERTEX_3D));

			// 頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			// テクスチャの設定
			pDevice->SetTexture(0, g_pD3DTextureEffect);

			// ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, (nCntEffect * 4), NUM_POLYGON);
		}
	}

	// ライティングを有効に
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	// 通常ブレンドに戻す
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// αソースカラーの指定
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	// αデスティネーションカラーの指定
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
}

//=============================================================================
// 頂点情報の作成
//=============================================================================
HRESULT MakeVertexEffect(LPDIRECT3DDEVICE9 pDevice)
{
	// オブジェクトの頂点バッファを生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * NUM_VERTEX * MAX_EFFECT,	// 頂点データ用に確保するバッファサイズ(バイト単位)
		D3DUSAGE_WRITEONLY,							// 頂点バッファの使用法　
		FVF_VERTEX_3D,								// 使用する頂点フォーマット
		D3DPOOL_MANAGED,							// リソースのバッファを保持するメモリクラスを指定
		&g_pD3DVtxBuffEffect,					// 頂点バッファインターフェースへのポインタ
		NULL)))										// NULLに設定
	{
		return E_FAIL;
	}

	{//頂点バッファの中身を埋める
		VERTEX_3D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pD3DVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

		for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++, pVtx += 4)
		{
			// 頂点座標の設定
			pVtx[0].vtx = D3DXVECTOR3(-EFFECT_SIZE_X / 2, -EFFECT_SIZE_Y / 2, 0.0f);
			pVtx[1].vtx = D3DXVECTOR3(EFFECT_SIZE_X / 2, -EFFECT_SIZE_Y / 2, 0.0f);
			pVtx[2].vtx = D3DXVECTOR3(-EFFECT_SIZE_X / 2, EFFECT_SIZE_Y / 2, 0.0f);
			pVtx[3].vtx = D3DXVECTOR3(EFFECT_SIZE_X / 2, EFFECT_SIZE_Y / 2, 0.0f);

			// 法線の設定
			pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
			pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
			pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
			pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

			// 反射光の設定
			pVtx[0].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[1].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[2].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			// テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
		}

		// 頂点データをアンロックする
		g_pD3DVtxBuffEffect->Unlock();
	}

	return S_OK;
}

//=============================================================================
// 頂点座標の設定
//=============================================================================
void SetVertexEffect(int nIdxEffect, float fSizeX, float fSizeY)
{
	{//頂点バッファの中身を埋める
		VERTEX_3D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pD3DVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (nIdxEffect * 4);

		// 頂点座標の設定
		pVtx[0].vtx = D3DXVECTOR3(-fSizeX / 2, -fSizeY / 2, 0.0f);
		pVtx[1].vtx = D3DXVECTOR3(-fSizeX / 2, fSizeY / 2, 0.0f);
		pVtx[2].vtx = D3DXVECTOR3(fSizeX / 2, -fSizeY / 2, 0.0f);
		pVtx[3].vtx = D3DXVECTOR3(fSizeX / 2, fSizeY / 2, 0.0f);

		// 頂点データをアンロックする
		g_pD3DVtxBuffEffect->Unlock();
	}
}

//=============================================================================
// 頂点カラーの設定
//=============================================================================
void SetColorEffect(int nIdxEffect, D3DXCOLOR col)
{
	{//頂点バッファの中身を埋める
		VERTEX_3D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pD3DVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (nIdxEffect * 4);

		// 頂点座標の設定
		pVtx[0].diffuse = col;
		pVtx[1].diffuse = col;
		pVtx[2].diffuse = col;
		pVtx[3].diffuse = col;

		// 頂点データをアンロックする
		g_pD3DVtxBuffEffect->Unlock();
	}
}

//=============================================================================
// 頂点情報の作成
//=============================================================================
int SetEffect(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, float fSizeX, float fSizeY, int nTimer)
{
	int nIdxEffect = -1;

	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		if (!g_aEffect[nCntEffect].bUse)
		{
			g_aEffect[nCntEffect].pos = pos;
			g_aEffect[nCntEffect].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_aEffect[nCntEffect].scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
			g_aEffect[nCntEffect].move = move;
			g_aEffect[nCntEffect].col = col;
			g_aEffect[nCntEffect].fSizeX = fSizeX;
			g_aEffect[nCntEffect].fSizeY = fSizeY;
			g_aEffect[nCntEffect].nTimer = nTimer;
			g_aEffect[nCntEffect].nDecAlpha = col.a / nTimer;
			//g_aEffect[nCntEffect].nLife = nLife;
			g_aEffect[nCntEffect].bUse = true;

			// 頂点座標の設定
			SetVertexEffect(nCntEffect, fSizeX, fSizeY);

			// 頂点カラーの設定
			SetColorEffect(nCntEffect,
				D3DXCOLOR(g_aEffect[nCntEffect].col.r, g_aEffect[nCntEffect].col.b,
					g_aEffect[nCntEffect].col.b, g_aEffect[nCntEffect].col.a));

			nIdxEffect = nCntEffect;

			break;
		}
	}

	return nIdxEffect;
}