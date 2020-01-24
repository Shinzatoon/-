//=============================================================================
// カメラ処理 [camera.h]
// Author : 新里　将士
//
//=============================================================================
#pragma once

#include "main.h"
#include "model.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	POS_CAMERA_P_X	(0.0f)											//カメラ視点の初期位置(X座標)
#define	POS_CAMERA_P_Y	(30.0f)											//カメラ視点の初期位置(Y座標)
#define	POS_CAMERA_P_Z	(-150.0f)										//カメラ視点の初期位置(Z座標)
#define	VIEW_ANGLE		(D3DXToRadian(45.0f))							//ビュー平面の視野角
#define	VIEW_ASPECT		((float)SCREEN_WIDTH / (float)SCREEN_HEIGHT)	//ビュー平面のアスペクト比	
#define	VIEW_NEAR_Z		(10.0f)											//ビュー平面のNearZ値
#define	VIEW_FAR_Z		(2000.0f)										//ビュー平面のFarZ値

//*****************************************************************************
// クラス宣言
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

	D3DXVECTOR3			g_posCameraP;			// カメラの視点
	D3DXVECTOR3			g_posCameraR;			// カメラの注視点
	D3DXVECTOR3			g_vecCameraU;			// カメラの上方向ベクトル
	D3DXVECTOR3			relativeVector;
	float				degree;

	D3DXMATRIX			g_mtxView;				// ビューマトリックス
	D3DXMATRIX			g_mtxProjection;		// プロジェクションマトリックス

	float				g_Ofset = -400.0f;				// カメラの画角の変更

	HRESULT InitCamera(void);
	void UpdateCamera(void);
	void DrawCamera(void);
	//CAMERA* GetMtxView(void);
	
	void setTargetPostion(D3DXVECTOR3* target) {targetPosition = target;};
	void setTargetDirection(D3DXVECTOR3* target) {targetDirection = target;};
	
};

//CameraClass* getCamera();