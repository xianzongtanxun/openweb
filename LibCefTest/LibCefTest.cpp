// LibCefTest.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "LibCefTest.h"
#include "MainFrame.h"
//#include "include/cef_sandbox_win.h"
#include "DuiCefApp.h"

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
	MessageBox(NULL, _T("test"), _T("caption"), MB_OKCANCEL);
	CefEnableHighDPISupport();
	::CoInitialize(NULL);
	void* sandbox_info = NULL;
	//CefScopedSandboxInfo scoped_sandbox;
	//sandbox_info = scoped_sandbox.sandbox_info();
	CefMainArgs main_args(hInstance);
	CefRefPtr<CDuiCefApp> app(new CDuiCefApp);
	if (CefExecuteProcess(main_args,app.get(),sandbox_info) == -1)
	{
		CefSettings settings;
		settings.no_sandbox = true;
#ifdef _DEBUG
		settings.single_process = true;
#endif
		//设置为true目的是使cef的browser ui线程和我的程序的线程分离，
		//我们可以使用duilib的消息函数MessageLoop()而不必调用cef的CefRunMessageLoop();
		settings.multi_threaded_message_loop = true;
		bool bRet = CefInitialize(main_args,settings,app.get(),sandbox_info);
		int k = 0;
	}

	CPaintManagerUI::SetInstance(hInstance);
	CPaintManagerUI::SetResourcePath(CPaintManagerUI::GetInstancePath() + _T("skin"));//初始化skin目录

	CMainFrame* pMain = new CMainFrame();
	HWND hwnd = pMain->Create(NULL, _T("MainFrame"));
	//pMain->ShowWindow();
	//pMain->CenterWindow();
	ShowWindow(hwnd, SW_HIDE);
	SendMessage(hwnd, WM_MSG_SHOW, 0, 0);
	CPaintManagerUI::MessageLoop();
	CPaintManagerUI::Term();
	//CefShutdown();
	::CoUninitialize();
	return 0;
}
