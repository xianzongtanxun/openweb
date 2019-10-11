#ifndef _UI_FILE_MENU_H_
#define _UI_FILE_MENU_H_

class CTrayListElementUI;
class CMainFrame;
class CFileMenuDlg : public WindowImplBase {
public:
	CFileMenuDlg(CMainFrame* pMain);
	~CFileMenuDlg();
	void InitWindow();
	HWND Create(HWND hwnd);
	virtual LPCTSTR GetWindowClassName() const { return _T("UIFILEMANUDLG"); }
	void Notify(TNotifyUI& msg);
	CDuiString GetSkinFile() { return _T("file_menu.xml"); }
	CDuiString GetSkinFolder() { return _T("skin"); }
	UILIB_RESOURCETYPE GetResourceType() const {
		return UILIB_FILE;
	}
	virtual void OnFinalMessage(HWND hWnd){
		WindowImplBase::OnFinalMessage(hWnd);
		delete this;
	}
	//virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual LRESULT OnKillFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnKeyDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual CControlUI* CreateControl(LPCTSTR pstrClass);
	void AdjustPos(RECT& rc, POINT pt);
	void ShowWnd();
private:
	CButtonUI*	m_pBtnOk;
	CButtonUI*	m_pBtnCancel;
	CEditUI*    m_pEdit;
	POINT m_ptPos;
	std::vector<std::string> m_tagList;
	CVerticalLayoutUI*       m_pVlyDefault;
	ShadowHelp m_shadowHelp;
	CTrayListElementUI* m_pBtnOpen;
	CTrayListElementUI*	m_pBtnSave;
	CTrayListElementUI*	m_pBtnSaveAs;
	CTrayListElementUI*	m_pBtnPrint;
	CMainFrame* m_pMainFrame;
};

#endif // !_UI_FILE_MENU_H_
