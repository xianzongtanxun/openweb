#pragma once
#include "include/cef_v8.h"
class CCEFV8HandlerEx : public CefV8Handler
{
public:
	CCEFV8HandlerEx();
	virtual ~CCEFV8HandlerEx(void);

public:
	virtual bool Execute(const CefString& name, CefRefPtr<CefV8Value> object, const CefV8ValueList& arguments, CefRefPtr<CefV8Value>& retval, CefString& exception) override;

private:
	//map of message callbacks
	typedef std::map<std::pair<std::string, int>, std::pair<CefRefPtr<CefV8Context>, CefRefPtr<CefV8Value>>> CallbackMap;
	CallbackMap callback_map_;
	CefRefPtr<CefV8Value> callback_func_;
	CefRefPtr<CefV8Context> callback_context_;
private:
	IMPLEMENT_REFCOUNTING(CCEFV8HandlerEx);
};

