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
#include "DrawingInterfaceClass.h" //画图界面
#include "CPopNewAttributeClass.h"
#include "CPopDeleteAttClass.h"

#include "CallFmuClass.h"//调用FMU的函数
//网络管理界面
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
	void AddComboBox();//给tab3的com增加变量

	void CreateTreeNode(string tableName);//创建根节点------------tab1
	void CreateTreeNode_2(string tableName);//创建根节点------------tab2
	void CreateTreeNode_3(string tableName);//创建根节点------------tab3
	void CreateTreeNode(CDuiString name1, CDuiString time);//创建树节点
	void CreateTreeNode_3(int num,int node,string name);//创建卫星树节点
	void CreateTreeNode_livel1(string tableName,int num);//创建1级节点--------tab1
	void CreateTreeNode_livel1_2(string tableName, int num);//创建1级节点--------tab2
	void CreateTreeNode_livel1_3(string tableName, int num);//创建1级节点--------tab3

	void ModifyNodeName();
	void ModifyNodeName_2();
	void ModifyNodeName_3();
	void saveDataLocally(string noodName,map<string,string> m_map);
	void saveDataLocally_WX(string noodName, map<string, string> m_map);
	void saveDataLocally_DQZ(string noodName, map<string, string> m_map);
	void saveDataLocally_DMZD(string noodName, map<string, string> m_map);
	//void CreateTreeNode_3(int num, string name);//创建卫星树节点

	void Adddisplay(vector<string> vec);//界面中tab页
	void Adddisplay_2(vector<string> vec);//界面中tab2页
	void Adddisplay_3(vector<string> vec);//界面中tab2页
	void parseElements();//tab1界面筛选高轨卫星 中轨卫星 低轨卫星 和设备
	void parseElements_2();//筛选要显示的设备
	void parseElements_3();//筛选要显示的设备
	void DrawAPicture(vector<string> vec_line1, vector<string> vec_line2, vector<string> vec_line3, vector<string> vec_line4);
	void DrawAPicture_Tab2(vector<string> vec_line1);//界面2的绘图
	void DrawAPicture_Tab3(vector<string> vec_line1);//界面3的绘图
	void FindTheMap();//查找map
	void FindTheMap_2();//查找map tab2
	void FindTheMap_3();//查找map tab3
	void HideThCcontrol(int num);//隐藏控件
	void HideThCcontrol_2(int num,bool flag);//隐藏控件
	void HideThCcontrol_3(int num,bool flag);//隐藏控件tab3
	void HideThCcontrol_Draw(int num);//隐藏界面1画图控件
	void HideThCcontro2_Draw(int num);//隐藏界面2画图控件
	void HideThCcontro3_Draw(int num);//隐藏界面3画图控件

	void DisplayInformation(map<string, string> m_map);
	void DisplayInformation_2(map<string, string> m_map);//tab2右上角界面显示
	void DisplayInformation_3(map<string, string> m_map);//tab3右上角界面显示

	void UncheckedTab1();//将Tab1选中的树节点取消选中
	void UncheckedTab2();//将Tab2选中的树节点取消选中
	void UncheckedTab3();//将Tab3选中的树节点取消选中
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


	void GenerateTheFile(string path, string filename, map<string, string> m_map);//生成文件 星座、卫星
	void GenerateTheFile(string path, string filename, vector<vector<string>> vecVec);//生成星座和卫星对应的配置文件
	void CPopNetworkManage::GenerateTheFile(string path, string filename, map<string, string> m_map, string sign);

	map<string, string> GetCurrentEquipInform(string name);//获取tab1页当前设备信息
	map<string, string> GetCurrentEquipInform_tab2(string name);//获取tab2页当前设备信息
	map<string, string> GetCurrentEquipInform_tab3(string name);//获取tab2页当前设备信息

	void RealTimeSceneData_Tab1();//tab1 实时场景数据导入 现在是模拟数据
	void RealTimeSceneData_Tab2();//tab2 实时场景数据导入 现在是模拟数据
	void RealTimeSceneData_Tab3();//tab3 实时场景数据导入 现在是模拟数据

	void NotDrawable(string tabName,bool flag);//本地数据的话不可以绘图 把最下面那个所有控件设置成不可见

	void ImportingLocalData_Tab1();//将tab1界面的本地数据导入界面函数
	void ImportingLocalData_Tab2();
	void ImportingLocalData_Tab3();

	void RefreshTheInterface_tab1();//刷新界面 将数据置0
	void RefreshTheInterface_tab2();
	void RefreshTheInterface_tab3();

	vector<double> AssemblePackageFMU_tab1(vector<string> vec_WX, vector<string> vec_DQZ, vector<string> vec_DMZD);//组成调用fmu的包 tab1
	vector<double> AssemblePackageFMU_tab2(vector<string> vec_WX, vector<string> vec_DQZ, vector<string> vec_DMZD);//组成调用fmu的包 tab2
	vector<double> AssemblePackageFMU_tab3(vector<string> vec_WX, vector<string> vec_DQZ, vector<string> vec_DMZD);//组成调用fmu的包 tab3

	int getRootNodeOrder(string name);//tab1 获取该设备所在的根节点位置
	int getRootNodeOrder_2(string name);//tab2 获取该设备所在的根节点位置
	int getRootNodeOrder_3(string name);//tab2 获取该设备所在的根节点位置
	bool getWhetherDeviceStored(string name);//tab1 获取当前设备是否已经存储 是为之后双击设备显示信息做准备
	static UINT WINAPI UDPSendDataThread(void* pParam);//将数据使用UDP发送

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

	CComboBoxUI  * select_combox_1;//创建卫星界面的--选择运营商
	CComboBoxUI  * select_combox_2;//创建地球站界面的--选择运营商
	CComboBoxUI  * select_combox_3;//创建用户界面的--选择运营商

	CComboBoxUI  * select_BusinessType;//显示设置业务类型的控件
	CComboBoxUI  * select_BusinessType_1;//画图要选择的业务类型控件
	CListTextExtElementUI* pItem_File[1024];//依附于创建卫星界面的listEx每一行控件 
	CListTextExtElementUI* pItem_DQZH[1024];//依附于创建地球站界面的listEx每一行控件 
	CListTextExtElementUI* pItem_YH[1024];//依附于创建用户界面的listEX每一行控件

	string parameter_YYS[100];//放运营商的
	vector<string> vec_OperatorName;//存放运营商
	vector<string> vec_SatelliteName;//存放卫星名称
	vector<string> vec_EarthStation;//存放地球站名称
	vector<string> vec_UserName;//存放用户名称
	CTreeNodeUI* pNode_1[1024];//存放根节点
	CTreeNodeUI* pNode_2[1024];//存放1层节点 pNode_2[pNode_2_num]
	CTreeNodeUI* pNode_3[1024];//存放2层节点

	int pNode_1_num;//当前根节点在树中的顺序
	int pNode_2_num;//1层节点在树中的顺序
	int pNode_3_num;//2层节点在树中的顺序

	CTreeNodeUI* pNode_2_1[1024];//存放根节点
	CTreeNodeUI* pNode_2_2[1024];//存放1层节点 pNode_2[pNode_2_num]
	CTreeNodeUI* pNode_2_3[1024];//存放2层节点

	int pNode_2_1_num;//当前根节点在树中的顺序
	int pNode_2_2_num;//1层节点在树中的顺序
	int pNode_2_3_num;//2层节点在树中的顺序

	CTreeNodeUI* pNode_3_1[1024];//存放根节点
	CTreeNodeUI* pNode_3_2[1024];//存放1层节点 pNode_2[pNode_2_num]
	CTreeNodeUI* pNode_3_3[1024];//存放2层节点

	int pNode_3_1_num;//当前根节点在树中的顺序
	int pNode_3_2_num;//1层节点在树中的顺序
	int pNode_3_3_num;//2层节点在树中的顺序

	bool isLevel0_1;//第一个界面里面选择的是0级
	bool isLevel0_2;//第一个界面里面选择的是0级
	bool isLevel0_3;//第一个界面里面选择的是0级

	int selectNode[1][3];//存放选中的在树形结构中的位置 [0][0]存放选中的0级节点 [0][1]存放选中的1级节点 [0][2]存放选中的2级节点
	int selectNode_2[1][3];//存放选中的在树形结构中的位置 [0][0]存放选中的0级节点 [0][1]存放选中的1级节点 [0][2]存放选中的2级节点
	int selectNode_3[1][3];//存放选中的在树形结构中的位置 [0][0]存放选中的0级节点 [0][1]存放选中的1级节点 [0][2]存放选中的2级节点


	vector<string> vec_root_1;//第一个界面的树节点0级目录
	vector<string> vec_root_2;//第二个界面的树节点0级目录
	vector<string> vec_root_3;//第二个界面的树节点0级目录

	vector<string> vec_level2_1;//存储tab2的1级节点名称
	vector<map<string, string>> m_mapLevel2_1;//存放tab2一级节点其他属性

	vector<string> vec_level3_1;//存储tab2的1级节点名称
	vector<map<string, string>> m_mapLevel3_1;//存放tab2一级节点其他属性

	NetworkManagement2 m_NetworkManagement2;
	NetworkManagement m_NetworkManagement;//网络传输的结构体
	CTreeViewUI*	m_treeView;//界面上显示消息的控件
	CTreeViewUI*	m_treeView_2;//界面上显示消息的控件
	CTreeViewUI*	m_treeView_3;//界面上显示消息的控件

	CpopModifyInterface * m_CpopModifyInterface;
	CPopNewConstellation * m_CPopNewConstellation;//新建网络
	string InterfaceNetwork_1;
	CpopSelectEquipment * m_CpopSelectEquipment;

	DrawingInterfaceClass * m_DrawingInterfaceClass;//网络界面的画图界面
	CPopNewAttributeClass * m_CPopNewAttributeClass_Net;//网络界面新建设备属性
	CPopDeleteAttClass * m_CPopDeleteAttClass;//删除星座属性

	CDuiString m_modiftName_1;//要修改的树节点名称
	string newName_1;//修改后的树节点名称

	CDuiString m_modiftName_2;//要修改的树节点名称
	string newName_2;//修改后的树节点名称

	CDuiString m_modiftName_3;//要修改的树节点名称
	string newName_3;//修改后的树节点名称

	vector<string> vec_recEquipmentName;//接收到的设备名称 卫星、地球站、终端
	vector<map<string, string>> m_RecvMap;//存放所有设备包含的信息 卫星、地球站、终端

	vector<string> vec_recSatelliteName;//接收到的卫星名称
	vector<map<string, string>> m_RecvSatelliteMap;//存放卫星包含的信息

	vector<string> vec_recEarthStationName;//接收到的地球站名称
	vector<map<string, string>> m_RecvEarthStationMap;//存放所包含的地球站的信息

	vector<string> vec_recGroundTerminalName;//接收到的地面终端名称
	vector<map<string, string>> m_RecvGroundTerminalMap;//存放所有地面终端所包含的信息
	
	vector<string> vec_StartEndPointTab1;//tab1界面选择的当前网络中的所有卫星、地球站、地面终端
	vector<string> vec_SequenceDisplayTab1;//对tab1选择的卫星、地球站、地面终端按照给fmu组包中的顺序进行排序

	vector<string> vec_StartEndPointTab2;//tab2界面选择的当前网络中的所有卫星、地球站、地面终端
	vector<string> vec_SequenceDisplayTab2;//对tab2选择的卫星、地球站、地面终端按照给fmu组包中的顺序进行排序

	vector<string> vec_StartEndPointTab3;//tab3界面选择的当前网络中的所有卫星、地球站、地面终端
	vector<string> vec_SequenceDisplayTab3;//对tab3选择的卫星、地球站、地面终端按照给fmu组包中的顺序进行排序

	double intTime[6];
	double ts;

	vector<string> vec_Type;//存放所属机构类型
	ClueScreenClass * m_ClueScreenClass;
	UDPAdmin1* m_UDPAdmin1;

	map<string, string> m_Demo_Tab2;//存放tab2的二级节点所包含属性和默认值
	map<string, string> m_Demo_Tab3;//存放tab3的二级节点所包含属性和默认值

	string siftType;//tab2的选择的筛选属性
	string siftTypeValue;//tab2的选择的筛选属性的值

	string str_BusinessType;//选择要画图的业务类型
	double tab1Group[1818];	//调fmu的包存放的地方

	NetworkPackets m_NetworkPackets;//网络管理界面数据包(为曹工那边调用fmu使用)

	CallFmuClass * m_CallFmuClass;//调用fmu的函数

	string CurrentPath;//当前路径
};

