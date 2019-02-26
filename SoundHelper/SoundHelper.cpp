// SoundHelper.cpp : ����Ӧ�ó������ڵ㡣
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
		//����ture��Ŀ����ʹcef��browser ui�̺߳ͳ���� �̷߳���
		//���ǿ���ʹ��duilib����Ϣ����MessageLoop()�����ص���cef��CefRunMessageLoop()
		CefInitialize(main_args, settings, app.get(), sanbox_info);
	}
	
    // TODO: �ڴ˷��ô��롣
	CPaintManagerUI::SetInstance(hInstance);
	CPaintManagerUI::SetResourcePath(CPaintManagerUI::GetInstancePath() + _T("skin"));//��ʼ��skinĿ¼
	// TODO: �ڴ˷��ô��롣
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
