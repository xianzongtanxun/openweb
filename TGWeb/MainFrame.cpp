#include "stdafx.h"
#include <shellapi.h>
#include <WinUser.h>
#include <thread>
//#include <comutil.h>
//comsuppw.lib
#include <comdef.h>
#include "MainFrame.h"
#include "Utils\UIDelegate.h"
#include "include\cef_app.h"
#include "SimpleCefApp.h"
#include "include/cef_request_context.h"
#include "CCefRequestContextHandlerEx.h"
#include "include/base/cef_bind_helpers.h"
#include "CCookieVistor.h"
#include "ui_file_menu_dlg.h"
#include "ui_view_menu_dlg.h"
#include "ui_check_menu_dlg.h"
#include "ui_sign_menu_dlg.h"
///////////////////////////
CMainFrame::CMainFrame(const string& sInitUrl, const string& sInitParam) :
	m_pLblTitle(nullptr),   
	m_pBtnMin(nullptr),
	m_edtUrl(nullptr),
	m_pBtnClose(nullptr),
	m_pBtnBackUp(nullptr),
	m_pBtnForward(nullptr),
	m_pBtnGo(nullptr),
	m_pBtnTitleFile(nullptr),
	m_pBtnHost(nullptr),
	m_pBtnHelp(nullptr),
	m_pBtnMenuFile(nullptr),
	m_pBtnMenuView(nullptr),
	m_pBtnMenuSign(nullptr),
	m_pBtnMenuCheck(nullptr),
	m_pFileMenuDlg(nullptr),
	m_sInitUrl(sInitUrl),
	m_pInitParam(sInitParam),
	m_width(0),
	m_height(0),
	m_bShowMin(false)
{
	

}

CMainFrame::~CMainFrame()
{
	
}

void CMainFrame::InitWindow()
{
	m_pLblTitle   = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("lbl_title")));
	m_pBtnMin     = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btnmin")));
	m_pBtnClose   = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btnclose")));
	m_pContainer  = static_cast<CContainerUI*>(m_PaintManager.FindControl(_T("container")));
	m_edtUrl      = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("edt_path")));
	m_pBtnBackUp  = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btn_backup")));
	m_pBtnForward = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btn_forword")));
	m_pBtnGo      = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btn_go")));
	m_pBtnTitleFile      = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btn_menu")));

	m_pBtnHost      = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btn_host")));
	m_pBtnHelp      = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btn_help")));
	m_pBtnMenuFile  = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btn_menufile")));
	m_pBtnMenuView  = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btn_menuview")));
	m_pBtnMenuSign  = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btn_menusign")));
	m_pBtnMenuCheck = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btn_menucheck")));

	ASSERT(m_edtUrl);
	ASSERT(m_pLblTitle);
	ASSERT(m_pBtnMin);
	ASSERT(m_pBtnClose);
	ASSERT(m_pContainer);
	ASSERT(m_pBtnBackUp);
	ASSERT(m_pBtnForward);
	ASSERT(m_pBtnGo);
	ASSERT(m_pBtnTitleFile);
	ASSERT(m_pBtnHost);
	ASSERT(m_pBtnHelp);
	ASSERT(m_pBtnMenuFile);
	ASSERT(m_pBtnMenuView);
	ASSERT(m_pBtnMenuSign);
	ASSERT(m_pBtnMenuCheck);
	string strJsonData = m_pInitParam;
	bool succeed = false;
	Json::CharReaderBuilder reader_b;
	Json::Value out_value;
	Json::CharReader* reader(reader_b.newCharReader());
	JSONCPP_STRING errs;
	succeed = reader->parse(strJsonData.c_str(),
		strJsonData.c_str() + strJsonData.length(),
		&out_value, &errs);
	delete reader;
	string sRet;
	if (succeed && errs.size() == 0 && out_value.isObject())
	{
		m_width   = out_value["width"].isInt() ? out_value["width"].asInt() : 0;
		m_height  = out_value["height"].isInt() ? out_value["height"].asInt() : 0;
		m_bShowMin = out_value["showMin"].isInt() ? out_value["showMin"].asInt() : false;
	}
	if (m_pBtnMin){
		//m_pBtnMin->SetVisible(m_bShowMin);
	}
	::DragAcceptFiles(GetHWND(),true);
}

