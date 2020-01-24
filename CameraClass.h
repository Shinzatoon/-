//=============================================================================
// �J�������� [camera.h]
// Author : �V���@���m
//
//=============================================================================
#pragma once

#include "main.h"
#include "model.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	POS_CAMERA_P_X	(0.0f)											//�J�������_�̏����ʒu(X���W)
#define	POS_CAMERA_P_Y	(30.0f)											//�J�������_�̏����ʒu(Y���W)
#define	POS_CAMERA_P_Z	(-150.0f)										//�J�������_�̏����ʒu(Z���W)
#define	VIEW_ANGLE		(D3DXToRadian(45.0f))							//�r���[���ʂ̎���p
#define	VIEW_ASPECT		((float)SCREEN_WIDTH / (float)SCREEN_HEIGHT)	//�r���[���ʂ̃A�X�y�N�g��	
#define	VIEW_NEAR_Z		(10.0f)											//�r���[���ʂ�NearZ�l
#define	VIEW_FAR_Z		(2000.0f)										//�r���[���ʂ�FarZ�l

//*****************************************************************************
// �N���X�錾
//*****************************************************************************
class CameraClass
{
private:
	

	OBJECT3D			*CplayerWK;
	OBJECT3D			*Cplayer2WK;
	//RETICLE			*pReticle;
	//D3DXVECTOR3			ReticleMtxVector = D3DXVECTOR3(0, 0, 0);
	D3DXVECTOR3*		targetPosition = NULL;
	D3DXVECTOR3*		targetDirection = NULL;


public:
	CameraClass();
	~CameraClass();

	D3DXVECTOR3			g_posCameraP;			// �J�����̎��_
	D3DXVECTOR3			g_posCameraR;			// �J�����̒����_
	D3DXVECTOR3			g_vecCameraU;			// �J�����̏�����x�N�g��
	D3DXVECTOR3			relativeVector;
	float				degree;

	D3DXMATRIX			g_mtxView;				// �r���[�}�g���b�N�X
	D3DXMATRIX			g_mtxProjection;		// �v���W�F�N�V�����}�g���b�N�X

	float				g_Ofset = -400.0f;				// �J�����̉�p�̕ύX

	HRESULT InitCamera(void);
	void UpdateCamera(void);
	void DrawCamera(void);
	//CAMERA* GetMtxView(void);
	
	void setTargetPostion(D3DXVECTOR3* target) {targetPosition = target;};
	void setTargetDirection(D3DXVECTOR3* target) {targetDirection = target;};
	
};

//CameraClass* getCamera();