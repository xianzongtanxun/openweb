// Copyright (c) 2013 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.
#include "stdafx.h"
#include "simple_app.h"

#include <string>

#include "simple_handler.h"
#include "include/cef_browser.h"
#include "include/cef_command_line.h"
//#include "include/views/cef_browser_view.h"
//#include "include/views/cef_window.h"
#include "include/wrapper/cef_helpers.h"

namespace {

// When using the Views framework this object provides the delegate
// implementation for the CefWindow that hosts the Views-based browser.
//class SimpleWindowDelegate : public CefWindowDelegate {
// public:
//  explicit SimpleWindowDelegate(CefRefPtr<CefBrowserView> browser_view)
//      : browser_view_(browser_view) {
//  }
//
//  void OnWindowCreated(CefRefPtr<CefWindow> window) OVERRIDE {
//    // Add the browser view and show the window.
//    window->AddChildView(browser_view_);
//    window->Show();
//
//    // Give keyboard focus to the browser view.
//    browser_view_->RequestFocus();
//  }
//
//  void OnWindowDestroyed(CefRefPtr<CefWindow> window) OVERRIDE {
//    browser_view_ = NULL;
//  }
//
//  bool CanClose(CefRefPtr<CefWindow> window) OVERRIDE {
//    // Allow the window to close if the browser says it's OK.
//    CefRefPtr<CefBrowser> browser = browser_view_->GetBrowser();
//    if (browser)
//      return browser->GetHost()->TryCloseBrowser();
//    return true;
//  }
//
// private:
//  CefRefPtr<CefBrowserView> browser_view_;
//
//  IMPLEMENT_REFCOUNTING(SimpleWindowDelegate);
//  DISALLOW_COPY_AND_ASSIGN(SimpleWindowDelegate);
//};

}  // namespace

SimpleApp::SimpleApp() {
}

void SimpleApp::OnContextInitialized() {
  CEF_REQUIRE_UI_THREAD();

//  CefRefPtr<CefCommandLine> command_line =
//      CefCommandLine::GetGlobalCommandLine();
//
//#if defined(OS_WIN) || defined(OS_LINUX)
//  // Create the browser using the Views framework if "--use-views" is specified
//  // via the command-line. Otherwise, create the browser using the native
//  // platform framework. The Views framework is currently only supported on
//  // Windows and Linux.
//  const bool use_views = command_line->HasSwitch("use-views");
//#else
//  const bool use_views = false;
//#endif

  // SimpleHandler implements browser-level callbacks.
  //CefRefPtr<SimpleHandler> handler(new SimpleHandler(use_views));

  //// Specify CEF browser settings here.
  //CefBrowserSettings browser_settings;

  //std::string url;

  //// Check if a "--url=" value was provided via the command-line. If so, use
  //// that instead of the default URL.
  //url = command_line->GetSwitchValue("url");
  //if (url.empty())
  //  url = "http://www.baidu.com";

//  if (use_views) {
//    // Create the BrowserView.
//	  CefRefPtr<CefBrowserView> browser_view = CefBrowserView::CreateBrowserView(
//		  handler, url, browser_settings, NULL, NULL);
//
//    // Create the Window. It will show itself after creation.
//    CefWindow::CreateTopLevelWindow(new SimpleWindowDelegate(browser_view));
//  } else {
//    // Information used when creating the native window.
    //CefWindowInfo window_info;
//
//#if defined(OS_WIN)
//    // On Windows we need to specify certain flags that will be passed to
//    // CreateWindowEx().
//    window_info.SetAsPopup(NULL, "cefsimple");
//#endif
//
//    // Create the first browser window.
    //CefBrowserHost::CreateBrowser(window_info, handler, url, browser_settings,
//                                  NULL);
  //}
}

void SimpleApp::OnContextCreated(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefV8Context> context)
{
	CEF_REQUIRE_RENDERER_THREAD();
	m_js_callback_context = NULL;
	m_js_callback_func = NULL;
	m_hWndMain = FindWindow(L"MainSound", NULL);
	CefRefPtr<CefV8Value> object = context->GetGlobal();
	CefRefPtr<CefV8Handler> myhandler = this;
	CefRefPtr<CefV8Value> func = CefV8Value::CreateFunction(L"testfunc", myhandler);
	CefRefPtr<CefV8Value> func2 = CefV8Value::CreateFunction(L"testfunc2", myhandler);
	object->SetValue(L"myfunc", func, V8_PROPERTY_ATTRIBUTE_NONE);
	object->SetValue(L"test2", func2, V8_PROPERTY_ATTRIBUTE_NONE);
	object->SetValue(L"registercb", CefV8Value::CreateFunction(L"registercb", myhandler), V8_PROPERTY_ATTRIBUTE_NONE);
}

void SimpleApp::OnBrowserDestroyed(CefRefPtr<CefBrowser> browser)
{

}

bool SimpleApp::OnProcessMessageReceived(CefRefPtr<CefBrowser> browser, CefProcessId source_process, CefRefPtr<CefProcessMessage> message)
{
	if (message->GetName().compare(L"callback_func") == 0)
	{
		if (m_js_callback_func)
		{
			CefV8ValueList args;
			CefString str = message->GetArgumentList()->GetString(0);
			args.push_back(CefV8Value::CreateString(str));
			args.push_back(CefV8Value::CreateInt(message->GetArgumentList()->GetInt(1)));
			m_js_callback_func->ExecuteFunctionWithContext(m_js_callback_context, NULL, args);
		}
		return true;
	}
	return false;
}

bool SimpleApp::Execute(const CefString& name, CefRefPtr<CefV8Value> object, const CefV8ValueList& arguments, CefRefPtr<CefV8Value>& retval, CefString& exception)
{
	if (name.compare(L"testfunc") == 0)
	{
		COPYDATASTRUCT cds;
		const wchar_t* lpData = name.c_str();
		cds.lpData = (LPVOID)lpData;
		cds.cbData = (wcslen(lpData) + 1) * sizeof(WCHAR);
		if (m_hWndMain && IsWindow(m_hWndMain))
			::SendMessage(m_hWndMain, WM_COPYDATA, NULL, (LPARAM)&cds);
		return true;
	}
	else if (name.compare(L"testfunc2") == 0)
	{
		if (arguments.size() == 1 && arguments[0]->IsString())
		{
			COPYDATASTRUCT cds;
			std::wstring str;
			str = L"function name: ";
			str += name;
			str += L"   arg: ";
			str += arguments[0]->GetStringValue();
			LPCWSTR lpData = str.c_str();
			cds.lpData = (LPVOID)lpData;
			cds.cbData = (wcslen(lpData) + 1) * sizeof(WCHAR);
			if (m_hWndMain && IsWindow(m_hWndMain))
				::SendMessage(m_hWndMain, WM_COPYDATA, NULL, (LPARAM)&cds);

			return true;
		}
	}
	else if (name.compare(L"registercb") == 0)
	{
		if (arguments.size() == 1 && arguments[0]->IsFunction())
		{
			m_js_callback_func = arguments[0].get();
			m_js_callback_context = CefV8Context::GetCurrentContext();
			CefV8ValueList args;
			args.push_back(CefV8Value::CreateString(L"register call back function"));
			args.push_back(CefV8Value::CreateInt(0));
			m_js_callback_func->ExecuteFunctionWithContext(m_js_callback_context, NULL, args);
		}
		return true;
	}
	return false;
}
