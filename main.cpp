//=============================================================================
// ���C������ [main.cpp]
// Author : �V���@���m
//
//=============================================================================
#include "main.h"
#include "input.h"
#include "game.h"
#include "TitleClass.h"
#include "ResultClass.h"
#include "FadeClass.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define CLASS_NAME		"AppClass"			// �E�C���h�E�̃N���X��
#define WINDOW_NAME		"�C���Z�N�g�E�p���c�@�["		// �E�C���h�E�̃L���v�V������

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);
void Uninit(void);
void Update(void);
void Draw(void);
void DrawFPS(void);
#ifdef _DEBUG

#endif

//*****************************************************************************
// �O���[�o���ϐ�:
//*****************************************************************************

TitleClass title;
ResultClass result;
FadeClass fadeMain;
LPDIRECT3D9			g_pD3D = NULL;			// Direct3D �I�u�W�F�N�g
LPDIRECT3DDEVICE9	g_pD3DDevice = NULL;	// Device�I�u�W�F�N�g(�`��ɕK�v)
LPD3DXFONT			g_pD3DXFont = NULL;		// �t�H���g�ւ̃|�C���^
int					g_nCountFPS;			// FPS�J�E���^
int					g_Timecnt;				//�Q�[���i�s�J�E���^
int					frameMilliTime;			//�~���b
float				frameTime;				//�t���[���^�C��
float				progressTime = 0.0f;	//�o�ߎ���

#ifdef _DEBUG

#endif

MODE				g_mode = MODE_GAME;	// ���[�h

//=============================================================================
// ���C���֐�
//=============================================================================
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);	// �����Ă��ǂ����ǁA�x�����o��i���g�p�錾�j
	UNREFERENCED_PARAMETER(lpCmdLine);		// �����Ă��ǂ����ǁA�x�����o��i���g�p�錾�j

	DWORD dwExecLastTime;
	DWORD dwFPSLastTime;
	DWORD dwCurrentTime;
	DWORD dwFrameCount;

	WNDCLASSEX wcex =
	{
		sizeof(WNDCLASSEX),
		CS_CLASSDC,
		WndProc,
		0,
		0,
		hInstance,
		NULL,
		LoadCursor(NULL, IDC_ARROW),
		(HBRUSH)(COLOR_WINDOW + 1),
		NULL,
		CLASS_NAME,
		NULL
	};
	HWND hWnd;
	MSG msg;
	
	// �E�B���h�E�N���X�̓o�^
	RegisterClassEx(&wcex);

	// �E�B���h�E�̍쐬
	hWnd = CreateWindowEx(0,
						CLASS_NAME,
						WINDOW_NAME,
						WS_OVERLAPPEDWINDOW,
						CW_USEDEFAULT,
						CW_USEDEFAULT,
						SCREEN_WIDTH + GetSystemMetrics(SM_CXDLGFRAME) * 2,
						SCREEN_HEIGHT + GetSystemMetrics(SM_CXDLGFRAME) * 2 + GetSystemMetrics(SM_CYCAPTION),
						NULL,
						NULL,
						hInstance,
						NULL);

	// ����������(�E�B���h�E���쐬���Ă���s��)
	if(FAILED(Init(hInstance, hWnd, TRUE)))
	{
		return -1;
	}

	//�t���[���J�E���g������
	timeBeginPeriod(1);				// ����\��ݒ�
	dwExecLastTime = 
	dwFPSLastTime = timeGetTime();
	dwCurrentTime =
	dwFrameCount = 0;

	// �E�C���h�E�̕\��(�����������̌�ɌĂ΂Ȃ��Ƒʖ�)
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);
	
	// ���b�Z�[�W���[�v
	while(1)
	{
        if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if(msg.message == WM_QUIT)
			{// PostQuitMessage()���Ă΂ꂽ�烋�[�v�I��
				break;
			}
			else
			{
				// ���b�Z�[�W�̖|��ƃf�B�X�p�b�`
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
        }
		else
		{
			dwCurrentTime = timeGetTime();
			if((dwCurrentTime - dwFPSLastTime) >= 500)	// 0.5�b���ƂɎ��s
			{
#ifdef _DEBUG
				
#endif
				g_nCountFPS = dwFrameCount * 1000 / (dwCurrentTime - dwFPSLastTime);

				dwFPSLastTime = dwCurrentTime;
				dwFrameCount = 0;
			}

			if((dwCurrentTime - dwExecLastTime) >= (1000 / 60))
			{
				frameMilliTime = (dwCurrentTime - dwExecLastTime);

				frameTime = (float)frameMilliTime / 1000.0f;
				progressTime += frameTime;

				dwExecLastTime = dwCurrentTime;

				// �X�V����
				Update();

				// �`�揈��
				Draw();

				dwFrameCount++;
			}
		}
	}
	
	// �E�B���h�E�N���X�̓o�^������
	UnregisterClass(CLASS_NAME, wcex.hInstance);

	// �I������
	Uninit();

	timeEndPeriod(1);				// ����\��߂�

	return (int)msg.wParam;
}

