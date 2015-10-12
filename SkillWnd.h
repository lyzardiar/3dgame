// SkillWnd.h: interface for the CSkillWnd class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SKILLWND_H__AAAA9A1D_914C_4BE2_862D_3449A93CC3C3__INCLUDED_)
#define AFX_SKILLWND_H__AAAA9A1D_914C_4BE2_862D_3449A93CC3C3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "ItemBtn.h"
#include "Panel.h"
typedef struct tagLOCK
{ 
	bool bLock;
	float time;
}LOCK,*LPLOCK;
class CSkillWnd  
{
public:
	CSkillWnd();
	virtual ~CSkillWnd();
    CPanel *m_pPanel;
	void Render();

	RECT m_Rect;

	LOCK m_xLock;

	bool OnMouseMove(POINT ptMouse);
	bool OnRButtonDown(POINT ptMouse);
	bool OnLButtonDown(POINT ptMouse);
	bool OnLButtonUp(POINT ptMouse);
	LRESULT OnKeyDown(WPARAM wParam, LPARAM lParam);

	void Update(float fElapsedTime);
	__inline RECT GetGameBtnRect()
	{
		return m_Rect;
	}
	
	__inline bool IsInRect(int nPosX, int nPosY)
	{	
		if (nPosX >= m_Rect.left && nPosX <= m_Rect.right && 
			nPosY >= m_Rect.top && nPosY <= m_Rect.bottom) return true;
		else 
			return false;
	}

	CItemBtn *m_pItemBtn[12];


	LPDIRECT3DTEXTURE9 m_pTexture;

};

#endif // !defined(AFX_SKILLWND_H__AAAA9A1D_914C_4BE2_862D_3449A93CC3C3__INCLUDED_)
