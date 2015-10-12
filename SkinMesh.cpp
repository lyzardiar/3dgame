// SkinMesh.cpp: implementation of the CSkinMesh class.
//
//////////////////////////////////////////////////////////////////////
#include "StdAfx.h"
#include "SkinMesh.h"
#include <stdio.h>

//-----------------------------------------------------------------------------
// Name: AllocateName()
// Desc: Allocates memory for a string to hold the name of a frame or mesh
//-----------------------------------------------------------------------------
HRESULT AllocateName( LPCTSTR Name, LPTSTR *pNewName )
{
    UINT cbLength;
	
    if (Name != NULL)
    {
        cbLength = lstrlen(Name) + 1;
        *pNewName = new TCHAR[cbLength];
        if (*pNewName == NULL)
            return E_OUTOFMEMORY;
        memcpy(*pNewName, Name, cbLength*sizeof(TCHAR));
    }
    else
    {
        *pNewName = NULL;
    }
	
    return S_OK;
}

//-----------------------------------------------------------------------------
// Name: CAllocateHierarchy::CreateFrame()
// Desc: 
//-----------------------------------------------------------------------------
HRESULT CAllocateHierarchy::CreateFrame(LPCTSTR Name, LPD3DXFRAME *ppNewFrame)
{
    HRESULT hr = S_OK;
    D3DXFRAME_DERIVED *pFrame;
	
    *ppNewFrame = NULL;
	
    pFrame = new D3DXFRAME_DERIVED;
    if (pFrame == NULL)
    {
        hr = E_OUTOFMEMORY;
        goto e_Exit;
    }
	
    hr = AllocateName(Name, &pFrame->Name);
    if (FAILED(hr))
        goto e_Exit;
	
    // initialize other data members of the frame
    D3DXMatrixIdentity(&pFrame->TransformationMatrix);
    D3DXMatrixIdentity(&pFrame->CombinedTransformationMatrix);
	
    pFrame->pMeshContainer = NULL;
    pFrame->pFrameSibling = NULL;
    pFrame->pFrameFirstChild = NULL;
	
    *ppNewFrame = pFrame;
    pFrame = NULL;
e_Exit:
    delete pFrame;
    return hr;
}




//-----------------------------------------------------------------------------
// Name: CAllocateHierarchy::CreateMeshContainer()
// Desc: 
//-----------------------------------------------------------------------------
HRESULT CAllocateHierarchy::CreateMeshContainer(LPCTSTR Name, LPD3DXMESHDATA pMeshData,
												LPD3DXMATERIAL pMaterials, LPD3DXEFFECTINSTANCE pEffectInstances, DWORD NumMaterials, 
												DWORD *pAdjacency, LPD3DXSKININFO pSkinInfo, 
												LPD3DXMESHCONTAINER *ppNewMeshContainer) 
{
    HRESULT hr;
    D3DXMESHCONTAINER_DERIVED *pMeshContainer = NULL;
    UINT NumFaces;
    UINT iMaterial;
    UINT iBone, cBones;
    LPDIRECT3DDEVICE9 pd3dDevice = NULL;
	
    LPD3DXMESH pMesh = NULL;
	
    *ppNewMeshContainer = NULL;
	
    // this sample does not handle patch meshes, so fail when one is found
    if (pMeshData->Type != D3DXMESHTYPE_MESH)
    {
        hr = E_FAIL;
        goto e_Exit;
    }
	
    // get the pMesh interface pointer out of the mesh data structure
    pMesh = pMeshData->pMesh;
	
    // this sample does not FVF compatible meshes, so fail when one is found
    if (pMesh->GetFVF() == 0)
    {
        hr = E_FAIL;
        goto e_Exit;
    }
	
    // allocate the overloaded structure to return as a D3DXMESHCONTAINER
    pMeshContainer = new D3DXMESHCONTAINER_DERIVED;
    if (pMeshContainer == NULL)
    {
        hr = E_OUTOFMEMORY;
        goto e_Exit;
    }
    memset(pMeshContainer, 0, sizeof(D3DXMESHCONTAINER_DERIVED));
	
    // make sure and copy the name.  All memory as input belongs to caller, interfaces can be addref'd though
    hr = AllocateName(Name, &pMeshContainer->Name);
    if (FAILED(hr))
        goto e_Exit;        
	
    pMesh->GetDevice(&pd3dDevice);
    NumFaces = pMesh->GetNumFaces();//返回三角形个数
	
    // if no normals are in the mesh, add them
    if (!(pMesh->GetFVF() & D3DFVF_NORMAL))
    {
        pMeshContainer->MeshData.Type = D3DXMESHTYPE_MESH;
		
        // clone the mesh to make room for the normals
        hr = pMesh->CloneMeshFVF( pMesh->GetOptions(), 
			pMesh->GetFVF() | D3DFVF_NORMAL, 
			pd3dDevice, &pMeshContainer->MeshData.pMesh );
        if (FAILED(hr))
            goto e_Exit;
		
        // get the new pMesh pointer back out of the mesh container to use
        // NOTE: we do not release pMesh because we do not have a reference to it yet
        pMesh = pMeshContainer->MeshData.pMesh;
		
        // now generate the normals for the pmesh
        D3DXComputeNormals( pMesh, NULL );
    }
    else  // if no normals, just add a reference to the mesh for the mesh container
    {
        pMeshContainer->MeshData.pMesh = pMesh;
        pMeshContainer->MeshData.Type = D3DXMESHTYPE_MESH;
		
        pMesh->AddRef();
    }
	
    // allocate memory to contain the material information.  This sample uses
    //   the D3D9 materials and texture names instead of the EffectInstance style materials
    pMeshContainer->NumMaterials = max(1, NumMaterials);
    pMeshContainer->pMaterials = new D3DXMATERIAL[pMeshContainer->NumMaterials];
    pMeshContainer->ppTextures = new LPDIRECT3DTEXTURE9[pMeshContainer->NumMaterials];
    pMeshContainer->pAdjacency = new DWORD[NumFaces*3];
    if ((pMeshContainer->pAdjacency == NULL) || (pMeshContainer->pMaterials == NULL))
    {
        hr = E_OUTOFMEMORY;
        goto e_Exit;
    }
	
    memcpy(pMeshContainer->pAdjacency, pAdjacency, sizeof(DWORD) * NumFaces*3);
    memset(pMeshContainer->ppTextures, 0, sizeof(LPDIRECT3DTEXTURE9) * pMeshContainer->NumMaterials);
	
    // if materials provided, copy them
    if (NumMaterials > 0)            
    {
        memcpy(pMeshContainer->pMaterials, pMaterials, sizeof(D3DXMATERIAL) * NumMaterials);
		
        for (iMaterial = 0; iMaterial < NumMaterials; iMaterial++)
        {
            if (pMeshContainer->pMaterials[iMaterial].pTextureFilename != NULL)
            {
                TCHAR strTexturePath[MAX_PATH] = _T("");
                DXUtil_FindMediaFileCb( strTexturePath, sizeof(strTexturePath), pMeshContainer->pMaterials[iMaterial].pTextureFilename );
                if( FAILED( D3DXCreateTextureFromFile( pd3dDevice, strTexturePath, 
					&pMeshContainer->ppTextures[iMaterial] ) ) )
                    pMeshContainer->ppTextures[iMaterial] = NULL;
				
				
                // don't remember a pointer into the dynamic memory, just forget the name after loading
                pMeshContainer->pMaterials[iMaterial].pTextureFilename = NULL;
            }
        }
    }
    else // if no materials provided, use a default one
    {
        pMeshContainer->pMaterials[0].pTextureFilename = NULL;
        memset(&pMeshContainer->pMaterials[0].MatD3D, 0, sizeof(D3DMATERIAL9));
        pMeshContainer->pMaterials[0].MatD3D.Diffuse.r = 0.5f;
        pMeshContainer->pMaterials[0].MatD3D.Diffuse.g = 0.5f;
        pMeshContainer->pMaterials[0].MatD3D.Diffuse.b = 0.5f;
        pMeshContainer->pMaterials[0].MatD3D.Specular = pMeshContainer->pMaterials[0].MatD3D.Diffuse;
    }
	
    // if there is skinning information, save off the required data and then setup for HW skinning
    if (pSkinInfo != NULL)
    {
        // first save off the SkinInfo and original mesh data
        pMeshContainer->pSkinInfo = pSkinInfo;
        pSkinInfo->AddRef();
		
        pMeshContainer->pOrigMesh = pMesh;
        pMesh->AddRef();
		
        // Will need an array of offset matrices to move the vertices from the figure space to the bone's space
        cBones = pSkinInfo->GetNumBones();
        pMeshContainer->pBoneOffsetMatrices = new D3DXMATRIX[cBones];
        if (pMeshContainer->pBoneOffsetMatrices == NULL)
        {
            hr = E_OUTOFMEMORY;
            goto e_Exit;
        }
		
        // get each of the bone offset matrices so that we don't need to get them later
        for (iBone = 0; iBone < cBones; iBone++)
        {
            pMeshContainer->pBoneOffsetMatrices[iBone] = *(pMeshContainer->pSkinInfo->GetBoneOffsetMatrix(iBone));
        }
		
        // GenerateSkinnedMesh will take the general skinning information and transform it to a HW friendly version
        hr = m_pSkinMesh->GenerateSkinnedMesh(pMeshContainer);
        if (FAILED(hr))
            goto e_Exit;
    }
	
    *ppNewMeshContainer = pMeshContainer;
    pMeshContainer = NULL;
e_Exit:
    SAFE_RELEASE(pd3dDevice);
	
    // call Destroy function to properly clean up the memory allocated 
    if (pMeshContainer != NULL)
    {
        DestroyMeshContainer(pMeshContainer);
    }
	
    return hr;
}




//-----------------------------------------------------------------------------
// Name: CAllocateHierarchy::DestroyFrame()
// Desc: 
//-----------------------------------------------------------------------------
HRESULT CAllocateHierarchy::DestroyFrame(LPD3DXFRAME pFrameToFree) 
{
    SAFE_DELETE_ARRAY( pFrameToFree->Name );
    SAFE_DELETE( pFrameToFree );
    return S_OK; 
}




//-----------------------------------------------------------------------------
// Name: CAllocateHierarchy::DestroyMeshContainer()
// Desc: 
//-----------------------------------------------------------------------------
HRESULT CAllocateHierarchy::DestroyMeshContainer(LPD3DXMESHCONTAINER pMeshContainerBase)
{
    UINT iMaterial;
    D3DXMESHCONTAINER_DERIVED *pMeshContainer = (D3DXMESHCONTAINER_DERIVED*)pMeshContainerBase;
	
    SAFE_DELETE_ARRAY( pMeshContainer->Name );
    SAFE_DELETE_ARRAY( pMeshContainer->pAdjacency );
    SAFE_DELETE_ARRAY( pMeshContainer->pMaterials );
    SAFE_DELETE_ARRAY( pMeshContainer->pBoneOffsetMatrices );
	
    // release all the allocated textures
    if (pMeshContainer->ppTextures != NULL)
    {
        for (iMaterial = 0; iMaterial < pMeshContainer->NumMaterials; iMaterial++)
        {
            SAFE_RELEASE( pMeshContainer->ppTextures[iMaterial] );
        }
    }
	
    SAFE_DELETE_ARRAY( pMeshContainer->ppTextures );
    SAFE_DELETE_ARRAY( pMeshContainer->ppBoneMatrixPtrs );
    SAFE_RELEASE( pMeshContainer->pBoneCombinationBuf );
    SAFE_RELEASE( pMeshContainer->MeshData.pMesh );
    SAFE_RELEASE( pMeshContainer->pSkinInfo );
    SAFE_RELEASE( pMeshContainer->pOrigMesh );
    SAFE_DELETE( pMeshContainer );
    return S_OK;
}



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSkinMesh::CSkinMesh(LPDIRECT3DDEVICE9 pD3DDevice)
{
	m_pd3dDevice=pD3DDevice;
	m_fElapsedTime=0.0f;
	m_pd3dDevice->GetDeviceCaps( &m_d3dCaps );

	m_bMoving=TRUE;
    m_pAnimController = NULL;
    m_pFrameRoot = NULL;
    m_SkinningMethod = D3DINDEXED;
    m_pBoneMatrices = NULL;
    m_NumBoneMatricesMax = 0;
	m_fRotateAngle=0;
	m_fObjectScale=1.0;

	m_bFoucs=false;
	m_vOrigMin=	D3DXVECTOR3(0,0,0);
	m_vOrigMax=D3DXVECTOR3(0,0,0);
	m_pBoundBox=new CBoundBox(pD3DDevice);
	m_bHightlight=false;
}

