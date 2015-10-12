// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__A9DB83DB_A9FD_11D0_BFD1_444553540000__INCLUDED_)
#define AFX_STDAFX_H__A9DB83DB_A9FD_11D0_BFD1_444553540000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers

#define SafeRelease(pInterface) if(pInterface != NULL) {pInterface->Release(); pInterface=NULL;}
#define SafeDelete(pObject) if(pObject != NULL) {delete pObject; pObject=NULL;}

#include <windows.h>
#include <mmsystem.h>
#include <stdio.h>
#include <fstream>
#include <iostream>
// #include <stdlib.h>
// #include <time.h>
// #include <math.h>
// 
 #include <string>
// #include <vector>
// #include <map>
// #include <list>
#include "vector"
using namespace std;

#include "Font.h"
#include "GameLoop.h"
#include "Panel.h"
#include "SkinMesh.h"
#include "dxutil.h"
#include "Player.h"
#include "Terrain.h"
#include "SkyBox.h"
#include "extern.h"
#include "Monster.h"
#include "BoundBox.h"
#include "PDList.h"

#include "ItemBtn.h"

// TODO: reference additional headers your program requires here

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__A9DB83DB_A9FD_11D0_BFD1_444553540000__INCLUDED_)
