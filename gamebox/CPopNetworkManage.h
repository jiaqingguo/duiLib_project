#pragma once

//#include <Windows.h>
#include <iostream>
#include "UtilTool.h"
#include "ClueScreenClass.h"
#include "UdpRecvClass.h"
#include "Structural.h"
#include "CpopModifyInterface.h"
#include "CPopNewConstellation.h"
#include "CpopSelectEquipment.h"
#include "DrawingInterfaceClass.h" //��ͼ����
#include "CPopNewAttributeClass.h"
#include "CPopDeleteAttClass.h"

#include "CallFmuClass.h"//����FMU�ĺ���
//����������
class CPopNetworkManage: public WindowImplBase
{
public:
	CPopNetworkManage(void);
	~CPopNetworkManage(void);

	static int MessageBox(HWND hParent)
	{

		CPopNetworkManage* pWnd = new CPopNetworkManage();

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
	void AddComboBox();//��tab3��com���ӱ���

	void CreateTreeNode(string tableName);//�������ڵ�------------tab1
	void CreateTreeNode_2(string tableName);//�������ڵ�------------tab2
	void CreateTreeNode_3(string tableName);//�������ڵ�------------tab3
	void CreateTreeNode(CDuiString name1, CDuiString time);//�������ڵ�
	void CreateTreeNode_3(int num,int node,string name);//�����������ڵ�
	void CreateTreeNode_livel1(string tableName,int num);//����1���ڵ�--------tab1
	void CreateTreeNode_livel1_2(string tableName, int num);//����1���ڵ�--------tab2
	void CreateTreeNode_livel1_3(string tableName, int num);//����1���ڵ�--------tab3

	void ModifyNodeName();
	void ModifyNodeName_2();
	void ModifyNodeName_3();
	void saveDataLocally(string noodName,map<string,string> m_map);
	void saveDataLocally_WX(string noodName, map<string, string> m_map);
	void saveDataLocally_DQZ(string noodName, map<string, string> m_map);
	void saveDataLocally_DMZD(string noodName, map<string, string> m_map);
	//void CreateTreeNode_3(int num, string name);//�����������ڵ�

	void Adddisplay(vector<string> vec);//������tabҳ
	void Adddisplay_2(vector<string> vec);//������tab2ҳ
	void Adddisplay_3(vector<string> vec);//������tab2ҳ
	void parseElements();//tab1����ɸѡ�߹����� �й����� �͹����� ���豸
	void parseElements_2();//ɸѡҪ��ʾ���豸
	void parseElements_3();//ɸѡҪ��ʾ���豸
	void DrawAPicture(vector<string> vec_line1, vector<string> vec_line2, vector<string> vec_line3, vector<string> vec_line4);
	void DrawAPicture_Tab2(vector<string> vec_line1);//����2�Ļ�ͼ
	void DrawAPicture_Tab3(vector<string> vec_line1);//����3�Ļ�ͼ
	void FindTheMap();//����map
	void FindTheMap_2();//����map tab2
	void FindTheMap_3();//����map tab3
	void HideThCcontrol(int num);//���ؿؼ�
	void HideThCcontrol_2(int num,bool flag);//���ؿؼ�
	void HideThCcontrol_3(int num,bool flag);//���ؿؼ�tab3
	void HideThCcontrol_Draw(int num);//���ؽ���1��ͼ�ؼ�
	void HideThCcontro2_Draw(int num);//���ؽ���2��ͼ�ؼ�
	void HideThCcontro3_Draw(int num);//���ؽ���3��ͼ�ؼ�

	void DisplayInformation(map<string, string> m_map);
	void DisplayInformation_2(map<string, string> m_map);//tab2���Ͻǽ�����ʾ
	void DisplayInformation_3(map<string, string> m_map);//tab3���Ͻǽ�����ʾ

	void UncheckedTab1();//��Tab1ѡ�е����ڵ�ȡ��ѡ��
	void UncheckedTab2();//��Tab2ѡ�е����ڵ�ȡ��ѡ��
	void UncheckedTab3();//��Tab3ѡ�е����ڵ�ȡ��ѡ��
	map<string, string> GetDataFromInterfaces(int num);
	map<string, string> GetDataFromInterfaces_Tab3(int num);

	wstring stringToWstring(string str);
	string wstringToString(wstring wstr);

	string UTF8_To_string(const std::string & str);
	string string_To_UTF8(const std::string & str);

	vector<string> my_split(string str, string pattern);

	virtual string get_cur_timelogdata();

	DUI_DECLARE_MESSAGE_MAP()
	virtual void OnClick(TNotifyUI& msg);
	virtual void OnSelectChanged(TNotifyUI &msg);
	virtual void OnItemSelect(TNotifyUI &msg);


	void GenerateTheFile(string path, string filename, map<string, string> m_map);//�����ļ� ����������
	void GenerateTheFile(string path, string filename, vector<vector<string>> vecVec);//�������������Ƕ�Ӧ�������ļ�
	void CPopNetworkManage::GenerateTheFile(string path, string filename, map<string, string> m_map, string sign);

	map<string, string> GetCurrentEquipInform(string name);//��ȡtab1ҳ��ǰ�豸��Ϣ
	map<string, string> GetCurrentEquipInform_tab2(string name);//��ȡtab2ҳ��ǰ�豸��Ϣ
	map<string, string> GetCurrentEquipInform_tab3(string name);//��ȡtab2ҳ��ǰ�豸��Ϣ

	void RealTimeSceneData_Tab1();//tab1 ʵʱ�������ݵ��� ������ģ������
	void RealTimeSceneData_Tab2();//tab2 ʵʱ�������ݵ��� ������ģ������
	void RealTimeSceneData_Tab3();//tab3 ʵʱ�������ݵ��� ������ģ������

	void NotDrawable(string tabName,bool flag);//�������ݵĻ������Ի�ͼ ���������Ǹ����пؼ����óɲ��ɼ�

	void ImportingLocalData_Tab1();//��tab1����ı������ݵ�����溯��
	void ImportingLocalData_Tab2();
	void ImportingLocalData_Tab3();

	void RefreshTheInterface_tab1();//ˢ�½��� ��������0
	void RefreshTheInterface_tab2();
	void RefreshTheInterface_tab3();

	vector<double> AssemblePackageFMU_tab1(vector<string> vec_WX, vector<string> vec_DQZ, vector<string> vec_DMZD);//��ɵ���fmu�İ� tab1
	vector<double> AssemblePackageFMU_tab2(vector<string> vec_WX, vector<string> vec_DQZ, vector<string> vec_DMZD);//��ɵ���fmu�İ� tab2
	vector<double> AssemblePackageFMU_tab3(vector<string> vec_WX, vector<string> vec_DQZ, vector<string> vec_DMZD);//��ɵ���fmu�İ� tab3

	int getRootNodeOrder(string name);//tab1 ��ȡ���豸���ڵĸ��ڵ�λ��
	int getRootNodeOrder_2(string name);//tab2 ��ȡ���豸���ڵĸ��ڵ�λ��
	int getRootNodeOrder_3(string name);//tab2 ��ȡ���豸���ڵĸ��ڵ�λ��
	bool getWhetherDeviceStored(string name);//tab1 ��ȡ��ǰ�豸�Ƿ��Ѿ��洢 ��Ϊ֮��˫���豸��ʾ��Ϣ��׼��
	static UINT WINAPI UDPSendDataThread(void* pParam);//������ʹ��UDP����

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
	CStdStringPtrMap m_MenuInfos;
	CMenuWnd* m_pMenu;
public:
	vector<string> Ptempdata;
	int PSatelliteNum;
	static unsigned __stdcall Busys(void* arglist);

	CComboBoxUI  * select_combox_1;//�������ǽ����--ѡ����Ӫ��
	CComboBoxUI  * select_combox_2;//��������վ�����--ѡ����Ӫ��
	CComboBoxUI  * select_combox_3;//�����û������--ѡ����Ӫ��

	CComboBoxUI  * select_BusinessType;//��ʾ����ҵ�����͵Ŀؼ�
	CComboBoxUI  * select_BusinessType_1;//��ͼҪѡ���ҵ�����Ϳؼ�
	CListTextExtElementUI* pItem_File[1024];//�����ڴ������ǽ����listExÿһ�пؼ� 
	CListTextExtElementUI* pItem_DQZH[1024];//�����ڴ�������վ�����listExÿһ�пؼ� 
	CListTextExtElementUI* pItem_YH[1024];//�����ڴ����û������listEXÿһ�пؼ�

	string parameter_YYS[100];//����Ӫ�̵�
	vector<string> vec_OperatorName;//�����Ӫ��
	vector<string> vec_SatelliteName;//�����������
	vector<string> vec_EarthStation;//��ŵ���վ����
	vector<string> vec_UserName;//����û�����
	CTreeNodeUI* pNode_1[1024];//��Ÿ��ڵ�
	CTreeNodeUI* pNode_2[1024];//���1��ڵ� pNode_2[pNode_2_num]
	CTreeNodeUI* pNode_3[1024];//���2��ڵ�

	int pNode_1_num;//��ǰ���ڵ������е�˳��
	int pNode_2_num;//1��ڵ������е�˳��
	int pNode_3_num;//2��ڵ������е�˳��

	CTreeNodeUI* pNode_2_1[1024];//��Ÿ��ڵ�
	CTreeNodeUI* pNode_2_2[1024];//���1��ڵ� pNode_2[pNode_2_num]
	CTreeNodeUI* pNode_2_3[1024];//���2��ڵ�

	int pNode_2_1_num;//��ǰ���ڵ������е�˳��
	int pNode_2_2_num;//1��ڵ������е�˳��
	int pNode_2_3_num;//2��ڵ������е�˳��

	CTreeNodeUI* pNode_3_1[1024];//��Ÿ��ڵ�
	CTreeNodeUI* pNode_3_2[1024];//���1��ڵ� pNode_2[pNode_2_num]
	CTreeNodeUI* pNode_3_3[1024];//���2��ڵ�

	int pNode_3_1_num;//��ǰ���ڵ������е�˳��
	int pNode_3_2_num;//1��ڵ������е�˳��
	int pNode_3_3_num;//2��ڵ������е�˳��

	bool isLevel0_1;//��һ����������ѡ�����0��
	bool isLevel0_2;//��һ����������ѡ�����0��
	bool isLevel0_3;//��һ����������ѡ�����0��

	int selectNode[1][3];//���ѡ�е������νṹ�е�λ�� [0][0]���ѡ�е�0���ڵ� [0][1]���ѡ�е�1���ڵ� [0][2]���ѡ�е�2���ڵ�
	int selectNode_2[1][3];//���ѡ�е������νṹ�е�λ�� [0][0]���ѡ�е�0���ڵ� [0][1]���ѡ�е�1���ڵ� [0][2]���ѡ�е�2���ڵ�
	int selectNode_3[1][3];//���ѡ�е������νṹ�е�λ�� [0][0]���ѡ�е�0���ڵ� [0][1]���ѡ�е�1���ڵ� [0][2]���ѡ�е�2���ڵ�


	vector<string> vec_root_1;//��һ����������ڵ�0��Ŀ¼
	vector<string> vec_root_2;//�ڶ�����������ڵ�0��Ŀ¼
	vector<string> vec_root_3;//�ڶ�����������ڵ�0��Ŀ¼

	vector<string> vec_level2_1;//�洢tab2��1���ڵ�����
	vector<map<string, string>> m_mapLevel2_1;//���tab2һ���ڵ���������

	vector<string> vec_level3_1;//�洢tab2��1���ڵ�����
	vector<map<string, string>> m_mapLevel3_1;//���tab2һ���ڵ���������

	NetworkManagement2 m_NetworkManagement2;
	NetworkManagement m_NetworkManagement;//���紫��Ľṹ��
	CTreeViewUI*	m_treeView;//��������ʾ��Ϣ�Ŀؼ�
	CTreeViewUI*	m_treeView_2;//��������ʾ��Ϣ�Ŀؼ�
	CTreeViewUI*	m_treeView_3;//��������ʾ��Ϣ�Ŀؼ�

	CpopModifyInterface * m_CpopModifyInterface;
	CPopNewConstellation * m_CPopNewConstellation;//�½�����
	string InterfaceNetwork_1;
	CpopSelectEquipment * m_CpopSelectEquipment;

	DrawingInterfaceClass * m_DrawingInterfaceClass;//�������Ļ�ͼ����
	CPopNewAttributeClass * m_CPopNewAttributeClass_Net;//��������½��豸����
	CPopDeleteAttClass * m_CPopDeleteAttClass;//ɾ����������

	CDuiString m_modiftName_1;//Ҫ�޸ĵ����ڵ�����
	string newName_1;//�޸ĺ�����ڵ�����

	CDuiString m_modiftName_2;//Ҫ�޸ĵ����ڵ�����
	string newName_2;//�޸ĺ�����ڵ�����

	CDuiString m_modiftName_3;//Ҫ�޸ĵ����ڵ�����
	string newName_3;//�޸ĺ�����ڵ�����

	vector<string> vec_recEquipmentName;//���յ����豸���� ���ǡ�����վ���ն�
	vector<map<string, string>> m_RecvMap;//��������豸��������Ϣ ���ǡ�����վ���ն�

	vector<string> vec_recSatelliteName;//���յ�����������
	vector<map<string, string>> m_RecvSatelliteMap;//������ǰ�������Ϣ

	vector<string> vec_recEarthStationName;//���յ��ĵ���վ����
	vector<map<string, string>> m_RecvEarthStationMap;//����������ĵ���վ����Ϣ

	vector<string> vec_recGroundTerminalName;//���յ��ĵ����ն�����
	vector<map<string, string>> m_RecvGroundTerminalMap;//������е����ն�����������Ϣ
	
	vector<string> vec_StartEndPointTab1;//tab1����ѡ��ĵ�ǰ�����е��������ǡ�����վ�������ն�
	vector<string> vec_SequenceDisplayTab1;//��tab1ѡ������ǡ�����վ�������ն˰��ո�fmu����е�˳���������

	vector<string> vec_StartEndPointTab2;//tab2����ѡ��ĵ�ǰ�����е��������ǡ�����վ�������ն�
	vector<string> vec_SequenceDisplayTab2;//��tab2ѡ������ǡ�����վ�������ն˰��ո�fmu����е�˳���������

	vector<string> vec_StartEndPointTab3;//tab3����ѡ��ĵ�ǰ�����е��������ǡ�����վ�������ն�
	vector<string> vec_SequenceDisplayTab3;//��tab3ѡ������ǡ�����վ�������ն˰��ո�fmu����е�˳���������

	double intTime[6];
	double ts;

	vector<string> vec_Type;//���������������
	ClueScreenClass * m_ClueScreenClass;
	UDPAdmin1* m_UDPAdmin1;

	map<string, string> m_Demo_Tab2;//���tab2�Ķ����ڵ����������Ժ�Ĭ��ֵ
	map<string, string> m_Demo_Tab3;//���tab3�Ķ����ڵ����������Ժ�Ĭ��ֵ

	string siftType;//tab2��ѡ���ɸѡ����
	string siftTypeValue;//tab2��ѡ���ɸѡ���Ե�ֵ

	string str_BusinessType;//ѡ��Ҫ��ͼ��ҵ������
	double tab1Group[1818];	//��fmu�İ���ŵĵط�

	NetworkPackets m_NetworkPackets;//�������������ݰ�(Ϊ�ܹ��Ǳߵ���fmuʹ��)

	CallFmuClass * m_CallFmuClass;//����fmu�ĺ���

	string CurrentPath;//��ǰ·��
};

