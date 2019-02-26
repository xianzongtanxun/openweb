#include "stdafx.h"
#include "DuiCefApp.h"
#include <string>
#include "include/cef_browser.h"
#include "include/cef_command_line.h"
#include "include/wrapper/cef_helpers.h"
#include "DuiMsgHandler.h"
#include "include/cef_v8.h"

CDuiCefApp::CDuiCefApp(HWND hwnd):m_hwnd(hwnd)
{
	//v8Handler_ = new CCEFV8HandlerEx;
}


CDuiCefApp::~CDuiCefApp(void)
{
}

CefRefPtr<CefBrowserProcessHandler> CDuiCefApp::GetBrowserProcessHandler()
{
	return this;
}

CefRefPtr<CefRenderProcessHandler> CDuiCefApp::GetRenderProcessHandler()
{
	return this;
}

void CDuiCefApp::OnContextInitialized()
{
	Sleep(0);
	//CefWindowInfo windows_info;
	//RECT rc;
	//::GetClientRect(m_hwnd,&rc);
	//windows_info.SetAsChild(m_hwnd,rc);
	////windows_info.SetAsPopup(NULL, _T("FrameCef"));
	//CefRefPtr<CDuiMsgHandler> handler(new CDuiMsgHandler());
	//CefBrowserSettings browser_settings;
	//browser_settings.default_encoding.str = _T("GB2312");
	//browser_settings.default_encoding.length = wcslen(_T("GB2312"));
	//std::string url;
	//CefRefPtr<CefCommandLine> command_line = CefCommandLine::GetGlobalCommandLine();
	//url = command_line->GetSwitchValue("url");
	//if (url.empty()){
	//	url = "http://www.baidu.com";
	//}
	//CefBrowserHost::CreateBrowser(windows_info,handler.get(),url,browser_settings,NULL);
}

void CDuiCefApp::OnBeforeCommandLineProcessing(const CefString& process_type, CefRefPtr<CefCommandLine> command_line)
{
	command_line->AppendSwitchWithValue("--lang", "zh-CN");//��������
}

void CDuiCefApp::OnWebKitInitialized()
{
	return;
	//jsע�Ჿ��
	std::string extensionCode1 =
		"var test;"
		"if(!test)"
		"	test = {};"
		"(function(){"
		"	test.myval = 'My Value!';"
		"})();";

	std::string extensionCode2 =
		"var test;"
		"if (!test)"
		"	test = {};"
		"(function() {"
		"	test.myfunc = function(){"
		"		native function myfunc();"
		"		return myfunc();"
		"	};"
		"})();";

	std::string extensionCode3 =
		"var g_value= global value hehe;"
		"var test;"
		"if(!test)"
		"	test = {};"
		"(function(){"
		"	test.myfunc = function(){"
		"		native function myfunc(int,int);"
		"		return myfunc(10,50);"
		"	};"
		"})();";
		//�������غ���native function hehe() ����в����б���ҪЩ��������ͣ�������дvar���ͣ��뱾������һ��
		//���ñ��غ��� return hehe()
		//create an instance of my CefV8Handler object

	//	CefRefPtr<CefV8Handler> handler
	//javascript�����app.jsInvokeCplusPlusʱ���ͻ�ȥͨ��CefRegisterExtensionע���CefV8Handler�б����
	//�ҵ���v8/app����Ӧ��CCEFV8HandlerEx,�͵�������Execute����
	//����v8Handler_��DuiCefApp��һ����Ա����
	//v8Handler_ = new CCEFV8HandlerEx()
	//register the extension
	bool bRet = CefRegisterExtension("v8/test", extensionCode2, v8Handler_);
	//bool bRet = CefRegisterExtension("v8/cloudAccountClient", extensionCode, v8Handler_);
	int k = 0;

}

void CDuiCefApp::OnContextCreated(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefV8Context> context)
{
	OutputDebugString(_T("ClientAppRenderer::OnContextCreated, create window binding\r\n"));

	// Retrieve the context's window object.
	CefRefPtr<CefV8Value> object = context->GetGlobal();
	/////////////////////////////////////////////////////////////////////////
	CefRefPtr<CefV8Value> strValue = CefV8Value::CreateString(_T("My Value11!"));
	object->SetValue(_T("myval"), strValue, V8_PROPERTY_ATTRIBUTE_NONE);
	v8Handler_ = new CCEFV8HandlerEx;
	//bind function
	CefRefPtr<CefV8Value> func1 = CefV8Value::CreateFunction(_T("addFunction"), v8Handler_);
	object->SetValue("addFunction", func1, V8_PROPERTY_ATTRIBUTE_NONE);

	CefRefPtr<CefV8Value> func2 = CefV8Value::CreateFunction(_T("register"), v8Handler_);
	object->SetValue("register", func2, V8_PROPERTY_ATTRIBUTE_NONE);

	CefRefPtr<CefV8Value> func3 = CefV8Value::CreateFunction(_T("myfunc"), v8Handler_);
	object->SetValue("myfunc", func2, V8_PROPERTY_ATTRIBUTE_NONE);
}
