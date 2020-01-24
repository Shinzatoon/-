//============================================================================
// ���f������ [model.cpp]
// Author : �V���@���m
//
//=============================================================================
#include "model.h"
#include "input.h"
#include "bullet.h"
#include "reticle.h"
#include "effect.h"
//#include "BulletClass.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define TEXTURE_FILENAME "data/TEXTURE/�e�N�X�`������.jpg"
#define	VALUE_MOVE		(6.0f)											// �ړ���
#define	VALUE_ROTATE	(D3DX_PI / 180.0f)								// ��]��
#define INVINCIBLE_TIME (2.0f)											//���G����
#define BLINK_INTERVAL	(0.5f)											//�_�ŊԊu


#define PAR_INERTIA		(0.9f)						//������
#define SPEED_LIMIT		(3.0f)						//���E���x



//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************

D3DXVECTOR3				CazrelativeVector;				//�e�̔��ˈʒu�̒���

D3DXVECTOR3				rot2 = D3DXVECTOR3(0, 0, 0);

D3DXVECTOR3				rot = D3DXVECTOR3(0, 0, 0);

D3DXVECTOR3				 ReticleVector = D3DXVECTOR3(0, 0, 0);
D3DXVECTOR3				 ReticleLength = D3DXVECTOR3(0, 0, 0);

D3DXVECTOR3				 EffectVector = D3DXVECTOR3(0, 0, 0);
D3DXVECTOR3				 EffectLength = D3DXVECTOR3(0, 0, 0);

D3DXVECTOR3				Temporary = D3DXVECTOR3(0, 0, 0);

//OBJECT3D				*ParentBulletWK;


bool renderFlag;
bool hit;
float blinkFrameTime;
float invincibleTime;
float blinkTime;

//*************�L�����N�^�[�f�[�^�ϐ��̗p��*****************************
#define		NUM_PLAYER	(3)
OBJECT3D	Player[NUM_PLAYER];			//�v���C���[�L�����N�^�\����

//*************���f���[�f�[�^�ϐ��̗p��*****************************
#define		NUM_MODEL	(3)
MODEL3D		PlayerModel[NUM_MODEL];		//�v���C���[���f���\����


float speedinertia;								//���x�����ۑ�
float rotationinertia;

//*********************���f���f�[�^�t�@�C�����z��********************
const char* ModelData[] =
{
	"data/MODEL/���-����2.2.x",
	"data/MODEL/���-�C�����2.2.x",
	"data/MODEL/���-��C����2.2.x",

};



//float					g_Ofset = 1.0f;				// �J�����̉�p�̕ύX

