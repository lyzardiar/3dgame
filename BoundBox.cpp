// BoundBox.cpp: implementation of the CBoundBox class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BoundBox.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
CBoundBox::CBoundBox(LPDIRECT3DDEVICE9 pDevice)
{
	ResetMat();

	//for( int i = 0; i < 8; i++ )
	//{
	//	m_pTrueVertices[i] = new D3DXVECTOR3( 0.f, 0.f, 0.f );
	//}

	m_pDevice=pDevice;
	m_box=NULL,
		m_boxOrig=NULL,
		m_pCollideInfo=NULL,
		//tmp_pVertexBuffer(NULL),
	m_pPlaneTop		=	new D3DXPLANE;
	m_pPlaneBottom	=	new D3DXPLANE;
	m_pPlaneSides[0]=	new D3DXPLANE;
	m_pPlaneSides[1]=	new D3DXPLANE;
	m_pPlaneSides[2]=	new D3DXPLANE;
	m_pPlaneSides[3]=	new D3DXPLANE;

	m_pCollideInfo	=	new CollideInfo;

	m_vCenterPoint.x = 0.f;
	m_vCenterPoint.y = 0.f;
	m_vCenterPoint.z = 0.f;
}

CBoundBox::~CBoundBox(void)
{
	//for( int i = 0; i < 8; i++ )
	//{
	//	SAFE_DELETE( m_pTrueVertices[i] );
	//}
	SAFE_DELETE( m_pCollideInfo );
	SAFE_DELETE( m_pPlaneTop );
	SAFE_DELETE( m_pPlaneBottom );
	SAFE_DELETE( m_pPlaneSides[0] );
	SAFE_DELETE( m_pPlaneSides[1] );
	SAFE_DELETE( m_pPlaneSides[2] );
	SAFE_DELETE( m_pPlaneSides[3] );
	//SAFE_RELEASE( tmp_pVertexBuffer );
	SAFE_RELEASE( m_box );
	SAFE_RELEASE( m_boxOrig );
}

void CBoundBox::ResetMat()
{
	D3DXMatrixIdentity( &m_matWorld );
	D3DXMatrixIdentity( &m_matRot );
	D3DXMatrixIdentity( &m_matScall );
	D3DXMatrixIdentity( &m_matTrans );
	D3DXMatrixIdentity( &m_matRender );
}

void CBoundBox::CreateBox( const D3DXVECTOR3 *pVMin, const D3DXVECTOR3 *pVMax )
{
    m_fWidth = pVMax->x - pVMin->x;
	m_fHeight = pVMax->y - pVMin->y;
	m_fDepth = pVMax->z - pVMin->z;
	
	LPD3DXMESH tmpMesh;
	D3DXCreateBox( m_pDevice, m_fWidth, m_fHeight, m_fDepth, &tmpMesh, NULL );
	tmpMesh->CloneMeshFVF( tmpMesh->GetOptions(), D3DFVF_XYZ|D3DFVF_DIFFUSE, m_pDevice, &m_box );
	tmpMesh->CloneMeshFVF( tmpMesh->GetOptions(), D3DFVF_XYZ|D3DFVF_DIFFUSE, m_pDevice, &m_boxOrig );
	tmpMesh->Release();
}

void CBoundBox::ApplyRectify(float fScale)
{
    m_fHeight=m_fHeight*fScale;
}

void CBoundBox::Rotate( const D3DXMATRIX *pMatRot )
{
	m_matRot = *pMatRot;
}
void CBoundBox::Scall( const D3DXMATRIX *pMatScall )
{
	m_matScall = *pMatScall;
}
void CBoundBox::Trans( const D3DXMATRIX *pMatTrans )
{
	m_matTrans = *pMatTrans;
}

