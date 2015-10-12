// Monster.cpp: implementation of the CMonster class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Monster.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMonster::CMonster()
{
	m_nCurAction=0;
}


CMonster::~CMonster()
{

}
VOID CMonster::GetAnimPeriod()
{
	for (int i=0;i<6;i++)
	{
		m_fAnimation[i]=m_pMonsterMesh->GetAnimPeriod(i);
	}
}
void CMonster::Create(int nId,LPDIRECT3DDEVICE9 pD3DDevice)
{
	m_pMonsterMesh=new CSkinMesh(pD3DDevice);

	m_xMonsterInfo.Id=nId;
	char buf[16];
	char szfilePath[MAX_PATH];
	sprintf(buf,"Mon%d",nId);
    GetPrivateProfileString(buf,"filepath","",szfilePath,64,"data\\Monster.txt");
	m_xMonsterInfo.scale=GetPrivateProfileInt(buf,"scale",1,"data\\Monster.txt");
    m_xMonsterInfo.modelhigh=GetPrivateProfileInt(buf,"modelhigh",1,"data\\Monster.txt");
	m_xMonsterInfo.x=GetPrivateProfileInt(buf,"x",0,"data\\Monster.txt");
	m_xMonsterInfo.y=GetPrivateProfileInt(buf,"y",0,"data\\Monster.txt");
	m_xMonsterInfo.z=GetPrivateProfileInt(buf,"z",0,"data\\Monster.txt");
	m_xMonsterInfo.TaskIndex=GetPrivateProfileInt(buf,"TaskIndex",0,"data\\Monster.txt");
	m_xMonsterInfo.Hp=m_xMonsterInfo.MaxHp=GetPrivateProfileInt(buf,"MaxHp",0,"data\\Monster.txt");
	m_xMonsterInfo.Mp=m_xMonsterInfo.MaxMp=GetPrivateProfileInt(buf,"MaxMp",0,"data\\Monster.txt");
	GetPrivateProfileString(buf,"name","",m_xMonsterInfo.szMonName,64,"data\\Monster.txt");
	m_xMonsterInfo.rotateAngle=D3DX_PI*GetPrivateProfileInt(buf,"rotateAngle",0,"data\\Monster.txt")*0.1f;
	m_xMonsterInfo.RailwayIndex=GetPrivateProfileInt(buf,"RailwayIndex",0,"data\\Monster.txt");

	if (m_xMonsterInfo.RailwayIndex!=0)
	{
		char szRailwayPath[MAX_PATH];
		sprintf(szRailwayPath,"Railway%d",m_xMonsterInfo.RailwayIndex);
		m_pRailway=(LPRAILWAY)malloc(sizeof(RAILWAY));
		m_pRailway->a=GetPrivateProfileInt(szRailwayPath,"a",0,"data\\Monster.txt");
		m_pRailway->b=GetPrivateProfileInt(szRailwayPath,"b",0,"data\\Monster.txt");
		m_pRailway->r=GetPrivateProfileInt(szRailwayPath,"r",0,"data\\Monster.txt");
		m_pRailway->w=GetPrivateProfileInt(szRailwayPath,"w",0,"data\\Monster.txt");
		m_pRailway->CenterX=GetPrivateProfileInt(szRailwayPath,"CenterX",0,"data\\Monster.txt");
		m_pRailway->CenterY=GetPrivateProfileInt(szRailwayPath,"CenterY",0,"data\\Monster.txt");
	}
	m_pMonsterMesh->LoadFromXFile(szfilePath);
	m_pMonsterMesh->SetScale(m_xMonsterInfo.scale*0.1f);
	m_pMonsterMesh->SetRotateAngle(m_xMonsterInfo.rotateAngle);//角度为顺时针的
	m_fAngel=m_xMonsterInfo.rotateAngle;
	D3DXMATRIXA16 scale; 
//	m_pMonsterMesh->SetAnimationName("death");
	D3DXMatrixScaling(&scale,m_xMonsterInfo.scale*0.1f,m_xMonsterInfo.scale*0.1f,m_xMonsterInfo.scale*0.1f);
	m_pMonsterMesh->ApplyRectify(&(m_xMonsterInfo.rotateAngle*scale),m_xMonsterInfo.scale*0.1f);
	m_pMonsterMesh->SetPosition(D3DXVECTOR3(m_xMonsterInfo.x,m_xMonsterInfo.modelhigh+m_xMonsterInfo.y,m_xMonsterInfo.z));
	

	if (nId==2)
	{
			GetAnimPeriod();
	}

}
void CMonster::Update(float nLoopTime)
{	

	static float fDeadtime=0;
	static float fCurtime=0;
	fCurtime+=nLoopTime;

	if (IsDead())
	{
		if (fDeadtime<0.000001)
		{
			m_pMonsterMesh->SetAnimationName("death");
		}
		else if (fDeadtime>m_fAnimation[5]-0.1)
		{
			//触发掉落装备
			return;
		}
		fDeadtime+=nLoopTime;
			m_pMonsterMesh->Update(nLoopTime);
		return;
	}

// 	g_pCamera->Setup2DCamera();
// 	char buf[255];
// 	sprintf(buf,"%f",m_fAnimation[5])
// 	g_pFont->DrawText("")
	if (m_xMonsterInfo.RailwayIndex!=0)
	{
		int x=m_pRailway->CenterX+cosf(fCurtime/m_pRailway->w)*m_pRailway->r/m_pRailway->a;
		int z=m_pRailway->CenterY+sinf(fCurtime/m_pRailway->w)*m_pRailway->r/m_pRailway->b;
		int nflag=z-m_pRailway->CenterY;
		m_pMonsterMesh->SetRotateAngle(D3DX_PI/2*nflag/sqrt(nflag*nflag)-atanf(m_pRailway->a/m_pRailway->b*tanf(fCurtime/m_pRailway->w)));
        m_pMonsterMesh->SetPosition(D3DXVECTOR3(x,m_xMonsterInfo.modelhigh+m_xMonsterInfo.y,z));		
	}

	static float fPlaytime=0;
	if (m_xMonsterInfo.Id==2)
	{
		if (m_nCurAction==4)
		{
			m_xMonsterInfo.x+=120.0*nLoopTime*cosf(m_fAngel);
	        m_xMonsterInfo.z+=120.0*nLoopTime*sinf(m_fAngel);
		}
	//	m_pMonsterMesh->SetAnimationNum(4);
		if (fPlaytime>m_fAnimation[m_nCurAction])
		{
			fPlaytime=0;
			AI(nLoopTime);		
		}
		fPlaytime+=nLoopTime;
	}
  
//	m_pMonsterMesh->SetRotateAngle(D3DX_PI-nCurtime/60*2*D3DX_PI);
//	m_pMonsterMesh->SetPosition(D3DXVECTOR3(0,m_xMonsterInfo.modelhigh,100*m_xMonsterInfo.Id));
	m_pMonsterMesh->Update(nLoopTime);

}
void CMonster::Render()
{
	m_pMonsterMesh->Render();
}
bool CMonster::IsDead()
{
	if(m_xMonsterInfo.Hp<=0)
	{
		m_xMonsterInfo.Hp=0;
		return true;
	}
	else
		return false;
}
void CMonster::AI(float nLoopTime)
{
// 	m_pMonsterMesh->SetAnimationNum(4);
// 	m_nCurAction=4;
	if (CalculateTagDistanceSquece()<80*80&&g_pPlayer->m_nState!=14)
 	{
 		//攻击
 		Attack();
 	}
 	else if (CalculateTagDistanceSquece()<300*300&&g_pPlayer->m_nState!=14)
 	{
 		//追击
 		Chase();
 	}
 	else
 	{
 		m_pMonsterMesh->SetAnimationNum(0);
 	    m_nCurAction=0;
 	}
}
void CMonster::Attack()
{
	int nHpPercent=10*m_xMonsterInfo.Hp/m_xMonsterInfo.MaxHp;
	if (nHpPercent<2)
	{
		m_nCurAction=3;
		m_pMonsterMesh->SetAnimationNum(3);
		g_pPlayer->m_xPlayInfo.Hp-=rand()%150+250;
	}
	else if (nHpPercent<8)
	{
		m_nCurAction=2;
		m_pMonsterMesh->SetAnimationNum(2);
		g_pPlayer->m_xPlayInfo.Hp-=rand()%150+100;
	}
	else
	{
		m_nCurAction=2;
		m_pMonsterMesh->SetAnimationNum(2);
		g_pPlayer->m_xPlayInfo.Hp-=rand()%150+100;	}

}
void CMonster::Chase()
{
	float x=g_pPlayer->m_vPos.x-m_xMonsterInfo.x;
	float z=g_pPlayer->m_vPos.z-m_xMonsterInfo.z;
	 
	if (x<0.000001)
	{
		m_fAngel=D3DX_PI+atanf(z/x);
	}
	else
	    m_fAngel=atanf(z/x);

	m_pMonsterMesh->SetRotateAngle(-1.0*m_fAngel);
	m_pMonsterMesh->SetAnimationNum(4);
	m_nCurAction=4;

//	m_pMonsterMesh->SetAnimationName("death");
    
}

float CMonster::CalculateTagDistanceSquece()
{
	return (g_pPlayer->m_vPos.x-m_xMonsterInfo.x)*
		(g_pPlayer->m_vPos.x-m_xMonsterInfo.x)+(g_pPlayer->m_vPos.z-m_xMonsterInfo.z)*(g_pPlayer->m_vPos.z-m_xMonsterInfo.z);
}

