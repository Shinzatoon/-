#pragma once
//=============================================================================
// ゲーム画面処理 [game.h]
// Author : 新里　将士
//
//=============================================================================
#pragma once

#ifndef _GAME_H_
#define _GAME_H_

#include "main.h"

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitGame(void);
void UninitGame(void);
void UpdateGame(void);
void DrawGame(void);


#endif
