// GameLoop.cpp: implementation of the CGameLoop class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFont *g_pFont;
CCamera *g_pCamera;
CGameLoop::CGameLoop()
{
//	m_nGameState=_LOGIN_PROC;
	m_nGameState=_GAME_PROC;
}

CGameLoop::~CGameLoop()
{


}
HRESULT CGameLoop::InitD3D(HWND hWnd)
{
	if(NULL==(m_pD3D=Direct3DCreate9(D3D_SDK_VERSION)))
		return E_FAIL;
	// 	D3DDISPLAYMODE d3ddm;
	//     if( FAILED( g_pD3D->GetAdapterDisplayMode( D3DADAPTER_DEFAULT, &d3ddm ) ) )
	//         return E_FAIL;
	// 	D3DADAPTER_IDENTIFIER9 ident;
	// 	m_pD3D->GetAdapterIdentifier(D3DADAPTER_DEFAULT,0,&ident);
	// 	UINT numModes=m_pD3D->GetAdapterModeCount(D3DADAPTER_DEFAULT,D3DFMT_X8R8G8B8);
	// 	for (UINT i=0;i<numModes;i++)
	// 	{
	// 		D3DDISPLAYMODE mode;
	// 		char modeString[255];
	// 		m_pD3D->EnumAdapterModes(D3DADAPTER_DEFAULT,D3DFMT_X8R8G8B8,i,&mode);
	// 	}
	
	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp,sizeof(d3dpp));
	
	if (1)
	{
		d3dpp.Windowed=TRUE;
    } 
	else
    {
		d3dpp.Windowed=FALSE;
		d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT; //--屏幕刷新率*	
		d3dpp.PresentationInterval       = D3DPRESENT_INTERVAL_ONE;
    }
    

    d3dpp.BackBufferCount = 1;
	
	d3dpp.BackBufferFormat=D3DFMT_X8R8G8B8;
	d3dpp.BackBufferWidth=1024;
	d3dpp.BackBufferHeight=738;//1440 900  1360 768  1280 800 1280 768  1024 738
    d3dpp.hDeviceWindow = hWnd;
    d3dpp.SwapEffect=D3DSWAPEFFECT_DISCARD;
//	d3dpp.SwapEffect=	D3DSWAPEFFECT_COPY;



	d3dpp.EnableAutoDepthStencil=TRUE;
 	d3dpp.AutoDepthStencilFormat=D3DFMT_D24X8;
	
	if(FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,D3DDEVTYPE_HAL,hWnd,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING,
		&d3dpp,&m_pD3DDevice)))
	{
		MessageBox(NULL,"初始化错误","错误",MB_OK);
		return E_FAIL;
	}
	
 	m_pD3DDevice->SetRenderState( D3DRS_DITHERENABLE,     TRUE );
 	m_pD3DDevice->SetRenderState( D3DRS_ZENABLE,          TRUE );//激活深度测试
	m_pD3DDevice->SetRenderState(D3DRS_ZFUNC,D3DCMP_LESS);
	m_pD3DDevice->SetRenderState(D3DRS_ZWRITEENABLE,TRUE);

    m_pD3DDevice->SetRenderState( D3DRS_CULLMODE,         D3DCULL_CCW );
    m_pD3DDevice->SetRenderState( D3DRS_AMBIENT,          0xffffffff);

    m_pD3DDevice->SetRenderState( D3DRS_NORMALIZENORMALS, TRUE ); //////////////////////////////////
    m_pD3DDevice->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_POINT );
    m_pD3DDevice->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_POINT );
	m_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	m_pD3DDevice->SetRenderState( D3DRS_LIGHTING,         TRUE);

