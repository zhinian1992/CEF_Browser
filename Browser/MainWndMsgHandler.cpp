#include "stdafx.h"
#include "resource.h"
#include "MainWndMsgHandler.h"
#include <dwmapi.h>
#include <map>
#include <string>
#include <functional>

using namespace std;

#define LEFTEXTENDWIDTH 0
#define RIGHTEXTENDWIDTH 0
#define BOTTOMEXTENDWIDTH 0
#define TOPEXTENDWIDTH 25

// 全局变量:
CefRefPtr<CCefHandler> m_CefHandler;
typedef bool (MainWndMsgHandler::*pMsgHandler)(HINSTANCE hInst,HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
map<int, pMsgHandler> m_FuncMap;


MainWndMsgHandler::MainWndMsgHandler()
{
	m_FuncMap.insert(make_pair(WM_CREATE,&MainWndMsgHandler::CreateMsgHandler));
	m_FuncMap.insert(make_pair(WM_COMMAND,&MainWndMsgHandler::CommandMsgHandler));
	m_FuncMap.insert(make_pair(WM_PAINT, &MainWndMsgHandler::PaintMsgHandler));
	m_FuncMap.insert(make_pair(WM_SIZE, &MainWndMsgHandler::SizeMsgHandler));
	m_FuncMap.insert(make_pair(WM_ACTIVATE, &MainWndMsgHandler::ActivateMsgHandler));
	m_FuncMap.insert(make_pair(WM_DESTROY, &MainWndMsgHandler::DestroyMsgHandler));

}


MainWndMsgHandler::~MainWndMsgHandler()
{
}

LRESULT CALLBACK MainWndMsgHandler::MainWndProc(HINSTANCE hInst,HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	map<int, pMsgHandler>::iterator it;
	for(it = m_FuncMap.begin();it != m_FuncMap.end();++it)
	{
		if(it->first == message)
		{
			(this->*(it->second))(hInst,hWnd,message,wParam,lParam);
			return 0;
		}
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}

INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

bool MainWndMsgHandler::CommandMsgHandler(HINSTANCE hInst, HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId = LOWORD(wParam);
    // 分析菜单选择:
    switch (wmId)
    {
    case IDM_ABOUT:
        DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
        break;
    case IDM_EXIT:
        DestroyWindow(hWnd);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return true;
}

bool MainWndMsgHandler::ActivateMsgHandler(HINSTANCE hInst, HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	//extend the frame into the client area
	MARGINS margins;

	margins.cxLeftWidth = LEFTEXTENDWIDTH;
	margins.cxRightWidth = RIGHTEXTENDWIDTH;
	margins.cyBottomHeight = BOTTOMEXTENDWIDTH;
	margins.cyTopHeight = TOPEXTENDWIDTH;

	HRESULT hr = S_OK;
    hr = DwmExtendFrameIntoClientArea(hWnd, &margins);

	if(!SUCCEEDED(hr))
	{
		
	}

	return true;
}

bool MainWndMsgHandler::SizeMsgHandler(HINSTANCE hInst, HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (wParam == SIZE_MINIMIZED
		|| m_CefHandler == NULL
		|| m_CefHandler->GetBrowserHostWnd() == NULL)
		return false;

	RECT rect;
	::GetWindowRect(hWnd,&rect);
	::SetWindowPos(m_CefHandler->GetBrowserHostWnd(),NULL,
		0, TOPEXTENDWIDTH, rect.right - rect.left, rect.bottom - rect.top - TOPEXTENDWIDTH, SWP_SHOWWINDOW);

	return true;
}

bool MainWndMsgHandler::PaintMsgHandler(HINSTANCE hInst, HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(hWnd, &ps);
	// TODO: 在此处添加使用 hdc 的任何绘图代码...
	EndPaint(hWnd, &ps);

	return true;
}

bool MainWndMsgHandler::CreateMsgHandler(HINSTANCE hInst, HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	CreateBrowser(hWnd);

	return true;
}

bool MainWndMsgHandler::DestroyMsgHandler(HINSTANCE hInst, HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PostQuitMessage(0);

	return true;
}

void MainWndMsgHandler::CreateBrowser(HWND hWnd)
{
	m_CefHandler = new CCefHandler();
	if(m_CefHandler != NULL)
	{
		RECT rect;
		::GetClientRect(hWnd, &rect);
		rect.top += TOPEXTENDWIDTH;
		std::string sURL = "www.baidu.com";
		CefString cURL = sURL;
		m_CefHandler->CreateBrowser(hWnd, rect, cURL);
	}
}