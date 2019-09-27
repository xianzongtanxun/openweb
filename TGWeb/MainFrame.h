#pragma once
#include "SimpleCefHandler.h"

class CSimpleCefApp;
class CCookieVisitor;
class CMainFrame :public DuiLib::WindowImplBase
{
public:
	CMainFrame(const string& sInitUrl, const string& sInitParam);
	~CMainFrame();
	static LPCTSTR GetCaption(){ return _T("tgweb"); }
	HWND Create(HWND hwndParent, LPCTSTR pstrName);
	static HWND g_HWnd;
	void SetCaption(wstring sCaption);
	void SetWebCookies(std::wstring domain, std::wstring key, std::wstring sValue);
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
	CLabelUI*  m_pLblTitle;
	std::string m_pInitUrl;//初始化url地址
	std::string m_pInitParam;//初始化参数
	CContainerUI* m_pContainer;
	CSimpleCefHandler* m_handler;
	std::mutex m_mutex;
	CefRefPtr<CCookieVisitor> m_CookieVisitor;
	CefRefPtr<CefRequestContextHandler> handlerCookies;
private:
	std::string m_sInitUrl;
	int	m_width;
	int	m_height;
	bool	m_bShowMin;
};