CSkinMesh::~CSkinMesh()
{
	CAllocateHierarchy Alloc(this);
	D3DXFrameDestroy(m_pFrameRoot, &Alloc);
	
    SAFE_RELEASE(m_pAnimController);
	SAFE_DELETE(m_pBoundBox);
	
}

HRESULT CSkinMesh::LoadFromXFile(char *strFileName)
{
    TCHAR      strMeshPath[MAX_PATH];
    HRESULT    hr;
	CAllocateHierarchy Alloc(this);
	
    // Load the mesh from the specified file
    hr = DXUtil_FindMediaFileCb( strMeshPath, sizeof(strMeshPath), strFileName );
    if (FAILED(hr))
        return hr;
	
    hr = D3DXLoadMeshHierarchyFromX(strMeshPath, D3DXMESH_MANAGED, m_pd3dDevice, &Alloc, NULL, &m_pFrameRoot, &m_pAnimController);
    if (FAILED(hr))
        return hr;
	
    hr = SetupBoneMatrixPointers(m_pFrameRoot);
    if (FAILED(hr))
        return hr;
	
    hr = D3DXFrameCalculateBoundingSphere(m_pFrameRoot, &m_vObjectCenter, &m_fObjectRadius);
    if (FAILED(hr))
        return hr;

	
	if(m_pFrameRoot)	// 创建包围盒，m_vMin最小点，m_vMax最大点
		CalculateBondingBox(m_pFrameRoot, &m_vOrigMin, &m_vOrigMax );

	return S_OK;
}

