// SkinMesh.h: interface for the CSkinMesh class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SKINMESH_H__AE3ADAAF_4BE2_4B23_9A1D_9379FA1BAFD9__INCLUDED_)
#define AFX_SKINMESH_H__AE3ADAAF_4BE2_4B23_9A1D_9379FA1BAFD9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <tchar.h>
#include <d3dx9.h>
#include "BoundBox.h"
//-----------------------------------------------------------------------------
// Name: struct D3DXFRAME_DERIVED
// Desc: Structure derived from D3DXFRAME so we can add some app-specific
//       info that will be stored with each frame
//-----------------------------------------------------------------------------
struct D3DXFRAME_DERIVED: public D3DXFRAME
{
    D3DXMATRIXA16        CombinedTransformationMatrix;
};

//-----------------------------------------------------------------------------
// Name: struct D3DXMESHCONTAINER_DERIVED
// Desc: Structure derived from D3DXMESHCONTAINER so we can add some app-specific
//       info that will be stored with each mesh
//-----------------------------------------------------------------------------
struct D3DXMESHCONTAINER_DERIVED: public D3DXMESHCONTAINER
{
    LPDIRECT3DTEXTURE9*  ppTextures;       // array of textures, entries are NULL if no texture specified    
                                
    // SkinMesh info             
    LPD3DXMESH           pOrigMesh;
    LPD3DXATTRIBUTERANGE pAttributeTable;
    DWORD                NumAttributeGroups; 
    DWORD                NumInfl;
    LPD3DXBUFFER         pBoneCombinationBuf;
    D3DXMATRIX**         ppBoneMatrixPtrs;
    D3DXMATRIX*          pBoneOffsetMatrices;
    DWORD                NumPaletteEntries;
    bool                 UseSoftwareVP;
    DWORD                iAttributeSW;     // used to denote the split between SW and HW if necessary for non-indexed skinning
};

// enum for various skinning modes possible
enum METHOD 
{
    D3DNONINDEXED,
    D3DINDEXED,
    SOFTWARE,
    NONE
};
struct D3DXFRAME_EX: public D3DXFRAME
{
    D3DXMATRIXA16        CombinedTransformationMatrix;
};

class CSkinMesh;

//-----------------------------------------------------------------------------
// Name: class CAllocateHierarchy
// Desc: Custom version of ID3DXAllocateHierarchy with custom methods to create
//       frames and meshcontainers.
//-----------------------------------------------------------------------------
class CAllocateHierarchy: public ID3DXAllocateHierarchy
{
public:
    STDMETHOD(CreateFrame)(THIS_ LPCTSTR Name, LPD3DXFRAME *ppNewFrame);
    STDMETHOD(CreateMeshContainer)(THIS_ LPCTSTR Name, LPD3DXMESHDATA pMeshData,
                            LPD3DXMATERIAL pMaterials, LPD3DXEFFECTINSTANCE pEffectInstances, DWORD NumMaterials, 
                            DWORD *pAdjacency, LPD3DXSKININFO pSkinInfo, 
                            LPD3DXMESHCONTAINER *ppNewMeshContainer);
    STDMETHOD(DestroyFrame)(THIS_ LPD3DXFRAME pFrameToFree);
    STDMETHOD(DestroyMeshContainer)(THIS_ LPD3DXMESHCONTAINER pMeshContainerBase);
    CAllocateHierarchy(CSkinMesh *pSkinMesh) :m_pSkinMesh(pSkinMesh) {}

public:
	CSkinMesh*		m_pSkinMesh;
};


class CSkinMesh  
{
public:
	VOID SetAnim(BOOL bAnim);
	VOID SetRotateAngle(FLOAT fAngle);
	VOID UpdateFrameMatrices( LPD3DXFRAME pFrameBase, LPD3DXMATRIX pParentMatrix );
	LPDIRECT3DVERTEXSHADER9 m_pIndexedVertexShader[4];
	D3DXMATRIXA16* m_pBoneMatrices;
	UINT m_NumBoneMatricesMax;
	METHOD m_SkinningMethod;
    HRESULT CalculateBondingBox( LPD3DXFRAME pFrameParent, D3DXVECTOR3 *pVmin, D3DXVECTOR3 *pVmax );

    CBoundBox* m_pBoundBox;
	D3DXMATRIXA16 m_matWorld;
	D3DCAPS9 					m_d3dCaps;
    D3DXVECTOR3                 m_vObjectCenter;    // Center of bounding sphere of object
    D3DXVECTOR3                 m_vMapPos;
    FLOAT                       m_fObjectRadius;    // Radius of bounding sphere of object
	FLOAT                       m_fObjectScale;

	FLOAT						m_fElapsedTime;		// 单帧时间
 
	CBoundBox* GetBoundBox();

	void SetPosition(D3DXVECTOR3 ObjectPos);
    void SetScale(float ObjectScal);
	VOID Render();
	HRESULT LoadFromXFile(char* strFileName);
	HRESULT GenerateSkinnedMesh(D3DXMESHCONTAINER_DERIVED *pMeshContainer);

	VOID ApplyRectify( LPD3DXMATRIX pMatOrig, float fScall );

	VOID SetAnimationName( char *strAnimName );	
	VOID SetAnimationNum(int nAnim);
	FLOAT GetAnimPeriod(int nAnim);

	int GetNumAnimationSets();

	VOID Update(float nLoopTime);	//更新

	bool m_bFoucs;
	bool m_bHightlight;
	CSkinMesh(LPDIRECT3DDEVICE9 pD3DDevice);
	virtual ~CSkinMesh();
protected:
	FLOAT						m_fRotateAngle;
	BOOL						m_bMoving;
	LPDIRECT3DDEVICE9			m_pd3dDevice;
	LPD3DXFRAME					m_pFrameRoot;
	LPD3DXANIMATIONCONTROLLER	m_pAnimController;
 /*   FLOAT						m_fElapsedTime;      // Time elapsed since last frame*/

	D3DXVECTOR3				m_vMin;			
	D3DXVECTOR3				m_vMax;		
	D3DXVECTOR3				m_vOrigMin;
	D3DXVECTOR3				m_vOrigMax;
    D3DXMATRIXA16               m_matView;

	D3DXMATRIX					m_matOrig;	
    float						m_fScall;
    D3DXVECTOR3					m_vCameraBindPos;

	VOID DrawMeshContainer(LPD3DXMESHCONTAINER pMeshContainerBase, LPD3DXFRAME pFrameBase);
	VOID DrawFrame(LPD3DXFRAME pFrame);
	HRESULT SetupBoneMatrixPointersOnMesh( LPD3DXMESHCONTAINER pMeshContainerBase );
	HRESULT SetupBoneMatrixPointers( LPD3DXFRAME pFrame );
};

#endif // !defined(AFX_SKINMESH_H__AE3ADAAF_4BE2_4B23_9A1D_9379FA1BAFD9__INCLUDED_)
