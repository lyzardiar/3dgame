// Player.cpp: implementation of the CPlayer class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Player.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPlayer::CPlayer()
{	
    m_bControl=false;
	m_nState=STAND;

	m_bGTarget=FALSE;

	m_vPos=D3DXVECTOR3(0.f,0.f,0.f);
	m_fAngle=3*D3DX_PI/2;

	m_xPlayInfo.Name="巫妖王";
	m_xPlayInfo.Hp=m_xPlayInfo.MaxHp=4000;
	m_xPlayInfo.Mp=m_xPlayInfo.MaxMp=2000;

	m_pTargetInfo=NULL;
}

CPlayer::~CPlayer()
{

}
VOID CPlayer::GetAnimPeriod()
{
	for (int i=0;i<18;i++)
	{
		m_fAnimPeriod[i]=m_pPlayerMesh->GetAnimPeriod(i);
	}
}
VOID CPlayer::Init(LPDIRECT3DDEVICE9 pD3DDevice)
{
	m_pPlayerMesh=new CSkinMesh(pD3DDevice);
   
	m_pPlayerMesh->LoadFromXFile("wuyaowang.x");
	m_pPlayerMesh->SetScale(10.0);
	m_pPlayerMesh->SetRotateAngle(m_fAngle);
	D3DXMATRIXA16 scale,trans; 
	D3DXMatrixScaling(&scale,10.0,10.0,10.0);
	D3DXMatrixTranslation(&trans,0,64.0,0);
	m_pPlayerMesh->ApplyRectify(&(scale*trans),10.0);
	m_pPlayerMesh->SetPosition(D3DXVECTOR3(0.f,64.f,0.f));
/*	m_pPlayerMesh->SetRotateAngle(D3DX_PI/4);*/
	GetAnimPeriod();
}
VOID CPlayer::Render()
{
	m_pPlayerMesh->Render();
}
bool CPlayer::IsDead()
{
	if(m_xPlayInfo.Hp<=0)
	{
		m_xPlayInfo.Hp=0;
		return true;
	}
	else
		return false;
}
VOID CPlayer::Update(float nLoopTime)
{
    static float nStarttime=0;
	static int nOldState=STAND;
    nStarttime+=nLoopTime;

	if (IsDead())
	{
		m_nState=DEAD;
	}
	if (nOldState!=m_nState)
	{
		m_pPlayerMesh->SetAnimationNum(m_nState);
		nStarttime=0;
		nOldState=m_nState;
	}

	if (m_nState==DEAD)
	{
		if (nStarttime>m_fAnimPeriod[m_nState]-0.1)
		{
			return;
		}
	}


	if (m_bControl)
	{
		if (nStarttime>m_fAnimPeriod[m_nState])
		{
			nStarttime=0;
			m_bControl=false;
		}
	}
	else
	{
		if (m_bGTarget)
		{
			m_nState=READYATTACK;
		}
		else
			m_nState=STAND;
	}
		m_pPlayerMesh->Update(nLoopTime); 	
}
bool CPlayer::OnKeyDown(WPARAM wParam, LPARAM lParam)
{
	switch (wParam)
	{
	case 'W':
		{
			if (m_bControl)
			return false;
			m_bControl=true;
			m_nState=RUN;
		}
		break;
	case 'S':
		{
			if (m_bControl)
		    return false;
			m_bControl=true;
			m_nState=WALK;
		}
        break;
	case 'A':
		{
			m_fAngle-=D3DX_PI/24;
            m_pPlayerMesh->SetRotateAngle(m_fAngle);
		}
		break;
	case 'D':
		{
			m_fAngle+=D3DX_PI/24;
			m_pPlayerMesh->SetRotateAngle(m_fAngle);
		}
		break;
	case 'T':
		{
			if (m_bControl)
				return false;
			m_bControl=true;
			m_nState=FOOTATTACK;
		}
		break;
	case 'H':
		{
			if (m_pPlayerMesh->m_bHightlight)
			{
				m_pPlayerMesh->m_bHightlight=false;
			}
			else
				m_pPlayerMesh->m_bHightlight=true;
			
		}
		break;

	case 49:
		{
// 			if (m_bControl)
// 			return false;
// 			m_bControl=true;
// 			m_nState=ATTACK9;
		}
		break;
	case 50:
		{
// 			if (m_bControl)
// 				return false;
// 			m_bControl=true;
// 			m_nState=ATTACK2;
		}
		break;
	case 51:
		{
			if (m_bControl)
				return false;
			m_bControl=true;
			m_nState=ATTACK3;
		}
		break;
	case 52:
		{
			if (m_bControl)
				return false;
			m_bControl=true;
			m_nState=ATTACK4;
		}
		break;
	case 53:
		{
			if (m_bControl)
				return false;
			m_bControl=true;
			m_nState=ATTACK5;
		}
		break;
	case 54:
		{
			if (m_bControl)
				return false;
			m_bControl=true;
			m_nState=ATTACK6;
		}
		break;
	case 55:
		{
			if (m_bControl)
				return false;
			m_bControl=true;
			m_nState=ATTACK7;
		}
		break;
	case 56:
		{
 			if (m_bControl)
 				return false;
 			m_bControl=true;
 			m_nState=ATTACK8;
		}
		break;
	case 57:
		{
			if (m_bControl)
				return false;
			m_bControl=true;
			m_nState=DEAD;
		}
		break;
	}
	return true;
}
void CPlayer::PlayAnimation(int nAnimationIndex)
{
	if (m_bControl)
		return ;
	m_bControl=true;
	m_nState=nAnimationIndex;				
}
bool CPlayer::OnKeyUp(WPARAM wParam, LPARAM lParam)
{
	return true;
}
HRESULT CPlayer::GameProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_KEYDOWN:
		OnKeyDown(wParam,lParam);
		break;
	case WM_KEYUP:
		OnKeyUp(wParam,lParam);
		break;
	}
	return DefWindowProc(hWnd,msg,wParam,lParam);
}
void CPlayer::Walk(D3DXVECTOR3 vDir,float fUnits)
{
	m_vPos+=vDir*fUnits;
//计算向量的角度；
// 	float fAngle;
// 	if (vDir.x>0)
// 	{
// 		fAngle=asin(vDir.z);
// 	}
// 	else if (vDir.x==0)
// 	{
// 		if (vDir.z>0)
// 		{
// 			fAngle=D3DX_PI/2;
// 		}
// 		else
// 			fAngle=3*D3DX_PI/2;
// 	}
// 	else
// 	{
// 		fAngle=D3DX_PI-asin(vDir.z);
// 	}
// 	m_pPlayerMesh->SetRotateAngle(2*D3DX_PI-fAngle);	
}
void CPlayer::GetPos(D3DXVECTOR3 *vPos)
{
	*vPos=m_vPos;
}
void CPlayer::SetPos(D3DXVECTOR3 vPos)
{
	m_pPlayerMesh->SetPosition(vPos);
}
void CPlayer::SetAngle(float fAngle)
{
	m_fAngle+=fAngle;
	m_pPlayerMesh->SetRotateAngle(m_fAngle);
}
