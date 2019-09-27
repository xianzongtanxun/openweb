#pragma once
#include "include/cef_cookie.h"
class CCookieVisitor : public CefCookieVisitor
{
public:
	CCookieVisitor(){};
	~CCookieVisitor(void){};
	bool Visit(const CefCookie& cookie, int count, int total, bool&deleteCookie);
private:
	IMPLEMENT_REFCOUNTING(CCookieVisitor);
};