void CBoundBox::Update()
{
	//m_matWorld = m_matRot * m_matScall * m_matTrans;
	VERTEX *v, *vOrig;

	m_vCenterPoint.x = 0.f;
	m_vCenterPoint.y = 0.f;
	m_vCenterPoint.z = 0.f;
    D3DXMATRIXA16 matPos;
	D3DXMatrixTranslation(&matPos,0,m_fHeight/2,0);
	D3DXMatrixMultiply(&m_matWorld,&m_matWorld,&matPos);
	D3DXVec3TransformCoord( &m_vCenterPoint, &m_vCenterPoint, &m_matWorld );

	m_boxOrig->LockVertexBuffer( 0, (void**)&vOrig );
	m_box->LockVertexBuffer( 0, (void**)&v );
	static D3DXVECTOR3 vec;
	int i = 0;
	while( i<24 )
	{
		vec.x = vOrig[i].x;
		vec.y = vOrig[i].y /*+ fabs( vOrig[i].y )*/;
		vec.z = vOrig[i].z;
		D3DXVec3TransformCoord( &vec, &vec, &m_matWorld );
		v[i].x = vec.x;
		v[i].y = vec.y;
		v[i].z = vec.z;
		v[i].color = 0xffff00ff;
		m_vertices[i] = vec;
		i++;
	}
	m_box->UnlockVertexBuffer();
	m_boxOrig->UnlockVertexBuffer();
	CalculatePlanes();
	CalculateNormals();
}

