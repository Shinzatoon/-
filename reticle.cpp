//============================================================================
// 照準処理 [reticle.cpp]
// Author : 新里　将士
//
//=============================================================================

#include "reticle.h"
#include "input.h"
#include "model.h"
#include "CameraClass.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	TEXTURE_RETICLE		"data/TEXTURE/ターゲットのアイコン.png"	// 読み込むテクスチャファイル名
#define	RETICLE_SIZE_X		(20.0f)							// ビルボードの幅
#define	RETICLE_SIZE_Y		(20.0f)							// ビルボードの高さ

#define MAX_RETICLE			(1)								//ビルボードの最大数


//*****************************************************************************
// 構造体定義
//*****************************************************************************



//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexReticle(LPDIRECT3DDEVICE9 pDevice);
void SetVertexReticle(int nIdxReticle, float fSizeX, float fSizeY);
void SetColorReticle(int nIdxReticle, D3DXCOLOR col);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pD3DTextureReticle = NULL;	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuffReticle = NULL;	// 頂点バッファインターフェースへのポインタ


RETICLE		g_aReticle;				// レティクルワーク

OBJECT3D	*pModel2;								

//CAMERA		*ReticleCamera;							//カメラの情報
OBJECT3D	*ParentModel2;							//モデル２の情報　ペアレント処理

D3DXVECTOR3		*PlayerVector;
D3DXVECTOR3		*PlayerLength;


//============================================================
// 初期化
//==============================================================
HRESULT InitReticle()
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	ParentModel2 = GetPlayer2();
	pModel2 = GetPlayer2();

	// 頂点情報の作成
	MakeVertexReticle(pDevice);
	
	//CameraClass camera;


	//Reticle.m_mtxWorld = Reticle.m_mtxWorld * ParentModel2->m_mtxWorld;
	//Reticle.m_posModel = D3DXVECTOR3(0.0f, 50.0f, 0.0f);

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,						// デバイスへのポインタ
		TEXTURE_RETICLE,			// ファイルの名前
		&g_pD3DTextureReticle);	// 読み込むメモリー


		g_aReticle.pos = pModel2->m_posModel;	//ここにmodel２のpos
		g_aReticle.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aReticle.scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		g_aReticle.move = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		g_aReticle.col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		g_aReticle.fSizeX = RETICLE_SIZE_X;
		g_aReticle.fSizeY = RETICLE_SIZE_Y;
		g_aReticle.nIdxShadow = -1;
		g_aReticle.nLife = 0;
		g_aReticle.bUse = true;
		g_aReticle.posBase = D3DXVECTOR3(0.0f, 0.0f, 0.0f);





	return S_OK;


	//構造体を未使用にする
	//for (int i = 0; i < NUM_BULLET; i++)
	//{
	//	Bullet[i].m_bUse = FALSE;
	//	Bullet[i].m_Radius = D3DXVECTOR3(10.0f, 10.0f, 10.0f);//あたり判定サイズ
	//}



}



