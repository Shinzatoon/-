//============================================================================
// �G�l�~�[A���� [enemy_A.cpp]
// Author : �V���@���m
//
//=============================================================================
#include "enemy_A.h"	//�C���\��
#include "model.h"
#include "input.h"
#include "bullet.h"
#include "particle.h" 
#include "enemyBullet.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	MODEL_ENEMY		"data/MODEL/EnemyMantis.x"				// �ǂݍ��ރ��f����
#define	ENEMY_MOVE		(2.0f)									// �ړ���
#define	ENEMY_ROTATE	(D3DX_PI * 0.02f)						// ��]��
#define NUM_ENEMY		(10)

#define ENEMY_A_WAVE_1	(5)
#define ENEMY_A_WAVE_2	(10)

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9	e_pD3DTextureModel;		// �e�N�X�`���ւ̃|�C���^
LPD3DXMESH			e_pD3DXMeshSphere;		// �����蔻��p�̃��b�V�����
LPD3DXMESH			e_pD3DXMeshModel;		// ���b�V�����ւ̃|�C���^�@�ǂݍ���X�t�@�C�����f����\���B
LPD3DXBUFFER		e_pD3DXBuffMatModel;	// �}�e���A�����ւ̃|�C���^�@�ǂݍ���X�t�@�C�����f���̃}�e���A����\���B
DWORD				e_nNumMatModel;			// �}�e���A�����̐��@�ǂݍ���X�t�@�C�����f���̃}�e���A���̐��B

OBJECT3D			enemyWK[NUM_ENEMY];		//�G�l�~�[���

bool				E_wave1;				//Wave�����t���O
int					cntWaveEnemy1;			//Wave�P����true��Ԃ̃G�l�~�[���J�E���g
bool				occurrenceWate1;		//Wavw1�G�l�~�[�����ҋ@���

bool				E_wave2;				//Wave2�����t���O
int					cntWaveEnemy2;			//Wave2����true��Ԃ̃G�l�~�[���J�E���g
bool				occurrenceWate2;		//Wavw2�G�l�~�[�����ҋ@���

D3DXVECTOR3			distancePlayer[NUM_ENEMY];			//�v���C���[�ƃG�l�~�[�̋���
D3DXVECTOR3			enemyBulletSpeed[NUM_ENEMY];		//�G�l�~�[�̃o���b�g���x


OBJECT3D			*pEnemyModel0;			//�v���C���[���̎擾


