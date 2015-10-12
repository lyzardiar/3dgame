// GameProc.cpp: implementation of the CGameProc class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GameProc.h"

CChatPopWnd* g_pChatPopWnd;
CPlayer *g_pPlayer;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
char g_szBuf[255];
CGameProc::CGameProc()
{
	GetClientRect(GetActiveWindow(), &m_clientRect );
	m_clientWidth = m_clientRect.right - m_clientRect.left;
	m_clientHeight = m_clientRect.bottom - m_clientRect.top;

	
	m_fAnlgeX=m_fAnlgeY=0.f; 
	m_bRCapture=false;
	m_bLCapture=false;
	m_pD3DDevice=g_pGame->GetDevice();

	m_pSkyBox=new CSkyBox(128*20);
	m_pSkyBox->Init("sky\\", m_pD3DDevice, ".JPG");
	
	g_pPlayer=new CPlayer();	
	g_pPlayer->Init(m_pD3DDevice);

	g_pChatPopWnd=new CChatPopWnd(m_pD3DDevice,0,130,300,400);
	Loaddata();

	m_pTerrain=new CTerrain(m_pD3DDevice,"terrain/heightmap.raw",129,129,20.f,2.f);
	
	m_pWater=new CWater();
	m_pWater->Init(m_pD3DDevice,1200,1500,D3DXVECTOR3(-680,42,-200));

	m_pRoleInfo=new CRoleInfo(&g_pPlayer->m_xPlayInfo);


}
void CGameProc::Loaddata()
{
	int nMonsterCnt;
	nMonsterCnt=GetPrivateProfileInt("map0","monstercnt",0,"data\\map.txt");
	for (int i=0;i<nMonsterCnt;i++)
	{
		CMonster *pMonster;
		pMonster=new CMonster();
		pMonster->Create(i,m_pD3DDevice);
		m_xMonsterList.MoveCurrentToTop();
		m_xMonsterList.AddNode(pMonster);
	}
}

CGameProc::~CGameProc()
{
	SAFE_DELETE(g_pPlayer);
	SAFE_DELETE(m_pSkyBox);
	SAFE_DELETE(m_pTerrain);
	SAFE_DELETE(m_pWater);
	SAFE_DELETE(m_pD3DDevice);

}

