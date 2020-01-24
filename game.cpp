//=============================================================================
// �Q�[����ʏ��� [game.cpp]
// Author : �V���@���m
//
//=============================================================================
#include "game.h"
#include "bullet.h"
#include "model.h"
#include "enemy_A.h"
#include "enemy_B.h"
#include "particle.h"
#include "reticle.h"
#include "effect.h"
#include "enemyBullet.h"
#include "input.h"
#include "WallClass.h"
#include "FieldClass.h"
#include "LightClass.h"
#include "BackGroundClass.h"
#include "FadeClass.h"
#include "CameraClass.h"
#include "ReticleClass.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
float gameProgressTime;		//frameTime�v���ϐ�

WallClass wall;
FieldClass field;
LightClass light;
BackGroundClass bg;
//BulletClass bullet;
CameraClass camera;
ReticleClass reticle;
//=============================================================================
// ����������
//=============================================================================
HRESULT InitGame(void)
{
	
	FadeClass* fade = getFadeMain();
	
	//�Q�[�����o�ߎ��Ԃ̏�����
	gameProgressTime = 0.0f;

	// ���C�g�̏�����
	light.InitLight();

	// �J�����̏�����
	camera.InitCamera();
	camera.setTargetPostion(&reticle.reticlePos);
	camera.setTargetDirection(&reticle.reticleRot);

	// �e�̏�����
	InitBullet();
	//bullet.InitBullet();

	//�G�l�~�[�̒e
	InitEnemyBullet();

	// ���f���̏�����
	InitModel();

	// �t�B�[���h�̏�����
	//InitField();
	field.InitField();

	// �w�i�̏�����
	//InitBG();
	bg.InitBG();

	// �G�l�~�[�̏�����
	InitEnemy_A();
	InitEnemy_B();

	// �p�[�e�B�N���̏�����
	InitParticle();

	//�E�H�[���̏�����
	//InitWall();
	wall.InitWall();

	// ���e�B�N���̏�����
	//InitReticle();
	reticle.InitReticle();

	//�G�t�F�N�g
	InitEffect();

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitGame(void)
{
	//���C�g�̏I������
	//UninitLight();


	//�J�����̏I������
	//UninitCamera();


	//���f���̏I������
	UninitModel();

	//�t�B�[���h�̏I������
	//UninitField();
	field.UninitField();

	//�w�i�̏I������
	//UninitBG();
	bg.UninitBG();

	//�G�l�~�[�̏I������
	//UninitEnemy();
	UninitEnemy_A();
	UninitEnemy_B();
	
	//�e�̏I������
	UninitBullet();
	//bullet.UninitBullet();
	UninitEnemyBullet();

	//�p�[�e�B�N���̏I������
	UninitParticle();

	//�E�H�[���̏I������
	//UninitWall();
	wall.UninitWall();

	//���e�B�N���̏I������
	//UninitReticle();
	reticle.UninitReticle();

	//�G�t�F�N�g�̏I������
	UninitEffect();

	
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateGame(void)
{
	FadeClass* fade = getFadeMain();

	//frameTime�𑫂������
	gameProgressTime += GetFrameTime();	

	// �J�����X�V
	//UpdateCamera();
	camera.UpdateCamera();

	// �e�����̍X�V
	UpdateBullet();
	//bullet.UpdateBullet();
	UpdateEnemyBullet();

	// ���f���̍X�V����
	UpdateModel();

	// �t�B�[���h�̍X�V����
	//UpdateField();
	field.UpdateField();

	// �w�i�̍X�V����
	//UpdateBG();
	bg.UpdateBG();

	// �G�l�~�[�̍X�V����
	UpdateEnemy_A();
	UpdateEnemy_B();

	//�E�H�[���̍X�V����
	//UpdateWall();
	wall.UpdateWall();

	// ���e�B�N���̍X�V����
	//UpdateReticle();
	reticle.UpdateReticle();

	//�p�[�e�B�N���̍X�V����
	UpdateParticle();

	//�G�t�F�N�g�̍X�V����
	UpdateEffect();

	//�Q�[�����Ԃ�100f�𒴂�����Q�[���I��
	if (gameProgressTime > 100.0f)	
	{
		fade->SetFade(FADE::FADE_OUT);
	}

}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawGame(void)
{
	
	//�J�����̐ݒ�
	//DrawCamera();
	camera.DrawCamera();

	//���f���̕`�揈��
	DrawModel();
	
	//�t�B�[���h�̕`�揈��
	//DrawField();
	field.DrawField();

	//�w�i�̕`�揈��
	//DrawBG();
	bg.DrawBG();

	//�G�l�~�[�̕`�揈��
	DrawEnemy_A();
	DrawEnemy_B();

	//�e�����̕`��
	DrawBullet(camera.g_mtxView);
	//bullet.DrawBullet(camera.g_mtxView);
	DrawEnemyBullet(camera.g_mtxView);

	//�p�[�e�B�N���̕`�揈��
	DrawParticle(camera.g_mtxView);

	//�E�H�[���̕`�揈��
	//DrawWall();
	wall.DrawWall();

	//���e�B�N���̕`��
	//DrawReticle();
	reticle.DrawReticle(camera.g_mtxView);

	//�G�t�F�N�g�̕`��
	DrawEffect(camera.g_mtxView);

	//�Q�[���V�[���^�C���̕`��
	DrawGameTime(gameProgressTime);

}


