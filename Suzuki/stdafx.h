// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             //  �� Windows ͷ�ļ����ų�����ʹ�õ���Ϣ
// Windows ͷ�ļ�:
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
#include <wininet.h>

#include "../Duilib/UIlib.h"
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

// TODO: �ڴ˴����ó�����Ҫ������ͷ�ļ�
