//=============================================================================
// �p�[�e�B�N������ [particle.cpp]
// Author : �V���@���m
//
//=============================================================================
#include "particle.h"
#include "input.h"
#include "CameraClass.h"


//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	TEXTURE_PARTICLE	"data/TEXTURE/EffectNew.png"	// �ǂݍ��ރe�N�X�`���t�@�C����
#define	PARTICLE_SIZE_X		(20.0f)							// �r���{�[�h�̕�
#define	PARTICLE_SIZE_Y		(20.0f)							// �r���{�[�h�̍���
#define	VALUE_MOVE_PARTICLE	(2.0f)							// �ړ����x

#define	MAX_PARTICLE			(1080)						// �r���{�[�h�ő吔

#define	DISP_SHADOW				// �e�̕\��
//#undef DISP_SHADOW

//*****************************************************************************
// �\���̒�`
//*****************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;		// �ʒu
	D3DXVECTOR3 rot;		// ��]
	D3DXVECTOR3 scale;		// �X�P�[��
	D3DXVECTOR3 move;		// �ړ���
	D3DXCOLOR col;			// �F
	float fSizeX;			// ��
	float fSizeY;			// ����
	int nIdxShadow;			// �eID
	int nLife;				// ����
	bool bUse;				// �g�p���Ă��邩�ǂ���

	

} PARTICLE;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT MakeVertexParticle(LPDIRECT3DDEVICE9 pDevice);
void SetVertexParticle(int nIdxParticle, float fSizeX, float fSizeY);
void SetColorParticle(int nIdxParticle, D3DXCOLOR col);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pD3DTextureParticle = NULL;	// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuffParticle = NULL;	// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^

D3DXMATRIX				g_mtxWorldParticle;				// ���[���h�}�g���b�N�X

PARTICLE				g_aParticle[MAX_PARTICLE];		// �p�[�e�B�N�����[�N
D3DXVECTOR3				g_posBase;						// �r���{�[�h�����ʒu
float					g_fWidthBase = 5.0f;			// ��̕�
float					g_fHeightBase = 5.0f;			// ��̍���
bool					g_bPause = false;				// �|�[�YON/OFF

bool					g_PAlpaTest;					// �A���t�@�e�X�gON/OFF
int						g_PnAlpha;						// �A���t�@�e�X�g��臒l

//CAMERA					*PcameraWK;

