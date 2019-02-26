#include "stdafx.h"
#include "SimpleCefApp.h"
#include <string>
#include "include/cef_browser.h"
#include "include/cef_command_line.h"
#include "include/wrapper/cef_helpers.h"
//#include "SimpleCefHandler.h"
#include "include/cef_v8.h"
#include "MainFrame.h"

CSimpleCefApp::CSimpleCefApp(CMainFrame* pMainFrame, HWND hwnd):m_hwnd(hwnd)
{
	//offset_ = 0;
	m_pMainFrame = pMainFrame;
	v8Handler_ = new CCefV8HandlerEx(m_pMainFrame, m_hwnd);
}


CSimpleCefApp::~CSimpleCefApp(void)
{
}

void CSimpleCefApp::SetMainFrame(CMainFrame* pMainFrame)
{
	m_pMainFrame = pMainFrame;
	if (v8Handler_)
	{
		v8Handler_->SetMainFrame(pMainFrame);
	}
}

CefRefPtr<CefBrowserProcessHandler> CSimpleCefApp::GetBrowserProcessHandler()
{
	return this;
}

CefRefPtr<CefRenderProcessHandler> CSimpleCefApp::GetRenderProcessHandler()
{
	return this;
}

void CSimpleCefApp::OnContextInitialized()
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

void CSimpleCefApp::OnBeforeCommandLineProcessing(const CefString& process_type, CefRefPtr<CefCommandLine> command_line)
{
	command_line->AppendSwitchWithValue("lang", "zh-CN");//设置中文
	command_line->AppendSwitchWithValue("enable-media-stream", "enable-media-stream");//开启摄像头
	//command_line->AppendSwitchWithValue("use-fake-ui-for-media-stream","1");
	//command_line->AppendSwitchWithValue("enable-usermedia-screen-capturing", "1");
	//command_line->AppendSwitchWithValue("enable-media-screen-capturing", "1");
	//command_line->AppendSwitch("process-per-site");
	//command_line->AppendSwitch("disable-web-security");
	//
	////command_line->AppendSwitch("disable-surfaces");
	//command_line->AppendSwitch("disable-gpu");
	//command_line->AppendSwitch("disable-gpu-compositing");
	//command_line->AppendSwitch("disable-accelerated-video-decode");
	//command_line->AppendSwitchWithValue("enable-begin-frame-scheduling","1");
}

//void CSimpleCefApp::Cancel() {
//	CEF_REQUIRE_IO_THREAD();
//}
//
//bool CSimpleCefApp::ProcessRequest(CefRefPtr<CefRequest> request,CefRefPtr<CefCallback> callback)
//{
//	CEF_REQUIRE_IO_THREAD();
//
//	bool handled = false;
//
//	std::string url = request->GetURL();
//	if (strstr(url.c_str(), "handler.html") != NULL) {
//		// Build the response html
//		data_ = "<html><head><title>Client Scheme Handler</title></head>"
//			"<body bgcolor=\"white\">"
//			"This contents of this page page are served by the "
//			"ClientSchemeHandler class handling the client:// protocol."
//			"<br/>You should see an image:"
//			"<br/><img src=\"client://tests/logo.png\"><pre>";
//
//		// Output a string representation of the request
//		const std::string& dump = DumpRequestContents(request);
//		data_.append(dump);
//
//		data_.append("</pre><br/>Try the test form:"
//			"<form method=\"POST\" action=\"handler.html\">"
//			"<input type=\"text\" name=\"field1\">"
//			"<input type=\"text\" name=\"field2\">"
//			"<input type=\"submit\">"
//			"</form></body></html>");
//
//		handled = true;
//
//		// Set the resulting mime type
//		mime_type_ = "text/html";
//	}
//	else if (strstr(url.c_str(), "logo.png") != NULL) {
//		// Load the response image
//		//if (LoadBinaryResource("logo.png", data_)) {
//		//	handled = true;
//		//	// Set the resulting mime type
//		//	mime_type_ = "image/png";
//		//}
//	}
//
//	if (handled) {
//		// Indicate the headers are available.
//		callback->Continue();
//		return true;
//	}
//
//	return false;
//}
//
//std::string CSimpleCefApp::DumpRequestContents(CefRefPtr<CefRequest> request)
//{
//	std::stringstream ss;
//
//	ss << "URL: " << std::string(request->GetURL());
//	ss << "\nMethod: " << std::string(request->GetMethod());
//
//	CefRequest::HeaderMap headerMap;
//	request->GetHeaderMap(headerMap);
//	if (headerMap.size() > 0) {
//		ss << "\nHeaders:";
//		CefRequest::HeaderMap::const_iterator it = headerMap.begin();
//		for (; it != headerMap.end(); ++it) {
//			ss << "\n\t" << std::string((*it).first) << ": " <<
//				std::string((*it).second);
//		}
//	}
//
//	CefRefPtr<CefPostData> postData = request->GetPostData();
//	if (postData.get()) {
//		CefPostData::ElementVector elements;
//		postData->GetElements(elements);
//		if (elements.size() > 0) {
//			ss << "\nPost Data:";
//			CefRefPtr<CefPostDataElement> element;
//			CefPostData::ElementVector::const_iterator it = elements.begin();
//			for (; it != elements.end(); ++it) {
//				element = (*it);
//				if (element->GetType() == PDE_TYPE_BYTES) {
//					// the element is composed of bytes
//					ss << "\n\tBytes: ";
//					if (element->GetBytesCount() == 0) {
//						ss << "(empty)";
//					}
//					else {
//						// retrieve the data.
//						size_t size = element->GetBytesCount();
//						char* bytes = new char[size];
//						element->GetBytes(size, bytes);
//						ss << std::string(bytes, size);
//						delete[] bytes;
//					}
//				}
//				else if (element->GetType() == PDE_TYPE_FILE) {
//					ss << "\n\tFile: " << std::string(element->GetFile());
//				}
//			}
//		}
//	}
//
//	return ss.str();
//}
//
//void CSimpleCefApp::GetResponseHeaders(CefRefPtr<CefResponse> response,	int64& response_length,	CefString& redirectUrl)
//{
//	CEF_REQUIRE_IO_THREAD();
//
//	DCHECK(!data_.empty());
//
//	response->SetMimeType(mime_type_);
//	response->SetStatus(200);
//
//	// Set the resulting response length
//	response_length = data_.length();
//}
//
//bool CSimpleCefApp::ReadResponse(void* data_out,int bytes_to_read,int& bytes_read,CefRefPtr<CefCallback> callback)
//{
//	CEF_REQUIRE_IO_THREAD();
//
//	bool has_data = false;
//	bytes_read = 0;
//
//	if (offset_ < data_.length()) {
//		// Copy the next block of data into the buffer.
//		int transfer_size = min(bytes_to_read, static_cast<int>(data_.length() - offset_));
//		memcpy(data_out, data_.c_str() + offset_, transfer_size);
//		offset_ += transfer_size;
//
//		bytes_read = transfer_size;
//		has_data = true;
//	}
//
//	return has_data;
//}

