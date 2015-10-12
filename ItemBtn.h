// ItemBtn.h: interface for the CItemBtn class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ITEMBTN_H__E12D93B6_CD5C_428A_9CEB_2690B49D62B6__INCLUDED_)
#define AFX_ITEMBTN_H__E12D93B6_CD5C_428A_9CEB_2690B49D62B6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

typedef struct SkillInfo
{
	char name[16];
	int Distance;
	int demage;
	int demageAdd;
	int Maxlevel;
	float Cooldown;
	int MaxCooldown;
	int resist;
	int resistAdd;
	int aHp;
	int aMp;
	char Info[128];
	int AnimationIndex;
	char ImagePath[128];
}SkillInfo,*LPSkillInfo;

typedef struct ItemInfo
{
	int id;
	char name[64];
	int Hp;
	int Mp;
	int Strength;//力量
	int Celerity;//敏捷
	int Brains;//智力
	int value;
	int skillindex;
	char ImagePath[128];
	char Info[128];
}ItemInfo,*LPItemInfo;

class CItemBtn  
{
public:
	CItemBtn(int x,int y);
	virtual ~CItemBtn();
	int m_clientWidth;		// 窗口客户区宽度
	int m_clientHeight;		// 窗口客户区高度
	
	BYTE			m_bBtnState;	

	LPItemInfo m_pItemInfo;
	LPSkillInfo m_pSkillInfo;
	BOOL m_bType;
	RECT			m_Rect;	

	CPanel *m_pPanel;

	bool m_bShowItemInfo;
	void AddBtnInfo(LPItemInfo pItemInfo);
	void AddBtnInfo(LPSkillInfo pSkillInfo);
	bool OnMouseMove(POINT ptMouse);
	bool OnRButtonDown(POINT ptMouse);
	bool OnLButtonDown(POINT ptMouse);
	bool OnLButtonUp(POINT ptMouse);
	void Render();

	void UseItem();
	bool CastSkill();

	void LoadSkill(int nSkillIndex);
	void LoadItem(int nItemIndex);

    void Update(float nLoopTime);
	__inline RECT GetGameBtnRect()
	{
		return m_Rect;
	}

	__inline bool IsInRect(int nPosX, int nPosY)
	{	
		if (nPosX >= m_Rect.left && nPosX <= m_Rect.right && 
			nPosY >= m_Rect.top && nPosY <= m_Rect.bottom) return true;
		else 
			return false;
	}
	
	LPDIRECT3DDEVICE9 m_pD3DDevice;
private:
	LPDIRECT3DTEXTURE9 m_pTexture;
	LPDIRECT3DVERTEXBUFFER9 m_pVB;
	int m_nWidth;
	int m_nHeigth;

};

#endif // !defined(AFX_ITEMBTN_H__E12D93B6_CD5C_428A_9CEB_2690B49D62B6__INCLUDED_)