//=============================================================================
// �v���V�[�W��
//=============================================================================
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:
		switch(wParam)
		{
		case VK_ESCAPE:
			DestroyWindow(hWnd);
			break;
		}
		break;

	default:
		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

//=============================================================================
// ����������
//=============================================================================
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	D3DPRESENT_PARAMETERS d3dpp;
    D3DDISPLAYMODE d3ddm;

	// Direct3D�I�u�W�F�N�g�̍쐬
	g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if(g_pD3D == NULL)
	{
		return E_FAIL;
	}

	// ���݂̃f�B�X�v���C���[�h���擾
    if(FAILED(g_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}

	// �f�o�C�X�̃v���[���e�[�V�����p�����[�^�̐ݒ�
	ZeroMemory(&d3dpp, sizeof(d3dpp));							// ���[�N���[���N���A
	d3dpp.BackBufferCount			= 1;						// �o�b�N�o�b�t�@�̐�
	d3dpp.BackBufferWidth			= SCREEN_WIDTH;				// �Q�[����ʃT�C�Y(��)
	d3dpp.BackBufferHeight			= SCREEN_HEIGHT;			// �Q�[����ʃT�C�Y(����)
	d3dpp.BackBufferFormat			= d3ddm.Format;				// �o�b�N�o�b�t�@�t�H�[�}�b�g�̓f�B�X�v���C���[�h�ɍ��킹�Ďg��
	d3dpp.SwapEffect				= D3DSWAPEFFECT_DISCARD;	// �f���M���ɓ������ăt���b�v����
	d3dpp.Windowed					= bWindow;					// �E�B���h�E���[�h
	d3dpp.EnableAutoDepthStencil	= TRUE;						// �f�v�X�o�b�t�@�i�y�o�b�t�@�j�ƃX�e���V���o�b�t�@���쐬
	d3dpp.AutoDepthStencilFormat	= D3DFMT_D16;				// �f�v�X�o�b�t�@�Ƃ���16bit���g��

	if(bWindow)
	{// �E�B���h�E���[�h
		d3dpp.FullScreen_RefreshRateInHz = 0;								// ���t���b�V�����[�g
		d3dpp.PresentationInterval       = D3DPRESENT_INTERVAL_IMMEDIATE;	// �C���^�[�o��
	}
	else
	{// �t���X�N���[�����[�h
		d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;			// ���t���b�V�����[�g
		d3dpp.PresentationInterval       = D3DPRESENT_INTERVAL_DEFAULT;		// �C���^�[�o��
	}

	// �f�o�C�X�I�u�W�F�N�g�̐���
	// [�f�o�C�X�쐬����]<�`��>��<���_����>���n�[�h�E�F�A�ōs�Ȃ�
	if(FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, 
									D3DDEVTYPE_HAL, 
									hWnd, 
									D3DCREATE_HARDWARE_VERTEXPROCESSING, 
									&d3dpp, &g_pD3DDevice)))
	{
		// ��L�̐ݒ肪���s������
		// [�f�o�C�X�쐬����]<�`��>���n�[�h�E�F�A�ōs���A<���_����>��CPU�ōs�Ȃ�
		if(FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, 
										D3DDEVTYPE_HAL, 
										hWnd, 
										D3DCREATE_SOFTWARE_VERTEXPROCESSING, 
										&d3dpp, &g_pD3DDevice)))
		{
			// ��L�̐ݒ肪���s������
			// [�f�o�C�X�쐬����]<�`��>��<���_����>��CPU�ōs�Ȃ�
			if(FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, 
											D3DDEVTYPE_REF,
											hWnd, 
											D3DCREATE_SOFTWARE_VERTEXPROCESSING, 
											&d3dpp, &g_pD3DDevice)))
			{
				// ���������s
				return E_FAIL;
			}
		}
	}

	// �����_�[�X�e�[�g�p�����[�^�̐ݒ�
    g_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);				// ���ʂ��J�����O
	g_pD3DDevice->SetRenderState(D3DRS_ZENABLE, TRUE);						// Z�o�b�t�@���g�p
	g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);				// ���u�����h���s��
	g_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// ���\�[�X�J���[�̎w��
	g_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	// ���f�X�e�B�l�[�V�����J���[�̎w��

	// �T���v���[�X�e�[�g�p�����[�^�̐ݒ�
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);	// �e�N�X�`���A�h���b�V���O���@(U�l)��ݒ�
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);	// �e�N�X�`���A�h���b�V���O���@(V�l)��ݒ�
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);	// �e�N�X�`���k���t�B���^���[�h��ݒ�
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);	// �e�N�X�`���g��t�B���^���[�h��ݒ�

	// �e�N�X�`���X�e�[�W�X�e�[�g�̐ݒ�
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);	// �A���t�@�u�����f�B���O����
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);	// �ŏ��̃A���t�@����
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);	// �Q�Ԗڂ̃A���t�@����

	// ���\���p�t�H���g��ݒ�
	D3DXCreateFont(g_pD3DDevice, 18, 0, 0, 0, FALSE, SHIFTJIS_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "Terminal", &g_pD3DXFont);