//=============================================================================
// ����������
//=============================================================================
HRESULT InitParticle(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���_���̍쐬
	MakeVertexParticle(pDevice);

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,						// �f�o�C�X�ւ̃|�C���^
		TEXTURE_PARTICLE,			// �t�@�C���̖��O
		&g_pD3DTextureParticle);	// �ǂݍ��ރ������[

	for (int nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++)
	{
		g_aParticle[nCntParticle].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aParticle[nCntParticle].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aParticle[nCntParticle].scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		g_aParticle[nCntParticle].move = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		g_aParticle[nCntParticle].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		g_aParticle[nCntParticle].fSizeX = PARTICLE_SIZE_X;
		g_aParticle[nCntParticle].fSizeY = PARTICLE_SIZE_Y;
		g_aParticle[nCntParticle].nIdxShadow = -1;
		g_aParticle[nCntParticle].nLife = 0;
		g_aParticle[nCntParticle].bUse = false;
	}

	g_posBase = D3DXVECTOR3(0.0f, 0.0f, 500.0f);

	//g_PnAlpha = 0x66;
	g_PAlpaTest = true;

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitParticle(void)
{
	if (g_pD3DTextureParticle != NULL)
	{// �e�N�X�`���̊J��
		g_pD3DTextureParticle->Release();
		g_pD3DTextureParticle = NULL;
	}

	if (g_pD3DVtxBuffParticle != NULL)
	{// ���_�o�b�t�@�̊J��
		g_pD3DVtxBuffParticle->Release();
		g_pD3DVtxBuffParticle = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateParticle(void)
{	
	 
	//D3DXVECTOR3 rotCamera;

	//PcameraWK = GetMtxView();
	 
	//CameraClass camera;

	// �J�����̉�]���擾
	//rotCamera = PcameraWK->relativeVector;
	 
	//if (GetKeyboardPress(DIK_LEFT))
	//{
	//	if (GetKeyboardPress(DIK_UP))
	//	{// ���O�ړ�
	//		g_posBase.x -= sinf(rotCamera.y + D3DX_PI * 0.75f) * VALUE_MOVE_PARTICLE;
	//		g_posBase.z -= cosf(rotCamera.y + D3DX_PI * 0.75f) * VALUE_MOVE_PARTICLE;
	//	}
	//	else if (GetKeyboardPress(DIK_DOWN))
	//	{// ����ړ�
	//		g_posBase.x -= sinf(rotCamera.y + D3DX_PI * 0.25f) * VALUE_MOVE_PARTICLE;
	//		g_posBase.z -= cosf(rotCamera.y + D3DX_PI * 0.25f) * VALUE_MOVE_PARTICLE;
	//	}
	//	else
	//	{// ���ړ�
	//		g_posBase.x -= sinf(rotCamera.y + D3DX_PI * 0.50f) * VALUE_MOVE_PARTICLE;
	//		g_posBase.z -= cosf(rotCamera.y + D3DX_PI * 0.50f) * VALUE_MOVE_PARTICLE;
	//	}
	//}
	//else if (GetKeyboardPress(DIK_RIGHT))
	//{
	//	if (GetKeyboardPress(DIK_UP))
	//	{// �E�O�ړ�
	//		g_posBase.x -= sinf(rotCamera.y - D3DX_PI * 0.75f) * VALUE_MOVE_PARTICLE;
	//		g_posBase.z -= cosf(rotCamera.y - D3DX_PI * 0.75f) * VALUE_MOVE_PARTICLE;
	//	}
	//	else if (GetKeyboardPress(DIK_DOWN))
	//	{// �E��ړ�
	//		g_posBase.x -= sinf(rotCamera.y - D3DX_PI * 0.25f) * VALUE_MOVE_PARTICLE;
	//		g_posBase.z -= cosf(rotCamera.y - D3DX_PI * 0.25f) * VALUE_MOVE_PARTICLE;
	//	}
	//	else
	//	{// �E�ړ�
	//		g_posBase.x -= sinf(rotCamera.y - D3DX_PI * 0.50f) * VALUE_MOVE_PARTICLE;
	//		g_posBase.z -= cosf(rotCamera.y - D3DX_PI * 0.50f) * VALUE_MOVE_PARTICLE;
	//	}
	//}
	//else if (GetKeyboardPress(DIK_UP))
	//{// �O�ړ�
	//	g_posBase.x -= sinf(D3DX_PI + rotCamera.y) * VALUE_MOVE_PARTICLE;
	//	g_posBase.z -= cosf(D3DX_PI + rotCamera.y) * VALUE_MOVE_PARTICLE;
	//}
	//else if (GetKeyboardPress(DIK_DOWN))
	//{// ��ړ�
	//	g_posBase.x -= sinf(rotCamera.y) * VALUE_MOVE_PARTICLE;
	//	g_posBase.z -= cosf(rotCamera.y) * VALUE_MOVE_PARTICLE;
	//}
	 
	if (GetKeyboardPress(DIK_1))
	{
		g_fWidthBase -= 0.1f;
		if (g_fWidthBase < 2.0f)
		{
			g_fWidthBase = 2.0f;
		}
	}
	if (GetKeyboardPress(DIK_2))
	{
		g_fWidthBase += 0.1f;
		if (g_fWidthBase > 10.0f)
		{
			g_fWidthBase = 10.0f;
		}
	}
	if (GetKeyboardPress(DIK_3))
	{
		g_fHeightBase -= 0.1f;
		if (g_fHeightBase < 5.0f)
		{
			g_fHeightBase = 5.0f;
		}
	}
	if (GetKeyboardPress(DIK_4))
	{
		g_fHeightBase += 0.1f;
		if (g_fHeightBase > 15.0f)
		{
			g_fHeightBase = 15.0f;
		}
	}
	
	if (GetKeyboardTrigger(DIK_P))
	{
		g_bPause = g_bPause ? false : true;
	}
	 
	if (g_bPause == false)
	{
		for (int nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++)
		{
			if (g_aParticle[nCntParticle].bUse)
			{// �g�p��
				g_aParticle[nCntParticle].pos.x += g_aParticle[nCntParticle].move.x;
				g_aParticle[nCntParticle].pos.z += g_aParticle[nCntParticle].move.z;

				g_aParticle[nCntParticle].pos.y += g_aParticle[nCntParticle].move.y;
				if (g_aParticle[nCntParticle].pos.y <= g_aParticle[nCntParticle].fSizeY / 2)
				{// ���n����
					g_aParticle[nCntParticle].pos.y = g_aParticle[nCntParticle].fSizeY / 2;
					g_aParticle[nCntParticle].move.y = -g_aParticle[nCntParticle].move.y * 0.75f;
				}

				g_aParticle[nCntParticle].move.x += (0.0f - g_aParticle[nCntParticle].move.x) * 0.15f;
				g_aParticle[nCntParticle].move.y -= 1.25f;
				g_aParticle[nCntParticle].move.z += (0.0f - g_aParticle[nCntParticle].move.z) * 0.15f;


				g_aParticle[nCntParticle].nLife--;
				if (g_aParticle[nCntParticle].nLife <= 0)
				{
					g_aParticle[nCntParticle].bUse = false;
					//DeleteShadow(g_aParticle[nCntParticle].nIdxShadow);
					//g_aParticle[nCntParticle].nIdxShadow = -1;
				}
				else
				{
					/*if (g_aParticle[nCntParticle].nLife <= 80)
					{
						g_aParticle[nCntParticle].col.r = 0.00f - (float)(80 - g_aParticle[nCntParticle].nLife) / 8.0f * 0.06f + 0.2f;
						g_aParticle[nCntParticle].col.g = 1.0f - (float)(80 - g_aParticle[nCntParticle].nLife) / 8.0f * 0.07f;
						g_aParticle[nCntParticle].col.b = 0.00f;
					}*/

					if (g_aParticle[nCntParticle].nLife <= 80)
					{
						//g_aParticle[nCntParticle].col.r = 0.0f; //- (float)(80 - g_aParticle[nCntParticle].nLife) / 8.0f * 0.06f + 0.2f;
						//g_aParticle[nCntParticle].col.g = 0.0f; //- (float)(80 - g_aParticle[nCntParticle].nLife) / 8.0f * 0.07f;
						//g_aParticle[nCntParticle].col.b = 0.0f;
						//g_aParticle[nCntParticle].col.a = 0.5f;
					}

					if (g_aParticle[nCntParticle].nLife <= 20)
					{
						// ���l�ݒ�
						/*g_aParticle[nCntParticle].col.a -= 0.05f;
						if (g_aParticle[nCntParticle].col.a < 0.0f)
						{
							g_aParticle[nCntParticle].col.a = 0.0f;
						}*/
					}

					// �F�̐ݒ�
					SetColorParticle(nCntParticle, g_aParticle[nCntParticle].col);
				}
			}
		}

		// �p�[�e�B�N������
		if ((rand() % 2) == 0)
		{
			D3DXVECTOR3 pos;
			D3DXVECTOR3 move;
			float fAngle, fLength;
			int nLife;
			float fSize;

			pos = g_posBase;

			//fAngle = rand() % 2;		//�U��΂�͈́i�p�x�j
			fAngle = (float)(rand() % 628 - 314) / 100.0f;
			fLength = rand() % (int)(g_fWidthBase * 20) / 10.0f - g_fWidthBase;			////�U��΂�͈́i�����j
			move.x = sinf(fAngle) * fLength;
			move.y = rand() % 10 / 10.0f + g_fHeightBase;
			move.z = cosf(fAngle) * fLength;

			nLife = rand() % 90 + 15;

			//�ŏ��l + (int)(rand() * (�ő�l - �ŏ��l + 1.0) / (1.0 + RAND_MAX))
			fSize = 5 + (float)(rand() * (10 - 5 + 1.0) / (1.0 + RAND_MAX));
			//fSize = (float)(rand() % 50 + 50);


			pos.y = fSize / 2;

			// �r���{�[�h�̐ݒ�
			//SetParticle(pos, move, D3DXCOLOR(0.0f, 1.00f, 0.0f, 1.0f), fSize, fSize, nLife);
		}



	}
			
	 
			
	 
#ifdef DISP_SHADOW
				//if (g_aParticle[nCntParticle].nIdxShadow != -1)
				//{// �e�g�p��
				//	float colA;
				//
				//	// �e�̈ʒu�ݒ�
				//	SetPositionShadow(g_aParticle[nCntParticle].nIdxShadow, D3DXVECTOR3(g_aParticle[nCntParticle].pos.x, 0.1f, g_aParticle[nCntParticle].pos.z));
				//
				//	// �e�̃��l�ݒ�
				//	if (g_aParticle[nCntParticle].col.a > 0.0f)
				//	{
				//		colA = (300.0f - (g_aParticle[nCntParticle].pos.y - 9.0f)) / (300.0f / g_aParticle[nCntParticle].col.a);
				//		if (colA < 0.0f)
				//		{
				//			colA = 0.0f;
				//		}
				//	}
				//	else
				//	{
				//		colA = 0.0f;
				//	}
				//
				//	// �e�̐F�̐ݒ�
				//	SetColorShadow(g_aParticle[nCntParticle].nIdxShadow, D3DXCOLOR(0.15f, 0.15f, 0.15f, colA));
				//}
#endif
			
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawParticle(D3DXMATRIX cameraView)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxView, mtxScale, mtxTranslate;

	//PcameraWK = GetMtxView();
	//CameraClass camera;

	// ���e�X�g��L����
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);		//FALSE��OFF
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0x0);

	// ���e�X�g�𖳌���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	// ���C�e�B���O�𖳌���
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	// �p�[�e�B�N�����ۂ��ݒ������
	pDevice->SetRenderState(D3DRS_POINTSPRITEENABLE, TRUE);

	// �u�����h��\���������ݒ�ɂȂ�悤�ɂ���
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);		// ���\�[�X�J���[�̎w��
	//pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);		// ���f�X�e�B�l�[�V�����J���[�̎w��
	//pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);	// ���\�[�X�J���[�̎w��
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);	// ���\�[�X�J���[�̎w��
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);		// ���f�X�e�B�l�[�V�����J���[�̎w��
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);//���s�����l���Ȃ�




	for (int nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++)
	{
		if (g_aParticle[nCntParticle].bUse)
		{
			// ���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_mtxWorldParticle);

			// �r���[�}�g���b�N�X���擾
			mtxView = cameraView;

			g_mtxWorldParticle._11 = cameraView._11;
			g_mtxWorldParticle._12 = cameraView._21;
			g_mtxWorldParticle._13 = cameraView._31;
			g_mtxWorldParticle._21 = cameraView._12;
			g_mtxWorldParticle._22 = cameraView._22;
			g_mtxWorldParticle._23 = cameraView._32;
			g_mtxWorldParticle._31 = cameraView._13;
			g_mtxWorldParticle._32 = cameraView._23;
			g_mtxWorldParticle._33 = cameraView._33;

			// �X�P�[���𔽉f
			D3DXMatrixScaling(&mtxScale, g_aParticle[nCntParticle].scale.x, g_aParticle[nCntParticle].scale.y, g_aParticle[nCntParticle].scale.z);
			D3DXMatrixMultiply(&g_mtxWorldParticle, &g_mtxWorldParticle, &mtxScale);

			// �ړ��𔽉f
			D3DXMatrixTranslation(&mtxTranslate, g_aParticle[nCntParticle].pos.x, g_aParticle[nCntParticle].pos.y, g_aParticle[nCntParticle].pos.z);
			D3DXMatrixMultiply(&g_mtxWorldParticle, &g_mtxWorldParticle, &mtxTranslate);

			// ���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldParticle);

			// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
			pDevice->SetStreamSource(0, g_pD3DVtxBuffParticle, 0, sizeof(VERTEX_3D));

			// ���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_3D);

			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pD3DTextureParticle);

			// �|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, (nCntParticle * 4), NUM_POLYGON);
		}
	}

	// ���C�e�B���O��L����
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	// �ʏ�u�����h�ɖ߂�
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// ���\�[�X�J���[�̎w��
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	// ���f�X�e�B�l�[�V�����J���[�̎w��
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
}

