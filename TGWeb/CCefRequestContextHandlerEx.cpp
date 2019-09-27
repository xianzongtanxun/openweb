#pragma once
#include "CCefRequestContextHandlerEx.h"

CCefRequestContextHandlerEx::CCefRequestContextHandlerEx()
{
	m_CookieManager = CefCookieManager::CreateManager("D:\\codeTest\\openweb\\Release\\CefCookie", TRUE, NULL);
}

CefRefPtr<CefCookieManager> CCefRequestContextHandlerEx::GetCookieManager()
{
	return m_CookieManager;
}
