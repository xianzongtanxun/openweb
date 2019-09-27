#pragma once
#include "include/cef_request_context_handler.h"
class CCefRequestContextHandlerEx : public CefRequestContextHandler
{
public:
	CCefRequestContextHandlerEx();
	virtual ~CCefRequestContextHandlerEx(void){};
	CefRefPtr<CefCookieManager> GetCookieManager();
private:
	CefRefPtr<CefCookieManager> m_CookieManager;
private:
	IMPLEMENT_REFCOUNTING(CCefRequestContextHandlerEx);
};

