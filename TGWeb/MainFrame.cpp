#include "stdafx.h"
#include <commdlg.h>
#include <shellapi.h>
#include <WinUser.h>
#include <thread>
#include "MainFrame.h"
#include "Utils\UIDelegate.h"
#include "include\cef_app.h"
#include "SimpleCefApp.h"
#include "include/cef_request_context.h"
#include "CCefRequestContextHandlerEx.h"
#include "include/base/cef_bind_helpers.h"
#include "CCookieVistor.h"
///////////////////////////
CMainFrame::CMainFrame(const string& sInitUrl, const string& sInitParam) :
	m_pLblTitle(nullptr),   
	m_pBtnMin(nullptr),
	m_pBtnClose(nullptr),
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
	//支持文件拖拽功能

	ASSERT(m_pLblTitle);
	ASSERT(m_pBtnMin);
	ASSERT(m_pBtnClose);
	ASSERT(m_pContainer);

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
	//SetWindowPos(GetHWND(), HWND_TOPMOST, 0, 0, m_width, m_height, SWP_NOMOVE | SWP_NOSIZE);
}

HWND CMainFrame::Create(HWND hwndParent, LPCTSTR pstrName)
{
	//UI_WNDSTYLE_DIALOG
	//UI_WNDSTYLE_DIALOG | WS_MAXIMIZEBOX
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
			//url = m_sInitUrl;
			//url = "https://www.baidu.com/";
			
			//最大化
			CefWindowInfo windows_info;
			RECT rc;
			::GetClientRect(GetHWND(), &rc);
			rc.top = rc.top + 36;
			windows_info.SetAsChild(GetHWND(), rc);
			CefRequestContextSettings recoSetting;

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
			CefRefPtr<CefRequestContext> cefReCo = CefRequestContext::CreateContext(recoSetting, handlerCookies);
			
			/*CefRefPtr<CefCookieManager> manager = handlerCookies->GetCookieManager();
			CefCookie cookie;
			
			CefString(&cookie.domain).FromWString(Domain.c_str());
			CefString(&cookie.path).FromWString(_T("\\CefCookie"));
			std::wstring httpDomain = _T("http://");
			httpDomain.append(Domain);
			manager->SetCookie(httpDomain, cookie, NULL);*/
			//std::wstring Domain = _T("testfront.tsign.cn:8870");
			//std::wstring sKey = _T("username");
			//std::wstring sValue = _T("xidada");
			//SetWebCookies(Domain, sKey, sValue);
			bool bRet = CefBrowserHost::CreateBrowser(windows_info, m_handler, url, browser_settings, cefReCo);
			int k = 0;
		}
		break;
	case WM_MSG_REFRESH_SHADOW:
		{
			m_ShaowHelp.ReFlush(GetHWND());
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
	default:
		break;
	}
	if (bHandled) return lRes;
	return __super::HandleMessage(uMsg, wParam, lParam);
}