void CSimpleCefApp::OnWebKitInitialized()
{
	//return;
	//js注册部分
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

	std::string extensionCode4 =
		"var test;"
		"if(!test)"
		"	test = {};"
		"(function(){"
		"	test.testfun = function(){"
		"		native function testfun();"
		"		return clientStart();"
		"	};"
		"})();";

	std::string extensionCode5 =
		"var test;"
		"if(!test)"
		"	test = {};"
		"(function(){"
		"	test.teststop = function(){"
		"		native function teststop();"
		"		return clientStop();"
		"	};"
		"})();";

		//申明本地函数native function hehe() 如果有参数列表需要些具体的类型，而不能写var类型！与本地申明一致
		//调用本地函数 return hehe()
		//create an instance of my CefV8Handler object

	//	CefRefPtr<CefV8Handler> handler
	//javascript里调用app.jsInvokeCplusPlus时，就回去通过CefRegisterExtension注册的CefV8Handler列表查找
	//找到“v8/app”对应的CCEFV8HandlerEx,就调用它的Execute方法
	//假设v8Handler_是DuiCefApp的一个成员变量
	//v8Handler_ = new CCEFV8HandlerEx()
	//register the extension
	if (v8Handler_ == NULL)
	{
		v8Handler_ = new CCefV8HandlerEx(m_pMainFrame, m_hwnd);
	}
	bool bRet = CefRegisterExtension("v8/test", extensionCode4, v8Handler_);
	//bool bRet = CefRegisterExtension("v8/cloudAccountClient", extensionCode, v8Handler_);
	int k = 0;

}

//void CSimpleCefApp::UploadFileApp()
//{
//	if (v8Handler_){
//		v8Handler_->UploadFile();
//	}
//}

void CSimpleCefApp::OnContextCreated(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefV8Context> context)
{
	OutputDebugString(_T("ClientAppRenderer::OnContextCreated, create window binding\r\n"));

	// Retrieve the context's window object.
	CefRefPtr<CefV8Value> object = context->GetGlobal();
	/////////////////////////////////////////////////////////////////////////
	CefRefPtr<CefV8Value> strValue = CefV8Value::CreateString(_T("My Value11!"));
	object->SetValue(_T("myval"), strValue, V8_PROPERTY_ATTRIBUTE_NONE);
	//bind function
	CefRefPtr<CefV8Value> func1 = CefV8Value::CreateFunction(_T("addFunction"), v8Handler_);
	object->SetValue("addFunction", func1, V8_PROPERTY_ATTRIBUTE_NONE);

	CefRefPtr<CefV8Value> func2 = CefV8Value::CreateFunction(_T("register"), v8Handler_);
	object->SetValue("register", func2, V8_PROPERTY_ATTRIBUTE_NONE);

	CefRefPtr<CefV8Value> func3 = CefV8Value::CreateFunction(_T("clientStart"), v8Handler_);
	object->SetValue("clientStart", func3, V8_PROPERTY_ATTRIBUTE_NONE);

	CefRefPtr<CefV8Value> func4 = CefV8Value::CreateFunction(_T("clientStop"), v8Handler_);
	object->SetValue("clientStop", func4, V8_PROPERTY_ATTRIBUTE_NONE);
	/*CefRefPtr<CefV8Value> func4 = CefV8Value::CreateFunction(_T("handleSubmitIcbcVideo"), v8Handler_);
	object->SetValue("handleSubmitIcbcVideo", func4, V8_PROPERTY_ATTRIBUTE_NONE);*/

	/*CefRefPtr<CefV8Value> func5 = CefV8Value::CreateFunction(_T("testupload"), v8Handler_);
	object->SetValue("testupload", func5, V8_PROPERTY_ATTRIBUTE_NONE);*/
}
