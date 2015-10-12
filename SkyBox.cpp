// SkyBox.cpp: implementation of the CSkyBox class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SkyBox.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
void BoxVertex::SetVertex( float fx, float fy, float fz, float ftu, float ftv )
{
	x = fx;
	y = fy;
	z = fz;
	tu = ftu;
	tv = ftv;
}

CSkyBox::CSkyBox( float fDistance ) :
m_fDistance( fDistance ),
m_pDevice(NULL),
m_pBoxVB( NULL )
{
	ZeroMemory( m_pBoxTextures, sizeof(m_pBoxTextures) );
}

CSkyBox::~CSkyBox( void )
{
	SAFE_RELEASE( m_pBoxVB );

	for ( int i = 0; i < 6; ++i )
	{
		SAFE_RELEASE( m_pBoxTextures[i] );
	}

}

void CSkyBox::Init(LPSTR pPrefix, 	LPDIRECT3DDEVICE9 pDevice, LPSTR exName )
{
	m_pDevice = pDevice;
	string TextureName;
	for( int i = 0; i < 6; ++i )
	{
		TextureName = pPrefix;
		switch(i)
		{
		case BP_FRONT:
			TextureName += "front";
			break;
		case BP_BACK:
			TextureName += "back";
			break;
		case BP_LEFT:
			TextureName += "left";
			break;
		case BP_RIGHT:
			TextureName += "right";
			break;
		case BP_UP:
			TextureName += "top";
			break;
		case BP_DOWN:
			TextureName += "bottom";
			break;
		}
		TextureName += exName;
		if(FAILED(
		D3DXCreateTextureFromFile( m_pDevice, 
			TextureName.c_str(), &m_pBoxTextures[i] ) ))
			m_pBoxTextures[i] = NULL;
	}

	// front
	m_BoxVertices[0].SetVertex(  -m_fDistance, m_fDistance,-m_fDistance,  0.0f,0.0f );
	m_BoxVertices[1].SetVertex(   m_fDistance, m_fDistance,-m_fDistance,  1.0f,0.0f );
	m_BoxVertices[2].SetVertex(  -m_fDistance,-m_fDistance,-m_fDistance,  0.0f,1.0f );
	m_BoxVertices[3].SetVertex(   m_fDistance,-m_fDistance,-m_fDistance,  1.0f,1.0f );

	// back
	m_BoxVertices[4].SetVertex(  -m_fDistance, m_fDistance, m_fDistance,  1.0f,0.0f );
	m_BoxVertices[5].SetVertex(  -m_fDistance,-m_fDistance, m_fDistance,  1.0f,1.0f );
	m_BoxVertices[6].SetVertex(   m_fDistance, m_fDistance, m_fDistance,  0.0f,0.0f );
	m_BoxVertices[7].SetVertex(   m_fDistance,-m_fDistance, m_fDistance,  0.0f,1.0f );

	// top
	m_BoxVertices[8].SetVertex(  -m_fDistance, m_fDistance, m_fDistance,  0.0f,0.0f );
	m_BoxVertices[9].SetVertex(   m_fDistance, m_fDistance, m_fDistance,  1.0f,0.0f );
	m_BoxVertices[10].SetVertex( -m_fDistance, m_fDistance,-m_fDistance,  0.0f,1.0f );
	m_BoxVertices[11].SetVertex(  m_fDistance, m_fDistance,-m_fDistance,  1.0f,1.0f );
	// bottom
	m_BoxVertices[12].SetVertex( -m_fDistance,-m_fDistance, m_fDistance,  0.0f,1.0f );
	m_BoxVertices[13].SetVertex( -m_fDistance,-m_fDistance,-m_fDistance,  0.0f,0.0f );
	m_BoxVertices[14].SetVertex(  m_fDistance,-m_fDistance, m_fDistance,  1.0f,1.0f );
	m_BoxVertices[15].SetVertex(  m_fDistance,-m_fDistance,-m_fDistance,  1.0f,0.0f );

	// right
	m_BoxVertices[16].SetVertex(  m_fDistance, m_fDistance,-m_fDistance,  0.0f,0.0f );
	m_BoxVertices[17].SetVertex(  m_fDistance, m_fDistance, m_fDistance,  1.0f,0.0f );
	m_BoxVertices[18].SetVertex(  m_fDistance,-m_fDistance,-m_fDistance,  0.0f,1.0f );
	m_BoxVertices[19].SetVertex(  m_fDistance,-m_fDistance, m_fDistance,  1.0f,1.0f );

	// left
	m_BoxVertices[20].SetVertex( -m_fDistance, m_fDistance,-m_fDistance,  1.0f,0.0f );
	m_BoxVertices[21].SetVertex( -m_fDistance,-m_fDistance,-m_fDistance,  1.0f,1.0f );
	m_BoxVertices[22].SetVertex( -m_fDistance, m_fDistance, m_fDistance,  0.0f,0.0f );
	m_BoxVertices[23].SetVertex( -m_fDistance,-m_fDistance, m_fDistance,  0.0f,1.0f );

	m_pDevice->CreateVertexBuffer( 24* sizeof(BoxVertex), D3DUSAGE_WRITEONLY,
		BoxVertex::FVF_Flags, D3DPOOL_DEFAULT, &m_pBoxVB, 0 );

	void* pBuffer = NULL;
	m_pBoxVB->Lock( 0, 24* sizeof(BoxVertex), &pBuffer, 0 );
	memcpy( pBuffer, m_BoxVertices, 24* sizeof(BoxVertex) );
	m_pBoxVB->Unlock();
 
}

