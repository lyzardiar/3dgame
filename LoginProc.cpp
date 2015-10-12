// LoginProc.cpp: implementation of the CLoginProc class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "LoginProc.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLoginProc::CLoginProc()
{

   m_pD3DDevice=g_pGame->GetDevice();

   m_pLoginTexture[0]=NULL;
   m_pLoginTexture[1]=NULL;
   m_pLoginTexture[2]=NULL;

   m_pLoadPanel=new CPanel();
   m_pLoadPanel->UpdateVertices(1024,738);

   m_pBackBarPanel=new CPanel();
   m_pBackBarPanel->UpdateVertices(512,64);

   m_pFillBarPanel=new CPanel();
   LoadTexture();
}

CLoginProc::~CLoginProc()
{
    SAFE_DELETE(m_pFillBarPanel);
	SAFE_DELETE(m_pLoadPanel);
	SAFE_DELETE(m_pBackBarPanel);
	for (int i=0;i<3;i++)
	{
		SAFE_DELETE(m_pLoginTexture[i]);
	}
}
void CLoginProc::LoadTexture()
{
	for (int i=1;i<3;i++)
	{
		char szTextureFilePath[255];
		sprintf(szTextureFilePath,"login\\%d.bmp",i);
//		D3DXCreateTextureFromFile(g_pGame->GetDevice(), szTextureFilePath, &m_pLoginTexture[i] );
		D3DXCreateTextureFromFileEx(g_pGame->GetDevice(), szTextureFilePath, D3DX_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, 0, 
			D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, D3DCOLOR_XRGB(255,255,255), NULL, NULL, &m_pLoginTexture[i]);
   //      D3DXCreateTextureFromFileEx(m_pD3DDevice, szTextureFilePath, 0, 0, 0, 0,
//										  D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT,
	//									  D3DX_DEFAULT, D3DCOLOR_XRGB(255,255,255), NULL, NULL,  &m_pLoginTexture[i]);
	}
	D3DXCreateTextureFromFileEx(g_pGame->GetDevice(), "login\\0.jpg", D3DX_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, 0, 
			D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, D3DCOLOR_XRGB(0,0,0), NULL, NULL, &m_pLoginTexture[0]);
//	D3DXCreateTextureFromFile(g_pGame->GetDevice(), "login\\0.jpg", &m_pLoginTexture[0] );
 //	D3DXCreateTextureFromFileEx(g_pGame->GetDevice(),"login\\0.jpg", 1024, 1024, 1, 0,
 //		D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT,
 	//							D3DX_DEFAULT,D3DCOLOR_XRGB(255,255,255), NULL, NULL, &m_pLoginTexture[0]);


}
bool CLoginProc::Render(float fElapsedTime)
{
	static float fTime=0;
	bool bRenderOver=false;
	if(m_pD3DDevice == NULL)
    {
		return 0;
    }	
	m_pD3DDevice->Clear(0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, NULL, 1.0f, 0);
    m_pD3DDevice->BeginScene();
	
	g_pCamera->Setup2DCamera();
	// 	Render2D();

	m_pLoadPanel->MoveTo(0,0);
	m_pLoadPanel->Render(m_pLoginTexture[0]);

	m_pBackBarPanel->MoveTo(256,600);
	m_pBackBarPanel->Render(m_pLoginTexture[1]);
// 
	if (fTime<5.000001)
	{
		m_pFillBarPanel->MoveTo(287,617);
		m_pFillBarPanel->UpdateVertices(448/5*fTime,32);
   	}
	else
		bRenderOver=true;
	fTime+=fElapsedTime;
	m_pFillBarPanel->Render(m_pLoginTexture[2]);
	
    m_pD3DDevice->EndScene();
    //Filp the back and front buffers so that whatever has been rendered on the back buffer
    //will now be visible on screen (front buffer).
    m_pD3DDevice->Present(NULL, NULL, NULL, NULL);
	return bRenderOver;

}
LRESULT CLoginProc::DefMainWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{

	return DefWindowProc(hWnd,msg,wParam,lParam);
}
