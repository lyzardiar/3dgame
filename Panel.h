// Panel.h: interface for the CPanel class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PANEL_H__22B0FAAD_937B_41CF_848E_C899BC8CEF50__INCLUDED_)
#define AFX_PANEL_H__22B0FAAD_937B_41CF_848E_C899BC8CEF50__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <d3dx9.h>

#define PANEL_D3DFVF_CUSTOMVERTEX (D3DFVF_XYZ|D3DFVF_DIFFUSE|D3DFVF_TEX1)

class CPanel  
{
private:
	//Define a custom vertex for our panel
	struct PANEL_CUSTOMVERTEX
	{
		FLOAT x, y, z;		//Position of vertex
		DWORD colour;		//Colour of vertex
		FLOAT u, v;			//Texture coordinates
	};
	
	public:
		void MoveTo(int x, int y);
		DWORD Render(LPDIRECT3DTEXTURE9 pTexture);
		CPanel(DWORD dwColour = -1);
		virtual ~CPanel();		
//		bool UpdateVertices();
		bool UpdateVertices(int nWidth=0,int nHeight=0,int i=0,int j=0,float u=1.0,float v=1.0);
		bool UpdateVertices(RECT rc,float u=1.0,float v=1.0);
		LPDIRECT3DVERTEXBUFFER9 m_pVertexBuffer;
		
	private:
		
		bool CreateVertexBuffer();		
		LPDIRECT3DDEVICE9 m_pD3DDevice;
		
		int m_nWidth;
		int m_nHeight;
		DWORD m_dwColour;
		
};

#endif // !defined(AFX_PANEL_H__22B0FAAD_937B_41CF_848E_C899BC8CEF50__INCLUDED_)
