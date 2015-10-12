// SkyBox.h: interface for the CSkyBox class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SKYBOX_H__F6FBEBDE_0FF5_40D6_97D2_EB29F2B86468__INCLUDED_)
#define AFX_SKYBOX_H__F6FBEBDE_0FF5_40D6_97D2_EB29F2B86468__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

enum BoxPosition
{
	BP_BACK = 0,
	BP_FRONT,
	BP_UP,
	BP_DOWN,
	BP_RIGHT,
	BP_LEFT,
};
struct BoxVertex
{
	float x, y, z;
	float tu, tv;

	void SetVertex( float fx, float fy, float fz, float ftu, float ftv );

	enum FVF
	{
		FVF_Flags = D3DFVF_XYZ | D3DFVF_TEX1
	};
};

//class Camera;

class CSkyBox
{
protected:
	float									m_fDistance;	// 自由设置天空盒的大小
	BoxVertex								m_BoxVertices[24];
	LPDIRECT3DVERTEXBUFFER9					m_pBoxVB;
	LPDIRECT3DTEXTURE9						m_pBoxTextures[6];
	LPDIRECT3DDEVICE9						m_pDevice;
	
public:
	CSkyBox( float fDistance );
	~CSkyBox( void );
	void Init(LPSTR pPrefix, LPDIRECT3DDEVICE9 pDevice, LPSTR exName=".jpg" );
	void Render(D3DXVECTOR3 vEye);
};

#endif // !defined(AFX_SKYBOX_H__F6FBEBDE_0FF5_40D6_97D2_EB29F2B86468__INCLUDED_)
