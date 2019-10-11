// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             //  从 Windows 头文件中排除极少使用的信息
// Windows 头文件:
#include <windows.h>

//#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS
//#include <atlbase.h>
//#include <atlstr.h>

#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <vector>
#include <list>
#include <mutex>
#include <map>
#include <string.h>
#include <Shlwapi.h>
#include <ShlObj.h>
#include <atlconv.h>
#include <algorithm>
#include <wininet.h>
#include <shellapi.h>
#include <thread>
#include <commdlg.h>
#include "../Duilib/UIlib.h"
#include "json/json.h"
using namespace std;
using namespace DuiLib;
#undef GetFirstChild
#undef GetNextSibling

#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "wldap32.lib")
#pragma comment(lib, "Urlmon.lib")
#pragma comment(lib, "shlwapi.lib")
#pragma comment(lib,"Wininet.lib")

#define WM_GRAPHNOTIFY  (WM_USER+2000)
#define WM_MSG_REFRESH_SHADOW (WM_USER + 2001)
#define WM_MSG_SHOW     (WM_USER + 2002)
#define WM_COPY_STOP    (WM_USER + 2003)
#define WM_MSG_NAVIGATION (WM_USER + 2004)

// TODO: 在此处引用程序需要的其他头文件
