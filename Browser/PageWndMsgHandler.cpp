#include "stdafx.h"
#include "PageWndMsgHandler.h"
#include <map>
#include <string>
#include <functional>


using namespace std;

typedef void (PageWndMsgHandler::*pMsgHandler)(HINSTANCE hInst,HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
map<int, pMsgHandler> m_FuncMap;		//��Ϣ����map
CefRefPtr<CCefHandler> m_CefHandler;

PageWndMsgHandler::PageWndMsgHandler(HWND parientHWnd,std::string sURL)
	:m_URL(sURL),
m_ParientHWnd(parientHWnd)
{
	m_FuncMap.insert(make_pair(WM_CREATE,&PageWndMsgHandler::CreateMsgHandler));
	m_FuncMap.insert(make_pair(WM_PAINT, &PageWndMsgHandler::PaintMsgHandler));
	m_FuncMap.insert(make_pair(WM_SIZE, &PageWndMsgHandler::SizeMsgHandler));
	m_FuncMap.insert(make_pair(WM_DESTROY, &PageWndMsgHandler::DestroyMsgHandler));
}


PageWndMsgHandler::~PageWndMsgHandler()
{
}

LRESULT PageWndMsgHandler::PageWndProc(HINSTANCE hInst, HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	map<int, pMsgHandler>::iterator it;
	for (it = m_FuncMap.begin(); it != m_FuncMap.end(); ++it)
	{
		if (it->first == message)
		{
			(this->*(it->second))(hInst, hWnd, message, wParam, lParam);
			return 0;
		}
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}

//
//  ����: CreateMsgHandler(HINSTANCE,HWND,UINT,WPARAM,LPARAM)
//
//  Ŀ��: �´��ڴ�����Ϣ��Ӧ������
//
void PageWndMsgHandler::CreateMsgHandler(HINSTANCE hInst, HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	CreateBrowserPage(hWnd);

	return;
}

//
//  ����: PaintMsgHandler(HINSTANCE,HWND,UINT,WPARAM,LPARAM)
//
//  Ŀ��: ���ڻ�����Ϣ��Ӧ������
//
void PageWndMsgHandler::PaintMsgHandler(HINSTANCE hInst, HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(hWnd, &ps);
	// TODO: �ڴ˴����ʹ�� hdc ���κλ�ͼ����...
	EndPaint(hWnd, &ps);

	return;
}

//
//  ����: SizeMsgHandler(HINSTANCE,HWND,UINT,WPARAM,LPARAM)
//
//  Ŀ��: ���ڴ�С�ı���Ϣ��Ӧ������
//
//  ע��:
//
//		  �ڴ˺����д����С�ı�ʱ�ı��Ӵ����С
//
void PageWndMsgHandler::SizeMsgHandler(HINSTANCE hInst, HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	RECT rect;
	::GetClientRect(hWnd, &rect);

	m_Width = rect.right - rect.left;
	m_Height = rect.bottom - rect.top;

	if (wParam == SIZE_MINIMIZED
		|| m_CefHandler == NULL
		|| m_CefHandler->GetBrowserHostWnd() == NULL)
		return;

	::SetWindowPos(m_CefHandler->GetBrowserHostWnd(),NULL,
		0, 0, m_Width, m_Height, SWP_SHOWWINDOW);

	return;
}

//
//  ����: DestroyMsgHandler(HINSTANCE,HWND,UINT,WPARAM,LPARAM)
//
//  Ŀ��: ����������Ϣ��Ӧ������
//
void PageWndMsgHandler::DestroyMsgHandler(HINSTANCE hInst, HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PostQuitMessage(0);

	return;
}


void PageWndMsgHandler::CreateBrowserPage(HWND hWnd)
{
	m_CefHandler = new CCefHandler();
	if(m_CefHandler != NULL)
	{
		RECT rect;
		::GetClientRect(hWnd, &rect);
		CefString cURL = m_URL;
		m_CefHandler->CreateBrowser(hWnd, rect, cURL);
	}
}