#ifdef _DEBUG
	
#endif

	// ���͏����̏�����
	InitInput(hInstance, hWnd);

	// �t�F�[�h�̏�����
	//InitFade();
	fadeMain.InitFade();

	// �ŏ��̓^�C�g����ʂ�
	SetMode(MODE_TITLE);

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void Uninit(void)
{
	if (g_pD3DXFont != NULL)
	{// ���\���p�t�H���g�̊J��
		g_pD3DXFont->Release();
		g_pD3DXFont = NULL;
	}

#ifdef _DEBUG
	
#endif
	if(g_pD3DDevice != NULL)
	{// �f�o�C�X�̊J��
		g_pD3DDevice->Release();
		g_pD3DDevice = NULL;
	}

	if(g_pD3D != NULL)
	{// Direct3D�I�u�W�F�N�g�̊J��
		g_pD3D->Release();
		g_pD3D = NULL;
	}

	// �t�F�[�h�̏I������
	//UninitFade();
	fadeMain.UninitFade();

	// ���͏����̏I������
	UninitInput();


}

//=============================================================================
// �X�V����
//=============================================================================
void Update(void)
{

	// ���͍X�V
	UpdateInput();

	switch (g_mode)
	{
	case MODE_TITLE:		// �^�C�g����ʂ̍X�V
		//UpdateTitle();
		title.UpdateTitle();
		break;

	case MODE_GAME:			// �Q�[����ʂ̍X�V
		UpdateGame();
		break;

	case MODE_RESULT:		// ���U���g��ʂ̍X�V
		//UpdateResult();
		result.UpdateResult();
		break;
	}

	// �t�F�[�h�����̍X�V
	//UpdateFade();
	fadeMain.UpdateFade();
}

//=============================================================================
// �`�揈��
//=============================================================================
void Draw(void)
{
	// �o�b�N�o�b�t�@���y�o�b�t�@�̃N���A
	g_pD3DDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), D3DCOLOR_RGBA(192, 192, 192, 0), 1.0f, 0);

	// Direct3D�ɂ��`��̊J�n
	if(SUCCEEDED(g_pD3DDevice->BeginScene()))
	{
		switch (g_mode)
		{
		case MODE_TITLE:		// �^�C�g����ʂ̕`��
			//DrawTitle();
			title.DrawTitle();
			break;

		case MODE_GAME:			// �Q�[����ʂ̕`��
			DrawGame();
			break;

		case MODE_RESULT:		// ���U���g��ʂ̕`��
			//DrawResult();
			result.DrawResult();
			break;
		}
		// �t�F�[�h�`��
		//DrawFade();
		fadeMain.DrawFade();

		// FPS�\��
		DrawFPS();
		
		
		

		

#ifdef _DEBUG
		
#endif

		//Direct3D�ɂ��`��̏I��
		g_pD3DDevice->EndScene();
	}

	//�o�b�N�o�b�t�@�ƃt�����g�o�b�t�@�̓���ւ�
	g_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

//=============================================================================
// �f�o�C�X�̎擾
//=============================================================================
LPDIRECT3DDEVICE9 GetDevice(void)
{
	return g_pD3DDevice;
}

//=============================================================================
// ���[�h�̐ݒ�
//=============================================================================
void SetMode(MODE mode)
{
	g_mode = mode;

	switch (g_mode)
	{
	case MODE_TITLE:
		// ���U���g��ʂ̏I������
		//UninitResult();
		result.UninitResult();

		// �^�C�g����ʂ̏�����
		//InitTitle();
		title.InitTitle();
		break;

	case MODE_GAME:
		// �^�C�g����ʂ̏I������
		//UninitTitle();
		title.UninitTitle();
		// �Q�[����ʂ̏�����
		InitGame();

		break;

	case MODE_RESULT:
		// �Q�[����ʂ̏I������
		UninitGame();

		// ���U���g��ʂ̏�����
		//InitResult();
		result.InitResult();

		break;
	}
}

