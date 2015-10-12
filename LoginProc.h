// LoginProc.h: interface for the CLoginProc class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGINPROC_H__D3CD49C5_737C_42B6_82D8_F44E7D7F77ED__INCLUDED_)
#define AFX_LOGINPROC_H__D3CD49C5_737C_42B6_82D8_F44E7D7F77ED__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Panel.h"
class CLoginProc  
{
public:
	CLoginProc();
	virtual ~CLoginProc();
	void LoadTexture();
	bool Render(float fElapsedTime);
	void Setup2DCamera();

	CPanel *m_pLoadPanel;
	CPanel *m_pBackBarPanel;
	CPanel *m_pFillBarPanel;
	LPDIRECT3DTEXTURE9 m_pLoginTexture[3];

	LPDIRECT3DDEVICE9 m_pD3DDevice;

	LRESULT DefMainWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

};

#endif // !defined(AFX_LOGINPROC_H__D3CD49C5_737C_42B6_82D8_F44E7D7F77ED__INCLUDED_)
