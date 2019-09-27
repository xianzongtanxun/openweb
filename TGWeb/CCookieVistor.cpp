#pragma once
#include "CCookieVistor.h"

bool CCookieVisitor::Visit(const CefCookie& cookie, int count, int total, bool&deleteCookie)
{
	if (count == total)
	{
		return false;
	}
	if (cookie.name.str && cookie.value.str)
	{
		std::wstring strName = cookie.name.str;
		std::wstring strValue = cookie.value.str;
	}
	return true;
}
