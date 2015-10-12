// ItemBtn.cpp: implementation of the CItemBtn class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ItemBtn.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
#define ITEMWIDTH 48
#define ITEMHEIGHT 48
CItemBtn::CItemBtn(int x,int y)
{
	m_Rect.left=x;
	m_Rect.top=y;
	m_Rect.right=x+ITEMWIDTH;
	m_Rect.bottom=y+ITEMHEIGHT;

	m_pPanel=new CPanel();
	m_pPanel->UpdateVertices(ITEMWIDTH,ITEMHEIGHT);
	m_bType=0;

	
   m_bShowItemInfo=false;

}

CItemBtn::~CItemBtn()
{

}
void CItemBtn::AddBtnInfo(LPSkillInfo pSkillInfo)
{
	m_bType=1;
	m_pSkillInfo=pSkillInfo;
	D3DXCreateTextureFromFileEx(g_pGame->GetDevice(),m_pSkillInfo->ImagePath,
		D3DX_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, 0, 	D3DFMT_UNKNOWN, D3DPOOL_MANAGED,
		D3DX_DEFAULT, D3DX_DEFAULT, D3DCOLOR_XRGB(0,0,0), NULL, NULL, &m_pTexture);
}
void CItemBtn::AddBtnInfo(LPItemInfo pItemInfo)
{
	m_bType=2;
	m_pItemInfo=pItemInfo;
	D3DXCreateTextureFromFileEx(g_pGame->GetDevice(), m_pItemInfo->ImagePath, 
		D3DX_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, 0, 	D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, D3DCOLOR_XRGB(0,0,0), NULL, NULL, &m_pTexture);
}
bool CItemBtn::OnLButtonDown(POINT ptMouse)
{
	if (!IsInRect(ptMouse.x+4,ptMouse.y+16))
	{
		return false;
	}
    CastSkill();
	return true;
}
void CItemBtn::UseItem()
{

}
bool CItemBtn::CastSkill()
{
	if (m_bType!=1)
	{
		return false;
	}
	if(g_pPlayer->m_pTargetInfo!=NULL&&g_pPlayer->m_pTargetInfo->Id==2)//目标为怪物
	{
		if (g_pPlayer->m_pTargetInfo->Hp==0)
		{
			sprintf(g_szBuf,"目标已经死亡");
			return false;
		}
		if( m_pSkillInfo->Cooldown ==0 )//冷却时间检测
		{
			if(g_pPlayer->m_xPlayInfo.Mp >= m_pSkillInfo->aMp * (-1))//魔法值检测
			{
				if(m_pSkillInfo->Distance==0||g_pPlayer->CalculateTagDistanceSquece()<=m_pSkillInfo->Distance*m_pSkillInfo->Distance)//距离检测
				{
					m_pSkillInfo->Cooldown = m_pSkillInfo->MaxCooldown ;
					g_pPlayer->m_pTargetInfo->Hp-=m_pSkillInfo->demage;
					if((g_pPlayer->m_xPlayInfo.Hp+=m_pSkillInfo->aHp)>g_pPlayer->m_xPlayInfo.MaxHp)
						g_pPlayer->m_xPlayInfo.Hp=g_pPlayer->m_xPlayInfo.MaxHp;
					if((g_pPlayer->m_xPlayInfo.Mp+=m_pSkillInfo->aMp)>g_pPlayer->m_xPlayInfo.MaxMp)
						g_pPlayer->m_xPlayInfo.Mp=g_pPlayer->m_xPlayInfo.MaxMp;
					sprintf(g_szBuf,"%s\n",m_pSkillInfo->name);
					if((g_pPlayer->m_xPlayInfo.Hp+=m_pSkillInfo->aHp)>g_pPlayer->m_xPlayInfo.MaxHp)
						g_pPlayer->m_xPlayInfo.Hp=g_pPlayer->m_xPlayInfo.MaxHp;
					if((g_pPlayer->m_xPlayInfo.Mp+=m_pSkillInfo->aMp)>g_pPlayer->m_xPlayInfo.MaxMp)
						g_pPlayer->m_xPlayInfo.Mp=g_pPlayer->m_xPlayInfo.MaxMp;
					return true;
				}
				else
				{
					sprintf(g_szBuf,"距离太远");
				}
			}
			else
			{
				sprintf(g_szBuf,"法力值不足");
			}
		}
		else 
			sprintf(g_szBuf,"我还不能使用这项技能");
	}
	else if(g_pPlayer->m_pTargetInfo==NULL)
		sprintf(g_szBuf,"我没有目标");
	else
		sprintf(g_szBuf,"我不能攻击这个目标");
	return false;
}
void CItemBtn::Update(float nLoopTime)
{
	if (m_bType==1&&m_pSkillInfo->Cooldown>0.000001)
	{
		m_pSkillInfo->Cooldown-=nLoopTime;
		if (m_pSkillInfo->Cooldown<=0.000001)
		{
			m_pSkillInfo->Cooldown=0;
		}
	}
}
bool CItemBtn::OnLButtonUp(POINT ptMouse)
{
	if (!IsInRect(ptMouse.x,ptMouse.y+12))
	{
		return false;
	}

    return true;
}
bool CItemBtn::OnMouseMove(POINT ptMouse)
{
   	if (!IsInRect(ptMouse.x+4,ptMouse.y+14))
	{
		m_bShowItemInfo=false;
		return false;
	}
	m_bShowItemInfo=true;
    return true;
}