void CBoundBox::Render()
{
	m_pDevice->SetTransform( D3DTS_WORLD, &m_matRender );

	m_pDevice->SetTexture( 0, NULL);
	m_pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	m_box->DrawSubset( 0 );
	m_pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);


	//// 以下用于找出盒子的真实顶点，测试用码
	//if(	m_pPlaneTop	)
	//{
	//	VERTEX vs[] = {
	//		{ m_vertices[4].x, m_vertices[4].y, m_vertices[4].z, D3DCOLOR_XRGB( 255,0,0 ) },
	//		{ m_vertices[5].x, m_vertices[5].y, m_vertices[5].z, D3DCOLOR_XRGB( 255,0,0 ) },
	//		{ m_vertices[6].x, m_vertices[6].y, m_vertices[6].z, D3DCOLOR_XRGB( 255,0,0 ) },

	//		{ m_vertices[12].x, m_vertices[12].y, m_vertices[12].z, D3DCOLOR_XRGB( 255,0,0 ) },
	//		{ m_vertices[13].x, m_vertices[13].y, m_vertices[13].z, D3DCOLOR_XRGB( 255,0,0 ) },
	//		{ m_vertices[14].x, m_vertices[14].y, m_vertices[14].z, D3DCOLOR_XRGB( 255,0,0 ) },

	//	};
	//	this->m_pDevice->CreateVertexBuffer(
	//			6*sizeof(VERTEX),
	//			D3DUSAGE_WRITEONLY,
	//			D3DFVF_XYZ|D3DFVF_DIFFUSE,
	//			D3DPOOL_DEFAULT,
	//			&tmp_pVertexBuffer,
	//			NULL);
	//	VERTEX *pV;
	//	tmp_pVertexBuffer->Lock( 0, sizeof(vs), (void**)&pV, 0 );
	//	memcpy( pV, vs, sizeof(vs) );
	//	tmp_pVertexBuffer->Unlock();
	//	this->m_pDevice->SetStreamSource( 0, tmp_pVertexBuffer, 0, sizeof(VERTEX) );
	//	this->m_pDevice->SetFVF( D3DFVF_XYZ|D3DFVF_DIFFUSE );
	//	this->m_pDevice->DrawPrimitive( D3DPT_POINTLIST, 0, 6 );

	//	int a = 10;
	//	VERTEX vs2[] = {
	//		{ m_vertices[a].x, m_vertices[a].y, m_vertices[a].z, D3DCOLOR_XRGB( 255,0,255 ) },

	//	};

	//	this->m_pDevice->CreateVertexBuffer(
	//			sizeof(VERTEX),
	//			D3DUSAGE_WRITEONLY,
	//			D3DFVF_XYZ|D3DFVF_DIFFUSE,
	//			D3DPOOL_DEFAULT,
	//			&tmp_pVertexBuffer,
	//			NULL);
	//	tmp_pVertexBuffer->Lock( 0, sizeof(vs2), (void**)&pV, 0 );
	//	memcpy( pV, vs2, sizeof(vs2) );
	//	tmp_pVertexBuffer->Unlock();
	//	this->m_pDevice->SetStreamSource( 0, tmp_pVertexBuffer, 0, sizeof(VERTEX) );
	//	this->m_pDevice->SetFVF( D3DFVF_XYZ|D3DFVF_DIFFUSE );
	//	this->m_pDevice->DrawPrimitive( D3DPT_POINTLIST, 0, 1 );
	//}

	//// 以下用于找出盒子各表面，测试用码
	//if(	m_pPlaneTop	)
	//{
	//	WORD *pIndex = 0;
	//	WORD ids[36];

	//	m_box->LockIndexBuffer( 0, (void**)&pIndex );
	//	for( int x = 0; x < 36; x++ )
	//	{
	//		ids[x] = pIndex[x];
	//	}
	//	m_box->UnlockIndexBuffer();

	//	for( int j=0; j<6; j++ )
	//	{
	//		int i = j*6;
	//		
	//		cout<<ids[i]<<","<<ids[i+1]<<","<<ids[i+2]<<endl;
	//		VERTEX vs[] =
	//		{
	//			{ m_vertices[ids[i]].x, m_vertices[ids[i]].y, m_vertices[ids[i]].z, D3DCOLOR_XRGB( 255,0,0 ) },
	//			{ m_vertices[ids[i+1]].x, m_vertices[ids[i+1]].y, m_vertices[ids[i+1]].z, D3DCOLOR_XRGB( 0,255,0 ) },
	//			{ m_vertices[ids[i+2]].x, m_vertices[ids[i+2]].y, m_vertices[ids[i+2]].z, D3DCOLOR_XRGB( 0,0,255 ) },
	//		};
	//		this->m_pDevice->CreateVertexBuffer(
	//			3*sizeof(VERTEX),
	//			D3DUSAGE_WRITEONLY,
	//			D3DFVF_XYZ|D3DFVF_DIFFUSE,
	//			D3DPOOL_DEFAULT,
	//			&tmp_pVertexBuffer,
	//			NULL);
	//		VERTEX *pV;
	//		tmp_pVertexBuffer->Lock( 0, sizeof(vs), (void**)&pV, 0 );
	//		memcpy( pV, vs, sizeof(vs) );
	//		tmp_pVertexBuffer->Unlock();
	//		this->m_pDevice->SetStreamSource( 0, tmp_pVertexBuffer, 0, sizeof(VERTEX) );
	//		this->m_pDevice->SetFVF( D3DFVF_XYZ|D3DFVF_DIFFUSE );
	//		this->m_pDevice->DrawPrimitive( D3DPT_TRIANGLELIST, 0, 1 );
	//	}
	//}

	//// 以下用于测试各平面法线，测试用码
	//for( int i = 0; i< 4; i++ )
	//{
	//	int j = 1;
	//	VERTEX vs[] =
	//		{
	//			{ m_vCenterPoint.x, m_vCenterPoint.y, m_vCenterPoint.z, D3DCOLOR_XRGB( 255,0,0 ) },
	//			{ (m_vNormals[i].x+j*m_vCenterPoint.x), (m_vNormals[i].y+j*m_vCenterPoint.y),(m_vNormals[i].z+j*m_vCenterPoint.z), D3DCOLOR_XRGB( 0,255,0 ) }
	//		};
	//		this->m_pDevice->CreateVertexBuffer(
	//			2*sizeof(VERTEX),
	//			D3DUSAGE_WRITEONLY,
	//			D3DFVF_XYZ|D3DFVF_DIFFUSE,
	//			D3DPOOL_DEFAULT,
	//			&tmp_pVertexBuffer,
	//			NULL);
	//		VERTEX *pV;
	//		tmp_pVertexBuffer->Lock( 0, sizeof(vs), (void**)&pV, 0 );
	//		memcpy( pV, vs, sizeof(vs) );
	//		tmp_pVertexBuffer->Unlock();
	//		this->m_pDevice->SetStreamSource( 0, tmp_pVertexBuffer, 0, sizeof(VERTEX) );
	//		this->m_pDevice->SetFVF( D3DFVF_XYZ|D3DFVF_DIFFUSE );
	//		this->m_pDevice->DrawPrimitive( D3DPT_LINELIST, 0, 1 );
	//}
}

