#pragma once
#include "SimpleCefHandler.h"

class CSimpleCefApp;
class CCookieVisitor;
class CFileMenuDlg;
class CMainFrame :public DuiLib::WindowImplBase
{
public:
	CMainFrame(const string& sInitUrl, const string& sInitParam);
	~CMainFrame();
	static LPCTSTR GetCaption(){ return _T("tgweb"); }
	HWND Create(HWND hwndParent, LPCTSTR pstrName);
	static HWND g_HWnd;
	void SetCaption(wstring sCaption);
	void SetCurrentUrl(wstring sCaption);
	void SetWebCookies(std::wstring domain, std::wstring key, std::wstring sValue);
	void OpenLocalPdfFile();
	void CallJsFucntion();
private:
	DuiLib::CDuiString GetSkinFile();
    void InitWindow();
	UILIB_RESOURCETYPE GetResourceType() const;
	CDuiString GetSkinFolder();
	UINT GetClassStyle() const;
	virtual LPCTSTR GetWindowClassName(void)const{return _T("tgweb");};
	void OnFinalMessage( HWND hWnd );
protected:
	LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM /*lParam*/, bool& /*bHandled*/);
	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	//LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	void Notify(TNotifyUI& msg);
private:
	ShadowHelp m_ShaowHelp;
	CButtonUI *m_pBtnMin;
	CButtonUI* m_pBtnClose;
	CButtonUI*  m_pBtnBackUp;
	CButtonUI*	m_pBtnForward;
	CButtonUI*	m_pBtnGo;
	CButtonUI*  m_pBtnTitleFile;

	CButtonUI*  m_pBtnHost;
	CButtonUI*	m_pBtnHelp;
	CButtonUI*	m_pBtnMenuFile;
	CButtonUI*	m_pBtnMenuView;
	CButtonUI*	m_pBtnMenuSign;
	CButtonUI*	m_pBtnMenuCheck;

	CEditUI*    m_edtUrl;
	CLabelUI*  m_pLblTitle;
	std::string m_pInitUrl;//初始化url地址
	std::string m_pInitParam;//初始化参数
	CContainerUI* m_pContainer;
	CActiveXUI* m_pActiveUI;
	CWebBrowserUI* m_pBrowserUI;
	CSimpleCefHandler* m_handler;
	std::mutex m_mutex;
	CefRefPtr<CCookieVisitor> m_CookieVisitor;
	CefRefPtr<CefRequestContextHandler> handlerCookies;
private:
	std::string m_sInitUrl;
	int	m_width;
	int	m_height;
	bool	m_bShowMin;
	CFileMenuDlg* m_pFileMenuDlg;
};

