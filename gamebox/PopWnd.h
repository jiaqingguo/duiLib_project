#pragma once

#include "UtilTool.h"
//////////////////////////////////////////////////////////////////////////
///

class CPopWnd : public WindowImplBase
{
public:
	CPopWnd(void);
	~CPopWnd(void);

	static int MessageBox(HWND hParent)
	{
		
		CPopWnd* pWnd = new CPopWnd();

		pWnd->Create(hParent, _T("msgwnd"), WS_POPUP | WS_CLIPCHILDREN, WS_EX_TOOLWINDOW);
		//pWnd->Create(hParent, _T("msgwnd"), UI_WNDSTYLE_FRAME, 0L, 0, 0, 800, 572);
		pWnd->CenterWindow();
		pWnd->ShowModal();

		return 0;
	}
public:
	virtual void OnFinalMessage( HWND );
	virtual CDuiString GetSkinFile();
	virtual LPCTSTR GetWindowClassName( void ) const;
	virtual void Notify( TNotifyUI &msg );
	virtual void InitWindow();
	virtual string get_cur_timelogdata();
	
	DUI_DECLARE_MESSAGE_MAP()
		virtual void OnClick(TNotifyUI& msg);
	virtual void OnSelectChanged( TNotifyUI &msg );
	virtual void OnItemSelect( TNotifyUI &msg );

	virtual LRESULT OnSysCommand( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled );
	LRESULT HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM /*lParam*/, bool& /*bHandled*/)
	{
		if (uMsg == WM_KEYDOWN)
		{
			switch (wParam)
			{
			case VK_RETURN:
			case VK_ESCAPE:
				return ResponseDefaultKeyEvent(wParam);
			default:
				break;
			}
		}
		return FALSE;
	}
private:
	CButtonUI* m_pCloseBtn;
	CButtonUI* m_pMaxBtn;
	CButtonUI* m_pRestoreBtn;
	CButtonUI* m_pMinBtn;
	CButtonUI* m_pMenuBtn;
public:
	
	vector<string> Ptempdata;
	int PSatelliteNum;
	static unsigned __stdcall Busys(void* arglist);
	int DataDiffer;//需要补多少字符--全局
	int IdDiffer;//需要补多少包识别符字符--全局
	bool Complement;//是否需要补码--全局true
	bool Id_Complement;//每一帧的包识别符是否需要补码--全局，true
	string strtempXX;//全局
};
