//============================================================================
// �Ə����� [reticle.cpp]
// Author : �V���@���m
//
//=============================================================================
#include "ReticleClass.h"
#include "input.h"
#include "CameraClass.h"
#include "model.h"


ReticleClass::ReticleClass()
{
}


ReticleClass::~ReticleClass()
{
}

//============================================================
// ������
//==============================================================
HRESULT ReticleClass::InitReticle()
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	ParentModel2 = GetPlayer2();
	pModel2 = GetPlayer2();

	// ���_���̍쐬
	MakeVertexReticle(pDevice);




	//Reticle.m_mtxWorld = Reticle.m_mtxWorld * ParentModel2->m_mtxWorld;
	//Reticle.m_posModel = D3DXVECTOR3(0.0f, 50.0f, 0.0f);

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,						// �f�o�C�X�ւ̃|�C���^
		TEXTURE_RETICLE,			// �t�@�C���̖��O
		&g_pD3DTextureReticle);	// �ǂݍ��ރ������[


	reticlePos = pModel2->m_posModel;	//������model�Q��pos
	reticleRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	reticleScale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	reticleMove = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	reticleCol = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	reticlefSizeX = RETICLE_SIZE_X;
	reticlefSizeY = RETICLE_SIZE_Y;
	reticleIdxShadow = -1;
	reticleLife = 0;
	reticleUse = true;
	posBase = D3DXVECTOR3(0.0f, 0.0f, 0.0f);





	return S_OK;


	//�\���̂𖢎g�p�ɂ���
	//for (int i = 0; i < NUM_BULLET; i++)
	//{
	//	Bullet[i].m_bUse = FALSE;
	//	Bullet[i].m_Radius = D3DXVECTOR3(10.0f, 10.0f, 10.0f);//�����蔻��T�C�Y
	//}



}



//==============================================================
//�@���e�B�N���̕`��
//==============================================================
void ReticleClass::DrawReticle(D3DXMATRIX cameraView)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxView, mtxScale, mtxTranslate;

	D3DXVECTOR3 temp;

	//ReticleCamera = GetMtxView();
	//CameraClass camera;

	ParentModel2 = GetPlayer2();

	// ���e�X�g��L����

	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);		//FALSE��OFF

	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);

	pDevice->SetRenderState(D3DRS_ALPHAREF, 0x0);



	// ���e�X�g�𖳌���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);



	// ���C�e�B���O�𖳌���
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// �u�����h��\���������ݒ�ɂȂ�悤�ɂ���
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);		// ���\�[�X�J���[�̎w��
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);	// ���\�[�X�J���[�̎w��
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);		// ���f�X�e�B�l�[�V�����J���[�̎w��
	pDevice->SetRenderState(D3DRS_ZENABLE, FALSE);//���s�����l���Ȃ�

	//g_aReticle.g_mtxProvisional = g_aReticle.g_mtxWorldReticle;

	if (reticleUse)
	{
		// ���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&g_mtxWorldReticle);

		// �r���[�}�g���b�N�X���擾
		//mtxView = camera->g_mtxView;




		// �|���S���𐳖ʂɌ����邽�߃J�����̓]�u�s����R�s�[
		g_mtxWorldReticle._11 = cameraView._11;
		g_mtxWorldReticle._12 = cameraView._21;
		g_mtxWorldReticle._13 = cameraView._31;
		g_mtxWorldReticle._21 = cameraView._12;
		g_mtxWorldReticle._22 = cameraView._22;
		g_mtxWorldReticle._23 = cameraView._32;
		g_mtxWorldReticle._31 = cameraView._13;
		g_mtxWorldReticle._32 = cameraView._23;
		g_mtxWorldReticle._33 = cameraView._33;

		// �X�P�[���𔽉f
		D3DXMatrixScaling(&mtxScale, reticleScale.x, reticleScale.y, reticleScale.z);
		D3DXMatrixMultiply(&g_mtxWorldReticle, &g_mtxWorldReticle, &mtxScale);

		// �ړ��𔽉f
		D3DXMatrixTranslation(&mtxTranslate, reticlePos.x, reticlePos.y, reticlePos.z);
		D3DXMatrixMultiply(&g_mtxWorldReticle, &g_mtxWorldReticle, &mtxTranslate);


		// ���[���h�}�g���b�N�X�̐ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldReticle);

		// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
		pDevice->SetStreamSource(0, g_pD3DVtxBuffReticle, 0, sizeof(VERTEX_3D));

		// ���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_3D);

		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pD3DTextureReticle);

		// �|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);

	}


	// ���C���e�B���O��L���ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	// �ʏ�u�����h�ɖ߂�
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// ���\�[�X�J���[�̎w��
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	// ���f�X�e�B�l�[�V�����J���[�̎w��
	pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);



}

