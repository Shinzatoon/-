//=============================================================================
// �Ǐ��� [wall.h]
// Author :  �V���@���m
//
//=============================================================================
#pragma once
#include "main.h"

//******************************************************
//�}�N����`
//******************************************************
#define SCROOL_SPEED (0.009f)
#define MODEL_WALL		"data/MODEL/side.x"
#define TEXTURE_WALLFILE "data/TEXTURE/side_arrow.png"
#define RIGHT_WALL		(20)
#define LEFT_WALL		(20)
#define NUM_WALL		(RIGHT_WALL+LEFT_WALL)				//�ǂ̖���
#define LIMIT_WALL_Z	(NUM_WALL/2 * 200.0f)

//*****************************************************************************
// �N���X�錾
//*****************************************************************************
class WallClass
{
private:
	LPDIRECT3DTEXTURE9	w_pD3DTextureModel;		// �e�N�X�`���ւ̃|�C���^
	LPD3DXMESH			w_pD3DXMeshSphere;
	LPD3DXMESH			w_pD3DXMeshModel;		// ���b�V�����ւ̃|�C���^�@�ǂݍ���X�t�@�C�����f����\���B���f���𑝂₵���������炱��𑝂₷�B
	LPD3DXBUFFER		w_pD3DXBuffMatModel;	// �}�e���A�����ւ̃|�C���^�@�ǂݍ���X�t�@�C�����f���̃}�e���A����\���B
	DWORD				w_nNumMatModel;			// �}�e���A�����̐��@�ǂݍ���X�t�@�C�����f���̃}�e���A���̐��B

	OBJECT3D			WallWK[NUM_WALL];

	OBJECT3D			*pPlayerWall;

	float					StractdistanceWall = 0;			//Player��Wall��Z�������Z���ʂ̊i�[�ϐ�
public:
	WallClass();
	~WallClass();

	HRESULT InitWall(void);
	void UninitWall(void);
	void UpdateWall(void);
	void DrawWall(void);
};
