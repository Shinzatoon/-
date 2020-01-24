//=============================================================================
// ���e�B�N����ʏ��� [reticle.h]
// Author : �V���@���m
//
//=============================================================================
#pragma once

#include "main.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	TEXTURE_RETICLE		"data/TEXTURE/�^�[�Q�b�g�̃A�C�R��.png"	// �ǂݍ��ރe�N�X�`���t�@�C����
#define	RETICLE_SIZE_X		(20.0f)							// �r���{�[�h�̕�
#define	RETICLE_SIZE_Y		(20.0f)							// �r���{�[�h�̍���

#define MAX_RETICLE			(1)								//�r���{�[�h�̍ő吔

//===========================
//�N���X�錾
//===========================
class ReticleClass
{
private:
	LPDIRECT3DTEXTURE9		g_pD3DTextureReticle = NULL;	// �e�N�X�`���ւ̃|�C���^
	LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuffReticle = NULL;	// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^


	OBJECT3D		*pModel2;
	OBJECT3D		*ParentModel2;							//���f���Q�̏��@�y�A�����g����

	D3DXVECTOR3		*PlayerVector;
	D3DXVECTOR3		*PlayerLength;

public:
	ReticleClass();
	~ReticleClass();

	//���e�B�N���\����
	D3DXMATRIX			g_mtxWorldReticle;				// ���[���h�}�g���b�N�X
	D3DXMATRIX			g_mtxProvisional;				// ���e�B�N���{���̃��[���h�}�g���b�N�X�̔��ꏊ
	D3DXVECTOR3			reticlePos;						// �ʒu
	D3DXVECTOR3			reticleRot;						// ��]
	D3DXVECTOR3			reticleScale;					// �X�P�[��
	D3DXVECTOR3			reticleMove;					// �ړ���
	D3DXCOLOR			reticleCol;						// �F
	float				reticlefSizeX;					// ��
	float				reticlefSizeY;					// ����
	int					reticleIdxShadow;				// �eID
	int					reticleLife;					// ����
	bool				reticleUse;						// �g�p���Ă��邩�ǂ���
	D3DXVECTOR3			posBase;						// �r���{�[�h�����ʒu

	HRESULT MakeVertexReticle(LPDIRECT3DDEVICE9 pDevice);
	void SetVertexReticle(int nIdxReticle, float fSizeX, float fSizeY);
	void SetColorReticle(int nIdxReticle, D3DXCOLOR col);

	HRESULT InitReticle();
	void DrawReticle(D3DXMATRIX cameraView);
	void UpdateReticle();
	void UninitReticle();

	int SetReticle(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, float fSizeX, float fSizeY, int nLife);

};

