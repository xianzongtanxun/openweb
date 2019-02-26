#pragma once
#include "include/cef_app.h"
#include "CCefV8HandlerEx.h"

class CMainFrame;
class CSimpleCefApp : public CefApp,
	public CefBrowserProcessHandler,
	public CefRenderProcessHandler//,
	//public CefResourceHandler
{
public:
	CSimpleCefApp(CMainFrame* pMainFrame,HWND hwnd = NULL);
	virtual ~CSimpleCefApp(void);
	void SetMainFrame(CMainFrame* pMainFrame);
	//void UploadFileApp();
	//CefApp methods
	virtual CefRefPtr<CefBrowserProcessHandler> GetBrowserProcessHandler() override;
	virtual CefRefPtr<CefRenderProcessHandler> GetRenderProcessHandler() override;
	//CefBrowserProcessHandler methods
	virtual void OnContextInitialized() override;
	virtual void OnBeforeCommandLineProcessing(const CefString& process_type, CefRefPtr<CefCommandLine> command_line) override;
	//////////////////////////////////
	/*virtual bool ProcessRequest(CefRefPtr<CefRequest> request, CefRefPtr<CefCallback> callback) override;
	virtual bool ReadResponse(void* data_out, int bytes_to_read, int& bytes_read, CefRefPtr<CefCallback> callback) override;
	virtual void GetResponseHeaders(CefRefPtr<CefResponse> response, int64& response_length, CefString& redirectUrl) override;
	virtual void Cancel() override;*/
	//��webkit��ʼ��֮�����
	virtual void OnWebKitInitialized() override;

	//�ڴ�����һ�������V8������֮����������
	//ע��js����
	virtual void OnContextCreated(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefV8Context> context) override;
	//std::string DumpRequestContents(CefRefPtr<CefRequest> request);
private:
	HWND m_hwnd;
	/*std::string data_;
	std::string mime_type_;
	size_t offset_;*/
	CefRefPtr<CCefV8HandlerEx> v8Handler_;
	CMainFrame* m_pMainFrame;
	//Include the default reference counting implementation
	IMPLEMENT_REFCOUNTING(CSimpleCefApp);
};