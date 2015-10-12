// SkillWnd.cpp: implementation of the CSkillWnd class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SkillWnd.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSkillWnd::CSkillWnd()
{
   m_pPanel=new CPanel();
   m_pPanel->UpdateVertices(768,64);
   
   m_xLock.bLock=false;

   m_Rect.left=128;
   m_Rect.top=670;
   m_Rect.right=128+768;
   m_Rect.bottom=670+64;

   m_pItemBtn[0]=new CItemBtn(128+48+48,670+16);
   m_pItemBtn[0]->LoadSkill(1);
   m_pItemBtn[1]=new CItemBtn(128+48+48+48,686);
   m_pItemBtn[1]->LoadSkill(2);

   for(int i=2;i<12;i++)
   {
	   m_pItemBtn[i]=new CItemBtn(128+48*i+48+48,670+16);
	   m_pItemBtn[i]->LoadItem(0);	   
   }
 //  D3DXCreateTextureFromFileEx(g_pGame->GetDevice(),"UI\\skillbar_bg.bmp", 0, 0, 1, 0,D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_FILTER_NONE,D3DX_DEFAULT, D3DCOLOR_XRGB(0,0,0), NULL, NULL, &m_pTexture);
   D3DXCreateTextureFromFileEx(g_pGame->GetDevice(), "UI\\skillbar_bg.bmp", 
	   D3DX_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, 0, 	D3DFMT_UNKNOWN,
	   D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT,D3DCOLOR_XRGB(0,0,0), NULL, NULL, &m_pTexture);

   //14 14 14 1 1 1
}


CSkillWnd::~CSkillWnd()
{

}
void CSkillWnd::Render()
{	
// 	char buf[255];
// 	sprintf(buf,"°´Å¥Î»ÖÃ%d %d %d %d",m_pItemBtn[0]->m_Rect.left,
// 		m_pItemBtn[0]->m_Rect.top,m_pItemBtn[0]->m_Rect.right,m_pItemBtn[0]->m_Rect.bottom);
// 	g_pFont->DrawText(buf,200,200,D3DCOLOR_XRGB(0,0,0));
	for(int i=0;i<12;i++)
       m_pItemBtn[i]->Render();
	m_pPanel->MoveTo(m_Rect.left,m_Rect.top);
	m_pPanel->Render(m_pTexture);	
}
bool CSkillWnd::OnLButtonDown(POINT ptMouse)
{
   	if (!IsInRect(ptMouse.x,ptMouse.y+25)||m_xLock.bLock)
	{
		return false;
	}
    for (int i=0;i<12;i++)
    {
		if(m_pItemBtn[i]->OnLButtonDown(ptMouse))
		{
			m_xLock.bLock=true;
			m_xLock.time=1.0;
		    break;
		}
    }
	return true;
}
bool CSkillWnd::OnMouseMove(POINT ptMouse)
{ 
	if (!IsInRect(ptMouse.x,ptMouse.y))
	{
		return false;
	}

	for (int i=0;i<12;i++)
	{
		m_pItemBtn[i]->OnMouseMove(ptMouse);		
    }
    return true;
}
bool CSkillWnd::OnLButtonUp(POINT ptMouse)
{
	if (!IsInRect(ptMouse.x,ptMouse.y))
	{
		return false;
	}
	
	for (int i=0;i<12;i++)
	{
		m_pItemBtn[i]->OnLButtonUp(ptMouse);
    }
    return true;
}
void CSkillWnd::Update(float fElapsedTime)
{
	if (m_xLock.bLock)
	{
		m_xLock.time-=fElapsedTime;
		if (m_xLock.time<0.000001)
		{
			m_xLock.bLock=false;
		}
	}
	for (int i=0;i<12;i++)
	{
		m_pItemBtn[i]->Update(fElapsedTime);
    }

}
LRESULT CSkillWnd::OnKeyDown(WPARAM wParam, LPARAM lParam)
{
	switch (wParam)
	{
	case 49:
		{
			if (m_xLock.bLock)
			{
				return 0;
			}
			if (m_pItemBtn[0]->CastSkill())
			{
				g_pPlayer->PlayAnimation(2);
			}
			
			m_xLock.bLock=true;
			m_xLock.time=1.0;
			
		}
		break;
	case 50:
		{
			if (m_xLock.bLock)
			{
				return 0;
			}
			if (m_pItemBtn[1]->CastSkill())
			{
				g_pPlayer->PlayAnimation(3);
			}
			m_xLock.bLock=true;
			m_xLock.time=1.0;
		}

		break;
	}
	return 0;
}