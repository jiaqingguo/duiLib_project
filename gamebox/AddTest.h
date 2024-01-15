#pragma once

//////////////////////////////////////////////////////////////////////////
///
#include "Util/UtilTool.h"
#define MSGID_OK		1
#define MSGID_CANCEL	0
#include <string>
#include <map>

using namespace  std;
class CAddTest : public WindowImplBase
{
public:
	static int MessageBox(HWND hParent, LPCTSTR lpstrTitle, LPCTSTR lpstrMsg,CDuiString Test_Name, CDuiString DLX_Name)
	{
		CAddTest* pWnd = new CAddTest();
		pWnd->Create(hParent, _T("msgwnd"), WS_POPUP | WS_CLIPCHILDREN, WS_EX_TOOLWINDOW);
		pWnd->CenterWindow();
		pWnd->SetTitle(lpstrTitle);
		pWnd->SetMsg(lpstrMsg);
		pWnd->SetTestName(Test_Name,DLX_Name);
		return pWnd->ShowModal();
	}


	//static int  ShowMessageBox(HWND hParent, LPCTSTR lpstrTitle, LPCTSTR lpstrMsg,std::vector<PKPARAMINFO>* paramvec, std::vector<PkJudgeFilter> * pCmdResponses)
	//{
	//	CAddTest* pWnd = new CAddTest();
	//	pWnd->Create(hParent, _T("msgwnd"), UI_WNDSTYLE_FRAME, 0);
	//	pWnd->CenterWindow();
	//	pWnd->SetTitle(lpstrTitle);
	//	pWnd->SetMsg(lpstrMsg);
	//	pWnd->SetMsgInfo(paramvec);
	//	return	pWnd->ShowModal();
	//}

public:
	CAddTest(void);
	~CAddTest(void);

	void SetMsg(LPCTSTR lpstrMsg);
	void SetTitle(LPCTSTR lpstrTitle);
	void SetTestName(CDuiString pTest_Name,CDuiString pDLX_Name)
	{
		//this->Test_Name=pTest_Name;
		//m_pm.FindControl(L"id")->SetText(pTest_Name);
		//this->DLX_Name = pDLX_Name;
		//m_pm.FindControl(L"id1")->SetText(pDLX_Name);
	}

public:
	virtual void OnFinalMessage( HWND );
	virtual CDuiString GetSkinFile();
	virtual LPCTSTR GetWindowClassName( void ) const;
	virtual void Notify( TNotifyUI &msg );
	virtual void InitWindow();

	DUI_DECLARE_MESSAGE_MAP()
		virtual void OnClick(TNotifyUI& msg);

	void SetTextString(string text,CControlUI * pControl){
		wstring str;
		UtilTool::setWstring(str,text.c_str());
		pControl->SetText(str.c_str());
	};


	string GetTextString(CControlUI * pControl){
		string str;
		UtilTool::setString(str,pControl->GetText().GetData());
		return str;
	};

	virtual LRESULT OnSysCommand( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled );
	LRESULT HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	map<string,map<string,string>> * pCmdMaps;

private:
	CButtonUI* m_pCloseBtn;
	CButtonUI* m_pMaxBtn;
	CButtonUI* m_pRestoreBtn;
	CButtonUI* m_pMinBtn;
	CButtonUI* m_pMenuBtn;

	CLabelUI * pLabel_name;
	CLabelUI * pLabel_ID;
	CLabelUI * pLabel_ID1;
	CLabelUI * pLabel_Source;
	CLabelUI * pLabel_BusID;
	CLabelUI * pLabel_RTID;
	CDuiString Test_Name;
	CDuiString DLX_Name;

	bool m_ZhTFlag;//转台复选框选择与否
	bool m_DLXFlag;//动力学复选框选择与否

	map<string,string> * msginfo;
};