//=============================================================================
// ���[�h�̎擾
//=============================================================================
MODE GetMode(void)
{
	return g_mode;
}

//=============================================================================
// FPS�\��
//=============================================================================
void DrawFPS(void)
{
	RECT rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	char str[256];
	

	wsprintf(str, "FPS:%d\n", g_nCountFPS);
	

	// �e�L�X�g�`��
	g_pD3DXFont->DrawText(NULL, str, -1, &rect, DT_LEFT, D3DCOLOR_ARGB(0xff, 0xff, 0xff, 0xff));
	//g_pD3DXFont->DrawText(NULL, str2, -1, &rect, DT_CENTER, D3DCOLOR_ARGB(0xff, 0xff, 0xff, 0xff));
}

void DrawGameTime(float a)
{
	RECT g_rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	char str3[256];

	sprintf_s(str3, "Time:%f\n", a);

	// �e�L�X�g�`��
	g_pD3DXFont->DrawText(NULL, str3, -1, &g_rect, DT_CENTER, D3DCOLOR_ARGB(0xff, 0xff, 0xff, 0xff));

}
//=============================================
//OBJECT3D�\���̓��m�́@���̓����蔻��
// 
//==============================================
BOOL HitOBJECT3D(OBJECT3D *obj1, OBJECT3D *obj2)
{
	//���g�̏���p��
	float	obj1rad = obj1->m_Radius.x;
	D3DXVECTOR3 obj1pos = obj1->m_posModel;

	//�Ώۂ̒e�̏���p��
	D3DXVECTOR3 obj2pos = obj2->m_posModel;
	float	obj2rad = obj2->m_Radius.x;
	//�o���̒��S���m�̋���
	D3DXVECTOR3 vec = obj2pos - obj1pos;
	float length = D3DXVec3Length(&vec);
	//�����Ɣ��a�̑��a�`�F�b�N
	if (length < (obj2rad + obj1rad))
	{

		return TRUE;//���������̂Ŏ��g�̏����I��

	}
	return FALSE;	
}
// �L�����N�^�p���s��Z�o�֐�
D3DXMATRIX* CalcLookAtMatrix(
	D3DXMATRIX* pout,
	D3DXVECTOR3* pPos,
	D3DXVECTOR3* pLook,
	D3DXVECTOR3* pUp ) 
{
	D3DXVECTOR3 X, Y, Z;
	Z = -(*pLook - *pPos);
	D3DXVec3Normalize(&Z, &Z);
	D3DXVec3Cross(&X, D3DXVec3Normalize(&Y, pUp), &Z);
	D3DXVec3Normalize(&X, &X);
	D3DXVec3Normalize(&Y, D3DXVec3Cross(&Y, &Z, &X));


	pout->_11 = X.x; pout->_12 = X.y; pout->_13 = X.z; pout->_14 = 0;
	pout->_21 = Y.x; pout->_22 = Y.y; pout->_23 = Y.z; pout->_24 = 0;
	pout->_31 = Z.x; pout->_32 = Z.y; pout->_33 = Z.z; pout->_34 = 0;
	pout->_41 = 0.0f; pout->_42 = 0.0f; pout->_43 = 0.0f; pout->_44 = 1.0f;

	return pout;
}

// �L�����N�^�p���s��Z�o�֐�:Y����]�Œ�
D3DXMATRIX* CalcLookAtMatrixAxisFix(
	D3DXMATRIX* pout,
	D3DXVECTOR3* pPos,
	D3DXVECTOR3* pLook,
	D3DXVECTOR3* pUp ) 
{
	D3DXVECTOR3 X, Y, Z, D;
	D = -(*pLook - *pPos);
	D3DXVec3Normalize(&D, &D);
	D3DXVec3Cross(&X, D3DXVec3Normalize(&Y, pUp), &D);
	D3DXVec3Normalize(&X, &X);
	D3DXVec3Normalize(&Z, D3DXVec3Cross(&Z, &X, &Y));

	pout->_11 = X.x; pout->_12 = X.y; pout->_13 = X.z; pout->_14 = 0;
	pout->_21 = Y.x; pout->_22 = Y.y; pout->_23 = Y.z; pout->_24 = 0;
	pout->_31 = Z.x; pout->_32 = Z.y; pout->_33 = Z.z; pout->_34 = 0;
	pout->_41 = 0.0f; pout->_42 = 0.0f; pout->_43 = 0.0f; pout->_44 = 1.0f;

	return pout;
}


float GetFrameTime(void)
{
	return frameTime;
}


