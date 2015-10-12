// Terrain.cpp: implementation of the CTerrain class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Terrain.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTerrain::CTerrain(LPDIRECT3DDEVICE9 pDevice,
				 char* heightmapFileName,
				 int numVertsPerRow,
				 int numVertsPerCol,
				 int cellSpacing,
				 float heightScale)
{
	m_pDevice=pDevice;
	m_numVertsPerRow=numVertsPerRow;
	m_numVertsPerCol=numVertsPerCol;
	m_cellSpacing=cellSpacing;

	m_numCellsPerRow=m_numVertsPerRow-1;
	m_numCellsPerCol=m_numVertsPerCol-1;

	m_width=m_numCellsPerRow*m_cellSpacing;
	m_depth=m_numCellsPerCol*m_cellSpacing;

	m_numVertices  = m_numVertsPerRow * m_numVertsPerCol;
	m_numTriangles = m_numCellsPerRow * m_numCellsPerCol *2;

	m_heightScale=heightScale;

	// load heightmap
	if( !readRawFile(heightmapFileName) )
	{
		::MessageBox(NULL,"readRawFile - FAILED",0,0);
		::PostQuitMessage(0);
	}



	// scale heights
	for (int i=0;i<m_heightmap.size();i++)
	{
		m_heightmap[i]*=m_heightScale;
	}

	// compute the vertices
	if( !computeVertices() )
	{
		::MessageBox(NULL,"computeVertices - FAILED",0,0);
		::PostQuitMessage(0);
	}

	// compute the indices
	if(  !computeIndices() )
	{
		::MessageBox(NULL,"computeIndices - FAILED",0,0);
		::PostQuitMessage(0);
	}


	loadTexture();
	BuildEffect();
}

CTerrain::~CTerrain(void)
{
	SAFE_RELEASE(m_pVB);
	SAFE_RELEASE(m_pIB);
}

int CTerrain::getHeightmapEntry(int row,int col)
{
	if (row<0 || col<0 || row>=m_numVertsPerRow || col>=m_numVertsPerCol)
	{
		return 0;
	}
	return m_heightmap[row*m_numVertsPerRow + col];
}






std::vector<D3DXVECTOR3> CTerrain::ComputeNormals()
{
	///计算地形的法线信息
	std::vector<D3DXVECTOR3> normal;
	for(int i=0;i<m_numVertsPerCol;i++)
	{
		for(int j=0;j<m_numVertsPerRow;j++)
		{
			normal.push_back(CreateNormal(i,j));
		}
	}
	return	normal;
}
D3DXVECTOR3	CTerrain::CreateNormal(int i,int j)
{

	D3DXVECTOR3	avg=D3DXVECTOR3(0,0,0);
	D3DXVECTOR3	nTemp,uTemp,vTemp;
	float		nums=0.0f;
	for(int m=i-1;m<=i;m++)
	{
		for(int n=j-1;n<=j;n++)
		{
			if(m<0||n<0||m>=m_numVertsPerRow-1||n>=m_numVertsPerCol-1)
			{
				avg+=D3DXVECTOR3(0,1,0);
			}
			else
			{
				avg+=quadNormal(m,n);
			}
			nums+=1.0f;
		}
	}
	avg/=nums;
	D3DXVec3Normalize(&avg,&avg);
	return	avg;
}




D3DXVECTOR3	CTerrain::quadNormal(int	i,int	j)
{
	// Function computes the normal for the ijth quad.
	// We take the quad normal as the average of the two
	// triangles that make up the quad.
	//
	//       u
	// h0*-------*h1
	//   |      /|
	//  v|    /  |t
	//   |  /    |
	//   |/      |
	// h2*-------*h3
	//       s

	float h0 = (float)m_heightmap[i*m_numVertsPerRow+j];
	float h1 = (float)m_heightmap[i*m_numVertsPerRow+j+1]; 
	float h2 = (float)m_heightmap[(i+1)*m_numVertsPerRow+j];  
	float h3 = (float)m_heightmap[(i+1)*m_numVertsPerRow+j+1];

	D3DXVECTOR3 u = D3DXVECTOR3(m_cellSpacing, h1-h0, 0.0f);
	D3DXVECTOR3 v = D3DXVECTOR3(0.0f, h2-h0, -m_cellSpacing);

	D3DXVECTOR3 s = D3DXVECTOR3(-m_cellSpacing, h2-h3, 0.0f);
	D3DXVECTOR3 t = D3DXVECTOR3(0.0f, h1-h3, m_cellSpacing);

	D3DXVECTOR3 n1(0.0f, 1.0f, 0.0f);
	D3DXVec3Cross(&n1, &u, &v);
	D3DXVec3Normalize(&n1, &n1);

	D3DXVECTOR3 n2(0.0f, 1.0f, 0.0f);
	D3DXVec3Cross(&n2, &s, &t);
	D3DXVec3Normalize(&n2, &n2);

	return (n1 + n2) * 0.5f;
}



