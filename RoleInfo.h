// RoleInfo.h: interface for the CRoleInfo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ROLEINFO_H__C42225E6_7446_4062_A227_5AE6C35F0F01__INCLUDED_)
#define AFX_ROLEINFO_H__C42225E6_7446_4062_A227_5AE6C35F0F01__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "Player.h"
#include "Monster.h"
typedef struct tagRoleInfo
{
	char* name;
	int Hp;
	int Mp;
}ROLEINFO,*LPROLEINFO;
class CRoleInfo  
{
public:

	ROLEINFO m_xPlayer,m_xTarget;
	bool m_bTarget;
	CRoleInfo(const LPPLAYERINFO player);
	virtual ~CRoleInfo();
	void Update(const LPPLAYERINFO player,const LPMONINFO target,bool bTarget);
    void Render();
};

#endif // !defined(AFX_ROLEINFO_H__C42225E6_7446_4062_A227_5AE6C35F0F01__INCLUDED_)