bool CItemBtn::OnRButtonDown(POINT ptMouse)
{



    return true;
}
void CItemBtn::Render()
{
	if (m_bShowItemInfo)
	{
		if (m_bType==1)
		{
			g_pFont->DrawText(m_pSkillInfo->Info,224,m_Rect.top-50,D3DCOLOR_XRGB(0,0,0));
		}
		else if (m_bType==2)
		{
			g_pFont->DrawText(m_pItemInfo->Info,224,m_Rect.top-50,D3DCOLOR_XRGB(0,0,0));
		}
	}
	m_pPanel->MoveTo(m_Rect.left,m_Rect.top);
	m_pPanel->Render(m_pTexture);
	if (m_bType==1&&m_pSkillInfo->Cooldown>0.000001)
	{
		char buf[255];
		sprintf(buf,"%d",(int)m_pSkillInfo->Cooldown);
		g_pFont->DrawText(buf,m_Rect.left+12,m_Rect.top+12,D3DCOLOR_XRGB(255,0,0));
	}
}
void CItemBtn::LoadSkill(int nSkillIndex)
{
	m_bType=1;
	
	m_pSkillInfo=(LPSkillInfo)malloc(sizeof(SkillInfo));
	char buf[16];
	sprintf(buf,"Skill%d",nSkillIndex);
	GetPrivateProfileString(buf,"name","",m_pSkillInfo->name,64,"data\\Skill.txt");
	m_pSkillInfo->Distance=GetPrivateProfileInt(buf,"Distance",0,"data\\Skill.txt");
	m_pSkillInfo->MaxCooldown=GetPrivateProfileInt(buf,"MaxCooldown",0,"data\\Skill.txt");
	m_pSkillInfo->demage=GetPrivateProfileInt(buf,"demage",0,"data\\Skill.txt");
	m_pSkillInfo->resist=GetPrivateProfileInt(buf,"resist",0,"data\\Skill.txt");
	m_pSkillInfo->aHp=GetPrivateProfileInt(buf,"aHp",0,"data\\Skill.txt");
	m_pSkillInfo->aMp=GetPrivateProfileInt(buf,"aMp",0,"data\\Skill.txt");
	m_pSkillInfo->Cooldown=0;
	GetPrivateProfileString(buf,"Info","",m_pSkillInfo->Info,128,"data\\Skill.txt");
	GetPrivateProfileString(buf,"ImagePath","",m_pSkillInfo->ImagePath,64,"data\\Skill.txt");

	if (FAILED(D3DXCreateTextureFromFileEx(g_pGame->GetDevice(), m_pSkillInfo->ImagePath, 
		D3DX_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, 0, 	D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT, 
		D3DX_DEFAULT, D3DCOLOR_XRGB(0,0,0), NULL, NULL, &m_pTexture)))
	{
		MessageBox(GetActiveWindow(),"加载技能图片失败",m_pSkillInfo->ImagePath,MB_OK);
	}


}
void CItemBtn::LoadItem(int nItemIndex)
{
	m_bType=2;
	m_pItemInfo=(LPItemInfo)malloc(sizeof(ItemInfo));
	char buf[16];
	sprintf(buf,"item%d",nItemIndex);
	GetPrivateProfileString(buf,"name","",m_pItemInfo->name,64,"data\\Item.txt");
	m_pItemInfo->skillindex=GetPrivateProfileInt(buf,"skillindex",0,"data\\Item.txt");
	m_pItemInfo->value=GetPrivateProfileInt(buf,"value",0,"data\\Item.txt");
	m_pItemInfo->Celerity=GetPrivateProfileInt(buf,"Celerity",0,"data\\Item.txt");
	m_pItemInfo->Strength=GetPrivateProfileInt(buf,"Strength",0,"data\\Item.txt");
	m_pItemInfo->Brains=GetPrivateProfileInt(buf,"Brains",0,"data\\Item.txt");
	m_pItemInfo->Hp=GetPrivateProfileInt(buf,"Hp",0,"data\\Item.txt");
	GetPrivateProfileString(buf,"Info","",m_pItemInfo->Info,128,"data\\Item.txt");
	GetPrivateProfileString(buf,"ImagePath","",m_pItemInfo->ImagePath,64,"data\\Item.txt");
	
	D3DXCreateTextureFromFileEx(g_pGame->GetDevice(), m_pItemInfo->ImagePath, 
		D3DX_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, 0, 	D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT, 
		D3DX_DEFAULT, D3DCOLOR_XRGB(,0,0,0), NULL, NULL, &m_pTexture);

}