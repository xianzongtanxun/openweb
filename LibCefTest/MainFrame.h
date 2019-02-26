#pragma once
//class CDuiMsgHandler;
#include "DuiMsgHandler.h"
class CMainFrame :public WindowImplBase
{
public:
	CMainFrame();
	~CMainFrame();
	static LPCTSTR GetCaption(){ return _T("mainframecef"); }
	HWND Create(HWND hwndParent, LPCTSTR pstrName);
	static HWND g_HWnd;
	void SetCaption(wstring sCaption);
private:
	DuiLib::CDuiString GetSkinFile();
    void InitWindow();
	UILIB_RESOURCETYPE GetResourceType() const;
	CDuiString GetSkinFolder();
	UINT GetClassStyle() const;
	virtual LPCTSTR GetWindowClassName(void)const{return _T("mainframecef");};
	void OnFinalMessage( HWND hWnd );
protected:
	LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM /*lParam*/, bool& /*bHandled*/);
	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	void Notify(TNotifyUI& msg);
	void OnWndSizeMax(bool bMax);
	void EnterFullScreen(bool bFull);
private:
	ShadowHelp m_ShaowHelp;
	CEditUI* m_edtUrl;
	CButtonUI *m_pBtnBackup;
	CButtonUI *m_pBtnForword;
	CButtonUI *m_pBtnGo;

	CButtonUI* m_pBtnRestore;
	CButtonUI* m_pBtnMax;
	CButtonUI* m_pBtnClose;
	CLabelUI*  m_pLblTitle;
	CContainerUI* m_pContainer;
	CDuiMsgHandler* m_handler;
};

