//=============================================================================
// �J�������� [camera.cpp]
// Author : �V���@���m
//
//=============================================================================
#include "CameraClass.h"
#include "input.h"
#include "model.h"
#include "reticle.h"
#include "ReticleClass.h"

//=============================================================================
//�O���[�o���ϐ�
//=============================================================================
//CameraClass* pointerCamera = NULL;

//CameraClass* getCamera() { return pointerCamera; };

CameraClass::CameraClass()
{

	//pointerCamera = this;
}


CameraClass::~CameraClass()
{
	//pointerCamera = NULL;
}
//RETICLE				*pReticle;
//=============================================================================
// �J�����̏���������
//=============================================================================
HRESULT CameraClass::InitCamera(void)
{
	CplayerWK = GetPlayer();
	Cplayer2WK = GetPlayer2();

	g_posCameraP = Cplayer2WK->m_posModel;		//�J�����̈ʒu
	g_posCameraR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);											//D3DXVECTOR3(0.0f, 0.0f, 0.0f);									//�J�����̒����_
	g_vecCameraU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);									//�J�����̏�x�N�g���@�@�Œ�

	relativeVector = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	degree = 90.0f;

	return S_OK;

	//D3DXVECTOR3(POS_CAMERA_P_X, POS_CAMERA_P_Y, POS_CAMERA_P_Z)
}

//=============================================================================
// �X�V����
//=============================================================================
void CameraClass::UpdateCamera(void)
{
	CplayerWK = GetPlayer();
	Cplayer2WK = GetPlayer2();
	//pReticle = GetReticle();
	//ReticleClass reticle;

	//ReticleMtxVector.x = reticle.g_mtxWorldReticle._31;
	//ReticleMtxVector.y = reticle.g_mtxWorldReticle._32;
	//ReticleMtxVector.z = reticle.g_mtxWorldReticle._33;



	//�J�����k��
	if (GetKeyboardPress(DIK_X) || IsButtonPressed(0, BUTTON_ZR))
	{
		g_Ofset += 1.0f;

		if (g_Ofset >= -150.0f)
		{
			g_Ofset = -150.0f;
		}
	}

	//�J�����g��
	if (GetKeyboardPress(DIK_Z) || IsButtonPressed(0, BUTTON_LR))
	{
		g_Ofset -= 1.0f;

		if (g_Ofset <= -450.0f)
		{
			g_Ofset = -450.0f;
		}
	}

	
	if (targetPosition == NULL && targetDirection == NULL)
	{
		g_posCameraP = D3DXVECTOR3(0,0,0);
	}
	else{
		g_posCameraP = *targetPosition + *targetDirection * g_Ofset;
	}
	g_posCameraP.y = POS_CAMERA_P_Y;

}

//=============================================================================
// �`�揈��
//=============================================================================
void CameraClass::DrawCamera(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	CplayerWK = GetPlayer();
	//pReticle = GetReticle();

	// �r���[�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_mtxView);
	if (targetPosition == NULL) 
	{// �r���[�}�g���b�N�X�̍쐬
		D3DXMatrixLookAtLH(&g_mtxView,
			&g_posCameraP,			// �J�����̎��_
			&D3DXVECTOR3(0,0,0),	// �J�����̒����_
			&g_vecCameraU);			// �J�����̏�����x�N�g��
	}
	else
	{// �r���[�}�g���b�N�X�̍쐬
		D3DXMatrixLookAtLH(&g_mtxView,
			&g_posCameraP,		// �J�����̎��_
			targetPosition,		// �J�����̒����_
			&g_vecCameraU);		// �J�����̏�����x�N�g��
	}
// �r���[�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_VIEW, &g_mtxView);


	// �v���W�F�N�V�����}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_mtxProjection);

	// �v���W�F�N�V�����}�g���b�N�X�̍쐬
	D3DXMatrixPerspectiveFovLH(&g_mtxProjection,
		VIEW_ANGLE,		// �r���[���ʂ̎���p
		VIEW_ASPECT,		// �r���[���ʂ̃A�X�y�N�g��
		VIEW_NEAR_Z,		// �r���[���ʂ�NearZ�l
		VIEW_FAR_Z);		// �r���[���ʂ�FarZ�l

// �v���W�F�N�V�����}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_PROJECTION, &g_mtxProjection);
}

//CameraClass *GetMtxView(void)
//{
//	return CameraClass;
//}