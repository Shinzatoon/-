#pragma once
//=============================================================================
// パーティクル処理 [particle.h]
// Author : 新里　将士
//
//=============================================================================
#ifndef _PARTICLE_H_
#define _PARTICLE_H_

#include "main.h"

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitParticle(void);
void UninitParticle(void);
void UpdateParticle(void);
void DrawParticle(D3DXMATRIX cameraView);

int SetParticle(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, float fSizeX, float fSizeY, int nLife);

#endif
