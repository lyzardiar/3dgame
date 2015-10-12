// Terrain.h: interface for the CTerrain class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TERRAIN_H__777AB22C_F9BC_411A_8469_212557D3E237__INCLUDED_)
#define AFX_TERRAIN_H__777AB22C_F9BC_411A_8469_212557D3E237__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "StdAfx.h"
#define		TerrainVertex_FVF	D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_TEX2
class CTerrain  
{
public:
CTerrain(LPDIRECT3DDEVICE9 pDevice,
		char* heightmapFileName,
		int numVertsPerRow,
		int numVertsPerCol,
		int cellSpacing,
		float heightScale);

virtual ~CTerrain(void);

int   getHeightmapEntry(int row,int col);
float getHeight(float x,float z);

void  loadTexture();
bool  genTexture(D3DXVECTOR3 *directionToLight);

void  PreRender();
void  EndRender();
bool  Render(D3DXMATRIX* matViewProj,bool bRenderTris);
void	FilterMap();
float	FilterHeight3X3(int i,int j);
bool	inBounds(int i, int j);//判断顶点是否在地图之上

void BuildEffect();

private:
	LPDIRECT3DDEVICE9		m_pDevice;
	LPDIRECT3DVERTEXBUFFER9	m_pVB;
	LPDIRECT3DINDEXBUFFER9	m_pIB;
	
	//效果相关
	IDirect3DVertexDeclaration9* DECL;
	IDirect3DTexture9*      mLayerMap0;
	IDirect3DTexture9*      mLayerMap1;
	IDirect3DTexture9*      mLayerMap2;
	IDirect3DTexture9*      mBlendMap;
	ID3DXEffect*            mTerrainFX;
	D3DXHANDLE              mhTech;
	D3DXHANDLE              mhWVP;
	D3DXHANDLE              mhTexScale;
	D3DXHANDLE              mhDirToSunW;
	D3DXHANDLE              mhLayerMap0;
	D3DXHANDLE              mhLayerMap1;
	D3DXHANDLE              mhLayerMap2;
	D3DXHANDLE              mhBlendMap;
	D3DXVECTOR2				texScale;
	D3DXVECTOR3				dirToSun;
	
	int m_numVertsPerRow;
	int m_numVertsPerCol;
	int m_cellSpacing;
	
	int m_numCellsPerRow;
	int m_numCellsPerCol;
	int m_width;
	int m_depth;
	int m_numVertices;
	int m_numTriangles;

	float m_heightScale;

    vector<int> m_heightmap;


	bool  readRawFile(string fileName);
	bool  computeVertices();
	bool  computeIndices();
	std::vector<D3DXVECTOR3>	ComputeNormals();//计算地形法线信息
	D3DXVECTOR3	CreateNormal(int i,int j);//生成法线
	D3DXVECTOR3	quadNormal(int	i,int	j);

	struct TerrainVertex 
	{
		D3DXVECTOR3		pos;
		D3DXVECTOR3		normal;
		D3DXVECTOR2		tex1;
		D3DXVECTOR2		tex2;
	};
};

#endif // !defined(AFX_TERRAIN_H__777AB22C_F9BC_411A_8469_212557D3E237__INCLUDED_)
