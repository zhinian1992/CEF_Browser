#include "stdafx.h"
#include "resource.h"
#include "MainWndMsgHandler.h"
#include "SinglePageWnd.h"
#include <windows.h>
#include <dwmapi.h>
#include <map>
#include <vector>
#include <string>
#include <functional>

using namespace std;

typedef void (MainWndMsgHandler::*pMsgHandler)(HINSTANCE hInst,HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
map<int, pMsgHandler> m_FuncMap;		//消息处理map
vector<SinglePageWnd *> m_PageList;		//页面集合


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

void MainWndMsgHandler::CommandMsgHandler(HINSTANCE hInst, HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
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
        DefWindowProc(hWnd, message, wParam, lParam);
		return;
	}

	return;
}

void MainWndMsgHandler::ActivateMsgHandler(HINSTANCE hInst, HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
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

	return;
}

void MainWndMsgHandler::SizeMsgHandler(HINSTANCE hInst, HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	OutputDebugStringA("SizeMsgHandler");

	RECT rect;
	::GetWindowRect(hWnd,&rect);

	vector<SinglePageWnd *>::iterator iter;
	for(iter = m_PageList.begin();iter != m_PageList.end();++iter)
	{
		(*iter)->WndSizeChanged(rect, wParam);
	}

	return;
}

void MainWndMsgHandler::PaintMsgHandler(HINSTANCE hInst, HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(hWnd, &ps);
	// TODO: 在此处添加使用 hdc 的任何绘图代码...
	EndPaint(hWnd, &ps);

	return;
}

void MainWndMsgHandler::CreateMsgHandler(HINSTANCE hInst, HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	OutputDebugStringA("CreateMsgHandler");

	CreateNewPage(hInst,hWnd);

	return;
}

void MainWndMsgHandler::DestroyMsgHandler(HINSTANCE hInst, HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PostQuitMessage(0);

	return;
}

void MainWndMsgHandler::CreateNewPage(HINSTANCE hInst,HWND hWnd)
{
	RECT rect;
	::GetWindowRect(hWnd,&rect);

	string sURL = "www.baidu.com";
	SinglePageWnd *pageWnd = new SinglePageWnd(hInst,hWnd,
		rect.right - rect.left,rect.bottom - rect.top,sURL);

	m_PageList.push_back(pageWnd);

	return;
}