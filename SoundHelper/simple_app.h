// Copyright (c) 2013 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#ifndef CEF_TESTS_CEFSIMPLE_SIMPLE_APP_H_
#define CEF_TESTS_CEFSIMPLE_SIMPLE_APP_H_

#include "include/cef_app.h"

// Implement application-level callbacks for the browser process.
class SimpleApp : public CefApp,
				  public CefRenderProcessHandler,
				  public CefV8Handler,
                  public CefBrowserProcessHandler {
 public:
  SimpleApp();

  // CefApp methods:
  virtual CefRefPtr<CefBrowserProcessHandler> GetBrowserProcessHandler()
      OVERRIDE { return this; }

  // CefBrowserProcessHandler methods:
  virtual void OnContextInitialized() OVERRIDE;

  virtual void OnContextCreated(CefRefPtr<CefBrowser> browser,
	  CefRefPtr<CefFrame> frame,
	  CefRefPtr<CefV8Context> context);

  virtual void OnBrowserDestroyed(CefRefPtr<CefBrowser> browser);

  virtual bool OnProcessMessageReceived(CefRefPtr<CefBrowser> browser,
	  CefProcessId source_process,
	  CefRefPtr<CefProcessMessage> message);

  virtual bool Execute(const CefString& name,
	  CefRefPtr<CefV8Value> object,
	  const CefV8ValueList& arguments,
	  CefRefPtr<CefV8Value>& retval,
	  CefString& exception);
 private:
  // Include the default reference counting implementation.
	 HWND m_hWndMain; 
	 CefRefPtr<CefV8Value> m_js_callback_func;
	 CefRefPtr<CefV8Context> m_js_callback_context;
  IMPLEMENT_REFCOUNTING(SimpleApp);
};

#endif  // CEF_TESTS_CEFSIMPLE_SIMPLE_APP_H_
