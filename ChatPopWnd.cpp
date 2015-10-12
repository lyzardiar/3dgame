// ChatPopWnd.cpp: implementation of the CChatPopWnd class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ChatPopWnd.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CChatPopWnd::CChatPopWnd(LPDIRECT3DDEVICE9 pDevice,int x,int y,int nWidth,int nHeight)
{
	m_pD3DDevice=pDevice;
	m_Rect.left=x;
	m_Rect.right=x+nWidth;
	m_Rect.top=y;
	m_Rect.bottom=y+nHeight;


	m_pPanel=new CPanel();
    m_pPanel->UpdateVertices(nWidth,nHeight);
    LoadTexture();

	m_pOkBtn=new CChatButtom();
	m_pCancelBtn=new CChatButtom();

	strcpy(m_szText,"任务界面");
	strcpy(m_szOkText,"OK");
	strcpy(m_szCancelText,"取消");
	m_bOpen=false;
	m_nTaskState=0;//任务未接受
	m_nPageIndex=1;//页面为1

	m_pOkBtn->AddBottom(m_pD3DDevice,m_szOkText,m_Rect.left+50,m_Rect.top+140,m_Rect.right-m_Rect.left-100,20);
	m_pCancelBtn->AddBottom(m_pD3DDevice,m_szCancelText,m_Rect.left+50,m_Rect.top+170,m_Rect.right-m_Rect.left-100,20);
}

CChatPopWnd::~CChatPopWnd()
{

}
void CChatPopWnd::Open()
{
	m_bOpen=true;
	
	m_nPageIndex=1;
}
void CChatPopWnd::SetChatText(char *sztext,char *okbtn,char *cancelbtn)
{
	strcpy(m_szText,sztext);
	strcpy(m_szOkText,okbtn);
	strcpy(m_szCancelText,cancelbtn);
}
void CChatPopWnd::Close()
{
	m_bOpen=false;
}
void CChatPopWnd::Render()
{
	if (!m_bOpen)
		return;

	m_pPanel->MoveTo(m_Rect.left,m_Rect.top);
	m_pPanel->Render(m_pTexture);
	
	int height1=g_pFont->DrawText(m_szText,m_Rect.left+5,m_Rect.top+50,D3DCOLOR_XRGB(255,255,255));
	m_pOkBtn->SetBtn(m_szOkText,m_Rect.left+5,m_Rect.top+height1+80);
	m_pCancelBtn->SetBtn(m_szCancelText,m_Rect.left+5,m_Rect.top+height1+100);
	m_pOkBtn->Render();
	m_pCancelBtn->Render();

}
bool CChatPopWnd::OnLButtonDown(POINT ptMouse)
{
	if (!m_bOpen||!IsInRect(ptMouse.x,ptMouse.y))
	{
		return false;
	}

    if (m_pOkBtn->OnLButtonDown(ptMouse))
    {
        m_nPageIndex+=1;
    }
   	else if(m_pCancelBtn->OnLButtonDown(ptMouse))
	{
		m_nPageIndex=0;
	}
	return true;
}
bool CChatPopWnd::OnLButtonUp(POINT ptMouse)
{
	if (!m_bOpen||!IsInRect(ptMouse.x,ptMouse.y))
	{
		return false;
	}

	m_pOkBtn->OnLButtonUp(ptMouse);
	m_pCancelBtn->OnLButtonUp(ptMouse);
	return true;

}
bool CChatPopWnd::OnMouseMove(POINT ptMouse)
{
	if (!m_bOpen||!IsInRect(ptMouse.x,ptMouse.y))
	{
		m_pOkBtn->m_bBtnState=0;
		m_pCancelBtn->m_bBtnState=0;
		return false;
	}
	m_pOkBtn->OnMouseMove(ptMouse);
	m_pCancelBtn->OnMouseMove(ptMouse);


	return true;
}
void CChatPopWnd::LoadTexture()
{
	char szTextureFilePath[255];
		sprintf(szTextureFilePath,"UI\\dialog.jpg");
		//		D3DXCreateTextureFromFile(g_pGame->GetDevice(), szTextureFilePath, &m_pLoginTexture[i] );
		D3DXCreateTextureFromFileEx(g_pGame->GetDevice(), szTextureFilePath, D3DX_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, 0, 
			D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, D3DCOLOR_XRGB(255,255,255), NULL, NULL, &m_pTexture);
		//      D3DXCreateTextureFromFileEx(m_pD3DDevice, szTextureFilePath, 0, 0, 0, 0,
		//										  D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT,
		//									  D3DX_DEFAULT, D3DCOLOR_XRGB(255,255,255), NULL, NULL,  &m_pLoginTexture[i]);
//	}
//	D3DXCreateTextureFromFileEx(g_pGame->GetDevice(), "login\\0.jpg", D3DX_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, 0, 
//		D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, D3DCOLOR_XRGB(0,0,0), NULL, NULL, &m_pLoginTexture[0]);
	//	D3DXCreateTextureFromFile(g_pGame->GetDevice(), "login\\0.jpg", &m_pLoginTexture[0] );
	//	D3DXCreateTextureFromFileEx(g_pGame->GetDevice(),"login\\0.jpg", 1024, 1024, 1, 0,
	//		D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT,
	//							D3DX_DEFAULT,D3DCOLOR_XRGB(255,255,255), NULL, NULL, &m_pLoginTexture[0]);
	
	
}