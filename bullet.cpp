//=============================================================================
// 弾処理 [bullet.cpp]
// Author : 新里　将士
//
//=============================================================================

#include "bullet.h"
#include "input.h"
#include "model.h"
#include "CameraClass.h"
#include "ReticleClass.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	TEXTURE_BILLBOARD		"data/TEXTURE/effect09.png"	//読み込むテクスチャファイル名
#define	BULLET_SIZE_X		(10.0f)							//ビルボードの幅
#define	BULLET_SIZE_Y		(10.0f)							//ビルボードの高さ
#define MAX_RANGE			(500.0f)						//弾の射程

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexBullet(LPDIRECT3DDEVICE9 pDevice);



//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pD3DTextureBullet = NULL;	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuffBullet = NULL;	// 頂点バッファインターフェースへのポインタ

int						g_nIdxShadowBullet;			// 影ID

OBJECT3D	Bullet[NUM_BULLET];		//弾　構造体情報をmain.hから参照
OBJECT3D	*b_player;

D3DXVECTOR3 bulletFiringPoint[NUM_BULLET];	//弾の発射地点
D3DXVECTOR3 bulletCurrentPoint[NUM_BULLET];	//弾の現在位置
float limitLength[NUM_BULLET];				//弾の進行距離
//============================================================
// 初期化
//==============================================================
void InitBullet(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//BazrelativeVector = D3DXVECTOR3(0.0f, 0.0f, 1.0f);

	// 頂点情報の作成
	MakeVertexBullet(pDevice);

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,						// デバイスへのポインタ
		TEXTURE_BILLBOARD,			// ファイルの名前
		&g_pD3DTextureBullet);	// 読み込むメモリー


	//構造体を未使用にする
	for (int i = 0; i < NUM_BULLET; i++)
	{
		Bullet[i].m_bUse = FALSE;
		Bullet[i].m_Radius = D3DXVECTOR3(5.0f, 5.0f, 5.0f);//あたり判定サイズ
	}
}

//==============================================================
//　弾の描画
//==============================================================
void DrawBullet(D3DXMATRIX cameraView)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxView, mtxScale, mtxTranslate;
	

	// αテストを有効に
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);		//FALSEでOFF
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0x0);

	// αテストを無効に
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);



	// ライティングを無効に
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// ブレンドを表現したい設定になるようにする
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);		// αソースカラーの指定
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);	// αソースカラーの指定
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);		// αデスティネーションカラーの指定
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);//奥行きを考えない

	// ビューマトリックスを取得
	//mtxView = camera->g_mtxView;

	//mtxView = GetMtxView();

	//弾処理
	for (int i = 0; i < NUM_BULLET; i++)
	{	//使用中の構造体のみを未処理
		if (Bullet[i].m_bUse)
		{
			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&Bullet[i].m_mtxWorld);

			// ポリゴンを正面に向けるためカメラの転置行列をコピー
			Bullet[i].m_mtxWorld._11 = cameraView._11;
			Bullet[i].m_mtxWorld._12 = cameraView._21;
			Bullet[i].m_mtxWorld._13 = cameraView._31;
			Bullet[i].m_mtxWorld._21 = cameraView._12;
			Bullet[i].m_mtxWorld._22 = cameraView._22;
			Bullet[i].m_mtxWorld._23 = cameraView._32;
			Bullet[i].m_mtxWorld._31 = cameraView._13;
			Bullet[i].m_mtxWorld._32 = cameraView._23;
			Bullet[i].m_mtxWorld._33 = cameraView._33;

			// 移動を反映
			D3DXMatrixTranslation(&mtxTranslate, Bullet[i].m_posModel.x,
				Bullet[i].m_posModel.y,
				Bullet[i].m_posModel.z);
			D3DXMatrixMultiply(&Bullet[i].m_mtxWorld,
				&Bullet[i].m_mtxWorld, &mtxTranslate);

			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &Bullet[i].m_mtxWorld);

			// 頂点バッファをデバイスのデータストリームにバインド
			pDevice->SetStreamSource(0, g_pD3DVtxBuffBullet, 0, sizeof(VERTEX_3D));

			// 頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			// テクスチャの設定
			pDevice->SetTexture(0, g_pD3DTextureBullet);

			// ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);
		}
	}
	// ラインティングを有効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	// 通常ブレンドに戻す
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// αソースカラーの指定
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	// αデスティネーションカラーの指定
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

}

