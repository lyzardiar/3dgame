// GameScript.cpp: implementation of the CGameScript class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GameScript.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

#include "GameScript.h"

CGameScript::CGameScript()
{
  L= lua_open(); 
  if(L==NULL) 
	 MessageBox(0,"luabad0",0,0);
  luaL_openlibs(L);
  luaL_dofile(L,"data\\task.lua");
  RegisterFun();
}
CGameScript::~CGameScript()
{
	 Close();
	 L=NULL;
}

//加载脚本---------------------------------------------------------------------------
// void  CGameScript::Loadfile(const char *fileName)
// {
// 	int erro=luaL_loadfile(L, fileName);
// 	if (erro|| lua_pcall(L, 0, 0, 0)!=0)
// 	{
// 		char msg[256];
// 		sprintf(msg,"加载LUA文件错误%d%s",erro,lua_tostring(L, -1));
// 		MessageBox(0,msg,0,0);
// 	}
// }

 //注册函数
void  CGameScript::RegisterFun()
{
	//NPC----------------------------------------		
	lua_register(L,"SetNpcTalk",  SetNpcTalk);	
	lua_register(L,"CloseTalk",   CloseTalk );
	lua_register(L,"EndTalk",  EndTalk);	
	
}
void  CGameScript::CallFun(int nTaskIndex)
{

	char szFunName[255];
	sprintf(szFunName,"task%d",nTaskIndex);
	lua_getglobal(L,szFunName);
	lua_pushnumber(L,g_pChatPopWnd->m_nTaskState);
	lua_pushnumber(L,g_pChatPopWnd->m_nPageIndex);
	lua_call(L,2,1);
}
//----------------------------------------------------------------------------------------------------------
//NPC脚本
//----------------------------------------------------------------------------------------------------------
int CGameScript::CloseTalk(lua_State *L) 
{
	g_pChatPopWnd->Close();
	return 0;
}
int CGameScript::EndTalk(lua_State *L) 
{
	g_pChatPopWnd->m_nTaskState=(g_pChatPopWnd->m_nTaskState+1)%3;
	return 0;
}
//---------------------------------------------------

//----------------------------------------------------
int CGameScript::SetNpcTalk(lua_State*L)
{
    int n = lua_gettop(L);
	//获取参数---------------------------------------
	std::string m_str    =lua_tostring(L,1);
	std::string m_txtbtn1=lua_tostring(L,2);
    std::string m_txtbtn2=lua_tostring(L,3);
	char buf1[255];
	char buf2[255];
	char buf3[255];
	sprintf(buf1,m_str.data());
	sprintf(buf2,m_txtbtn1.data());
	sprintf(buf3,m_txtbtn2.data());
    g_pChatPopWnd->SetChatText(buf1,buf2,buf3);
	return 0;
}



    