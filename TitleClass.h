//=============================================================================
// �^�C�g����ʏ��� [title.h]
// Author :  �V���@���m
//
//=============================================================================
#pragma once

#include "main.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	TEXTURE_TITLE		"data/TEXTURE/titleBg.jpg"		// �ǂݍ��ރe�N�X�`���t�@�C����
#define	TEXTURE_TITLE_LOGO	"data/TEXTURE/title.png"	// �ǂݍ��ރe�N�X�`���t�@�C����
#define	TEXTURE_LOGO_START	"data/TEXTURE/press_enter.png"	// �ǂݍ��ރe�N�X�`���t�@�C����

#define	TITLE_LOGO_POS_X		(200)		// �^�C�g�����S�̈ʒu(X���W)
#define	TITLE_LOGO_POS_Y		(40)		// �^�C�g�����S�̈ʒu(Y���W)
#define	TITLE_LOGO_WIDTH		(900)		// �^�C�g�����S�̕�
#define	TITLE_LOGO_HEIGHT		(440)		// �^�C�g�����S�̍���

#define	START_POS_X				(400)		// �X�^�[�g�{�^���̈ʒu(X���W)
#define	START_POS_Y				(720)		// �X�^�[�g�{�^���̈ʒu(Y���W)
#define	START_WIDTH				(480)		// �X�^�[�g�{�^���̕�
#define	START_HEIGHT			(120)		// �X�^�[�g�{�^���̍���

#define	COUNT_APPERA_START		(60)		// �X�^�[�g�{�^���o���܂ł̎���
#define	INTERVAL_DISP_START		(60)		// �X�^�[�g�{�^���_�ł̎���

#define	COUNT_WAIT_DEMO			(60 * 5)	// �f���܂ł̑҂�����


//*****************************************************************************
// �N���X�錾
//*****************************************************************************
class TitleClass
{
private:
	LPDIRECT3DTEXTURE9		g_pD3DTextureTitle = NULL;		// �e�N�X�`���ւ̃|�C���^
	LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuffTitle = NULL;		// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
	LPDIRECT3DTEXTURE9		g_pD3DTextureTitleLogo = NULL;	// �e�N�X�`���ւ̃|�C���^
	LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuffTitleLogo = NULL;	// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
	LPDIRECT3DTEXTURE9		g_pD3DTextureStart = NULL;		// �e�N�X�`���ւ̃|�C���^
	LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuffStart = NULL;		// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
	int						g_nCountAppearStart = 0;		//
	float					g_fAlphaLogo = 0.0f;			//
	int						g_nCountDisp = 0;				//
	bool					g_bDispStart = false;			//
	int						g_nConutDemo = 0;				//
public:
	TitleClass();
	~TitleClass();

	HRESULT MakeVertexTitle(LPDIRECT3DDEVICE9 pDevice);
	void SetColorTitleLogo(void);

	HRESULT InitTitle(void);
	void UninitTitle(void);
	void UpdateTitle(void);
	void DrawTitle(void);
};

