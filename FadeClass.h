//=============================================================================
// �t�F�[�h���� [fade.h]
// Author : �V���@���m
//
//=============================================================================
#pragma once

#include "main.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	FADE_RATE		(0.01f)		// �t�F�[�h�W��

//�t�F�[�h�̏�Ԏ��
enum class FADE
{
	FADE_NONE = 0,		// �����Ȃ����
	FADE_IN,			// �t�F�[�h�C������
	FADE_OUT,			// �t�F�[�h�A�E�g����
	FADE_MAX
};

//*****************************************************************************
// �N���X�錾
//*****************************************************************************
class FadeClass
{
private:

	LPDIRECT3DTEXTURE9		g_p3DTextureFade = NULL;	// �e�N�X�`���ւ̃|�C���^
	LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuffFade = NULL;	// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
	D3DXCOLOR				g_color;
	FADE					g_fade = FADE::FADE_IN;
	
public:
	FadeClass();
	~FadeClass();

	HRESULT MakeVertexFade(LPDIRECT3DDEVICE9 pDevice);
	void SetColor(D3DCOLOR col);
	
	HRESULT InitFade(void);
	void UninitFade(void);
	void UpdateFade(void);
	void DrawFade(void);

	void SetFade(FADE fade);
	FADE GetFade(void);
	
};


FadeClass* getFadeMain();
