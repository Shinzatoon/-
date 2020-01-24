//=============================================================================
// 地面処理 [field.h]
// Author : 新里　将士
//
//=============================================================================
#pragma once

#include "main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	TEXTURE_FIELD	"data/TEXTURE/field004.jpg"	// 読み込むテクスチャファイル名
#define NUM_FIELD (3)								//フィールドの枚数	
#define LIMIT_FIELD_Z (3 * 500.0f)					//フィールドの切り替え点

//*****************************************************************************
// クラス宣言
//*****************************************************************************
class FieldClass
{
private:
	LPDIRECT3DTEXTURE9		g_pD3DTextureField = NULL;	// テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuffField = NULL;	// 頂点バッファへのポインタ

	D3DXMATRIX				g_mtxWorldField;			// ワールドマトリックス
	D3DXVECTOR3				g_posField;					// 現在の位置
	D3DXVECTOR3				g_rotField;					// 現在の向き

	float					StractdistanceZ[NUM_FIELD];			//PlayerとFieldのZ距離減算結果の格納変数

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