//=============================================================================
// ���_���̍쐬
//=============================================================================
HRESULT MakeVertexParticle(LPDIRECT3DDEVICE9 pDevice)
{
	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * NUM_VERTEX * MAX_PARTICLE,	// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
		D3DUSAGE_WRITEONLY,							// ���_�o�b�t�@�̎g�p�@�@
		FVF_VERTEX_3D,								// �g�p���钸�_�t�H�[�}�b�g
		D3DPOOL_MANAGED,							// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
		&g_pD3DVtxBuffParticle,					// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
		NULL)))										// NULL�ɐݒ�
	{
		return E_FAIL;
	}

	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D *pVtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		g_pD3DVtxBuffParticle->Lock(0, 0, (void**)&pVtx, 0);

		for (int nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++, pVtx += 4)
		{
			// ���_���W�̐ݒ�
			pVtx[0].vtx = D3DXVECTOR3(-PARTICLE_SIZE_X / 2, -PARTICLE_SIZE_Y / 2, 0.0f);
			pVtx[1].vtx = D3DXVECTOR3(PARTICLE_SIZE_X / 2, -PARTICLE_SIZE_Y / 2, 0.0f);
			pVtx[2].vtx = D3DXVECTOR3(-PARTICLE_SIZE_X / 2, PARTICLE_SIZE_Y / 2, 0.0f);
			pVtx[3].vtx = D3DXVECTOR3(PARTICLE_SIZE_X / 2, PARTICLE_SIZE_Y / 2, 0.0f);

			// �@���̐ݒ�
			pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
			pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
			pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
			pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

			// ���ˌ��̐ݒ�
			pVtx[0].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[1].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[2].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			// �e�N�X�`�����W�̐ݒ�
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
		}

		// ���_�f�[�^���A�����b�N����
		g_pD3DVtxBuffParticle->Unlock();
	}

	return S_OK;
}

