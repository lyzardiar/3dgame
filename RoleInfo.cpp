// RoleInfo.cpp: implementation of the CRoleInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RoleInfo.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRoleInfo::CRoleInfo(const LPPLAYERINFO player)
{
	m_bTarget=false;
	m_xPlayer.name=player->Name;
	m_xPlayer.Hp=player->Hp;
	m_xPlayer.Mp=player->Mp;
}

CRoleInfo::~CRoleInfo()
{

}
void CRoleInfo::Update(const LPPLAYERINFO player,const LPMONINFO target,bool bTarget)
{
	m_bTarget=bTarget;
	m_xPlayer.name=player->Name;
	m_xPlayer.Hp=player->Hp;
	m_xPlayer.Mp=player->Mp;
	if (bTarget)
	{
 	//	m_xTarget.name="dd";
//		m_xTarget.Hp=1;
// 	    m_xTarget.Mp=1;
		m_xTarget.name=target->szMonName;
 		m_xTarget.Hp=target->Hp;
 	    m_xTarget.Mp=target->Mp;
	}

}
void CRoleInfo::Render()
{
	char buf[255];
	sprintf(buf,"%s\n%d\n%d",m_xPlayer.name,m_xPlayer.Hp,m_xPlayer.Mp);
	g_pFont->DrawText(buf,0,0,D3DCOLOR_XRGB(0,0,0));
	if (m_bTarget)
	{
		sprintf(buf,"%s\n%d\n%d",m_xTarget.name,m_xTarget.Hp,m_xTarget.Mp);
     	g_pFont->DrawText(buf,100,0,D3DCOLOR_XRGB(255,0,0));
	}
}
