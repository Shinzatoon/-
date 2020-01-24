//=============================================================================
// �e���� [bullet.cpp]
// Author : �V���@���m
//
//=============================================================================

#include "bullet.h"
#include "input.h"
#include "model.h"
#include "CameraClass.h"
#include "ReticleClass.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	TEXTURE_BILLBOARD		"data/TEXTURE/effect09.png"	//�ǂݍ��ރe�N�X�`���t�@�C����
#define	BULLET_SIZE_X		(10.0f)							//�r���{�[�h�̕�
#define	BULLET_SIZE_Y		(10.0f)							//�r���{�[�h�̍���
#define MAX_RANGE			(500.0f)						//�e�̎˒�

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT MakeVertexBullet(LPDIRECT3DDEVICE9 pDevice);



//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pD3DTextureBullet = NULL;	// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuffBullet = NULL;	// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^

int						g_nIdxShadowBullet;			// �eID

OBJECT3D	Bullet[NUM_BULLET];		//�e�@�\���̏���main.h����Q��
OBJECT3D	*b_player;

D3DXVECTOR3 bulletFiringPoint[NUM_BULLET];	//�e�̔��˒n�_
D3DXVECTOR3 bulletCurrentPoint[NUM_BULLET];	//�e�̌��݈ʒu
float limitLength[NUM_BULLET];				//�e�̐i�s����
//============================================================
// ������
//==============================================================
void InitBullet(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//BazrelativeVector = D3DXVECTOR3(0.0f, 0.0f, 1.0f);

	// ���_���̍쐬
	MakeVertexBullet(pDevice);

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,						// �f�o�C�X�ւ̃|�C���^
		TEXTURE_BILLBOARD,			// �t�@�C���̖��O
		&g_pD3DTextureBullet);	// �ǂݍ��ރ������[


	//�\���̂𖢎g�p�ɂ���
	for (int i = 0; i < NUM_BULLET; i++)
	{
		Bullet[i].m_bUse = FALSE;
		Bullet[i].m_Radius = D3DXVECTOR3(5.0f, 5.0f, 5.0f);//�����蔻��T�C�Y
	}
}

//==============================================================
//�@�e�̕`��
//==============================================================
void DrawBullet(D3DXMATRIX cameraView)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxView, mtxScale, mtxTranslate;
	

	// ���e�X�g��L����
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);		//FALSE��OFF
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0x0);

	// ���e�X�g�𖳌���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);



	// ���C�e�B���O�𖳌���
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// �u�����h��\���������ݒ�ɂȂ�悤�ɂ���
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);		// ���\�[�X�J���[�̎w��
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);	// ���\�[�X�J���[�̎w��
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);		// ���f�X�e�B�l�[�V�����J���[�̎w��
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);//���s�����l���Ȃ�

	// �r���[�}�g���b�N�X���擾
	//mtxView = camera->g_mtxView;

	//mtxView = GetMtxView();

	//�e����
	for (int i = 0; i < NUM_BULLET; i++)
	{	//�g�p���̍\���݂̂̂𖢏���
		if (Bullet[i].m_bUse)
		{
			// ���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&Bullet[i].m_mtxWorld);

			// �|���S���𐳖ʂɌ����邽�߃J�����̓]�u�s����R�s�[
			Bullet[i].m_mtxWorld._11 = cameraView._11;
			Bullet[i].m_mtxWorld._12 = cameraView._21;
			Bullet[i].m_mtxWorld._13 = cameraView._31;
			Bullet[i].m_mtxWorld._21 = cameraView._12;
			Bullet[i].m_mtxWorld._22 = cameraView._22;
			Bullet[i].m_mtxWorld._23 = cameraView._32;
			Bullet[i].m_mtxWorld._31 = cameraView._13;
			Bullet[i].m_mtxWorld._32 = cameraView._23;
			Bullet[i].m_mtxWorld._33 = cameraView._33;

			// �ړ��𔽉f
			D3DXMatrixTranslation(&mtxTranslate, Bullet[i].m_posModel.x,
				Bullet[i].m_posModel.y,
				Bullet[i].m_posModel.z);
			D3DXMatrixMultiply(&Bullet[i].m_mtxWorld,
				&Bullet[i].m_mtxWorld, &mtxTranslate);

			// ���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &Bullet[i].m_mtxWorld);

			// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
			pDevice->SetStreamSource(0, g_pD3DVtxBuffBullet, 0, sizeof(VERTEX_3D));

			// ���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_3D);

			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pD3DTextureBullet);

			// �|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);
		}
	}
	// ���C���e�B���O��L���ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	// �ʏ�u�����h�ɖ߂�
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// ���\�[�X�J���[�̎w��
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	// ���f�X�e�B�l�[�V�����J���[�̎w��
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

}

//============================================================
//�@�e�̍X�V
//==============================================================
void UpdateBullet(void)
{
	b_player = GetPlayer2();




	//���ׂĂ̒e�̍X�V
	for (int i = 0; i < NUM_BULLET; i++)
	{
		if (GetKeyboardPress(DIK_SPACE))
		{
			bulletFiringPoint[i] = Bullet[i].m_posModel;
		}
		//�g�p���̍\���̂̂ݏ���
		if (Bullet[i].m_bUse)
		{

			bulletCurrentPoint[i] = Bullet[i].m_posModel - bulletFiringPoint[i];
			limitLength[i] = D3DXVec3Length(&bulletCurrentPoint[i]);

			//�ړ�
			Bullet[i].m_posModel += Bullet[i].m_DirSpeed;
			//��ʊO�`�F�b�N
			if (MAX_RANGE < limitLength[i])
			{
				Bullet[i].m_bUse = FALSE;
			}
		}
	}
}


//==============================================================
//�@�e�̌�n��
//==============================================================
void UninitBullet(void)
{
	if (g_pD3DTextureBullet != NULL)
	{// �e�N�X�`���̊J��
		g_pD3DTextureBullet->Release();
		g_pD3DTextureBullet = NULL;
	}

	if (g_pD3DVtxBuffBullet != NULL)
	{// ���_�o�b�t�@�̊J��
		g_pD3DVtxBuffBullet->Release();
		g_pD3DVtxBuffBullet = NULL;
	}
}

//==============================================================
//�@�e�̍쐬�i���ˁj�@�����������I�u�W�F�N�g���Ăяo��
//==============================================================
void CreateBullet(D3DXVECTOR3 pos, D3DXVECTOR3 speed)
{
	//�e���쐬
	for (int i = 0; i < NUM_BULLET; i++)
	{
		//���g�p�̍\���̂�T���ď�����
		if (Bullet[i].m_bUse == FALSE)
		{
			Bullet[i].m_posModel = pos;
			Bullet[i].m_DirSpeed = speed;

			Bullet[i].m_bUse = TRUE;//�g�p���ɂ���

			break;//�쐬�I��
		}
	}
}

//============================================================
//
//�@�e�|���S���̍쐬  �|���S���f�[�^��1���̂ݍ쐬
//==============================================================
HRESULT MakeVertexBullet(LPDIRECT3DDEVICE9 pDevice)
{
	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * NUM_VERTEX,	// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
		D3DUSAGE_WRITEONLY,			// ���_�o�b�t�@�̎g�p�@�@
		FVF_VERTEX_3D,				// �g�p���钸�_�t�H�[�}�b�g
		D3DPOOL_MANAGED,			// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
		&g_pD3DVtxBuffBullet,	// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
		NULL)))						// NULL�ɐݒ�
	{
		return E_FAIL;
	}

	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D *pVtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		g_pD3DVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

		// ���_���W�̐ݒ�
		pVtx[0].vtx = D3DXVECTOR3(-BULLET_SIZE_X / 2, -BULLET_SIZE_Y / 2, 0.0f);
		pVtx[1].vtx = D3DXVECTOR3(-BULLET_SIZE_X / 2, BULLET_SIZE_Y / 2, 0.0f);
		pVtx[2].vtx = D3DXVECTOR3(BULLET_SIZE_X / 2, -BULLET_SIZE_Y / 2, 0.0f);
		pVtx[3].vtx = D3DXVECTOR3(BULLET_SIZE_X / 2, BULLET_SIZE_Y / 2, 0.0f);

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
		g_pD3DVtxBuffBullet->Unlock();
	}

	return S_OK;
}


//==================================
//�e�\���̂̐擪�|�C���^��Ԃ�
//================================
OBJECT3D* GetBullet()
{
	return &Bullet[0];
}

OBJECT3D* GetParentBullet(void)
{
	return &Bullet[NUM_BULLET];
}
