#include "stdafx.h"
#include "SinglePageWnd.h"
#include "PageWndMsgHandler.h"

#define WINDOWCLASSNAME L"SingleWnd"	


SinglePageWnd* SinglePageWnd::m_SinglePageWnd;
PageWndMsgHandler *msgHandler;					// 窗体消息处理实例


SinglePageWnd::SinglePageWnd(HINSTANCE hInstance, HWND parientHWnd,int iWidth,int iHeight,std::string sURL)
	:m_hIns(hInstance),
m_Width(iWidth),
m_Height(iHeight)
{
	m_SinglePageWnd = this;

	msgHandler = new PageWndMsgHandler(parientHWnd,sURL);

	MyRegisterClass();

	m_hWnd = CreateWindow(
    WINDOWCLASSNAME,
    WINDOWCLASSNAME,
    WS_CLIPCHILDREN|WS_CLIPSIBLINGS|WS_CHILD,
    CW_USEDEFAULT,
    CW_USEDEFAULT,
    m_Width,
    m_Height,
    parientHWnd,
    NULL,
    m_hIns,
    NULL);

    ShowWindow(m_hWnd, SW_SHOW);
    UpdateWindow(m_hWnd);
}


SinglePageWnd::~SinglePageWnd()
{
}

void SinglePageWnd::MyRegisterClass()
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = SinglePageWnd::WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = m_hIns;
    wcex.hIcon          = NULL;
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1); 
    wcex.lpszMenuName   = NULL;
    wcex.lpszClassName  = WINDOWCLASSNAME;
    wcex.hIconSm        = NULL;

    RegisterClassExW(&wcex);
}

LRESULT CALLBACK SinglePageWnd::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	return msgHandler->PageWndProc(m_SinglePageWnd->m_hIns,hWnd,message,wParam,lParam);
}


void SinglePageWnd::WndSizeChanged(RECT rect,WPARAM wParam)
{
	if (wParam == SIZE_MINIMIZED)
		return;

	m_Width = rect.right - rect.left;
	m_Height = rect.bottom - rect.top;

	::SetWindowPos(m_hWnd,NULL,
		LEFTEXTENDWIDTH, TOPEXTENDWIDTH, m_Width, m_Height, SWP_SHOWWINDOW);

	return;
}
