// ChatPopWnd.h: interface for the CChatPopWnd class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHATPOPWND_H__1BA77949_471D_4BD7_9F4B_E0543A6A295F__INCLUDED_)
#define AFX_CHATPOPWND_H__1BA77949_471D_4BD7_9F4B_E0543A6A295F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "ChatButtom.h"
#include "Panel.h"

class CChatPopWnd  
{
public:
	CChatPopWnd(LPDIRECT3DDEVICE9 pDevice,int x,int y,int nWidth,int nHeight);
	RECT m_Rect;
	virtual ~CChatPopWnd();

		bool m_bOpen;
	CPanel *m_pPanel;
	int m_nTaskState;
	int m_nPageIndex;
	char m_szText[255];
	char m_szOkText[255];
	char m_szCancelText[255];
	CChatButtom* m_pOkBtn;
	CChatButtom* m_pCancelBtn;
    void Open();
	void Close();
	void Render();
	void GetPageText();

	__inline bool IsInRect(int nPosX, int nPosY)
	{	
		if (nPosX >= m_Rect.left && nPosX <= m_Rect.right && 
			nPosY >= m_Rect.top && nPosY <= m_Rect.bottom) return true;
		else 
			return false;
	}
	void SetChatText(char *sztext,char *okbtn,char *cancelbtn);
	void LoadTexture();
	bool OnMouseMove(POINT ptMouse);
	bool OnLButtonDown(POINT ptMouse);
	bool OnLButtonUp(POINT ptMouse);
private:
	LPDIRECT3DDEVICE9 m_pD3DDevice;
	LPDIRECT3DTEXTURE9 m_pTexture;
};

#endif // !defined(AFX_CHATPOPWND_H__1BA77949_471D_4BD7_9F4B_E0543A6A295F__INCLUDED_)