void CBoundBox::CalculatePlanes()
{
	D3DXPlaneFromPoints( m_pPlaneTop, &m_vertices[4], &m_vertices[5], &m_vertices[6] );
	D3DXPlaneFromPoints( m_pPlaneBottom, &m_vertices[12], &m_vertices[13], &m_vertices[14] );

	D3DXPlaneFromPoints( m_pPlaneSides[0],&m_vertices[0], &m_vertices[1], &m_vertices[2] );	// 后面
	D3DXPlaneFromPoints( m_pPlaneSides[1],&m_vertices[8], &m_vertices[9], &m_vertices[10] );// 前面
	D3DXPlaneFromPoints( m_pPlaneSides[2],&m_vertices[16], &m_vertices[17], &m_vertices[18] );// 角色左面
	D3DXPlaneFromPoints( m_pPlaneSides[3],&m_vertices[20], &m_vertices[21], &m_vertices[22] );// 角色右面

	// 更新真实顶点
	memcpy( m_pTrueVertices[0], m_vertices[4], sizeof(D3DXVECTOR3) );
	memcpy( m_pTrueVertices[1], m_vertices[5], sizeof(D3DXVECTOR3) );
	memcpy( m_pTrueVertices[2], m_vertices[6], sizeof(D3DXVECTOR3) );
	memcpy( m_pTrueVertices[3], m_vertices[8], sizeof(D3DXVECTOR3) );

	memcpy( m_pTrueVertices[4], m_vertices[12], sizeof(D3DXVECTOR3) );
	memcpy( m_pTrueVertices[5], m_vertices[13], sizeof(D3DXVECTOR3) );
	memcpy( m_pTrueVertices[6], m_vertices[14], sizeof(D3DXVECTOR3) );
	memcpy( m_pTrueVertices[7], m_vertices[10], sizeof(D3DXVECTOR3) );
}

void CBoundBox::CalculateNormals()
{
	m_vNormals[0].x = m_pPlaneSides[0]->a;
	m_vNormals[0].y = m_pPlaneSides[0]->b;
	m_vNormals[0].z = m_pPlaneSides[0]->c;

	m_vNormals[1].x = m_pPlaneSides[1]->a;
	m_vNormals[1].y = m_pPlaneSides[1]->b;
	m_vNormals[1].z = m_pPlaneSides[1]->c;

	m_vNormals[2].x = m_pPlaneSides[2]->a;
	m_vNormals[2].y = m_pPlaneSides[2]->b;
	m_vNormals[2].z = m_pPlaneSides[2]->c;

	m_vNormals[3].x = m_pPlaneSides[3]->a;
	m_vNormals[3].y = m_pPlaneSides[3]->b;
	m_vNormals[3].z = m_pPlaneSides[3]->c;
	//static D3DXVECTOR3 vN,vA,vB,vO;

	//vA = m_vertices[0] - m_vertices[1];
	//vB = m_vertices[0] - m_vertices[2];
	//D3DXVec3Cross( &vN, &vA, &vB );
	//memcpy( &m_vNormals[0], &vN, sizeof(D3DXVECTOR3) );

	//vA = m_vertices[8] - m_vertices[9];
	//vB = m_vertices[8] - m_vertices[10];
	//D3DXVec3Cross( &vN, &vA, &vB );
	//memcpy( &m_vNormals[1], &vN, sizeof(D3DXVECTOR3) );

	//vA = m_vertices[16] - m_vertices[17];
	//vB = m_vertices[16] - m_vertices[18];
	//D3DXVec3Cross( &vN, &vA, &vB );
	//memcpy( &m_vNormals[2], &vN, sizeof(D3DXVECTOR3) );

	//vA = m_vertices[20] - m_vertices[21];
	//vB = m_vertices[20] - m_vertices[22];
	//D3DXVec3Cross( &vN, &vA, &vB );
	//memcpy( &m_vNormals[3], &vN, sizeof(D3DXVECTOR3) );

	//vA = m_vertices[4] - m_vertices[5];
	//vB = m_vertices[4] - m_vertices[6];
	//D3DXVec3Cross( &vN, &vA, &vB );
	//memcpy( &m_vNormals[4], &vN, sizeof(D3DXVECTOR3) );

	//vA = m_vertices[12] - m_vertices[13];
	//vB = m_vertices[12] - m_vertices[14];
	//D3DXVec3Cross( &vN, &vA, &vB );
	//memcpy( &m_vNormals[5], &vN, sizeof(D3DXVECTOR3) );
}

