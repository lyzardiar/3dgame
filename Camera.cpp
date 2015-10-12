// Camera.cpp: implementation of the CCamera class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Camera.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCamera::CCamera(LPDIRECT3DDEVICE9 pDevice)
{
	m_pD3DDevice=pDevice;
	_pos=D3DXVECTOR3(0.0f,150.0f,-250.0f);
	_at=D3DXVECTOR3(0.0f,100.0f,0.0f);
/*	_at=D3DXVECTOR3(0.0f,100.0f,0.0f);*/
	_look=_at-_pos;
	_dir=D3DXVECTOR3(_look.x,0.f,_look.z);
}

CCamera::~CCamera()
{

}
void CCamera::Setup3DCamera()
{
	//Here we will setup the camera.
	//The camera has three settings: "Camera Position", "Look at Position" and "Up Direction"
	//We have set the following:
	//Camera Position:	(0, 15, -50)
	//Look at Position: (0, 0, 0)
	//Up direction:		Y-Axis.
    D3DXMatrixLookAtLH(&m_matView, &_pos,		//Camera Position
		&_at,		//Look At Position
		&D3DXVECTOR3(0.0f, 1.0f, 0.0f));		//Up Direction
    m_pD3DDevice->SetTransform(D3DTS_VIEW, &m_matView);
	
	//Here we specify the field of view, aspect ration and near and far clipping planes.
   D3DXMatrixPerspectiveFovLH(&m_matProj, D3DX_PI/4, 1.0f, 1.0f, 128*20*1.74f);
    m_pD3DDevice->SetTransform(D3DTS_PROJECTION, &m_matProj);


	m_pD3DDevice->SetRenderState( D3DRS_ZENABLE,          TRUE );
}
void CCamera::Setup2DCamera()
{
	D3DXMATRIX matOrtho;	
	D3DXMATRIX matIdentity;
	
	//Setup the orthogonal projection matrix and the default world/view matrix
	D3DXMatrixOrthoLH(&matOrtho, 1024.0f, 738.0f, 0.0f, 1.0f);
	D3DXMatrixIdentity(&matIdentity);
	
	m_pD3DDevice->SetTransform(D3DTS_PROJECTION, &matOrtho);
	m_pD3DDevice->SetTransform(D3DTS_WORLD, &matIdentity);
	m_pD3DDevice->SetTransform(D3DTS_VIEW, &matIdentity);
	
	//Make sure that the z-buffer and lighting are disabled
	m_pD3DDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
	m_pD3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	m_pD3DDevice->SetTextureStageState(0,D3DTSS_ALPHAARG1,D3DTA_TEXTURE);
	m_pD3DDevice->SetTextureStageState(0,D3DTSS_ALPHAOP,D3DTOP_SELECTARG1);
	m_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE,TRUE);
	m_pD3DDevice->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_SRCALPHA);
	m_pD3DDevice->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_INVSRCALPHA);
}
void CCamera::Walk(float fUnits)
{
	D3DXVec3Normalize(&_dir,&_dir);   
	_pos+=_dir*fUnits;
	_at.x=_look.x+_pos.x;
	_at.z=_look.z+_pos.z;
}
void CCamera::MoveOnX(float fUnits)
{
	D3DXVec3Normalize(&_look,&_look);   
	_pos+=D3DXVECTOR3(_look.z,0.f,-_look.x)*fUnits;//fUnits为正数 往右移动 为负数往左移动
}
void CCamera::GetViewProj(D3DXMATRIX *matViewProj)
{
	*matViewProj=m_matView*m_matProj;
}
D3DXMATRIX CCamera::GetViewMatrix(D3DXMATRIX *matView)
{
	*matView=m_matView;
	return m_matView;
}
D3DXMATRIX CCamera::GetProjMatrix(D3DXMATRIX *matProj)
{
	*matProj=m_matProj;
	return m_matProj;
}
void CCamera::GetCameraLo(D3DXVECTOR3 *look)
{
	*look=_look;
}
void CCamera::GetCameraPos(D3DXVECTOR3* Pos)
{
	*Pos=_pos;
}
void CCamera::GetCameraAt(D3DXVECTOR3 *at)
{
	*at=_at;
}
void CCamera::GetCameraDir(D3DXVECTOR3 *dir)
{
	*dir=_dir;
}
void CCamera::SetPos(D3DXVECTOR3 vPos)
{
	D3DXVec3Normalize(&_look,&_look);
    _pos=vPos+D3DXVECTOR3(0.f,36,0)-255.0f*_look;
	_at=_pos+_look;
}
void CCamera::SetCameraRotationX(float fAngle)
{
    D3DXVec3Normalize(&_look,&_look);
    if ((_look.y<0.12&&_look.y>-0.8)||(_look.y>0.12&&fAngle>0)||(_look.y<-0.8&&fAngle<0))
    {
		D3DXVECTOR3 vOldLook=_look;
		float fRad=0.f,fNewRad;
		fRad=sqrt(_look.x*_look.x+_look.z*_look.z);
		fNewRad=fRad*cos(fAngle)+vOldLook.y*sin(fAngle);
		_look.y=vOldLook.y*cos(fAngle)-fRad*sin(fAngle);
        _look.x=vOldLook.x*fNewRad/fRad;
		_look.z=vOldLook.z*fNewRad/fRad;
		_pos+=-255.f*(_look-vOldLook);
		_at=_look+_pos;
		_dir=D3DXVECTOR3(_look.x,0.f,_look.z);

// 		D3DXVec3Normalize(&_look,&_look);
// 		D3DXVec3Normalize(&vOldLook,&vOldLook);
// 		_pos.y+=-255.f*(_look.y-vOldLook.y);
// 		_pos.z+=-255.f*(_look.z-vOldLook.z);
// 		_at=_look+_pos;
// 		_dir=D3DXVECTOR3(_look.x,0.f,_look.z);
    }



}
void CCamera::SetCameraRotationY(float fAngle)
{
	D3DXVECTOR3 vOldLook=_look;
	float fRad=0.f;
	fRad=sqrt(_look.x*_look.x+_look.z*_look.z);
    _look.x=vOldLook.x*cos(fAngle)+vOldLook.z*sin(fAngle);
	_look.z=vOldLook.z*cos(fAngle)-vOldLook.x*sin(fAngle);
	D3DXVec3Normalize(&_look,&_look);
	D3DXVec3Normalize(&vOldLook,&vOldLook);
	_pos+=-255.f*(_look-vOldLook);
//     _pos.x+=-250*(_look.x-vOldLook.x)/fRad;
// 	_pos.z+=-250*(_look.z-vOldLook.z)/fRad;
	_at=_look+_pos;
	_dir=D3DXVECTOR3(_look.x,0.f,_look.z);
}