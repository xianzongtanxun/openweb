#include "stdafx.h"
#include "ui_view_menu_dlg.h"
#include "MainFrame.h"
#include "ui_list_element.h"

CViewMenuDlg::CViewMenuDlg(CMainFrame* pMain)
	:m_pMainFrame(pMain)
	, m_pBtnNext(nullptr)
	, m_pBtnPercent(nullptr)
	, m_pBtnCallJS(nullptr)
{

}	 

CViewMenuDlg::~CViewMenuDlg()
{

}

void CViewMenuDlg::InitWindow()
{
	m_pBtnBack     = static_cast<CTrayListElementUI*>(m_PaintManager.FindControl(_T("view_menu_back")));
	m_pBtnNext     = static_cast<CTrayListElementUI*>(m_PaintManager.FindControl(_T("view_menu_next")));
	m_pBtnPercent  = static_cast<CTrayListElementUI*>(m_PaintManager.FindControl(_T("view_menu_percent")));
	m_pBtnCallJS   = static_cast<CTrayListElementUI*>(m_PaintManager.FindControl(_T("view_menu_test")));
	

	ASSERT(m_pBtnBack);
	ASSERT(m_pBtnNext);
	ASSERT(m_pBtnPercent);
	ASSERT(m_pBtnCallJS);
}

HWND CViewMenuDlg::Create(HWND hwnd)
{
	HWND hWnd = __super::Create(hwnd, GetWindowClassName(), UI_WNDSTYLE_DIALOG, 0, 0, 0, 0);
	CenterWindow();
	m_shadowHelp.Init(GetHWND());
	
	return hWnd;
}

void CViewMenuDlg::AdjustPos(RECT& rc, POINT pt)
{
	m_ptPos = pt;
	MONITORINFO oMonitor = { 0 };
	oMonitor.cbSize = sizeof(oMonitor);
	::GetMonitorInfo(::MonitorFromWindow(*this,
		MONITOR_DEFAULTTOPRIMARY), &oMonitor);
	CDuiRect rcWork, rcWnd;
	rcWork = oMonitor.rcWork;
	::GetWindowRect(this->m_hWnd, &rcWnd);

	int nWidth = rcWnd.GetWidth();
	int nHeight = rcWnd.GetHeight();
	if (m_ptPos.x + nWidth < rcWork.right)
	{
		rcWnd.left = m_ptPos.x;
		rcWnd.right = m_ptPos.x + nWidth;
	}
	else
	{
		rcWnd.left = m_ptPos.x - nWidth;
		rcWnd.right = m_ptPos.x;
	}
	if (m_ptPos.y + nHeight < rcWork.bottom)
	{
		rcWnd.top = m_ptPos.y;
		rcWnd.bottom = m_ptPos.y + nHeight;
	}
	else
	{
		rcWnd.top = m_ptPos.y - nHeight;
		rcWnd.bottom = m_ptPos.y;
	}
	::SetWindowPos(this->m_hWnd, NULL, rcWnd.left,
		rcWnd.top, rcWnd.GetWidth(), rcWnd.GetHeight(),
		SWP_NOZORDER | SWP_NOSIZE | SWP_NOACTIVATE);
}


void CViewMenuDlg::ShowWnd()
{
	ShowWindow(true);
}

void CViewMenuDlg::Notify(TNotifyUI& msg)
{
	if (msg.sType == DUI_MSGTYPE_ITEMCLICK)
	{
		if (msg.pSender == m_pBtnCallJS)
		{
			::PostMessage(m_pMainFrame->GetHWND(), WM_MSG_CALL_JS, NULL, NULL);
			int k = 0;
			
		}
		this->Close();
	}
}

LRESULT CViewMenuDlg::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT lRes = 0;

	return __super::HandleMessage(uMsg, wParam, lParam);
}

LRESULT CViewMenuDlg::OnKillFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if (this->m_hWnd != (HWND)wParam)
	{
		PostMessage(WM_CLOSE);
	}
	return 0;
}

LRESULT CViewMenuDlg::OnKeyDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	PostMessage(WM_CLOSE);
	return 0;
}

CControlUI* CViewMenuDlg::CreateControl(LPCTSTR pstrClass) {
	if (_tcscmp(pstrClass, _T("TrayListElement")) == 0)
	{
		return new CTrayListElementUI();
	}
	return NULL;
}