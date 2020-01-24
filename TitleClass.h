//=============================================================================
// タイトル画面処理 [title.h]
// Author :  新里　将士
//
//=============================================================================
#pragma once

#include "main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	TEXTURE_TITLE		"data/TEXTURE/titleBg.jpg"		// 読み込むテクスチャファイル名
#define	TEXTURE_TITLE_LOGO	"data/TEXTURE/title.png"	// 読み込むテクスチャファイル名
#define	TEXTURE_LOGO_START	"data/TEXTURE/press_enter.png"	// 読み込むテクスチャファイル名

#define	TITLE_LOGO_POS_X		(200)		// タイトルロゴの位置(X座標)
#define	TITLE_LOGO_POS_Y		(40)		// タイトルロゴの位置(Y座標)
#define	TITLE_LOGO_WIDTH		(900)		// タイトルロゴの幅
#define	TITLE_LOGO_HEIGHT		(440)		// タイトルロゴの高さ

#define	START_POS_X				(400)		// スタートボタンの位置(X座標)
#define	START_POS_Y				(720)		// スタートボタンの位置(Y座標)
#define	START_WIDTH				(480)		// スタートボタンの幅
#define	START_HEIGHT			(120)		// スタートボタンの高さ

#define	COUNT_APPERA_START		(60)		// スタートボタン出現までの時間
#define	INTERVAL_DISP_START		(60)		// スタートボタン点滅の時間

#define	COUNT_WAIT_DEMO			(60 * 5)	// デモまでの待ち時間


//*****************************************************************************
// クラス宣言
//*****************************************************************************
class TitleClass
{
private:
	LPDIRECT3DTEXTURE9		g_pD3DTextureTitle = NULL;		// テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuffTitle = NULL;		// 頂点バッファインターフェースへのポインタ
	LPDIRECT3DTEXTURE9		g_pD3DTextureTitleLogo = NULL;	// テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuffTitleLogo = NULL;	// 頂点バッファインターフェースへのポインタ
	LPDIRECT3DTEXTURE9		g_pD3DTextureStart = NULL;		// テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuffStart = NULL;		// 頂点バッファインターフェースへのポインタ
	int						g_nCountAppearStart = 0;		//
	float					g_fAlphaLogo = 0.0f;			//
	int						g_nCountDisp = 0;				//
	bool					g_bDispStart = false;			//
	int						g_nConutDemo = 0;				//
public:
	TitleClass();
	~TitleClass();

	HRESULT MakeVertexTitle(LPDIRECT3DDEVICE9 pDevice);
	void SetColorTitleLogo(void);

	HRESULT InitTitle(void);
	void UninitTitle(void);
	void UpdateTitle(void);
	void DrawTitle(void);
};

