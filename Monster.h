// Monster.h: interface for the CMonster class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MONSTER_H__4F095F59_9597_481D_86AC_376814CB23DC__INCLUDED_)
#define AFX_MONSTER_H__4F095F59_9597_481D_86AC_376814CB23DC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

extern "C" 
{ 
#include "lua.h" 
#include "lualib.h" 
#include "lauxlib.h" 
} 


typedef struct TagMONINFO
{
	int Id;
	char szMonName[64];
	int MaxHp;
	int MaxMp;
	int Hp;
	int Mp;
	float OrigX,OrigY,OrigZ;
	float scall;
	int x;
	int y;
	int z;
	int state;
	int scale;
	float rotateAngle;
	int modelhigh;
	int TaskIndex;
	int RailwayIndex;
}MONINFO,*LPMONINFO;
typedef struct tagRAILWAY
{
	int a;
	int b;
	int CenterX;
	int CenterY;
	int r;//a(x-x0)*(x-x0)+b(y-y0)(y-y0)=r*r
	int w;//½ÇËÙ¶È
}RAILWAY,*LPRAILWAY;
class CMonster  
{
public:
	CMonster();
	virtual ~CMonster();

	LPRAILWAY m_pRailway;
		
	float m_fAnimation[18];

	float m_fAngel;
	MONINFO m_xMonsterInfo;
	int m_nCurAction;
	
	D3DXVECTOR3 m_vPos;
	CSkinMesh* m_pMonsterMesh;

	void Create(int nId,LPDIRECT3DDEVICE9 pD3DDevice);
	void Update(float nLoopTime);
	void Render();
	void GetAnimPeriod();
	void AI(float nLoopTime);
	void Chase();
	void Attack();
	float CalculateTagDistanceSquece();

    bool IsDead();
};

#endif // !defined(AFX_MONSTER_H__4F095F59_9597_481D_86AC_376814CB23DC__INCLUDED_)
