//============================================================================
// �G�l�~�[B���� [enemy_B.cpp]
// Author : �V���@���m
//
//=============================================================================
#include "enemy_B.h"
#include "enemy_A.h"
#include "model.h"
#include "bullet.h"
#include "enemyBullet.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	MODEL_ENEMY_B		"data/MODEL/BeeEnemy.x"							// �ǂݍ��ރ��f����
#define TEXTURE_ENEMYFILE_B "data/TEXTURE/side_arrow.png"
#define	ENEMY_MOVE		(2.0f)											// �ړ���
#define	ENEMY_ROTATE	(D3DX_PI * 0.02f)								// ��]��
#define NUM_ENEMY_B		(30)

int					enemyB_wave1;

int					beforeWaveFlag;

OBJECT3D			B_enemyWK[NUM_ENEMY_B];

D3DXVECTOR3			BeeToDistancePlayer[NUM_ENEMY_B];
D3DXVECTOR3			BeeEnemyBulletSpeed[NUM_ENEMY_B];

OBJECT3D			*enemyMantis;

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9	eb_pD3DTextureModel;		// �e�N�X�`���ւ̃|�C���^
LPD3DXMESH			eb_pD3DXMeshSphere;
LPD3DXMESH			eb_pD3DXMeshModel;			// ���b�V�����ւ̃|�C���^�@�ǂݍ���X�t�@�C�����f����\���B���f���𑝂₵���������炱��𑝂₷�B
LPD3DXBUFFER		eb_pD3DXBuffMatModel;		// �}�e���A�����ւ̃|�C���^�@�ǂݍ���X�t�@�C�����f���̃}�e���A����\���B
DWORD				eb_nNumMatModel;			// �}�e���A�����̐��@�ǂݍ���X�t�@�C�����f���̃}�e���A���̐��B


