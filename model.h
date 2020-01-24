#pragma once
//=============================================================================
// ���f������ [model.h]
// Author :�V���@���m
//
//=============================================================================
#ifndef _MODEL_H_
#define _MODEL_H_

#include "main.h"

//*****************************************************************************
// �\���̐錾
//*****************************************************************************
//typedef struct
//{
//
//	D3DXVECTOR3			g_posModel;				// ���f���̈ʒu
//	D3DXVECTOR3			g_rotModel;				// ���f���̌���(��])
//	D3DXVECTOR3			g_sclModel;				// ���f���̑傫��(�X�P�[��)
//	D3DXMATRIX			g_mtxWorld;				// ���[���h�}�g���b�N�X
//
//	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;
//
//}MODEL;

class BulletClass;

//*****************************************************************************
// �v���g�^�C�v�錾
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
