#pragma once
//============================================================================
// エネミーの弾処理 [enemyBullet.h]
// Author : 新里　将士
//
//=============================================================================

#pragma once

#ifndef __ENEMY_BULLET_H_
#define __ENEMY_BULLET_H_

#include "main.h"



//===========================
//プロトタイプ宣言

void InitEnemyBullet();
void DrawEnemyBullet(D3DXMATRIX cameraView);
void UpdateEnemyBullet();
void UninitEnemyBullet();

//弾構造体
#define		NUM_ENEMY_BULLET  (5000)


void CreateEnemyBullet(D3DXVECTOR3 pos, D3DXVECTOR3 speed);


OBJECT3D* GetParentEnemyBullet(void);

OBJECT3D* GetEnemyBullet();//構造体の先頭ポインタを取得

#endif