//-----------------------------------------------------------------------------
// Name: GenerateSkinnedMesh()
// Desc: Called either by CreateMeshContainer when loading a skin mesh, or when 
//       changing methods.  This function uses the pSkinInfo of the mesh 
//       container to generate the desired drawable mesh and bone combination 
//       table.
//-----------------------------------------------------------------------------
HRESULT CSkinMesh::GenerateSkinnedMesh(D3DXMESHCONTAINER_DERIVED *pMeshContainer)
{
    HRESULT hr = S_OK;

    if (pMeshContainer->pSkinInfo == NULL)
        return hr;

    SAFE_RELEASE( pMeshContainer->MeshData.pMesh );
    SAFE_RELEASE( pMeshContainer->pBoneCombinationBuf );

    // if non-indexed skinning mode selected, use ConvertToBlendedMesh to generate drawable mesh
    if (m_SkinningMethod == D3DNONINDEXED)
    {

        hr = pMeshContainer->pSkinInfo->ConvertToBlendedMesh
                                   (
                                       pMeshContainer->pOrigMesh,
                                       D3DXMESH_MANAGED|D3DXMESHOPT_VERTEXCACHE, 
                                       pMeshContainer->pAdjacency, 
                                       NULL, NULL, NULL, 
                                       &pMeshContainer->NumInfl,
                                       &pMeshContainer->NumAttributeGroups, 
                                       &pMeshContainer->pBoneCombinationBuf, 
                                       &pMeshContainer->MeshData.pMesh
                                   );
        if (FAILED(hr))
            goto e_Exit;


        /* If the device can only do 2 matrix blends, ConvertToBlendedMesh cannot approximate all meshes to it
           Thus we split the mesh in two parts: The part that uses at most 2 matrices and the rest. The first is
           drawn using the device's HW vertex processing and the rest is drawn using SW vertex processing. */
        LPD3DXBONECOMBINATION rgBoneCombinations  = reinterpret_cast<LPD3DXBONECOMBINATION>(pMeshContainer->pBoneCombinationBuf->GetBufferPointer());

        // look for any set of bone combinations that do not fit the caps
        for (pMeshContainer->iAttributeSW = 0; pMeshContainer->iAttributeSW < pMeshContainer->NumAttributeGroups; pMeshContainer->iAttributeSW++)
        {
            DWORD cInfl   = 0;

            for (DWORD iInfl = 0; iInfl < pMeshContainer->NumInfl; iInfl++)
            {
                if (rgBoneCombinations[pMeshContainer->iAttributeSW].BoneId[iInfl] != UINT_MAX)
                {
                    ++cInfl;
                }
            }

            if (cInfl > m_d3dCaps.MaxVertexBlendMatrices)
            {
                break;
            }
        }

        // if there is both HW and SW, add the Software Processing flag
        if (pMeshContainer->iAttributeSW < pMeshContainer->NumAttributeGroups)
        {
            LPD3DXMESH pMeshTmp;

            hr = pMeshContainer->MeshData.pMesh->CloneMeshFVF(D3DXMESH_SOFTWAREPROCESSING|pMeshContainer->MeshData.pMesh->GetOptions(), 
                                                pMeshContainer->MeshData.pMesh->GetFVF(),
                                                m_pd3dDevice, &pMeshTmp);
            if (FAILED(hr))
            {
                goto e_Exit;
            }

            pMeshContainer->MeshData.pMesh->Release();
            pMeshContainer->MeshData.pMesh = pMeshTmp;
            pMeshTmp = NULL;
        }
    }
    // if indexed skinning mode selected, use ConvertToIndexedsBlendedMesh to generate drawable mesh
    else if (m_SkinningMethod == D3DINDEXED)
    {
        DWORD NumMaxFaceInfl;
        DWORD Flags = D3DXMESHOPT_VERTEXCACHE;

        LPDIRECT3DINDEXBUFFER9 pIB;
        hr = pMeshContainer->pOrigMesh->GetIndexBuffer(&pIB);
        if (FAILED(hr))
            goto e_Exit;

        hr = pMeshContainer->pSkinInfo->GetMaxFaceInfluences(pIB, pMeshContainer->pOrigMesh->GetNumFaces(), &NumMaxFaceInfl);
        pIB->Release();
        if (FAILED(hr))
            goto e_Exit;

        // 12 entry palette guarantees that any triangle (4 independent influences per vertex of a tri)
        // can be handled
        NumMaxFaceInfl = min(NumMaxFaceInfl, 12);

        if (m_d3dCaps.MaxVertexBlendMatrixIndex + 1 < NumMaxFaceInfl)
        {
            // HW does not support indexed vertex blending. Use SW instead
            pMeshContainer->NumPaletteEntries = min(256, pMeshContainer->pSkinInfo->GetNumBones());
            pMeshContainer->UseSoftwareVP = true;
            Flags |= D3DXMESH_SYSTEMMEM;
        }
        else
        {
            // using hardware - determine palette size from caps and number of bones
            // If normals are present in the vertex data that needs to be blended for lighting, then 
            // the number of matrices is half the number specified by MaxVertexBlendMatrixIndex.
            pMeshContainer->NumPaletteEntries = min( ( m_d3dCaps.MaxVertexBlendMatrixIndex + 1 ) / 2, 
                                                     pMeshContainer->pSkinInfo->GetNumBones() );
            pMeshContainer->UseSoftwareVP = false;
            Flags |= D3DXMESH_MANAGED;
        }

        hr = pMeshContainer->pSkinInfo->ConvertToIndexedBlendedMesh
                                                (
                                                pMeshContainer->pOrigMesh,
                                                Flags, 
                                                pMeshContainer->NumPaletteEntries, 
                                                pMeshContainer->pAdjacency, 
                                                NULL, NULL, NULL, 
                                                &pMeshContainer->NumInfl,
                                                &pMeshContainer->NumAttributeGroups, 
                                                &pMeshContainer->pBoneCombinationBuf, 
                                                &pMeshContainer->MeshData.pMesh);
        if (FAILED(hr))
            goto e_Exit;
    }
     // if software skinning selected, use GenerateSkinnedMesh to create a mesh that can be used with UpdateSkinnedMesh
    else if (m_SkinningMethod == SOFTWARE)
    {
        hr = pMeshContainer->pOrigMesh->CloneMeshFVF(D3DXMESH_MANAGED, pMeshContainer->pOrigMesh->GetFVF(),
                                              m_pd3dDevice, &pMeshContainer->MeshData.pMesh);
        if (FAILED(hr))
            goto e_Exit;

        hr = pMeshContainer->MeshData.pMesh->GetAttributeTable(NULL, &pMeshContainer->NumAttributeGroups);
        if (FAILED(hr))
            goto e_Exit;

        delete[] pMeshContainer->pAttributeTable;
        pMeshContainer->pAttributeTable  = new D3DXATTRIBUTERANGE[pMeshContainer->NumAttributeGroups];
        if (pMeshContainer->pAttributeTable == NULL)
        {
            hr = E_OUTOFMEMORY;
            goto e_Exit;
        }

        hr = pMeshContainer->MeshData.pMesh->GetAttributeTable(pMeshContainer->pAttributeTable, NULL);//获得属性表
        if (FAILED(hr))
            goto e_Exit;

        // allocate a buffer for bone matrices, but only if another mesh has not allocated one of the same size or larger
        if (m_NumBoneMatricesMax < pMeshContainer->pSkinInfo->GetNumBones())
        {
            m_NumBoneMatricesMax = pMeshContainer->pSkinInfo->GetNumBones();

            // Allocate space for blend matrices
            delete []m_pBoneMatrices; 
            m_pBoneMatrices  = new D3DXMATRIXA16[m_NumBoneMatricesMax];
            if (m_pBoneMatrices == NULL)
            {
                hr = E_OUTOFMEMORY;
                goto e_Exit;
            }
        }
    }
    else  // invalid m_SkinningMethod value
    {        
        // return failure due to invalid skinning method value
        hr = E_INVALIDARG;
        goto e_Exit;
    }

e_Exit:
    return hr;
}



HRESULT CSkinMesh::SetupBoneMatrixPointers(LPD3DXFRAME pFrame)
{
    HRESULT hr;

    if (pFrame->pMeshContainer != NULL)
    {
        hr = SetupBoneMatrixPointersOnMesh(pFrame->pMeshContainer);
        if (FAILED(hr))
            return hr;
    }

    if (pFrame->pFrameSibling != NULL)
    {
        hr = SetupBoneMatrixPointers(pFrame->pFrameSibling);
        if (FAILED(hr))
            return hr;
    }

    if (pFrame->pFrameFirstChild != NULL)
    {
        hr = SetupBoneMatrixPointers(pFrame->pFrameFirstChild);
        if (FAILED(hr))
            return hr;
    }

    return S_OK;
}


