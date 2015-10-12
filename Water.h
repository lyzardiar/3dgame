// Water.h: interface for the CWater class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WATER_H__6E5F422F_41EA_47DC_8479_0909E6F711C5__INCLUDED_)
#define AFX_WATER_H__6E5F422F_41EA_47DC_8479_0909E6F711C5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#define D3DFVF_SEAVERTEX (D3DFVF_XYZ|D3DFVF_TEX1)
class CWater  
{
public:
	struct SEAVERTEX 
	{
		float x,y,z;
		float u,v;
	};
	CWater();
	virtual ~CWater();
	void Init(LPDIRECT3DDEVICE9 pDevice,float nWidth,float nHeight,D3DXVECTOR3 vPos);
	void Update(float nLoopTime);
	void Render();
private:
	D3DMATERIAL9 m_mtrl;
	LPDIRECT3DDEVICE9 m_pDevice;
	D3DXVECTOR3 m_vPos;
	LPDIRECT3DTEXTURE9 m_pTexture;
	LPDIRECT3DVERTEXBUFFER9 m_pVertexBuffer;

};

#endif // !defined(AFX_WATER_H__6E5F422F_41EA_47DC_8479_0909E6F711C5__INCLUDED_)
