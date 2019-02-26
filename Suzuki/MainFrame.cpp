#include "stdafx.h"
#include <commdlg.h>
#include <shellapi.h>
#include <WinUser.h>
#include "MainFrame.h"
#include "Utils\UIDelegate.h"
#include "include\cef_app.h"
#include "SimpleCefApp.h"
#include "sound_handler.h"
#include "SimpleCefApp.h"

///////////////////////////
CMainFrame::CMainFrame():
	m_pLblTitle(nullptr),
	m_edtUrl(nullptr),   
	//m_pBtnBackup(nullptr),      
	//m_pBtnForword(nullptr),     
	m_pBtnGo(nullptr),
	//m_pBtnRestore(nullptr),
	//m_pBtnMax(nullptr),
	m_pBtnClose(nullptr)
{
	m_fpCapture = nullptr;
	m_fpRender = nullptr;
	m_myAudioSink = nullptr;
	m_myAudioSinkRender = nullptr;
	m_pSoundCaptureHandler = nullptr;
	m_pSoundRenderHandler = nullptr;
}

CMainFrame::~CMainFrame()
{
	
}

void CMainFrame::InitWindow()
{
	m_pLblTitle   = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("lbl_title")));
	m_edtUrl      = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("edt_path")));
	//m_pBtnBackup  = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btn_backup")));
	//m_pBtnForword = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btn_forword")));
	m_pBtnGo      = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btn_go")));
	//m_pBtnRestore = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btnrestore")));
	//m_pBtnMax     = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btnmax")));
	m_pBtnClose   = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btnclose")));
	m_pContainer  = static_cast<CContainerUI*>(m_PaintManager.FindControl(_T("container")));
	//支持文件拖拽功能

	ASSERT(m_pLblTitle);
	ASSERT(m_edtUrl);
	//ASSERT(m_pBtnBackup);
	//ASSERT(m_pBtnForword);
	ASSERT(m_pBtnGo);
	//ASSERT(m_pBtnRestore);
	//ASSERT(m_pBtnMax);
	ASSERT(m_pBtnClose);
	ASSERT(m_pContainer);

	//m_pBtnRestore->SetVisible(true);
	//m_pBtnMax->SetVisible(false);
	::DragAcceptFiles(GetHWND(),true);

	//void* sandbox_info = NULL;
	//CefMainArgs main_args(m_PaintManager.GetInstance());
	//m_cefApp = new CSimpleCefApp(this);
	//if (CefExecuteProcess(main_args, m_cefApp, sandbox_info) == -1)
	//{
	//	CefSettings settings;
	//	settings.no_sandbox = true;
	//#ifdef _DEBUG
	//	settings.single_process = true;
	//#endif
	//	//设置为true目的是使cef的browser ui线程和我的程序的线程分离，
	//	//我们可以使用duilib的消息函数MessageLoop()而不必调用cef的CefRunMessageLoop();
	//	settings.multi_threaded_message_loop = true;
	//	bool bRet = CefInitialize(main_args, settings, m_cefApp, sandbox_info);
	//	int k = 0;
	//}
	
	//OnWndSizeMax(true);
	std::thread th(&CMainFrame::ThDeleteFileThread, this);
	th.detach();
	int k = 0;
}

HWND CMainFrame::Create(HWND hwndParent, LPCTSTR pstrName)
{
	//UI_WNDSTYLE_DIALOG
	//UI_WNDSTYLE_DIALOG | WS_MAXIMIZEBOX
	HWND hwnd = __super::Create(hwndParent, pstrName, UI_WNDSTYLE_DIALOG & ~WS_MAXIMIZEBOX, WS_EX_WINDOWEDGE, 0, 0, 0, 0);
	m_ShaowHelp.Init(hwnd);
	return hwnd;
}

void CMainFrame::SetCaption(wstring sCaption)
{
	if (m_pLblTitle){
		m_pLblTitle->SetText(sCaption.c_str());
	}
}

DuiLib::CDuiString CMainFrame::GetSkinFile()
{
	return _T("MainSuzuki.xml");
}

UINT CMainFrame::GetClassStyle() const
{
	return UI_CLASSSTYLE_DIALOG;
}

void CMainFrame::OnFinalMessage(HWND hWnd)
{
	__super::OnFinalMessage(hWnd);
}

DuiLib::CDuiString CMainFrame::GetSkinFolder()
{
	return _T("skin");
}

UILIB_RESOURCETYPE CMainFrame::GetResourceType() const
{
	return UILIB_FILE;
}

LRESULT CMainFrame::MessageHandler(UINT uMsg, WPARAM wParam, LPARAM /*lParam*/, bool& /*bHandled*/)
{
	if (uMsg == WM_KEYDOWN){
		if (wParam == VK_RETURN || wParam == VK_ESCAPE){
			//pWindow->put_FullScreenMode(OAFALSE);
		}
	}
	return true;
}

