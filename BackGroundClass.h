//=============================================================================
// 背景処理 [BG.h]
// Author : 新里　将士
//
//=============================================================================
#pragma once

#include "main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	MODEL_BG		"data/MODEL/skyhigh.x"							// 読み込むモデル名
#define TEXTURE_BG_FILENAME "data/TEXTURE/bluesky2.jpg"

//*****************************************************************************
// クラス宣言
//*****************************************************************************
class BackGroundClass
{
private:
	LPDIRECT3DTEXTURE9	g_pD3DTextureBG;		// テクスチャへのポインタ
	LPD3DXMESH			g_pD3DXMeshBG;		// メッシュ情報へのポインタ　読み込んだXファイルモデルを表す。モデルを増やしたかったらこれを増やす。
	LPD3DXBUFFER		g_pD3DXBuffMatBG;	// マテリアル情報へのポインタ　読み込んだXファイルモデルのマテリアルを表す。
	DWORD				g_nNumMatBG;			// マテリアル情報の数　読み込んだXファイルモデルのマテリアルの数。

	D3DXVECTOR3			g_posBG;				// モデルの位置
	D3DXVECTOR3			g_rotBG;				// モデルの向き(回転)
	D3DXVECTOR3			g_sclBG;				// モデルの大きさ(スケール)



	D3DXMATRIX			g_mtxViewBG;				// ビューマトリックス
	D3DXMATRIX			g_mtxProjectionBG;		// プロジェクションマトリックス
	D3DXMATRIX			g_mtxWorldBG;				// ワールドマトリックス


	D3DXMATRIX mtxSclBG, mtxRotBG, mtxTranslateBG;


	OBJECT3D	*pBgModel0;
public:
	BackGroundClass();
	~BackGroundClass();

	HRESULT InitBG(void);
	void UninitBG(void);
	void UpdateBG(void);
	void DrawBG(void);
};

