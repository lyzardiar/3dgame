// ChatButtom.cpp: implementation of the CChatButtom class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ChatButtom.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CChatButtom::CChatButtom()
{
	m_clientWidth = 1024;
	m_clientHeight = 738;
}

CChatButtom::~CChatButtom()
{

}
void CChatButtom::AddBottom(LPDIRECT3DDEVICE9 pDevice,const char *sztext,int x,int y,int nWidth,int nHeight,D3DCOLOR fontColor,D3DCOLOR bkColor)
{
//	
	strcpy(m_szText,sztext);
	m_nWidth=nWidth;
	m_nHeigth=nHeight;
	m_Rect.left=x;
	m_Rect.right=x+nWidth;
	m_Rect.top=y;
	m_Rect.bottom=y+nHeight;

	m_pD3DDevice=pDevice;
	m_pD3DDevice->CreateVertexBuffer(4 * sizeof(BTNVERTEX),0, _D3DFVF_BTNVERTEX,D3DPOOL_DEFAULT, &m_pVB,0);
	BTNVERTEX* pVertices = NULL;
	m_pVB->Lock(0, 4 * sizeof(BTNVERTEX), (void**)&pVertices, 0);
	
	//Set all the vertices to selected colour
	pVertices[0].colour = bkColor;
	pVertices[1].colour = bkColor;
	pVertices[2].colour = bkColor;
	pVertices[3].colour = bkColor;
		
	//Set the positions of the vertices
	pVertices[0].x =m_Rect.left-m_clientWidth/2;
	pVertices[0].y =- m_Rect.bottom+m_clientHeight/2;
	
	pVertices[1].x = m_Rect.left-m_clientWidth/2;
	pVertices[1].y =- m_Rect.top+m_clientHeight/2;
	
	pVertices[2].x =m_Rect.right-m_clientWidth/2;
	pVertices[2].y =- m_Rect.bottom+m_clientHeight/2;
	
	pVertices[3].x =m_Rect.right-m_clientWidth/2;
	pVertices[3].y =- m_Rect.top+m_clientHeight/2;

	pVertices[0].z = 1.0f;
	pVertices[1].z = 1.0f;
	pVertices[2].z = 1.0f; 
	pVertices[3].z = 1.0f;
	
	
	//Set the texture coordinates of the vertices

	m_pVB->Unlock();

}
void CChatButtom::SetBtn(const char *sztext,int x,int y)
{
    strcpy(m_szText,sztext);
	m_Rect.left=x;
	m_Rect.right=x+m_nWidth;
	m_Rect.top=y;
	m_Rect.bottom=y+m_nHeigth;
}
bool CChatButtom::OnLButtonDown(POINT ptMouse)
{
	if (!IsInRect(ptMouse.x,ptMouse.y+12))
	{
		return false;
	}

    m_bBtnState=_BUTTON_STATE_DOWN;
	return true;
}
bool CChatButtom::OnLButtonUp(POINT ptMouse)
{
	if (!IsInRect(ptMouse.x,ptMouse.y+12))
	{
		return false;
	}

	m_bBtnState=_BUTTON_STATE_UP;
    return true;
}
bool CChatButtom::OnMouseMove(POINT ptMouse)
{
	//12ÎªÆÁÄ»Ð£Õý
	if (!IsInRect(ptMouse.x,ptMouse.y+12))
	{
		m_bBtnState=_BUTTON_STATE_UP;
		return false;
	}
	m_bBtnState=_BUTTON_STATE_ON;
	return true;
}
void CChatButtom::Render()
{
	
	if (m_bBtnState==_BUTTON_STATE_ON)
	{
		g_pFont->DrawText(m_szText,m_Rect.left,m_Rect.top,D3DCOLOR_XRGB(255,0,0));
		m_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE,TRUE);
		m_pD3DDevice->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_SRCALPHA);
		m_pD3DDevice->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_INVSRCALPHA);
		m_pD3DDevice->SetStreamSource(0, m_pVB,0, sizeof(BTNVERTEX));
		m_pD3DDevice->SetFVF(_D3DFVF_BTNVERTEX);
		m_pD3DDevice->SetTexture(0, NULL);	
	    m_pD3DDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
	}
	else
		g_pFont->DrawText(m_szText,m_Rect.left,m_Rect.top,D3DCOLOR_XRGB(255,255,255));
}