HRESULT CSkinMesh::SetupBoneMatrixPointersOnMesh(LPD3DXMESHCONTAINER pMeshContainerBase)
{
    UINT iBone, cBones;
    D3DXFRAME_DERIVED *pFrame;

    D3DXMESHCONTAINER_DERIVED *pMeshContainer = (D3DXMESHCONTAINER_DERIVED*)pMeshContainerBase;

    // if there is a skinmesh, then setup the bone matrices
    if (pMeshContainer->pSkinInfo != NULL)
    {
        cBones = pMeshContainer->pSkinInfo->GetNumBones();

        pMeshContainer->ppBoneMatrixPtrs = new D3DXMATRIX*[cBones];
        if (pMeshContainer->ppBoneMatrixPtrs == NULL)
            return E_OUTOFMEMORY;

        for (iBone = 0; iBone < cBones; iBone++)
        {
            pFrame = (D3DXFRAME_DERIVED*)D3DXFrameFind(m_pFrameRoot, pMeshContainer->pSkinInfo->GetBoneName(iBone));
            if (pFrame == NULL)
                return E_FAIL;

            pMeshContainer->ppBoneMatrixPtrs[iBone] = &pFrame->CombinedTransformationMatrix;
        }
    }

    return S_OK;
}
VOID CSkinMesh::Update(float nLoopTime)
{	
	if (nLoopTime==0)
	{
		return;
	}

// 	D3DXMatrixScaling(&matScale2, m_fObjectScale, m_fObjectScale, m_fObjectScale);
// 	D3DXMatrixMultiply(&matWorld,&matWorld,&matScale2);

	
	if(m_pAnimController)
	{
#if ((D3D_SDK_VERSION & 0xFF)==31)	// Directx9.0b
		if (m_bMoving)
			m_pAnimController->SetTime( m_pAnimController->GetTime()+nLoopTime );
		else
			m_pAnimController->SetTime( 0 );
		
#else // Directx9.0c
		if (m_bMoving){
			m_pAnimController->AdvanceTime( nLoopTime+m_pAnimController->GetTime(), NULL ); 
		}
		else
			m_pAnimController->ResetTime();
#endif
	}


}

VOID CSkinMesh::Render()
{	
	D3DXMATRIXA16 matWorld,matWorld2,matScale2;
  
	D3DXMatrixScaling(&matScale2, m_fObjectScale, m_fObjectScale, m_fObjectScale);
	D3DXMatrixRotationY(&matWorld,m_fRotateAngle);
	
	D3DXMatrixMultiply(&matWorld,&matWorld,&matScale2);
	D3DXMatrixTranslation(&matWorld2,-m_vObjectCenter.x+m_vMapPos.x,-m_vObjectCenter.y+m_vMapPos.y,-m_vObjectCenter.z+m_vMapPos.z);
	D3DXMatrixMultiply(&matWorld,&matWorld,&matWorld2);
	
	D3DXMatrixTranslation(&matWorld2,0,m_fObjectRadius,0);
	D3DXMatrixMultiply(&matWorld,&matWorld,&matWorld2);
	
	m_pd3dDevice->SetTransform( D3DTS_WORLD, &matWorld );
	
	m_pd3dDevice->SetRenderState(D3DRS_LIGHTING, false);
	
	UpdateFrameMatrices(m_pFrameRoot, &matWorld);
	
	
	m_pBoundBox->TranWorld(&matWorld);
	m_pBoundBox->Update();
	DrawFrame(m_pFrameRoot);
	if (m_bFoucs)
	{
		m_pBoundBox->Render();
	}
}



VOID CSkinMesh::DrawFrame(LPD3DXFRAME pFrame)
{
    LPD3DXMESHCONTAINER pMeshContainer;
	
    pMeshContainer = pFrame->pMeshContainer;
    while (pMeshContainer != NULL)
    {
        DrawMeshContainer(pMeshContainer, pFrame);
		
        pMeshContainer = pMeshContainer->pNextMeshContainer;
    }
	
    if (pFrame->pFrameSibling != NULL)
    {
        DrawFrame(pFrame->pFrameSibling);
    }
	
    if (pFrame->pFrameFirstChild != NULL)
    {
        DrawFrame(pFrame->pFrameFirstChild);
    }
}

