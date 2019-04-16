// stdafx.h: 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 项目特定的包含文件
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // 从 Windows 头文件中排除极少使用的内容
// Windows 头文件
#include <windows.h>

// C 运行时头文件
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>


// 在此处引用程序需要的其他标头
#include "resource.h"

#include <strsafe.h>
#include <shlobj.h>
#include <assert.h>

/*cef headers*/
#include "include\cef_app.h"
#include "cef\CefHandler.h"
#include "cef\MyCefApp.h"

//include lib files 
#ifdef _DEBUG
#pragma comment(lib, "lib/Debug/libcef.lib")
#pragma comment(lib, "lib/Debug/libcef_dll_wrapper.lib")
#else
#pragma comment(lib, "lib/Release/libcef.lib")
#pragma comment(lib, "lib/Release/libcef_dll_wrapper.lib")
#endif



#define LEFTEXTENDWIDTH		8
#define RIGHTEXTENDWIDTH	8
#define BOTTOMEXTENDWIDTH	8
#define TOPEXTENDWIDTH		50  //客户区上限高度