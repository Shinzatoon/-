//=============================================================================
// カメラ処理 [camera.cpp]
// Author : 新里　将士
//
//=============================================================================
#include "CameraClass.h"
#include "input.h"
#include "model.h"
#include "reticle.h"
#include "ReticleClass.h"

//=============================================================================
//グローバル変数
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
// カメラの初期化処理
//=============================================================================
HRESULT CameraClass::InitCamera(void)
{
	CplayerWK = GetPlayer();
	Cplayer2WK = GetPlayer2();

	g_posCameraP = Cplayer2WK->m_posModel;		//カメラの位置
	g_posCameraR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);											//D3DXVECTOR3(0.0f, 0.0f, 0.0f);									//カメラの注視点
	g_vecCameraU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);									//カメラの上ベクトル　　固定

	relativeVector = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	degree = 90.0f;

	return S_OK;

	//D3DXVECTOR3(POS_CAMERA_P_X, POS_CAMERA_P_Y, POS_CAMERA_P_Z)
}

//=============================================================================
// 更新処理
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



	//カメラ縮小
	if (GetKeyboardPress(DIK_X) || IsButtonPressed(0, BUTTON_ZR))
	{
		g_Ofset += 1.0f;

		if (g_Ofset >= -150.0f)
		{
			g_Ofset = -150.0f;
		}
	}

	//カメラ拡大
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
// 描画処理
//=============================================================================
void CameraClass::DrawCamera(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	CplayerWK = GetPlayer();
	//pReticle = GetReticle();

	// ビューマトリックスの初期化
	D3DXMatrixIdentity(&g_mtxView);
	if (targetPosition == NULL) 
	{// ビューマトリックスの作成
		D3DXMatrixLookAtLH(&g_mtxView,
			&g_posCameraP,			// カメラの視点
			&D3DXVECTOR3(0,0,0),	// カメラの注視点
			&g_vecCameraU);			// カメラの上方向ベクトル
	}
	else
	{// ビューマトリックスの作成
		D3DXMatrixLookAtLH(&g_mtxView,
			&g_posCameraP,		// カメラの視点
			targetPosition,		// カメラの注視点
			&g_vecCameraU);		// カメラの上方向ベクトル
	}
// ビューマトリックスの設定
	pDevice->SetTransform(D3DTS_VIEW, &g_mtxView);


	// プロジェクションマトリックスの初期化
	D3DXMatrixIdentity(&g_mtxProjection);

	// プロジェクションマトリックスの作成
	D3DXMatrixPerspectiveFovLH(&g_mtxProjection,
		VIEW_ANGLE,		// ビュー平面の視野角
		VIEW_ASPECT,		// ビュー平面のアスペクト比
		VIEW_NEAR_Z,		// ビュー平面のNearZ値
		VIEW_FAR_Z);		// ビュー平面のFarZ値

// プロジェクションマトリックスの設定
	pDevice->SetTransform(D3DTS_PROJECTION, &g_mtxProjection);
}

//CameraClass *GetMtxView(void)
//{
//	return CameraClass;
//}