// Browser.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "Browser.h"
#include "MainWndMsgHandler.h"

#define MAX_LOADSTRING 100
#define WINDOWCLASSNAME L"MyBrowser"			// 主窗口类名


// 全局变量:
HINSTANCE hInst;                                // 当前实例
MainWndMsgHandler *msgHandler;					// 窗体消息处理实例


// 此代码模块中包含的函数的前向声明:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
BOOL				InitCef(HINSTANCE hInstance);


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 在此处放置代码。
	if(!InitCef(hInstance))
		return 0;

	msgHandler = new MainWndMsgHandler();
	
    MyRegisterClass(hInstance);

    // 执行应用程序初始化:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

	MSG nMsg = { 0 };
	while (GetMessage(&nMsg, NULL, 0, 0))
	{
		TranslateMessage(&nMsg);
		DispatchMessage(&nMsg);
	}
	CefShutdown();
	return 0;
}


//
//  函数: MyRegisterClass()
//
//  目标: 注册窗口类。
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_BROWSER));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)GetStockObject(BLACK_BRUSH); 
    wcex.lpszMenuName   = NULL;
    wcex.lpszClassName  = WINDOWCLASSNAME;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_BROWSER));

    return RegisterClassExW(&wcex);
}

//
//   函数: InitInstance(HINSTANCE, int)
//
//   目标: 保存实例句柄并创建主窗口
//
//   注释:
//
//        在此函数中，我们在全局变量中保存实例句柄并
//        创建和显示主程序窗口。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 将实例句柄存储在全局变量中

	//WS_CLIPCHILDREN 父窗口不对子窗口区域进行绘制 解决闪烁问题
   HWND hWnd = CreateWindowW(WINDOWCLASSNAME, NULL, WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  函数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目标: 处理主窗口的消息。
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	return msgHandler->MainWndProc(hInst,hWnd,message,wParam,lParam);
}

//
//  函数: InitCef(HINSTANCE)
//
//  目标: 初始化Cef。
//
//  注释:
//
//		  multi_threaded_message_loop设置多进程
//		  cache_path设置缓存路径
//
BOOL InitCef(HINSTANCE hInstance)
{
	CefMainArgs main_args(hInstance);
	CefRefPtr<CMyCefApp> app(new CMyCefApp);
	int exit_code = CefExecuteProcess(main_args, app.get(), NULL);
	if (exit_code >= 0) {
		return false;
	}
	CefSettings settings;
	settings.multi_threaded_message_loop = true;
	settings.ignore_certificate_errors = true;
	settings.log_severity = LOGSEVERITY_DISABLE;

	//设置缓存路径
	TCHAR szSpecialPath[MAX_PATH];
	memset(szSpecialPath, '\0', sizeof(szSpecialPath));
	if (FALSE != SHGetSpecialFolderPath(NULL, szSpecialPath, CSIDL_PROFILE, FALSE))
	{
		StringCbCat(szSpecialPath, sizeof(szSpecialPath), L"\\AppData\\Local\\Temp\\MyBrowser");
		CefString(&settings.cache_path).FromString(szSpecialPath, sizeof(szSpecialPath) / 2, true);
	}

	return CefInitialize(main_args, settings, app.get(), NULL);
}