HWND CMainFrame::Create(HWND hwndParent, LPCTSTR pstrName)
{
	HWND hwnd = __super::Create(hwndParent, pstrName, UI_WNDSTYLE_DIALOG/* & ~WS_MAXIMIZEBOX*/, WS_EX_WINDOWEDGE, 0, 0, 0, 0);
	m_ShaowHelp.Init(hwnd);
	return hwnd;
}

void CMainFrame::SetCaption(wstring sCaption)
{
	if (m_pLblTitle){
		m_pLblTitle->SetText(sCaption.c_str());
	}
}

void CMainFrame::SetCurrentUrl(wstring sCaption)
{
	if (m_edtUrl)
	{
		m_edtUrl->SetText(sCaption.c_str());
	}
}

DuiLib::CDuiString CMainFrame::GetSkinFile()
{
	return _T("tgweb.xml");
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
	case WM_COPYDATA:
		{
			COPYDATASTRUCT* pCopyData = (COPYDATASTRUCT*)lParam;
			wchar_t *pData = new wchar_t[pCopyData->cbData + 1];
			memset(pData, 0, pCopyData->cbData + 1);
			memcpy_s(pData, pCopyData->cbData, (char *)pCopyData->lpData, pCopyData->cbData);
			pData[pCopyData->cbData] = '\0';
			USES_CONVERSION;
			string strNameData = W2A(pData);
			int ipos = strNameData.find("-tgweb");
			if (ipos != string::npos)
			{
				string strJsonData = strNameData.substr(0, ipos - 1);
				bool succeed = false;
				Json::CharReaderBuilder reader_b;
				Json::Value out_value;
				Json::CharReader* reader(reader_b.newCharReader());
				JSONCPP_STRING errs;
				succeed = reader->parse(strJsonData.c_str(),
					strJsonData.c_str() + strJsonData.length(),
					&out_value, &errs);
				delete reader;
				string sRet;
				if (succeed && errs.size() == 0 && out_value.isObject()) {
					int carType = out_value["carType"].asInt();
					if (carType == 1)
					{
						m_mutex.lock();
						m_sInitUrl = out_value["initurl"].asString();
						m_width    = out_value["width"].isInt() ? out_value["width"].asInt() : 0;
						m_height   = out_value["height"].isInt() ? out_value["height"].asInt() : 0;
						m_bShowMin = out_value["showMin"].isInt() ? out_value["showMin"].asInt() : false;
						m_mutex.unlock();
						::PostMessage(GetHWND(), WM_MSG_NAVIGATION, 0, 0);
					}
					else if (carType == 2)
					{
						::ShowWindow(GetHWND(), SW_HIDE);
						m_width = 0;
						m_height = 0;
						m_bShowMin = false;
					}
				}
			}
		}
		break;
	case WM_MSG_NAVIGATION:
		{
			if (!m_sInitUrl.empty() && m_handler) {
				m_handler->GetBrowser()->GetMainFrame()->LoadURL(m_sInitUrl);
				Sleep(20);
			}
			
			if (m_width && m_height){
				ResizeClient(m_width, m_height);
				RECT rc;
				::GetClientRect(GetHWND(), &rc);
				m_pBtnClose->SetFloat(true);
				rc.bottom = rc.top + 36;
				rc.left = rc.right - 30;
				m_pBtnClose->SetPos(rc);
				if (m_pBtnMin){
					rc.left = rc.right - 30;
					m_pBtnMin->SetVisible(m_bShowMin);
					m_pBtnMin->SetPos(rc);
				}
			}
			if (m_handler)
			{
				CefRefPtr<CefBrowser> browser = m_handler->GetBrowser();
				if (browser.get())
				{
					HWND hwnd_ = browser->GetHost()->GetWindowHandle();
					if (hwnd_)
					{
						RECT rcCef;
						::GetClientRect(GetHWND(), &rcCef);
						rcCef.top = rcCef.top + 36;
						::MoveWindow(hwnd_, rcCef.left, rcCef.top, rcCef.right - rcCef.left, rcCef.bottom - rcCef.top, TRUE);
					}
				}
			}
			::ShowWindow(GetHWND(), SW_SHOWNOACTIVATE);
			CenterWindow();
			::PostMessage(GetHWND(), WM_MSG_REFRESH_SHADOW, 0, 0);
		}
		break;
	case WM_MSG_SHOW:
		{
			CenterWindow();
			//::ShowWindow(GetHWND(), SW_SHOWMAXIMIZED);
			::ShowWindow(GetHWND(), SW_SHOWNOACTIVATE);
			::PostMessage(GetHWND(), WM_MSG_REFRESH_SHADOW, 0, 0);
			m_handler = new CSimpleCefHandler(this);
			CefBrowserSettings browser_settings;
			std::string url = "http://testfront.tsign.cn:8870/oauth/show?redirectUri=https://localhost:7688/TGRedirect&responseType=code&appId=3876544261&scope=get_user_info,op_seal&state=1&flowId=1&platformAccountId=8ac8019401a5454fb1ea7e01aa0e81e8&platformOrgId=7efb35cf060c407190fa8554a0858743&platformIdCardNum=43021919800312105X";
			url = m_sInitUrl;
			//url = "https://www.baidu.com/";
			
			//最大化
			CefWindowInfo windows_info;
			RECT rc;
			::GetClientRect(GetHWND(), &rc);
			rc.top = rc.top + 36 + 40;
			windows_info.SetAsChild(GetHWND(), rc);
			/*CefRequestContextSettings recoSetting;
			wchar_t buffer[MAX_PATH] = { 0 };
			ULONG name_size = ::GetModuleFileNameW(NULL, buffer, MAX_PATH);
			if (name_size == 0) {
				return false;
			}
			::PathRemoveFileSpecW(buffer);
			wstring strCookiePath(buffer);
			strCookiePath.append(_T("\\WebCookies"));
			CefString(&recoSetting.cache_path).FromWString(strCookiePath);
			handlerCookies = new CCefRequestContextHandlerEx();
			CefRefPtr<CefRequestContext> cefReCo = CefRequestContext::CreateContext(recoSetting, handlerCookies);*/

			bool bRet = CefBrowserHost::CreateBrowser(windows_info, m_handler, url, browser_settings, NULL);
			int k = 0;
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
		::DragQueryFile(hDropInfo, 0, pFilePathName, 500);
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
	case WM_CLOSE:
		{
			PostQuitMessage(0);
		}
		break;
	case WM_OPEN_PDF_FILE:
	{
		std::thread th(&CMainFrame::OpenLocalPdfFile, this);
		th.detach();
		break;
	}
	case WM_MSG_CALL_JS:
	{
		std::thread th(&CMainFrame::CallJsFucntion, this);
		th.detach();
		break;
	}
	default:
		break;
	}
	if (bHandled) return lRes;
	return __super::HandleMessage(uMsg, wParam, lParam);
}

