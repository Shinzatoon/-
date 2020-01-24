//=============================================================================
// パーティクル処理 [particle.cpp]
// Author : 新里　将士
//
//=============================================================================
#include "particle.h"
#include "input.h"
#include "CameraClass.h"


//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	TEXTURE_PARTICLE	"data/TEXTURE/EffectNew.png"	// 読み込むテクスチャファイル名
#define	PARTICLE_SIZE_X		(20.0f)							// ビルボードの幅
#define	PARTICLE_SIZE_Y		(20.0f)							// ビルボードの高さ
#define	VALUE_MOVE_PARTICLE	(2.0f)							// 移動速度

#define	MAX_PARTICLE			(1080)						// ビルボード最大数

#define	DISP_SHADOW				// 影の表示
//#undef DISP_SHADOW

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
	int nIdxShadow;			// 影ID
	int nLife;				// 寿命
	bool bUse;				// 使用しているかどうか

	

} PARTICLE;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexParticle(LPDIRECT3DDEVICE9 pDevice);
void SetVertexParticle(int nIdxParticle, float fSizeX, float fSizeY);
void SetColorParticle(int nIdxParticle, D3DXCOLOR col);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pD3DTextureParticle = NULL;	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuffParticle = NULL;	// 頂点バッファインターフェースへのポインタ

D3DXMATRIX				g_mtxWorldParticle;				// ワールドマトリックス

PARTICLE				g_aParticle[MAX_PARTICLE];		// パーティクルワーク
D3DXVECTOR3				g_posBase;						// ビルボード発生位置
float					g_fWidthBase = 5.0f;			// 基準の幅
float					g_fHeightBase = 5.0f;			// 基準の高さ
bool					g_bPause = false;				// ポーズON/OFF

bool					g_PAlpaTest;					// アルファテストON/OFF
int						g_PnAlpha;						// アルファテストの閾値

