#include "stdafx.h"
#include "MainFrame.h"
#include <commdlg.h>
#include <shellapi.h>
#include <WinUser.h>
#include "Utils\UIDelegate.h"
#include "include\cef_app.h"
#include "DuiCefApp.h"
//#include "DuiMsgHandler.h"

///////////////////////////
CMainFrame::CMainFrame():
	m_pLblTitle(nullptr),
	m_edtUrl(nullptr),   
	m_pBtnBackup(nullptr),      
	m_pBtnForword(nullptr),     
	m_pBtnGo(nullptr),
	m_pBtnRestore(nullptr),
	m_pBtnMax(nullptr),
	m_pBtnClose(nullptr)
{

}

CMainFrame::~CMainFrame()
{
	
}

void CMainFrame::InitWindow()
{
	m_pLblTitle   = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("lbl_title")));
	m_edtUrl      = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("edt_path")));
	m_pBtnBackup  = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btn_backup")));
	m_pBtnForword = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btn_forword")));
	m_pBtnGo      = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btn_go")));
	m_pBtnRestore = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btnrestore")));
	m_pBtnMax     = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btnmax")));
	m_pBtnClose   = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btnclose")));
	m_pContainer  = static_cast<CContainerUI*>(m_PaintManager.FindControl(_T("container")));
	//支持文件拖拽功能

	ASSERT(m_pLblTitle);
	ASSERT(m_edtUrl);
	ASSERT(m_pBtnBackup);
	ASSERT(m_pBtnForword);
	ASSERT(m_pBtnGo);
	ASSERT(m_pBtnRestore);
	ASSERT(m_pBtnMax);
	ASSERT(m_pBtnClose);
	ASSERT(m_pContainer);

	m_pBtnRestore->SetVisible(false);
	m_pBtnMax->SetVisible(false);
	::DragAcceptFiles(GetHWND(),true);
	//OnWndSizeMax(true);
	CefWindowInfo windows_info;
	RECT rc;
	::GetClientRect(GetHWND(),&rc);
	rc.top = rc.top + 70;

	RECT rc2;
	rc2 = m_pContainer->GetPos();
	windows_info.SetAsChild(GetHWND(),rc);
	m_handler = new CDuiMsgHandler(this);
	CefBrowserSettings browser_settings;
	//browser_settings.default_encoding.str = _T("GB2312");
	//browser_settings.default_encoding.length = wcslen(_T("GB2312"));
	std::string url = "https://www.hao123.com/";
	//最大化
	
	//::SendMessageA(GetHWND(), WM_SYSCOMMAND, SC_MAXIMIZE, 0);//最大化
	bool bRet = CefBrowserHost::CreateBrowser(windows_info,m_handler,url,browser_settings,NULL);
	int k = 0;
}

HWND CMainFrame::Create(HWND hwndParent, LPCTSTR pstrName)
{
	//UI_WNDSTYLE_DIALOG
	HWND hwnd = __super::Create(hwndParent, pstrName, UI_WNDSTYLE_DIALOG | WS_MAXIMIZEBOX, 0, 0, 0, 0, 0);
	m_ShaowHelp.Init(hwnd);
	return hwnd;
}

void CMainFrame::SetCaption(wstring sCaption)
{
	if (m_pLblTitle)
	{
		m_pLblTitle->SetText(sCaption.c_str());
	}
}

DuiLib::CDuiString CMainFrame::GetSkinFile()
{
	return _T("mainframecef.xml");
}

UINT CMainFrame::GetClassStyle() const
{
	return UI_CLASSSTYLE_DIALOG;
}

void CMainFrame::OnFinalMessage(HWND hWnd)
{
	__super::OnFinalMessage(hWnd);
}

DuiLib::CDuiString CMainFrame::GetSkinFolder()
{
	return _T("skin");
}

UILIB_RESOURCETYPE CMainFrame::GetResourceType() const
{
	return UILIB_FILE;
}

LRESULT CMainFrame::MessageHandler(UINT uMsg, WPARAM wParam, LPARAM /*lParam*/, bool& /*bHandled*/)
{
	if (uMsg == WM_KEYDOWN){
		if (wParam == VK_RETURN || wParam == VK_ESCAPE){
			//pWindow->put_FullScreenMode(OAFALSE);
		}
	}
	return true;
}

