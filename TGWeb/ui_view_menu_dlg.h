#ifndef _UI_VIEW_MENU_H_
#define _UI_VIEW_MENU_H_

class CTrayListElementUI;
class CMainFrame;
class CViewMenuDlg : public WindowImplBase {
public:
	CViewMenuDlg(CMainFrame* pMain);
	~CViewMenuDlg();
	void InitWindow();
	HWND Create(HWND hwnd);
	virtual LPCTSTR GetWindowClassName() const { return _T("UI_VIEW_MENU_DLG"); }
	void Notify(TNotifyUI& msg);
	CDuiString GetSkinFile() { return _T("menu_view.xml"); }
	CDuiString GetSkinFolder() { return _T("skin"); }
	UILIB_RESOURCETYPE GetResourceType() const {
		return UILIB_FILE;
	}
	virtual void OnFinalMessage(HWND hWnd){
		WindowImplBase::OnFinalMessage(hWnd);
		delete this;
	}
	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual LRESULT OnKillFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnKeyDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual CControlUI* CreateControl(LPCTSTR pstrClass);
	void AdjustPos(RECT& rc, POINT pt);
	void ShowWnd();
private:
	POINT m_ptPos;
	ShadowHelp m_shadowHelp;
	CTrayListElementUI* m_pBtnBack;
	CTrayListElementUI* m_pBtnNext;
	CTrayListElementUI*	m_pBtnPercent;
	CTrayListElementUI* m_pBtnCallJS;
	CMainFrame* m_pMainFrame;
};

#endif // !_UI_VIEW_MENU_H_