//=============================================================================
// ����������
//=============================================================================
HRESULT InitModel(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	//BazrelativeVector = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	//MODEL *model = &modelWK;

	//ParentBulletWK = GetParentBullet();
	speedinertia = 0.0f;
	rotationinertia = 0.0f;
	blinkFrameTime = 0.0f;
	renderFlag = true;
	hit = false;

	invincibleTime = 0.0f;
	blinkTime = 0.0f;
	
	// ���f���f�[�^�̏����ݒ�
	for (int i = 0; i < NUM_MODEL; i++)
	{
		PlayerModel[i].m_pD3DTextureModel = NULL;
		PlayerModel[i].m_pD3DXMeshModel = NULL;
		PlayerModel[i].m_pD3DXBuffMatModel = NULL;
		PlayerModel[i].m_nNumMatModel = 0;

		// X�t�@�C���̓ǂݍ���
		if (D3D_OK != D3DXLoadMeshFromX(ModelData[i],					// �ǂݍ��ރ��f���t�@�C����(X�t�@�C��)�@����������������Α��̂��\���ł���B
								D3DXMESH_SYSTEMMEM,						// ���b�V���̍쐬�I�v�V�������w��
								pDevice,								// IDirect3DDevice9�C���^�[�t�F�C�X�ւ̃|�C���^
								NULL,									// �אڐ��f�[�^���܂ރo�b�t�@�ւ̃|�C���^
								&PlayerModel[i].m_pD3DXBuffMatModel,	// �}�e���A���f�[�^���܂ރo�b�t�@�ւ̃|�C���^	�}�e���A���Ǘ��@	�ǂݍ���X�t�@�C�����f���̏�񂪂��ꂼ��i�[����Ă���
								NULL,									// �G�t�F�N�g�C���X�^���X�̔z����܂ރo�b�t�@�ւ̃|�C���^							
								&PlayerModel[i].m_nNumMatModel,			// D3DXMATERIAL�\���̂̐�
								&PlayerModel[i].m_pD3DXMeshModel))		// ID3DXMesh�C���^�[�t�F�C�X�ւ̃|�C���^�̃A�h���X									
		{
			return E_FAIL;
		}

		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,					// �f�o�C�X�ւ̃|�C���^
			TEXTURE_FILENAME,		// �t�@�C���̖��O
			&PlayerModel[i].m_pD3DTextureModel);	// �ǂݍ��ރ�����
		for (int i=0; i < NUM_PLAYER; i++)
		{
			// �ʒu�E��]�E�X�P�[���̏����ݒ�
			Player[i].m_posModel = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			Player[i].m_rotModel = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			Player[i].m_sclModel = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

			Player[i].m_DirSpeed = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			Player[i].m_RotSpeed = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
			Player[i].m_Kakudo = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			Player[i].m_Radius = D3DXVECTOR3(300.0f, 1.0f, 385.0f);
			Player[i].m_ModelID = i;
			//Player[i].m_ShadowID = i;

			if (i != 0) 
			{
				SetRotParent(i, &Player[i - 1].m_mtxRot);
				SetParent(i, &Player[i - 1].m_mtxWorld);
				//SetParent(2, &ParentBulletWK->m_mtxWorld);
			}

		}

		Player[1].m_posModel = D3DXVECTOR3(0.0f, 20.0f, 0.0f);			//���f���ׂ̍�������
		Player[2].m_posModel = D3DXVECTOR3(0.0f, 0.0f, 10.0f);

	}


	
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
void UninitModel(void)
{
	for (int i = 0; i < NUM_MODEL; i++)
	{
		if(PlayerModel[i].m_pD3DTextureModel != NULL)
		{
			// �e�N�X�`���̊J��
			PlayerModel[i].m_pD3DTextureModel->Release();
			PlayerModel[i].m_pD3DTextureModel = NULL;
		}

		if(PlayerModel[i].m_pD3DXMeshModel != NULL)
		{
			// ���b�V���̊J��
			PlayerModel[i].m_pD3DXMeshModel->Release();
			PlayerModel[i].m_pD3DXMeshModel = NULL;
		}

		if(PlayerModel[i].m_pD3DXBuffMatModel != NULL)
		{
			// �}�e���A���̊J��
			PlayerModel[i].m_pD3DXBuffMatModel->Release();
			PlayerModel[i].m_pD3DXBuffMatModel = NULL;
		}

		}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateModel(void)
{

	
	D3DXVECTOR3 speed;
	speed.x = 0.0f;
	speed.y = 0.0f;
	speed.z = 0.0f;

	D3DXVECTOR3 rot1;
	rot1.x = 0.0f;
	rot1.y = 0.0f;
	rot1.z = 0.0f;

	D3DXVECTOR3 Bulletpos;
	Bulletpos.x = 0.0f;
	Bulletpos.y = 0.0f;
	Bulletpos.z = 0.0f;

	D3DXVECTOR3 Bulletspeed;
	Bulletspeed.x = 0.0f;
	Bulletspeed.y = 0.0f;
	Bulletspeed.z = 0.0f;

	D3DXVECTOR3 ModelWorldmtxZ;
	ModelWorldmtxZ.x = 0.0f;
	ModelWorldmtxZ.y = 0.0f;
	ModelWorldmtxZ.z = 0.0f;

	

	rot.x = 0.0f;
	rot.y = 0.0f;
	rot.z = 0.0f;

	ReticleVector.x = 0.0f;
	ReticleVector.y = 0.0f;
	ReticleVector.z = 0.0f;

	ReticleLength.x = 0.0f;
	ReticleLength.y = 0.0f;
	ReticleLength.z = 0.0f;
	
	EffectVector.x = 0.0f;
	EffectVector.y = 0.0f;
	EffectVector.z = 0.0f;

	EffectLength.x = 0.0f;
	EffectLength.y = 0.0f;
	EffectLength.z = 0.0f;

	//�G�t�F�N�g���Z�b�g, �G�t�F�N�g�ւ̃|�C���^�ϐ�
	EffectVector.x = Player[0].m_virtualMtxRot._11;
	EffectVector.y = Player[0].m_virtualMtxRot._12;
	EffectVector.z = Player[0].m_virtualMtxRot._13;

	ModelWorldmtxZ.x = Player[0].m_virtualMtxRot._31;
	ModelWorldmtxZ.y = Player[0].m_virtualMtxRot._32;
	ModelWorldmtxZ.z = Player[0].m_virtualMtxRot._33;

	EffectLength.x = Player[0].m_mtxWorld._41;
	EffectLength.y = Player[0].m_mtxWorld._42;
	EffectLength.z = Player[0].m_mtxWorld._43;

	//�G�t�F�N�g�i�y���j�̐ݒ�
	D3DXVECTOR3 move;
	move.x = Player[0].m_virtualMtxRot._31;
	move.y = Player[0].m_virtualMtxRot._32;
	move.z = Player[0].m_virtualMtxRot._33;
	float fAngle, fLength;
	int nLife;
	float fSize;

				
	fAngle = (float)(rand() % 628 - 314) / 100.0f;			//�U��΂�͈́i�p�x�j
	fLength = rand() % (int)(5.0f * 20) / 10.0f - 5.0f;		//�U��΂�͈́i�����j
	move.x = sinf(fAngle) * 0.0f;							//
	move.y = rand() % 10 / 10.0f + 5.0f * 0.0f;
	move.z = 0.0f;
	nLife = rand() % 90 + 15;
	fSize = 5 + (float)(rand() * (10 - 5 + 1.0) / (1.0 + RAND_MAX));


	
	speed.x = Player[0].m_mtxRot._31;
	speed.y = Player[0].m_mtxRot._32;
	speed.z = Player[0].m_mtxRot._33;
	speed *= speedinertia;

	//�ԑ̂̈ړ�
	if (GetKeyboardPress(DIK_S) || IsButtonPressed(0, STICK_RIGHT) || IsButtonPressed(0, STICK_RIGHTUP) || IsButtonPressed(0, STICK_RIGHTDOWN))
	{
		speedinertia = speedinertia * PAR_INERTIA - SPEED_LIMIT * (1.0f - PAR_INERTIA);
	
		//�ړ��G�t�F�N�g�i�y���j
		SetEffect(Player[0].m_posModel + (EffectVector * 10) + (ModelWorldmtxZ * 20),
			move, D3DXCOLOR(0.45f, 0.30f, 0.18f, 1.0f), fSize, fSize, nLife);
		SetEffect(Player[0].m_posModel + (EffectVector * -10) + (ModelWorldmtxZ * 20),
			move, D3DXCOLOR(0.45f, 0.30f, 0.18f, 1.0f), fSize, fSize, nLife);
	}
	else if (GetKeyboardPress(DIK_W) || IsButtonPressed(0, STICK_LEFT) || IsButtonPressed(0, STICK_LEFTUP) || IsButtonPressed(0, STICK_LEFTDOWN))
	{

		speedinertia = speedinertia * PAR_INERTIA + SPEED_LIMIT * (1.0f - PAR_INERTIA);
		
		//�ړ��G�t�F�N�g�i�y���j
		SetEffect(Player[0].m_posModel + (EffectVector * 10) + (ModelWorldmtxZ * -20),
			move, D3DXCOLOR(0.45f, 0.30f, 0.18f, 1.0f), fSize, fSize, nLife);
		SetEffect(Player[0].m_posModel + (EffectVector * -10) + (ModelWorldmtxZ * -20),
			move, D3DXCOLOR(0.45f, 0.30f, 0.18f, 1.0f), fSize, fSize, nLife);
		//speed.x = Player[0].m_mtxRot._31;
		//speed.y = Player[0].m_mtxRot._32;
		//speed.z = Player[0].m_mtxRot._33;
	}
	else
	{
		speedinertia = speedinertia * PAR_INERTIA;
	}


	//���f����x�����ւ̌��E�ړ�����
	if (Player[0].m_posModel.x >= 470)
	{
		Player[0].m_posModel.x = 470;
	}
	if (Player[0].m_posModel.x <= -470)
	{
		Player[0].m_posModel.x = -470;
	}


	//�ԑ̂̉�]�@Y��
	if (!GetKeyboardPress(DIK_A) || !IsButtonPressed(0, STICK_DOWN))
	{
		if (GetKeyboardPress(DIK_D) || IsButtonPressed(0, STICK_UP) || IsButtonPressed(0, STICK_RIGHTUP) || IsButtonPressed(0, STICK_LEFTUP))
		{
			//rot.x = Player[0].m_mtxRot._21;
			rot.y += VALUE_ROTATE;
			//rot.z = Player[0].m_mtxRot._23;
			//rot *= +VALUE_ROTATE;
			if (rot.y >= D3DXToRadian(10.0f))
			{
				rot.y = D3DXToRadian(10.0f);
			}
			//�ړ��G�t�F�N�g�i�y���j
			SetEffect(Player[0].m_posModel + (EffectVector * 10) + (ModelWorldmtxZ * 20),
				move, D3DXCOLOR(0.45f, 0.30f, 0.18f, 1.0f), fSize, fSize, nLife);
			SetEffect(Player[0].m_posModel + (EffectVector * -10) + (ModelWorldmtxZ * 20),
				move, D3DXCOLOR(0.45f, 0.30f, 0.18f, 1.0f), fSize, fSize, nLife);
			//�ړ��G�t�F�N�g�i�y���j
			SetEffect(Player[0].m_posModel + (EffectVector * 10) + (ModelWorldmtxZ * -20),
				move, D3DXCOLOR(0.45f, 0.30f, 0.18f, 1.0f), fSize, fSize, nLife);
			SetEffect(Player[0].m_posModel + (EffectVector * -10) + (ModelWorldmtxZ * -20),
				move, D3DXCOLOR(0.45f, 0.30f, 0.18f, 1.0f), fSize, fSize, nLife);
		}
	}
	
	if (!GetKeyboardPress(DIK_D) || !IsButtonPressed(0, STICK_UP))
	{
		if (GetKeyboardPress(DIK_A) || IsButtonPressed(0, STICK_DOWN) || IsButtonPressed(0, STICK_RIGHTDOWN) || IsButtonPressed(0, STICK_LEFTDOWN))
		{
			//rot.x = Player[0].m_mtxRot._21;
			rot.y += -VALUE_ROTATE;
			//rot.z = Player[0].m_mtxRot._23;
			//rot *= -VALUE_ROTATE;
			if (rot.y <= D3DXToRadian(-10.0f))
			{
				rot.y = D3DXToRadian(-10.0f);
			}
			//�ړ��G�t�F�N�g�i�y���j
			SetEffect(Player[0].m_posModel + (EffectVector * 10) + (ModelWorldmtxZ * 20),
				move, D3DXCOLOR(0.45f, 0.30f, 0.18f, 1.0f), fSize, fSize, nLife);
			SetEffect(Player[0].m_posModel + (EffectVector * -10) + (ModelWorldmtxZ * 20),
				move, D3DXCOLOR(0.45f, 0.30f, 0.18f, 1.0f), fSize, fSize, nLife);
			//�ړ��G�t�F�N�g�i�y���j
			SetEffect(Player[0].m_posModel + (EffectVector * 10) + (ModelWorldmtxZ * -20),
				move, D3DXCOLOR(0.45f, 0.30f, 0.18f, 1.0f), fSize, fSize, nLife);
			SetEffect(Player[0].m_posModel + (EffectVector * -10) + (ModelWorldmtxZ * -20),
				move, D3DXCOLOR(0.45f, 0.30f, 0.18f, 1.0f), fSize, fSize, nLife);
		}
	}
	
	
	// �C��̉�]
	if (!GetKeyboardPress(DIK_LEFT) || !IsButtonPressed(1, STICK_UP))
	{
		if (GetKeyboardPress(DIK_RIGHT) || IsButtonPressed(1, STICK_DOWN) || IsButtonPressed(1, STICK_RIGHTDOWN) || IsButtonPressed(1, STICK_LEFTDOWN))
		{

			//rot1.x = Player[1].m_mtxRot._21;
			rot1.y += VALUE_ROTATE;
			//rot1.z = Player[1].m_mtxRot._23;
			//rot1 *= +VALUE_ROTATE;

		}
	}
	
	if (!GetKeyboardPress(DIK_RIGHT) || !IsButtonPressed(1, STICK_DOWN))
	{
		if (GetKeyboardPress(DIK_LEFT) || IsButtonPressed(1, STICK_UP) || IsButtonPressed(1, STICK_RIGHTUP) || IsButtonPressed(1, STICK_LEFTUP))
		{

			//rot1.x = Player[1].m_mtxRot._21;
			rot1.y += -VALUE_ROTATE;
			//rot1.z = Player[1].m_mtxRot._23;
			//rot1 *= -VALUE_ROTATE;

		}
	}
	
	//��C�̏㉺�^��
	if (GetKeyboardPress(DIK_UP) || IsButtonPressed(1, STICK_RIGHT) || IsButtonPressed(1, STICK_RIGHTDOWN) || IsButtonPressed(1, STICK_RIGHTUP))
	{
		
		rot2.x += -VALUE_ROTATE;
		//rot2.y = Player[2].m_mtxRot._12;
		//rot2.z = Player[2].m_mtxRot._13;
		//rot2 *= -VALUE_ROTATE;
		if (rot2.x <= D3DXToRadian(-30.0f))
		{
			rot2.x = D3DXToRadian(-30.0f);
		}

	}
	
	if (GetKeyboardPress(DIK_DOWN) || IsButtonPressed(1, STICK_LEFT) || IsButtonPressed(1, STICK_LEFTDOWN) || IsButtonPressed(1, STICK_LEFTUP))
	{
		rot2.x += VALUE_ROTATE;
		if (rot2.x >= D3DXToRadian(0.0f))
		{
			rot2.x = D3DXToRadian(0.0f);
		}
	}
	
	//���f���̈ʒu
	Player[0].m_posModel += speed;					
	//���f���̌���(��])g_posCameraP
	Player[0].m_rotModel += rot;					
	// ���f���̌���(��])g_posCameraP
	Player[1].m_rotModel += rot1;					
	Player[2].m_rotModel = rot2;


	// �e����
	if (GetKeyboardTrigger(DIK_SPACE) || IsButtonTriggered(1, BUTTON_ZR))
	{
		//�e�̔��ˌ�������C�̌����Ɠ��@����
		Bulletspeed.x = Player[2].m_virtualMtxRot._31;
		Bulletspeed.y = Player[2].m_virtualMtxRot._32;
		Bulletspeed.z = Player[2].m_virtualMtxRot._33;
		//�e�̔��ˈʒu����C�̈ʒu�ƍ��킹�邽�߂̓��@�ƒ���
		CazrelativeVector.x = Player[2].m_mtxWorld._41;
		CazrelativeVector.y = Player[2].m_mtxWorld._42;
		CazrelativeVector.z = Player[2].m_mtxWorld._43;
		Bulletpos += CazrelativeVector + Bulletspeed * 30.0f;
		//�e�̃X�s�[�h
		Bulletspeed *= 30.0f;		

		//�v���C���[�̈ʒu�ƒe�̃X�s�[�h��^���č쐬
		CreateBullet(Bulletpos, Bulletspeed);
		
	}
	
	//���e�B�N���i�Ə��j���Z�b�g, ���e�B�N���ւ̃|�C���^�ϐ�
	ReticleVector.x = Player[2].m_virtualMtxRot._31;
	ReticleVector.y = Player[2].m_virtualMtxRot._32;
	ReticleVector.z = Player[2].m_virtualMtxRot._33;
	//���e�B�N���i�Ə��j���Z�b�g, ���e�B�N���ւ̃|�C���^�ϐ�
	ReticleLength.x = Player[2].m_mtxWorld._41;
	ReticleLength.y = Player[2].m_mtxWorld._42;
	ReticleLength.z = Player[2].m_mtxWorld._43;




	

}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawModel(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	
	
	D3DXMATERIAL *pD3DXMat;
	D3DMATERIAL9 matDef;		//�}�e���A���\���̂̃|�C���^(D3DMATERIAL9�\����)

	// ���݂̃}�e���A�����擾
	pDevice->GetMaterial(&matDef);		//���݃Z�b�g����Ă���}�e���A�����擾���ĕۑ����Ă����B


	////��e���̖��G�G�t�F�N�g�i�_�Łj
	//if (GetKeyboardTrigger(DIK_SPACE))
	//{
	//	hit = true;
	//}
	//if (hit == true)
	//{
	//	invincibleTime = INVINCIBLE_TIME; 
	//	renderFlag = !renderFlag;
	//	hit = false;
	//}
	//if (invincibleTime > 0)
	//{
	//	invincibleTime -= GetFrameTime();
	//	blinkTime += GetFrameTime();
	//	if (blinkTime > BLINK_INTERVAL)
	//	{
	//		blinkTime = 0;
	//		renderFlag = !renderFlag;
	//	}
	//}
	//else
	//{
	//	renderFlag = true;
	//}
	

		//*************************�S�ẴL�����N�^�[��\��************************
		for (int i = 0; i < NUM_PLAYER; i++)
		{
			D3DXVECTOR3 temp;
			
			


			// ���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&Player[i].m_mtxWorld);
			D3DXMatrixIdentity(&Player[i].m_virtualMtxRot);


			// �X�P�[���𔽉f(�X�P�[�����s��ɂ��ă��[���h�s��(�͂��߂͒P�ʍs��)�Ə�Z)
			temp = Player[i].m_sclModel;
			D3DXMatrixScaling(&Player[i].m_mtxScl, temp.x, temp.y, temp.z);
			D3DXMatrixMultiply(&Player[i].m_mtxWorld, &Player[i].m_mtxWorld, &Player[i].m_mtxScl);

			// ��]�𔽉f(��]���s��ɂ��ă��[���h�s��Ə�Z)
			temp = Player[i].m_rotModel;
			D3DXMatrixRotationYawPitchRoll(&Player[i].m_mtxRot, temp.y, temp.x, temp.z);
			D3DXMatrixMultiply(&Player[i].m_mtxWorld, &Player[i].m_mtxWorld, &Player[i].m_mtxRot);

			// �ړ��𔽉f(�ړ����s��ɂ��ă��[���h�s��Ə�Z)
			temp = Player[i].m_posModel;
			D3DXMatrixTranslation(&Player[i].m_mtxTranslate, temp.x, temp.y, temp.z);
			D3DXMatrixMultiply(&Player[i].m_mtxWorld, &Player[i].m_mtxWorld, &Player[i].m_mtxTranslate);

			//MAINTANK��e�ݒ�ɂ���
			D3DXMatrixMultiply(&Player[i].m_virtualMtxRot, &Player[i].m_virtualMtxRot, &Player[i].m_mtxRot);
			for (int j = i; j >= 0; j--)
			{
				if (Player[j].m_mtxRotParent != NULL)
				{
					D3DXMatrixMultiply(&Player[i].m_virtualMtxRot, &Player[i].m_virtualMtxRot, Player[j].m_mtxRotParent); //���z��]�s��Ɏq�Ɛe�̉�]�s�����Z�������ʂ��i�[����
				}
			}
			if (Player[i].m_mtxParent != NULL)
			{
				D3DXMatrixMultiply(&Player[i].m_mtxWorld, &Player[i].m_mtxWorld, Player[i].m_mtxParent);
			}
			// ���[���h�}�g���b�N�X�̐ݒ�(�����������[���h�}�g���b�N�X��DirectX�փZ�b�g)
			pDevice->SetTransform(D3DTS_WORLD, &Player[i].m_mtxWorld);

			// ���̃L�����N�^�[���g�����f����ID
			int modelno = Player[i].m_ModelID;


			// ���f���̃}�e���A�����ɑ΂���|�C���^���擾
			pD3DXMat = (D3DXMATERIAL*)PlayerModel[modelno].m_pD3DXBuffMatModel->GetBufferPointer();		//X�t�@�C������擾�����}�e���A���\���̔z��̐擪�̃|�C���^���擾����B

			// ���f���̏��ɏ]���ă��f�����\������I�u�W�F�N�g�̐������J��Ԃ�
			for (int nCntMat = 0; nCntMat < (int)PlayerModel[modelno].m_nNumMatModel; nCntMat++)		//�擾�����}�e���A���̐��������[�v
			{
				// �I�u�W�F�N�g�}�e���A���̐ݒ�@�}�e���A����DirectX�փZ�b�g
				pDevice->SetMaterial(&pD3DXMat[nCntMat].MatD3D);		//�\���̔z��̃}�e���A���f�[�^���f�o�C�X�փZ�b�g

				// �I�u�W�F�N�g�e�N�X�`���̐ݒ�
				pDevice->SetTexture(0, PlayerModel[modelno].m_pD3DTextureModel);

				if (renderFlag)
				{
						// �I�u�W�F�N�g���b�V���̕`��
						PlayerModel[modelno].m_pD3DXMeshModel->DrawSubset(nCntMat);
				}
			}

		}
	

	// �}�e���A�����f�t�H���g�ɖ߂�
	pDevice->SetMaterial(&matDef);

}

void SetParent(int i, D3DXMATRIX* newParent)
{
	Player[i].m_mtxParent = newParent;
}

void SetRotParent(int i, D3DXMATRIX* newParent)
{
	Player[i].m_mtxRotParent = newParent;
}




OBJECT3D *GetPlayer(void)
{
	return(&Player[0]);
}

MODEL3D *GetModel(void)
{
	return(&PlayerModel[0]);
}

OBJECT3D *GetPlayer1(void)
{
	return(&Player[1]);
}

OBJECT3D *GetPlayer2(void)
{
	return(&Player[2]);
}

D3DXVECTOR3 *GetReticleVector(void)
{
	return(&ReticleVector);
}

D3DXVECTOR3 *GetReticleLength(void)
{
	return(&ReticleLength);
}

D3DXVECTOR3 *GetEffectVector(void)
{
	return(&EffectVector);
}

D3DXVECTOR3 *GetEffectLength(void)
{
	return(&EffectLength);
}