//CAMERA					*PcameraWK;

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitParticle(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点情報の作成
	MakeVertexParticle(pDevice);

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,						// デバイスへのポインタ
		TEXTURE_PARTICLE,			// ファイルの名前
		&g_pD3DTextureParticle);	// 読み込むメモリー

	for (int nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++)
	{
		g_aParticle[nCntParticle].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aParticle[nCntParticle].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aParticle[nCntParticle].scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		g_aParticle[nCntParticle].move = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		g_aParticle[nCntParticle].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		g_aParticle[nCntParticle].fSizeX = PARTICLE_SIZE_X;
		g_aParticle[nCntParticle].fSizeY = PARTICLE_SIZE_Y;
		g_aParticle[nCntParticle].nIdxShadow = -1;
		g_aParticle[nCntParticle].nLife = 0;
		g_aParticle[nCntParticle].bUse = false;
	}

	g_posBase = D3DXVECTOR3(0.0f, 0.0f, 500.0f);

	//g_PnAlpha = 0x66;
	g_PAlpaTest = true;

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitParticle(void)
{
	if (g_pD3DTextureParticle != NULL)
	{// テクスチャの開放
		g_pD3DTextureParticle->Release();
		g_pD3DTextureParticle = NULL;
	}

	if (g_pD3DVtxBuffParticle != NULL)
	{// 頂点バッファの開放
		g_pD3DVtxBuffParticle->Release();
		g_pD3DVtxBuffParticle = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateParticle(void)
{	
	 
	//D3DXVECTOR3 rotCamera;

	//PcameraWK = GetMtxView();
	 
	//CameraClass camera;

	// カメラの回転を取得
	//rotCamera = PcameraWK->relativeVector;
	 
	//if (GetKeyboardPress(DIK_LEFT))
	//{
	//	if (GetKeyboardPress(DIK_UP))
	//	{// 左前移動
	//		g_posBase.x -= sinf(rotCamera.y + D3DX_PI * 0.75f) * VALUE_MOVE_PARTICLE;
	//		g_posBase.z -= cosf(rotCamera.y + D3DX_PI * 0.75f) * VALUE_MOVE_PARTICLE;
	//	}
	//	else if (GetKeyboardPress(DIK_DOWN))
	//	{// 左後移動
	//		g_posBase.x -= sinf(rotCamera.y + D3DX_PI * 0.25f) * VALUE_MOVE_PARTICLE;
	//		g_posBase.z -= cosf(rotCamera.y + D3DX_PI * 0.25f) * VALUE_MOVE_PARTICLE;
	//	}
	//	else
	//	{// 左移動
	//		g_posBase.x -= sinf(rotCamera.y + D3DX_PI * 0.50f) * VALUE_MOVE_PARTICLE;
	//		g_posBase.z -= cosf(rotCamera.y + D3DX_PI * 0.50f) * VALUE_MOVE_PARTICLE;
	//	}
	//}
	//else if (GetKeyboardPress(DIK_RIGHT))
	//{
	//	if (GetKeyboardPress(DIK_UP))
	//	{// 右前移動
	//		g_posBase.x -= sinf(rotCamera.y - D3DX_PI * 0.75f) * VALUE_MOVE_PARTICLE;
	//		g_posBase.z -= cosf(rotCamera.y - D3DX_PI * 0.75f) * VALUE_MOVE_PARTICLE;
	//	}
	//	else if (GetKeyboardPress(DIK_DOWN))
	//	{// 右後移動
	//		g_posBase.x -= sinf(rotCamera.y - D3DX_PI * 0.25f) * VALUE_MOVE_PARTICLE;
	//		g_posBase.z -= cosf(rotCamera.y - D3DX_PI * 0.25f) * VALUE_MOVE_PARTICLE;
	//	}
	//	else
	//	{// 右移動
	//		g_posBase.x -= sinf(rotCamera.y - D3DX_PI * 0.50f) * VALUE_MOVE_PARTICLE;
	//		g_posBase.z -= cosf(rotCamera.y - D3DX_PI * 0.50f) * VALUE_MOVE_PARTICLE;
	//	}
	//}
	//else if (GetKeyboardPress(DIK_UP))
	//{// 前移動
	//	g_posBase.x -= sinf(D3DX_PI + rotCamera.y) * VALUE_MOVE_PARTICLE;
	//	g_posBase.z -= cosf(D3DX_PI + rotCamera.y) * VALUE_MOVE_PARTICLE;
	//}
	//else if (GetKeyboardPress(DIK_DOWN))
	//{// 後移動
	//	g_posBase.x -= sinf(rotCamera.y) * VALUE_MOVE_PARTICLE;
	//	g_posBase.z -= cosf(rotCamera.y) * VALUE_MOVE_PARTICLE;
	//}
	 
	if (GetKeyboardPress(DIK_1))
	{
		g_fWidthBase -= 0.1f;
		if (g_fWidthBase < 2.0f)
		{
			g_fWidthBase = 2.0f;
		}
	}
	if (GetKeyboardPress(DIK_2))
	{
		g_fWidthBase += 0.1f;
		if (g_fWidthBase > 10.0f)
		{
			g_fWidthBase = 10.0f;
		}
	}
	if (GetKeyboardPress(DIK_3))
	{
		g_fHeightBase -= 0.1f;
		if (g_fHeightBase < 5.0f)
		{
			g_fHeightBase = 5.0f;
		}
	}
	if (GetKeyboardPress(DIK_4))
	{
		g_fHeightBase += 0.1f;
		if (g_fHeightBase > 15.0f)
		{
			g_fHeightBase = 15.0f;
		}
	}
	
	if (GetKeyboardTrigger(DIK_P))
	{
		g_bPause = g_bPause ? false : true;
	}
	 
	if (g_bPause == false)
	{
		for (int nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++)
		{
			if (g_aParticle[nCntParticle].bUse)
			{// 使用中
				g_aParticle[nCntParticle].pos.x += g_aParticle[nCntParticle].move.x;
				g_aParticle[nCntParticle].pos.z += g_aParticle[nCntParticle].move.z;

				g_aParticle[nCntParticle].pos.y += g_aParticle[nCntParticle].move.y;
				if (g_aParticle[nCntParticle].pos.y <= g_aParticle[nCntParticle].fSizeY / 2)
				{// 着地した
					g_aParticle[nCntParticle].pos.y = g_aParticle[nCntParticle].fSizeY / 2;
					g_aParticle[nCntParticle].move.y = -g_aParticle[nCntParticle].move.y * 0.75f;
				}

				g_aParticle[nCntParticle].move.x += (0.0f - g_aParticle[nCntParticle].move.x) * 0.15f;
				g_aParticle[nCntParticle].move.y -= 1.25f;
				g_aParticle[nCntParticle].move.z += (0.0f - g_aParticle[nCntParticle].move.z) * 0.15f;


				g_aParticle[nCntParticle].nLife--;
				if (g_aParticle[nCntParticle].nLife <= 0)
				{
					g_aParticle[nCntParticle].bUse = false;
					//DeleteShadow(g_aParticle[nCntParticle].nIdxShadow);
					//g_aParticle[nCntParticle].nIdxShadow = -1;
				}
				else
				{
					/*if (g_aParticle[nCntParticle].nLife <= 80)
					{
						g_aParticle[nCntParticle].col.r = 0.00f - (float)(80 - g_aParticle[nCntParticle].nLife) / 8.0f * 0.06f + 0.2f;
						g_aParticle[nCntParticle].col.g = 1.0f - (float)(80 - g_aParticle[nCntParticle].nLife) / 8.0f * 0.07f;
						g_aParticle[nCntParticle].col.b = 0.00f;
					}*/

					if (g_aParticle[nCntParticle].nLife <= 80)
					{
						//g_aParticle[nCntParticle].col.r = 0.0f; //- (float)(80 - g_aParticle[nCntParticle].nLife) / 8.0f * 0.06f + 0.2f;
						//g_aParticle[nCntParticle].col.g = 0.0f; //- (float)(80 - g_aParticle[nCntParticle].nLife) / 8.0f * 0.07f;
						//g_aParticle[nCntParticle].col.b = 0.0f;
						//g_aParticle[nCntParticle].col.a = 0.5f;
					}

					if (g_aParticle[nCntParticle].nLife <= 20)
					{
						// α値設定
						/*g_aParticle[nCntParticle].col.a -= 0.05f;
						if (g_aParticle[nCntParticle].col.a < 0.0f)
						{
							g_aParticle[nCntParticle].col.a = 0.0f;
						}*/
					}

					// 色の設定
					SetColorParticle(nCntParticle, g_aParticle[nCntParticle].col);
				}
			}
		}

		// パーティクル発生
		if ((rand() % 2) == 0)
		{
			D3DXVECTOR3 pos;
			D3DXVECTOR3 move;
			float fAngle, fLength;
			int nLife;
			float fSize;

			pos = g_posBase;

			//fAngle = rand() % 2;		//散らばる範囲（角度）
			fAngle = (float)(rand() % 628 - 314) / 100.0f;
			fLength = rand() % (int)(g_fWidthBase * 20) / 10.0f - g_fWidthBase;			////散らばる範囲（距離）
			move.x = sinf(fAngle) * fLength;
			move.y = rand() % 10 / 10.0f + g_fHeightBase;
			move.z = cosf(fAngle) * fLength;

			nLife = rand() % 90 + 15;

			//最小値 + (int)(rand() * (最大値 - 最小値 + 1.0) / (1.0 + RAND_MAX))
			fSize = 5 + (float)(rand() * (10 - 5 + 1.0) / (1.0 + RAND_MAX));
			//fSize = (float)(rand() % 50 + 50);


			pos.y = fSize / 2;

			// ビルボードの設定
			//SetParticle(pos, move, D3DXCOLOR(0.0f, 1.00f, 0.0f, 1.0f), fSize, fSize, nLife);
		}



	}
			
	 
			
	 
#ifdef DISP_SHADOW
				//if (g_aParticle[nCntParticle].nIdxShadow != -1)
				//{// 影使用中
				//	float colA;
				//
				//	// 影の位置設定
				//	SetPositionShadow(g_aParticle[nCntParticle].nIdxShadow, D3DXVECTOR3(g_aParticle[nCntParticle].pos.x, 0.1f, g_aParticle[nCntParticle].pos.z));
				//
				//	// 影のα値設定
				//	if (g_aParticle[nCntParticle].col.a > 0.0f)
				//	{
				//		colA = (300.0f - (g_aParticle[nCntParticle].pos.y - 9.0f)) / (300.0f / g_aParticle[nCntParticle].col.a);
				//		if (colA < 0.0f)
				//		{
				//			colA = 0.0f;
				//		}
				//	}
				//	else
				//	{
				//		colA = 0.0f;
				//	}
				//
				//	// 影の色の設定
				//	SetColorShadow(g_aParticle[nCntParticle].nIdxShadow, D3DXCOLOR(0.15f, 0.15f, 0.15f, colA));
				//}
#endif
			
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawParticle(D3DXMATRIX cameraView)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxView, mtxScale, mtxTranslate;

	//PcameraWK = GetMtxView();
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
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);		// αデスティネーションカラーの指定
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);//奥行きを考えない




	for (int nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++)
	{
		if (g_aParticle[nCntParticle].bUse)
		{
			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_mtxWorldParticle);

			// ビューマトリックスを取得
			mtxView = cameraView;

			g_mtxWorldParticle._11 = cameraView._11;
			g_mtxWorldParticle._12 = cameraView._21;
			g_mtxWorldParticle._13 = cameraView._31;
			g_mtxWorldParticle._21 = cameraView._12;
			g_mtxWorldParticle._22 = cameraView._22;
			g_mtxWorldParticle._23 = cameraView._32;
			g_mtxWorldParticle._31 = cameraView._13;
			g_mtxWorldParticle._32 = cameraView._23;
			g_mtxWorldParticle._33 = cameraView._33;

			// スケールを反映
			D3DXMatrixScaling(&mtxScale, g_aParticle[nCntParticle].scale.x, g_aParticle[nCntParticle].scale.y, g_aParticle[nCntParticle].scale.z);
			D3DXMatrixMultiply(&g_mtxWorldParticle, &g_mtxWorldParticle, &mtxScale);

			// 移動を反映
			D3DXMatrixTranslation(&mtxTranslate, g_aParticle[nCntParticle].pos.x, g_aParticle[nCntParticle].pos.y, g_aParticle[nCntParticle].pos.z);
			D3DXMatrixMultiply(&g_mtxWorldParticle, &g_mtxWorldParticle, &mtxTranslate);

			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldParticle);

			// 頂点バッファをデバイスのデータストリームにバインド
			pDevice->SetStreamSource(0, g_pD3DVtxBuffParticle, 0, sizeof(VERTEX_3D));

			// 頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			// テクスチャの設定
			pDevice->SetTexture(0, g_pD3DTextureParticle);

			// ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, (nCntParticle * 4), NUM_POLYGON);
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
HRESULT MakeVertexParticle(LPDIRECT3DDEVICE9 pDevice)
{
	// オブジェクトの頂点バッファを生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * NUM_VERTEX * MAX_PARTICLE,	// 頂点データ用に確保するバッファサイズ(バイト単位)
		D3DUSAGE_WRITEONLY,							// 頂点バッファの使用法　
		FVF_VERTEX_3D,								// 使用する頂点フォーマット
		D3DPOOL_MANAGED,							// リソースのバッファを保持するメモリクラスを指定
		&g_pD3DVtxBuffParticle,					// 頂点バッファインターフェースへのポインタ
		NULL)))										// NULLに設定
	{
		return E_FAIL;
	}

	{//頂点バッファの中身を埋める
		VERTEX_3D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pD3DVtxBuffParticle->Lock(0, 0, (void**)&pVtx, 0);

		for (int nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++, pVtx += 4)
		{
			// 頂点座標の設定
			pVtx[0].vtx = D3DXVECTOR3(-PARTICLE_SIZE_X / 2, -PARTICLE_SIZE_Y / 2, 0.0f);
			pVtx[1].vtx = D3DXVECTOR3(PARTICLE_SIZE_X / 2, -PARTICLE_SIZE_Y / 2, 0.0f);
			pVtx[2].vtx = D3DXVECTOR3(-PARTICLE_SIZE_X / 2, PARTICLE_SIZE_Y / 2, 0.0f);
			pVtx[3].vtx = D3DXVECTOR3(PARTICLE_SIZE_X / 2, PARTICLE_SIZE_Y / 2, 0.0f);

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
		g_pD3DVtxBuffParticle->Unlock();
	}

	return S_OK;
}

