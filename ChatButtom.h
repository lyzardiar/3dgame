// ChatButtom.h: interface for the CChatButtom class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHATBUTTOM_H__1968D2E3_470C_49A1_B4AF_8D9518CEF774__INCLUDED_)
#define AFX_CHATBUTTOM_H__1968D2E3_470C_49A1_B4AF_8D9518CEF774__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define _BUTTON_STATE_UP			1// 0
#define _BUTTON_STATE_ON			2// 1
#define _BUTTON_STATE_DOWN		3// 2

#define _D3DFVF_BTNVERTEX (D3DFVF_XYZ|D3DFVF_DIFFUSE)
struct BTNVERTEX
{
    FLOAT x, y, z;		// The transformed position for the vertex.
    DWORD colour;			// The vertex colour.
};


class CChatButtom  
{
public:
	CChatButtom();
	virtual ~CChatButtom();
	int m_clientWidth;		// 窗口客户区宽度
	int m_clientHeight;		// 窗口客户区高度


	RECT			m_Rect;		
	BYTE			m_bBtnState;			// 0 : 弊措肺... 1: 付快胶 器目胶惑怕. 2: 付快胶 努腐惑怕.

	D3DCOLOR m_fontColor;
	D3DCOLOR m_bkColor;

	char m_szText[255];
	__inline RECT GetGameBtnRect()
	{
		return m_Rect;
	}
	void AddBottom(LPDIRECT3DDEVICE9 pDevice,const char *sztext,int x,int y,int nWidth,int nHeigh,D3DCOLOR fontColor=D3DCOLOR_XRGB(0,0,0),D3DCOLOR bkColor=D3DCOLOR_ARGB(150,255,255,255));

	void SetBtn(const char *sztext,int x,int y);
	__inline bool IsInRect(int nPosX, int nPosY)
	{	
		if (nPosX >= m_Rect.left && nPosX <= m_Rect.right && 
			nPosY >= m_Rect.top && nPosY <= m_Rect.bottom) return true;
		else 
			return false;
	}

	LPDIRECT3DDEVICE9 m_pD3DDevice;
    bool OnMouseMove(POINT ptMouse);
	bool OnLButtonDown(POINT ptMouse);
	bool OnLButtonUp(POINT ptMouse);
	void Render();
private:
	
	LPDIRECT3DVERTEXBUFFER9 m_pVB;
	int m_nWidth;
	int m_nHeigth;
};

#endif // !defined(AFX_CHATBUTTOM_H__1968D2E3_470C_49A1_B4AF_8D9518CEF774__INCLUDED_)
