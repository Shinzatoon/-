#pragma once
//=============================================================================
// レティクル画面処理 [reticle.h]
// Author : 新里　将士
//
//=============================================================================

#ifndef _RETICLE_H_
#define _RETICLE_H_

#include "main.h"

//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef struct
{
	D3DXMATRIX	g_mtxWorldReticle;				// ワールドマトリックス
	D3DXMATRIX  g_mtxProvisional;				// レティクル本来のワールドマトリックスの避難場所
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
	D3DXVECTOR3				posBase;						// ビルボード発生位置
} RETICLE;

//===========================
//プロトタイプ宣言
//===========================

HRESULT InitReticle();
void DrawReticle();
void UpdateReticle();
void UninitReticle();

RETICLE* GetReticle(void);

int SetReticle(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, float fSizeX, float fSizeY, int nLife);

#endif
