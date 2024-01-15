#pragma once

#include <Windows.h>
#include <iostream>
#include "Structural.h"
//#include "UDP.h"
#include "UdpRecvClass.h"
#include "UtilTool.h"
#include "CPopNewConstellation.h"
//#include "DecodeDll.h"

//链路管理界面类声明
class CPopLinkManage : public WindowImplBase
{
public:
	CPopLinkManage(void);
	~CPopLinkManage(void);

	wstring stringToWstring(string str);
		//wstring转string
	string wstringToString(wstring wstr);
	string UTF8_To_string(const std::string & str);
	string string_To_UTF8(const std::string & str);

	static int MessageBox(HWND hParent)
	{

		CPopLinkManage* pWnd = new CPopLinkManage();

		pWnd->Create(hParent, _T("msgwnd"), WS_POPUP | WS_CLIPCHILDREN, WS_EX_TOOLWINDOW);
		//pWnd->Create(hParent, _T("msgwnd"), UI_WNDSTYLE_FRAME, 0L, 0, 0, 800, 572);
		pWnd->CenterWindow();
		pWnd->ShowModal();

		return 0;
	}

public:

	CComboBoxUI  * select_combox;


	string parameters[100];
	virtual void OnFinalMessage(HWND);
	virtual CDuiString GetSkinFile();
	virtual LPCTSTR GetWindowClassName(void) const;
	virtual void Notify(TNotifyUI &msg);
	void OnLClick(CControlUI *pControl);
	virtual void InitWindow();


	vector<string> my_split(string str, string pattern);

	void InitializeList();//向List里面添加东西
	void InitializeList(vector<string> vec);//向List里面添加东西
	void UpdateList(vector<vector<string>> vec);//更新list中的数据
	void ReceiveData(vector<string> vec);
	void InitializeCombo_link();//向combox里面添加东西
	void InitializeCombo_User(vector<string> vec);//给用户的combo加东西
	void InitializeCombo_Satellite(vector<string> vec);//给用户的combo加东西

	void GenerateTheFile(string path, string filename, vector<string> m_vec);
	void ImportingLocalData();//导入本地数据

	void AddingDataToList(int num);
	void JudgmentDataShows();//判断数据是否显示
	void JudgmentDataShows_1();//判断数据是否显示
	void ReceiveDataLocally(EffectiveLinkTable table);//接收数据到本地
	void UpdatingTheInterface();//更新界面
	void ParsingDisplay();//解析显示
	void ParsingDisplay(EffectiveLinkTable table);//解析并显示数据

	static UINT WINAPI UDPSendSerialPortThread(void* pParam);//将数据使用UDP发送
	static UINT WINAPI UDPRecvZKThread(void* pParam);//接收使用UDP发送来的数据


	virtual string get_cur_timelogdata();

	DUI_DECLARE_MESSAGE_MAP()
		virtual void OnClick(TNotifyUI& msg);
	virtual void OnSelectChanged(TNotifyUI &msg);
	virtual void OnItemSelect(TNotifyUI &msg);

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
public:
	vector<string> Ptempdata;
	int PSatelliteNum;
	static unsigned __stdcall Busys(void* arglist);

	UDPAdmin1 * m_Udpadmin1;//将串口收到的数据发送给总控

	string m_selectConditions;//已将选择的筛选条件

	EffectiveLinkTable m_EffectiveLinkTable;//有效链路列表结构体
	string m_ChooseIt;//用户输入的需要选择的名称

	UdpRecvClass* m_UdpRecvClass;
	int SelectListLine;

	vector<string> vec_localUser;//存放用户名称
	vector<string> vec_localSatellite;//存放卫星名称

	CPopNewConstellation * m_CPopNewConstellation;//新建星座或者数据
	vector<vector<string> > vec_LinkData;//将所有链路数据存放在这个变量中

	vector<EffectiveLinkTable> m_vecEffectLink;//将接收到的数据存储在本地
	CListUI* m_dataBSList;
};

