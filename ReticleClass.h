//=============================================================================
// レティクル画面処理 [reticle.h]
// Author : 新里　将士
//
//=============================================================================
#pragma once

#include "main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	TEXTURE_RETICLE		"data/TEXTURE/ターゲットのアイコン.png"	// 読み込むテクスチャファイル名
#define	RETICLE_SIZE_X		(20.0f)							// ビルボードの幅
#define	RETICLE_SIZE_Y		(20.0f)							// ビルボードの高さ

#define MAX_RETICLE			(1)								//ビルボードの最大数

//===========================
//クラス宣言
//===========================
class ReticleClass
{
private:
	LPDIRECT3DTEXTURE9		g_pD3DTextureReticle = NULL;	// テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuffReticle = NULL;	// 頂点バッファインターフェースへのポインタ


	OBJECT3D		*pModel2;
	OBJECT3D		*ParentModel2;							//モデル２の情報　ペアレント処理

	D3DXVECTOR3		*PlayerVector;
	D3DXVECTOR3		*PlayerLength;

public:
	ReticleClass();
	~ReticleClass();

	//レティクル構造体
	D3DXMATRIX			g_mtxWorldReticle;				// ワールドマトリックス
	D3DXMATRIX			g_mtxProvisional;				// レティクル本来のワールドマトリックスの避難場所
	D3DXVECTOR3			reticlePos;						// 位置
	D3DXVECTOR3			reticleRot;						// 回転
	D3DXVECTOR3			reticleScale;					// スケール
	D3DXVECTOR3			reticleMove;					// 移動量
	D3DXCOLOR			reticleCol;						// 色
	float				reticlefSizeX;					// 幅
	float				reticlefSizeY;					// 高さ
	int					reticleIdxShadow;				// 影ID
	int					reticleLife;					// 寿命
	bool				reticleUse;						// 使用しているかどうか
	D3DXVECTOR3			posBase;						// ビルボード発生位置

	HRESULT MakeVertexReticle(LPDIRECT3DDEVICE9 pDevice);
	void SetVertexReticle(int nIdxReticle, float fSizeX, float fSizeY);
	void SetColorReticle(int nIdxReticle, D3DXCOLOR col);

	HRESULT InitReticle();
	void DrawReticle(D3DXMATRIX cameraView);
	void UpdateReticle();
	void UninitReticle();

	int SetReticle(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, float fSizeX, float fSizeY, int nLife);

};