//=============================================================================
// ����������
//=============================================================================
HRESULT InitEnemy_A(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	pEnemyModel0 = GetPlayer();

	E_wave1 = false;
	cntWaveEnemy1 = 0;
	occurrenceWate1 = true;

	E_wave2 = false;
	occurrenceWate2 = true;

	e_pD3DTextureModel = NULL;
	e_pD3DXMeshModel = NULL;
	e_pD3DXBuffMatModel = NULL;
	e_nNumMatModel = 0;

	// X�t�@�C���̓ǂݍ���
	if (FAILED(D3DXLoadMeshFromX(MODEL_ENEMY,		// �ǂݍ��ރ��f���t�@�C����(X�t�@�C��)�@����������������Α��̂��\���ł���B
		D3DXMESH_SYSTEMMEM,							// ���b�V���̍쐬�I�v�V�������w��
		pDevice,									// IDirect3DDevice9�C���^�[�t�F�C�X�ւ̃|�C���^
		NULL,										// �אڐ��f�[�^���܂ރo�b�t�@�ւ̃|�C���^
		&e_pD3DXBuffMatModel,						// �}�e���A���f�[�^���܂ރo�b�t�@�ւ̃|�C���^	�}�e���A���Ǘ��@	�ǂݍ���X�t�@�C�����f���̏�񂪂��ꂼ��i�[����Ă���
		NULL,										// �G�t�F�N�g�C���X�^���X�̔z����܂ރo�b�t�@�ւ̃|�C���^							
		&e_nNumMatModel,							// D3DXMATERIAL�\���̂̐�
		&e_pD3DXMeshModel)))						// ID3DXMesh�C���^�[�t�F�C�X�ւ̃|�C���^�̃A�h���X					
	{
		return E_FAIL;
	}

	
	D3DXVECTOR3 initPos[NUM_ENEMY] =
	{
		D3DXVECTOR3(-100,25,0),
		D3DXVECTOR3(-50,25,0),
		D3DXVECTOR3(0,25,0),
		D3DXVECTOR3(50,25,0),
		D3DXVECTOR3(100,25,0),
		//5
		D3DXVECTOR3(-100,25,0),
		D3DXVECTOR3(-150,25,0),
		D3DXVECTOR3(-200,25,0),
		D3DXVECTOR3(-250,25,0),
		D3DXVECTOR3(-300,25,0),
		//10
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

	E_wave1 = true;
	E_wave2 = true;

	for (int i = 0; i < NUM_ENEMY; i++)
	{
		// �ʒu�E��]�E�X�P�[���̏����ݒ�
		enemyWK[i].m_bUse = false;				//�G�l�~�[�\���̂̎g�p�t���O��OFF�ɂ���
		enemyWK[i].m_posModel = initPos[i];
		enemyWK[i].m_rotModel = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		enemyWK[i].m_sclModel = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		enemyWK[i].m_DirSpeed = D3DXVECTOR3(1.0f, 1.0f, 1.0f);


		DWORD VertexNum = e_pD3DXMeshModel->GetNumVertices();
		DWORD VertexSize = e_pD3DXMeshModel->GetNumBytesPerVertex();	//�P�̒��_�̃T�C�Y
		BYTE *p;

		HRESULT Bhr = e_pD3DXMeshModel->LockVertexBuffer(D3DLOCK_READONLY, (LPVOID*)&p);

		D3DXComputeBoundingSphere((D3DXVECTOR3*)p, VertexNum, VertexSize,
									&enemyWK[i].EnemyCenter, &enemyWK[i].EnemyRadius);

		e_pD3DXMeshModel->UnlockVertexBuffer();

		D3DXCreateSphere(pDevice, enemyWK[i].EnemyRadius, 24, 24, &e_pD3DXMeshSphere, NULL);

		enemyWK[i].m_Radius = D3DXVECTOR3(enemyWK[i].EnemyRadius, 0.0f, 0.0f);
	}


	return S_OK;
}



//=============================================================================
// �`�揈��
//=============================================================================
void DrawEnemy_A(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();


	D3DXMATERIAL *pD3DXMat_E;
	D3DMATERIAL9 matDef_E;		//�}�e���A���\���̂̃|�C���^(D3DMATERIAL9�\����)

	
	pEnemyModel0 = GetPlayer();

	for (int i = 0; i < ENEMY_A_WAVE_1; i++)
	{
		
		if (enemyWK[i].m_bUse)
		{

			// ���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&enemyWK[i].m_mtxWorld);

			// �X�P�[���𔽉f
			D3DXMatrixScaling(&enemyWK[i].m_mtxScl, enemyWK[i].m_sclModel.x, enemyWK[i].m_sclModel.y, enemyWK[i].m_sclModel.z);
			D3DXMatrixMultiply(&enemyWK[i].m_mtxWorld, &enemyWK[i].m_mtxWorld, &enemyWK[i].m_mtxScl);

			// ��]�𔽉f
			D3DXMatrixRotationYawPitchRoll(&enemyWK[i].m_mtxRot, enemyWK[i].m_rotModel.y, enemyWK[i].m_rotModel.x, enemyWK[i].m_rotModel.z);
			CalcLookAtMatrixAxisFix(
				&enemyWK[i].m_mtxRot,
				&enemyWK[i].m_posModel,
				&pEnemyModel0->m_posModel,
				&D3DXVECTOR3(0.0f, 1.0f, 0.0f)
			);
			D3DXMatrixMultiply(&enemyWK[i].m_mtxWorld, &enemyWK[i].m_mtxWorld, &enemyWK[i].m_mtxRot);

			
			// �ړ��𔽉f
			D3DXMatrixTranslation(&enemyWK[i].m_mtxTranslate, enemyWK[i].m_posModel.x, enemyWK[i].m_posModel.y, enemyWK[i].m_posModel.z);
			D3DXMatrixMultiply(&enemyWK[i].m_mtxWorld, &enemyWK[i].m_mtxWorld, &enemyWK[i].m_mtxTranslate);

			// ���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &enemyWK[i].m_mtxWorld);

			// ���݂̃}�e���A�����擾
			pDevice->GetMaterial(&matDef_E);		//���݃Z�b�g����Ă���}�e���A�����擾���ĕۑ����Ă����B

			// �}�e���A�����ɑ΂���|�C���^���擾
			pD3DXMat_E = (D3DXMATERIAL*)e_pD3DXBuffMatModel->GetBufferPointer();		//X�t�@�C������擾�����}�e���A���\���̔z��̐擪�̃|�C���^���擾����B
			for (int nCntMat = 0; nCntMat < (int)e_nNumMatModel; nCntMat++)		//�擾�����}�e���A���̐��������[�v
			{
				// �}�e���A���̐ݒ�@�}�e���A����DirectX�փZ�b�g
				pDevice->SetMaterial(&pD3DXMat_E[nCntMat].MatD3D);		//�\���̔z��̃}�e���A���f�[�^���f�o�C�X�փZ�b�g

				// �e�N�X�`���̐ݒ�
				pDevice->SetTexture(0, e_pD3DTextureModel);

				// ���b�V���̕`��
				e_pD3DXMeshModel->DrawSubset(nCntMat);
			}
			pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

			pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	
			// �}�e���A�����f�t�H���g�ɖ߂�
			pDevice->SetMaterial(&matDef_E);
		}	
		
	}
	
	
	
	for (int i = ENEMY_A_WAVE_1; i < ENEMY_A_WAVE_2; i++)
	{
		
		if (enemyWK[i].m_bUse)
		{

			// ���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&enemyWK[i].m_mtxWorld);

			// �X�P�[���𔽉f
			D3DXMatrixScaling(&enemyWK[i].m_mtxScl, enemyWK[i].m_sclModel.x, enemyWK[i].m_sclModel.y, enemyWK[i].m_sclModel.z);
			D3DXMatrixMultiply(&enemyWK[i].m_mtxWorld, &enemyWK[i].m_mtxWorld, &enemyWK[i].m_mtxScl);

			// ��]�𔽉f
			D3DXMatrixRotationYawPitchRoll(&enemyWK[i].m_mtxRot, enemyWK[i].m_rotModel.y, enemyWK[i].m_rotModel.x, enemyWK[i].m_rotModel.z);
			CalcLookAtMatrixAxisFix(
				&enemyWK[i].m_mtxRot,
				&enemyWK[i].m_posModel,
				&pEnemyModel0->m_posModel,
				&D3DXVECTOR3(0.0f, 1.0f, 0.0f));
			D3DXMatrixMultiply(&enemyWK[i].m_mtxWorld, &enemyWK[i].m_mtxWorld, &enemyWK[i].m_mtxRot);

			// �ړ��𔽉f
			D3DXMatrixTranslation(&enemyWK[i].m_mtxTranslate, enemyWK[i].m_posModel.x, enemyWK[i].m_posModel.y, enemyWK[i].m_posModel.z);
			D3DXMatrixMultiply(&enemyWK[i].m_mtxWorld, &enemyWK[i].m_mtxWorld, &enemyWK[i].m_mtxTranslate);

			// ���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &enemyWK[i].m_mtxWorld);

			// ���݂̃}�e���A�����擾
			pDevice->GetMaterial(&matDef_E);		//���݃Z�b�g����Ă���}�e���A�����擾���ĕۑ����Ă����B

			// �}�e���A�����ɑ΂���|�C���^���擾
			pD3DXMat_E = (D3DXMATERIAL*)e_pD3DXBuffMatModel->GetBufferPointer();		//X�t�@�C������擾�����}�e���A���\���̔z��̐擪�̃|�C���^���擾����B
			for (int nCntMat = 0; nCntMat < (int)e_nNumMatModel; nCntMat++)		//�擾�����}�e���A���̐��������[�v
			{
				// �}�e���A���̐ݒ�@�}�e���A����DirectX�փZ�b�g
				pDevice->SetMaterial(&pD3DXMat_E[nCntMat].MatD3D);		//�\���̔z��̃}�e���A���f�[�^���f�o�C�X�փZ�b�g

				// �e�N�X�`���̐ݒ�
				pDevice->SetTexture(0, e_pD3DTextureModel);

				// ���b�V���̕`��
				e_pD3DXMeshModel->DrawSubset(nCntMat);
			}
			pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

			
			pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

			// �}�e���A�����f�t�H���g�ɖ߂�
			pDevice->SetMaterial(&matDef_E);

		}
	}

}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateEnemy_A(void)
{
	pEnemyModel0 = GetPlayer();
	
	//WAVE1�@
	//�G�l�~�[�̔���
	if (pEnemyModel0->m_posModel.z >= 100)
	{
		E_wave1 = true;
		if (occurrenceWate1)
		{
			for (int i = 0; i < ENEMY_A_WAVE_1; i++)
			{
				enemyWK[i].m_bUse = true;
			}
			occurrenceWate1 = false;
		}
	}
	//WAVE���̃G�l�~�[�����ׂē|���ꂽ��WAVE��false�ɂ���B�B
	cntWaveEnemy1 = ENEMY_A_WAVE_1;
	for (int i = 0; i < ENEMY_A_WAVE_1; i++)
	{
		if (enemyWK[i].m_bUse == false)
		{
			cntWaveEnemy1--;
		}
		if (cntWaveEnemy1 <= 0)
		{
			E_wave1 = false;
		}
	}
	

	//WAVE1�G�l�~�[�̍U���p�^�[��
	for (int i = 0; i < ENEMY_A_WAVE_1; i++)
	{
		distancePlayer[i] = pEnemyModel0->m_posModel - enemyWK[i].m_posModel;		//�v���C���[�Ƃ̋���
		float area = D3DXVec3Length(&distancePlayer[i]);							//�U���J�n�͈�

		//�U���̕������G�l�~�[�̌����Ă�������ɍ��킹�A�X�s�[�h�����߂�
		enemyBulletSpeed[i].x = enemyWK[i].m_mtxRot._31;
		enemyBulletSpeed[i].y = enemyWK[i].m_mtxRot._32;
		enemyBulletSpeed[i].z = enemyWK[i].m_mtxRot._33;
		enemyBulletSpeed[i] *= -1.0f;

		if (enemyWK[i].m_bUse)
		{
			//�o���ʒu���瓮���Ȃ�
			enemyWK[i].m_posModel.z = 100.0f;			

			//�U���͈͓��ɂȂ�����
			if (area < 200.0f)
			{
				CreateEnemyBullet(enemyWK[i].m_posModel, enemyBulletSpeed[i]);
			}
		}
	}
	
	
	//WAVE2
	//�G�l�~�[�̔���
	if (pEnemyModel0->m_posModel.z >= 800 && E_wave1 == false)
	{
		E_wave2 = true;
		if (occurrenceWate2)
		{
			for (int i = ENEMY_A_WAVE_1; i < ENEMY_A_WAVE_2; i++)
			{
				enemyWK[i].m_bUse = true;
			}
			occurrenceWate2 = false;
		}
	}

	//WAVE���̃G�l�~�[�����ׂē|���ꂽ��WAVE��false�ɂ���B�B
	cntWaveEnemy2 = ENEMY_A_WAVE_2;
	for (int i = ENEMY_A_WAVE_1; i < ENEMY_A_WAVE_2; i++)
	{
		if (enemyWK[i].m_bUse == false)
		{
			cntWaveEnemy2--;
		}
		if (cntWaveEnemy2 <= ENEMY_A_WAVE_1)
		{
			E_wave2 = false;
		}
	}

	//WAVE2�G�l�~�[�̍s���p�^�[��
	for (int i = ENEMY_A_WAVE_1; i < ENEMY_A_WAVE_2; i++)
	{
		if (enemyWK[i].m_bUse)
		{
			
			enemyWK[i].m_posModel.z = pEnemyModel0->m_posModel.z + 300.0f;
			
			if (enemyWK[i].m_posModel.x > 300.0f || enemyWK[i].m_posModel.x < -300.0f)
			{
				enemyWK[i].m_DirSpeed.x *= -1.0f;
			}
			enemyWK[i].m_posModel.x += enemyWK[i].m_DirSpeed.x;
		}
	}

	//�e�Ƃ̓����蔻�菈��
	for (int i = 0; i < NUM_ENEMY; i++)
	{
		if (enemyWK[i].m_bUse)
		{

			OBJECT3D* ebullet = GetBullet();
			for (int n = 0; n < 10; n++)
			{	
				if (ebullet[n].m_bUse)
				{
					//�`�F�b�N�֐����Ă�
					if (HitOBJECT3D(&ebullet[n], &enemyWK[i]) == TRUE)
					{

						ebullet[n].m_bUse = false;
						enemyWK[i].m_bUse = false;
					}
					
				}
			}
		}
	}
	

	for (int i = 0; i < NUM_ENEMY; i++)
	{
		if (enemyWK[i].m_bUse)
		{
			D3DXVECTOR3 move;
			float fAngle, fLength;
			int nLife;
			float fSize;

			fAngle = (float)(rand() % 628 - 314) / 100.0f;
			fLength = rand() % (int)(5.0f * 20) / 10.0f - 5.0f;			////�U��΂�͈́i�����j
			move.x = sinf(fAngle) * fLength;
			move.y = rand() % 10 / 10.0f + 5.0f;
			move.z = cosf(fAngle) * fLength;
			nLife = rand() % 90 + 15;
			fSize = 5 + (float)(rand() * (10 - 5 + 1.0) / (1.0 + RAND_MAX));

			SetParticle(enemyWK[i].m_posModel, move, D3DXCOLOR(0.0f, 1.00f, 0.0f, 10.0f), fSize, fSize, nLife);
		}
	}
}

//=============================================================================
// �I������
//=============================================================================
void UninitEnemy_A(void)
{
	if (e_pD3DTextureModel != NULL)
	{// �e�N�X�`���̊J��
		e_pD3DTextureModel->Release();
		e_pD3DTextureModel = NULL;
	}
	
	if (e_pD3DXMeshModel != NULL)
	{// ���b�V���̊J��
		e_pD3DXMeshModel->Release();
		e_pD3DXMeshModel = NULL;
	}
	
	if (e_pD3DXBuffMatModel != NULL)
	{// �}�e���A���̊J��
		e_pD3DXBuffMatModel->Release();
		e_pD3DXBuffMatModel = NULL;
	}
}

int GetEnemyWave2(void)
{
	return E_wave2;
}