bool CBoundBox::DeterminePointInBox( const D3DXVECTOR3 *pPoint )
{
	float fT = D3DXPlaneDotCoord( m_pPlaneTop, pPoint );
	float fB = D3DXPlaneDotCoord( m_pPlaneBottom, pPoint );
	float fS0 = D3DXPlaneDotCoord( m_pPlaneSides[0], pPoint );
	float fS1 = D3DXPlaneDotCoord( m_pPlaneSides[1], pPoint );
	float fS2 = D3DXPlaneDotCoord( m_pPlaneSides[2], pPoint );
	float fS3 = D3DXPlaneDotCoord( m_pPlaneSides[3], pPoint );


	if( fT <= 0 && fB <= 0 && fS0 <= 0 && fS1 <= 0 && fS2 <= 0 && fS3 <= 0 )
		return true;
	return false;
}

bool CBoundBox::CheckIntersectByRay( D3DXVECTOR3 *pPos, D3DXVECTOR3 *pDir )
{
	static BOOL bHit = false;
	static DWORD dwFaceIndex,dwCountOfHits;
	static float u,v,dist;
	static LPD3DXBUFFER pAllHits;
	HRESULT hr = D3DXIntersect(
		m_box,
		pPos,
		pDir,
		&bHit,
		&dwFaceIndex,
		&u,
		&v,
		&dist,
		&pAllHits,
		&dwCountOfHits);
	return bHit;
}
void CBoundBox::TranWorld(const D3DXMATRIX *pMatWorld)
{
	m_matWorld=*pMatWorld;
}

CollideInfo * CBoundBox::CheckCollide( D3DXVECTOR3 *vSrcPos, CBoundBox *pBoundBox )
{
	m_pCollideInfo->m_bCollided = false;
	m_pCollideInfo->m_pPlane = NULL;

	static float length,currLength;
	static int index;
	
	length = 0.f;
	currLength = 0.f;
	index = 0;

	D3DXVECTOR3 *vSrcPoints = pBoundBox->GetTrueVertices();
	for( int i = 0; i < 8; i++ )
	{
		if( DeterminePointInBox( vSrcPoints ) )
		{
			m_pCollideInfo->m_bCollided = true;
			break;
		}
		vSrcPoints++;
	}

	if( m_pCollideInfo->m_bCollided )
	{
		D3DXVECTOR3 vOut;
		for( int i = 0; i < 4; i++ )
		{
			D3DXPlaneIntersectLine( &vOut, m_pPlaneSides[i], vSrcPos, &m_vCenterPoint );
			currLength = D3DXVec3Length( &( vOut - *vSrcPos) );
			if( i == 0 )
				length = currLength;
			if( currLength < length ){
				length = currLength;
				index = i;
			}
		}
		m_pCollideInfo->m_pPlane = m_pPlaneSides[index];
	}

	return m_pCollideInfo;
}