#pragma once

#include <iostream>
#include "ConnectMysql.h"
//数据管理界面
class CPopDataManage : public WindowImplBase
{
public:
	CPopDataManage(void);
	~CPopDataManage(void);

	static int MessageBox(HWND hParent)
	{

		CPopDataManage* pWnd = new CPopDataManage();

		pWnd->Create(hParent, _T("msgwnd"), WS_POPUP | WS_CLIPCHILDREN, WS_EX_TOOLWINDOW);
		//pWnd->Create(hParent, _T("msgwnd"), UI_WNDSTYLE_FRAME, 0L, 0, 0, 800, 572);
		pWnd->CenterWindow();
		pWnd->ShowModal();

		return 0;
	}

public:
	virtual void OnFinalMessage(HWND);
	virtual CDuiString GetSkinFile();
	virtual LPCTSTR GetWindowClassName(void) const;
	virtual void Notify(TNotifyUI &msg);
	void OnLClick(CControlUI *pControl);
	virtual void InitWindow();

	void GettingDatabase();
	void InitializeMap();//初始化map

	void InitializeCombo();//向Combo里面添加东西
	void Filter_XZSJB();//星座数据表
	void Filter_DXSJB();//单星数据表筛选
	void Filter_WXZHSJB();//卫星载荷数据表筛选
	void Filter_WXTX();//卫星天线筛选
	void Filter_DXGSJB();//电信港数据表筛选
	void Filter_DQZSJB();//地球站数据表筛选
	void Filter_DQZKYSJB();//地球站溃源数据表筛选

	void Filter_DQZBXSBSJB();//地球站波形设备数据表筛选
	void Filter_DQZTXSJB();//地球站天线筛选
	void Filter_DMZDSJB();//地面终端数据表筛选
	void Filter_ZDZHSJBSJB();//终端载荷数据表筛选
	void Filter_BXSBSJBSJB();//波形设备数据表筛选
	void Filter_ZDTXSJB();//终端天线筛选


	//void Filter_DXSJB();//单星数据表筛选
	//void Filter_DXSJB();//单星数据表筛选
	virtual string get_cur_timelogdata();
	wstring stringToWstring(string str);
	string wstringToString(wstring wstr);

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
	CListUI* m_dataList_XZSJB;//星座数据表的List
	CListUI* m_dataList_DXSJB;//单星数据表的List
	CListUI* m_dataList_WXZHSJB;//卫星载荷数据表的List
	CListUI* m_dataList_WXTX;//卫星天线的List
	CListUI* m_dataList_DXGSJB;//电信港数据表的List
	CListUI* m_dataList_DQZSJB;//地球站数据表的List
	CListUI* m_dataList_DQZKYSJB;//地球站溃源数据表的List

	CListUI* m_dataList_DQZBXSB;//地球站波形设备数据表的List
	CListUI* m_dataList_DQZTX;//地球站天线的List
	CListUI* m_dataList_DMZDSJB;//地面终端数据表的List
	CListUI* m_dataList_ZDZHSJB;//终端载荷数据表的List
	CListUI* m_dataList_BXSBSJB;//波形设备数据表的List
	CListUI* m_dataList_ZDTX;//终端天线的List

private:
	vector<vector<string>> vec_vecData_XZSJB;//获取的数据库中---星座数据表
	vector<vector<string>> vec_vecData_DXSJB;//获取的数据库中---单星数据表
	vector<vector<string>> vec_vecData_WXZHSJB;//获取的数据库中---卫星载荷数据表
	vector<vector<string>> vec_vecData_WXTX;//获取的数据库中---卫星天线
	vector<vector<string>> vec_vecData_DXGSJB;//获取的数据库中---电信港数据表
	vector<vector<string>> vec_vecData_DQZSJB;//获取的数据库中---地球站数据表
	vector<vector<string>> vec_vecData_DQZKYSJB;//获取的数据库中---地球站溃源数据表

	vector<vector<string>> vec_vecData_DQZBXSB;//获取的数据库中---地球站波形设备数据表
	vector<vector<string>> vec_vecData_DQZTX;//获取的数据库中---地球站天线
	vector<vector<string>> vec_vecData_DMZDSJB;//获取的数据库中---地面终端数据表
	vector<vector<string>> vec_vecData_ZDZHSJB;//获取的数据库中---终端载荷数据表
	vector<vector<string>> vec_vecData_BXSBSJB;//获取的数据库中---波形设备数据表
	vector<vector<string>> vec_vecData_ZDTX;//获取的数据库中---终端天线

	CButtonUI* m_pCloseBtn;
	CButtonUI* m_pMaxBtn;
	CButtonUI* m_pRestoreBtn;
	CButtonUI* m_pMinBtn;
	CButtonUI* m_pMenuBtn;
	CTabLayoutUI* pTabDatabase_0;//page1和page2的tab
	CTabLayoutUI* pTabDatabase_1;//page1那7个界面的tab
	CTabLayoutUI* pTabDatabase_2;//page2那六张界面的tab

