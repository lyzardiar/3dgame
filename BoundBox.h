// BoundBox.h: interface for the CBoundBox class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BOUNDBOX_H__364346D9_15A2_4C40_8890_F81913D743A6__INCLUDED_)
#define AFX_BOUNDBOX_H__364346D9_15A2_4C40_8890_F81913D743A6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

typedef struct CollideInfo
{
	bool		m_bCollided;	// �Ƿ�����ײ
	LPD3DXPLANE m_pPlane;		// ��ײ��ƽ��
	LPD3DXPLANE m_pPlanes[2];	// ��ײƽ������飨���ڸ�����״��ײ���壬��ײ����ͬʱ������2�����ӵ�2�����棩
}CollideInfo;

class CBoundBox

{
private:
	typedef struct VERTEX
	{
		float x,y,z;
		DWORD color;
	}VERTEX;
	
	LPDIRECT3DDEVICE9  m_pDevice;
	D3DXVECTOR3 m_vertices[24];		// �洢���������
	D3DXVECTOR3 m_pTrueVertices[8];	// ���ӵİ˸���ʵ����
	LPD3DXMESH m_box;
	LPD3DXMESH m_boxOrig;
	D3DXMATRIX m_matWorld,m_matRot,m_matScall,m_matTrans,m_matRender;
	LPD3DXPLANE	m_pPlaneTop;	// �ϱ���
	LPD3DXPLANE	m_pPlaneBottom;	// �±���
	LPD3DXPLANE	m_pPlaneSides[4];	// ����
	
	float	m_fHeight;	// ���Ӹ߶�
	float	m_fWidth;	// ���ӿ��
	float	m_fDepth;	// �������
	
	D3DXVECTOR3 m_vNormals[6];	// ������������ķ���
	
	D3DXVECTOR3 m_vCenterPoint;	// �������ĵ㣨������ײ���ߵĲ�����
	
	LPDIRECT3DVERTEXBUFFER9 tmp_pVertexBuffer;	// �����ö��㻺��
	
	void CalculatePlanes();		// ������Ӹ�����
	void CalculateNormals();	// ������Ӹ�����ķ���
	
	CollideInfo *m_pCollideInfo;
public:
	CBoundBox(LPDIRECT3DDEVICE9 pDevice);
	~CBoundBox(void);
	
	void ResetMat();
	void CreateBox( const D3DXVECTOR3 *pVMin, const D3DXVECTOR3 *pVMax );
	void ApplyRectify(float fScale);
	void Rotate( const D3DXMATRIX *pMatRot );
	void Scall( const D3DXMATRIX *pMatScall );
	void Trans( const D3DXMATRIX *pMatTrans );
	void TranWorld(const D3DXMATRIX *pMatWorld);
	inline LPD3DXMESH GetBox() { return m_box; }
	
	inline D3DXVECTOR3 * GetCenterPoint() { return &m_vCenterPoint; }
	
	bool DeterminePointInBox( const D3DXVECTOR3 *pPoint );	// �жϸ������Ƿ��ں����ڲ�
	inline D3DXVECTOR3 * GetTrueVertices()	{ return &m_pTrueVertices[0]; }
	
	bool CheckIntersectByRay( D3DXVECTOR3 *pPos, D3DXVECTOR3 *pDir );	// �ж������Ƿ񴩹�����
	
	CollideInfo * CheckCollide( D3DXVECTOR3 *vSrcPos, CBoundBox *pBoundBox );
	
	void Update();
	void Render();
};


#endif // !defined(AFX_BOUNDBOX_H__364346D9_15A2_4C40_8890_F81913D743A6__INCLUDED_)