void CMainFrame::SetWebCookies(std::wstring domain, std::wstring key, std::wstring sValue)
{
	CefRefPtr<CefCookieManager> manager = CefCookieManager::GetGlobalManager(NULL);
	CefRefPtr<CefSetCookieCallback> callback = NULL;
	CefCookie cookie;
	CefString(&cookie.name).FromWString(key.c_str());
	CefString(&cookie.value).FromWString(sValue.c_str());
	CefString(&cookie.domain).FromWString(domain.c_str());
	CefString(&cookie.path).FromWString(_T("D:\\codeTest\\openweb\\Release\\CefCookie"));
	cookie.has_expires = true;
	//std::wstring httpDomain = _T("http://testfront.tsign.cn:8870/oauth/show?redirectUri=https://localhost:7688/TGRedirect&responseType=code&appId=3876544261&scope=get_user_info,op_seal&state=1&flowId=1&platformAccountId=8ac8019401a5454fb1ea7e01aa0e81e8&platformOrgId=7efb35cf060c407190fa8554a0858743&platformIdCardNum=43021919800312105X");
	std::wstring httpDomain = _T("http://");
	httpDomain.append(domain);
	//manager->SetCookie(httpDomain.c_str(), cookie, NULL);
	//base::IgnoreResult(&CefCookieManager::SetCookie);
	std::thread th(&CefCookieManager::SetCookie, manager.get(),httpDomain.c_str(), cookie, callback);
	th.detach();
}
//LRESULT CMainFrame::OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
//{
//	bHandled = true;
//	return true;
//	if (!m_handler){
//		return false;
//	}
//	RECT rc;
//	::GetClientRect(GetHWND(), &rc);
//	m_pBtnClose->SetFloat(true);
//	rc.bottom = rc.top + 36;
//	rc.left = rc.right - 30;
//	m_pBtnClose->SetPos(rc);
//	if (::IsZoomed(GetHWND())){
//		//m_pBtnMax->SetVisible(false);
//		//m_pBtnRestore->SetVisible(true);
//		//m_pBtnRestore->SetFloat(true);
//		rc.left = rc.left - 30;
//		rc.right = rc.left + 30;
//		//m_pBtnRestore->SetPos(rc);
//	}
//	else{
//		//m_pBtnMax->SetVisible(true);
//		//m_pBtnRestore->SetVisible(false);
//		//m_pBtnMax->SetFloat(true);
//		rc.left = rc.left - 30;
//		rc.right = rc.left + 30;
//		//m_pBtnMax->SetPos(rc);
//	}
//	
//	CefRefPtr<CefBrowser> browser = m_handler->GetBrowser();
//	if (browser.get())
//	{
//		HWND hwnd_ = browser->GetHost()->GetWindowHandle();
//		if (hwnd_)
//		{
//			RECT rcCef;
//			::GetClientRect(GetHWND(), &rcCef);
//			rcCef.top = rcCef.top + 80;
//			::MoveWindow(hwnd_, rcCef.left, rcCef.top, rcCef.right - rcCef.left, rcCef.bottom - rcCef.top, TRUE);
//		}
//	}
//	return true;
//}

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
			//CefRefPtr<CefCookieManager> cefCookieManager = CefCookieManager::GetGlobalManager(NULL);
			//if (cefCookieManager)
			//{
				//std::wstring httpDomain = _T("http://testfront.tsign.cn:8870/oauth/show?redirectUri=https://localhost:7688/TGRedirect&responseType=code&appId=3876544261&scope=get_user_info,op_seal&state=1&flowId=1&platformAccountId=8ac8019401a5454fb1ea7e01aa0e81e8&platformOrgId=7efb35cf060c407190fa8554a0858743&platformIdCardNum=43021919800312105X");
				CefCookie cookie;
				//std::wstring Domain = _T("testfront.tsign.cn:8870");
				//std::wstring sKey = _T("username");
				//std::wstring sValue = _T("xidada");
				//SetWebCookies(Domain, sKey, sValue);
				//CefString(&cookie.name).FromWString(sKey.c_str());
				//CefString(&cookie.value).FromWString(sValue.c_str());
				//CefString(&cookie.domain).FromWString(Domain.c_str());
				//CefString(&cookie.path).FromWString(_T("\\CefCookie"));
				//cookie.has_expires = true;
				//std::wstring httpDomain = _T("http://");
				//httpDomain.append(Domain);
				//std::thread th(&CefCookieManager::SetCookie, cefCookieManager.get(), httpDomain.c_str(), cookie, NULL);
				//th.detach();
				//std::wstring httpDomain = _T("htp://testfront.tsign.cn:8870");
				//bool bDelete = false;
				//m_CookieVisitor = new CCookieVisitor();
				//handlerCookies->GetCookieManager()->VisitAllCookies(m_CookieVisitor);
				//handlerCookies->GetCookieManager()->VisitUrlCookies(httpDomain, true, m_CookieVisitor);
				//int k = 0;
				//if (m_CookieVisitor)
				//{
				//	m_CookieVisitor->Visit(cookie, 1, 2, bDelete);
				//}
			//}
			SendMessage(WM_SYSCOMMAND, SC_MINIMIZE, 0);
			return;
		}
	}
}