// Camera.h: interface for the CCamera class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CAMERA_H__48B7BC27_B223_47B1_ABD4_C17EFD0A7401__INCLUDED_)
#define AFX_CAMERA_H__48B7BC27_B223_47B1_ABD4_C17EFD0A7401__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
class CCamera  
{
public:
	CCamera(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CCamera();
	void Setup3DCamera();
	void Setup2DCamera();
	void SetCameraRotationX(float fAngle);
	void SetCameraRotationY(float fAngle);
	void Walk(float fUnits);
	void MoveOnX(float fUnits);
	void GetViewProj(D3DXMATRIX *matViewProj);
	void GetCameraPos(D3DXVECTOR3 *Pos);
	void GetCameraLo(D3DXVECTOR3 *look);
	void GetCameraAt(D3DXVECTOR3 *at);
	void GetCameraDir(D3DXVECTOR3 *dir);
	void SetPos(D3DXVECTOR3 vPos);
	void SetLookFit();
	D3DXMATRIX GetViewMatrix(D3DXMATRIX *matView);
    D3DXMATRIX GetProjMatrix(D3DXMATRIX *matProj);

private:
	LPDIRECT3DDEVICE9 m_pD3DDevice;		
	D3DXMATRIX m_matView;
	D3DXMATRIX m_matProj;
	D3DXVECTOR3 _at;					
	D3DXVECTOR3 _pos;								
	D3DXVECTOR3 _look;
	D3DXVECTOR3 _dir;

};

#endif // !defined(AFX_CAMERA_H__48B7BC27_B223_47B1_ABD4_C17EFD0A7401__INCLUDED_)