//==============================================================
//　レティクルの描画
//==============================================================
void DrawReticle(D3DXMATRIX cameraView)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxView, mtxScale, mtxTranslate;

	D3DXVECTOR3 temp;

	//ReticleCamera = GetMtxView();
	//CameraClass camera;
	

	ParentModel2 = GetPlayer2();

	// αテストを有効に

	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);		//FALSEでOFF

	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);

	pDevice->SetRenderState(D3DRS_ALPHAREF, 0x0);



	// αテストを無効に
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);



	// ライティングを無効に
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// ブレンドを表現したい設定になるようにする
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);		// αソースカラーの指定
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);	// αソースカラーの指定
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);		// αデスティネーションカラーの指定
	pDevice->SetRenderState(D3DRS_ZENABLE, FALSE);//奥行きを考えない

	//g_aReticle.g_mtxProvisional = g_aReticle.g_mtxWorldReticle;
	
		if (g_aReticle.bUse)
		{
			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aReticle.g_mtxWorldReticle);

			// ビューマトリックスを取得
			mtxView = cameraView;

			// ポリゴンを正面に向けるためカメラの転置行列をコピー
			g_aReticle.g_mtxWorldReticle._11 = mtxView._11;
			g_aReticle.g_mtxWorldReticle._12 = mtxView._21;
			g_aReticle.g_mtxWorldReticle._13 = mtxView._31;
			g_aReticle.g_mtxWorldReticle._21 = mtxView._12;
			g_aReticle.g_mtxWorldReticle._22 = mtxView._22;
			g_aReticle.g_mtxWorldReticle._23 = mtxView._32;
			g_aReticle.g_mtxWorldReticle._31 = mtxView._13;
			g_aReticle.g_mtxWorldReticle._32 = mtxView._23;
			g_aReticle.g_mtxWorldReticle._33 = mtxView._33;

			// スケールを反映
			D3DXMatrixScaling(&mtxScale, g_aReticle.scale.x, g_aReticle.scale.y, g_aReticle.scale.z);
			D3DXMatrixMultiply(&g_aReticle.g_mtxWorldReticle, &g_aReticle.g_mtxWorldReticle, &mtxScale);

			// 移動を反映
			D3DXMatrixTranslation(&mtxTranslate, g_aReticle.pos.x, g_aReticle.pos.y, g_aReticle.pos.z);
			D3DXMatrixMultiply(&g_aReticle.g_mtxWorldReticle, &g_aReticle.g_mtxWorldReticle, &mtxTranslate);


			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aReticle.g_mtxWorldReticle);

			// 頂点バッファをデバイスのデータストリームにバインド
			pDevice->SetStreamSource(0, g_pD3DVtxBuffReticle, 0, sizeof(VERTEX_3D));

			// 頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			// テクスチャの設定
			pDevice->SetTexture(0, g_pD3DTextureReticle);

			// ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);

		}

	
	// ラインティングを有効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	// 通常ブレンドに戻す
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// αソースカラーの指定
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	// αデスティネーションカラーの指定
	pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);

	

}

//============================================================
//　レティクルの更新
//==============================================================
void UpdateReticle()
{
	
		PlayerVector = GetReticleVector();		//レティクルの方向を砲台とリンクする
		PlayerLength = GetReticleLength();		//レティクルの場所を砲台とリンクする
	

		g_aReticle.rot.x = PlayerVector->x;
		g_aReticle.rot.y = PlayerVector->y;
		g_aReticle.rot.z = PlayerVector->z;
		
		g_aReticle.pos.x = PlayerLength->x;
		g_aReticle.pos.y = PlayerLength->y;
		g_aReticle.pos.z = PlayerLength->z;
		g_aReticle.pos = g_aReticle.pos + g_aReticle.rot * 300.0f;
	
}

//==============================================================
//　レティクルの後始末
//==============================================================
void UninitReticle()
{
	if (g_pD3DTextureReticle != NULL)
	{// テクスチャの開放
		g_pD3DTextureReticle->Release();
		g_pD3DTextureReticle = NULL;
	}

	if (g_pD3DVtxBuffReticle != NULL)
	{// 頂点バッファの開放
		g_pD3DVtxBuffReticle->Release();
		g_pD3DVtxBuffReticle = NULL;
	}
}

