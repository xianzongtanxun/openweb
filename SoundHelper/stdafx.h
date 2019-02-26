#pragma once
#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // 从 Windows 头中排除极少使用的资料
// Windows 头文件: 
#include <windows.h>

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
#include "../Duilib/UIlib.h"
using namespace std;
using namespace DuiLib;
#undef GetFirstChild
#undef GetNextSibling
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "wldap32.lib")
#pragma comment(lib, "Urlmon.lib")
#pragma comment(lib, "shlwapi.lib")

