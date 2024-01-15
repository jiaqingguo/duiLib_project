#pragma once

#include <Windows.h>
#include <iostream>
#include "Util/UtilTool.h"
//#include "UtilTool.h"


class CpopSelectEquipment : public WindowImplBase
{
public:
	static int MessageBox(HWND hParent, vector<vector<string>> vec_vecName)
	{
		CpopSelectEquipment* pWnd = new CpopSelectEquipment();
		pWnd->Create(hParent, _T("msgwnd"), WS_POPUP | WS_CLIPCHILDREN, WS_EX_TOOLWINDOW, 0, 0, 400, 250);
		pWnd->CenterWindow();
		SetWindowPos(pWnd->GetHWND(), HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);//将界面这个界面显示在最上层

		pWnd->SetOptical_fiber(vec_vecName);
		//pWnd->SettingCues(vec_Type);
		//pWnd->SetTestName(Test_Name);
		return pWnd->ShowModal();
	}

	CpopSelectEquipment();
	~CpopSelectEquipment(void);

	vector<string> GetNodeName();//获取要移动到的节点名称

public:
	virtual void OnFinalMessage(HWND);
	virtual CDuiString GetSkinFile();
	virtual LPCTSTR GetWindowClassName(void) const;
	virtual void Notify(TNotifyUI &msg);
	void OnLClick(CControlUI *pControl);
	virtual void InitWindow();

	wstring stringToWstring(string str);
	string wstringToString(wstring wstr);

	string UTF8_To_string(const std::string & str);
	void SetOptical_fiber(vector<vector<string>> vec_vecName);//初始化给节点赋值
	void SettingCues(vector<string> vec_type);
	void SettingCues();
	virtual string get_cur_timelogdata();

	DUI_DECLARE_MESSAGE_MAP()
		virtual void OnClick(TNotifyUI& msg);
	virtual void OnSelectChanged(TNotifyUI &msg);
	virtual void OnItemSelect(TNotifyUI &msg);

	static vector<string> m_selectName;//选择的节点名称


	virtual LRESULT OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
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
	CTabLayoutUI* pTabDatabase;
	CLabelUI* m_CLabelUI;
	CEditUI * m_CEditUI_1;
	CEditUI * m_CEditUI_2;
	CEditUI * m_CEditUI_3;
	CEditUI * m_CEditUI_4;
	CEditUI * m_CEditUI_5;
	string StoreOptions[300];//存放option控件的数组
	vector<vector<string> > vec_opt;//存放已经用的控件
	vector<string> m_option;//显示出来的option
	vector<string> m_optionValue;//显示出来的所有option的内容

	//COptionUI * option1[1024];
	//COptionUI * option2[1024];
	//COptionUI * option3[1024];
	//COptionUI * option4[1024];
	//COptionUI * option5[1024];
	//COptionUI * option6[1024];
	//COptionUI * option7[1024];
	//COptionUI * option8[1024];
	//COptionUI * option9[1024];
	//COptionUI * option10[1024];
	//COptionUI * option11[1024];
	//COptionUI * option12[1024];
	//COptionUI * option13[1024];
	//COptionUI * option14[1024];
	//COptionUI * option15[1024];

	vector<vector<COptionUI * > > m_optionUI;
public:
	vector<string> Ptempdata;
	int PSatelliteNum;
	static unsigned __stdcall Busys(void* arglist);

};



