//=============================================================================
// ライト処理 [light.cpp]
// Author : 新里　将士
//
//=============================================================================
#include "LightClass.h"



LightClass::LightClass()
{
}


LightClass::~LightClass()
{
}

//=============================================================================
// ライトの初期化処理
//=============================================================================
void LightClass::InitLight(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXVECTOR3 vecDir;




	// D3DLIGHT9構造体を0でクリアする
	ZeroMemory(&g_aLight[0], sizeof(D3DLIGHT9));

	// ライト0のタイプの設定
	g_aLight[0].Type = D3DLIGHT_DIRECTIONAL;

	g_aLight[0].Range = 5000.0f;
	g_aLight[0].Attenuation0 = 0.5f;

	// ライト0の拡散光の設定 
	g_aLight[0].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// ライト0の環境光の設定　アンビエントライト
	g_aLight[0].Ambient = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// ライト0の鏡面反射
	g_aLight[0].Specular = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// ライト0の位置
	g_aLight[0].Position = D3DXVECTOR3(0.0f, 100.0f, 0.0f);

	// ライト0の方向の設定
	vecDir = D3DXVECTOR3(-1.0f, 0.3f, 1.0f);
	D3DXVec3Normalize((D3DXVECTOR3*)&g_aLight[0].Direction, &vecDir);

	// ライト0をレンダリングパイプラインに設定
	pDevice->SetLight(0, &g_aLight[0]);

	// ライト0を使用使用状態に
	pDevice->LightEnable(0, TRUE);


	// D3DLIGHT9構造体を0でクリアする
	ZeroMemory(&g_aLight[1], sizeof(D3DLIGHT9));

	// ライト1のタイプの設定
	g_aLight[1].Type = D3DLIGHT_DIRECTIONAL;

	g_aLight[1].Range = 5000.0f;
	g_aLight[1].Attenuation0 = 0.5f;

	// ライト1の拡散光の設定
	g_aLight[1].Diffuse = D3DXCOLOR(0.75, 0.95f, 0.75f, 1.0f);

	// ライト1の環境光の設定
	g_aLight[1].Ambient = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// ライト1の鏡面反射
	g_aLight[1].Specular = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// ライト1の位置
	g_aLight[1].Position = D3DXVECTOR3(0.0f, 100.0f, 0.0f);

	// ライト1の方向の設定
	vecDir = D3DXVECTOR3(-0.0f, -1.0f, 0.2f);
	D3DXVec3Normalize((D3DXVECTOR3*)&g_aLight[1].Direction, &vecDir);

	// ライト1をレンダリングパイプラインに設定
	pDevice->SetLight(1, &g_aLight[1]);

	// ライト1の設定
	pDevice->LightEnable(1, TRUE);

	// ライティングモードをON
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);


	//g_aLight[0].
}