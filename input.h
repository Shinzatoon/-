#pragma once
//=============================================================================
// ���͏��� [input.h]
// Author : �V���@���m
//
//=============================================================================
#ifndef _INPUT_H_
#define _INPUT_H_

#include "main.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************

// �v���O������������Ƃ��Ɏg��
#define	USE_KEYBOARD										// �錾����ƃL�[�{�[�h�ő���\�ɂȂ�
#define	USE_MOUSE											// �錾����ƃ}�E�X�ő���\�ɂȂ�
#define	USE_PAD												// �錾����ƃp�b�h�ő���\�ɂȂ�


/* game pad��� */
#define BUTTON_UP		0x00000001l	// �����L�[��(.IY<0)
#define BUTTON_DOWN		0x00000002l	// �����L�[��(.IY>0)
#define BUTTON_LEFT		0x00000004l	// �����L�[��(.IX<0)
#define BUTTON_RIGHT	0x00000008l	// �����L�[�E(.IX>0)
#define BUTTON_A		0x00000010l	// �`�{�^��(.rgbButtons[0]&0x80)
#define BUTTON_B		0x00000020l	// �a�{�^��(.rgbButtons[1]&0x80)
#define BUTTON_C		0x00000040l	// �b�{�^��(.rgbButtons[2]&0x80)
#define BUTTON_X		0x00000080l	// �w�{�^��(.rgbButtons[3]&0x80)
#define BUTTON_Y		0x00000100l	// �x�{�^��(.rgbButtons[4]&0x80)
#define BUTTON_Z		0x00000200l	// �y�{�^��(.rgbButtons[5]&0x80)
#define BUTTON_L		0x00000400l	// �k�{�^��(.rgbButtons[6]&0x80)
#define BUTTON_R		0x00000800l	// �q�{�^��(.rgbButtons[7]&0x80)
#define BUTTON_START	0x00001000l	// �r�s�`�q�s�{�^��(.rgbButtons[8]&0x80)
#define BUTTON_M		0x00002000l	// �l�{�^��(.rgbButtons[9]&0x80)
#define GAMEPADMAX		4			// �����ɐڑ�����W���C�p�b�h�̍ő吔���Z�b�g
#define BUTTON_LR		0x00004000l
#define BUTTON_ZR		0x00008000l

#define STICK_UP		0x00010000l
#define STICK_RIGHT		0x00020000l
#define STICK_DOWN		0x00040000l
#define STICK_LEFT		0x00080000l

#define STICK_RIGHTUP		0x00100000l
#define STICK_RIGHTDOWN		0x00200000l
#define STICK_LEFTDOWN		0x00400000l
#define STICK_LEFTUP		0x00800000l


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitInput(HINSTANCE hInst, HWND hWnd);
void UninitInput(void);
void UpdateInput(void);

bool GetKeyboardPress(int nKey);
bool GetKeyboardTrigger(int nKey);
bool GetKeyboardRepeat(int nKey);
bool GetKeyboardRelease(int nKey);

//---------------------------- mouse
BOOL IsMouseLeftPressed(void);      // ���N���b�N�������
BOOL IsMouseLeftTriggered(void);    // ���N���b�N�����u��
BOOL IsMouseRightPressed(void);     // �E�N���b�N�������
BOOL IsMouseRightTriggered(void);   // �E�N���b�N�����u��
BOOL IsMouseCenterPressed(void);    // ���N���b�N�������
BOOL IsMouseCenterTriggered(void);  // ���N���b�N�����u��
long GetMouseX(void);               // �}�E�X��X�����ɓ��������Βl
long GetMouseY(void);               // �}�E�X��Y�����ɓ��������Βl
long GetMouseZ(void);               // �}�E�X�z�C�[�������������Βl

//---------------------------- game pad
BOOL IsButtonPressed(int padNo, DWORD button);
BOOL IsButtonTriggered(int padNo, DWORD button);


enum POV
{
	UP = 0,
	RIGHTUP = 4500,
	RIGHT = 9000,
	RIGHTDOWN = 13500,
	DOWN = 18000,
	LEFTDOWN = 22500,
	LEFT = 27000,
	LEFTUP = 31500,
};

#endif