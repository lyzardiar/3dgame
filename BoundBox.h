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
	bool		m_bCollided;	// 是否发生碰撞
	LPD3DXPLANE m_pPlane;		// 碰撞的平面
	LPD3DXPLANE m_pPlanes[2];	// 碰撞平面的数组（对于复杂形状碰撞物体，碰撞可能同时发生于2个盒子的2个表面）
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
	D3DXVECTOR3 m_vertices[24];		// 存储顶点的数组
	D3DXVECTOR3 m_pTrueVertices[8];	// 盒子的八个真实顶点
	LPD3DXMESH m_box;
	LPD3DXMESH m_boxOrig;
	D3DXMATRIX m_matWorld,m_matRot,m_matScall,m_matTrans,m_matRender;
	LPD3DXPLANE	m_pPlaneTop;	// 上表面
	LPD3DXPLANE	m_pPlaneBottom;	// 下表面
	LPD3DXPLANE	m_pPlaneSides[4];	// 侧面
	
	float	m_fHeight;	// 盒子高度
	float	m_fWidth;	// 盒子宽度
	float	m_fDepth;	// 盒子深度
	
	D3DXVECTOR3 m_vNormals[6];	// 盒子六个表面的法线
	
	D3DXVECTOR3 m_vCenterPoint;	// 盒子中心点（用于碰撞射线的产生）
	
	LPDIRECT3DVERTEXBUFFER9 tmp_pVertexBuffer;	// 测试用顶点缓冲
	
	void CalculatePlanes();		// 计算盒子各表面
	void CalculateNormals();	// 计算盒子各表面的法线
	
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
	
	bool DeterminePointInBox( const D3DXVECTOR3 *pPoint );	// 判断给定点是否在盒子内部
	inline D3DXVECTOR3 * GetTrueVertices()	{ return &m_pTrueVertices[0]; }
	
	bool CheckIntersectByRay( D3DXVECTOR3 *pPos, D3DXVECTOR3 *pDir );	// 判断射线是否穿过盒子
	
	CollideInfo * CheckCollide( D3DXVECTOR3 *vSrcPos, CBoundBox *pBoundBox );
	
	void Update();
	void Render();
};


#endif // !defined(AFX_BOUNDBOX_H__364346D9_15A2_4C40_8890_F81913D743A6__INCLUDED_)
