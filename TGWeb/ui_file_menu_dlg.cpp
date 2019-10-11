#include "stdafx.h"
#include "ui_file_menu_dlg.h"
#include "MainFrame.h"
#include "ui_list_element.h"

const int FLAG_WIDTT = 172;
CFileMenuDlg::CFileMenuDlg(CMainFrame* pMain)
{
	m_pMainFrame = pMain;
	m_pBtnOpen = nullptr;
	m_pBtnSave = nullptr;
	m_pBtnSaveAs = nullptr;
	m_pBtnPrint = nullptr;
}

CFileMenuDlg::~CFileMenuDlg()
{

}

void CFileMenuDlg::InitWindow()
{
	m_pBtnOpen   = static_cast<CTrayListElementUI*>(m_PaintManager.FindControl(_T("file_menu_open")));
	m_pBtnSave   = static_cast<CTrayListElementUI*>(m_PaintManager.FindControl(_T("file_menu_save")));
	m_pBtnSaveAs = static_cast<CTrayListElementUI*>(m_PaintManager.FindControl(_T("file_menu_saveas")));
	m_pBtnPrint  = static_cast<CTrayListElementUI*>(m_PaintManager.FindControl(_T("file_menu_print")));

	ASSERT(m_pBtnOpen);
	ASSERT(m_pBtnSave);
	ASSERT(m_pBtnSaveAs);
	ASSERT(m_pBtnPrint);
}

HWND CFileMenuDlg::Create(HWND hwnd)
{
	HWND hWnd = __super::Create(hwnd, GetWindowClassName(), UI_WNDSTYLE_DIALOG, 0, 0, 0, 0);
	CenterWindow();
	m_shadowHelp.Init(GetHWND());
	
	return hWnd;
}

void CFileMenuDlg::AdjustPos(RECT& rc, POINT pt)
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


void CFileMenuDlg::ShowWnd()
{
	ShowWindow(true);
}

void CFileMenuDlg::Notify(TNotifyUI& msg)
{
	if (msg.sType == DUI_MSGTYPE_CLICK)
	{
		if (msg.pSender == m_pBtnOpen)
		{
			std::thread th(&CMainFrame::OpenLocalPdfFile, m_pMainFrame);
			th.detach();
			return;
		}
	}
}

//LRESULT CFileMenuDlg::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
//{
//	LRESULT lRes = 0;
//
//	return __super::HandleMessage(uMsg, wParam, lParam);
//}

LRESULT CFileMenuDlg::OnKillFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if (this->m_hWnd != (HWND)wParam)
	{
		PostMessage(WM_CLOSE);
	}
	return 0;
}

LRESULT CFileMenuDlg::OnKeyDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	//PostMessage(WM_CLOSE);
	return 0;
}

CControlUI* CFileMenuDlg::CreateControl(LPCTSTR pstrClass) {
	if (_tcscmp(pstrClass, _T("TrayListElement")) == 0)
	{
		return new CTrayListElementUI();
	}
	return NULL;
}