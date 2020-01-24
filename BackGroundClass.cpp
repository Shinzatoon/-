#include "BackGroundClass.h"
#include "model.h"
#include "model.h"


BackGroundClass::BackGroundClass()
{
}


BackGroundClass::~BackGroundClass()
{
}

//=============================================================================
// ����������
//=============================================================================
HRESULT BackGroundClass::InitBG(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();




	// �ʒu�E��]�E�X�P�[���̏����ݒ�
	g_posBG = D3DXVECTOR3(0.0f, 180.0f, 0.0f);
	g_rotBG = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_sclBG = D3DXVECTOR3(10.0f, 10.0f, 10.0f);

	g_pD3DTextureBG = NULL;
	g_pD3DXMeshBG = NULL;
	g_pD3DXBuffMatBG = NULL;
	g_nNumMatBG = 0;

	// X�t�@�C���̓ǂݍ���
	if (FAILED(D3DXLoadMeshFromX(MODEL_BG,				// �ǂݍ��ރ��f���t�@�C����(X�t�@�C��)�@����������������Α��̂��\���ł���B
		D3DXMESH_SYSTEMMEM,		// ���b�V���̍쐬�I�v�V�������w��
		pDevice,				// IDirect3DDevice9�C���^�[�t�F�C�X�ւ̃|�C���^
		NULL,					// �אڐ��f�[�^���܂ރo�b�t�@�ւ̃|�C���^
		&g_pD3DXBuffMatBG,		// �}�e���A���f�[�^���܂ރo�b�t�@�ւ̃|�C���^	�}�e���A���Ǘ��@	�ǂݍ���X�t�@�C�����f���̏�񂪂��ꂼ��i�[����Ă���
		NULL,					// �G�t�F�N�g�C���X�^���X�̔z����܂ރo�b�t�@�ւ̃|�C���^							
		&g_nNumMatBG,			// D3DXMATERIAL�\���̂̐�															//
		&g_pD3DXMeshBG)))		// ID3DXMesh�C���^�[�t�F�C�X�ւ̃|�C���^�̃A�h���X									//
	{
		return E_FAIL;
	}

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,					// �f�o�C�X�ւ̃|�C���^
		TEXTURE_BG_FILENAME,	// �t�@�C���̖��O
		&g_pD3DTextureBG);		// �ǂݍ��ރ�����
#if 0
	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,					// �f�o�C�X�ւ̃|�C���^
		TEXTURE_FILENAME,		// �t�@�C���̖��O
		&g_pD3DTextureModel);	// �ǂݍ��ރ������[
#endif

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void BackGroundClass::UninitBG(void)
{
	if (g_pD3DTextureBG != NULL)
	{// �e�N�X�`���̊J��
		g_pD3DTextureBG->Release();
		g_pD3DTextureBG = NULL;
	}

	if (g_pD3DXMeshBG != NULL)
	{// ���b�V���̊J��
		g_pD3DXMeshBG->Release();
		g_pD3DXMeshBG = NULL;
	}

	if (g_pD3DXBuffMatBG != NULL)
	{// �}�e���A���̊J��
		g_pD3DXBuffMatBG->Release();
		g_pD3DXBuffMatBG = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void BackGroundClass::UpdateBG(void)
{

}

//=============================================================================
// �`�揈��
//=============================================================================
void BackGroundClass::DrawBG(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	D3DXMATERIAL *pD3DXMat_BG;
	D3DMATERIAL9 matDef_BG;		//�}�e���A���\���̂̃|�C���^(D3DMATERIAL9�\����)

	pBgModel0 = GetPlayer();


	// �v���W�F�N�V�����}�g���b�N�X�̐ݒ�
	//	pDevice->SetTransform(D3DTS_PROJECTION, &g_mtxProjectionBG);

		//���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_mtxWorldBG);

	//�X�P�[���𔽉f
	D3DXMatrixScaling(&mtxSclBG, g_sclBG.x, g_sclBG.y, g_sclBG.z);
	D3DXMatrixMultiply(&g_mtxWorldBG, &g_mtxWorldBG, &mtxSclBG);

	//��]�𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRotBG, g_rotBG.y, g_rotBG.x, g_rotBG.z);
	D3DXMatrixMultiply(&g_mtxWorldBG, &g_mtxWorldBG, &mtxRotBG);

	//�ړ��𔽉f
	//���f���Q�̈ړ��Ƃ̃y�A�����g����
	D3DXMatrixTranslation(&mtxTranslateBG, pBgModel0->m_posModel.x, pBgModel0->m_posModel.y, pBgModel0->m_posModel.z);
	D3DXMatrixMultiply(&g_mtxWorldBG, &g_mtxWorldBG, &mtxTranslateBG);


	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldBG);

	// ���C���e�B���O�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// ���݂̃}�e���A�����擾
	pDevice->GetMaterial(&matDef_BG);		//���݃Z�b�g����Ă���}�e���A�����擾���ĕۑ����Ă����B

	// �}�e���A�����ɑ΂���|�C���^���擾
	pD3DXMat_BG = (D3DXMATERIAL*)g_pD3DXBuffMatBG->GetBufferPointer();		//X�t�@�C������擾�����}�e���A���\���̔z��̐擪�̃|�C���^���擾����B


	for (int nCntMat = 0; nCntMat < (int)g_nNumMatBG; nCntMat++)		//�擾�����}�e���A���̐��������[�v
	{
		// �}�e���A���̐ݒ�@�}�e���A����DirectX�փZ�b�g
		pDevice->SetMaterial(&pD3DXMat_BG[nCntMat].MatD3D);		//�\���̔z��̃}�e���A���f�[�^���f�o�C�X�փZ�b�g

		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pD3DTextureBG);

		// ���b�V���̕`��
		g_pD3DXMeshBG->DrawSubset(nCntMat);
	}

	// �}�e���A�����f�t�H���g�ɖ߂�
	pDevice->SetMaterial(&matDef_BG);

	// ���C���e�B���O��L���ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}