LRESULT CMainFrame::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT lRes = 0;
	BOOL bHandled = FALSE;
	switch (uMsg)
	{
	case WM_TIMER:
		{
		}
		break;
	case WM_GRAPHNOTIFY:
		{
			
		}
		break;
	case WM_MSG_SHOW:
		{
			//::ShowWindow(GetHWND(), SW_SHOWNOACTIVATE);
			::ShowWindow(GetHWND(), SW_SHOWMAXIMIZED);
			::PostMessage(GetHWND(), WM_MSG_REFRESH_SHADOW, 0, 0);
		}
		break;
	case WM_MSG_REFRESH_SHADOW:
		{
			m_ShaowHelp.ReFlush(GetHWND());
		}
		break;
	case WM_DROPFILES:
		{
			//拖拽消息
			LPTSTR pFilePathName = (LPTSTR)malloc(500);
			HDROP hDropInfo = (HDROP)wParam;
			::DragQueryFile(hDropInfo,0,pFilePathName,500);
			m_edtUrl->SetText(pFilePathName);
			m_handler->GetBrowser()->GetMainFrame()->LoadURL(pFilePathName);
			::DragFinish(hDropInfo);
			free(pFilePathName);
		}
		break;
	case WM_SIZE:
		{
			return OnSize(uMsg, wParam, lParam, bHandled);
		}
	default:
		break;
	}
	if (bHandled) return lRes;
	return __super::HandleMessage(uMsg, wParam, lParam);
}

LRESULT CMainFrame::OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	//bHandled = true;
	RECT rc;
	::GetClientRect(GetHWND(), &rc);
	rc.top = rc.top + 70;

	RECT rc2;
	if (m_pContainer){
		rc2 = m_pContainer->GetPos();
	}
	
	//if (!m_handler){
	//	return false;
	//}
	//CefRefPtr<CefBrowser> browser = m_handler->GetBrowser();
	//if (browser.get())
	//{
	//	HWND hwnd_ = browser->GetHost()->GetWindowHandle();
	//	if (hwnd_)
	//	{
	//		RECT rc;
	//		//::GetClientRect(GetHWND(), &rc);
	//		rc = m_pContainer->GetPos();
	//		rc.top = rc.top;
	//		::MoveWindow(hwnd_, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, TRUE);
	//	}
	//}
	return true;
}

void CMainFrame::Notify(TNotifyUI& msg)
{
	if(msg.sType == DUI_MSGTYPE_CLICK)
	{
		if (msg.pSender == m_pBtnBackup)
		{
			m_handler->GetBrowser()->GoBack();
			return;
		}
		else if (msg.pSender == m_pBtnForword)
		{
			m_handler->GetBrowser()->GoForward();
			return;
		}
		else if (msg.pSender == m_pBtnGo)
		{
			//Stop();
			wstring str = m_edtUrl->GetText();
			m_handler->GetBrowser()->GetMainFrame()->LoadURL(str);
			return;
		}
		else if (msg.pSender == m_pBtnRestore)
		{
			//EnterFullScreen(false);
			//::SendMessageA(GetHWND(), WM_SYSCOMMAND, SC_RESTORE, 0);//还原
			//OnWndSizeMax(false);
		}
		else if (msg.pSender == m_pBtnMax)
		{
			//EnterFullScreen(true);
			//::SendMessageA(GetHWND(), WM_SYSCOMMAND, SC_MAXIMIZE, 0);//最大化
			//OnWndSizeMax(true);
		}
		else if (msg.pSender == m_pBtnClose)
		{
			Close(IDCANCEL);
			CefShutdown();
			::PostQuitMessage(0);
			return;
		}
	}
}

void CMainFrame::OnWndSizeMax(bool bMax)
{
	//m_pBtnMax->SetVisible(!bMax);
	//m_pBtnRestore->SetVisible(bMax);
	static RECT rc;//记录下还原时的位置
	if (bMax)
	{
		GetWindowRect(GetHWND(), &rc);
		::SendMessageA(GetHWND(), WM_SYSCOMMAND, SC_MAXIMIZE, 0);//最大化
	}
	else
	{
		//ResizeClient(1024, 720);
		//SetWindowPos(GetHWND(), HWND_NOTOPMOST, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, SWP_SHOWWINDOW);
		//::SendMessageA(GetHWND(), WM_SYSCOMMAND, SC_RESTORE, 0);//还原
		//::PostMessage(GetHWND(), WM_MSG_REFRESH_SHADOW, 0, 0);
	}
}

void CMainFrame::EnterFullScreen(bool bFull)
{
	static LONG oldStyle;//记录下原窗口style
	static RECT rc;//记录下还原时的位置
	if (bFull)
	{
		//SendMessageA(GetHWND(), WM_SYSCOMMAND, SC_RESTORE, 0);
		GetWindowRect(GetHWND(), &rc);
		//oldStyle = SetWindowLong(GetHWND(), GWL_STYLE, WS_POPUP);
		int w = GetSystemMetrics(SM_CXSCREEN);
		int h = GetSystemMetrics(SM_CYSCREEN);
		SetWindowPos(GetHWND(), HWND_TOPMOST, 0, 0, w, h, SWP_SHOWWINDOW);
		::PostMessage(GetHWND(), WM_MSG_REFRESH_SHADOW, 0, 0);
	}
	else
	{
		//SetWindowLong(GetHWND(), GWL_STYLE, oldStyle);
		SetWindowPos(GetHWND(), HWND_NOTOPMOST, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, SWP_SHOWWINDOW);
		::PostMessage(GetHWND(), WM_MSG_REFRESH_SHADOW, 0, 0);
	}
}