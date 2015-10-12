// Player.h: interface for the CPlayer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PLAYER_H__BBD1F904_A11F_4D91_9E3F_77842AE160B6__INCLUDED_)
#define AFX_PLAYER_H__BBD1F904_A11F_4D91_9E3F_77842AE160B6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "SkinMesh.h"
#include "Monster.h"

#define STAND               0
#define QUITEWALK           1
#define ATTACK9             2
#define ATTACK8             3
#define ATTACK7             4
#define ATTACK6             5
#define ATTACK5             6
#define UNDERATTACK2        7
#define UNDERATTACK         8
#define ATTACK4             9
#define ATTACK3             10
#define ATTACK2             11
#define FOOTATTACK          12
#define ATTACK              13
#define DEAD                14
#define READYATTACK         15
#define RUN                 16
#define WALK                17

typedef struct TagPLAYERINFO
{
	char *Name;
	int MaxHp;
	int MaxMp;
	int Hp;
	int Mp;
	float OrigX,OrigY,OrigZ;
	float scall;
	int x;
	int y;
}PLAYERINFO,*LPPLAYERINFO;
class CPlayer  
{
public:
	CPlayer();
	virtual ~CPlayer();

	LPMONINFO m_pTargetInfo;
	CSkinMesh* m_pPlayerMesh;
	int m_nState;
	bool m_bControl;
	bool m_bGTarget;
	FLOAT m_fAnimPeriod[18];///////////¶¯»­³¤¶È
	float m_fAngle;
	D3DXVECTOR3 m_vPos;
	PLAYERINFO m_xPlayInfo;

	void PlayAnimation(int nAnimationIndex);

	void GetAnimPeriod();
	HRESULT GameProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	
	void Init(LPDIRECT3DDEVICE9 pD3DDevice);
	void Update(float nLoopTime);
	void Render();
	void Walk(D3DXVECTOR3 vDir,float fUnits);
	void GetPos(D3DXVECTOR3 *vPos);
	void SetPos(D3DXVECTOR3 vPos);
	void SetAngle(float fAngle);
	bool IsDead();

	bool OnKeyDown(WPARAM wParam, LPARAM lParam);
	bool OnKeyUp(WPARAM wParam, LPARAM lParam);

	inline int CalculateTagDistanceSquece()
	{
		return (m_vPos.x-m_pTargetInfo->x)*(m_vPos.x-m_pTargetInfo->x)+(m_vPos.z-m_pTargetInfo->z)*(m_vPos.z-m_pTargetInfo->z);
	}
};

#endif // !defined(AFX_PLAYER_H__BBD1F904_A11F_4D91_9E3F_77842AE160B6__INCLUDED_)