bool CTerrain::computeVertices()
{
	HRESULT hr = 0;
	std::vector<D3DXVECTOR3>normal=ComputeNormals();

	hr = m_pDevice->CreateVertexBuffer(
		m_numVertices * sizeof(TerrainVertex),
		D3DUSAGE_WRITEONLY,
		TerrainVertex_FVF,
		D3DPOOL_MANAGED,
		&m_pVB,
		0);
	if(FAILED(hr))
		return false;

	int startX = -m_width / 2;
	int startZ =  m_depth / 2;
	int endX =  m_width / 2;
	int endZ = -m_depth / 2;	


	TerrainVertex* v = NULL;
	m_pVB->Lock(0, 0, (void**)&v, 0);

	int i = 0;
	for(int z = startZ; z >= endZ; z -= m_cellSpacing)
	{
		int j = 0;
		for(int x = startX; x <= endX; x += m_cellSpacing)
		{
			int index = i * m_numVertsPerRow + j;

			v->pos = D3DXVECTOR3(
				(float)x,
				(float)m_heightmap[index],
				(float)z);
			v->tex1=D3DXVECTOR2(j,i);
			v->normal=normal[i*m_numVertsPerRow+j];
			v->tex2=D3DXVECTOR2((float)j / (float)m_numVertsPerCol,(float)i / (float)m_numVertsPerRow);
			v++;
			j++;
		}
		i++;
	}

	m_pVB->Unlock();
	return true;
}

bool CTerrain::computeIndices()
{
	HRESULT hr=0;

	hr=m_pDevice->CreateIndexBuffer(
		m_numTriangles * 3 *sizeof(WORD),
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&m_pIB,
		NULL);
	if(FAILED(hr))
		return false;

	WORD *indices=NULL;
	m_pIB->Lock(0,0,(void **)&indices,0);

	int baseIndex=0;
	for (int i=0;i<m_numCellsPerCol;i++)
	{	
		for (int j=0;j<m_numCellsPerRow;j++)
		{
			indices[baseIndex]   = i*    m_numVertsPerRow + j;
			indices[baseIndex+1] = i*    m_numVertsPerRow + j+1;
			indices[baseIndex+2] = (i+1)*m_numVertsPerRow + j;

			indices[baseIndex+3] = (i+1)*m_numVertsPerRow + j;
			indices[baseIndex+4] = i*    m_numVertsPerRow + j+1;
			indices[baseIndex+5] = (i+1)*m_numVertsPerRow + j+1;

			baseIndex+=6;
		}

	}

	m_pIB->Unlock();
	return true;
}



void CTerrain::loadTexture()
{
	D3DXCreateTextureFromFile(m_pDevice,TEXT("Terrain/stone.jpg"),&mLayerMap2);
	D3DXCreateTextureFromFile(m_pDevice,TEXT("Terrain/grasspatch.bmp"),&mLayerMap1);
	D3DXCreateTextureFromFile(m_pDevice,TEXT("Terrain/cliff1.bmp"),&mLayerMap0);
	D3DXCreateTextureFromFile(m_pDevice,TEXT("Terrain/blend.jpg"),&mBlendMap);
	
}



bool CTerrain::readRawFile(string fileName)
{
	std::vector<BYTE> in(m_numVertices);

	std::ifstream inFile(fileName.c_str(),std::ios_base::binary);
	if(inFile==NULL)
		return false;

	inFile.read((char *)&in[0],in.size());

	inFile.close();

	m_heightmap.resize(m_numVertices);
	for(int i=0;i<m_numVertices;i++)
	{
		m_heightmap[i]=in[i];
	}

	FilterMap(); // 平滑
	return true;
}

float CTerrain::getHeight(float x,float z)
{
	x = x + (float)m_width/2.0f;
	z =-z + (float)m_depth/2.0f;

	x /= (float)m_cellSpacing;
	z /= (float)m_cellSpacing;

	float col = ::floorf(x);
	float row = ::floorf(z);

	//  A   B
	//  *---*
	//  | / |
	//  *---*  
	//  C   D
	
	float A=getHeightmapEntry(row,   col);
	float B=getHeightmapEntry(row,   col+1);
	float C=getHeightmapEntry(row+1, col);
	float D=getHeightmapEntry(row+1, col+1);

	//
	float dx=x-col;
	float dz=z-row;

	float height=0.0f;
	if(dz+dx<1.0f)
	{
		float uy=B-A;
		float vy=C-A;
		height = A + uy*dx + vy*dz;
	}
	else
	{
		float uy=C-D;
		float vy=B-D;
		height = D + uy*(1.0f-dx) + vy*(1.0f-dz);
	}

	return height;
}

void CTerrain::PreRender()
{
	// Set texture filters.
	m_pDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	m_pDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	m_pDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

	//address mode
	m_pDevice->SetSamplerState(0,D3DSAMP_ADDRESSU,D3DTADDRESS_WRAP);
	m_pDevice->SetSamplerState(0,D3DSAMP_ADDRESSV,D3DTADDRESS_WRAP);
}

