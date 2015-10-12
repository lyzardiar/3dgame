// Water.cpp: implementation of the CWater class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Water.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CWater::CWater()
{

}

CWater::~CWater()
{
	if(m_pTexture!=NULL)
		m_pTexture->Release();
	
	if(m_pVertexBuffer!=NULL)
		m_pVertexBuffer->Release();

}
void CWater::Init(LPDIRECT3DDEVICE9 pDevice,float nWidth,float nHeight,D3DXVECTOR3 vPos)
{
	m_pDevice=pDevice;
	m_vPos=vPos;

	m_mtrl.Ambient=D3DXCOLOR(D3DCOLOR_XRGB(255,255,255));
	m_mtrl.Diffuse=D3DXCOLOR(D3DCOLOR_XRGB(255,255,255));
	m_mtrl.Specular=D3DXCOLOR(D3DCOLOR_XRGB(255,255,255));
	m_mtrl.Emissive=D3DXCOLOR(D3DCOLOR_XRGB(0,0,0));
	m_mtrl.Power=2.0f;
	m_mtrl.Diffuse.a=0.7f;
	m_mtrl.Specular.a=1.0f;

	SEAVERTEX vertices[]=
	{
		{-nWidth/2,0.0f,-nHeight/2,0.0f,5.86f,},
		{-nWidth/2,0.0f, nHeight/2,0.0f,0.0f ,},
		{ nWidth/2,0.0f,-nHeight/2,4.68f,5.86f,},
		{ nWidth/2,0.0f, nHeight/2,4.68f,0.0f,}
	};

	m_pDevice->CreateVertexBuffer(4*sizeof(SEAVERTEX),0,D3DFVF_SEAVERTEX,D3DPOOL_DEFAULT,&m_pVertexBuffer,NULL);

	VOID* pVertices;
	m_pVertexBuffer->Lock(0, sizeof(SEAVERTEX), (void**)&pVertices, 0);
	memcpy(pVertices,vertices,sizeof(vertices));	
	m_pVertexBuffer->Unlock();

	D3DXCreateTextureFromFile(m_pDevice,"Water/water2.bmp",&m_pTexture);
}
void CWater::Render()
{
	D3DXMATRIX matWorld;
	D3DXMatrixTranslation(&matWorld,m_vPos.x,m_vPos.y,m_vPos.z);
	m_pDevice->SetTransform(D3DTS_WORLD,&matWorld);

// 	m_pDevice->SetRenderState(D3DRS_FOGENABLE,TRUE);
// 	m_pDevice->SetRenderState(D3DRS_FOGCOLOR,0xffffffff);
// 	m_pDevice->SetRenderState(D3DRS_FOGTABLEMODE,D3DFOG_LINEAR);
// 	float fogStart=0;
// 	float fogEnd=10;
// 	m_pDevice->SetRenderState(D3DRS_FOGSTART,*(DWORD*)&fogStart);
// 	m_pDevice->SetRenderState(D3DRS_FOGEND,*(DWORD*)&fogEnd);

	m_pDevice->SetRenderState(D3DRS_LIGHTING,TRUE);
//	m_pDevice->SetSamplerState(0,D3DSAMP_ADDRESSU,D3DTADDRESS_WRAP);
	
	//blend
	m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE,true);
	m_pDevice->SetTextureStageState(0,D3DTSS_ALPHAARG1,D3DTA_DIFFUSE);
	m_pDevice->SetTextureStageState(0,D3DTSS_ALPHAOP,D3DTOP_SELECTARG1);
	m_pDevice->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_SRCALPHA);
	m_pDevice->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_INVSRCALPHA);
// 	m_pDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_SELECTARG1 );
// 	m_pDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
// 	m_pDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_DISABLE );

	m_pDevice->SetStreamSource(0, m_pVertexBuffer,0, sizeof(SEAVERTEX));
	m_pDevice->SetFVF(D3DFVF_SEAVERTEX);
	m_pDevice->SetTexture(0, m_pTexture);
	m_pDevice->SetMaterial(&m_mtrl);
	m_pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

/*	m_pDevice->SetRenderState(D3DRS_FOGENABLE,FALSE);*/
// 	m_pDevice->SetRenderState(D3DRS_LIGHTING,FALSE);
// 	m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE,FALSE);
}
void CWater::Update(float nLoopTime)
{
	float fSpeed = nLoopTime/25.f;
	/*	lastTime=currTime;*/
	
	SEAVERTEX *pV=NULL;
	m_pVertexBuffer->Lock(0, sizeof(SEAVERTEX), (void**)&pV, 0);	
	
	pV[0].v += fSpeed;
	pV[1].v += fSpeed;
	pV[2].v += fSpeed;
	pV[3].v += fSpeed;
	
	if(pV[0].u >= 4.68f)
	{
		pV[0].v = 0.0f;
		pV[1].v = 0.0f;
		pV[2].v = 5.86f;
		pV[3].v = pV[2].v;
	}
	
	m_pVertexBuffer->Unlock();
}