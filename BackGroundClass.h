//=============================================================================
// �w�i���� [BG.h]
// Author : �V���@���m
//
//=============================================================================
#pragma once

#include "main.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	MODEL_BG		"data/MODEL/skyhigh.x"							// �ǂݍ��ރ��f����
#define TEXTURE_BG_FILENAME "data/TEXTURE/bluesky2.jpg"

//*****************************************************************************
// �N���X�錾
//*****************************************************************************
class BackGroundClass
{
private:
	LPDIRECT3DTEXTURE9	g_pD3DTextureBG;		// �e�N�X�`���ւ̃|�C���^
	LPD3DXMESH			g_pD3DXMeshBG;		// ���b�V�����ւ̃|�C���^�@�ǂݍ���X�t�@�C�����f����\���B���f���𑝂₵���������炱��𑝂₷�B
	LPD3DXBUFFER		g_pD3DXBuffMatBG;	// �}�e���A�����ւ̃|�C���^�@�ǂݍ���X�t�@�C�����f���̃}�e���A����\���B
	DWORD				g_nNumMatBG;			// �}�e���A�����̐��@�ǂݍ���X�t�@�C�����f���̃}�e���A���̐��B

	D3DXVECTOR3			g_posBG;				// ���f���̈ʒu
	D3DXVECTOR3			g_rotBG;				// ���f���̌���(��])
	D3DXVECTOR3			g_sclBG;				// ���f���̑傫��(�X�P�[��)



	D3DXMATRIX			g_mtxViewBG;				// �r���[�}�g���b�N�X
	D3DXMATRIX			g_mtxProjectionBG;		// �v���W�F�N�V�����}�g���b�N�X
	D3DXMATRIX			g_mtxWorldBG;				// ���[���h�}�g���b�N�X


	D3DXMATRIX mtxSclBG, mtxRotBG, mtxTranslateBG;


	OBJECT3D	*pBgModel0;
public:
	BackGroundClass();
	~BackGroundClass();

	HRESULT InitBG(void);
	void UninitBG(void);
	void UpdateBG(void);
	void DrawBG(void);
};

