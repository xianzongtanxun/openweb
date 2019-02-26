#pragma once
#include "include/cef_client.h"
#include "include/cef_base.h"
#include "include/cef_browser.h"
#include "include/base/cef_lock.h"
#include "include/cef_web_plugin.h"

class CMainFrame;
#include <list>
class CSimpleCefHandler :
	public CefClient,
	public CefContextMenuHandler,
	public CefDisplayHandler,
	public CefDownloadHandler,
	public CefDragHandler,
	public CefGeolocationHandler,
	public CefKeyboardHandler,
	public CefLifeSpanHandler,
	public CefLoadHandler//,
	//public CefRequestHandler
{
public:
	CSimpleCefHandler(CMainFrame* pMainFrame);
	~CSimpleCefHandler(void);
	//Provide access to the single global instance of this object
	static CSimpleCefHandler* GetInstance();
	CefRefPtr<CefBrowser> GetBrowser();
#pragma region CefClient
	virtual CefRefPtr<CefContextMenuHandler> GetContextMenuHandler() override{return this;}
	virtual CefRefPtr<CefDisplayHandler> GetDisplayHandler() override{return this;}
	virtual CefRefPtr<CefDownloadHandler> GetDownloadHandler() override{return this;}
	virtual CefRefPtr<CefDragHandler> GetDragHandler() override{return this;}
	//virtual CefRefPtr<CefGeolocationHandler> GetGeolocationHandler() override{return this;}
	virtual CefRefPtr<CefKeyboardHandler> GetKeyboardHandler() override{return this;}
	virtual CefRefPtr<CefLifeSpanHandler> GetLifeSpanHandler() override{return this;}
	virtual CefRefPtr<CefLoadHandler> GetLoadHandler() override{return this;}
	//virtual CefRefPtr<CefRequestHandler> GetRequestHandler() override{return this;}
#pragma endregion //CefClient

#pragma region CefDownloadHandler
	//���ؿ�ʼ֮ǰ����
	virtual void OnBeforeDownload(CefRefPtr<CefBrowser> browser, CefRefPtr<CefDownloadItem> download_item, const CefString& suggested_name, CefRefPtr<CefBeforeDownloadCallback> callback);
	//����״̬���߽�������Ϣ����ʱ����
	virtual void OnDownloadUpdated(CefRefPtr<CefBrowser> browser, CefRefPtr<CefDownloadItem> download_item, CefRefPtr<CefDownloadItemCallback> callback);
#pragma endregion //CefClient
	//CefDisplayHandler methods
	virtual void OnTitleChange(CefRefPtr<CefBrowser> browser, const CefString& title) override;
	//CefLifeSpanHandler methods
	//�����µ������֮�����
	virtual void OnAfterCreated(CefRefPtr<CefBrowser> browser) override;
	//��������յ�����ʱ����
	virtual bool DoClose(CefRefPtr<CefBrowser> browser) override;
	//���������֮ǰ����
	virtual void OnBeforeClose(CefRefPtr<CefBrowser> browser) override;

	virtual bool OnBeforePopup(CefRefPtr<CefBrowser> browser, 
		CefRefPtr<CefFrame> frame, 
		const CefString& target_url, 
		const CefString& target_frame_name, 
		WindowOpenDisposition target_disposition,
		bool user_gesture, 
		const CefPopupFeatures& popupFeatures,
		CefWindowInfo& windowInfo, 
		CefRefPtr<CefClient>& client, 
		CefBrowserSettings& settings, 
		bool* no_javascript_access);
	//�ڴ����µĵ���ʽ�����IO�߳�֮ǰ����
	//��������Ҫ��ֹ��������������д�

	//menu����
	//����ʾ�����Ĳ˵�֮ǰ����
	virtual void OnBeforeContextMenu(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefContextMenuParams> params, CefRefPtr<CefMenuModel> model);
	//������ִ�д������Ĳ˵���ѡ��� ����
	virtual bool OnContextMenuCommand(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefContextMenuParams> params, int command_id, EventFlags event_flags);
	//CefLoadHandler methods
	virtual void OnLoadError(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, ErrorCode errorCode, const CefString& errorText, const CefString& failedUrl) override;
	virtual void OnLoadEnd(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, int httpStatusCode);
	//virtual CefRefPtr<CefContextMenuHandler> GetContextMenuHandler();
	//Request that all existing browser windows close
	void CloseAllBrowser(bool force_close);
	bool IsClosing() const {return is_closing_;}
public:
	void InvokeDefaultBrowser(std::wstring url);//ʹ��Ĭ���������url
	int  JugelsAdvertisementWindow(CefString strUrl);
	void SetMainHwnd(CefWindowHandle hwnd);//��ȡ������ĸ����ھ��
	void CloseHostBrowser(CefRefPtr<CefBrowser> browser, bool bforce_close);
	void ShowDevTools(CefRefPtr<CefBrowser> browser);
private:
	//the browser reference
	CMainFrame* m_pMainFrame;
	CefRefPtr<CefBrowser> m_browser;
	//typedef std::list<CefRefPtr<CefBrowser>> BrowserList;
	//BrowserList browser_list_;
	bool is_closing_;
	RECT m_ClientRc;//��������ڴ�С
	int m_BrowserId;//�����IDΨһ��ʶ��
	string m_name;
	bool m_end;
	CefWindowHandle m_main_wnd;//������ĸ����ھ��
protected:
	IMPLEMENT_REFCOUNTING(CSimpleCefHandler);
	IMPLEMENT_LOCKING(CSimpleCefHandler);
};