	CComboBoxUI  *  select_combox;
	ConnectMysql myMysql;

	string FilterArray_1[5];//星座数据表筛选列名
	string FilterArray_2[7];//单星数据表筛选列名
	string FilterArray_3[4];//卫星载荷数据表筛选列名
	string FilterArray_4[4];//卫星天线数据表筛选列名
	string FilterArray_5[5];//电信港数据表筛选列名
	string FilterArray_6[5];//地球站数据表筛选列名
	string FilterArray_7[5];//地球站溃源数据表筛选列名
	string FilterArray_8[4];//地球站波形设备数据表筛选列名
	string FilterArray_9[4];//地球站天线数据表筛选列名
	string FilterArray_10[3];//地面终端数据表筛选列名
	string FilterArray_11[5];//终端载荷数据表筛选列名
	string FilterArray_12[3];//终端波形设备数据表筛选列名
	string FilterArray_13[4];//终端天线数据表筛选列名

	map<string, string>  FilterArray_map1;//存放要筛选的星座数据表的map
	map<string, string>  FilterArray_map2;//存放单星数据表的map
	map<string, string>  FilterArray_map3;//存放卫星载荷数据表数据表的map
	map<string, string>  FilterArray_map4;//存放卫星天线数据表的map
	map<string, string>  FilterArray_map5;//存放电信港数据表的map
	map<string, string>  FilterArray_map6;//存放地球站数据表的map
	map<string, string>  FilterArray_map7;//存放地球站溃源数据表的map

	map<string, string>  FilterArray_map8;//存放地球站波形设备数据表的map
	map<string, string>  FilterArray_map9;//存放地球站天线的map
	map<string, string>  FilterArray_map10;//存放地面终端数据表的map
	map<string, string>  FilterArray_map11;//存放终端载荷数据表的map
	map<string, string>  FilterArray_map12;//存放终端波形设备数据表的map
	map<string, string>  FilterArray_map13;//存放终端天线的map

	string m_selectConditionsXZSJB;//选择的星座数据表列名
	string m_selectConditions_DXSJB;//选择的单星数据表列名
	string m_selectConditions_WXZHSJB;//选择的卫星载荷数据表列名
	string m_selectConditions_WXTX;//选择的卫星天线列名
	string m_selectConditions_DXGSJB;//选择的电信港数据表列名

	string m_selectConditions_DQZSJB;//选择的地球站数据表列名
	string m_selectConditions_DQZKYSJB;//选择的地球站馈源数据表列名
	string m_selectConditions_DQZBXSJB;//选择的地球站波形数据表列名
	string m_selectConditions_DQZTXSJB;//选择的地球站天线列名
	string m_selectConditions_DMZDSJB;//选择的地面终端数据表列名

	string m_selectConditions_DQZDZHSJB;//选择的终端载荷数据表列名
	string m_selectConditions_BXSBSJB;//选择的终端波形设备数据表列名
	string m_selectConditions_ZDTXSJB;//选择的终端天线列名

	CListUI* m_dataBSList_1;//星座数据表的LIst

	string m_ChooseIt_XZSJB;//用户选择筛选的列名称 ---星座数据表
	string m_ChooseIt_DXSJB;//用户选择筛选的列名称 ---单星数据表
	string m_ChooseIt_WXZHSJB;//用户选择筛选的列名称 ---卫星载荷数据表
	string m_ChooseIt_WXTX;//用户选择筛选的列名称 ---卫星天线
	string m_ChooseIt_DXGSJB;//用户选择筛选的列名称 ---电信港数据表
	string m_ChooseIt_DQZSJB;//用户选择筛选的列名称 ---地球站数据表
	string m_ChooseIt_DQZKYSJB;//用户选择筛选的列名称 ---地球站溃源数据表

	string m_ChooseIt_DQZBXSB;//用户选择筛选的列名称 ---地球站波形设备数据表
	string m_ChooseIt_DQZTX;//用户选择筛选的列名称 ---地球站天线
	string m_ChooseIt_DMZDSJB;//用户选择筛选的列名称 ---地面终端数据表
	string m_ChooseIt_ZDZHSJB;//用户选择筛选的列名称 ---终端载荷数据表
	string m_ChooseIt_BXSBSJB;//用户选择筛选的列名称 ---波形设备数据表
	string m_ChooseIt_ZDTX;//用户选择筛选的列名称 ---终端天线



	//string m_ChooseIt_DXSJB;//用户选择筛选的列名称 ---单星数据表
public:
	vector<string> Ptempdata;
	int PSatelliteNum;
	static unsigned __stdcall Busys(void* arglist);
};