LRESULT CMainFrame::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT lRes = 0;
	BOOL bHandled = FALSE;
	switch (uMsg)
	{
	case WM_TIMER:
		{
		}
		break;
	case WM_GRAPHNOTIFY:
		{
			
		}
		break;
	case WM_MSG_SHOW:
		{
			CenterWindow();
			::ShowWindow(GetHWND(), SW_SHOWMAXIMIZED);
			::PostMessage(GetHWND(), WM_MSG_REFRESH_SHADOW, 0, 0);
			m_handler = new CSimpleCefHandler(this);
			CefBrowserSettings browser_settings;
			std::string url = "https://suzuki-dev-finance.haimaiche.net/#/login";
			//最大化
			CefWindowInfo windows_info;
			RECT rc;
			::GetClientRect(GetHWND(), &rc);
			rc.top = rc.top + 80;
			windows_info.SetAsChild(GetHWND(), rc);
			bool bRet = CefBrowserHost::CreateBrowser(windows_info, m_handler, url, browser_settings, NULL);
		}
		break;
	case WM_MSG_REFRESH_SHADOW:
		{
			m_ShaowHelp.ReFlush(GetHWND());
		}
		break;
	case WM_COPYDATA:
		{
			OutputDebugStringA("\n");
			OutputDebugStringA("client WM_COPYDATA");
			OutputDebugStringA("\n");
			COPYDATASTRUCT* pCopyData = (COPYDATASTRUCT*)lParam;
			wchar_t *pData = new wchar_t[pCopyData->cbData + 1];
			memset(pData, 0, pCopyData->cbData + 1);
			memcpy_s(pData, pCopyData->cbData, (char *)pCopyData->lpData, pCopyData->cbData);
			pData[pCopyData->cbData] = '\0';
			wstring strNameData = pData;
			if (strNameData == _T("clientStop"))
			{
				OutputDebugStringA("\n");
				OutputDebugStringA("clientStop prepare StopFromJs");
				OutputDebugStringA("\n");
				::PostMessage(GetHWND(), WM_COPY_STOP, 0, 0);
			}
			else if (strNameData.find(_T("clientStart")) != string::npos)
			{
				std::vector<std::string> vCurrentInfo;//当前时间信息
				USES_CONVERSION;
				string strNameDataA = W2A(strNameData.c_str());
				GetKeyPaths(strNameDataA, "$", vCurrentInfo);
				OutputDebugStringA("\n");
				OutputDebugStringA("clientStart prepare StartFromJsWithFileName");
				OutputDebugStringA(strNameDataA.c_str());
				OutputDebugStringA("\n");
				if (vCurrentInfo.size() == 5)
				{
					string sParam0 = vCurrentInfo[0];
					string sParam1 = vCurrentInfo[1];
					string sParam2 = vCurrentInfo[2];
					string sParam3 = vCurrentInfo[3];
					string sParam4 = vCurrentInfo[4];
					StartFromJsWithFileName(sParam1, sParam2, sParam3, sParam4);
				}
				else
				{
					OutputDebugStringA("\n");
					OutputDebugStringA("clientStart StartFromJsWithFileName param number error!");
					OutputDebugStringA("\n");
				}
			}
		}
		break;
	case WM_COPY_STOP:
		{
			OutputDebugStringA("\n");
			OutputDebugStringA("WM_COPY_STOP");
			OutputDebugStringA("\n");
			StopFromJs();
		}
		break;
	case WM_DROPFILES:
		{
			//拖拽消息
			LPTSTR pFilePathName = (LPTSTR)malloc(500);
			HDROP hDropInfo = (HDROP)wParam;
			::DragQueryFile(hDropInfo,0,pFilePathName,500);
			m_edtUrl->SetText(pFilePathName);
			m_handler->GetBrowser()->GetMainFrame()->LoadURL(pFilePathName);
			::DragFinish(hDropInfo);
			free(pFilePathName);
		}
		break;
	case WM_SIZE:
		{
			return OnSize(uMsg, wParam, lParam, bHandled);
		}
	default:
		break;
	}
	if (bHandled) return lRes;
	return __super::HandleMessage(uMsg, wParam, lParam);
}

LRESULT CMainFrame::OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = true;	
	if (!m_handler){
		return false;
	}
	RECT rc;
	::GetClientRect(GetHWND(), &rc);
	m_pBtnClose->SetFloat(true);
	rc.bottom = rc.top + 36;
	rc.left = rc.right - 30;
	m_pBtnClose->SetPos(rc);
	if (::IsZoomed(GetHWND())){
		//m_pBtnMax->SetVisible(false);
		//m_pBtnRestore->SetVisible(true);
		//m_pBtnRestore->SetFloat(true);
		rc.left = rc.left - 30;
		rc.right = rc.left + 30;
		//m_pBtnRestore->SetPos(rc);
	}
	else{
		//m_pBtnMax->SetVisible(true);
		//m_pBtnRestore->SetVisible(false);
		//m_pBtnMax->SetFloat(true);
		rc.left = rc.left - 30;
		rc.right = rc.left + 30;
		//m_pBtnMax->SetPos(rc);
	}
	
	CefRefPtr<CefBrowser> browser = m_handler->GetBrowser();
	if (browser.get())
	{
		HWND hwnd_ = browser->GetHost()->GetWindowHandle();
		if (hwnd_)
		{
			RECT rcCef;
			::GetClientRect(GetHWND(), &rcCef);
			rcCef.top = rcCef.top + 80;
			::MoveWindow(hwnd_, rcCef.left, rcCef.top, rcCef.right - rcCef.left, rcCef.bottom - rcCef.top, TRUE);
		}
	}
	return true;
}

