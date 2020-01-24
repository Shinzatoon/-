//=============================================================================
// ���U���g��ʏ��� [result.h]
// Author : �V���@���m
//
//=============================================================================
#pragma once

#include "main.h"


//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	TEXTURE_RESULT		"data/TEXTURE/bg001.jpg"		// �ǂݍ��ރe�N�X�`���t�@�C����
#define	TEXTURE_RESULT_LOGO	"data/TEXTURE/result_logo.png"	// �ǂݍ��ރe�N�X�`���t�@�C����

#define	RESULT_LOGO_POS_X	(240)		// ���U���g���S�̈ʒu(X���W)
#define	RESULT_LOGO_POS_Y	(320)		// ���U���g���S�̈ʒu(Y���W)
#define	RESULT_LOGO_WIDTH	(800)		// ���U���g���S�̕�
#define	RESULT_LOGO_HEIGHT	(240)		// ���U���g���S�̍���

#define	COUNT_APPERA_RESULT	(60)		// ���U���g���S�o���܂ł̑҂�����	
#define	LIMIT_COUNT_WAIT	(60 * 5)	// �҂�����

//*****************************************************************************
// �N���X�錾
//*****************************************************************************
class ResultClass
{
private:
	LPDIRECT3DTEXTURE9		g_pD3DTextureResult = NULL;		// �e�N�X�`���ւ̃|�C���^
	LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuffResult = NULL;		// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
	LPDIRECT3DTEXTURE9		g_pD3DTextureResultLogo = NULL;	// �e�N�X�`���ւ̃|�C���^
	LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuffResultLogo = NULL;	// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
	int						g_nCountAppearResult = 0;		// �o���܂ł̑҂�����
	float					g_fAlphaResult = 0.0f;			// ���U���g���S�̃��l
	int						g_nCountWaitResult = 0;			// �҂�����
public:
	ResultClass();
	~ResultClass();

	HRESULT MakeVertexResult(LPDIRECT3DDEVICE9 pDevice);
	void SetColorResultLogo(void);

	HRESULT InitResult(void);
	void UninitResult(void);
	void UpdateResult(void);
	void DrawResult(void);
};