void CMainFrame::OpenLocalPdfFile()
{
	OPENFILENAME ofn;
	TCHAR szFile[MAX_PATH] = _T("");

	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = *this;
	ofn.lpstrFile = szFile;
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = _T("PDF文件(.pdf)\0*.pdf*\0");
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_EXPLORER | OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	if (GetOpenFileName(&ofn))
	{
		if (m_handler)
		{
			CDuiString strUrl = _T("https://localhost:7688/index.html/?file=");
			strUrl.Append(ofn.lpstrFile);
			USES_CONVERSION;
			string strName = W2A(ofn.lpstrFile);
			int iPos = strName.find(".pdf");
			if (iPos != std::string::npos)
			{
				m_handler->GetBrowser()->GetMainFrame()->LoadURL(strUrl.GetData());
			}
		}
	}
}

void CMainFrame::CallJsFucntion()
{
	CefRefPtr<CefFrame> frame = m_handler->GetBrowser()->GetMainFrame();
	if (frame) {
		CefString strCode;
		USES_CONVERSION;
		CDuiString strData;
		strData.Format(_T("functionjs();"));
		strCode = strData.GetData();
		OutputDebugStringA("\n");
		OutputDebugStringA("client ExecuteJavaScript");
		OutputDebugStringA("\n");
		frame->ExecuteJavaScript(strCode, frame->GetURL(), 0);
	}
}