//-----------------------------------------------------------------------------
// Name: DrawMeshContainer()
// Desc: Called to render a mesh in the hierarchy
//-----------------------------------------------------------------------------
VOID CSkinMesh::DrawMeshContainer(LPD3DXMESHCONTAINER pMeshContainerBase, LPD3DXFRAME pFrameBase)
{
//	m_pd3dDevice->SetRenderState( D3DRS_LIGHTING,         TRUE);
    D3DXMESHCONTAINER_DERIVED *pMeshContainer = (D3DXMESHCONTAINER_DERIVED*)pMeshContainerBase;
    D3DXFRAME_DERIVED *pFrame = (D3DXFRAME_DERIVED*)pFrameBase;
    UINT iMaterial;
    UINT NumBlend;
    UINT iAttrib;
    DWORD AttribIdPrev;
    LPD3DXBONECOMBINATION pBoneComb;

    UINT iMatrixIndex;
    UINT iPaletteEntry;
    D3DXMATRIXA16 matTemp;

    // first check for skinning
    if (pMeshContainer->pSkinInfo != NULL)
    {
        if (m_SkinningMethod == D3DNONINDEXED)
        {
            AttribIdPrev = UNUSED32; 
            pBoneComb = reinterpret_cast<LPD3DXBONECOMBINATION>(pMeshContainer->pBoneCombinationBuf->GetBufferPointer());

            // Draw using default vtx processing of the device (typically HW)
            for (iAttrib = 0; iAttrib < pMeshContainer->NumAttributeGroups; iAttrib++)
            {
                NumBlend = 0;
                for (DWORD i = 0; i < pMeshContainer->NumInfl; ++i)
                {
                    if (pBoneComb[iAttrib].BoneId[i] != UINT_MAX)
                    {
                        NumBlend = i;
                    }
                }

                if (m_d3dCaps.MaxVertexBlendMatrices >= NumBlend + 1)
                {
                    // first calculate the world matrices for the current set of blend weights and get the accurate count of the number of blends
                    for (DWORD i = 0; i < pMeshContainer->NumInfl; ++i)
                    {
                        iMatrixIndex = pBoneComb[iAttrib].BoneId[i];
                        if (iMatrixIndex != UINT_MAX)
                        {
                            D3DXMatrixMultiply( &matTemp, &pMeshContainer->pBoneOffsetMatrices[iMatrixIndex], pMeshContainer->ppBoneMatrixPtrs[iMatrixIndex] );
                            m_pd3dDevice->SetTransform( D3DTS_WORLDMATRIX( i ), &matTemp );
                        }
                    }

                    m_pd3dDevice->SetRenderState(D3DRS_VERTEXBLEND, NumBlend);

                    // lookup the material used for this subset of faces
                    if ((AttribIdPrev != pBoneComb[iAttrib].AttribId) || (AttribIdPrev == UNUSED32))
                    {
                        m_pd3dDevice->SetMaterial( &pMeshContainer->pMaterials[pBoneComb[iAttrib].AttribId].MatD3D );
                        m_pd3dDevice->SetTexture( 0, pMeshContainer->ppTextures[pBoneComb[iAttrib].AttribId] );
                        AttribIdPrev = pBoneComb[iAttrib].AttribId;
                    }

                    // draw the subset now that the correct material and matrices are loaded
                    pMeshContainer->MeshData.pMesh->DrawSubset(iAttrib);
                }
            }

            // If necessary, draw parts that HW could not handle using SW
            if (pMeshContainer->iAttributeSW < pMeshContainer->NumAttributeGroups)
            {
                AttribIdPrev = UNUSED32; 
                m_pd3dDevice->SetSoftwareVertexProcessing(TRUE);
                for (iAttrib = pMeshContainer->iAttributeSW; iAttrib < pMeshContainer->NumAttributeGroups; iAttrib++)
                {
                    NumBlend = 0;
                    for (DWORD i = 0; i < pMeshContainer->NumInfl; ++i)
                    {
                        if (pBoneComb[iAttrib].BoneId[i] != UINT_MAX)
                        {
                            NumBlend = i;
                        }
                    }

                    if (m_d3dCaps.MaxVertexBlendMatrices < NumBlend + 1)
                    {
                        // first calculate the world matrices for the current set of blend weights and get the accurate count of the number of blends
                        for (DWORD i = 0; i < pMeshContainer->NumInfl; ++i)
                        {
                            iMatrixIndex = pBoneComb[iAttrib].BoneId[i];
                            if (iMatrixIndex != UINT_MAX)
                            {
                                D3DXMatrixMultiply( &matTemp, &pMeshContainer->pBoneOffsetMatrices[iMatrixIndex], pMeshContainer->ppBoneMatrixPtrs[iMatrixIndex] );
                                m_pd3dDevice->SetTransform( D3DTS_WORLDMATRIX( i ), &matTemp );
                            }
                        }

                        m_pd3dDevice->SetRenderState(D3DRS_VERTEXBLEND, NumBlend);

                        // lookup the material used for this subset of faces
                        if ((AttribIdPrev != pBoneComb[iAttrib].AttribId) || (AttribIdPrev == UNUSED32))
                        {
                            m_pd3dDevice->SetMaterial( &pMeshContainer->pMaterials[pBoneComb[iAttrib].AttribId].MatD3D );
                            m_pd3dDevice->SetTexture( 0, pMeshContainer->ppTextures[pBoneComb[iAttrib].AttribId] );
                            AttribIdPrev = pBoneComb[iAttrib].AttribId;
                        }

                        // draw the subset now that the correct material and matrices are loaded
                        pMeshContainer->MeshData.pMesh->DrawSubset(iAttrib);
                    }
                }
                m_pd3dDevice->SetSoftwareVertexProcessing(FALSE);
            }

            m_pd3dDevice->SetRenderState(D3DRS_VERTEXBLEND, 0);
        }
        else if (m_SkinningMethod == D3DINDEXED)
        {
            // if hw doesn't support indexed vertex processing, switch to software vertex processing
            if (pMeshContainer->UseSoftwareVP)
            {
                m_pd3dDevice->SetSoftwareVertexProcessing(TRUE);
            }

            // set the number of vertex blend indices to be blended
            if (pMeshContainer->NumInfl == 1)
                m_pd3dDevice->SetRenderState(D3DRS_VERTEXBLEND, D3DVBF_0WEIGHTS);
            else
                m_pd3dDevice->SetRenderState(D3DRS_VERTEXBLEND, pMeshContainer->NumInfl - 1);

            if (pMeshContainer->NumInfl)
                m_pd3dDevice->SetRenderState(D3DRS_INDEXEDVERTEXBLENDENABLE, TRUE);

            // for each attribute group in the mesh, calculate the set of matrices in the palette and then draw the mesh subset
            pBoneComb = reinterpret_cast<LPD3DXBONECOMBINATION>(pMeshContainer->pBoneCombinationBuf->GetBufferPointer());
            for (iAttrib = 0; iAttrib < pMeshContainer->NumAttributeGroups; iAttrib++)
            {
                // first calculate all the world matrices
                for (iPaletteEntry = 0; iPaletteEntry < pMeshContainer->NumPaletteEntries; ++iPaletteEntry)
                {
                    iMatrixIndex = pBoneComb[iAttrib].BoneId[iPaletteEntry];
                    if (iMatrixIndex != UINT_MAX)
                    {
                        D3DXMatrixMultiply( &matTemp, &pMeshContainer->pBoneOffsetMatrices[iMatrixIndex], pMeshContainer->ppBoneMatrixPtrs[iMatrixIndex] );
                        m_pd3dDevice->SetTransform( D3DTS_WORLDMATRIX( iPaletteEntry ), &matTemp );
                    }
                }
                
                // setup the material of the mesh subset - REMEMBER to use the original pre-skinning attribute id to get the correct material id
				static D3DMATERIAL9 material;
				material = pMeshContainer->pMaterials[pBoneComb[iAttrib].AttribId].MatD3D;
				if( !m_bHightlight )
				{
					material.Emissive.a = 1.0f;
					material.Emissive.r = 0.4f;
					material.Emissive.g = 0.4f;
					material.Emissive.b = 0.4f;
					
				}
				else
				{
					material.Emissive.a = 1.0f;
					material.Emissive.r = 1.0f;
					material.Emissive.g = 1.0f;
					material.Emissive.b = 1.0f;
				}
				//m_pd3dDevice->SetMaterial( &material );
                m_pd3dDevice->SetMaterial( &pMeshContainer->pMaterials[pBoneComb[iAttrib].AttribId].MatD3D );
                m_pd3dDevice->SetTexture( 0, pMeshContainer->ppTextures[pBoneComb[iAttrib].AttribId] );
              //  m_pd3dDevice->SetTexture( 0,NULL );

                // finally draw the subset with the current world matrix palette and material state
                pMeshContainer->MeshData.pMesh->DrawSubset( iAttrib );
            }

            // reset blending state
            m_pd3dDevice->SetRenderState(D3DRS_INDEXEDVERTEXBLENDENABLE, FALSE);
            m_pd3dDevice->SetRenderState(D3DRS_VERTEXBLEND, 0);

            // remember to reset back to hw vertex processing if software was required
            if (pMeshContainer->UseSoftwareVP)
            {
                m_pd3dDevice->SetSoftwareVertexProcessing(FALSE);
            }
        }
        else if (m_SkinningMethod == SOFTWARE)
        {
            D3DXMATRIX  Identity;
            DWORD       cBones  = pMeshContainer->pSkinInfo->GetNumBones();
            DWORD       iBone;
            PBYTE       pbVerticesSrc;
            PBYTE       pbVerticesDest;

            // set up bone transforms
            for (iBone = 0; iBone < cBones; ++iBone)
            {
                D3DXMatrixMultiply
                (
                    &m_pBoneMatrices[iBone],                 // output
                    &pMeshContainer->pBoneOffsetMatrices[iBone], 
                    pMeshContainer->ppBoneMatrixPtrs[iBone]
                );
            }

            // set world transform
            D3DXMatrixIdentity(&Identity);
            m_pd3dDevice->SetTransform(D3DTS_WORLD, &Identity);

            pMeshContainer->pOrigMesh->LockVertexBuffer(D3DLOCK_READONLY, (LPVOID*)&pbVerticesSrc);
            pMeshContainer->MeshData.pMesh->LockVertexBuffer(0, (LPVOID*)&pbVerticesDest);

            // generate skinned mesh
            pMeshContainer->pSkinInfo->UpdateSkinnedMesh(m_pBoneMatrices, NULL, pbVerticesSrc, pbVerticesDest);

            pMeshContainer->pOrigMesh->UnlockVertexBuffer();
            pMeshContainer->MeshData.pMesh->UnlockVertexBuffer();

            for (iAttrib = 0; iAttrib < pMeshContainer->NumAttributeGroups; iAttrib++)
            {

                m_pd3dDevice->SetMaterial(&(pMeshContainer->pMaterials[pMeshContainer->pAttributeTable[iAttrib].AttribId].MatD3D));
                m_pd3dDevice->SetTexture(0, pMeshContainer->ppTextures[pMeshContainer->pAttributeTable[iAttrib].AttribId]);
                pMeshContainer->MeshData.pMesh->DrawSubset(pMeshContainer->pAttributeTable[iAttrib].AttribId);
            }
        }
        else // bug out as unsupported mode
        {
            return;
        }
    }
    else  // standard mesh, just draw it after setting material properties
    {
        m_pd3dDevice->SetTransform(D3DTS_WORLD, &pFrame->CombinedTransformationMatrix);

        for (iMaterial = 0; iMaterial < pMeshContainer->NumMaterials; iMaterial++)
        {
            m_pd3dDevice->SetMaterial( &pMeshContainer->pMaterials[iMaterial].MatD3D );
            m_pd3dDevice->SetTexture( 0, pMeshContainer->ppTextures[iMaterial] );
            pMeshContainer->MeshData.pMesh->DrawSubset(iMaterial);
        }
    }	
}


