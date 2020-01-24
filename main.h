#pragma once
//=============================================================================
// メイン処理 [main.h]
// Author : 新里　将士
//
//=============================================================================
#ifndef _MAIN_H_
#define _MAIN_H_

//*****************************************************************************
// インクルードファイル
//*****************************************************************************
#include <windows.h>
#include <stdio.h>
#include "d3dx9.h"

#define DIRECTINPUT_VERSION (0x0800)	// 警告対策
#include "dinput.h"
#include "xaudio2.h"		// XAudio2関連のヘッダー

//*****************************************************************************
// ライブラリのリンク
//*****************************************************************************
#if 1	// [ここを"0"にした場合、"構成プロパティ" -> "リンカ" -> "入力" -> "追加の依存ファイル"に対象ライブラリを設定する]
#pragma comment (lib, "d3d9.lib")
#pragma comment (lib, "d3dx9.lib")
#pragma comment (lib, "dxguid.lib")
#pragma comment (lib, "dinput8.lib")
#pragma comment (lib, "winmm.lib")
#endif

//*****************************************************************************
// マクロ定義
//*****************************************************************************
// ２Ｄポリゴン頂点フォーマット( 頂点座標[2D] / 反射光 / テクスチャ座標 )
#define	FVF_VERTEX_2D	(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)
// ３Ｄポリゴン頂点フォーマット( 頂点座標[3D] / 法線 / 反射光 / テクスチャ座標 )
#define	FVF_VERTEX_3D	(D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1)

#define SCREEN_WIDTH	(1280)				// ウインドウの幅
#define SCREEN_HEIGHT	(960)				// ウインドウの高さ
#define SCREEN_CENTER_X	(SCREEN_WIDTH / 2)	// ウインドウの中心Ｘ座標
#define SCREEN_CENTER_Y	(SCREEN_HEIGHT / 2)	// ウインドウの中心Ｙ座標

#define	NUM_VERTEX		(16)		// 頂点数
#define	NUM_POLYGON		(8)		// ポリゴン数

#define PI (3.141592654f)

// 上記２Ｄポリゴン頂点フォーマットに合わせた構造体を定義
typedef struct
{
	D3DXVECTOR3 vtx;		// 頂点座標
	float rhw;				// テクスチャのパースペクティブコレクト用
	D3DCOLOR diffuse;		// 反射光
	D3DXVECTOR2 tex;		// テクスチャ座標
} VERTEX_2D;

// 上記３Ｄポリゴン頂点フォーマットに合わせた構造体を定義
typedef struct
{
	D3DXVECTOR3 vtx;		// 頂点座標
	D3DXVECTOR3 nor;		// 法線ベクトル
	D3DCOLOR diffuse;		// 反射光
	D3DXVECTOR2 tex;		// テクスチャ座標
} VERTEX_3D;

//**************************************************************************
// モデル構造体
//**************************************************************************
typedef struct
{
	//ポリゴンXファイルモデル1つ分の情報
	LPDIRECT3DTEXTURE9	m_pD3DTextureModel;		// テクスチャへ情報のポインタ
	LPD3DXMESH			m_pD3DXMeshModel;		// メッシュ情報へのポインタ
	LPD3DXBUFFER		m_pD3DXBuffMatModel;	// マテリアル情報へのポインタ
	DWORD				m_nNumMatModel;			// マテリアル情報の数

	//D3DXMATRIX			m_mtxScl, m_mtxRot, m_mtxTranslate;

}MODEL3D;

//*************************************
// モードの種類
//*************************************
typedef enum
{
	MODE_TITLE = 0,			// タイトル画面
	MODE_GAME,				// ゲーム画面
	MODE_RESULT,			// リザルト画面
	MODE_MAX
} MODE;

//**************************************************************************
// キャラクタ構造体
//**************************************************************************
typedef struct
{

	//キャラクタ1つ分の情報
	int                 m_bUse;                 // この構造体が使用中か(TRUE:使用中)
	D3DXVECTOR3			m_posModel;				// モデルの位置
	D3DXVECTOR3			m_rotModel;				// モデルの向き(回転)
	D3DXVECTOR3			m_sclModel;				// モデルの大きさ(スケール)
	D3DXMATRIX			m_mtxWorld;				// ワールドマトリックス

	D3DXMATRIX*			m_mtxParent = NULL;			// 親ワールドマトリックス
	D3DXMATRIX*			m_mtxRotParent = NULL;		// 親ワールドマトリックス[回転]
	D3DXMATRIX			m_virtualMtxRot;			// モデルの向き(回転)

	D3DXVECTOR3			m_DirSpeed;				//移動ベクトル
	D3DXVECTOR3			m_RotSpeed;				//回転スピード
	D3DXVECTOR3			m_Kakudo;				//回転移動用角度
	D3DXVECTOR3			m_Radius;				//回転移動用半径

	D3DXVECTOR3			EnemyCenter;
	D3DXVECTOR3			bulletLength;

	D3DXMATRIX			m_mtxScl, m_mtxRot, m_mtxTranslate;

	int					m_ModelID;				// このキャラクタのモデルデータのID
	int					m_ShadowID;				// このキャラクタの影オブジェクトのID

	float				EnemyRadius;

}OBJECT3D;



//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
LPDIRECT3DDEVICE9 GetDevice(void);
float GetFrameTime(void);
void SetMode(MODE mode);
MODE GetMode(void);
void DrawGameTime(float a);

//当たり判定（球VS球）
BOOL	HitOBJECT3D(OBJECT3D *obj1, OBJECT3D *obj2);
//注視関数
D3DXMATRIX* CalcLookAtMatrixAxisFix(
	D3DXMATRIX* pout,
	D3DXVECTOR3* pPos,
	D3DXVECTOR3* pLook,
	D3DXVECTOR3* pUp);
// キャラクタ姿勢行列算出関数
D3DXMATRIX* CalcLookAtMatrix(
	D3DXMATRIX* pout,
	D3DXVECTOR3* pPos,
	D3DXVECTOR3* pLook,
	D3DXVECTOR3* pUp);


#endif