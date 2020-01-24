//============================================================================
// �Ə����� [reticle.cpp]
// Author : �V���@���m
//
//=============================================================================

#include "reticle.h"
#include "input.h"
#include "model.h"
#include "CameraClass.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	TEXTURE_RETICLE		"data/TEXTURE/�^�[�Q�b�g�̃A�C�R��.png"	// �ǂݍ��ރe�N�X�`���t�@�C����
#define	RETICLE_SIZE_X		(20.0f)							// �r���{�[�h�̕�
#define	RETICLE_SIZE_Y		(20.0f)							// �r���{�[�h�̍���

#define MAX_RETICLE			(1)								//�r���{�[�h�̍ő吔


//*****************************************************************************
// �\���̒�`
//*****************************************************************************



//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT MakeVertexReticle(LPDIRECT3DDEVICE9 pDevice);
void SetVertexReticle(int nIdxReticle, float fSizeX, float fSizeY);
void SetColorReticle(int nIdxReticle, D3DXCOLOR col);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pD3DTextureReticle = NULL;	// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuffReticle = NULL;	// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^


RETICLE		g_aReticle;				// ���e�B�N�����[�N

OBJECT3D	*pModel2;								

//CAMERA		*ReticleCamera;							//�J�����̏��
OBJECT3D	*ParentModel2;							//���f���Q�̏��@�y�A�����g����

D3DXVECTOR3		*PlayerVector;
D3DXVECTOR3		*PlayerLength;


//============================================================
// ������
//==============================================================
HRESULT InitReticle()
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	ParentModel2 = GetPlayer2();
	pModel2 = GetPlayer2();

	// ���_���̍쐬
	MakeVertexReticle(pDevice);
	
	//CameraClass camera;


	//Reticle.m_mtxWorld = Reticle.m_mtxWorld * ParentModel2->m_mtxWorld;
	//Reticle.m_posModel = D3DXVECTOR3(0.0f, 50.0f, 0.0f);

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,						// �f�o�C�X�ւ̃|�C���^
		TEXTURE_RETICLE,			// �t�@�C���̖��O
		&g_pD3DTextureReticle);	// �ǂݍ��ރ������[


		g_aReticle.pos = pModel2->m_posModel;	//������model�Q��pos
		g_aReticle.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aReticle.scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		g_aReticle.move = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		g_aReticle.col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		g_aReticle.fSizeX = RETICLE_SIZE_X;
		g_aReticle.fSizeY = RETICLE_SIZE_Y;
		g_aReticle.nIdxShadow = -1;
		g_aReticle.nLife = 0;
		g_aReticle.bUse = true;
		g_aReticle.posBase = D3DXVECTOR3(0.0f, 0.0f, 0.0f);





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
void DrawReticle(D3DXMATRIX cameraView)
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
	
		if (g_aReticle.bUse)
		{
			// ���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_aReticle.g_mtxWorldReticle);

			// �r���[�}�g���b�N�X���擾
			mtxView = cameraView;

			// �|���S���𐳖ʂɌ����邽�߃J�����̓]�u�s����R�s�[
			g_aReticle.g_mtxWorldReticle._11 = mtxView._11;
			g_aReticle.g_mtxWorldReticle._12 = mtxView._21;
			g_aReticle.g_mtxWorldReticle._13 = mtxView._31;
			g_aReticle.g_mtxWorldReticle._21 = mtxView._12;
			g_aReticle.g_mtxWorldReticle._22 = mtxView._22;
			g_aReticle.g_mtxWorldReticle._23 = mtxView._32;
			g_aReticle.g_mtxWorldReticle._31 = mtxView._13;
			g_aReticle.g_mtxWorldReticle._32 = mtxView._23;
			g_aReticle.g_mtxWorldReticle._33 = mtxView._33;

			// �X�P�[���𔽉f
			D3DXMatrixScaling(&mtxScale, g_aReticle.scale.x, g_aReticle.scale.y, g_aReticle.scale.z);
			D3DXMatrixMultiply(&g_aReticle.g_mtxWorldReticle, &g_aReticle.g_mtxWorldReticle, &mtxScale);

			// �ړ��𔽉f
			D3DXMatrixTranslation(&mtxTranslate, g_aReticle.pos.x, g_aReticle.pos.y, g_aReticle.pos.z);
			D3DXMatrixMultiply(&g_aReticle.g_mtxWorldReticle, &g_aReticle.g_mtxWorldReticle, &mtxTranslate);


			// ���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_aReticle.g_mtxWorldReticle);

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
//�@���e�B�N���̍X�V
//==============================================================
void UpdateReticle()
{
	
		PlayerVector = GetReticleVector();		//���e�B�N���̕�����C��ƃ����N����
		PlayerLength = GetReticleLength();		//���e�B�N���̏ꏊ��C��ƃ����N����
	

		g_aReticle.rot.x = PlayerVector->x;
		g_aReticle.rot.y = PlayerVector->y;
		g_aReticle.rot.z = PlayerVector->z;
		
		g_aReticle.pos.x = PlayerLength->x;
		g_aReticle.pos.y = PlayerLength->y;
		g_aReticle.pos.z = PlayerLength->z;
		g_aReticle.pos = g_aReticle.pos + g_aReticle.rot * 300.0f;
	
}

//==============================================================
//�@���e�B�N���̌�n��
//==============================================================
void UninitReticle()
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
HRESULT MakeVertexReticle(LPDIRECT3DDEVICE9 pDevice)
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
void SetVertexReticle(int nIdxReticle, float fSizeX, float fSizeY)
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
void SetColorReticle(int nIdxReticle, D3DXCOLOR col)
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
int SetReticle(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col, float fSizeX, float fSizeY, int nLife)
{
	int nIdxReticle = -1;

	for (int nCntReticle = 0; nCntReticle < MAX_RETICLE; nCntReticle++)
	{	//���g�p�̍\���̂փZ�b�g
		if (!g_aReticle.bUse)
		{
			g_aReticle.pos = pos;
			g_aReticle.rot = rot;
			g_aReticle.scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
			g_aReticle.move = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
			g_aReticle.col = col;
			g_aReticle.fSizeX = fSizeX;
			g_aReticle.fSizeY = fSizeY;
			g_aReticle.nLife = nLife;
			g_aReticle.bUse = true;

			// ���_���W�̐ݒ�
			SetVertexReticle(nCntReticle, fSizeX, fSizeY);
			SetColorReticle(nCntReticle, col);

			nIdxReticle = nCntReticle;

#ifdef DISP_SHADOW
			// �e�̐ݒ�	//�����̏ꍇ�͉e�͂���Ȃ�
//			g_aParticle[nCntParticle].nIdxShadow = SetShadow(D3DXVECTOR3(pos.x, 0.1f, pos.z), fSizeX, fSizeY);		// �e�̐ݒ�
//			if(g_aParticle[nCntParticle].nIdxShadow != -1)
//			{
//				SetColorShadow(g_aParticle[nCntParticle].nIdxShadow, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f));
//			}
#endif

			break;
		}
	}

	return nIdxReticle;
}


RETICLE *GetReticle(void)
{
	return(&g_aReticle);
}