//=============================================================================
// ����������
//=============================================================================
HRESULT InitEnemy_B(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	enemyMantis = GetPlayer();

	eb_pD3DTextureModel = NULL;
	eb_pD3DXMeshModel = NULL;
	eb_pD3DXBuffMatModel = NULL;
	eb_nNumMatModel = 0;

	// X�t�@�C���̓ǂݍ���
	if (FAILED(D3DXLoadMeshFromX(MODEL_ENEMY_B,				// �ǂݍ��ރ��f���t�@�C����(X�t�@�C��)�@����������������Α��̂��\���ł���B
		D3DXMESH_SYSTEMMEM,		// ���b�V���̍쐬�I�v�V�������w��
		pDevice,				// IDirect3DDevice9�C���^�[�t�F�C�X�ւ̃|�C���^
		NULL,					// �אڐ��f�[�^���܂ރo�b�t�@�ւ̃|�C���^
		&eb_pD3DXBuffMatModel,	// �}�e���A���f�[�^���܂ރo�b�t�@�ւ̃|�C���^	�}�e���A���Ǘ��@	�ǂݍ���X�t�@�C�����f���̏�񂪂��ꂼ��i�[����Ă���
		NULL,					// �G�t�F�N�g�C���X�^���X�̔z����܂ރo�b�t�@�ւ̃|�C���^							
		&eb_nNumMatModel,		// D3DXMATERIAL�\���̂̐�															//
		&eb_pD3DXMeshModel)))	// ID3DXMesh�C���^�[�t�F�C�X�ւ̃|�C���^�̃A�h���X									//
	{
		return E_FAIL;
	}

	

	D3DXVECTOR3 initPos[NUM_ENEMY_B] =
	{
		D3DXVECTOR3(100,50,0),
		D3DXVECTOR3(100,50,0),
		D3DXVECTOR3(0,50,0),
		D3DXVECTOR3(-100,50,0),
		D3DXVECTOR3(-100,50,0),
		//5
		//D3DXVECTOR3(-100,50,0),
		//D3DXVECTOR3(-150,50,0),
		//D3DXVECTOR3(-200,50,0),
		//D3DXVECTOR3(-250,50,0),
		//D3DXVECTOR3(-300,50,0),
		////10
		//D3DXVECTOR3(-100,20,0),
		//D3DXVECTOR3(-50,20,0),
		//D3DXVECTOR3(0,20,0),
		//D3DXVECTOR3(50,20,0),
		//D3DXVECTOR3(100,20,0),
		////15
		//D3DXVECTOR3(-100,20,0),
		//D3DXVECTOR3(50,20,0),
		//D3DXVECTOR3(0,20,0),
		//D3DXVECTOR3(50,20,0),
		//D3DXVECTOR3(100,20,0),
		////20
		//D3DXVECTOR3(-100,20,0),
		//D3DXVECTOR3(-50,20,0),
		//D3DXVECTOR3(0,20,0),
		//D3DXVECTOR3(50,20,0),
		//D3DXVECTOR3(100,20,0),
		////25
		//D3DXVECTOR3(-100,20,0),
		//D3DXVECTOR3(-50,20,0),
		//D3DXVECTOR3(0,20,0),
		//D3DXVECTOR3(50,20,0),
		//D3DXVECTOR3(100,20,0)
		//30
	};

	enemyB_wave1 = TRUE;
	

	for (int i = 0; i < NUM_ENEMY_B; i++)
	{
		// �ʒu�E��]�E�X�P�[���̏����ݒ�
		B_enemyWK[i].m_bUse = FALSE;				//�G�l�~�[�\���̂̎g�p�t���O��OFF�ɂ���
		B_enemyWK[i].m_posModel = initPos[i];
		B_enemyWK[i].m_rotModel = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		B_enemyWK[i].m_sclModel = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		B_enemyWK[i].m_DirSpeed = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

		DWORD VertexNum = eb_pD3DXMeshModel->GetNumVertices();
		DWORD VertexSize = eb_pD3DXMeshModel->GetNumBytesPerVertex();	//�P�̒��_�̃T�C�Y
		BYTE *p;

		HRESULT Bhr = eb_pD3DXMeshModel->LockVertexBuffer(D3DLOCK_READONLY, (LPVOID*)&p);

		D3DXComputeBoundingSphere((D3DXVECTOR3*)p, VertexNum, VertexSize,
			&B_enemyWK[i].EnemyCenter, &B_enemyWK[i].EnemyRadius);

		eb_pD3DXMeshModel->UnlockVertexBuffer();

		D3DXCreateSphere(pDevice, B_enemyWK[i].EnemyRadius, 24, 24, &eb_pD3DXMeshSphere, NULL);

		B_enemyWK[i].m_Radius = D3DXVECTOR3(B_enemyWK[i].EnemyRadius, 0.0f, 0.0f);
	}


	return S_OK;
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawEnemy_B(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();


	D3DXMATERIAL *pD3DXMat_BE;
	D3DMATERIAL9 matDef_BE;		//�}�e���A���\���̂̃|�C���^(D3DMATERIAL9�\����)


	//enemyMantis = GetPlayer();

	

	for (int i = 0; i < 5; i++)
	{
		
		if (B_enemyWK[i].m_bUse)
		{

			// ���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&B_enemyWK[i].m_mtxWorld);

			// �X�P�[���𔽉f
			D3DXMatrixScaling(&B_enemyWK[i].m_mtxScl, B_enemyWK[i].m_sclModel.x, B_enemyWK[i].m_sclModel.y, B_enemyWK[i].m_sclModel.z);
			D3DXMatrixMultiply(&B_enemyWK[i].m_mtxWorld, &B_enemyWK[i].m_mtxWorld, &B_enemyWK[i].m_mtxScl);

			// ��]�𔽉f
			D3DXMatrixRotationYawPitchRoll(&B_enemyWK[i].m_mtxRot, B_enemyWK[i].m_rotModel.y, B_enemyWK[i].m_rotModel.x, B_enemyWK[i].m_rotModel.z);
			CalcLookAtMatrix(
				&B_enemyWK[i].m_mtxRot,
				&B_enemyWK[i].m_posModel,
				&enemyMantis->m_posModel,
				&D3DXVECTOR3(0.0f, 1.0f, 0.0f));
			D3DXMatrixMultiply(&B_enemyWK[i].m_mtxWorld, &B_enemyWK[i].m_mtxWorld, &B_enemyWK[i].m_mtxRot);

			// �ړ��𔽉f
			D3DXMatrixTranslation(&B_enemyWK[i].m_mtxTranslate, B_enemyWK[i].m_posModel.x, B_enemyWK[i].m_posModel.y, B_enemyWK[i].m_posModel.z);
			D3DXMatrixMultiply(&B_enemyWK[i].m_mtxWorld, &B_enemyWK[i].m_mtxWorld, &B_enemyWK[i].m_mtxTranslate);

			// ���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &B_enemyWK[i].m_mtxWorld);

			// ���݂̃}�e���A�����擾
			pDevice->GetMaterial(&matDef_BE);		//���݃Z�b�g����Ă���}�e���A�����擾���ĕۑ����Ă����B

			// �}�e���A�����ɑ΂���|�C���^���擾
			pD3DXMat_BE = (D3DXMATERIAL*)eb_pD3DXBuffMatModel->GetBufferPointer();		//X�t�@�C������擾�����}�e���A���\���̔z��̐擪�̃|�C���^���擾����B
			for (int nCntMat = 0; nCntMat < (int)eb_nNumMatModel; nCntMat++)		//�擾�����}�e���A���̐��������[�v
			{
				// �}�e���A���̐ݒ�@�}�e���A����DirectX�փZ�b�g
				pDevice->SetMaterial(&pD3DXMat_BE[nCntMat].MatD3D);		//�\���̔z��̃}�e���A���f�[�^���f�o�C�X�փZ�b�g

				// �e�N�X�`���̐ݒ�
				pDevice->SetTexture(0, eb_pD3DTextureModel);

				// ���b�V���̕`��
				eb_pD3DXMeshModel->DrawSubset(nCntMat);
			}
			pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

			//e_pD3DXMeshSphere->DrawSubset(0);
			pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
			//}

			// �}�e���A�����f�t�H���g�ɖ߂�
			pDevice->SetMaterial(&matDef_BE);
		}

	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateEnemy_B(void)
{
	//enemyMantis = GetPlayer();
	beforeWaveFlag = GetEnemyWave2();


	if (enemyMantis->m_posModel.z >= 2000 && beforeWaveFlag == false)
	{
		//E_wave1 = TRUE;
		if (enemyB_wave1)
		{
			for (int i = 0; i < 5; i++)
			{
				B_enemyWK[i].m_bUse = TRUE;
			}
			enemyB_wave1 = FALSE;
		}
	}

	for (int i = 0; i < 5; i++)
	{
		BeeToDistancePlayer[i] = enemyMantis->m_posModel - B_enemyWK[i].m_posModel;
		float BeeArea = D3DXVec3Length(&BeeToDistancePlayer[i]);

		BeeEnemyBulletSpeed[i].x = B_enemyWK[i].m_mtxRot._31;
		BeeEnemyBulletSpeed[i].y = B_enemyWK[i].m_mtxRot._32;
		BeeEnemyBulletSpeed[i].z = B_enemyWK[i].m_mtxRot._33;
		BeeEnemyBulletSpeed[i] *= -10.0f;

		if (B_enemyWK[i].m_bUse)
		{
			B_enemyWK[i].m_posModel.z = enemyMantis->m_posModel.z + (50.0f*i) - 50.0f *2 ;

			//float radian = PI * 2 / 180.0f * actionTime;
			//float moveLength = 100.0f * sinf(radian);
			//B_enemyWK[i].m_posModel.z += moveLength;

			if (BeeArea < 200)
			{
				CreateEnemyBullet(B_enemyWK[i].m_posModel, BeeEnemyBulletSpeed[i]);
			}


			//OBJECT3D* ebullet = GetBullet();
			//for (int n = 0; n < NUM_BULLET; n++)
			//{	//�`�F�b�N�֐����Ă�
			//	HitOBJECT3D(&ebullet[n], &enemyWK[i]);
			//}
		}
	}
	//EU_use1[i] = TRUE;


	for (int i = 0; i < 5; i++)
	{
		if (B_enemyWK[i].m_bUse)
		{

			OBJECT3D* ebullet = GetBullet();
			for (int n = 0; n < 10; n++)
			{
				if (ebullet[n].m_bUse)
				{
					//�`�F�b�N�֐����Ă�

					if (HitOBJECT3D(&ebullet[n], &B_enemyWK[i]) == TRUE)
					{
						ebullet[n].m_bUse = FALSE;
						B_enemyWK[i].m_bUse = FALSE;
					}

				}
			}
		}
	}
}

//=============================================================================
// �I������
//=============================================================================
void UninitEnemy_B(void)
{
	if (eb_pD3DTextureModel != NULL)
	{// �e�N�X�`���̊J��
		eb_pD3DTextureModel->Release();
		eb_pD3DTextureModel = NULL;
	}

	if (eb_pD3DXMeshModel != NULL)
	{// ���b�V���̊J��
		eb_pD3DXMeshModel->Release();
		eb_pD3DXMeshModel = NULL;
	}

	if (eb_pD3DXBuffMatModel != NULL)
	{// �}�e���A���̊J��
		eb_pD3DXBuffMatModel->Release();
		eb_pD3DXBuffMatModel = NULL;
	}
}