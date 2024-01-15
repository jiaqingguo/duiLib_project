#pragma once

#include <iostream>
#include "ConnectMysql.h"
//���ݹ������
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
	void InitializeMap();//��ʼ��map

	void InitializeCombo();//��Combo������Ӷ���
	void Filter_XZSJB();//�������ݱ�
	void Filter_DXSJB();//�������ݱ�ɸѡ
	void Filter_WXZHSJB();//�����غ����ݱ�ɸѡ
	void Filter_WXTX();//��������ɸѡ
	void Filter_DXGSJB();//���Ÿ����ݱ�ɸѡ
	void Filter_DQZSJB();//����վ���ݱ�ɸѡ
	void Filter_DQZKYSJB();//����վ��Դ���ݱ�ɸѡ

	void Filter_DQZBXSBSJB();//����վ�����豸���ݱ�ɸѡ
	void Filter_DQZTXSJB();//����վ����ɸѡ
	void Filter_DMZDSJB();//�����ն����ݱ�ɸѡ
	void Filter_ZDZHSJBSJB();//�ն��غ����ݱ�ɸѡ
	void Filter_BXSBSJBSJB();//�����豸���ݱ�ɸѡ
	void Filter_ZDTXSJB();//�ն�����ɸѡ


	//void Filter_DXSJB();//�������ݱ�ɸѡ
	//void Filter_DXSJB();//�������ݱ�ɸѡ
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
	CListUI* m_dataList_XZSJB;//�������ݱ��List
	CListUI* m_dataList_DXSJB;//�������ݱ��List
	CListUI* m_dataList_WXZHSJB;//�����غ����ݱ��List
	CListUI* m_dataList_WXTX;//�������ߵ�List
	CListUI* m_dataList_DXGSJB;//���Ÿ����ݱ��List
	CListUI* m_dataList_DQZSJB;//����վ���ݱ��List
	CListUI* m_dataList_DQZKYSJB;//����վ��Դ���ݱ��List

	CListUI* m_dataList_DQZBXSB;//����վ�����豸���ݱ��List
	CListUI* m_dataList_DQZTX;//����վ���ߵ�List
	CListUI* m_dataList_DMZDSJB;//�����ն����ݱ��List
	CListUI* m_dataList_ZDZHSJB;//�ն��غ����ݱ��List
	CListUI* m_dataList_BXSBSJB;//�����豸���ݱ��List
	CListUI* m_dataList_ZDTX;//�ն����ߵ�List

