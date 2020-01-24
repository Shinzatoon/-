//=============================================================================
// �n�ʏ��� [field.h]
// Author : �V���@���m
//
//=============================================================================
#pragma once

#include "main.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	TEXTURE_FIELD	"data/TEXTURE/field004.jpg"	// �ǂݍ��ރe�N�X�`���t�@�C����
#define NUM_FIELD (3)								//�t�B�[���h�̖���	
#define LIMIT_FIELD_Z (3 * 500.0f)					//�t�B�[���h�̐؂�ւ��_

//*****************************************************************************
// �N���X�錾
//*****************************************************************************
class FieldClass
{
private:
	LPDIRECT3DTEXTURE9		g_pD3DTextureField = NULL;	// �e�N�X�`���ւ̃|�C���^
	LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuffField = NULL;	// ���_�o�b�t�@�ւ̃|�C���^

	D3DXMATRIX				g_mtxWorldField;			// ���[���h�}�g���b�N�X
	D3DXVECTOR3				g_posField;					// ���݂̈ʒu
	D3DXVECTOR3				g_rotField;					// ���݂̌���

	float					StractdistanceZ[NUM_FIELD];			//Player��Field��Z�������Z���ʂ̊i�[�ϐ�

	D3DXMATRIX e_mtxScl, e_mtxRot, e_mtxTranslate;

	OBJECT3D				field[NUM_FIELD];

	OBJECT3D				*pPlayerField;

public:
	FieldClass();
	~FieldClass();

	HRESULT MakeVertexField(LPDIRECT3DDEVICE9 pDevice);

	HRESULT InitField(void);
	void UninitField(void);
	void UpdateField(void);
	void DrawField(void);

};

