#pragma once
#include "include/cef_v8.h"
class CMainFrame;
class CCefV8HandlerEx : public CefV8Handler
{
public:
	CCefV8HandlerEx(CMainFrame* pMainFrame,HWND hwnd);
	virtual ~CCefV8HandlerEx(void);

public:
	virtual bool Execute(const CefString& name, CefRefPtr<CefV8Value> object, const CefV8ValueList& arguments, CefRefPtr<CefV8Value>& retval, CefString& exception) override;
	void SetMainFrame(CMainFrame* m_MainFrame);
private:
	//map of message callbacks
	typedef std::map<std::pair<std::string, int>, std::pair<CefRefPtr<CefV8Context>, CefRefPtr<CefV8Value>>> CallbackMap;
	CallbackMap callback_map_;
	CefRefPtr<CefV8Value> callback_func_;
	CefRefPtr<CefV8Context> callback_context_;
	CMainFrame* m_pMainFrame;
	HWND m_hwnd;
private:
	IMPLEMENT_REFCOUNTING(CCefV8HandlerEx);
};

