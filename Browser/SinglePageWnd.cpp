#include "stdafx.h"
#include "SinglePageWnd.h"

#define WINDOWCLASSNAME L"SingleWnd"	

CefRefPtr<CCefHandler> m_CefHandler;
SinglePageWnd* SinglePageWnd::m_staticSPW;


SinglePageWnd::SinglePageWnd(HINSTANCE hInstance, HWND parientHWnd,int iWidth,int iHeight,std::string sURL)
	:m_ParientHWnd(parientHWnd),
m_hIns(hInstance),
m_Width(iWidth),
m_Height(iHeight),
m_URL(sURL)
{
	m_staticSPW = this;

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
	switch (message)
	{
		case WM_COMMAND:
	        {
	            int wmId = LOWORD(wParam);
	            // ·ÖÎö²Ëµ¥Ñ¡Ôñ:
	            switch (wmId)
	            {
	            default:
	                return DefWindowProc(hWnd, message, wParam, lParam);
	            }
	        }
	        break;
		case WM_SIZE:
			{
				RECT rect;
				::GetClientRect(hWnd, &rect);

				if (wParam == SIZE_MINIMIZED
					|| m_CefHandler == NULL
					|| m_CefHandler->GetBrowserHostWnd() == NULL)
					return 0;

				::SetWindowPos(m_CefHandler->GetBrowserHostWnd(),NULL,
					0, 0, m_staticSPW->m_Width, m_staticSPW->m_Height, SWP_SHOWWINDOW);
			}
			break;
		case WM_CREATE:
			{
				m_staticSPW->m_hWnd = hWnd;
				m_staticSPW->CreateBrowserPage();
			}
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

void SinglePageWnd::CreateBrowserPage()
{
	assert(m_hWnd != NULL);
	m_CefHandler = new CCefHandler();
	if(m_CefHandler != NULL)
	{
		RECT rect;
		::GetClientRect(m_hWnd, &rect);
		CefString cURL = m_URL;
		m_CefHandler->CreateBrowser(m_hWnd, rect, cURL);
	}
}

void SinglePageWnd::WndSizeChanged(RECT rect,WPARAM wParam)
{
	if (wParam == SIZE_MINIMIZED)
		return;

	m_Width = rect.right - (rect.left < 0? -rect.left:rect.left);
	m_Height = rect.bottom - (rect.top < 0? -rect.top:rect.top) - TOPEXTENDWIDTH;

	::SetWindowPos(m_hWnd,NULL,
		0, TOPEXTENDWIDTH, m_Width, m_Height, SWP_SHOWWINDOW);

	return;
}
