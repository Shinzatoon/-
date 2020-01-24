//=============================================================================
// �G�l�~�[�̒e���� [enemyBullet.cpp]
// Author : �V���@���m
//
//=============================================================================

#include "enemyBullet.h"
#include "input.h"
#include "model.h"
#include "CameraClass.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	TEXTURE_BILLBOARD		"data/TEXTURE/EffectNew.png"	// �ǂݍ��ރe�N�X�`���t�@�C����
#define	ENEMY_BULLET_SIZE_X		(10.0f)							// �r���{�[�h�̕�
#define	ENEMY_BULLET_SIZE_Y		(10.0f)							// �r���{�[�h�̍���

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT MakeVertexEnemyBullet(LPDIRECT3DDEVICE9 pDevice);



//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pD3DTextureEnemyBullet = NULL;	// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuffEnemyBullet = NULL;	// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^


int			ecount = 0;							//�e�̕`�掞�ԃJ�E���g



OBJECT3D	enemyBullet[NUM_ENEMY_BULLET];		//�e�@�\���̏���main.h����Q��



//============================================================
// ������
//==============================================================
void InitEnemyBullet()
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//BazrelativeVector = D3DXVECTOR3(0.0f, 0.0f, 1.0f);

	// ���_���̍쐬
	MakeVertexEnemyBullet(pDevice);

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,						// �f�o�C�X�ւ̃|�C���^
		TEXTURE_BILLBOARD,			// �t�@�C���̖��O
		&g_pD3DTextureEnemyBullet);	// �ǂݍ��ރ������[


	//�\���̂𖢎g�p�ɂ���
	for (int i = 0; i < NUM_ENEMY_BULLET; i++)
	{
		enemyBullet[i].m_bUse = FALSE;
		enemyBullet[i].m_Radius = D3DXVECTOR3(5.0f, 5.0f, 5.0f);//�����蔻��T�C�Y
	}



}