//	m_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	//透明处理

	m_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE,TRUE);
    m_pD3DDevice->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_SRCALPHA);
	m_pD3DDevice->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_INVSRCALPHA);
	//把alpha参数设为纹理的alpha值(下面2行其实也可以去掉,因为默认就是这样的)
	// 	m_pD3DDevice->SetTextureStageState(0,D3DTSS_ALPHAARG1,D3DTA_TEXTURE); 
	// 	m_pD3DDevice->SetTextureStageState(0,D3DTSS_ALPHAOP,D3DTOP_SELECTARG1);	
	// 	m_pD3DDevice->SetRenderState( D3DRS_LIGHTING, TRUE );
	// 	
	/*	InitLights();*/
	
	return S_OK;
}
void CGameLoop::InitLights()
{
	D3DLIGHT9					m_light_0,m_light_1;
    ZeroMemory( &m_light_0, sizeof(D3DLIGHT9) );
    m_light_0.Type = D3DLIGHT_DIRECTIONAL;
    m_light_0.Direction = D3DXVECTOR3( 0.f, -1.0f, 0.f );
	
	m_light_0.Diffuse.a = 1.0f;
    m_light_0.Diffuse.r = 1.0f;
    m_light_0.Diffuse.g = 1.0f;
    m_light_0.Diffuse.b = 1.0f;
	
	m_light_0.Ambient.a = 0.4f;
	m_light_0.Ambient.r = 0.4f;
	m_light_0.Ambient.g = 0.4f;
	m_light_0.Ambient.b = 0.4f;
	
	m_light_0.Specular.a=0.6f;
	m_light_0.Specular.r=0.6f; 
	m_light_0.Specular.g=0.6f;
	m_light_0.Specular.b=0.6f;
    m_pD3DDevice->SetLight( 0, &m_light_0 );

	ZeroMemory( &m_light_1, sizeof(D3DLIGHT9) );
    m_light_1.Type = D3DLIGHT_POINT;
    m_light_1.Position = D3DXVECTOR3( 100.f, 100.0f, 0.f );
/*	 m_light_0.Direction = D3DXVECTOR3( 0.f, -1.0f, 0.f );*/
	m_light_1.Range=1000.f;
	m_light_1.Attenuation0=1.0f;
	
	m_light_1.Diffuse.a = 1.0f;
    m_light_1.Diffuse.r = 2.0f;
    m_light_1.Diffuse.g = 2.0f;
    m_light_1.Diffuse.b = 2.0f;
	
	m_light_1.Ambient.a = 1.0f;
	m_light_1.Ambient.r = 1.0f;
	m_light_1.Ambient.g = 0.0f;
	m_light_1.Ambient.b = 0.0f;
	
    m_pD3DDevice->SetLight( 1, &m_light_1 );
}

bool CGameLoop::InitGame()
{
	g_pFont = new CFont("宋体", 20, false, false, false);
	g_pCamera=new CCamera(m_pD3DDevice);

	InitLights();
	m_pD3DDevice->LightEnable(0,TRUE);

	m_pGameProc=new CGameProc();
	m_pLoginProc=new CLoginProc();

	return true;
}

LPDIRECT3DDEVICE9 CGameLoop::GetDevice()
{
	return m_pD3DDevice;
}

void CGameLoop::TheGameLoop()
{
    //Enter the game loop
    MSG msg; 
    BOOL fMessage;

    PeekMessage(&msg, NULL, 0U, 0U, PM_NOREMOVE);

    while(msg.message != WM_QUIT)
    {
        fMessage = PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE);

        if(fMessage)
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else
        {
			timeBeginPeriod(1);
			static float fPreTime = static_cast<float>(timeGetTime());
			float fCurrentTime = static_cast<float>(timeGetTime());
			float fElapsedTime = (fCurrentTime - fPreTime)*0.001f;
			timeEndPeriod(1);

			switch (m_nGameState)
			{
			case 0:
				m_nGameState=m_pLoginProc->Render(fElapsedTime);
				fPreTime = fCurrentTime;
			//	if (m_nGameState==1)
			//	{
			//		fCurrentTime=0;
			//	}
				break;
			case 1:
				m_pGameProc->Render(fElapsedTime);
				fPreTime = fCurrentTime;
				break;
			}			
        }
    }
}


void CGameLoop::RenderText()
{
// 	//Draw some text at the top of the screen showing stats
	static DWORD	dwFPS 		= 0;
    static DWORD	dwLastTime	= 0;
    static DWORD	dwFrames	= 0;
	
    DWORD dwTime = timeGetTime();
    ++dwFrames;
	
    if ( dwTime - dwLastTime > 1000 )
    {
        dwFPS      = dwFrames / ( (dwTime - dwLastTime) / 1000 );
        dwLastTime = dwTime;
        dwFrames   = 0;
    }

// 	sprintf(buffer," CameraLookAt:%f %f %f \n Height: %f \n %f %f ",at.x,at.y,at.z,m_pTerrain->getHeight(0.f,0.f),fAnlgeX,fAnlgeY);
// 	m_pFont->DrawText(buffer,900,100,D3DCOLOR_XRGB(255,0,0));
//     sprintf(buffer," the player coordinate %f %f ",playerPos.x,playerPos.z);
// 	m_pFont->DrawText(buffer,900,150,D3DCOLOR_XRGB(255,0,0));
}

int CGameLoop::GetScreenWidth()
{
	return m_nScreenWidth;
}
int CGameLoop::GetScreenHeight()
{
	return  m_nScreenHeight;
}

LRESULT CGameLoop::Proc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (m_nGameState)
	{
	case _LOGIN_PROC:
		return m_pLoginProc->DefMainWndProc(hWnd,msg,wParam,lParam);
		break;
	case _GAME_PROC:
		return m_pGameProc->DefMainWndProc(hWnd,msg,wParam,lParam);
		break;
    default:
		return DefWindowProc(hWnd,msg,wParam,lParam);
		break;
	}

}
