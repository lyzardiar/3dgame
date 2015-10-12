// Font.cpp: implementation of the CFont class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Font.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFont::CFont(LPSTR pFontFace, int nHeight, bool fBold, bool fItalic, bool fUnderlined)
{
	HFONT hFont;
	
	m_pD3DDevice = g_pGame->GetDevice();
	
	int nWeight = FW_NORMAL;
	DWORD dwItalic = 0;
	DWORD dwUnderlined = 0;
	
	if(fBold)
	{
		nWeight = FW_BOLD;
	}
	
	if(fItalic)
	{
		dwItalic = 1;
	}
	
	if(fUnderlined)
	{
		dwUnderlined = 1;
	}
	
	hFont = CreateFont(nHeight, 0, 0, 0, nWeight, dwItalic, dwUnderlined, 0, ANSI_CHARSET, 0, 0, 0, 0, pFontFace);
	
	D3DXCreateFont(m_pD3DDevice, hFont, &m_pFont);
	
}

CFont::~CFont()
{
	SafeRelease(m_pFont);
}

int CFont::DrawText(LPSTR pText, int x, int y, D3DCOLOR rgbFontColour)
{
	RECT Rect;
	Rect.left = x;
	Rect.top = y;
	Rect.right = 0;
	Rect.bottom = 0;
	
	m_pFont->Begin();
	m_pFont->DrawTextA(pText, -1, &Rect, DT_CALCRECT, 0);			//Calculate the size of the rect needed
	m_pFont->DrawTextA(pText, -1, &Rect, DT_LEFT, rgbFontColour);	//Draw the text
	m_pFont->End();
	return Rect.bottom-Rect.top;
}

