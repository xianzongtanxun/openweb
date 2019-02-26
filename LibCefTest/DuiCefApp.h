#pragma once
#include "include/cef_app.h"
#include "CCEFV8HandlerEx.h"
class CDuiCefApp : public CefApp,
	public CefBrowserProcessHandler,
	public CefRenderProcessHandler
{
public:
	CDuiCefApp(HWND hwnd = NULL);
	virtual ~CDuiCefApp(void);

	//CefApp methods
	virtual CefRefPtr<CefBrowserProcessHandler> GetBrowserProcessHandler() override;
	virtual CefRefPtr<CefRenderProcessHandler> GetRenderProcessHandler() override;
	//CefBrowserProcessHandler methods
	virtual void OnContextInitialized() override;
	virtual void OnBeforeCommandLineProcessing(const CefString& process_type, CefRefPtr<CefCommandLine> command_line) override;

	//在webkit初始化之后调用
	virtual void OnWebKitInitialized() override;

	//在创建了一个对象的V8上下文之后立即调用
	//注册js方法
	virtual void OnContextCreated(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefV8Context> context) override;

private:
	HWND m_hwnd;
	CefRefPtr<CCEFV8HandlerEx> v8Handler_;
	//Include the default reference counting implementation
	IMPLEMENT_REFCOUNTING(CDuiCefApp);
};

