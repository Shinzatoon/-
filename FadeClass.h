//=============================================================================
// フェード処理 [fade.h]
// Author : 新里　将士
//
//=============================================================================
#pragma once

#include "main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	FADE_RATE		(0.01f)		// フェード係数

//フェードの状態種類
enum class FADE
{
	FADE_NONE = 0,		// 何もない状態
	FADE_IN,			// フェードイン処理
	FADE_OUT,			// フェードアウト処理
	FADE_MAX
};

//*****************************************************************************
// クラス宣言
//*****************************************************************************
class FadeClass
{
private:

	LPDIRECT3DTEXTURE9		g_p3DTextureFade = NULL;	// テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuffFade = NULL;	// 頂点バッファインターフェースへのポインタ
	D3DXCOLOR				g_color;
	FADE					g_fade = FADE::FADE_IN;
	
public:
	FadeClass();
	~FadeClass();

	HRESULT MakeVertexFade(LPDIRECT3DDEVICE9 pDevice);
	void SetColor(D3DCOLOR col);
	
	HRESULT InitFade(void);
	void UninitFade(void);
	void UpdateFade(void);
	void DrawFade(void);

	void SetFade(FADE fade);
	FADE GetFade(void);
	
};


FadeClass* getFadeMain();
