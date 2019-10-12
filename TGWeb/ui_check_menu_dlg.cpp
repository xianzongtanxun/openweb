#include "stdafx.h"
#include "ui_check_menu_dlg.h"
#include "MainFrame.h"
#include "ui_list_element.h"

const int FLAG_WIDTT = 172;
CCheckMenuDlg::CCheckMenuDlg(CMainFrame* pMain)
	:m_pMainFrame(pMain)
	, m_pBtnVerify(nullptr)
	, m_pBtnScan(nullptr)
{
}

CCheckMenuDlg::~CCheckMenuDlg()
{

}

void CCheckMenuDlg::InitWindow()
{
	m_pBtnVerify = static_cast<CTrayListElementUI*>(m_PaintManager.FindControl(_T("check_menu_verify")));
	m_pBtnScan   = static_cast<CTrayListElementUI*>(m_PaintManager.FindControl(_T("check_menu_scan")));

	ASSERT(m_pBtnVerify);
	ASSERT(m_pBtnScan);
}

HWND CCheckMenuDlg::Create(HWND hwnd)
{
	HWND hWnd = __super::Create(hwnd, GetWindowClassName(), UI_WNDSTYLE_DIALOG, 0, 0, 0, 0);
	CenterWindow();
	m_shadowHelp.Init(GetHWND());
	
	return hWnd;
}

void CCheckMenuDlg::AdjustPos(RECT& rc, POINT pt)
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


void CCheckMenuDlg::ShowWnd()
{
	ShowWindow(true);
}

void CCheckMenuDlg::Notify(TNotifyUI& msg)
{
	if (msg.sType == DUI_MSGTYPE_ITEMCLICK)
	{
		int k = 0;
		this->Close();
	}
}

LRESULT CCheckMenuDlg::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT lRes = 0;
	return __super::HandleMessage(uMsg, wParam, lParam);
}

LRESULT CCheckMenuDlg::OnKillFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if (this->m_hWnd != (HWND)wParam)
	{
		PostMessage(WM_CLOSE);
	}
	return 0;
}

LRESULT CCheckMenuDlg::OnKeyDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	PostMessage(WM_CLOSE);
	return 0;
}

CControlUI* CCheckMenuDlg::CreateControl(LPCTSTR pstrClass) {
	if (_tcscmp(pstrClass, _T("TrayListElement")) == 0)
	{
		return new CTrayListElementUI();
	}
	return NULL;
}