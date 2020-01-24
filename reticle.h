#pragma once
//=============================================================================
// ���e�B�N����ʏ��� [reticle.h]
// Author : �V���@���m
//
//=============================================================================

#ifndef _RETICLE_H_
#define _RETICLE_H_

#include "main.h"

//*****************************************************************************
// �\���̒�`
//*****************************************************************************
typedef struct
{
	D3DXMATRIX	g_mtxWorldReticle;				// ���[���h�}�g���b�N�X
	D3DXMATRIX  g_mtxProvisional;				// ���e�B�N���{���̃��[���h�}�g���b�N�X�̔��ꏊ
	D3DXVECTOR3 pos;		// �ʒu
	D3DXVECTOR3 rot;		// ��]
	D3DXVECTOR3 scale;		// �X�P�[��
	D3DXVECTOR3 move;		// �ړ���
	D3DXCOLOR col;			// �F
	float fSizeX;			// ��
	float fSizeY;			// ����
	int nIdxShadow;			// �eID
	int nLife;				// ����
	bool bUse;				// �g�p���Ă��邩�ǂ���
	D3DXVECTOR3				posBase;						// �r���{�[�h�����ʒu
} RETICLE;

//===========================
//�v���g�^�C�v�錾
//===========================

HRESULT InitReticle();
void DrawReticle();
void UpdateReticle();
void UninitReticle();

RETICLE* GetReticle(void);

int SetReticle(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, float fSizeX, float fSizeY, int nLife);

#endif