void CMainFrame::Notify(TNotifyUI& msg)
{
	if(msg.sType == DUI_MSGTYPE_CLICK)
	{
		if (msg.pSender == m_pBtnGo)
		{
			wstring str = m_edtUrl->GetText();
			if (!str.empty() && m_handler){
				m_handler->GetBrowser()->GetMainFrame()->LoadURL(str);
			}
			return;
		}
		/*else if (msg.pSender == m_pBtnRestore)
		{
			if (::IsZoomed(GetHWND())) {
				::ShowWindow(GetHWND(), SW_RESTORE);
				::PostMessage(GetHWND(), WM_MSG_REFRESH_SHADOW, 0, 0);
			}
		}
		else if (msg.pSender == m_pBtnMax)
		{
			if (!::IsZoomed(GetHWND()))
			{
				::ShowWindow(GetHWND(), SW_SHOWMAXIMIZED);
				::PostMessage(GetHWND(), WM_MSG_REFRESH_SHADOW, 0, 0);
				m_PaintManager.Invalidate();
			}
		}
		else */
		if (msg.pSender == m_pBtnClose)
		{
			Close(IDCANCEL);
			if (m_handler){
				m_handler->CloseAllBrowser(true);
			}
			CefShutdown();
			::PostQuitMessage(0);
			return;
		}
	}
}

void CMainFrame::GetKeyPaths(string strPath, string delimiter, std::vector<string>& parts)
{
	char* strInstId = (char*)strPath.c_str();
	char* part = new char[50];
	part = strtok_s(strInstId, delimiter.c_str(), &strInstId);
	parts.push_back(part);
	while (nullptr != (part = strtok_s(NULL, delimiter.c_str(), &strInstId))) {
		parts.push_back(part);
	}
	delete[]part;
}

string CMainFrame::GetRecorderFileFolder()
{
	static string sPath;
	if (!sPath.empty()) {
		return sPath;
	}
	char buffer[MAX_PATH] = { 0 };
	ULONG name_size = ::GetModuleFileNameA(NULL, buffer, MAX_PATH);
	if (name_size == 0) {
		return false;
	}
	::PathRemoveFileSpecA(buffer);
	string sFilePath = buffer;
	sFilePath.append("\\temp\\");
	//replace(sFilePath.begin(), sFilePath.end(), '\\', '/');
	if (!::PathFileExistsA(sFilePath.c_str())) {
		::CreateDirectoryA(sFilePath.c_str(), NULL);
	}
	sPath = sFilePath;
	return sPath;
}

void CMainFrame::StartCapture(const string& sName)
{
	if (!sName.empty())
	{
		if (m_fpCapture) {
			fclose(m_fpCapture);
			m_fpCapture = nullptr;
		}
		fopen_s(&m_fpCapture, sName.c_str(), "wb");
		m_myAudioSink = new MyAudioSink(m_fpCapture);
		if (!m_pSoundCaptureHandler){
			m_pSoundCaptureHandler = new CSoundCaptureHandler();
		}
		m_pSoundCaptureHandler->Initialize(m_myAudioSink, true);
		if (!m_pSoundCaptureHandler->StartRecord()){
			MessageBoxA(GetHWND(), "启动录制麦克风失败！", "提示",MB_OKCANCEL);
		}
	}
}

void CMainFrame::StopCapture()
{
	OutputDebugStringA("m_pSoundCaptureHandler");
	if (m_pSoundCaptureHandler)
	{
		m_pSoundCaptureHandler->StopRecord();
		//m_pSoundCaptureHandler->ReleaseAll();
		delete m_pSoundCaptureHandler;
		m_pSoundCaptureHandler = nullptr;
	}
	OutputDebugStringA("m_fpCapture");
	m_mutex.lock();
	if (m_fpCapture) {
		fclose(m_fpCapture);
		m_fpCapture = nullptr;
	}
	m_mutex.unlock();
	OutputDebugStringA("m_myAudioSink");
	if (m_myAudioSink) {
		delete m_myAudioSink;
		m_myAudioSink = nullptr;
	}
}

void CMainFrame::StartRender(const string& sName)
{
	if (!sName.empty())
	{
		if (m_fpRender) {
			fclose(m_fpRender);
			m_fpRender = nullptr;
		}
		fopen_s(&m_fpRender, sName.c_str(), "wb");
		m_myAudioSinkRender = new MyAudioSink(m_fpRender);
		if (!m_pSoundRenderHandler){
			m_pSoundRenderHandler = new CSoundRenderHandler();
		}
		m_pSoundRenderHandler->Initialize(m_myAudioSinkRender, false);
		if (!m_pSoundRenderHandler->StartRecord()) {
			MessageBoxA(GetHWND(), "启动录制扬声器失败！", "提示", MB_OKCANCEL);
		}
	}
}

