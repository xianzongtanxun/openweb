// Suzuki.cpp : ����Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "Suzuki.h"
#include "MainFrame.h"
#include "SimpleCefApp.h"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);
	CefEnableHighDPISupport();
	//CefEnableHighDPISupport();
	::CoInitialize(NULL);
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
		//����ΪtrueĿ����ʹcef��browser ui�̺߳��ҵĳ�����̷߳��룬
		//���ǿ���ʹ��duilib����Ϣ����MessageLoop()�����ص���cef��CefRunMessageLoop();
		settings.single_process = true;
		settings.multi_threaded_message_loop = true;
		settings.windowless_rendering_enabled = true;
		bool bRet = CefInitialize(main_args, settings, app.get(), sandbox_info);
		int k = 0;
	}
	CPaintManagerUI::SetInstance(hInstance);
	CPaintManagerUI::SetResourcePath(CPaintManagerUI::GetInstancePath() + _T("skin"));//��ʼ��skinĿ¼

	CMainFrame* pMain = new CMainFrame();
	HWND hwnd = pMain->Create(NULL, _T("Suzuki"));
	app->SetMainFrame(pMain);
	ShowWindow(hwnd, SW_HIDE);
	SendMessage(hwnd, WM_MSG_SHOW, 0, 0);

	CPaintManagerUI::MessageLoop();
	CPaintManagerUI::Term();
	CefShutdown();
	::CoUninitialize();
    return 0;
}