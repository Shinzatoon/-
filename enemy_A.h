#pragma once
//=============================================================================
// エネミーA処理 [enemy_A.h]
// Author : 新里　将士
//
//=============================================================================
#ifndef _ENEMY_A_H_
#define _ENEMY_A_H_

#include "main.h"

//*****************************************************************************
// 構造体宣言
//*****************************************************************************
//typedef struct
//{
//	int                 m_bUse;                 // この構造体が使用中か(TRUE:使用中)
//	D3DXVECTOR3			e_posModel;				// モデルの位置
//	D3DXVECTOR3			e_rotModel;				// モデルの向き(回転)
//	D3DXVECTOR3			e_sclModel;				// モデルの大きさ(スケール)
//	D3DXMATRIX			e_mtxWorld;				// ワールドマトリックス
//
//	D3DXVECTOR3			m_Radius;				//回転移動用半径
//
//	D3DXMATRIX e_mtxScl, e_mtxRot, e_mtxTranslate;
//
//
//
//
//}ENEMY;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitEnemy_A(void);
void UninitEnemy_A(void);
void UpdateEnemy_A(void);
void DrawEnemy_A(void);

int GetEnemyWave2(void);


#endif