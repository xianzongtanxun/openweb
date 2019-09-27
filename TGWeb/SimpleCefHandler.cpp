#include "stdafx.h"
#include "SimpleCefHandler.h"
#include <sstream>
#include <string>
#include <shellapi.h>

#include "include/base/cef_bind.h"
#include "include/cef_app.h"
#include "include/wrapper/cef_closure_task.h"
#include "include/wrapper/cef_helpers.h"
#include "MainFrame.h"
#define  URL_Redirect 1
#define  URL_DYZS     2
#define  URL_OUTLIST  3
CSimpleCefHandler* g_instance = NULL;
CSimpleCefHandler::CSimpleCefHandler(CMainFrame* pMainFrame):is_closing_(false)
{
	DCHECK(!g_instance);
	g_instance = this;
	m_browser = NULL;
	m_main_wnd = NULL;
	m_end = false;
	m_pMainFrame = pMainFrame;
	memset(&m_ClientRc, 0, sizeof(RECT));
}

CSimpleCefHandler::~CSimpleCefHandler(void)
{
	g_instance = NULL;
}

CSimpleCefHandler* CSimpleCefHandler::GetInstance()
{
	return g_instance;
}

CefRefPtr<CefBrowser> CSimpleCefHandler::GetBrowser()
{
	return m_browser;
}

void CSimpleCefHandler::OnBeforeDownload(CefRefPtr<CefBrowser> browser, CefRefPtr<CefDownloadItem> download_item, const CefString& suggested_name, CefRefPtr<CefBeforeDownloadCallback> callback)
{
	UNREFERENCED_PARAMETER(browser);
	UNREFERENCED_PARAMETER(download_item);
	callback->Continue(suggested_name,true);
}

void CSimpleCefHandler::OnDownloadUpdated(CefRefPtr<CefBrowser> browser, CefRefPtr<CefDownloadItem> download_item, CefRefPtr<CefDownloadItemCallback> callback)
{
	CefString strUrl = download_item->GetURL();
	if (download_item->IsComplete())
	{
		if (browser->IsPopup() && !browser->HasDocument())
		{
			//browser->GetHost()->ParentWindowWillClose();
			//browser->GetHost()->CloseBrowser(true);
		}
	}
}

void CSimpleCefHandler::OnTitleChange(CefRefPtr<CefBrowser> browser, const CefString& title)
{
	CEF_REQUIRE_UI_THREAD();
	CefWindowHandle hwnd = browser->GetHost()->GetWindowHandle();
	SetWindowText(hwnd,title.c_str());
	if (m_pMainFrame){
		m_pMainFrame->SetCaption(title.c_str());
	}
}

void CSimpleCefHandler::OnAfterCreated(CefRefPtr<CefBrowser> browser)
{
	CEF_REQUIRE_UI_THREAD();
	if (!m_browser.get())
	{
		m_browser = browser;
		m_BrowserId = browser->GetIdentifier();
	}
	else if (browser->IsPopup())
	{

	}
	
	//browser_list_.push_back(browser);
}

bool CSimpleCefHandler::DoClose(CefRefPtr<CefBrowser> browser)
{
	CEF_REQUIRE_UI_THREAD();
	if (m_browser)
	{
		is_closing_ = true;
	}
	//if (browser_list_.size() == 1)
	//{
	//	//is_closing_ = true;
	//}
	return false;
}

void CSimpleCefHandler::OnBeforeClose(CefRefPtr<CefBrowser> browser)
{
	CEF_REQUIRE_UI_THREAD();
	/*BrowserList::iterator bit = browser_list_.begin();
	for (;bit != browser_list_.end();++bit)
	{
		if ((*bit)->IsSame(browser))
		{
			browser_list_.erase(bit);
			break;
		}
	}
	if (browser_list_.empty())
	{
		CefQuitMessageLoop();
	}*/
	if (m_browser == NULL){
		return;
	}
	if (m_browser->IsSame(browser)){
		//m_browser = NULL;
	}
}

bool CSimpleCefHandler::OnBeforePopup(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, const CefString& target_url, const CefString& target_frame_name, WindowOpenDisposition target_disposition, bool user_gesture, const CefPopupFeatures& popupFeatures, CefWindowInfo& windowInfo, CefRefPtr<CefClient>& client, CefBrowserSettings& settings, bool* no_javascript_access)
{
	std::wstring urlFind = target_url;
	int iURL_type = JugelsAdvertisementWindow(target_url);
	if (iURL_type == URL_Redirect)
	{
		std::wstring url = target_url;
		this->GetBrowser()->GetMainFrame()->LoadURL(url);
		return true;
	}
	return false;
}