void CGameProc::Render(float fElapsedTime)
{
	int nCnt;
	if(m_pD3DDevice == NULL)
    {
		return;
    }
	
	g_pPlayer->Update(fElapsedTime);

	m_xSkillWnd.Update(fElapsedTime);

	m_xMonsterList.MoveCurrentToTop();
	for (nCnt=0;nCnt<m_xMonsterList.GetCounter();nCnt++)
	{
		CMonster *pMonster;
		pMonster=m_xMonsterList.GetCurrentData();
		pMonster->Update(fElapsedTime);
		m_xMonsterList.MoveNextNode();
	}

	m_pWater->Update(fElapsedTime);

 	if (g_pPlayer->m_pTargetInfo)
 		m_pRoleInfo->Update(&g_pPlayer->m_xPlayInfo,g_pPlayer->m_pTargetInfo,true);
 	else
    	m_pRoleInfo->Update(&g_pPlayer->m_xPlayInfo,g_pPlayer->m_pTargetInfo,false);
	
	//Clear the back buffer and depth buffer
    m_pD3DDevice->Clear(0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, NULL, 1.0f, 0);
    m_pD3DDevice->BeginScene();

	g_pCamera->Setup3DCamera();
	
	g_pPlayer->Render();

	float i;
	m_xMonsterList.MoveCurrentToTop();
	for (nCnt=0;nCnt<m_xMonsterList.GetCounter();nCnt++)
	{
		CMonster *pMonster;
		pMonster=m_xMonsterList.GetCurrentData();
		if (nCnt==2)
		{
			pMonster->m_pMonsterMesh->SetPosition(D3DXVECTOR3(pMonster->m_xMonsterInfo.x,m_pTerrain->getHeight(pMonster->m_xMonsterInfo.x,pMonster->m_xMonsterInfo.z),pMonster->m_xMonsterInfo.z));
			i=pMonster->m_fAngel;
		}
		pMonster->Render();
		m_xMonsterList.MoveNextNode();
	}
	
	D3DXVECTOR3 playerPos;
	g_pPlayer->GetPos(&playerPos);
	m_pSkyBox->Render(playerPos);
	
	D3DXMATRIX matViewProj;
    g_pCamera->GetViewProj(&matViewProj);
  m_pTerrain->Render(&matViewProj,false);	 
	
	// 绘制透明物体
    m_pWater->Render();

	g_pCamera->Setup2DCamera();

	g_pChatPopWnd->Render();
	m_xSkillWnd.Render();
	m_pRoleInfo->Render();

	

	char buf[255];
	if (g_pPlayer->m_pPlayerMesh->m_bHightlight)
	{
       g_pFont->DrawText("打开深度光照",0,0,D3DCOLOR_XRGB(255,255,0));
	}
	D3DXVECTOR3 vPos;
	g_pCamera->GetCameraPos(&vPos);



	sprintf(buf,"坐标%f %f %f\n 摄像机位置 %f %f %f	\n 鼠标坐标%d,%d \n%d %d\n%f",playerPos.x,playerPos.z,fElapsedTime,vPos.x,vPos.y,vPos.z,m_pt.x,m_pt.y
	,g_pChatPopWnd->m_nTaskState,g_pChatPopWnd->m_nPageIndex,i/D3DX_PI);
   g_pFont->DrawText(buf,0,512,D3DCOLOR_XRGB(0,0,0));

   if (g_szBuf!=NULL&&m_xSkillWnd.m_xLock.bLock)
   {
	      g_pFont->DrawText(g_szBuf,456,150,D3DCOLOR_XRGB(0,0,0));
   }

    m_pD3DDevice->EndScene();
    //Filp the back and front buffers so that whatever has been rendered on the back buffer
    //will now be visible on screen (front buffer).
    m_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

LRESULT CGameProc::DefMainWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_RBUTTONDOWN:
		{
            OnRButtonDown(wParam,lParam);
		}		
		break;
	case WM_RBUTTONUP:
		{
			m_bRCapture=false;
		}
		break;
	case WM_LBUTTONDOWN:
		{
// 			bLCapture=true;
// 			fAngleRotY=0.f;
// 			Oldpt.x = LOWORD(lParam);
// 			Oldpt.y = HIWORD(lParam);
			OnLButtonDown(wParam,lParam);
		}		
		break;
	case WM_LBUTTONUP:
		{
			m_pt.x = (float)LOWORD(lParam);
			m_pt.y = (float)HIWORD(lParam);
			if (m_xSkillWnd.OnLButtonUp(m_pt))
			{
			}
// 			if (bLCapture)
// 			{
// 				m_pCamera->SetCameraRotationY(-1*fAngleRotY);
// 			}
// 			bLCapture=false;
		}
		break;
	case WM_MOUSEMOVE:
		{
			m_pt.x = (float)LOWORD(lParam);
			m_pt.y = (float)HIWORD(lParam);

			if (m_xSkillWnd.OnMouseMove(m_pt))
			{
			}
			else if (g_pChatPopWnd->OnMouseMove(m_pt))
			{

				return 0;
			}    
			

			if (m_pt.x<100||m_pt.x>924||m_pt.y<50||m_pt.y>688)
			{
					m_bRCapture=false;
			}
		
			if (m_bRCapture)
			{ 	
     			m_fAnlgeX=(m_pt.y-m_Oldpt.y)*(4*D3DX_PI/5)/m_clientHeight;
		        g_pCamera->SetCameraRotationX(m_fAnlgeX);

				m_fAnlgeY=(m_pt.x-m_Oldpt.x)*(D3DX_PI)/m_clientWidth;
             	g_pCamera->SetCameraRotationY(m_fAnlgeY);
	    		g_pPlayer->SetAngle(m_fAnlgeY);
  			}
			else if (m_bLCapture)
			{
				m_fAnlgeX=(m_pt.y-m_Oldpt.y)*(4*D3DX_PI/5)/m_clientHeight;
		        g_pCamera->SetCameraRotationX(m_fAnlgeX);
                
				m_fAnlgeY=(m_pt.x-m_Oldpt.x)*(D3DX_PI)/m_clientWidth;
             	g_pCamera->SetCameraRotationY(m_fAnlgeY);
				m_fAngleRotY+=m_fAnlgeY;
			}
			m_Oldpt=m_pt;
		}
		break;
	case WM_KEYDOWN:
		{
			if (g_pPlayer->IsDead())
			{
				return 0;
			}
			switch (wParam)
			{
			case 'W':
				{
                     g_pCamera->Walk(2.f);
					 D3DXVECTOR3 vDir,vPos;
				     g_pCamera->GetCameraDir(&vDir);
                   	 g_pPlayer->Walk(vDir,2.f);
					 g_pPlayer->GetPos(&vPos);
					 g_pPlayer->SetPos(D3DXVECTOR3(vPos.x,m_pTerrain->getHeight(vPos.x,vPos.z),vPos.z));
					 g_pCamera->SetPos(D3DXVECTOR3(vPos.x,m_pTerrain->getHeight(vPos.x,vPos.z),vPos.z));
				}
				break;
			case 'S':
				{

                     g_pCamera->Walk(-2.f);
					 D3DXVECTOR3 vDir,vPos;
					 g_pCamera->GetCameraDir(&vDir);
					 g_pPlayer->Walk(vDir,-2.f);
					 g_pPlayer->GetPos(&vPos);
					 g_pPlayer->SetPos(D3DXVECTOR3(vPos.x,m_pTerrain->getHeight(vPos.x,vPos.z),vPos.z));
					 g_pCamera->SetPos(D3DXVECTOR3(vPos.x,m_pTerrain->getHeight(vPos.x,vPos.z),vPos.z));
				}
				break;
			case 'A':
				{
					g_pCamera->SetCameraRotationY(-D3DX_PI/24);
				}
				break;
			case 'D':
				{  
           			g_pCamera->SetCameraRotationY(D3DX_PI/24);
				}
				break;
			case  'L':
				{
					g_pChatPopWnd->Open();
				}
				break;;
			case VK_DOWN:
				{
					g_pCamera->SetCameraRotationX(D3DX_PI/32);
				}
				break;
			case VK_UP:
				{
					g_pCamera->SetCameraRotationX(-D3DX_PI/32);
				}
				break;
			case VK_RETURN:
				{

				}
				break;
			}
			g_pPlayer->OnKeyDown(wParam,lParam);
			m_xSkillWnd.OnKeyDown(wParam,lParam);
		}
		break;
	case WM_KEYUP:
		g_pPlayer->OnKeyUp(wParam,lParam);
		break;
	}
	return DefWindowProc(hWnd,msg,wParam,lParam);
}
bool CGameProc::Pick(POINT ptMouse)
{
	LPMONINFO oldMonInfo=g_pPlayer->m_pTargetInfo;
	D3DXMATRIX pmatProj = g_pCamera->GetProjMatrix(&pmatProj);
	D3DXVECTOR3 v;
	v.x = ( ( ( 2.0f * m_pt.x ) / m_clientWidth) - 1 ) / pmatProj._11;
	v.y = -( ( ( 2.0f * m_pt.y ) / m_clientHeight) - 1 ) / pmatProj._22;
	v.z = 1.0f;
	
	D3DXMATRIX matView=g_pCamera->GetViewMatrix(&matView);
	D3DXMATRIX m;
	D3DXVECTOR3 vDir,vPos;
	D3DXMatrixInverse(&m,NULL,&matView);
    vDir.x = v.x * m._11 + v.y * m._21 + v.z * m._31;
    vDir.y = v.x * m._12 + v.y * m._22 + v.z * m._32;
	vDir.z = v.x * m._13 + v.y * m._23 + v.z * m._33;
	vPos.x = m._41;
    vPos.y = m._42;
    vPos.z = m._43;
	m_xMonsterList.MoveCurrentToTop();
	for (int nCnt=0;nCnt<m_xMonsterList.GetCounter();nCnt++)
	{
		CMonster *pMonster;
		pMonster=m_xMonsterList.GetCurrentData();
		
		if(pMonster->m_pMonsterMesh->GetBoundBox()->CheckIntersectByRay(&vPos,&vDir))
		{
			//	pMonster->m_pMonsterMesh->m_bFoucs=true;
			g_pPlayer->m_pTargetInfo=&pMonster->m_xMonsterInfo;
			return true;
		}
// 		else
// 		{
// 			//	pMonster->m_pMonsterMesh->m_bFoucs=false;
// 			g_pPlayer->m_pTargetInfo=NULL;
// 		}
		m_xMonsterList.MoveNextNode();
	}
	return false;

}
LRESULT CGameProc::OnLButtonDown(WPARAM wParam, LPARAM lParam)
{
	m_pt.x = (float)LOWORD(lParam);
	m_pt.y = (float)HIWORD(lParam);
	if (m_xSkillWnd.OnLButtonDown(m_pt))
	{
	}
	else if(g_pChatPopWnd->OnLButtonDown(m_pt))
		  m_xTaskScript.CallFun(g_pPlayer->m_pTargetInfo->TaskIndex);
	else 
	{	
		if (!Pick(m_pt))
		{
            g_pPlayer->m_pTargetInfo=NULL;
		}
	}


	return true;
}
LRESULT CGameProc::OnRButtonDown(WPARAM wParam, LPARAM lParam)
{
	m_bRCapture=true;
	m_Oldpt.x = LOWORD(lParam);
	m_Oldpt.y = HIWORD(lParam);

	if (Pick(m_Oldpt))
	{
		if (g_pPlayer->CalculateTagDistanceSquece()<50*50&&g_pPlayer->m_pTargetInfo->TaskIndex!=0)
		{
			//判断任务怪死亡没有
			if (g_pChatPopWnd->m_nTaskState==2)
			{
				m_xMonsterList.MoveCurrentToTop();
				for (int nCnt=0;nCnt<m_xMonsterList.GetCounter();nCnt++)
				{
					CMonster *pMonster;
					pMonster=m_xMonsterList.GetCurrentData();
					
					if (pMonster->m_xMonsterInfo.Id==2)
					{
						if (!pMonster->IsDead())
						{
							g_pChatPopWnd->m_nTaskState=1;
						}
					}
					m_xMonsterList.MoveNextNode();
				}
			}

			g_pChatPopWnd->Open();
			m_xTaskScript.CallFun(g_pPlayer->m_pTargetInfo->TaskIndex);
		}
	}
	return true;
}

