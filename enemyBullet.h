#pragma once
//============================================================================
// �G�l�~�[�̒e���� [enemyBullet.h]
// Author : �V���@���m
//
//=============================================================================

#pragma once

#ifndef __ENEMY_BULLET_H_
#define __ENEMY_BULLET_H_

#include "main.h"



//===========================
//�v���g�^�C�v�錾

void InitEnemyBullet();
void DrawEnemyBullet(D3DXMATRIX cameraView);
void UpdateEnemyBullet();
void UninitEnemyBullet();

//�e�\����
#define		NUM_ENEMY_BULLET  (5000)


void CreateEnemyBullet(D3DXVECTOR3 pos, D3DXVECTOR3 speed);


OBJECT3D* GetParentEnemyBullet(void);

OBJECT3D* GetEnemyBullet();//�\���̂̐擪�|�C���^���擾

#endif