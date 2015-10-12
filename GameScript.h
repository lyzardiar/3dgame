// GameScript.h: interface for the CGameScript class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GAMESCRIPT_H__919F406F_BDE1_45D1_92F0_DA3E335211E8__INCLUDED_)
#define AFX_GAMESCRIPT_H__919F406F_BDE1_45D1_92F0_DA3E335211E8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

extern "C"
{
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
};

class CGameScript  
{
public:
	CGameScript();
	virtual ~CGameScript();
	  lua_State *L;
	//��ȡ�����ļ�-----------------------------------
	void CallFun(int nTaskIndex);
	//NPC�ű����ú���--------------------------------
	static int   SetNpcTalk  (lua_State *L); 
	static int   EndTalk   (lua_State *L);
    static int   CloseTalk   (lua_State *L);
	//ע�ắ��
	void  RegisterFun ();
	void  Close(){ lua_pop(L, 1);lua_close (L);}
};

#endif // !defined(AFX_GAMESCRIPT_H__919F406F_BDE1_45D1_92F0_DA3E335211E8__INCLUDED_)