void CSkyBox::Render(D3DXVECTOR3 vEye)
{
	D3DXMATRIX matWorld,matRotY;
	static float rotY=0.0f;
	rotY+=0.0001f;

	D3DXMatrixRotationY(&matRotY,rotY);
	D3DXMatrixTranslation( &matWorld,vEye.x, vEye.y, vEye.z );
	matWorld=matRotY*matWorld;
	m_pDevice->SetTransform( D3DTS_WORLD, &matWorld );

	DWORD Vl_D3DSAMP_ADDRESSU=NULL;
	DWORD Vl_D3DSAMP_ADDRESSV=NULL;
	m_pDevice->GetSamplerState( 0, D3DSAMP_ADDRESSU, &Vl_D3DSAMP_ADDRESSU );
	m_pDevice->GetSamplerState( 0, D3DSAMP_ADDRESSV, &Vl_D3DSAMP_ADDRESSV );

	/*m_pDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE );
	m_pDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );
	m_pDevice->SetRenderState( D3DRS_ZWRITEENABLE, FALSE );
	m_pDevice->SetRenderState( D3DRS_FOGENABLE, FALSE );*/
	m_pDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_CW );
	m_pDevice->SetRenderState( D3DRS_LIGHTING, FALSE );
	m_pDevice->SetSamplerState( 0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP );
	m_pDevice->SetSamplerState( 0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP );
	m_pDevice->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );
	m_pDevice->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );
	m_pDevice->SetStreamSource( 0, m_pBoxVB, 0, sizeof(BoxVertex) );
	m_pDevice->SetFVF( BoxVertex::FVF_Flags );

	for( int i = 0; i < 6; ++i )
	{
		m_pDevice->SetTexture( 0, m_pBoxTextures[i] );
		m_pDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP,  i*4, 2 );
	}
	m_pDevice->SetSamplerState( 0, D3DSAMP_ADDRESSU, Vl_D3DSAMP_ADDRESSU );
	m_pDevice->SetSamplerState( 0, D3DSAMP_ADDRESSV, Vl_D3DSAMP_ADDRESSV );
	m_pDevice->SetRenderState( D3DRS_LIGHTING, TRUE );
	m_pDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_CCW );
	/*m_pDevice->SetRenderState( D3DRS_ZWRITEENABLE, TRUE );*/
}