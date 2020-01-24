//=============================================================================
// フィールド処理 [field.cpp]
// Author : 新里　将士
//
//=============================================================================
#include "FieldClass.h"
#include "model.h"


FieldClass::FieldClass()
{
}


FieldClass::~FieldClass()
{
}


//=============================================================================
// 初期化処理
//=============================================================================
HRESULT FieldClass::InitField(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	HRESULT hr;

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,					// デバイスへのポインタ
		TEXTURE_FIELD,			// ファイルの名前
		&g_pD3DTextureField);	// 読み込むメモリー



	for (int i = 0; i < NUM_FIELD; i++)
	{
		StractdistanceZ[i] = 0;

		// 位置、向きの初期設定
		field[i].m_posModel = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		field[i].m_rotModel = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		field[i].m_posModel.z += 500.0f * i;
	}

	// 頂点情報の作成
	hr = MakeVertexField(pDevice);

	return hr;
}

//=============================================================================
// 終了処理
//=============================================================================
void FieldClass::UninitField(void)
{
	if (g_pD3DTextureField != NULL)
	{// テクスチャの開放
		g_pD3DTextureField->Release();
		g_pD3DTextureField = NULL;
	}

	if (g_pD3DVtxBuffField != NULL)
	{// 頂点バッファの開放
		g_pD3DVtxBuffField->Release();
		g_pD3DVtxBuffField = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void FieldClass::UpdateField(void)
{
	pPlayerField = GetPlayer();

	for (int i = 0; i < NUM_FIELD; i++)
	{

		StractdistanceZ[i] = field[i].m_posModel.z - pPlayerField->m_posModel.z;


		if (StractdistanceZ[i] < 0)
		{
			field[i].m_posModel.z += LIMIT_FIELD_Z;
		}

		if (StractdistanceZ[i] > LIMIT_FIELD_Z)
		{
			field[i].m_posModel.z -= LIMIT_FIELD_Z;
		}

		StractdistanceZ[i] = 0;

	}

}

//=============================================================================
// 描画処理
//=============================================================================
void FieldClass::DrawField(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTranslate;

	// ラインティングを無効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);


	for (int i = 0; i < NUM_FIELD; i++)
	{
		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&field[i].m_mtxWorld);

		// 回転を反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, field[i].m_rotModel.y, field[i].m_rotModel.x, field[i].m_rotModel.z);
		D3DXMatrixMultiply(&field[i].m_mtxWorld, &field[i].m_mtxWorld, &mtxRot);

		// 移動を反映
		D3DXMatrixTranslation(&mtxTranslate, field[i].m_posModel.x, field[i].m_posModel.y, field[i].m_posModel.z);
		D3DXMatrixMultiply(&field[i].m_mtxWorld, &field[i].m_mtxWorld, &mtxTranslate);

		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &field[i].m_mtxWorld);

		// 頂点バッファをデバイスのデータストリームにバインド
		pDevice->SetStreamSource(0, g_pD3DVtxBuffField, 0, sizeof(VERTEX_3D));

		// 頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_3D);

		// テクスチャの設定
		pDevice->SetTexture(0, g_pD3DTextureField);

		// ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);

	}
	// ラインティングを有効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT FieldClass::MakeVertexField(LPDIRECT3DDEVICE9 pDevice)
{
	// オブジェクトの頂点バッファを生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * NUM_VERTEX,	// 頂点データ用に確保するバッファサイズ(バイト単位)
		D3DUSAGE_WRITEONLY,			// 頂点バッファの使用法　
		FVF_VERTEX_3D,				// 使用する頂点フォーマット
		D3DPOOL_MANAGED,			// リソースのバッファを保持するメモリクラスを指定
		&g_pD3DVtxBuffField,		// 頂点バッファインターフェースへのポインタ
		NULL)))						// NULLに設定
	{
		return E_FAIL;
	}

	{//頂点バッファの中身を埋める
		VERTEX_3D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pD3DVtxBuffField->Lock(0, 0, (void**)&pVtx, 0);

		for (int i = 0; i < NUM_FIELD; i++)
		{
			// 頂点座標の設定
			pVtx[0].vtx = D3DXVECTOR3(-500.0f, 0.0f, field[i].m_posModel.z);
			pVtx[1].vtx = D3DXVECTOR3(500.0f, 0.0f, field[i].m_posModel.z);
			pVtx[2].vtx = D3DXVECTOR3(-500.0f, 0.0f, -field[i].m_posModel.z);
			pVtx[3].vtx = D3DXVECTOR3(500.0f, 0.0f, -field[i].m_posModel.z);
		}


		// 法線ベクトルの設定
		pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		// 反射光の設定
		pVtx[0].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		// テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(3.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 3.0f);
		pVtx[3].tex = D3DXVECTOR2(3.0f, 3.0f);

		// 頂点データをアンロックする
		g_pD3DVtxBuffField->Unlock();
	}

	return S_OK;
}