void CMainFrame::Notify(TNotifyUI& msg)
{
	if(msg.sType == DUI_MSGTYPE_CLICK)
	{
		if (msg.pSender == m_pBtnClose)
		{
			Close(IDCANCEL);
			if (m_handler){
				m_handler->CloseAllBrowser(true);
			}
			CefShutdown();
			::PostQuitMessage(0);
			return;
		}
		else if (msg.pSender == m_pBtnMin)
		{
			SendMessage(WM_SYSCOMMAND, SC_MINIMIZE, 0);
			return;
		}
		else if (msg.pSender == m_pBtnBackUp)
		{
			if (m_handler->GetBrowser()->CanGoBack())
			{
				m_handler->GetBrowser()->GoBack();
			}
		}
		else if (msg.pSender == m_pBtnForward)
		{
			if (m_handler->GetBrowser()->CanGoForward())
			{
				m_handler->GetBrowser()->GoForward();
			}
		}
		else if (msg.pSender == m_pBtnGo)
		{
			if (m_edtUrl && m_handler)
			{
				CDuiString strEditUrl = m_edtUrl->GetText();
				if (!strEditUrl.IsEmpty())
				{
					m_handler->GetBrowser()->GetMainFrame()->LoadURL(strEditUrl.GetData());
				}
			}
		}
		else if (msg.pSender == m_pBtnTitleFile)
		{
			std::thread th(&CMainFrame::OpenLocalPdfFile, this);
			th.detach();
		}
		else if (msg.pSender == m_pBtnHost)
		{
			if (m_handler)
			{
				CDuiString strUrl = _T("https://www.hao123.com/");
				m_handler->GetBrowser()->GetMainFrame()->LoadURL(strUrl.GetData());
			}
		}
		else if (msg.pSender == m_pBtnHelp)
		{
			//打开页面或者网页
			//帮助.docx
			char buffer[MAX_PATH] = { 0 };
			ULONG name_size = ::GetModuleFileNameA(NULL, buffer, MAX_PATH);
			if (name_size == 0) {
				return;
			}
			::PathRemoveFileSpecA(buffer);
			string sFilePath = buffer;
			sFilePath.append("\\帮助.docx");
			::ShellExecuteA(NULL, "open", sFilePath.c_str(), NULL, NULL, SW_SHOWNORMAL);
		}
		else if (msg.pSender == m_pBtnMenuFile)
		{
			m_pFileMenuDlg = new CFileMenuDlg(this);
			m_pFileMenuDlg->Create(GetHWND());
			RECT rc = m_pBtnMenuFile->GetPos();
			rc.top += 24;
			POINT pt = { 0 };
			::GetCursorPos(&pt);
			m_pFileMenuDlg->AdjustPos(rc, pt);
			m_pFileMenuDlg->ShowWnd();
		}
		else if (msg.pSender == m_pBtnMenuView)
		{
			CViewMenuDlg *m_pViewMenuDlg = new CViewMenuDlg(this);
			m_pViewMenuDlg->Create(GetHWND());
			RECT rc = m_pBtnMenuView->GetPos();
			rc.top += 24;
			POINT pt = { 0 };
			::GetCursorPos(&pt);
			m_pViewMenuDlg->AdjustPos(rc, pt);
			m_pViewMenuDlg->ShowWnd();
		}
		else if (msg.pSender == m_pBtnMenuSign)
		{
			CSignMenuDlg *m_pSignMenuDlg = new CSignMenuDlg(this);
			m_pSignMenuDlg->Create(GetHWND());
			RECT rc = m_pBtnMenuSign->GetPos();
			rc.top += 24;
			POINT pt = { 0 };
			::GetCursorPos(&pt);
			m_pSignMenuDlg->AdjustPos(rc, pt);
			m_pSignMenuDlg->ShowWnd();
		}
		else if (msg.pSender == m_pBtnMenuCheck)
		{
			CCheckMenuDlg *m_pCheckMenuDlg = new CCheckMenuDlg(this);
			m_pCheckMenuDlg->Create(GetHWND());
			RECT rc = m_pBtnMenuCheck->GetPos();
			rc.top += 24;
			POINT pt = { 0 };
			::GetCursorPos(&pt);
			m_pCheckMenuDlg->AdjustPos(rc, pt);
			m_pCheckMenuDlg->ShowWnd();
		}
	}
}