//====================================================
//頂点情報の作成
//====================================================
HRESULT MakeVertexReticle(LPDIRECT3DDEVICE9 pDevice)
{
	// オブジェクトの頂点バッファを生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * NUM_VERTEX,	// 頂点データ用に確保するバッファサイズ(バイト単位)
		D3DUSAGE_WRITEONLY,			// 頂点バッファの使用法　
		FVF_VERTEX_3D,				// 使用する頂点フォーマット
		D3DPOOL_MANAGED,			// リソースのバッファを保持するメモリクラスを指定
		&g_pD3DVtxBuffReticle,	// 頂点バッファインターフェースへのポインタ
		NULL)))						// NULLに設定
	{
		return E_FAIL;
	}

	{//頂点バッファの中身を埋める
		VERTEX_3D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pD3DVtxBuffReticle->Lock(0, 0, (void**)&pVtx, 0);

		// 頂点座標の設定
		pVtx[0].vtx = D3DXVECTOR3(-RETICLE_SIZE_X / 2, -RETICLE_SIZE_Y / 2, 0.0f);
		pVtx[1].vtx = D3DXVECTOR3(-RETICLE_SIZE_X / 2, RETICLE_SIZE_Y / 2, 0.0f);
		pVtx[2].vtx = D3DXVECTOR3(RETICLE_SIZE_X / 2, -RETICLE_SIZE_Y / 2, 0.0f);
		pVtx[3].vtx = D3DXVECTOR3(RETICLE_SIZE_X / 2, RETICLE_SIZE_Y / 2, 0.0f);

		// 法線の設定
		pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, 1.0f);

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

		// 頂点データをアンロックする
		g_pD3DVtxBuffReticle->Unlock();
	}

	return S_OK;
}

//=============================================================================
// 頂点座標の設定
//=============================================================================
void SetVertexReticle(int nIdxReticle, float fSizeX, float fSizeY)
{
	{//頂点バッファの中身を埋める
		VERTEX_3D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pD3DVtxBuffReticle->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (nIdxReticle * 4);

		// 頂点座標の設定
		pVtx[0].vtx = D3DXVECTOR3(-fSizeX / 2, -fSizeY / 2, 0.0f);
		pVtx[1].vtx = D3DXVECTOR3(-fSizeX / 2, fSizeY / 2, 0.0f);
		pVtx[2].vtx = D3DXVECTOR3(fSizeX / 2, -fSizeY / 2, 0.0f);
		pVtx[3].vtx = D3DXVECTOR3(fSizeX / 2, fSizeY / 2, 0.0f);

		// 頂点データをアンロックする
		g_pD3DVtxBuffReticle->Unlock();
	}
}


//=============================================================================
// 頂点カラーの設定
//=============================================================================
void SetColorReticle(int nIdxReticle, D3DXCOLOR col)
{
	{//頂点バッファの中身を埋める
		VERTEX_3D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pD3DVtxBuffReticle->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (nIdxReticle * 4);

		// 頂点座標の設定
		pVtx[0].diffuse =
			pVtx[1].diffuse =
			pVtx[2].diffuse =
			pVtx[3].diffuse = col;

		// 頂点データをアンロックする
		g_pD3DVtxBuffReticle->Unlock();
	}
}

//=============================================================================
// 頂点情報の作成
//=============================================================================
int SetReticle(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col, float fSizeX, float fSizeY, int nLife)
{
	int nIdxReticle = -1;

	for (int nCntReticle = 0; nCntReticle < MAX_RETICLE; nCntReticle++)
	{	//未使用の構造体へセット
		if (!g_aReticle.bUse)
		{
			g_aReticle.pos = pos;
			g_aReticle.rot = rot;
			g_aReticle.scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
			g_aReticle.move = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
			g_aReticle.col = col;
			g_aReticle.fSizeX = fSizeX;
			g_aReticle.fSizeY = fSizeY;
			g_aReticle.nLife = nLife;
			g_aReticle.bUse = true;

			// 頂点座標の設定
			SetVertexReticle(nCntReticle, fSizeX, fSizeY);
			SetColorReticle(nCntReticle, col);

			nIdxReticle = nCntReticle;

#ifdef DISP_SHADOW
			// 影の設定	//光物の場合は影はいらない
//			g_aParticle[nCntParticle].nIdxShadow = SetShadow(D3DXVECTOR3(pos.x, 0.1f, pos.z), fSizeX, fSizeY);		// 影の設定
//			if(g_aParticle[nCntParticle].nIdxShadow != -1)
//			{
//				SetColorShadow(g_aParticle[nCntParticle].nIdxShadow, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f));
//			}
#endif

			break;
		}
	}

	return nIdxReticle;
}


RETICLE *GetReticle(void)
{
	return(&g_aReticle);
}