private:
	vector<vector<string>> vec_vecData_XZSJB;//��ȡ�����ݿ���---�������ݱ�
	vector<vector<string>> vec_vecData_DXSJB;//��ȡ�����ݿ���---�������ݱ�
	vector<vector<string>> vec_vecData_WXZHSJB;//��ȡ�����ݿ���---�����غ����ݱ�
	vector<vector<string>> vec_vecData_WXTX;//��ȡ�����ݿ���---��������
	vector<vector<string>> vec_vecData_DXGSJB;//��ȡ�����ݿ���---���Ÿ����ݱ�
	vector<vector<string>> vec_vecData_DQZSJB;//��ȡ�����ݿ���---����վ���ݱ�
	vector<vector<string>> vec_vecData_DQZKYSJB;//��ȡ�����ݿ���---����վ��Դ���ݱ�

	vector<vector<string>> vec_vecData_DQZBXSB;//��ȡ�����ݿ���---����վ�����豸���ݱ�
	vector<vector<string>> vec_vecData_DQZTX;//��ȡ�����ݿ���---����վ����
	vector<vector<string>> vec_vecData_DMZDSJB;//��ȡ�����ݿ���---�����ն����ݱ�
	vector<vector<string>> vec_vecData_ZDZHSJB;//��ȡ�����ݿ���---�ն��غ����ݱ�
	vector<vector<string>> vec_vecData_BXSBSJB;//��ȡ�����ݿ���---�����豸���ݱ�
	vector<vector<string>> vec_vecData_ZDTX;//��ȡ�����ݿ���---�ն�����

	CButtonUI* m_pCloseBtn;
	CButtonUI* m_pMaxBtn;
	CButtonUI* m_pRestoreBtn;
	CButtonUI* m_pMinBtn;
	CButtonUI* m_pMenuBtn;
	CTabLayoutUI* pTabDatabase_0;//page1��page2��tab
	CTabLayoutUI* pTabDatabase_1;//page1��7�������tab
	CTabLayoutUI* pTabDatabase_2;//page2�����Ž����tab

	CComboBoxUI  *  select_combox;
	ConnectMysql myMysql;

	string FilterArray_1[5];//�������ݱ�ɸѡ����
	string FilterArray_2[7];//�������ݱ�ɸѡ����
	string FilterArray_3[4];//�����غ����ݱ�ɸѡ����
	string FilterArray_4[4];//�����������ݱ�ɸѡ����
	string FilterArray_5[5];//���Ÿ����ݱ�ɸѡ����
	string FilterArray_6[5];//����վ���ݱ�ɸѡ����
	string FilterArray_7[5];//����վ��Դ���ݱ�ɸѡ����
	string FilterArray_8[4];//����վ�����豸���ݱ�ɸѡ����
	string FilterArray_9[4];//����վ�������ݱ�ɸѡ����
	string FilterArray_10[3];//�����ն����ݱ�ɸѡ����
	string FilterArray_11[5];//�ն��غ����ݱ�ɸѡ����
	string FilterArray_12[3];//�ն˲����豸���ݱ�ɸѡ����
	string FilterArray_13[4];//�ն��������ݱ�ɸѡ����

	map<string, string>  FilterArray_map1;//���Ҫɸѡ���������ݱ��map
	map<string, string>  FilterArray_map2;//��ŵ������ݱ��map
	map<string, string>  FilterArray_map3;//��������غ����ݱ����ݱ��map
	map<string, string>  FilterArray_map4;//��������������ݱ��map
	map<string, string>  FilterArray_map5;//��ŵ��Ÿ����ݱ��map
	map<string, string>  FilterArray_map6;//��ŵ���վ���ݱ��map
	map<string, string>  FilterArray_map7;//��ŵ���վ��Դ���ݱ��map

	map<string, string>  FilterArray_map8;//��ŵ���վ�����豸���ݱ��map
	map<string, string>  FilterArray_map9;//��ŵ���վ���ߵ�map
	map<string, string>  FilterArray_map10;//��ŵ����ն����ݱ��map
	map<string, string>  FilterArray_map11;//����ն��غ����ݱ��map
	map<string, string>  FilterArray_map12;//����ն˲����豸���ݱ��map
	map<string, string>  FilterArray_map13;//����ն����ߵ�map

	string m_selectConditionsXZSJB;//ѡ����������ݱ�����
	string m_selectConditions_DXSJB;//ѡ��ĵ������ݱ�����
	string m_selectConditions_WXZHSJB;//ѡ��������غ����ݱ�����
	string m_selectConditions_WXTX;//ѡ���������������
	string m_selectConditions_DXGSJB;//ѡ��ĵ��Ÿ����ݱ�����

	string m_selectConditions_DQZSJB;//ѡ��ĵ���վ���ݱ�����
	string m_selectConditions_DQZKYSJB;//ѡ��ĵ���վ��Դ���ݱ�����
	string m_selectConditions_DQZBXSJB;//ѡ��ĵ���վ�������ݱ�����
	string m_selectConditions_DQZTXSJB;//ѡ��ĵ���վ��������
	string m_selectConditions_DMZDSJB;//ѡ��ĵ����ն����ݱ�����

	string m_selectConditions_DQZDZHSJB;//ѡ����ն��غ����ݱ�����
	string m_selectConditions_BXSBSJB;//ѡ����ն˲����豸���ݱ�����
	string m_selectConditions_ZDTXSJB;//ѡ����ն���������

	CListUI* m_dataBSList_1;//�������ݱ��LIst

	string m_ChooseIt_XZSJB;//�û�ѡ��ɸѡ�������� ---�������ݱ�
	string m_ChooseIt_DXSJB;//�û�ѡ��ɸѡ�������� ---�������ݱ�
	string m_ChooseIt_WXZHSJB;//�û�ѡ��ɸѡ�������� ---�����غ����ݱ�
	string m_ChooseIt_WXTX;//�û�ѡ��ɸѡ�������� ---��������
	string m_ChooseIt_DXGSJB;//�û�ѡ��ɸѡ�������� ---���Ÿ����ݱ�
	string m_ChooseIt_DQZSJB;//�û�ѡ��ɸѡ�������� ---����վ���ݱ�
	string m_ChooseIt_DQZKYSJB;//�û�ѡ��ɸѡ�������� ---����վ��Դ���ݱ�

	string m_ChooseIt_DQZBXSB;//�û�ѡ��ɸѡ�������� ---����վ�����豸���ݱ�
	string m_ChooseIt_DQZTX;//�û�ѡ��ɸѡ�������� ---����վ����
	string m_ChooseIt_DMZDSJB;//�û�ѡ��ɸѡ�������� ---�����ն����ݱ�
	string m_ChooseIt_ZDZHSJB;//�û�ѡ��ɸѡ�������� ---�ն��غ����ݱ�
	string m_ChooseIt_BXSBSJB;//�û�ѡ��ɸѡ�������� ---�����豸���ݱ�
	string m_ChooseIt_ZDTX;//�û�ѡ��ɸѡ�������� ---�ն�����



	//string m_ChooseIt_DXSJB;//�û�ѡ��ɸѡ�������� ---�������ݱ�
public:
	vector<string> Ptempdata;
	int PSatelliteNum;
	static unsigned __stdcall Busys(void* arglist);
};


