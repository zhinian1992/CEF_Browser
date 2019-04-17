#include "stdafx.h"
#include "MainWndMsgHandler.h"
#include "SinglePageWnd.h"
#include <windowsx.h>
#include <dwmapi.h>
#include <map>
#include <vector>
#include <string>
#include <functional>
#include <uxtheme.h>
#include <vssym32.h>

using namespace std;

#pragma comment(lib,"UxTheme.lib")


typedef int (MainWndMsgHandler::*pMsgHandler)(HINSTANCE hInst,HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
map<int, pMsgHandler> m_FuncMap;		//��Ϣ����map
vector<SinglePageWnd *> m_PageList;		//ҳ�漯��


MainWndMsgHandler::MainWndMsgHandler()
{
	m_FuncMap.insert(make_pair(WM_CREATE,&MainWndMsgHandler::CreateMsgHandler));
	m_FuncMap.insert(make_pair(WM_NCCALCSIZE,&MainWndMsgHandler::NccalcsizeMsgHandler));
	m_FuncMap.insert(make_pair(WM_NCHITTEST,&MainWndMsgHandler::NCHitTestMsgHandler));
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
//  ����: MainWndProc(HINSTANCE, HWND, UINT, WPARAM, LPARAM)
//
//  Ŀ��: �����������������Ϣ��
//
LRESULT MainWndMsgHandler::MainWndProc(HINSTANCE hInst,HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	bool fCallDWP = true;
    BOOL fDwmEnabled = FALSE;
    LRESULT lRet = 0;
    HRESULT hr = S_OK;

    // Winproc worker for custom frame issues.
	hr = DwmIsCompositionEnabled(&fDwmEnabled);	
    if (SUCCEEDED(hr))
    {
        map<int, pMsgHandler>::iterator it;
		for(it = m_FuncMap.begin();it != m_FuncMap.end();++it)
		{
			if(it->first == message)
			{
				return (this->*(it->second))(hInst,hWnd,message,wParam,lParam);
			}
		}
		return DefWindowProc(hWnd, message, wParam, lParam);
    }
	else
	{
		MessageBox(hWnd,L"��ʼ��ʧ��->DwmIsCompositionEnabled:false",L"����",MB_OK);
		PostQuitMessage(0);

		return 0;
	}
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
//  ����: CommandMsgHandler(HINSTANCE, HWND, UINT, WPARAM, LPARAM)
//
//  Ŀ��: ���ڲ˵�������ؼ�������Ϣ��
//
int MainWndMsgHandler::CommandMsgHandler(HINSTANCE hInst, HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId = LOWORD(wParam);
    // �����˵�ѡ��:
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

	return 0;
}

//
//  ����: ActivateMsgHandler(HINSTANCE,HWND,UINT,WPARAM,LPARAM)
//
//  Ŀ��: ���ڼ�����Ϣ��Ӧ������
//
//  ע��:
//
//		  �ڴ˺�����frame extension��չ���ʵ��
//		  �����ʼ�������ʱ��ȷ��������չ��
//
int MainWndMsgHandler::ActivateMsgHandler(HINSTANCE hInst, HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
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

	return DefWindowProc(hWnd, message, wParam, lParam);
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
int MainWndMsgHandler::SizeMsgHandler(HINSTANCE hInst, HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	RECT rect;
	::GetWindowRect(hWnd,&rect);

	vector<SinglePageWnd *>::iterator iter;
	for(iter = m_PageList.begin();iter != m_PageList.end();++iter)
	{
		rect.right = rect.right - RIGHTEXTENDWIDTH;
		rect.left = rect.left + LEFTEXTENDWIDTH;
		rect.top = rect.top + TOPEXTENDWIDTH;
		rect.bottom = rect.bottom - BOTTOMEXTENDWIDTH;

		(*iter)->WndSizeChanged(rect, wParam);
	}

	return 0;
}

//
//  ����: PaintMsgHandler(HINSTANCE,HWND,UINT,WPARAM,LPARAM)
//
//  Ŀ��: ���ڻ�����Ϣ��Ӧ������
//
int MainWndMsgHandler::PaintMsgHandler(HINSTANCE hInst, HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(hWnd, &ps);
	// TODO: �ڴ˴����ʹ�� hdc ���κλ�ͼ����...
	PaintCustomCaption(hWnd,hdc);


	EndPaint(hWnd, &ps);

	return DefWindowProc(hWnd, message, wParam, lParam);
}

//
//  ����: CreateMsgHandler(HINSTANCE,HWND,UINT,WPARAM,LPARAM)
//
//  Ŀ��: �´��ڴ�����Ϣ��Ӧ������
//
int MainWndMsgHandler::CreateMsgHandler(HINSTANCE hInst, HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	RECT rcClient;
    GetWindowRect(hWnd, &rcClient);

    // Inform the application of the frame change.
    SetWindowPos(hWnd, 
                 NULL, 
                 rcClient.left, rcClient.top,
                 rcClient.right - rcClient.left,rcClient.bottom - rcClient.top,
                 SWP_FRAMECHANGED);


	CreateBrowserPage(hInst,hWnd);

	return 0;
}

//
//  ����: NCHitTestMsgHandler(HINSTANCE,HWND,UINT,WPARAM,LPARAM)
//
//  Ŀ��: ���ڿ�����в�����Ϣ��Ӧ������
//
int MainWndMsgHandler::NCHitTestMsgHandler(HINSTANCE hInst, HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	LRESULT lRet = 0;
	bool fCallDWP = !DwmDefWindowProc(hWnd, message, wParam, lParam, &lRet);
	if(lRet == 0)
	{
		lRet = HitTestNCA(hWnd,wParam,lParam);
		if (lRet != HTNOWHERE)
			fCallDWP = false;
	}
	if (!fCallDWP)
		return lRet;

	return DefWindowProc(hWnd, message, wParam, lParam);
}

//
//  ����: NccalcsizeMsgHandler(HINSTANCE,HWND,UINT,WPARAM,LPARAM)
//
//  Ŀ��: �´��ڴ����Զ����ܺ�����
//
int MainWndMsgHandler::NccalcsizeMsgHandler(HINSTANCE hInst, HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if(wParam == TRUE)
	{
		NCCALCSIZE_PARAMS *pncsp = reinterpret_cast<NCCALCSIZE_PARAMS*>(lParam);

		pncsp->rgrc[0].left   = pncsp->rgrc[0].left   + 0;
        pncsp->rgrc[0].top    = pncsp->rgrc[0].top    + 0;
        pncsp->rgrc[0].right  = pncsp->rgrc[0].right  - 0;
        pncsp->rgrc[0].bottom = pncsp->rgrc[0].bottom - 0;
	}

	return 0;
}

//
//  ����: DestroyMsgHandler(HINSTANCE,HWND,UINT,WPARAM,LPARAM)
//
//  Ŀ��: ����������Ϣ��Ӧ������
//
int MainWndMsgHandler::DestroyMsgHandler(HINSTANCE hInst, HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PostQuitMessage(0);

	return 0;
}

//
//  ����: CreateBrowserPage(HINSTANCE,HWND)
//
//  Ŀ��: �����ҳ�洴����Ϣ��Ӧ������
//
void MainWndMsgHandler::CreateBrowserPage(HINSTANCE hInst,HWND hWnd)
{
	RECT rect;
	::GetWindowRect(hWnd,&rect);

	rect.right = rect.right - RIGHTEXTENDWIDTH;
	rect.left = rect.left + LEFTEXTENDWIDTH;
	rect.top = rect.top + TOPEXTENDWIDTH;
	rect.bottom = rect.bottom - BOTTOMEXTENDWIDTH;

	const string sURL = "www.baidu.com";
	SinglePageWnd *pageWnd = new SinglePageWnd(hInst,hWnd,
		rect.right - rect.left,rect.bottom - rect.top,sURL);

	m_PageList.push_back(pageWnd);

	return;
}

//
//  ����: PaintCustomCaption(HWND,HDC)
//
//  Ŀ��: ��չ����ϻ��Ʊ��⡣
//
void MainWndMsgHandler::PaintCustomCaption(HWND hWnd, HDC hdc)
{
	RECT rcClient;
    GetClientRect(hWnd, &rcClient);

    HTHEME hTheme = OpenThemeData(NULL, L"CompositedWindow::Window");
    if (hTheme)
    {
        HDC hdcPaint = CreateCompatibleDC(hdc);
        if (hdcPaint)
        {
            int cx = rcClient.right - rcClient.left;
            int cy = rcClient.bottom - rcClient.top;

            // Define the BITMAPINFO structure used to draw text.
            // Note that biHeight is negative. This is done because
            // DrawThemeTextEx() needs the bitmap to be in top-to-bottom
            // order.
            BITMAPINFO dib = { 0 };
            dib.bmiHeader.biSize            = sizeof(BITMAPINFOHEADER);
            dib.bmiHeader.biWidth           = cx;
            dib.bmiHeader.biHeight          = -cy;
            dib.bmiHeader.biPlanes          = 1;
            dib.bmiHeader.biBitCount        = 32;
            dib.bmiHeader.biCompression     = BI_RGB;

            HBITMAP hbm = CreateDIBSection(hdc, &dib, DIB_RGB_COLORS, NULL, NULL, 0);
            if (hbm)
            {
                HBITMAP hbmOld = (HBITMAP)SelectObject(hdcPaint, hbm);

                // Setup the theme drawing options.
                DTTOPTS DttOpts = {sizeof(DTTOPTS)};
                DttOpts.dwFlags = DTT_COMPOSITED | DTT_GLOWSIZE | DTT_TEXTCOLOR ;
				DttOpts.crText = RGB(0x00,0x00,0xCD);
                DttOpts.iGlowSize = 15;

                // Select a font.
                LOGFONT lgFont;
                HFONT hFontOld = NULL;
                if (SUCCEEDED(GetThemeSysFont(hTheme, TMT_CAPTIONFONT, &lgFont)))
                {
                    HFONT hFont = CreateFontIndirect(&lgFont);
                    hFontOld = (HFONT) SelectObject(hdcPaint, hFont);
                }

                // Draw the title.
                RECT rcPaint = rcClient;
                rcPaint.top += 8;
                rcPaint.right -= 125;
                rcPaint.left += 8;
                rcPaint.bottom = 50;
                DrawThemeTextEx(hTheme, 
                                hdcPaint, 
                                0, 0, 
                                L"browser", 
                                -1, 
                                DT_LEFT | DT_WORD_ELLIPSIS | DT_SINGLELINE | DT_VCENTER | DT_NOPREFIX, 
                                &rcPaint, 
                                &DttOpts);

                // Blit text to the frame.
                BitBlt(hdc, 0, 0, cx, cy, hdcPaint, 0, 0, SRCCOPY);

                SelectObject(hdcPaint, hbmOld);
                if (hFontOld)
                {
                    SelectObject(hdcPaint, hFontOld);
                }
                DeleteObject(hbm);
            }
            DeleteDC(hdcPaint);
        }
        CloseThemeData(hTheme);
    }
}

//
//  ����: HitTestNCA(HWND,WPARAM,LPARAM)
//
//  Ŀ��: ʵ�ֵ����ܽ����ƶ��ʹ�С�ı䡣
//
LRESULT MainWndMsgHandler::HitTestNCA(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	// Get the point coordinates for the hit test.
    POINT ptMouse = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)};

    // Get the window rectangle.
    RECT rcWindow;
    GetWindowRect(hWnd, &rcWindow);

    // Get the frame rectangle, adjusted for the style without a caption.
    RECT rcFrame = { 0 };
    AdjustWindowRectEx(&rcFrame, WS_OVERLAPPEDWINDOW & ~WS_CAPTION, FALSE, NULL);

    // Determine if the hit test is for resizing. Default middle (1,1).
    USHORT uRow = 1;
    USHORT uCol = 1;
    bool fOnResizeBorder = false;

    // Determine if the point is at the top or bottom of the window.
    if (ptMouse.y >= rcWindow.top && ptMouse.y < rcWindow.top + TOPEXTENDWIDTH)
    {
        fOnResizeBorder = (ptMouse.y < (rcWindow.top - rcFrame.top));
        uRow = 0;
    }
    else if (ptMouse.y < rcWindow.bottom && ptMouse.y >= rcWindow.bottom - BOTTOMEXTENDWIDTH)
    {
        uRow = 2;
    }

    // Determine if the point is at the left or right of the window.
    if (ptMouse.x >= rcWindow.left && ptMouse.x < rcWindow.left + LEFTEXTENDWIDTH)
    {
        uCol = 0; // left side
    }
    else if (ptMouse.x < rcWindow.right && ptMouse.x >= rcWindow.right - RIGHTEXTENDWIDTH)
    {
        uCol = 2; // right side
    }

    // Hit test (HTTOPLEFT, ... HTBOTTOMRIGHT)
    LRESULT hitTests[3][3] = 
    {
        { HTTOPLEFT,    fOnResizeBorder ? HTTOP : HTCAPTION,    HTTOPRIGHT },
        { HTLEFT,       HTNOWHERE,     HTRIGHT },
        { HTBOTTOMLEFT, HTBOTTOM, HTBOTTOMRIGHT },
    };

    return hitTests[uRow][uCol];
}