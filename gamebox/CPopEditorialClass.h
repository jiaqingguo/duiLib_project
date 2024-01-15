#pragma once

//#include <Windows.h>
#include <iostream>
#include "UtilTool.h"
#include <vector>
#include <string>
#include "CpopModifyInterface.h"
#include "udp.h"
#include "Structural.h"
#include "CPopNewConstellation.h"
#include "CpopSelectLevel1.h"
#include "CPopNewAttributeClass.h"
#include "CPopDeleteAttClass.h"
//�����������
class CPopEditorialClass : public WindowImplBase
{
public:
	CPopEditorialClass(void);
	~CPopEditorialClass(void);

	static int MessageBox(HWND hParent)
	{

		CPopEditorialClass* pWnd = new CPopEditorialClass();

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
	vector<string> my_split(string str, string pattern);
	wstring stringToWstring(string str);//stringתwstring
	string wstringToString(wstring wstr);//wstringתstring

	void CreateTreeNode();//�������ڵ�
	void CreateTreeNode(string tableName);//�������ڵ�

	void CreateTreeNode_2(string tableName);//�����ڶ�������ĸ��ڵ�
	void CreateTreeNode(string tableName,string str);//�����ӽڵ�

	void CreateTreeNode_3(string tableName);//��������������ĸ��ڵ�

	void CreateTreeNode_livel1(string tableName);//����һ���ڵ�
	void CreateTreeNode_livel2(string tableName,int first_level);//���������ڵ�
	void CreateTreeNode_livel3(string tableName, int first_level);//���������ڵ� firstlevel��ָ

	void CreateTreeNode_livel2_1(string tableName);//����һ���ڵ�-----------���Ÿ۵���վ����
	void CreateTreeNode_livel2_2(string tableName, int first_level);//���������ڵ�--------------���Ÿ۵���վ����
	void CreateTreeNode_livel2_3(string tableName, int first_level);//���������ڵ� firstlevel��ָ

	void CreateTreeNode_livel3_1(string tableName);//����һ���ڵ�-----------�����ն�
	void CreateTreeNode_livel3_2(string tableName,int first_level);//���������ڵ�-----------�����ն��ӽڵ�

	void GetNumberChild_0();//��ȡ0���ڵ��ӽڵ������ Ҳ���ǻ�ȡ���ڵ���ӽڵ�����
	void GetNumberChild();//��ȡһ���ڵ��ӽڵ������
	void Movelevel_2(string level1, CDuiString level2);//�ƶ����ǽڵ�
	void Categorize_XZ_RailType();//���������
	void Restore_XZ_RailType();//��������ԭ

	void Categorize_XZ_Functional();//����������
	void Restore_XZ_Functional();//�����������ԭ

	void Categorize_XZ_LoadType();//�����غ����ͷ���
	void Restore_XZ_LoadType();//�غ����ͷ����ԭ

	void  Restore_XZ_classification();//�鿴��ǰ�Ƿ��������ڷ���
	void  Restore_DXG_classification();//�鿴��ǰ�Ƿ��е��Ÿ��ڷ���


	void Categorize_DXG_characteristic();//���Ÿ۰����ʷ���
	void Restore_DXG_characteristic();//�������ʷ����ԭ

	void Categorize_WX_LoadFunction(string name);//����-----�����غɹ��ܷ���
	void Restore_WX_LoadFunction(string name);//����------���غɹ��ܷ����ԭ

	void Categorize_DQZ_FunctionType1(string name);//����վ-----�����غɹ��ܷ���
	void Restore_DQZ_FunctionType1(string name);//����վ------���غɹ��ܷ����ԭ

	void Categorize_DQZ_EarthStation(string name);//����վ-------������վ���ͷ���
	void Restore_DQZ_EarthStation(string name);//����վ-------������վ���ͷ����ԭ

	void Categorize_DMZD_TerminalType();//���Ÿ۰����ʷ���
	void Restore_DMZD_TerminalType();//�������ʷ����ԭ

	void Movelevel_2_ToDXG(string level1, CDuiString level2);//�ƶ�����վ��ĳ�����Ÿ۽ڵ�

	void RootNodeOnly1();//���ิԭ��ֻ�����ڵ�
	void EmptyChildNode_tab1();//����ӽڵ�
	void EmptyChildNode_tab2();//����ӽڵ�
	void EmptyChildNode_tab3();//����ӽڵ�
	int GetNoodLevel(string nodeName);//��ȡ������Ľڵ���������˳��
	int GetNoodLevel_Root(string nodeName);//��ȡ������ĸ��ڵ�����˳��
	int GetNoodLevel_level2(string nodeName,int serialNumber);//��ȡ������Ľڵ���������˳��

	int GetNoodLevel_Root_DXG(string nodeName);//����Ÿ����ʵĸ��ڵ�����˳��
	int GetNoodLevel_level2_DQZ(string nodeName, int serialNumber);//��ȡ����վ������������˳��
	void RootNodeOnly1_DXG();//���ิԭ��ֻ�����ڵ�

	int GetNoodLevel_Root_DMZD(string nodeName);//������ն����͵ĸ��ڵ�����˳��

	int GetNoodLevel_DXG_XZ(string nodeName);//��ȡ���Ÿ����ʵĽڵ���������˳��

	int GetNoodLevel_DMZD_ZDLX(string nodeName);//��ȡ�����ն˵��ն����͵Ľڵ���������˳��
	void RootNodeOnly1_DMZD();//�����ն˷��ิԭ��ֻ�����ڵ�

	void AssignmentToLocalVector_XZ(string nodeName);//����һ��Ŀ¼�����ڵ� --------����
	void AssignmentToLocalVector_XZ(string nodeName,map<string,string> m_map);//����һ��Ŀ¼�����ڵ� --------����
	void AssignmentToLocalVector(string nodeName, map<string, string> m_map, int first_level);//��������Ŀ¼�����ڵ� --------����
	void AssignmentToLocalVector_WXSX(string wx_name,string type,map<string, string> m_map);//��������Ŀ¼�����ڵ� --------����������Ϣ �غ�������Ϣ
	void AssignmentToLocalVector(string parent,string nodeName, map<string, string> m_map);//��������Ŀ¼�����ڵ� --------���� �����ʱ��
	void AssignmentToLocalVector_TxZh(string parent, string nodeName, map<string, string> m_map);//��������Ŀ¼�����ڵ� �����ʱ��--------����������Ϣ �غ�������Ϣ

	void AssignmentToLocalVector_DXG(string nodeName);//����һ��Ŀ¼�����ڵ� --------���Ÿ�
	void AssignmentToLocalVector_DXG(string nodeName, map<string, string> m_map);//�����ʱ�򴴽�һ��Ŀ¼�����ڵ� --------���Ÿ�
	void AssignmentToLocalVector_DQZ(string nodeName, map<string, string> m_map, int first_level);//��������Ŀ¼�����ڵ� --------����վ
	void AssignmentToLocalVector_DQZ(string wx_name, string type, map<string, string> m_map);//��������Ŀ¼�����ڵ� --------����վ
	void AssignmentToLocalVector_DQZ_level2(string parent, string nodeName, map<string, string> m_map);//��������Ŀ¼�����ڵ� --------����վ �����ʱ��
	void AssignmentToLocalVector_DQZ_level3(string parent, string nodeName, map<string, string> m_map);//��������Ŀ¼�����ڵ� �����ʱ��--------��Դ��������

	void AssignmentToLocalVector_DMZD(string nodeName, map<string, string> m_map);//����һ��Ŀ¼�����ڵ� --------�����ն�
	void AssignmentToLocalVector_DMZD_level2(string parent,string type, map<string, string> m_map);//��������Ŀ¼�����ڵ� --------�����ն��ӽڵ�
	void AssignmentToLocalVector_DMZD_Child(string parent, string nodeName, map<string, string> m_map);//��������Ŀ¼�����ڵ� --------�غɲ������� �����ʱ��

	//void AssignmentToLocalVector_DMZD(string nodeName, map<string, string> m_map);//�����ʱ�򴴽�һ��Ŀ¼�����ڵ� --------�����ն�


	void ImportingLocalData_Tab1();//��tab1����ı������ݵ�����溯��
	void ImportingLocalData_Tab2();
	void ImportingLocalData_Tab3();
	void RefreshTheInterface_tab1();//ˢ�½��� ��������0
	void RefreshTheInterface_tab2();
	void RefreshTheInterface_tab3();

	void DisplayInformation(map<string, string> m_map);//��ʾ��Ϣ
	void DisplayInformation_2(map<string, string> m_map);//��ʾ���Ÿ۵���վ��Ϣ
	void DisplayInformation_3(map<string, string> m_map);//��ʾ�����ն���Ϣ
	void FindTheMap();//����map
	void FindTheMap_2();//����map-------����2
	void FindTheMap_3();//����map-------����3
	void FindTheMap_level3(CDuiString cdui_name);//˫��3��Ŀ¼
	void FindTheMap_level4(CDuiString cdui_name);//��������������ǽ���ɸѡ��ʱ�� ˫��4��Ŀ¼
	void FindTheMap_level3_2(CDuiString cdui_name);//�ڶ�������˫������Ŀ¼
	void FindTheMap_level4_2(CDuiString cdui_name);//�Բ����������tab2 ˫��4��Ŀ¼
	//void FindTheMap_RailType();//����map
	map<string, string> GetHoroscopeInform();//��txt��ȡ������Ϣ
	map<string, string> GetSatelliteInform();//��txt��ȡ������Ϣ

	map<string, string> GetSatelliteInform_1();//��txt��ȡ���ǵ�����������Ϣ
	map<string, string> GetSatelliteInform_2();//��txt��ȡ���ǵ��غ�������Ϣ

	map<string, string> GetTeleport();//��txt��ȡ���Ÿ���Ϣ
	map<string, string> GetEarthStation();//��txt��ȡ����վ��Ϣ
	map<string, string> GetEarthStation_1();//��txt��ȡ����վ��Դ������Ϣ tab2
	map<string, string> GetEarthStation_2();//��txt��ȡ����վ��ȡ�����豸������Ϣ tab2
	map<string, string> GetEarthStation_3();//��txt��ȡ����վ����������Ϣ tab2

	map<string, string> GetGroundTerminal();//��txt��ȡ�����ն˵���Ϣ
	map<string, string> GetGroundTerminal_1();//��txt��ȡ�����ն����غ�������Ϣ
	map<string, string> GetGroundTerminal_2();//��txt��ȡ�����ն��в����豸������Ϣ
	map<string, string> GetGroundTerminal_3();//��txt��ȡ�����ն�������������Ϣ

	//void NodesContainData(string arrName,string arr,int num);//��������������������ݴ�����Ա������
	void HideThCcontrol(int num);//���ؿؼ�
	void HideThCcontrol_2(int num);//���ؿؼ�
	void HideThCcontrol_3(int num);//���ؿؼ� -------����3
	map<string,string> GetDataFromInterfaces();//������1�������޸ĺ�洢��txt
	map<string,string> GetDataFromInterfaces_2();//������2�������޸ĺ�洢��txt
	map<string,string> GetDataFromInterfaces_3();//������3�������޸ĺ�洢��txt

	void UncheckedTab1();
	void UncheckedTab2();
	void UncheckedTab3();

	void GenerateTheFile(string path,string filename,map<string,string> m_map,string sign);//�����ļ� ����������
	void GenerateTheFile(string path, string filename, vector<vector<string>> vecVec);//�������������Ƕ�Ӧ�������ļ�

	//void SaveData();

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
	CStdStringPtrMap m_MenuInfos;
	CMenuWnd* m_pMenu;

	
public:
	vector<string> Ptempdata;
	int PSatelliteNum;
	static unsigned __stdcall Busys(void* arglist);

	UDPRecvtle Udprecvtle;
	CListTextExtElementUI* pItem_File[1024];//�����ڴ������ǽ����listExÿһ�пؼ� 
	CListTextExtElementUI* pItem_DQZH[1024];//�����ڴ�������վ�����listExÿһ�пؼ� 
	CListTextExtElementUI* pItem_YH[1024];//�����ڴ����û������listEXÿһ�пؼ�

	void ModifyNodeName();//�޸Ľڵ�����
	void ModifyNodeName_2();//�޸Ľڵ����� --���Ÿ�
	void ModifyNodeName_3();//�޸Ľڵ����� ---�����ն��豸
	CTreeNodeUI* pNode_1[1024];//��Ÿ��ڵ�
	CTreeNodeUI* pNode_2[1024];//��Ŷ���ڵ� ������һ�� pNode_2[pNode_2_num]
	CTreeNodeUI* pNode_3[1024];//�������ڵ�
	CTreeNodeUI* pNode_4[1024];//�������ڵ�

	CTreeNodeUI* pNode_2_1[1024];//��ŵڶ�������ĸ��ڵ�
	CTreeNodeUI* pNode_2_2[1024];//��ŵڶ��������1���ڵ�
	CTreeNodeUI* pNode_2_3[1024];//��ŵڶ��������2���ڵ�
	CTreeNodeUI* pNode_2_4[1024];//��ŵڶ��������2���ڵ�

	CTreeNodeUI* pNode_3_1[1024];//��ŵڶ�������ĸ��ڵ�
	CTreeNodeUI* pNode_3_2[1024];//��ŵڶ��������1���ڵ�
	CTreeNodeUI* pNode_3_3[1024];//��ŵڶ��������2���ڵ�
	//CTreeNodeUI* pNode_4[1024];//����Ĳ�ڵ�
	CLabelUI* plabel[1024];//���label

	int tab3RootType;//tab3���������͵ĸ��ڵ����

	int pNode_1_num;//��ǰ���ڵ������е�˳��
	int pNode_2_num;//һ���ڵ������е�˳��
	int pNode_3_num;//�����ڵ������е�˳��
	int pNode_4_num;//�����ڵ������е�˳��
	int pNode_5_num;//�����ڵ������е�˳��

	int pNode_2_1_num;//��ǰ���ڵ������е�˳��
	int pNode_2_2_num;//һ���ڵ������е�˳��
	int pNode_2_3_num;//�����ڵ������е�˳��
	int pNode_2_4_num;//�����ڵ������е�˳��

	int pNode_3_1_num;//��ǰ���ڵ������е�˳��
	int pNode_3_2_num;//һ���ڵ������е�˳��
	int pNode_3_3_num;//�����ڵ������е�˳��
	int pNode_3_4_num;//�����ڵ������е�˳��


	vector<string> name_Level3;//��������ڵ�--��������������Ժ��غ���������
	vector<string> name_Level2;//��Ŷ����ڵ�--��������
	vector<string> name_Level1;//���һ���ڵ�--��������

	vector<bool> vec_ExpandFlags;//��������ڵ��Ƿ�չ��(�Ѿ�ɸѡ��־)
	vector<bool> vec_ExpandFlags2_FT;//��ŵ��Ÿ��Ƿ�����˹�������ɸѡ
	vector<bool> vec_ExpandFlags2_ES;//��ŵ��Ÿ��Ƿ�����˵���վ����ɸѡ

	vector<string> name2_Level3;//��������ڵ�--����վ��һ��
	vector<string> name2_Level2;//��Ŷ����ڵ�--����վ
	vector<string> name2_Level1;//���һ���ڵ�--���Ÿ�

	vector<string> name3_Level2;//��Ŷ����ڵ�--�����ն��ӽڵ�����
	vector<string> name3_Level1;//���һ���ڵ�--�����ն�

	vector<string> name_Level3_type;//�������ɸѡ��ʱ��Ľڵ�

	int NumberChild[100];//ÿ��һ��ڵ�Ķ���ڵ�ĸ���
	int NumberChild_0[100];//ÿ��0��ڵ��1��ڵ�ĸ���
	vector<vector<string>> SecondLevelNode;//�洢����ڵ�����
	int CurrentIndex;//ѡ�е����ڵ��ڶ���ڵ��λ��
	CTreeViewUI*	m_treeView;//��ʾ�������ǽ�������ؼ�
	CTreeViewUI*	m_treeView_2;//��ʾ���Ÿ۵���վ��������ؼ�
	CTreeViewUI*	m_treeView_3;//���泡�������ն˽�������ؼ�

	static unsigned __stdcall ThreadReadtle(void* arglist);
	vector<vector<string>> vecVec_Node1_1;//������а���������
	string SelectedNode;
	
	int selectNode[1][4];//���ѡ�е������νṹ�е�λ�� [0][0]���ѡ�е�0���ڵ� [0][1]���ѡ�е�1���ڵ� [0][2]���ѡ�е�2���ڵ�
	int selectNode_2[1][4];//���ѡ�е������νṹ�е�λ�� [0][0]���ѡ�е�0���ڵ� [0][1]���ѡ�е�1���ڵ� [0][2]���ѡ�е�2���ڵ�
	int selectNode_3[1][3];//���ѡ�е������νṹ�е�λ�� [0][0]���ѡ�е�0���ڵ� [0][1]���ѡ�е�1���ڵ� [0][2]���ѡ�е�2���ڵ�
	int ModifyNode[1][3];//Ҫ�޸ĵ����ڵ�������ṹ�е�λ��
	vector<string> vec_root;//��Ÿ��ڵ�����
	vector<string> vec_2_root;//��ŵڶ�������ĸ��ڵ�
	vector<string> vec_3_root;//��ŵ���������ĸ��ڵ�
	//map<string, string> map1_16;
	//map<string, string> map1_17;
	//map<string, string> map1_18;

	bool isLevel0;//ѡ�е���0���ڵ���һ��
	bool isLevel1;//ѡ�е���1���ڵ���һ��
	bool isLevel2;//ѡ�е���2���ڵ���һ��

	bool isLevel_2_0;//ѡ�е���0���ڵ���һ��
	bool isLevel_2_1;//ѡ�е���1���ڵ���һ��
	bool isLevel_2_2;//ѡ�е���2���ڵ���һ��

	bool isLevel_3_0;//ѡ�е���0���ڵ���һ��
	bool isLevel_3_1;//ѡ�е���1���ڵ���һ��



	vector<map<string, string>> m_RecvMap_XZ;//���������������������Ϣ
	//vector<map<string, string>> m_RecvMap_XZ_RailType;//��Ź������vector
	vector<map<string, string>> m_RecvMap;//���������������������Ϣ
	vector<map<string, string>> m_RecvMap_TxZh;//���������ص����ߺ��غ�������Ϣ

	vector<map<string, string>> m_RecvMap_DQZChild;//��ŵ���վ��������������Ϣ
	vector<map<string, string>> m_RecvMap_DQZ;//��ŵ���վ��������������Ϣ
//vector<map<string, string>> m_RecvMap_XZ_RailType;//��Ź������vector
	vector<map<string, string>> m_RecvMap_DXG;//��ŵ��Ÿ���������������Ϣ

	vector<map<string, string>> m_RecvMap_DMZDChild;//��ŵ����ն���������������Ϣ
	vector<map<string, string>> m_RecvMap_DMZD;//��ŵ����ն���������������Ϣ
	vector<pair<string, string>> ModifiedData;//�޸ĺ������

	CpopModifyInterface * m_CpopModifyInterface;
	CPopNewConstellation * m_CPopNewConstellation;//�½�������������
	CpopSelectLevel1 * m_CpopSelectLevel1;//ѡ��Ҫ�ƶ����Ľڵ����
	CDuiString m_modiftName; //Ҫ�޸ĵĽڵ�����
	CPopNewAttributeClass * m_CPopNewAttributeClass;//�½���������
	CPopDeleteAttClass * m_CPopDeleteAttClass;//ɾ����������
	CDuiString m_modiftName_2; //Ҫ�޸ĵĽڵ�����
	CDuiString m_modiftName_3; //Ҫ�޸ĵĽڵ�����
	string newName;//�޸ĺ������
	string newName_2;//�޸ĺ������
	string newName_3;//�޸ĺ������
	string newHoroscope;//�½�������������
	string newsatellite;//�½������ǵ�����
	string newTeleport;//�½��ĵ��Ÿ�����
	string newEarthStation;//�½��ĵ���վ����
	string newGroundTerminal;//�½��ĵ����ն˵�����

	bool RailTypeFlag;//���ڹ������
	bool FunctionalCategoryFlag;//���ܷ��� FunctionalCategory
	bool PayloadFlag;//�غ����ͷ���


};



