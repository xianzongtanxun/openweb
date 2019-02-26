// SoundHelper.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "SoundHelper.h"
#include "MainFrame.h"
#include "simple_app.h"
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

	CefEnableHighDPISupport();
	HRESULT hr = ::CoInitialize(NULL);
	void* sanbox_info = NULL;
	CefMainArgs main_args(hInstance);
	CefRefPtr<SimpleApp> app(new SimpleApp);
	if (CefExecuteProcess(main_args,app.get(),sanbox_info) == -1)
	{
		CefSettings settings;
		settings.no_sandbox = true;
		settings.multi_threaded_message_loop = true;
		//设置ture的目的是使cef的browser ui线程和程序的 线程分离
		//我们可以使用duilib的消息函数MessageLoop()而不必调用cef的CefRunMessageLoop()
		CefInitialize(main_args, settings, app.get(), sanbox_info);
	}
	
    // TODO: 在此放置代码。
	CPaintManagerUI::SetInstance(hInstance);
	CPaintManagerUI::SetResourcePath(CPaintManagerUI::GetInstancePath() + _T("skin"));//初始化skin目录
	// TODO: 在此放置代码。
	CMainFrame* pMain = new CMainFrame();
	if (pMain)
	{
		HWND hwnd = pMain->Create(NULL, _T("WaveIn"));
		pMain->CenterWindow();
		//ShowWindow(hwnd, SW_HIDE);
		SendMessage(hwnd, WM_MSG_SHOW, 0, 0);
	}

	CPaintManagerUI::MessageLoop();
	CefShutdown();
	CPaintManagerUI::Term();
	::CoUninitialize();
   
    return 0;
}