void CMainFrame::StopRender()
{
	OutputDebugStringA("m_pSoundRenderHandler");
	if (m_pSoundRenderHandler){
		m_pSoundRenderHandler->StopRecord();
		//m_pSoundRenderHandler->ReleaseAll();
		delete m_pSoundRenderHandler;
		m_pSoundRenderHandler = nullptr;
	}
	OutputDebugStringA("m_fpRender");
	m_mutex.lock();
	if (m_fpRender) {
		fclose(m_fpRender);
		m_fpRender = nullptr;
	}
	m_mutex.unlock();
	OutputDebugStringA("m_myAudioSinkRender");
	if (m_myAudioSinkRender) {
		delete m_myAudioSinkRender;
		m_myAudioSinkRender = nullptr;
	}
}

void CMainFrame::StartFromJsWithFileName(const string& sFileCapture, const string& sFileRender, const string& sToken, const string& sObjectName)
{
	OutputDebugStringA("\n");
	OutputDebugStringA("client StartFromJsWithFileName");
	OutputDebugStringA("\n");
	USES_CONVERSION;
	string sPath = GetRecorderFileFolder();
	string strNameCapture = sPath + sFileCapture.c_str()+ ".pcm";// "foobarcapture.pcm";
	string strNameRender = sPath + sFileRender.c_str() + ".pcm";// "foobarcapture.pcm";
	m_mutex.lock();
	m_strCapturePath = strNameCapture;
	m_strRenderPath = strNameRender;
	m_strToken = sToken;
	m_strObjectName = sObjectName;
	m_mutex.unlock();
	StartCapture(strNameCapture);
	StartRender(strNameRender);
}

void CMainFrame::StopFromJs()
{
	OutputDebugStringA("CMainFrame StopFromJs");
	StopCapture();
	StopRender();
	OutputDebugStringA("\n");
	OutputDebugStringA("client m_strCapturePath");
	OutputDebugStringA(m_strCapturePath.c_str());
	OutputDebugStringA("\n");
	OutputDebugStringA("client m_strRenderPath");
	OutputDebugStringA(m_strRenderPath.c_str());
	OutputDebugStringA("\n");
	OutputDebugStringA("client m_strToken");
	OutputDebugStringA(m_strToken.c_str());
	OutputDebugStringA("\n");
	OutputDebugStringA("client m_strObjectName");
	OutputDebugStringA(m_strObjectName.c_str());
	OutputDebugStringA("\n");
	if (!m_strCapturePath.empty() && !m_strRenderPath.empty() && !m_strToken.empty() && !m_strObjectName.empty())
	{
		OutputDebugStringA("client 启动上传线程");
		OutputDebugStringA("\n");
		std::thread th(&CMainFrame::UploadFileToServer, this, m_strCapturePath.c_str(), m_strRenderPath.c_str(), m_strToken.c_str(),m_strObjectName.c_str());
		th.detach();
	}
}

void CMainFrame::UploadFileToServerDone()
{
	if (!m_handler){
		return;
	}
	CefRefPtr<CefFrame> frame = m_handler->GetBrowser()->GetMainFrame();
	if (frame) {
		CefString strCode;
		USES_CONVERSION;
		CDuiString strData;
		strData.Format(_T("endUpload();"));
		strCode = strData.GetData();
		OutputDebugStringA("\n");
		OutputDebugStringA("client ExecuteJavaScript");
		OutputDebugStringA("\n");
		frame->ExecuteJavaScript(strCode, frame->GetURL(), 0);
	}
	m_mutex.lock();
	m_strCapturePath = "";
	m_strRenderPath = "";
	m_strToken = "";
	m_strObjectName = "";
	m_mutex.unlock();
	return;
}

