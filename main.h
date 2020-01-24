#pragma once
//=============================================================================
// ���C������ [main.h]
// Author : �V���@���m
//
//=============================================================================
#ifndef _MAIN_H_
#define _MAIN_H_

//*****************************************************************************
// �C���N���[�h�t�@�C��
//*****************************************************************************
#include <windows.h>
#include <stdio.h>
#include "d3dx9.h"

#define DIRECTINPUT_VERSION (0x0800)	// �x���΍�
#include "dinput.h"
#include "xaudio2.h"		// XAudio2�֘A�̃w�b�_�[

//*****************************************************************************
// ���C�u�����̃����N
//*****************************************************************************
#if 1	// [������"0"�ɂ����ꍇ�A"�\���v���p�e�B" -> "�����J" -> "����" -> "�ǉ��̈ˑ��t�@�C��"�ɑΏۃ��C�u������ݒ肷��]
#pragma comment (lib, "d3d9.lib")
#pragma comment (lib, "d3dx9.lib")
#pragma comment (lib, "dxguid.lib")
#pragma comment (lib, "dinput8.lib")
#pragma comment (lib, "winmm.lib")
#endif

//*****************************************************************************
// �}�N����`
//*****************************************************************************
// �Q�c�|���S�����_�t�H�[�}�b�g( ���_���W[2D] / ���ˌ� / �e�N�X�`�����W )
#define	FVF_VERTEX_2D	(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)
// �R�c�|���S�����_�t�H�[�}�b�g( ���_���W[3D] / �@�� / ���ˌ� / �e�N�X�`�����W )
#define	FVF_VERTEX_3D	(D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1)

#define SCREEN_WIDTH	(1280)				// �E�C���h�E�̕�
#define SCREEN_HEIGHT	(960)				// �E�C���h�E�̍���
#define SCREEN_CENTER_X	(SCREEN_WIDTH / 2)	// �E�C���h�E�̒��S�w���W
#define SCREEN_CENTER_Y	(SCREEN_HEIGHT / 2)	// �E�C���h�E�̒��S�x���W

#define	NUM_VERTEX		(16)		// ���_��
#define	NUM_POLYGON		(8)		// �|���S����

#define PI (3.141592654f)

// ��L�Q�c�|���S�����_�t�H�[�}�b�g�ɍ��킹���\���̂��`
typedef struct
{
	D3DXVECTOR3 vtx;		// ���_���W
	float rhw;				// �e�N�X�`���̃p�[�X�y�N�e�B�u�R���N�g�p
	D3DCOLOR diffuse;		// ���ˌ�
	D3DXVECTOR2 tex;		// �e�N�X�`�����W
} VERTEX_2D;

// ��L�R�c�|���S�����_�t�H�[�}�b�g�ɍ��킹���\���̂��`
typedef struct
{
	D3DXVECTOR3 vtx;		// ���_���W
	D3DXVECTOR3 nor;		// �@���x�N�g��
	D3DCOLOR diffuse;		// ���ˌ�
	D3DXVECTOR2 tex;		// �e�N�X�`�����W
} VERTEX_3D;

//**************************************************************************
// ���f���\����
//**************************************************************************
typedef struct
{
	//�|���S��X�t�@�C�����f��1���̏��
	LPDIRECT3DTEXTURE9	m_pD3DTextureModel;		// �e�N�X�`���֏��̃|�C���^
	LPD3DXMESH			m_pD3DXMeshModel;		// ���b�V�����ւ̃|�C���^
	LPD3DXBUFFER		m_pD3DXBuffMatModel;	// �}�e���A�����ւ̃|�C���^
	DWORD				m_nNumMatModel;			// �}�e���A�����̐�

	//D3DXMATRIX			m_mtxScl, m_mtxRot, m_mtxTranslate;

}MODEL3D;

//*************************************
// ���[�h�̎��
//*************************************
typedef enum
{
	MODE_TITLE = 0,			// �^�C�g�����
	MODE_GAME,				// �Q�[�����
	MODE_RESULT,			// ���U���g���
	MODE_MAX
} MODE;

//**************************************************************************
// �L�����N�^�\����
//**************************************************************************
typedef struct
{

	//�L�����N�^1���̏��
	int                 m_bUse;                 // ���̍\���̂��g�p����(TRUE:�g�p��)
	D3DXVECTOR3			m_posModel;				// ���f���̈ʒu
	D3DXVECTOR3			m_rotModel;				// ���f���̌���(��])
	D3DXVECTOR3			m_sclModel;				// ���f���̑傫��(�X�P�[��)
	D3DXMATRIX			m_mtxWorld;				// ���[���h�}�g���b�N�X

	D3DXMATRIX*			m_mtxParent = NULL;			// �e���[���h�}�g���b�N�X
	D3DXMATRIX*			m_mtxRotParent = NULL;		// �e���[���h�}�g���b�N�X[��]]
	D3DXMATRIX			m_virtualMtxRot;			// ���f���̌���(��])

	D3DXVECTOR3			m_DirSpeed;				//�ړ��x�N�g��
	D3DXVECTOR3			m_RotSpeed;				//��]�X�s�[�h
	D3DXVECTOR3			m_Kakudo;				//��]�ړ��p�p�x
	D3DXVECTOR3			m_Radius;				//��]�ړ��p���a

	D3DXVECTOR3			EnemyCenter;
	D3DXVECTOR3			bulletLength;

	D3DXMATRIX			m_mtxScl, m_mtxRot, m_mtxTranslate;

	int					m_ModelID;				// ���̃L�����N�^�̃��f���f�[�^��ID
	int					m_ShadowID;				// ���̃L�����N�^�̉e�I�u�W�F�N�g��ID

	float				EnemyRadius;

}OBJECT3D;



//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
LPDIRECT3DDEVICE9 GetDevice(void);
float GetFrameTime(void);
void SetMode(MODE mode);
MODE GetMode(void);
void DrawGameTime(float a);

//�����蔻��i��VS���j
BOOL	HitOBJECT3D(OBJECT3D *obj1, OBJECT3D *obj2);
//�����֐�
D3DXMATRIX* CalcLookAtMatrixAxisFix(
	D3DXMATRIX* pout,
	D3DXVECTOR3* pPos,
	D3DXVECTOR3* pLook,
	D3DXVECTOR3* pUp);
// �L�����N�^�p���s��Z�o�֐�
D3DXMATRIX* CalcLookAtMatrix(
	D3DXMATRIX* pout,
	D3DXVECTOR3* pPos,
	D3DXVECTOR3* pLook,
	D3DXVECTOR3* pUp);


#endif