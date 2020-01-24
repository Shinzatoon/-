#pragma once
//=============================================================================
// モデル処理 [model.h]
// Author :新里　将士
//
//=============================================================================
#ifndef _MODEL_H_
#define _MODEL_H_

#include "main.h"

//*****************************************************************************
// 構造体宣言
//*****************************************************************************
//typedef struct
//{
//
//	D3DXVECTOR3			g_posModel;				// モデルの位置
//	D3DXVECTOR3			g_rotModel;				// モデルの向き(回転)
//	D3DXVECTOR3			g_sclModel;				// モデルの大きさ(スケール)
//	D3DXMATRIX			g_mtxWorld;				// ワールドマトリックス
//
//	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;
//
//}MODEL;

class BulletClass;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitModel(void);
void UninitModel(void);
void UpdateModel(void);
void DrawModel(void);
void SetRotParent(int i, D3DXMATRIX* newParent);
void SetParent(int i, D3DXMATRIX* newParent);
OBJECT3D* GetPlayer(void);
OBJECT3D* GetPlayer1(void);
OBJECT3D* GetPlayer2(void);
MODEL3D* GetModel(void);

D3DXVECTOR3* GetReticleVector(void);
D3DXVECTOR3* GetReticleLength (void);

D3DXVECTOR3* GetEffectVector(void);
D3DXVECTOR3* GetEffectLength (void);


enum MODELDATE
{
	MAINTANK,
	TANKTABLE,
	TANKBAZOOKA
};



#endif
