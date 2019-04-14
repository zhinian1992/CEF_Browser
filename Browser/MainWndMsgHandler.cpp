#include "stdafx.h"
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

//
//  函数: MainWndProc(HINSTANCE, HWND, UINT, WPARAM, LPARAM)
//
//  目标: 处理浏览器主窗体消息。
//
LRESULT MainWndMsgHandler::MainWndProc(HINSTANCE hInst,HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
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

//
//  函数: CommandMsgHandler(HINSTANCE, HWND, UINT, WPARAM, LPARAM)
//
//  目标: 窗口菜单命令项，控件触发消息。
//
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

//
//  函数: ActivateMsgHandler(HINSTANCE,HWND,UINT,WPARAM,LPARAM)
//
//  目标: 窗口激活消息响应函数。
//
//  注释:
//
//		  在此函数中frame extension扩展框架实现
//		  窗体初始化和最大化时正确处理框架扩展。
//
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

//
//  函数: SizeMsgHandler(HINSTANCE,HWND,UINT,WPARAM,LPARAM)
//
//  目标: 窗口大小改变消息响应函数。
//
//  注释:
//
//		  在此函数中窗体大小改变时改变子窗体大小
//
void MainWndMsgHandler::SizeMsgHandler(HINSTANCE hInst, HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	RECT rect;
	::GetWindowRect(hWnd,&rect);

	vector<SinglePageWnd *>::iterator iter;
	for(iter = m_PageList.begin();iter != m_PageList.end();++iter)
	{
		(*iter)->WndSizeChanged(rect, wParam);
	}

	return;
}

//
//  函数: PaintMsgHandler(HINSTANCE,HWND,UINT,WPARAM,LPARAM)
//
//  目标: 窗口绘制消息响应函数。
//
void MainWndMsgHandler::PaintMsgHandler(HINSTANCE hInst, HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(hWnd, &ps);
	// TODO: 在此处添加使用 hdc 的任何绘图代码...
	EndPaint(hWnd, &ps);

	return;
}

//
//  函数: CreateMsgHandler(HINSTANCE,HWND,UINT,WPARAM,LPARAM)
//
//  目标: 新窗口创建消息响应函数。
//
void MainWndMsgHandler::CreateMsgHandler(HINSTANCE hInst, HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	CreateBrowserPage(hInst,hWnd);

	return;
}

//
//  函数: DestroyMsgHandler(HINSTANCE,HWND,UINT,WPARAM,LPARAM)
//
//  目标: 窗口销毁消息响应函数。
//
void MainWndMsgHandler::DestroyMsgHandler(HINSTANCE hInst, HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PostQuitMessage(0);

	return;
}

//
//  函数: CreateBrowserPage(HINSTANCE,HWND)
//
//  目标: 浏览器页面创建消息响应函数。
//
void MainWndMsgHandler::CreateBrowserPage(HINSTANCE hInst,HWND hWnd)
{
	RECT rect;
	::GetWindowRect(hWnd,&rect);

	const string sURL = "www.baidu.com";
	SinglePageWnd *pageWnd = new SinglePageWnd(hInst,hWnd,
		rect.right - rect.left,rect.bottom - rect.top,sURL);

	m_PageList.push_back(pageWnd);

	return;
}