void CTerrain::EndRender()
{

}

bool CTerrain::Render(D3DXMATRIX* matViewProj,bool bRenderTris)
{
	HRESULT hr=0;

	if(m_pDevice)
	{
		PreRender();

		//begin effect
		D3DXMATRIX	viewproj,matWorld;
		D3DXMatrixTranslation(&matWorld,0,0,0);
		viewproj=matWorld*(*matViewProj);
		mTerrainFX->SetMatrix(mhWVP,&viewproj);
		mTerrainFX->SetTechnique(mhTech);

    	m_pDevice->SetRenderState(D3DRS_LIGHTING, true);
// 		D3DXMATRIXA16 matWorld1;
// 		D3DXMatrixScaling(&matWorld1, 10000, 10000,10000);
// 		m_pDevice->SetTransform( D3DTS_WORLD, &matWorld1 );

		UINT numPasses = 0;
		mTerrainFX->Begin(&numPasses, 0);
		mTerrainFX->Pass(0);
/*		mTerrainFX->BeginPass(0);*/
		//end effect

		m_pDevice->SetStreamSource(0,m_pVB,0,sizeof(TerrainVertex));
		m_pDevice->SetFVF(TerrainVertex_FVF);
		m_pDevice->SetIndices(m_pIB);
		hr=m_pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,0,0,m_numVertices,0,m_numTriangles);

		if(bRenderTris)//是否画线框
		{
			m_pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
			hr =m_pDevice->DrawIndexedPrimitive(
				D3DPT_TRIANGLELIST,
				0,
				0,
				m_numVertices,
				0,
				m_numTriangles);
			m_pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
		}

		mTerrainFX->End();

		EndRender();

		if(FAILED(hr))
			return false;
	}//end if

	return true;
}






void CTerrain::BuildEffect()
{
	ID3DXBuffer* errors = 0;
	D3DXCreateEffectFromFile(m_pDevice, "terrain_multitexture.fx",
		0, 0, D3DXSHADER_DEBUG, 0, &mTerrainFX, &errors);

	if(errors)
	{
		MessageBox(0, (char*)errors->GetBufferPointer(), 0, 0);
		SAFE_RELEASE(errors);
	}

	mhTech      = mTerrainFX->GetTechniqueByName("Terrain_MultiTex");
	mhWVP       = mTerrainFX->GetParameterByName(0, "gWVP");
	mhTexScale  = mTerrainFX->GetParameterByName(0, "gTexScale");
	mhDirToSunW = mTerrainFX->GetParameterByName(0, "gDirToSunW");

	mhLayerMap0   = mTerrainFX->GetParameterByName(0, "gLayerMap0");
	mhLayerMap1   = mTerrainFX->GetParameterByName(0, "gLayerMap1");
	mhLayerMap2   = mTerrainFX->GetParameterByName(0, "gLayerMap2");
	mhBlendMap    = mTerrainFX->GetParameterByName(0, "gBlendMap");

	mTerrainFX->SetTexture(mhLayerMap0, mLayerMap0);
	mTerrainFX->SetTexture(mhLayerMap1, mLayerMap1);
	mTerrainFX->SetTexture(mhLayerMap2, mLayerMap2);
	mTerrainFX->SetTexture(mhBlendMap, mBlendMap);

	texScale=D3DXVECTOR2(0.06f,0.06f);
	dirToSun=D3DXVECTOR3(0.f,1.f,0.f);
	mTerrainFX->SetValue(mhTexScale, &texScale, sizeof(D3DXVECTOR2));
	mTerrainFX->SetValue(mhDirToSunW, &dirToSun, sizeof(D3DXVECTOR3));
}

void	CTerrain::FilterMap()
{
	for(int i=0;i<m_numVertsPerCol;i++)
	{
		for(int j=0;j<m_numVertsPerRow;j++)
		{
			m_heightmap[i*m_numVertsPerRow+j]=FilterHeight3X3(i,j);
		}
	}
}


float	CTerrain::FilterHeight3X3(int i,int j)
{
	//
	// ----------
	// | 1| 2| 3|
	// ----------
	// |4 |ij| 6|
	// ----------
	// | 7| 8| 9|
	// ----------
	float avg = 0.0f;
	float num = 0.0f;

	for(int m = i-1; m <= i+1; ++m)
	{
		for(int n = j-1; n <= j+1; ++n)
		{
			if( inBounds(m,n) )
			{
				avg += m_heightmap[i*m_numVertsPerRow+j];
				num += 1.0f;
			}
		}
	}

	return avg / num;
}



bool CTerrain::inBounds(int i, int j)
{
	return 
		i >= 0 && 
		i <= m_numVertsPerCol && 
		j >= 0 && 
		j <= m_numVertsPerRow;
}
