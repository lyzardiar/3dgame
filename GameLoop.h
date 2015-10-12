// GameLoop.h: interface for the CGameLoop class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GAMELOOP_H__69DAC5B2_F3A7_497E_9B26_278026B425AE__INCLUDED_)
#define AFX_GAMELOOP_H__69DAC5B2_F3A7_497E_9B26_278026B425AE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "LoginProc.h"
#include "GameProc.h"

#define _LOGIN_PROC 0
#define _GAME_PROC 1

class CGameLoop  
{
public:
	CGameLoop();
	virtual ~CGameLoop();

	HRESULT InitD3D(HWND hWnd);
	void InitLights();
	bool InitGame();
	void TheGameLoop();
	LRESULT Proc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

	LPDIRECT3DDEVICE9 GetDevice();
	int GetScreenWidth();
	int GetScreenHeight();

	int m_nGameState;
	
//	CFont *m_pFont;
	
	LPDIRECT3DDEVICE9 m_pD3DDevice;
	
private:
	void RenderText();
	
	CLoginProc* m_pLoginProc;
	CGameProc*  m_pGameProc;
	
	int m_nScreenWidth;
	int m_nScreenHeight;
	LPDIRECT3D9 m_pD3D;
};

#endif // !defined(AFX_GAMELOOP_H__69DAC5B2_F3A7_497E_9B26_278026B425AE__INCLUDED_)