//=============================================================================
// 頂点座標の設定
//=============================================================================
void SetVertexParticle(int nIdxParticle, float fSizeX, float fSizeY)
{
	{//頂点バッファの中身を埋める
		VERTEX_3D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pD3DVtxBuffParticle->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (nIdxParticle * 4);

		// 頂点座標の設定
		pVtx[0].vtx = D3DXVECTOR3(-fSizeX / 2, -fSizeY / 2, 0.0f);
		pVtx[1].vtx = D3DXVECTOR3(-fSizeX / 2, fSizeY / 2, 0.0f);
		pVtx[2].vtx = D3DXVECTOR3(fSizeX / 2, -fSizeY / 2, 0.0f);
		pVtx[3].vtx = D3DXVECTOR3(fSizeX / 2, fSizeY / 2, 0.0f);

		// 頂点データをアンロックする
		g_pD3DVtxBuffParticle->Unlock();
	}
}

//=============================================================================
// 頂点カラーの設定
//=============================================================================
void SetColorParticle(int nIdxParticle, D3DXCOLOR col)
{
	{//頂点バッファの中身を埋める
		VERTEX_3D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pD3DVtxBuffParticle->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (nIdxParticle * 4);

		// 頂点座標の設定
			pVtx[0].diffuse = col;
			pVtx[1].diffuse = col;
			pVtx[2].diffuse = col;
			pVtx[3].diffuse = col;

		// 頂点データをアンロックする
		g_pD3DVtxBuffParticle->Unlock();
	}
}