void CMainFrame::UploadFileToServer(const string& sFileCapture, const string& sFileRender, const string& sToken, const string& sObjectName)
{
	OutputDebugStringA("\n");
	OutputDebugStringA("client 文件上传函数入口");
	OutputDebugStringA("\n");
	if (!m_handler) {
		return;
	}
	CefRefPtr<CefFrame> frame = m_handler->GetBrowser()->GetMainFrame();
	if (frame) {
		CefString strCode;
		USES_CONVERSION;
		CDuiString strData;
		INTERNET_PORT nPort = 8080;
	    //https://suzuki-dev-finance.haimaiche.net/room/getPCMFile
		string strFileCapture = sFileCapture;
		string strFileRender = sFileRender;
		string strServer = "suzuki-dev-finance.haimaiche.net";// "172.21.1.24";
		string strObject = sObjectName;// "/room/getPCMFile";//服务器文件对象
		string strToken = sToken;
		if (::PathFileExistsA(strFileCapture.c_str()) && ::PathFileExistsA(strFileRender.c_str()))
		{
			if (UploadLocalFile(strFileCapture, strServer, strObject, strToken, nPort) && UploadLocalFile(strFileRender, strServer, strObject, strToken, nPort))
			{
				OutputDebugStringA("\n");
				OutputDebugStringA("client 文件上传成功");
				OutputDebugStringA("\n");
				//
				UploadFileToServerDone();
				//通知浏览器
				if (::PathFileExistsA(strFileCapture.c_str())){
					DeleteFileA(strFileCapture.c_str());
					Sleep(100);
				}
				if (::PathFileExistsA(strFileRender.c_str())){
					DeleteFileA(strFileRender.c_str());
					Sleep(100);
				}
			}
			else
			{
				OutputDebugStringA("\n");
				OutputDebugStringA("client 文件上传失败，重试");
				OutputDebugStringA("\n");
				//重试逻辑
				int iTryTimes = 0;
				do 
				{
					Sleep(10 * 1000);//10s
					if (UploadLocalFile(strFileCapture, strServer, strObject, strToken, nPort) && UploadLocalFile(strFileRender, strServer, strObject, strToken, nPort))
					{
						//文件上传成功
						UploadFileToServerDone();
						//通知浏览器
						if (::PathFileExistsA(strFileCapture.c_str())) {
							DeleteFileA(strFileCapture.c_str());
							Sleep(100);
						}
						if (::PathFileExistsA(strFileRender.c_str())) {
							DeleteFileA(strFileRender.c_str());
							Sleep(100);
						}
						break;
					}
					iTryTimes++;

				} while (iTryTimes >= 3);
			}
		}
		else
		{
			OutputDebugStringA("文件不存在！");
		}
	}
	return;
}

void CMainFrame::OnWndSizeMax(bool bMax)
{
	//m_pBtnMax->SetVisible(!bMax);
	//m_pBtnRestore->SetVisible(bMax);
	static RECT rc;//记录下还原时的位置
	if (bMax)
	{
		GetWindowRect(GetHWND(), &rc);
		::SendMessageA(GetHWND(), WM_SYSCOMMAND, SC_MAXIMIZE, 0);//最大化
	}
	else
	{
		//ResizeClient(1024, 720);
		//SetWindowPos(GetHWND(), HWND_NOTOPMOST, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, SWP_SHOWWINDOW);
		//::SendMessageA(GetHWND(), WM_SYSCOMMAND, SC_RESTORE, 0);//还原
		//::PostMessage(GetHWND(), WM_MSG_REFRESH_SHADOW, 0, 0);
	}
}

void CMainFrame::EnterFullScreen(bool bFull)
{
	static LONG oldStyle;//记录下原窗口style
	static RECT rc;//记录下还原时的位置
	if (bFull)
	{
		//SendMessageA(GetHWND(), WM_SYSCOMMAND, SC_RESTORE, 0);
		GetWindowRect(GetHWND(), &rc);
		//oldStyle = SetWindowLong(GetHWND(), GWL_STYLE, WS_POPUP);
		int w = GetSystemMetrics(SM_CXSCREEN);
		int h = GetSystemMetrics(SM_CYSCREEN);
		SetWindowPos(GetHWND(), HWND_TOPMOST, 0, 0, w, h, SWP_SHOWWINDOW);
		::PostMessage(GetHWND(), WM_MSG_REFRESH_SHADOW, 0, 0);
	}
	else
	{
		//SetWindowLong(GetHWND(), GWL_STYLE, oldStyle);
		SetWindowPos(GetHWND(), HWND_NOTOPMOST, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, SWP_SHOWWINDOW);
		::PostMessage(GetHWND(), WM_MSG_REFRESH_SHADOW, 0, 0);
	}
}

