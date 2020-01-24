//=============================================================================
// �t�B�[���h���� [field.cpp]
// Author : �V���@���m
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
// ����������
//=============================================================================
HRESULT FieldClass::InitField(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	HRESULT hr;

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,					// �f�o�C�X�ւ̃|�C���^
		TEXTURE_FIELD,			// �t�@�C���̖��O
		&g_pD3DTextureField);	// �ǂݍ��ރ������[



	for (int i = 0; i < NUM_FIELD; i++)
	{
		StractdistanceZ[i] = 0;

		// �ʒu�A�����̏����ݒ�
		field[i].m_posModel = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		field[i].m_rotModel = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		field[i].m_posModel.z += 500.0f * i;
	}

	// ���_���̍쐬
	hr = MakeVertexField(pDevice);

	return hr;
}

//=============================================================================
// �I������
//=============================================================================
void FieldClass::UninitField(void)
{
	if (g_pD3DTextureField != NULL)
	{// �e�N�X�`���̊J��
		g_pD3DTextureField->Release();
		g_pD3DTextureField = NULL;
	}

	if (g_pD3DVtxBuffField != NULL)
	{// ���_�o�b�t�@�̊J��
		g_pD3DVtxBuffField->Release();
		g_pD3DVtxBuffField = NULL;
	}
}

//=============================================================================
// �X�V����
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
// �`�揈��
//=============================================================================
void FieldClass::DrawField(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTranslate;

	// ���C���e�B���O�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);


	for (int i = 0; i < NUM_FIELD; i++)
	{
		// ���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&field[i].m_mtxWorld);

		// ��]�𔽉f
		D3DXMatrixRotationYawPitchRoll(&mtxRot, field[i].m_rotModel.y, field[i].m_rotModel.x, field[i].m_rotModel.z);
		D3DXMatrixMultiply(&field[i].m_mtxWorld, &field[i].m_mtxWorld, &mtxRot);

		// �ړ��𔽉f
		D3DXMatrixTranslation(&mtxTranslate, field[i].m_posModel.x, field[i].m_posModel.y, field[i].m_posModel.z);
		D3DXMatrixMultiply(&field[i].m_mtxWorld, &field[i].m_mtxWorld, &mtxTranslate);

		// ���[���h�}�g���b�N�X�̐ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &field[i].m_mtxWorld);

		// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
		pDevice->SetStreamSource(0, g_pD3DVtxBuffField, 0, sizeof(VERTEX_3D));

		// ���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_3D);

		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pD3DTextureField);

		// �|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);

	}
	// ���C���e�B���O��L���ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

}

//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT FieldClass::MakeVertexField(LPDIRECT3DDEVICE9 pDevice)
{
	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * NUM_VERTEX,	// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
		D3DUSAGE_WRITEONLY,			// ���_�o�b�t�@�̎g�p�@�@
		FVF_VERTEX_3D,				// �g�p���钸�_�t�H�[�}�b�g
		D3DPOOL_MANAGED,			// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
		&g_pD3DVtxBuffField,		// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
		NULL)))						// NULL�ɐݒ�
	{
		return E_FAIL;
	}

	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D *pVtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		g_pD3DVtxBuffField->Lock(0, 0, (void**)&pVtx, 0);

		for (int i = 0; i < NUM_FIELD; i++)
		{
			// ���_���W�̐ݒ�
			pVtx[0].vtx = D3DXVECTOR3(-500.0f, 0.0f, field[i].m_posModel.z);
			pVtx[1].vtx = D3DXVECTOR3(500.0f, 0.0f, field[i].m_posModel.z);
			pVtx[2].vtx = D3DXVECTOR3(-500.0f, 0.0f, -field[i].m_posModel.z);
			pVtx[3].vtx = D3DXVECTOR3(500.0f, 0.0f, -field[i].m_posModel.z);
		}


		// �@���x�N�g���̐ݒ�
		pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		// ���ˌ��̐ݒ�
		pVtx[0].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		// �e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(3.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 3.0f);
		pVtx[3].tex = D3DXVECTOR2(3.0f, 3.0f);

		// ���_�f�[�^���A�����b�N����
		g_pD3DVtxBuffField->Unlock();
	}

	return S_OK;
}