VOID CSkinMesh::UpdateFrameMatrices(LPD3DXFRAME pFrameBase, LPD3DXMATRIX pParentMatrix)
{
    D3DXFRAME_DERIVED *pFrame = (D3DXFRAME_DERIVED*)pFrameBase;

    if (pParentMatrix != NULL)
        D3DXMatrixMultiply(&pFrame->CombinedTransformationMatrix, &pFrame->TransformationMatrix, pParentMatrix);
    else
        pFrame->CombinedTransformationMatrix = pFrame->TransformationMatrix;

    if (pFrame->pFrameSibling != NULL)
    {
        UpdateFrameMatrices(pFrame->pFrameSibling, pParentMatrix);
    }

    if (pFrame->pFrameFirstChild != NULL)
    {
        UpdateFrameMatrices(pFrame->pFrameFirstChild, &pFrame->CombinedTransformationMatrix);
    }
}

VOID CSkinMesh::SetRotateAngle(FLOAT fAngle)
{
	m_fRotateAngle=fAngle;
}

VOID CSkinMesh::SetAnim(BOOL bAnim)
{
		m_bMoving=bAnim;
		if(!m_bMoving && m_pAnimController)
		{	
#if ((D3D_SDK_VERSION&0xFF)== 31)		
			m_pAnimController->SetTime(0);	
#else	
			m_pAnimController->ResetTime();
#endif
		}
}
int CSkinMesh::GetNumAnimationSets()
{
	return m_pAnimController->GetNumAnimationSets();
}
VOID CSkinMesh::SetAnimationName( char *strAnimName )
{
	if(!m_bMoving || !m_pAnimController) 
		return ;
	DWORD nAnimSet;
	char strTempName[256];
	LPD3DXANIMATIONSET pAnimSet;
	nAnimSet=m_pAnimController->GetNumAnimationSets();

	for(DWORD i=0;i<nAnimSet;i++)
	{	
		m_pAnimController->GetAnimationSet(i,&pAnimSet);
		strcpy(strTempName,	pAnimSet->GetName());
		if(strcmp(strAnimName,strTempName)==0) 
		{
			m_pAnimController->SetTrackAnimationSet(0,pAnimSet);
			m_pAnimController->SetTime(0);
			break;
		}
	}
};
FLOAT CSkinMesh::GetAnimPeriod(int nAnim)
{
   LPD3DXANIMATIONSET pAnimSet;
   m_pAnimController->GetAnimationSet(nAnim,&pAnimSet);
   return pAnimSet->GetPeriod();
}

