// TGWeb.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "TGWeb.h"
#include "MainFrame.h"
#include "SimpleCefApp.h"

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	int argc = 0;
	LPWSTR *argv = NULL;
	argv = CommandLineToArgvW(::GetCommandLineW(), &argc);
	if (argc < 3){
		LocalFree(argv);
		return 0;
	}
	wstring strParam1 = argv[1];
	wstring strParam2;
	if (argc == 3){
		if (!strParam1.empty() && (_tcscmp(argv[2], _T("-tgweb")) == 0)) {
			int k = 0;
		}
		else{
			return 0;
		}
	}
	else if (argc == 4){
		strParam2 = argv[2];
		if (!strParam1.empty() && !strParam2.empty() && (_tcscmp(argv[3], _T("-tgweb")) == 0)) {
			int k = 0;
		}
		else{
			return 0;
		}
	}
	else{
		return 0;
	}
	HANDLE hEvent = CreateEvent(NULL, FALSE, TRUE, _T("tgweb"));
	if (GetLastError() == ERROR_ALREADY_EXISTS) {
		HWND hwndSnd = ::FindWindow(NULL, _T("TGBrowser"));
		if (IsWindow(hwndSnd))
		{
			//PostMessage(hwndSnd, WM_TRAY_MENU_NOTIFY, WM_MENU_SHOW, 0);
			OutputDebugStringA("\n找到窗体\n");
		}
		::TerminateProcess(::GetCurrentProcess(), 0L);
		return 0;
	}
	LocalFree(argv);

	::CoInitialize(NULL);
	::OleInitialize(NULL);
	void* sandbox_info = NULL;
#if defined(CEF_USE_SANDBOX)
	// Manage the life span of the sandbox information object. This is necessary
	// for sandbox support on Windows. See cef_sandbox_win.h for complete details.
	CefScopedSandboxInfo scoped_sandbox;
	sandbox_info = scoped_sandbox.sandbox_info();
#endif
	CefMainArgs main_args(hInstance);
	CefRefPtr<CSimpleCefApp> app(new CSimpleCefApp(NULL));
	if (CefExecuteProcess(main_args, app.get(), sandbox_info) == -1)
	{
		CefSettings settings;
		settings.no_sandbox = true;
#ifdef _DEBUG
		settings.single_process = true;
#endif
		//设置为true目的是使cef的browser ui线程和我的程序的线程分离，
		//我们可以使用duilib的消息函数MessageLoop()而不必调用cef的CefRunMessageLoop();
		settings.single_process = true;
		settings.multi_threaded_message_loop = true;
		settings.windowless_rendering_enabled = true;
		bool bRet = CefInitialize(main_args, settings, app.get(), sandbox_info);
		int k = 0;
	}
	CPaintManagerUI::SetInstance(hInstance);
	CPaintManagerUI::SetResourcePath(CPaintManagerUI::GetInstancePath() + _T("skin"));//初始化skin目录
	USES_CONVERSION;
	CMainFrame* pMain = new CMainFrame(W2A(strParam1.c_str()), W2A(strParam2.c_str()));
	HWND hwnd = pMain->Create(NULL, _T("TGBrowser"));
	//app->SetMainFrame(pMain);
	ShowWindow(hwnd, SW_HIDE);
	SendMessage(hwnd, WM_MSG_SHOW, 0, 0);

	CPaintManagerUI::MessageLoop();
	CPaintManagerUI::Term();
	CefShutdown();
	::CoUninitialize();
	::OleUninitialize();
	return 0;
}
