// GameProc.h: interface for the CGameProc class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GAMEPROC_H__21481161_A19F_4C09_8925_410B1AA6CC8F__INCLUDED_)
#define AFX_GAMEPROC_H__21481161_A19F_4C09_8925_410B1AA6CC8F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "Player.h"
#include "Terrain.h"
#include "SkyBox.h"
#include "Camera.h"
#include "Water.h"
#include "Monster.h"
#include "PDList.h"
#include "RoleInfo.h"
#include "ChatPopWnd.h"
#include "ChatButtom.h"
#include "GameScript.h"
#include "SkillWnd.h"

class CGameProc  
{
public:
	CGameProc();
	virtual ~CGameProc();

	CSkillWnd m_xSkillWnd;

	CGameScript m_xTaskScript;
    CPDLList<CMonster>	m_xMonsterList;
	CPDLList<ItemInfo>  m_xItemList;
	CPDLList<SkillInfo> m_xSkillList;

	CTerrain *m_pTerrain;
	CSkyBox *m_pSkyBox;
	CWater  *m_pWater;

	CRoleInfo *m_pRoleInfo;

	LPDIRECT3DDEVICE9 m_pD3DDevice;
	void Loaddata();
	void Render(float fElapsedTime);

	LRESULT DefMainWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	LRESULT OnLButtonDown(WPARAM wParam, LPARAM lParam);
	LRESULT OnRButtonDown(WPARAM wParam, LPARAM lParam);
	
	bool Pick(POINT ptMouse);

private:
	float m_fAnlgeX,m_fAnlgeY,m_fAngleRotY;
	bool m_bRCapture,m_bLCapture;
    POINT m_Oldpt,m_pt;
	RECT m_clientRect;			// 客户区大小
	DWORD m_clientWidth;		// 窗口客户区宽度
	DWORD m_clientHeight;		// 窗口客户区高度

};


#endif // !defined(AFX_GAMEPROC_H__21481161_A19F_4C09_8925_410B1AA6CC8F__INCLUDED_)
