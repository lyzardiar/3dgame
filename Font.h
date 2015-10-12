// Font.h: interface for the CFont class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FONT_H__C4FD1815_1954_473F_B87B_86C1C395A9EF__INCLUDED_)
#define AFX_FONT_H__C4FD1815_1954_473F_B87B_86C1C395A9EF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "d3dx9.h"

class CFont  
{
public:
	int DrawText(LPSTR pText, int x, int y, D3DCOLOR rgbFontColour);
	CFont(LPSTR pFontFace, int nHeight, bool fBold, bool fItalic, bool fUnderlined);
	virtual ~CFont();
private:
	LPDIRECT3DDEVICE9 m_pD3DDevice;
	LPD3DXFONT m_pFont;

};

#endif // !defined(AFX_FONT_H__C4FD1815_1954_473F_B87B_86C1C395A9EF__INCLUDED_)
