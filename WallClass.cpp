//=============================================================================
// �C���N���[�h
//=============================================================================
#include "WallClass.h"
#include "model.h"

//=============================================================================
// �R���X�g���N�^
//=============================================================================
WallClass::WallClass()
{

}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
WallClass::~WallClass()
{

}

//=============================================================================
// ����������
//=============================================================================
HRESULT WallClass::InitWall(void) 
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();


	w_pD3DTextureModel = NULL;
	w_pD3DXMeshModel = NULL;
	w_pD3DXBuffMatModel = NULL;
	w_nNumMatModel = 0;

	// X�t�@�C���̓ǂݍ���
	if (FAILED(D3DXLoadMeshFromX(MODEL_WALL,				// �ǂݍ��ރ��f���t�@�C����(X�t�@�C��)�@����������������Α��̂��\���ł���B
		D3DXMESH_SYSTEMMEM,		// ���b�V���̍쐬�I�v�V�������w��
		pDevice,				// IDirect3DDevice9�C���^�[�t�F�C�X�ւ̃|�C���^
		NULL,					// �אڐ��f�[�^���܂ރo�b�t�@�ւ̃|�C���^
		&w_pD3DXBuffMatModel,	// �}�e���A���f�[�^���܂ރo�b�t�@�ւ̃|�C���^	�}�e���A���Ǘ��@	�ǂݍ���X�t�@�C�����f���̏�񂪂��ꂼ��i�[����Ă���
		NULL,					// �G�t�F�N�g�C���X�^���X�̔z����܂ރo�b�t�@�ւ̃|�C���^							
		&w_nNumMatModel,		// D3DXMATERIAL�\���̂̐�															//
		&w_pD3DXMeshModel)))	// ID3DXMesh�C���^�[�t�F�C�X�ւ̃|�C���^�̃A�h���X									//
	{
		return E_FAIL;
	}

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,					// �f�o�C�X�ւ̃|�C���^
		TEXTURE_WALLFILE,		// �t�@�C���̖��O
		&w_pD3DTextureModel);	// �ǂݍ��ރ�����

	for (int i = 0; i < RIGHT_WALL; i++)
	{
		// �ʒu�E��]�E�X�P�[���̏����ݒ�
		WallWK[i].m_bUse = FALSE;				//�G�l�~�[�\���̂̎g�p�t���O��OFF�ɂ���
		WallWK[i].m_posModel = D3DXVECTOR3(-500.0f, 50.0f, 200.0f*i);
		WallWK[i].m_rotModel = D3DXVECTOR3(0.0f, -PI / 2, PI / 2);
		WallWK[i].m_sclModel = D3DXVECTOR3(50.0f, 200.0f, 1.0f);
		WallWK[i].m_DirSpeed = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	}

	for (int i = RIGHT_WALL; i < NUM_WALL; i++)
	{
		// �ʒu�E��]�E�X�P�[���̏����ݒ�
		WallWK[i].m_bUse = FALSE;				//�G�l�~�[�\���̂̎g�p�t���O��OFF�ɂ���
		WallWK[i].m_posModel = D3DXVECTOR3(500.0f, 50.0f, 200.0f*(i - RIGHT_WALL));
		WallWK[i].m_rotModel = D3DXVECTOR3(0.0f, PI / 2, -PI / 2);
		WallWK[i].m_sclModel = D3DXVECTOR3(50.0f, 200.0f, 1.0f);
		WallWK[i].m_DirSpeed = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	}

	//DWORD VertexNum = e_pD3DXMeshModel->GetNumVertices();
	//DWORD VertexSize = e_pD3DXMeshModel->GetNumBytesPerVertex();	//�P�̒��_�̃T�C�Y
	//BYTE *p;
	//
	//HRESULT Bhr = e_pD3DXMeshModel->LockVertexBuffer(D3DLOCK_READONLY, (LPVOID*)&p);
	//
	//D3DXComputeBoundingSphere((D3DXVECTOR3*)p, VertexNum, VertexSize,
	//	&enemyWK[i].EnemyCenter, &enemyWK[i].EnemyRadius);
	//
	//e_pD3DXMeshModel->UnlockVertexBuffer();
	//
	//D3DXCreateSphere(pDevice, enemyWK[i].EnemyRadius, 24, 24, &e_pD3DXMeshSphere, NULL);
	//
	//enemyWK[i].m_Radius = D3DXVECTOR3(enemyWK[i].EnemyRadius, 0.0f, 0.0f);

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void WallClass::UninitWall(void) 
{
	if (w_pD3DTextureModel != NULL)
	{// �e�N�X�`���̊J��
		w_pD3DTextureModel->Release();
		w_pD3DTextureModel = NULL;
	}

	if (w_pD3DXMeshModel != NULL)
	{// ���b�V���̊J��
		w_pD3DXMeshModel->Release();
		w_pD3DXMeshModel = NULL;
	}

	if (w_pD3DXBuffMatModel != NULL)
	{// �}�e���A���̊J��
		w_pD3DXBuffMatModel->Release();
		w_pD3DXBuffMatModel = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void WallClass::UpdateWall(void) 
{
	pPlayerWall = GetPlayer();

	for (int i = 0; i < NUM_WALL; i++)
	{

		StractdistanceWall = WallWK[i].m_posModel.z - pPlayerWall->m_posModel.z;


		if (StractdistanceWall < -LIMIT_WALL_Z / 2)
		{
			WallWK[i].m_posModel.z += LIMIT_WALL_Z;
		}

		if (StractdistanceWall > LIMIT_WALL_Z / 2)
		{
			WallWK[i].m_posModel.z -= LIMIT_WALL_Z;
		}

		StractdistanceWall = 0;

	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void WallClass::DrawWall(void) 
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();


	D3DXMATERIAL *pD3DXMat_W;
	D3DMATERIAL9 matDef_W;		//�}�e���A���\���̂̃|�C���^(D3DMATERIAL9�\����)

	// ���C���e�B���O�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);


	for (int i = 0; i < NUM_WALL; i++)
	{

		// ���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&WallWK[i].m_mtxWorld);

		// �X�P�[���𔽉f
		D3DXMatrixScaling(&WallWK[i].m_mtxScl, WallWK[i].m_sclModel.x, WallWK[i].m_sclModel.y, WallWK[i].m_sclModel.z);
		D3DXMatrixMultiply(&WallWK[i].m_mtxWorld, &WallWK[i].m_mtxWorld, &WallWK[i].m_mtxScl);

		// ��]�𔽉f
		D3DXMatrixRotationYawPitchRoll(&WallWK[i].m_mtxRot, WallWK[i].m_rotModel.y, WallWK[i].m_rotModel.x, WallWK[i].m_rotModel.z);
		D3DXMatrixMultiply(&WallWK[i].m_mtxWorld, &WallWK[i].m_mtxWorld, &WallWK[i].m_mtxRot);

		// �ړ��𔽉f
		D3DXMatrixTranslation(&WallWK[i].m_mtxTranslate, WallWK[i].m_posModel.x, WallWK[i].m_posModel.y, WallWK[i].m_posModel.z);
		D3DXMatrixMultiply(&WallWK[i].m_mtxWorld, &WallWK[i].m_mtxWorld, &WallWK[i].m_mtxTranslate);

		// ���[���h�}�g���b�N�X�̐ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &WallWK[i].m_mtxWorld);

		// ���݂̃}�e���A�����擾
		pDevice->GetMaterial(&matDef_W);		//���݃Z�b�g����Ă���}�e���A�����擾���ĕۑ����Ă����B

		// �}�e���A�����ɑ΂���|�C���^���擾
		pD3DXMat_W = (D3DXMATERIAL*)w_pD3DXBuffMatModel->GetBufferPointer();		//X�t�@�C������擾�����}�e���A���\���̔z��̐擪�̃|�C���^���擾����B
		for (int nCntMat = 0; nCntMat < (int)w_nNumMatModel; nCntMat++)		//�擾�����}�e���A���̐��������[�v
		{
			// �}�e���A���̐ݒ�@�}�e���A����DirectX�փZ�b�g
			pDevice->SetMaterial(&pD3DXMat_W[nCntMat].MatD3D);		//�\���̔z��̃}�e���A���f�[�^���f�o�C�X�փZ�b�g

			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, w_pD3DTextureModel);

			// ���b�V���̕`��
			w_pD3DXMeshModel->DrawSubset(nCntMat);
		}
		pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

		//e_pD3DXMeshSphere->DrawSubset(0);
		pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
		//}

		// �}�e���A�����f�t�H���g�ɖ߂�
		pDevice->SetMaterial(&matDef_W);


	}
	// ���C���e�B���O��L���ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}