//=============================================================================
// 頂点情報の作成
//=============================================================================
int SetParticle(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, float fSizeX, float fSizeY, int nLife)
{
	int nIdxParticle = -1;

	for (int nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++)
	{
		if (!g_aParticle[nCntParticle].bUse)
		{
			g_aParticle[nCntParticle].pos = pos;
			g_aParticle[nCntParticle].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_aParticle[nCntParticle].scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
			g_aParticle[nCntParticle].move = move;
			g_aParticle[nCntParticle].col = col;
			g_aParticle[nCntParticle].fSizeX = fSizeX;
			g_aParticle[nCntParticle].fSizeY = fSizeY;
			g_aParticle[nCntParticle].nLife = nLife;
			g_aParticle[nCntParticle].bUse = true;

			// 頂点座標の設定
			SetVertexParticle(nCntParticle, fSizeX, fSizeY);

			nIdxParticle = nCntParticle;

#ifdef DISP_SHADOW
			//// 影の設定
			//g_aParticle[nCntParticle].nIdxShadow = SetShadow(D3DXVECTOR3(pos.x, 0.1f, pos.z), fSizeX, fSizeY);		// 影の設定
			//if (g_aParticle[nCntParticle].nIdxShadow != -1)
			//{
			//	SetColorShadow(g_aParticle[nCntParticle].nIdxShadow, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f));
			//}
#endif

			break;
		}
	}

	return nIdxParticle;
}
