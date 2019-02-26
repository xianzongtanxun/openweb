#pragma once
//class CDuiMsgHandler;
#include "SimpleCefHandler.h"
typedef struct _FileInfo
{
	DWORD nFileLen;
	DWORD nFileHighLen;
	LPSTR FileBuf;
}FileInfo,*pFileInfo;

typedef struct _FileInfoTime
{
	string sTime;
	string sFilePath;
}FileInfoTime, *pFileInfoTime;

class MyAudioSink;
class CSimpleCefApp;
class CSoundCaptureHandler;
class CSoundRenderHandler;
class CMainFrame :public WindowImplBase
{
public:
	CMainFrame();
	~CMainFrame();
	static LPCTSTR GetCaption(){ return _T("MainSuzuki"); }
	HWND Create(HWND hwndParent, LPCTSTR pstrName);
	static HWND g_HWnd;
	void SetCaption(wstring sCaption);
	string GetRecorderFileFolder();
	void StartCapture(const string& sName);
	void StopCapture();
	void StartRender(const string& sName);
	void StopRender();
public:
	void StartFromJsWithFileName(const string& sFileCapture, const string& sFileRender, const string& sToken, const string& sObjectName);
	void StopFromJs();
	void UploadFileToServer(const string& sFileCapture, const string& sFileRender, const string& sToken, const string& sObjectName);
	void UploadFileToServerDone();
	void GetKeyPaths(string strPath, string delimiter, std::vector<string>& parts);
private:
	DuiLib::CDuiString GetSkinFile();
    void InitWindow();
	UILIB_RESOURCETYPE GetResourceType() const;
	CDuiString GetSkinFolder();
	UINT GetClassStyle() const;
	virtual LPCTSTR GetWindowClassName(void)const{return _T("MainSuzuki");};
	void OnFinalMessage( HWND hWnd );
protected:
	LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM /*lParam*/, bool& /*bHandled*/);
	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	void Notify(TNotifyUI& msg);
	void OnWndSizeMax(bool bMax);
	void EnterFullScreen(bool bFull);
	void ThDeleteFileThread();
	BOOL GetLocalFileTime(HANDLE hFile, string& lpszCreationTime);
	//文件上传相关函数
	BOOL GetFileInfo(HANDLE &hFile,OUT FileInfo& binaryInfo, IN string FilePath);
	BOOL UploadLocalFile(const string& strFilePath, IN string& ServerName, IN string& ObjectName, const string& sToken, IN INTERNET_PORT &nPort);
	bool GetAllFileFromFolder(const std::string& sFolder, std::vector<FileInfoTime>& vecFilePath);
private:
	ShadowHelp m_ShaowHelp;
	CEditUI* m_edtUrl;
	//CButtonUI *m_pBtnBackup;
	//CButtonUI *m_pBtnForword;
	CButtonUI *m_pBtnGo;
	MyAudioSink*   m_myAudioSink;
	MyAudioSink*   m_myAudioSinkRender;
	FILE *m_fpCapture;
	FILE *m_fpRender;
	CSoundCaptureHandler* m_pSoundCaptureHandler;
	CSoundRenderHandler* m_pSoundRenderHandler;
	HANDLE         m_record_handler;
	//CButtonUI* m_pBtnRestore;
	//CButtonUI* m_pBtnMax;
	CButtonUI* m_pBtnClose;
	CLabelUI*  m_pLblTitle;
	CContainerUI* m_pContainer;
	CSimpleCefHandler* m_handler;
	CSimpleCefApp*     m_cefApp;
	string m_strCapturePath;
	string m_strRenderPath;
	string m_strToken;
	string m_strObjectName;
	std::mutex m_mutex;
};

