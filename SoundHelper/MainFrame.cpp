#include "stdafx.h"
#include <shellapi.h>
#include <WinInet.h>
#include <WinUser.h>
#include "resource.h"
#include "MainFrame.h"
#include "Utils\UIDelegate.h"
#include "include\cef_app.h"
#include "simple_app.h"
#pragma comment(lib, "wininet.lib")

#define WM_GRAPHNOTIFY  (WM_USER+20)
///////////////////////////
CMainFrame::CMainFrame():  
	m_edtFilePath(nullptr),       
	m_pBtnBackup(nullptr),
	m_pBtnForword(nullptr),
	m_pBtnGo(nullptr),
	m_pVlyFrame(nullptr),
	m_pBtnClose(nullptr) 
{
	//m_handler = nullptr;
}

CMainFrame::~CMainFrame()
{
	
}

void CMainFrame::InitWindow()
{
	m_pBtnClose   = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("closebtn")));
	m_edtFilePath = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("edt_path")));
	m_pBtnBackup  = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btn_backup")));
	m_pBtnForword = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btn_forword")));
	m_pBtnGo      = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btn_go")));
	m_pVlyFrame   = static_cast<CVerticalLayoutUI*>(m_PaintManager.FindControl(_T("vly_frame")));
	
	ASSERT(m_pBtnClose);
	ASSERT(m_edtFilePath);
	ASSERT(m_pBtnBackup);
	ASSERT(m_pBtnForword);
	ASSERT(m_pBtnGo);
	ASSERT(m_pBtnGo);

	CefWindowInfo info;
	RECT rc;
	GetWindowRect(GetHWND(), &rc);
	rc.left = rc.left + 10;
	rc.right = rc.right - 10;
	rc.top = rc.top + 80;
	rc.bottom = rc.bottom - 10;

	info.SetAsChild(GetHWND(), rc);
	//info.SetAsPopup(NULL, _T("MainSound"));
	CefBrowserSettings settings;
	m_handler = new SimpleHandler();
	//m_handler->SetMainWnd(GetHWND());
	m_handler->m_ClientRc = rc;
	settings.default_encoding.str = _T("GB2312");
	settings.default_encoding.length= wcslen(_T("GB2312"));
	//CefString strUrl = "http://www.google.com";
	wstring strUrl = _T("https://123.sogou.com/");
	//strUrl = "http://www.google.com";
	CefBrowserHost::CreateBrowser(info, m_handler, strUrl, settings, NULL);
	//支持文件拖拽功能
	//::DragAcceptFiles(GetHWND(),true);
}

HWND CMainFrame::Create(HWND hwndParent, LPCTSTR pstrName)
{
	HWND hWnd = __super::Create(hwndParent, GetWindowClassName(), UI_WNDSTYLE_DIALOG, 0,0,0,0,0);
	m_ShaowHelp.Init(hWnd);
	ShowWindow(SW_HIDE);
	return hWnd;
}

DuiLib::CDuiString CMainFrame::GetSkinFile()
{
	return _T("MainSound.xml");
}

UINT CMainFrame::GetClassStyle() const
{
	return UI_CLASSSTYLE_DIALOG;
}

DuiLib::CDuiString CMainFrame::GetSkinFolder()
{
	return _T("skin");
}

UILIB_RESOURCETYPE CMainFrame::GetResourceType() const
{
	return UILIB_FILE;
}

LRESULT CMainFrame::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT lRes = 0;
	BOOL bHandled = FALSE;
	switch (uMsg)
	{
	case WM_MSG_SHOW:
		{
			::ShowWindow(GetHWND(), SW_SHOWNOACTIVATE);
			m_ShaowHelp.Init(GetHWND());
			break;
		}
	default:
		break;
	}
	if (bHandled) return lRes;
	return __super::HandleMessage(uMsg, wParam, lParam);
}

void CMainFrame::Notify(TNotifyUI& msg)
{
	if(msg.sType == DUI_MSGTYPE_CLICK)
	{
		if (msg.pSender == m_pBtnClose)
		{
			Close(IDCANCEL);
			CefShutdown();
			PostQuitMessage(WM_QUIT);
			//::TerminateProcess(::GetCurrentProcess(), 0);
			return;
		}
		else if (msg.pSender == m_pBtnBackup)
		{
			return;
		}
		else if (msg.pSender == m_pBtnGo)
		{
			CDuiString strUrl = m_edtFilePath->GetText();
			m_handler->GetBrowser()->GetMainFrame()->LoadURL(strUrl.GetData());

			
			return;
		}
		else if (msg.pSender == m_pBtnForword)
		{
			//Stop();
			return;
		}
		else if (msg.pSender == m_pBtnChangePath)
		{
			CDuiString strUrl = m_edtFilePath->GetText();
			m_handler->GetBrowser()->GetMainFrame()->LoadURL(strUrl.GetData());
			return;
			CefRefPtr<CefProcessMessage> msg = CefProcessMessage::Create("callback_func");
			CefRefPtr<CefListValue> args = msg->GetArgumentList();
			args->SetString(0, "call js call back function");
			args->SetInt(1, 10);
			m_handler->GetBrowser()->SendProcessMessage(PID_RENDERER, msg);
		}
	}
}