//==============================================================
//�@�e�̕`��
//==============================================================
void DrawEnemyBullet(D3DXMATRIX cameraView)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxView, mtxScale, mtxTranslate;

	//enemyBulletcameraWK = GetMtxView();
	//CameraClass camera;


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
	for (int i = 0; i < NUM_ENEMY_BULLET; i++)
	{	//�g�p���̍\���݂̂̂𖢏���
		if (enemyBullet[i].m_bUse)
		{
			// ���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&enemyBullet[i].m_mtxWorld);


			// �|���S���𐳖ʂɌ����邽�߃J�����̓]�u�s����R�s�[
			enemyBullet[i].m_mtxWorld._11 = cameraView._11;
			enemyBullet[i].m_mtxWorld._12 = cameraView._21;
			enemyBullet[i].m_mtxWorld._13 = cameraView._31;
			enemyBullet[i].m_mtxWorld._21 = cameraView._12;
			enemyBullet[i].m_mtxWorld._22 = cameraView._22;
			enemyBullet[i].m_mtxWorld._23 = cameraView._32;
			enemyBullet[i].m_mtxWorld._31 = cameraView._13;
			enemyBullet[i].m_mtxWorld._32 = cameraView._23;
			enemyBullet[i].m_mtxWorld._33 = cameraView._33;


			// �ړ��𔽉f
			D3DXMatrixTranslation(&mtxTranslate, enemyBullet[i].m_posModel.x,
				enemyBullet[i].m_posModel.y,
				enemyBullet[i].m_posModel.z);
			D3DXMatrixMultiply(&enemyBullet[i].m_mtxWorld,
				&enemyBullet[i].m_mtxWorld, &mtxTranslate);

			// ���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &enemyBullet[i].m_mtxWorld);

			// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
			pDevice->SetStreamSource(0, g_pD3DVtxBuffEnemyBullet, 0, sizeof(VERTEX_3D));

			// ���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_3D);

			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pD3DTextureEnemyBullet);

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
void UpdateEnemyBullet()
{
	//���ׂĂ̒e�̍X�V
	for (int i = 0; i < NUM_ENEMY_BULLET; i++)
	{
		//�g�p���̍\���̂̂ݏ���
		if (enemyBullet[i].m_bUse)
		{
			ecount++;
			//�ړ�
			enemyBullet[i].m_posModel += enemyBullet[i].m_DirSpeed;
			//��ʊO�`�F�b�N
			if (ecount == 60)
			{
				enemyBullet[i].m_bUse = FALSE;
				ecount = 0;
			}
		}
	}
}

//==============================================================
//�@�e�̌�n��
//==============================================================
void UninitEnemyBullet()
{
	if (g_pD3DTextureEnemyBullet != NULL)
	{// �e�N�X�`���̊J��
		g_pD3DTextureEnemyBullet->Release();
		g_pD3DTextureEnemyBullet = NULL;
	}

	if (g_pD3DVtxBuffEnemyBullet != NULL)
	{// ���_�o�b�t�@�̊J��
		g_pD3DVtxBuffEnemyBullet->Release();
		g_pD3DVtxBuffEnemyBullet = NULL;
	}
}

//==============================================================
//�@�e�̍쐬�i���ˁj�@�����������I�u�W�F�N�g���Ăяo��
//==============================================================
void CreateEnemyBullet(D3DXVECTOR3 pos, D3DXVECTOR3 speed)
{
	//�e���쐬
	for (int i = 0; i < NUM_ENEMY_BULLET; i++)
	{
		//���g�p�̍\���̂�T���ď�����
		if (enemyBullet[i].m_bUse == FALSE)
		{
			enemyBullet[i].m_posModel = pos;
			enemyBullet[i].m_DirSpeed = speed;

			enemyBullet[i].m_bUse = TRUE;//�g�p���ɂ���
			break;//�쐬�I��
		}
	}
}

//============================================================
//�@�e�|���S���̍쐬  �|���S���f�[�^��1���̂ݍ쐬
//==============================================================
HRESULT MakeVertexEnemyBullet(LPDIRECT3DDEVICE9 pDevice)
{
	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * NUM_VERTEX,	// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
		D3DUSAGE_WRITEONLY,			// ���_�o�b�t�@�̎g�p�@�@
		FVF_VERTEX_3D,				// �g�p���钸�_�t�H�[�}�b�g
		D3DPOOL_MANAGED,			// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
		&g_pD3DVtxBuffEnemyBullet,	// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
		NULL)))						// NULL�ɐݒ�
	{
		return E_FAIL;
	}

	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D *pVtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		g_pD3DVtxBuffEnemyBullet->Lock(0, 0, (void**)&pVtx, 0);

		// ���_���W�̐ݒ�
		pVtx[0].vtx = D3DXVECTOR3(-ENEMY_BULLET_SIZE_X / 2, -ENEMY_BULLET_SIZE_Y / 2, 0.0f);
		pVtx[1].vtx = D3DXVECTOR3(-ENEMY_BULLET_SIZE_X / 2, ENEMY_BULLET_SIZE_Y / 2, 0.0f);
		pVtx[2].vtx = D3DXVECTOR3(ENEMY_BULLET_SIZE_X / 2, -ENEMY_BULLET_SIZE_Y / 2, 0.0f);
		pVtx[3].vtx = D3DXVECTOR3(ENEMY_BULLET_SIZE_X / 2, ENEMY_BULLET_SIZE_Y / 2, 0.0f);

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
		g_pD3DVtxBuffEnemyBullet->Unlock();
	}

	return S_OK;
}


//==================================
//�e�\���̂̐擪�|�C���^��Ԃ�
//================================
OBJECT3D* GetEnemyBullet()
{
	return &enemyBullet[0];
}

OBJECT3D* GetParentEnemyBullet(void)
{
	return &enemyBullet[NUM_ENEMY_BULLET];
}