//============================================================
//　弾の更新
//==============================================================
void UpdateBullet(void)
{
	b_player = GetPlayer2();




	//すべての弾の更新
	for (int i = 0; i < NUM_BULLET; i++)
	{
		if (GetKeyboardPress(DIK_SPACE))
		{
			bulletFiringPoint[i] = Bullet[i].m_posModel;
		}
		//使用中の構造体のみ処理
		if (Bullet[i].m_bUse)
		{

			bulletCurrentPoint[i] = Bullet[i].m_posModel - bulletFiringPoint[i];
			limitLength[i] = D3DXVec3Length(&bulletCurrentPoint[i]);

			//移動
			Bullet[i].m_posModel += Bullet[i].m_DirSpeed;
			//画面外チェック
			if (MAX_RANGE < limitLength[i])
			{
				Bullet[i].m_bUse = FALSE;
			}
		}
	}
}


//==============================================================
//　弾の後始末
//==============================================================
void UninitBullet(void)
{
	if (g_pD3DTextureBullet != NULL)
	{// テクスチャの開放
		g_pD3DTextureBullet->Release();
		g_pD3DTextureBullet = NULL;
	}

	if (g_pD3DVtxBuffBullet != NULL)
	{// 頂点バッファの開放
		g_pD3DVtxBuffBullet->Release();
		g_pD3DVtxBuffBullet = NULL;
	}
}

//==============================================================
//　弾の作成（発射）　球を撃ったオブジェクトが呼び出す
//==============================================================
void CreateBullet(D3DXVECTOR3 pos, D3DXVECTOR3 speed)
{
	//弾を作成
	for (int i = 0; i < NUM_BULLET; i++)
	{
		//未使用の構造体を探して初期化
		if (Bullet[i].m_bUse == FALSE)
		{
			Bullet[i].m_posModel = pos;
			Bullet[i].m_DirSpeed = speed;

			Bullet[i].m_bUse = TRUE;//使用中にする

			break;//作成終了
		}
	}
}

//============================================================
//
//　弾ポリゴンの作成  ポリゴンデータは1つ分のみ作成
//==============================================================
HRESULT MakeVertexBullet(LPDIRECT3DDEVICE9 pDevice)
{
	// オブジェクトの頂点バッファを生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * NUM_VERTEX,	// 頂点データ用に確保するバッファサイズ(バイト単位)
		D3DUSAGE_WRITEONLY,			// 頂点バッファの使用法　
		FVF_VERTEX_3D,				// 使用する頂点フォーマット
		D3DPOOL_MANAGED,			// リソースのバッファを保持するメモリクラスを指定
		&g_pD3DVtxBuffBullet,	// 頂点バッファインターフェースへのポインタ
		NULL)))						// NULLに設定
	{
		return E_FAIL;
	}

	{//頂点バッファの中身を埋める
		VERTEX_3D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pD3DVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

		// 頂点座標の設定
		pVtx[0].vtx = D3DXVECTOR3(-BULLET_SIZE_X / 2, -BULLET_SIZE_Y / 2, 0.0f);
		pVtx[1].vtx = D3DXVECTOR3(-BULLET_SIZE_X / 2, BULLET_SIZE_Y / 2, 0.0f);
		pVtx[2].vtx = D3DXVECTOR3(BULLET_SIZE_X / 2, -BULLET_SIZE_Y / 2, 0.0f);
		pVtx[3].vtx = D3DXVECTOR3(BULLET_SIZE_X / 2, BULLET_SIZE_Y / 2, 0.0f);

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
		g_pD3DVtxBuffBullet->Unlock();
	}

	return S_OK;
}


//==================================
//弾構造体の先頭ポインタを返す
//================================
OBJECT3D* GetBullet()
{
	return &Bullet[0];
}

OBJECT3D* GetParentBullet(void)
{
	return &Bullet[NUM_BULLET];
}
