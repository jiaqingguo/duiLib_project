#pragma once

#include <Windows.h>
#include <iostream>
#include "Structural.h"
//#include "UDP.h"
#include "UdpRecvClass.h"
#include "UtilTool.h"
#include "CPopNewConstellation.h"
//#include "DecodeDll.h"

//��·�������������
class CPopLinkManage : public WindowImplBase
{
public:
	CPopLinkManage(void);
	~CPopLinkManage(void);

	wstring stringToWstring(string str);
		//wstringתstring
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

	void InitializeList();//��List������Ӷ���
	void InitializeList(vector<string> vec);//��List������Ӷ���
	void UpdateList(vector<vector<string>> vec);//����list�е�����
	void ReceiveData(vector<string> vec);
	void InitializeCombo_link();//��combox������Ӷ���
	void InitializeCombo_User(vector<string> vec);//���û���combo�Ӷ���
	void InitializeCombo_Satellite(vector<string> vec);//���û���combo�Ӷ���

	void GenerateTheFile(string path, string filename, vector<string> m_vec);
	void ImportingLocalData();//���뱾������

	void AddingDataToList(int num);
	void JudgmentDataShows();//�ж������Ƿ���ʾ
	void JudgmentDataShows_1();//�ж������Ƿ���ʾ
	void ReceiveDataLocally(EffectiveLinkTable table);//�������ݵ�����
	void UpdatingTheInterface();//���½���
	void ParsingDisplay();//������ʾ
	void ParsingDisplay(EffectiveLinkTable table);//��������ʾ����

	static UINT WINAPI UDPSendSerialPortThread(void* pParam);//������ʹ��UDP����
	static UINT WINAPI UDPRecvZKThread(void* pParam);//����ʹ��UDP������������


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

	UDPAdmin1 * m_Udpadmin1;//�������յ������ݷ��͸��ܿ�

	string m_selectConditions;//�ѽ�ѡ���ɸѡ����

	EffectiveLinkTable m_EffectiveLinkTable;//��Ч��·�б�ṹ��
	string m_ChooseIt;//�û��������Ҫѡ�������

	UdpRecvClass* m_UdpRecvClass;
	int SelectListLine;

	vector<string> vec_localUser;//����û�����
	vector<string> vec_localSatellite;//�����������

	CPopNewConstellation * m_CPopNewConstellation;//�½�������������
	vector<vector<string> > vec_LinkData;//��������·���ݴ�������������

	vector<EffectiveLinkTable> m_vecEffectLink;//�����յ������ݴ洢�ڱ���
	CListUI* m_dataBSList;
};