VOID CSkinMesh::SetAnimationNum(int nAnim)
{
   	if(!m_bMoving || !m_pAnimController) 
		return ;

	LPD3DXANIMATIONSET pAnimSet;
	m_pAnimController->GetAnimationSet(nAnim,&pAnimSet);
	m_pAnimController->SetTrackAnimationSet(0,pAnimSet);	
	m_pAnimController->SetTime(0);
}
		
VOID CSkinMesh::SetPosition(D3DXVECTOR3 ObjectPos)
{
	m_vMapPos=ObjectPos;
}
VOID CSkinMesh::SetScale(float ObjectScal)
{
	m_fObjectScale=ObjectScal;
}
HRESULT CSkinMesh::CalculateBondingBox( LPD3DXFRAME pFrameParent, D3DXVECTOR3 *pVmin, D3DXVECTOR3 *pVmax )		// 计算模型的包围盒，通过参数返回包围盒数据
{
	static LPVOID pV; 
	static LPD3DXMESH pMesh;
	static LPD3DXMATRIX pMat;
	static D3DXVECTOR3 vMin,vMax; 
	static D3DXVECTOR3 vTransFormed;
	
	if( pFrameParent->pMeshContainer )
	{
		pMesh = pFrameParent->pMeshContainer->MeshData.pMesh;    
		pMat = &(((D3DXFRAME_EX*)pFrameParent)->CombinedTransformationMatrix );
		
		pMesh->LockVertexBuffer( 0,&pV );	// 锁定顶点缓冲区
		D3DXComputeBoundingBox( (LPD3DXVECTOR3)pV, pMesh->GetNumVertices(), pMesh->GetNumBytesPerVertex(), &vMin, &vMax );	// 创建包围盒
		
		vTransFormed.x = pMat->_11*vMin.x + pMat->_21*vMin.y + pMat->_31*vMin.z + pMat->_41;
		vTransFormed.y = pMat->_12*vMin.x + pMat->_22*vMin.y + pMat->_32*vMin.z + pMat->_42;
		vTransFormed.z = pMat->_13*vMin.x + pMat->_23*vMin.y + pMat->_33*vMin.z + pMat->_43;
		
		if(vTransFormed.x<pVmin->x) pVmin->x = vTransFormed.x;
		if(vTransFormed.y<pVmin->y) pVmin->y = vTransFormed.y;
		if(vTransFormed.z<pVmin->z) pVmin->z = vTransFormed.z;
		
		vTransFormed.x=pMat->_11*vMax.x+pMat->_21*vMax.y+pMat->_31*vMax.z+pMat->_41;
		vTransFormed.y=pMat->_12*vMax.x+pMat->_22*vMax.y+pMat->_32*vMax.z+pMat->_42;
		vTransFormed.z=pMat->_13*vMax.x+pMat->_23*vMax.y+pMat->_33*vMax.z+pMat->_43;
		
		if(vTransFormed.x>pVmax->x) pVmax->x = vTransFormed.x;
		if(vTransFormed.y>pVmax->y) pVmax->y = vTransFormed.y;
		if(vTransFormed.z>pVmax->z) pVmax->z = vTransFormed.z;
		
		pMesh->UnlockVertexBuffer(); 
	}
	//else return S_OK;
	
	if( pFrameParent->pFrameSibling )
		CalculateBondingBox(pFrameParent->pFrameSibling,pVmin,pVmax);
	
	if( pFrameParent->pFrameFirstChild )
		CalculateBondingBox( pFrameParent->pFrameFirstChild, pVmin, pVmax );

	return S_OK;
	
}


VOID CSkinMesh::ApplyRectify( LPD3DXMATRIX pMatOrig, float fScall )
{
	m_matOrig = *pMatOrig;
	m_fScall = fScall;
	
	m_vMax *= m_fScall;
	m_vMin *= m_fScall;
	
	m_pBoundBox->CreateBox( &m_vOrigMin, &m_vOrigMax );
	m_pBoundBox->ApplyRectify(fScall);
	D3DXVec3TransformCoord( &m_vOrigMin, &m_vOrigMin, &m_matOrig );
	D3DXVec3TransformCoord( &m_vOrigMax, &m_vOrigMax, &m_matOrig );
	
	m_vCameraBindPos.x = ( m_vOrigMax.x + m_vOrigMin.x )/2;
	m_vCameraBindPos.z = ( m_vOrigMax.z + m_vOrigMin.z )/2;
	m_vCameraBindPos.y = m_vOrigMax.y;
}
CBoundBox* CSkinMesh::GetBoundBox()
{
	return m_pBoundBox;
}
