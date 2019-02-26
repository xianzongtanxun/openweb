#include "stdafx.h"
#include "DuiMsgHandler.h"
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
CDuiMsgHandler* g_instance = NULL;
CDuiMsgHandler::CDuiMsgHandler(CMainFrame* pMainFrame):is_closing_(false)
{
	DCHECK(!g_instance);
	g_instance = this;
	m_browser = NULL;
	m_main_wnd = NULL;
	m_end = false;
	m_pMainFrame = pMainFrame;
	memset(&m_ClientRc, 0, sizeof(RECT));
}

CDuiMsgHandler::~CDuiMsgHandler(void)
{
	g_instance = NULL;
}

CDuiMsgHandler* CDuiMsgHandler::GetInstance()
{
	return g_instance;
}

CefRefPtr<CefBrowser> CDuiMsgHandler::GetBrowser()
{
	return m_browser;
}

void CDuiMsgHandler::OnBeforeDownload(CefRefPtr<CefBrowser> browser, CefRefPtr<CefDownloadItem> download_item, const CefString& suggested_name, CefRefPtr<CefBeforeDownloadCallback> callback)
{
	UNREFERENCED_PARAMETER(browser);
	UNREFERENCED_PARAMETER(download_item);
	callback->Continue(suggested_name,true);
}

void CDuiMsgHandler::OnDownloadUpdated(CefRefPtr<CefBrowser> browser, CefRefPtr<CefDownloadItem> download_item, CefRefPtr<CefDownloadItemCallback> callback)
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

void CDuiMsgHandler::OnTitleChange(CefRefPtr<CefBrowser> browser, const CefString& title)
{
	CEF_REQUIRE_UI_THREAD();
	CefWindowHandle hwnd = browser->GetHost()->GetWindowHandle();
	SetWindowText(hwnd,title.c_str());
	if (m_pMainFrame){
		m_pMainFrame->SetCaption(title.c_str());
	}
}

void CDuiMsgHandler::OnAfterCreated(CefRefPtr<CefBrowser> browser)
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

bool CDuiMsgHandler::DoClose(CefRefPtr<CefBrowser> browser)
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

void CDuiMsgHandler::OnBeforeClose(CefRefPtr<CefBrowser> browser)
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
		m_browser = NULL;
	}
}

bool CDuiMsgHandler::OnBeforePopup(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, const CefString& target_url, const CefString& target_frame_name, WindowOpenDisposition target_disposition, bool user_gesture, const CefPopupFeatures& popupFeatures, CefWindowInfo& windowInfo, CefRefPtr<CefClient>& client, CefBrowserSettings& settings, bool* no_javascript_access)
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

void CDuiMsgHandler::OnBeforeContextMenu(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefContextMenuParams> params, CefRefPtr<CefMenuModel> model)
{
	cef_context_menu_type_flags_t flag = params->GetTypeFlags();
	if (flag & CM_TYPEFLAG_PAGE)
	{
		//普通页面的右键消息
		/*model->SetLabel(MENU_ID_BACK,_T("后退"));
		model->SetLabel(MENU_ID_FORWARD,_T("前进"));
		model->SetLabel(MENU_ID_RELOAD,_T("刷新"));*/
		model->AddItem(MENU_ID_BACK, _T("后退"));
		model->AddItem(MENU_ID_FORWARD, _T("前进"));
		model->AddSeparator();
		model->AddItem(MENU_ID_RELOAD, _T("刷新"));
	}
	else if (flag & CM_TYPEFLAG_EDITABLE)
	{
		//编辑框的右键消息
		//model->SetLabel(MENU_ID_COPY,_T("复制"));
		model->AddItem(MENU_ID_COPY, _T("复制"));
	}
}

bool CDuiMsgHandler::OnContextMenuCommand(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefContextMenuParams> params, int command_id, EventFlags event_flags)
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
	return false;
}

void CDuiMsgHandler::OnLoadError(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, ErrorCode errorCode, const CefString& errorText, const CefString& failedUrl)
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

void CDuiMsgHandler::OnLoadEnd(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, int httpStatusCode)
{
	m_end = true;
}

void CDuiMsgHandler::CloseAllBrowser(bool force_close)
{
	if (!CefCurrentlyOn(TID_UI)){
		CefPostTask(TID_UI,base::Bind(&CDuiMsgHandler::CloseAllBrowser,this,force_close));
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

void CDuiMsgHandler::InvokeDefaultBrowser(std::wstring url)
{
	::ShellExecuteW(NULL,_T("open"),url.c_str(),NULL,NULL,SW_NORMAL);
}
//#define WCHAR_T_IS_UTF32 1
int CDuiMsgHandler::JugelsAdvertisementWindow(CefString strUrl)
{
	/*if ((-1 != strUrl.find(_T("http"))) || 
		(-1 != strUrl.find(_T("https"))))
	{
		return URL_Redirect;
	}*/
	int k = 0;
	return true;
}

void CDuiMsgHandler::SetMainHwnd(CefWindowHandle hwnd)
{
	m_main_wnd = hwnd;
}

void CDuiMsgHandler::CloseHostBrowser(CefRefPtr<CefBrowser> browser, bool bforce_close)
{
	if (!CefCurrentlyOn(TID_UI))
	{
		CefPostTask(TID_UI, base::Bind(&CDuiMsgHandler::CloseHostBrowser, this, browser, bforce_close));
		return;
	}
	int nID = browser->GetIdentifier();
	browser->GetHost()->CloseBrowser(bforce_close);
}