//=============================================================================
// ���_���W�̐ݒ�
//=============================================================================
void SetVertexParticle(int nIdxParticle, float fSizeX, float fSizeY)
{
	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D *pVtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		g_pD3DVtxBuffParticle->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (nIdxParticle * 4);

		// ���_���W�̐ݒ�
		pVtx[0].vtx = D3DXVECTOR3(-fSizeX / 2, -fSizeY / 2, 0.0f);
		pVtx[1].vtx = D3DXVECTOR3(-fSizeX / 2, fSizeY / 2, 0.0f);
		pVtx[2].vtx = D3DXVECTOR3(fSizeX / 2, -fSizeY / 2, 0.0f);
		pVtx[3].vtx = D3DXVECTOR3(fSizeX / 2, fSizeY / 2, 0.0f);

		// ���_�f�[�^���A�����b�N����
		g_pD3DVtxBuffParticle->Unlock();
	}
}

//=============================================================================
// ���_�J���[�̐ݒ�
//=============================================================================
void SetColorParticle(int nIdxParticle, D3DXCOLOR col)
{
	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D *pVtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		g_pD3DVtxBuffParticle->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (nIdxParticle * 4);

		// ���_���W�̐ݒ�
			pVtx[0].diffuse = col;
			pVtx[1].diffuse = col;
			pVtx[2].diffuse = col;
			pVtx[3].diffuse = col;

		// ���_�f�[�^���A�����b�N����
		g_pD3DVtxBuffParticle->Unlock();
	}
}

