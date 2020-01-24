//=============================================================================
// 壁処理 [wall.h]
// Author :  新里　将士
//
//=============================================================================
#pragma once
#include "main.h"

//******************************************************
//マクロ定義
//******************************************************
#define SCROOL_SPEED (0.009f)
#define MODEL_WALL		"data/MODEL/side.x"
#define TEXTURE_WALLFILE "data/TEXTURE/side_arrow.png"
#define RIGHT_WALL		(20)
#define LEFT_WALL		(20)
#define NUM_WALL		(RIGHT_WALL+LEFT_WALL)				//壁の枚数
#define LIMIT_WALL_Z	(NUM_WALL/2 * 200.0f)

//*****************************************************************************
// クラス宣言
//*****************************************************************************
class WallClass
{
private:
	LPDIRECT3DTEXTURE9	w_pD3DTextureModel;		// テクスチャへのポインタ
	LPD3DXMESH			w_pD3DXMeshSphere;
	LPD3DXMESH			w_pD3DXMeshModel;		// メッシュ情報へのポインタ　読み込んだXファイルモデルを表す。モデルを増やしたかったらこれを増やす。
	LPD3DXBUFFER		w_pD3DXBuffMatModel;	// マテリアル情報へのポインタ　読み込んだXファイルモデルのマテリアルを表す。
	DWORD				w_nNumMatModel;			// マテリアル情報の数　読み込んだXファイルモデルのマテリアルの数。

	OBJECT3D			WallWK[NUM_WALL];

	OBJECT3D			*pPlayerWall;

	float					StractdistanceWall = 0;			//PlayerとWallのZ距離減算結果の格納変数
public:
	WallClass();
	~WallClass();

	HRESULT InitWall(void);
	void UninitWall(void);
	void UpdateWall(void);
	void DrawWall(void);
};
