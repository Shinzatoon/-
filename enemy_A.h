#pragma once
//=============================================================================
// �G�l�~�[A���� [enemy_A.h]
// Author : �V���@���m
//
//=============================================================================
#ifndef _ENEMY_A_H_
#define _ENEMY_A_H_

#include "main.h"

//*****************************************************************************
// �\���̐錾
//*****************************************************************************
//typedef struct
//{
//	int                 m_bUse;                 // ���̍\���̂��g�p����(TRUE:�g�p��)
//	D3DXVECTOR3			e_posModel;				// ���f���̈ʒu
//	D3DXVECTOR3			e_rotModel;				// ���f���̌���(��])
//	D3DXVECTOR3			e_sclModel;				// ���f���̑傫��(�X�P�[��)
//	D3DXMATRIX			e_mtxWorld;				// ���[���h�}�g���b�N�X
//
//	D3DXVECTOR3			m_Radius;				//��]�ړ��p���a
//
//	D3DXMATRIX e_mtxScl, e_mtxRot, e_mtxTranslate;
//
//
//
//
//}ENEMY;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitEnemy_A(void);
void UninitEnemy_A(void);
void UpdateEnemy_A(void);
void DrawEnemy_A(void);

int GetEnemyWave2(void);


#endif