BOOL CMainFrame::GetFileInfo(HANDLE &hFile,OUT FileInfo& binaryInfo, IN string FilePath)
{
	BOOL nRet = FALSE;
	DWORD nBytesRead;
	hFile = CreateFileA(FilePath.c_str(),
		GENERIC_READ,
		FILE_SHARE_READ,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	if (hFile == INVALID_HANDLE_VALUE){
		return nRet;
	}

	binaryInfo.nFileLen = GetFileSize(hFile, &binaryInfo.nFileHighLen);
	binaryInfo.FileBuf = new char[binaryInfo.nFileLen];
	if (!binaryInfo.FileBuf)
	{
		CloseHandle(hFile);
		return nRet;
	}
	ZeroMemory(binaryInfo.FileBuf, binaryInfo.nFileLen);
	if (!ReadFile(hFile,binaryInfo.FileBuf,binaryInfo.nFileLen,&nBytesRead,NULL))
	{
		CloseHandle(hFile);
		return nRet;
	}
	CloseHandle(hFile);
	nRet = TRUE;

	return nRet;
}

//本地路径 服务器地址 服务器路径 数据分割信息 端口
//通过以上传入信息 将二进制数据传入web服务器
BOOL CMainFrame::UploadLocalFile(const string& strFilePath, IN string& ServerName, IN string& ObjectName, const string& sToken, IN INTERNET_PORT &nPort)
{
	HINTERNET hSession = 0;
	HINTERNET hConnect = 0;
	HINTERNET hRequest = 0;
	DWORD dwNumberOfBytesWritten = 0;
	DWORD dwBytesSend = 0;

	INTERNET_BUFFERS BufferIn;
	DWORD dwFlag;
	LPCTSTR boundary = TEXT("--------67491722032265"); //随机字符串
	LPCSTR aboundary = "--------67491722032265"; //ansi
	USES_CONVERSION;
	string strFileName = strFilePath;
	int iSize = strFileName.length();
	int pos = strFileName.find_last_of("\\");
	if (pos != std::string::npos)
	{
		//找到了
		strFileName = strFileName.substr(pos+1, iSize - pos);
	}
	HANDLE hFile;
	hFile = CreateFileA(strFilePath.c_str(),GENERIC_READ,FILE_SHARE_READ | FILE_SHARE_WRITE,0,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,0);
	if (hFile == INVALID_HANDLE_VALUE){
		OutputDebugStringA("无效的文件句柄");
		return FALSE;
	}
	DWORD dwFileSize = GetFileSize(hFile, 0);
	TCHAR content_type[128];
	_stprintf_s(content_type, TEXT("Content-Type: multipart/form-data; boundary=%s"), boundary);
	//LPTSTR referer = TEXT("Referer: https://172.21.10.110:8999/room/getPCMFile");
	LPTSTR accept = TEXT("Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8");
	LPTSTR accept_lan = TEXT("Accept-Language: zh-cn,zh;q=0.8,en-us;q=0.5,en;q=0.3");
	LPTSTR accept_encoding = TEXT("Accept-Encoding: gzip, deflate");
	//LPTSTR accept_token;
	string sAccept_token = "x-auth-token:";
	sAccept_token += sToken;
	//swprintf_s(accept_token,"x-auth-token: %s", sToken.c_str());//682c6b7c-6cc8-4ff1-a78f-e4cf820c801c   9b287ec7-2e36-4900-a3ec-35e946fada4e
	LPTSTR user_agent = TEXT("User-Agent: Mozilla/5.0 (Windows NT 6.1; WOW64; rv:22.0) Gecko/20100101 Firefox/22.0");

	hSession = InternetOpen(_T("Mozilla/5.0 (Windows NT 6.1; WOW64; rv:22.0) Gecko/20100101 Firefox/22.0"),
		INTERNET_OPEN_TYPE_PRECONFIG,
		0,
		INTERNET_INVALID_PORT_NUMBER,
		0);
	if (0 == hSession) {
		return FALSE;
	}
	hConnect = InternetConnect(hSession,
		A2W(ServerName.c_str()),
		INTERNET_INVALID_PORT_NUMBER/*nPort*/,
		_T(""),
		_T(""),
		INTERNET_SERVICE_HTTP,
		0,
		0);
	if (0 == hConnect)
	{
		InternetCloseHandle(hSession);
		return FALSE;
	}
	dwFlag = INTERNET_FLAG_KEEP_CONNECTION;

	hRequest = HttpOpenRequest(hConnect,
		_T("POST"),
		A2W(ObjectName.c_str()),
		HTTP_VERSION,
		0,                //Referrer
		0,                //AcceptTypes 
		dwFlag,
		0);
	if (0 == hRequest) {
		InternetCloseHandle(hConnect);
		InternetCloseHandle(hSession);
		return FALSE;
	}
	
	if (!HttpAddRequestHeaders(hRequest, content_type, -1, HTTP_ADDREQ_FLAG_ADD | HTTP_ADDREQ_FLAG_REPLACE))
	{
		printf("Failed!%lu\n", GetLastError());
		InternetCloseHandle(hConnect);
		InternetCloseHandle(hSession);
		InternetCloseHandle(hRequest);
		return FALSE;
	}
	if (!HttpAddRequestHeaders(hRequest, accept, -1, HTTP_ADDREQ_FLAG_ADD | HTTP_ADDREQ_FLAG_REPLACE))
	{
		printf("Failed!%lu\n", GetLastError());
		InternetCloseHandle(hConnect);
		InternetCloseHandle(hSession);
		InternetCloseHandle(hRequest);
		return FALSE;
	}
	if (!HttpAddRequestHeaders(hRequest, accept_lan, -1, HTTP_ADDREQ_FLAG_ADD | HTTP_ADDREQ_FLAG_REPLACE))
	{
		printf("Failed!%lu\n", GetLastError());
		InternetCloseHandle(hConnect);
		InternetCloseHandle(hSession);
		InternetCloseHandle(hRequest);
		return FALSE;
	}
	if (!HttpAddRequestHeaders(hRequest, accept_encoding, -1, HTTP_ADDREQ_FLAG_ADD | HTTP_ADDREQ_FLAG_REPLACE))
	{
		printf("Failed!%lu\n", GetLastError());
		InternetCloseHandle(hConnect);
		InternetCloseHandle(hSession);
		InternetCloseHandle(hRequest);
		return FALSE;
	}
	if (!HttpAddRequestHeaders(hRequest, A2W(sAccept_token.c_str()), -1, HTTP_ADDREQ_FLAG_ADD | HTTP_ADDREQ_FLAG_REPLACE))
	{
		printf("Failed!%lu\n", GetLastError());
		InternetCloseHandle(hConnect);
		InternetCloseHandle(hSession);
		InternetCloseHandle(hRequest);
		return FALSE;
	}

	LPVOID lpBuffer = (LPVOID)VirtualAlloc(0, dwFileSize, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
	if (0 == lpBuffer)
	{
		int iRet = GetLastError();
		CDuiString strData;
		strData.Format(_T("VirtualAlloc ERROR：%d"), iRet);
		OutputDebugString(strData.GetData());
		InternetCloseHandle(hRequest);
		InternetCloseHandle(hConnect);
		InternetCloseHandle(hSession);
		return FALSE;
	}

	DWORD dwRead;
	ReadFile(hFile, lpBuffer, dwFileSize, &dwRead, 0);
	CloseHandle(hFile);
	char first_boundary[128];
	char delimiter[128];
	char end_boundary[128];
	sprintf_s(first_boundary, "--%s\r\n", aboundary);
	sprintf_s(delimiter, "\r\n--%s\r\n", aboundary);
	sprintf_s(end_boundary, "\r\n--%s--\r\n", aboundary);
	char content_dispos[128];
	sprintf_s(content_dispos, "Content-Disposition: form-data; name=\"file\"; filename=\"%s\"\r\n", strFileName.c_str());
	//LPSTR content_dispos = "Content-Disposition: form-data; name=\"file\"; filename=\"foobarcapture.pcm\"\r\n";
	LPSTR content_type2 = "Content-Type: application/octet-stream\r\n\r\n";
	LPSTR rn = "\r\n";
	BufferIn.dwStructSize = sizeof(INTERNET_BUFFERS);
	BufferIn.Next = NULL;
	BufferIn.lpcszHeader = NULL;
	BufferIn.dwHeadersLength = 0;
	BufferIn.dwHeadersTotal = 0;
	BufferIn.lpvBuffer = NULL;
	BufferIn.dwBufferLength = 0;
	BufferIn.dwBufferTotal = dwFileSize
		+ strlen(first_boundary)
		+ strlen(content_dispos)
		+ strlen(content_type2)
		+ strlen(end_boundary); //Content-Length:
	BufferIn.dwOffsetLow = 0;
	BufferIn.dwOffsetHigh = 0;

	if (!HttpSendRequestEx(hRequest, &BufferIn, 0, 0, 0))
	{
		InternetCloseHandle(hRequest);
		InternetCloseHandle(hConnect);
		InternetCloseHandle(hSession);
		VirtualFree(lpBuffer, 0, MEM_RELEASE/*MEM_DECOMMIT*/);
		return FALSE;
	}
	if (!InternetWriteFile(hRequest, (byte*)first_boundary, strlen(first_boundary), &dwNumberOfBytesWritten))
	{
		InternetCloseHandle(hRequest);
		InternetCloseHandle(hConnect);
		InternetCloseHandle(hSession);
		VirtualFree(lpBuffer, 0, MEM_RELEASE/*MEM_DECOMMIT*/);
		return FALSE;
	}
	 //first boundary
	if (!InternetWriteFile(hRequest, (byte*)content_dispos, strlen(content_dispos), &dwNumberOfBytesWritten))
	{
		InternetCloseHandle(hRequest);
		InternetCloseHandle(hConnect);
		InternetCloseHandle(hSession);
		VirtualFree(lpBuffer, 0, MEM_RELEASE/*MEM_DECOMMIT*/);
		return FALSE;
	}
	if (!InternetWriteFile(hRequest, (byte*)content_type2, strlen(content_type2), &dwNumberOfBytesWritten))
	{
		InternetCloseHandle(hRequest);
		InternetCloseHandle(hConnect);
		InternetCloseHandle(hSession);
		VirtualFree(lpBuffer, 0, MEM_RELEASE/*MEM_DECOMMIT*/);
		return FALSE;
	}
	if (!InternetWriteFile(hRequest, lpBuffer, dwFileSize, &dwNumberOfBytesWritten))
	{
		InternetCloseHandle(hRequest);
		InternetCloseHandle(hConnect);
		InternetCloseHandle(hSession);
		VirtualFree(lpBuffer, 0, MEM_RELEASE/*MEM_DECOMMIT*/);
		return FALSE;
	}
	//如果还有其他文件
	//InternetWriteFile(hRequest,(byte*)delimiter,strlen(delimiter),&dwNumberOfBytesWritten); //deimiter
	//InternetWriteFile(hRequest,(byte*)content_dispos,strlen(content_dispos),&dwNumberOfBytesWritten);
	//InternetWriteFile(hRequest,(byte*)content_type2,strlen(content_type2),&dwNumberOfBytesWritten);
	//...
	//last boundary
	if (!InternetWriteFile(hRequest, (byte*)end_boundary, strlen(end_boundary), &dwNumberOfBytesWritten))
	{
		InternetCloseHandle(hRequest);
		InternetCloseHandle(hConnect);
		InternetCloseHandle(hSession);
		VirtualFree(lpBuffer, 0, MEM_RELEASE/*MEM_DECOMMIT*/);
		return FALSE;
	}
	if (!HttpEndRequest(hRequest, 0, 0, 0))
	{
		InternetCloseHandle(hRequest);
		InternetCloseHandle(hConnect);
		InternetCloseHandle(hSession);
		VirtualFree(lpBuffer, 0, MEM_RELEASE/*MEM_DECOMMIT*/);
		return FALSE;
	}
	VirtualFree(lpBuffer, 0, MEM_RELEASE/*MEM_DECOMMIT*/);

	return TRUE;
}

bool CMainFrame::GetAllFileFromFolder(const std::string& sFolder, std::vector<FileInfoTime>& vecFilePath)
{
	if (!PathFileExistsA(sFolder.c_str())) {
		return false;
	}
	WIN32_FIND_DATAA find_data;
	char buf[MAX_PATH];
	ZeroMemory(buf, MAX_PATH);
	//char buftemp[MAX_PATH];
	strcpy_s(buf, _countof(buf), sFolder.c_str());
	PathAppendA(buf, "*.*");
	HANDLE hFileFind = ::FindFirstFileA(buf, &find_data);
	if (INVALID_HANDLE_VALUE == hFileFind)
		return false;
	do {
		if (FILE_ATTRIBUTE_DIRECTORY&find_data.dwFileAttributes)
		{
			if (strcmp(find_data.cFileName, ".") == 0 || strcmp(find_data.cFileName, "..") == 0)
			{
				continue;
			}
			string strBufTemp(sFolder);
			strBufTemp += "\\";
			strBufTemp += find_data.cFileName;
			GetAllFileFromFolder(strBufTemp, vecFilePath);
		}
		else
		{
			strcpy_s(buf, _countof(buf), sFolder.c_str());
			PathAppendA(buf, find_data.cFileName);
			FileInfoTime fileinfo;
			fileinfo.sFilePath = buf;
			
			HANDLE hFile;
			hFile = CreateFileA(buf, 0, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
			string strTime;
			GetLocalFileTime(hFile, strTime);
			fileinfo.sTime = strTime;
			vecFilePath.push_back(fileinfo);
		}
	} while (FindNextFileA(hFileFind, &find_data));

	FindClose(hFileFind);

	return true;
}

void CMainFrame::ThDeleteFileThread()
{
	string sFileFolder = GetRecorderFileFolder();
	if (!::PathFileExistsA(sFileFolder.c_str())) {
		::CreateDirectoryA(sFileFolder.c_str(), NULL);
	}
	//
	SYSTEMTIME st = { 0 };
	GetLocalTime(&st);
	TCHAR sTime[20] = { 0 };
	int isize = sizeof(sTime) / sizeof(TCHAR);
	swprintf_s(sTime, 20, _T("%4d-%02d-%02d"), st.wYear, st.wMonth, st.wDay);
	std::vector<std::string> vCurrentInfo;//当前时间信息
	USES_CONVERSION;
	string sCurrentTime = W2A(sTime);
	GetKeyPaths(sCurrentTime,"-", vCurrentInfo);
	std::vector<FileInfoTime> vecAllFile;
	GetAllFileFromFolder(sFileFolder, vecAllFile);
	for (size_t i = 0; i < vecAllFile.size(); i++)
	{
		FileInfoTime FileInfo = vecAllFile[i];
		if (::PathFileExistsA(FileInfo.sFilePath.c_str()))
		{
			std::vector<std::string> vFileInfo;//文件时间信息
			GetKeyPaths(FileInfo.sTime, "-", vFileInfo);
			if (vCurrentInfo.size() == vFileInfo.size())
			{
				if (vCurrentInfo[0] == vFileInfo[0])
				{
					int iCurrentDays = 0;
					iCurrentDays = atoi(vCurrentInfo[1].c_str()) * 30 + atoi(vCurrentInfo[2].c_str());
					int iFileDays = 0;
					iFileDays = atoi(vFileInfo[1].c_str()) * 30 + atoi(vFileInfo[2].c_str());
					if (iCurrentDays - iFileDays >= 30)//超过30天
					{
						DeleteFileA(FileInfo.sFilePath.c_str());
						Sleep(100);
					}
				}
				else
				{
					DeleteFileA(FileInfo.sFilePath.c_str());
					Sleep(100);
				}
			}
		}
	}
}

BOOL CMainFrame::GetLocalFileTime(HANDLE hFile, string& lpszCreationTime)
{
	FILETIME ftCreate, ftAccess, ftWrite;
	SYSTEMTIME stUTC1, stLocal1;

	// -------->获取 FileTime
	if (!GetFileTime(hFile, &ftCreate, &ftAccess, &ftWrite)) {
		return FALSE;
	}
	//---------> 转换: FileTime --> LocalTime
	FileTimeToSystemTime(&ftCreate, &stUTC1);
	SystemTimeToTzSpecificLocalTime(NULL, &stUTC1, &stLocal1);
	// ---------> Show the  date and time.
	TCHAR sTime[20] = { 0 };
	USES_CONVERSION;
	swprintf_s(sTime, 20, _T("%4d-%02d-%02d"), stLocal1.wYear, stLocal1.wMonth, stLocal1.wDay);
	lpszCreationTime = W2A(sTime);

	return TRUE;
}