void CSimpleCefHandler::OnBeforeContextMenu(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefContextMenuParams> params, CefRefPtr<CefMenuModel> model)
{
	cef_context_menu_type_flags_t flag = params->GetTypeFlags();
	//CefString devtools_url = browser->GetHost()->GetDevT
	if (flag & CM_TYPEFLAG_PAGE)
	{
		//普通页面的右键消息
		//model->AddItem(MENU_ID_BACK, _T("后退"));
		//model->AddItem(MENU_ID_FORWARD, _T("前进"));
		//model->AddSeparator();
		model->AddItem(MENU_ID_RELOAD, _T("调试"));
	}
	else if (flag & CM_TYPEFLAG_EDITABLE)
	{
		//编辑框的右键消息
		//model->SetLabel(MENU_ID_COPY,_T("复制"));
		model->AddItem(MENU_ID_COPY, _T("复制"));
	}
}

bool CSimpleCefHandler::OnContextMenuCommand(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefContextMenuParams> params, int command_id, EventFlags event_flags)
{
	/*CefString strURLLink;
	CefString* strTargetURL = NULL;
	int nBuffLength = 0;
	switch (command_id)
	{
		case MENU_ID_VIEW_DELETHC
	default:
		break;
	}*/
	switch (command_id)
	{
	case MENU_ID_RELOAD:
	{
		ShowDevTools(browser);
		break;
	}
	/*case MENU_ID_BACK:
	{
		if (browser)
		{
			browser->GoBack();
		}
		break;
	}
	case MENU_ID_FORWARD:
	{
		if (browser)
		{
			browser->GoForward();
		}
		break;
	}*/
	default:
		break;
	}
	return false;
}

void CSimpleCefHandler::OnLoadError(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, ErrorCode errorCode, const CefString& errorText, const CefString& failedUrl)
{
	CEF_REQUIRE_UI_THREAD();
	if (errorCode == ERR_ABORTED){
		return;
	}
	//Display a load error message
	std::stringstream ss;
	ss << "<html><body bgcolor=\"white\">"
		"<h2>Failed to load URL " << std::string(failedUrl) <<
		" with error " << std::string(errorText) << " (" << errorCode <<
		").</h2></body></html>";
	frame->LoadString(ss.str(),failedUrl);
}

void CSimpleCefHandler::OnLoadEnd(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, int httpStatusCode)
{
	m_end = true;
}

void CSimpleCefHandler::CloseAllBrowser(bool force_close)
{
	if (!CefCurrentlyOn(TID_UI)){
		CefPostTask(TID_UI,base::Bind(&CSimpleCefHandler::CloseAllBrowser,this,force_close));
		return;
	}
	/*if (browser_list_.empty()){
		return;
	}
	BrowserList::const_iterator it = browser_list_.begin();
	for (;it != browser_list_.end();++it)
	{
		(*it)->GetHost()->CloseBrowser(force_close);
	}*/
	if (m_browser)
	{
		m_browser->GetHost()->CloseBrowser(force_close);
	}
}

void CSimpleCefHandler::InvokeDefaultBrowser(std::wstring url)
{
	::ShellExecuteW(NULL,_T("open"),url.c_str(),NULL,NULL,SW_NORMAL);
}

//#define WCHAR_T_IS_UTF32 1
int CSimpleCefHandler::JugelsAdvertisementWindow(CefString strUrl)
{
	/*if ((-1 != strUrl.find(_T("http"))) || 
		(-1 != strUrl.find(_T("https"))))
	{
		return URL_Redirect;
	}*/
	int k = 0;
	return true;
}

void CSimpleCefHandler::SetMainHwnd(CefWindowHandle hwnd)
{
	m_main_wnd = hwnd;
}

void CSimpleCefHandler::CloseHostBrowser(CefRefPtr<CefBrowser> browser, bool bforce_close)
{
	if (!CefCurrentlyOn(TID_UI))
	{
		CefPostTask(TID_UI, base::Bind(&CSimpleCefHandler::CloseHostBrowser, this, browser, bforce_close));
		return;
	}
	int nID = browser->GetIdentifier();
	browser->GetHost()->CloseBrowser(bforce_close);
}

//CefRefPtr<CefContextMenuHandler> CSimpleCefHandler::GetContextMenuHandler()
//{
//	return this;
//}

void CSimpleCefHandler::ShowDevTools(CefRefPtr<CefBrowser> browser)
{
	
	CefWindowInfo win_info;
	win_info.width = 800;
	win_info.height = 600;
	win_info.style = WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_VISIBLE | WS_POPUP;
	//win_info.window_name = _T("Dev Tools");
	CefRefPtr<CefClient> client;
	CefBrowserSettings settings;
	
	browser->GetHost()->ShowDevTools(win_info, client, settings, CefPoint());
}