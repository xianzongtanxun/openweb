#include "stdafx.h"
#include "ui_sign_menu_dlg.h"
#include "MainFrame.h"
#include "ui_list_element.h"

CSignMenuDlg::CSignMenuDlg(CMainFrame* pMain)
	:m_pMainFrame(pMain)
	, m_pBtnSingle(nullptr)
	, m_pBtnMult(nullptr)
	, m_pBtnChink(nullptr)
	, m_pBtnKey(nullptr)
{
}

CSignMenuDlg::~CSignMenuDlg()
{

}

void CSignMenuDlg::InitWindow()
{
	m_pBtnSingle = static_cast<CTrayListElementUI*>(m_PaintManager.FindControl(_T("sign_menu_single")));
	m_pBtnMult   = static_cast<CTrayListElementUI*>(m_PaintManager.FindControl(_T("sign_menu_mult")));
	m_pBtnChink  = static_cast<CTrayListElementUI*>(m_PaintManager.FindControl(_T("sign_menu_chink")));
	m_pBtnKey    = static_cast<CTrayListElementUI*>(m_PaintManager.FindControl(_T("sign_menu_key")));

	ASSERT(m_pBtnSingle);
	ASSERT(m_pBtnMult);
	ASSERT(m_pBtnChink);
	ASSERT(m_pBtnKey);
}

HWND CSignMenuDlg::Create(HWND hwnd)
{
	HWND hWnd = __super::Create(hwnd, GetWindowClassName(), UI_WNDSTYLE_DIALOG, 0, 0, 0, 0);
	CenterWindow();
	m_shadowHelp.Init(GetHWND());
	
	return hWnd;
}

void CSignMenuDlg::AdjustPos(RECT& rc, POINT pt)
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


void CSignMenuDlg::ShowWnd()
{
	ShowWindow(true);
}

void CSignMenuDlg::Notify(TNotifyUI& msg)
{
	if (msg.sType == DUI_MSGTYPE_ITEMCLICK)
	{
		int k = 0;
		this->Close();
	}
}

//LRESULT CFileMenuDlg::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
//{
//	LRESULT lRes = 0;
//
//	return __super::HandleMessage(uMsg, wParam, lParam);
//}

LRESULT CSignMenuDlg::OnKillFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if (this->m_hWnd != (HWND)wParam)
	{
		PostMessage(WM_CLOSE);
	}
	return 0;
}

LRESULT CSignMenuDlg::OnKeyDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	PostMessage(WM_CLOSE);
	return 0;
}

CControlUI* CSignMenuDlg::CreateControl(LPCTSTR pstrClass) {
	if (_tcscmp(pstrClass, _T("TrayListElement")) == 0)
	{
		return new CTrayListElementUI();
	}
	return NULL;
}