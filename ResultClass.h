//=============================================================================
// リザルト画面処理 [result.h]
// Author : 新里　将士
//
//=============================================================================
#pragma once

#include "main.h"


//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	TEXTURE_RESULT		"data/TEXTURE/bg001.jpg"		// 読み込むテクスチャファイル名
#define	TEXTURE_RESULT_LOGO	"data/TEXTURE/result_logo.png"	// 読み込むテクスチャファイル名

#define	RESULT_LOGO_POS_X	(240)		// リザルトロゴの位置(X座標)
#define	RESULT_LOGO_POS_Y	(320)		// リザルトロゴの位置(Y座標)
#define	RESULT_LOGO_WIDTH	(800)		// リザルトロゴの幅
#define	RESULT_LOGO_HEIGHT	(240)		// リザルトロゴの高さ

#define	COUNT_APPERA_RESULT	(60)		// リザルトロゴ出現までの待ち時間	
#define	LIMIT_COUNT_WAIT	(60 * 5)	// 待ち時間

//*****************************************************************************
// クラス宣言
//*****************************************************************************
class ResultClass
{
private:
	LPDIRECT3DTEXTURE9		g_pD3DTextureResult = NULL;		// テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuffResult = NULL;		// 頂点バッファインターフェースへのポインタ
	LPDIRECT3DTEXTURE9		g_pD3DTextureResultLogo = NULL;	// テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuffResultLogo = NULL;	// 頂点バッファインターフェースへのポインタ
	int						g_nCountAppearResult = 0;		// 出現までの待ち時間
	float					g_fAlphaResult = 0.0f;			// リザルトロゴのα値
	int						g_nCountWaitResult = 0;			// 待ち時間
public:
	ResultClass();
	~ResultClass();

	HRESULT MakeVertexResult(LPDIRECT3DDEVICE9 pDevice);
	void SetColorResultLogo(void);

	HRESULT InitResult(void);
	void UninitResult(void);
	void UpdateResult(void);
	void DrawResult(void);
};