//============================================================
//
//�@���e�B�N���̍X�V
//==============================================================
void ReticleClass::UpdateReticle()
{

	PlayerVector = GetReticleVector();		//���e�B�N���̕�����C��ƃ����N����
	PlayerLength = GetReticleLength();		//���e�B�N���̏ꏊ��C��ƃ����N����


	reticleRot.x = PlayerVector->x;
	reticleRot.y = PlayerVector->y;
	reticleRot.z = PlayerVector->z;

	reticlePos.x = PlayerLength->x;
	reticlePos.y = PlayerLength->y;
	reticlePos.z = PlayerLength->z;
	reticlePos = reticlePos + reticleRot * 300.0f;

}

//==============================================================
//�@���e�B�N���̌�n��
//==============================================================
void ReticleClass::UninitReticle()
{
	if (g_pD3DTextureReticle != NULL)
	{// �e�N�X�`���̊J��
		g_pD3DTextureReticle->Release();
		g_pD3DTextureReticle = NULL;
	}

	if (g_pD3DVtxBuffReticle != NULL)
	{// ���_�o�b�t�@�̊J��
		g_pD3DVtxBuffReticle->Release();
		g_pD3DVtxBuffReticle = NULL;
	}
}

//====================================================
//���_���̍쐬
//====================================================
HRESULT ReticleClass::MakeVertexReticle(LPDIRECT3DDEVICE9 pDevice)
{
	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * NUM_VERTEX,	// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
		D3DUSAGE_WRITEONLY,			// ���_�o�b�t�@�̎g�p�@�@
		FVF_VERTEX_3D,				// �g�p���钸�_�t�H�[�}�b�g
		D3DPOOL_MANAGED,			// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
		&g_pD3DVtxBuffReticle,	// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
		NULL)))						// NULL�ɐݒ�
	{
		return E_FAIL;
	}

	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D *pVtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		g_pD3DVtxBuffReticle->Lock(0, 0, (void**)&pVtx, 0);

		// ���_���W�̐ݒ�
		pVtx[0].vtx = D3DXVECTOR3(-RETICLE_SIZE_X / 2, -RETICLE_SIZE_Y / 2, 0.0f);
		pVtx[1].vtx = D3DXVECTOR3(-RETICLE_SIZE_X / 2, RETICLE_SIZE_Y / 2, 0.0f);
		pVtx[2].vtx = D3DXVECTOR3(RETICLE_SIZE_X / 2, -RETICLE_SIZE_Y / 2, 0.0f);
		pVtx[3].vtx = D3DXVECTOR3(RETICLE_SIZE_X / 2, RETICLE_SIZE_Y / 2, 0.0f);

		// �@���̐ݒ�
		pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, 1.0f);

		// ���ˌ��̐ݒ�
		pVtx[0].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		// �e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		// ���_�f�[�^���A�����b�N����
		g_pD3DVtxBuffReticle->Unlock();
	}

	return S_OK;
}

//=============================================================================
// ���_���W�̐ݒ�
//=============================================================================
void ReticleClass::SetVertexReticle(int nIdxReticle, float fSizeX, float fSizeY)
{
	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D *pVtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		g_pD3DVtxBuffReticle->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (nIdxReticle * 4);

		// ���_���W�̐ݒ�
		pVtx[0].vtx = D3DXVECTOR3(-fSizeX / 2, -fSizeY / 2, 0.0f);
		pVtx[1].vtx = D3DXVECTOR3(-fSizeX / 2, fSizeY / 2, 0.0f);
		pVtx[2].vtx = D3DXVECTOR3(fSizeX / 2, -fSizeY / 2, 0.0f);
		pVtx[3].vtx = D3DXVECTOR3(fSizeX / 2, fSizeY / 2, 0.0f);

		// ���_�f�[�^���A�����b�N����
		g_pD3DVtxBuffReticle->Unlock();
	}
}


//=============================================================================
// ���_�J���[�̐ݒ�
//=============================================================================
void ReticleClass::SetColorReticle(int nIdxReticle, D3DXCOLOR col)
{
	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D *pVtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		g_pD3DVtxBuffReticle->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (nIdxReticle * 4);

		// ���_���W�̐ݒ�
		pVtx[0].diffuse =
			pVtx[1].diffuse =
			pVtx[2].diffuse =
			pVtx[3].diffuse = col;

		// ���_�f�[�^���A�����b�N����
		g_pD3DVtxBuffReticle->Unlock();
	}
}

//=============================================================================
// ���_���̍쐬
//=============================================================================
int ReticleClass::SetReticle(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col, float fSizeX, float fSizeY, int nLife)
{
	int nIdxReticle = -1;

	for (int nCntReticle = 0; nCntReticle < MAX_RETICLE; nCntReticle++)
	{	//���g�p�̍\���̂փZ�b�g
		if (!reticleUse)
		{
			reticlePos = pos;
			reticleRot = rot;
			reticleScale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
			reticleMove = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
			reticleCol = col;
			reticlefSizeX = fSizeX;
			reticlefSizeY = fSizeY;
			reticleLife = nLife;
			reticleUse = true;

			// ���_���W�̐ݒ�
			SetVertexReticle(nCntReticle, fSizeX, fSizeY);
			SetColorReticle(nCntReticle, col);

			nIdxReticle = nCntReticle;

			break;
		}
	}

	return nIdxReticle;
}


//RETICLE *GetReticle(void)
//{
//	return(&g_aReticle);
//}