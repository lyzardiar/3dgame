// 3dgame.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
CGameLoop* g_pGame;

void CleanUp()
{
	SafeDelete(g_pGame);
}
LRESULT WINAPI WinProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch(msg)
    {
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
        break;
	case WM_KEYUP: 
		{
			switch (wParam)
			{ 
			case VK_ESCAPE:
				//User has pressed the escape key, so quit
				DestroyWindow(hWnd);
				return 0;
				break;
			} 
		}
		break;
	}
	return g_pGame->Proc(hWnd,msg,wParam,lParam);
}
int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
 	// TODO: Place code here.

	g_pGame = new CGameLoop();
//	g_pFont = new CFont("Verdana", 15, false, false, false);

	WNDCLASSEX wc = {sizeof(WNDCLASSEX), CS_CLASSDC, WinProc, 0L, 0L, 
		GetModuleHandle(NULL), NULL, NULL, NULL, NULL,
		"game", NULL};
	
	//Set the mouse pointer to an arrow
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	
	RegisterClassEx(&wc);
	
    //Create the application's window
    HWND hWnd = CreateWindow("game", "game", 
		WS_OVERLAPPEDWINDOW,(GetSystemMetrics(SM_CXSCREEN)-1024)/2, (GetSystemMetrics(SM_CYSCREEN)-738)/2,1024, 
		738,NULL, NULL, wc.hInstance, NULL);
	
	//Show our window
    ShowWindow(hWnd, SW_SHOWDEFAULT);
    UpdateWindow(hWnd);
	
	// 	//Initialize Direct3D
	if(SUCCEEDED(g_pGame->InitD3D(hWnd)))
    { 
        //Start game running: Enter the game loop
		if (g_pGame->InitGame())
		{
			g_pGame->TheGameLoop();
		}
		
    } 
	CleanUp();
    UnregisterClass("game", wc.hInstance);
	return 0;
}