//=============================================================================
// ���_���̍쐬
//=============================================================================
int SetParticle(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, float fSizeX, float fSizeY, int nLife)
{
	int nIdxParticle = -1;

	for (int nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++)
	{
		if (!g_aParticle[nCntParticle].bUse)
		{
			g_aParticle[nCntParticle].pos = pos;
			g_aParticle[nCntParticle].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_aParticle[nCntParticle].scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
			g_aParticle[nCntParticle].move = move;
			g_aParticle[nCntParticle].col = col;
			g_aParticle[nCntParticle].fSizeX = fSizeX;
			g_aParticle[nCntParticle].fSizeY = fSizeY;
			g_aParticle[nCntParticle].nLife = nLife;
			g_aParticle[nCntParticle].bUse = true;

			// ���_���W�̐ݒ�
			SetVertexParticle(nCntParticle, fSizeX, fSizeY);

			nIdxParticle = nCntParticle;

#ifdef DISP_SHADOW
			//// �e�̐ݒ�
			//g_aParticle[nCntParticle].nIdxShadow = SetShadow(D3DXVECTOR3(pos.x, 0.1f, pos.z), fSizeX, fSizeY);		// �e�̐ݒ�
			//if (g_aParticle[nCntParticle].nIdxShadow != -1)
			//{
			//	SetColorShadow(g_aParticle[nCntParticle].nIdxShadow, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f));
			//}
#endif

			break;
		}
	}

	return nIdxParticle;
}