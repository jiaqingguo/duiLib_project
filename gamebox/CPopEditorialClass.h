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
//参数管理界面
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
	wstring stringToWstring(string str);//string转wstring
	string wstringToString(wstring wstr);//wstring转string

	void CreateTreeNode();//创建树节点
	void CreateTreeNode(string tableName);//创建根节点

	void CreateTreeNode_2(string tableName);//创建第二个界面的根节点
	void CreateTreeNode(string tableName,string str);//创建子节点

	void CreateTreeNode_3(string tableName);//创建第三个界面的根节点

	void CreateTreeNode_livel1(string tableName);//创建一级节点
	void CreateTreeNode_livel2(string tableName,int first_level);//创建二级节点
	void CreateTreeNode_livel3(string tableName, int first_level);//创建三级节点 firstlevel是指

	void CreateTreeNode_livel2_1(string tableName);//创建一级节点-----------电信港地球站界面
	void CreateTreeNode_livel2_2(string tableName, int first_level);//创建二级节点--------------电信港地球站界面
	void CreateTreeNode_livel2_3(string tableName, int first_level);//创建三级节点 firstlevel是指

	void CreateTreeNode_livel3_1(string tableName);//创建一级节点-----------地面终端
	void CreateTreeNode_livel3_2(string tableName,int first_level);//创建二级节点-----------地面终端子节点

	void GetNumberChild_0();//获取0级节点子节点的数量 也就是获取根节点的子节点数量
	void GetNumberChild();//获取一级节点子节点的数量
	void Movelevel_2(string level1, CDuiString level2);//移动卫星节点
	void Categorize_XZ_RailType();//轨道类别分类
	void Restore_XZ_RailType();//轨道分类后还原

	void Categorize_XZ_Functional();//功能类别分类
	void Restore_XZ_Functional();//功能类别分类后还原

	void Categorize_XZ_LoadType();//按照载荷类型分类
	void Restore_XZ_LoadType();//载荷类型分类后还原

	void  Restore_XZ_classification();//查看当前是否有星座在分类
	void  Restore_DXG_classification();//查看当前是否有电信港在分类


	void Categorize_DXG_characteristic();//电信港按性质分类
	void Restore_DXG_characteristic();//按照性质分类后还原

	void Categorize_WX_LoadFunction(string name);//卫星-----按照载荷功能分类
	void Restore_WX_LoadFunction(string name);//卫星------按载荷功能分类后还原

	void Categorize_DQZ_FunctionType1(string name);//地球站-----按照载荷功能分类
	void Restore_DQZ_FunctionType1(string name);//地球站------按载荷功能分类后还原

	void Categorize_DQZ_EarthStation(string name);//地球站-------按地球站类型分类
	void Restore_DQZ_EarthStation(string name);//地球站-------按地球站类型分类后还原

	void Categorize_DMZD_TerminalType();//电信港按性质分类
	void Restore_DMZD_TerminalType();//按照性质分类后还原

	void Movelevel_2_ToDXG(string level1, CDuiString level2);//移动地球站到某个电信港节点

	void RootNodeOnly1();//分类复原后只留根节点
	void EmptyChildNode_tab1();//清空子节点
	void EmptyChildNode_tab2();//清空子节点
	void EmptyChildNode_tab3();//清空子节点
	int GetNoodLevel(string nodeName);//获取轨道类别的节点类型所在顺序
	int GetNoodLevel_Root(string nodeName);//获取轨道类别的根节点所在顺序
	int GetNoodLevel_level2(string nodeName,int serialNumber);//获取轨道类别的节点类型所在顺序

	int GetNoodLevel_Root_DXG(string nodeName);//获电信港性质的根节点所在顺序
	int GetNoodLevel_level2_DQZ(string nodeName, int serialNumber);//获取地球站功能类型所在顺序
	void RootNodeOnly1_DXG();//分类复原后只留根节点

	int GetNoodLevel_Root_DMZD(string nodeName);//获地面终端类型的根节点所在顺序

	int GetNoodLevel_DXG_XZ(string nodeName);//获取电信港性质的节点类型所在顺序

	int GetNoodLevel_DMZD_ZDLX(string nodeName);//获取地面终端的终端类型的节点类型所在顺序
	void RootNodeOnly1_DMZD();//地面终端分类复原后只留根节点

	void AssignmentToLocalVector_XZ(string nodeName);//创建一级目录的树节点 --------星座
	void AssignmentToLocalVector_XZ(string nodeName,map<string,string> m_map);//创建一级目录的树节点 --------星座
	void AssignmentToLocalVector(string nodeName, map<string, string> m_map, int first_level);//创建二级目录的树节点 --------卫星
	void AssignmentToLocalVector_WXSX(string wx_name,string type,map<string, string> m_map);//创建三级目录的树节点 --------天线属性信息 载荷属性信息
	void AssignmentToLocalVector(string parent,string nodeName, map<string, string> m_map);//创建二级目录的树节点 --------卫星 导入的时候
	void AssignmentToLocalVector_TxZh(string parent, string nodeName, map<string, string> m_map);//创建三级目录的树节点 导入的时候--------天线属性信息 载荷属性信息

	void AssignmentToLocalVector_DXG(string nodeName);//创建一级目录的树节点 --------电信港
	void AssignmentToLocalVector_DXG(string nodeName, map<string, string> m_map);//导入的时候创建一级目录的树节点 --------电信港
	void AssignmentToLocalVector_DQZ(string nodeName, map<string, string> m_map, int first_level);//创建二级目录的树节点 --------地球站
	void AssignmentToLocalVector_DQZ(string wx_name, string type, map<string, string> m_map);//创建二级目录的树节点 --------地球站
	void AssignmentToLocalVector_DQZ_level2(string parent, string nodeName, map<string, string> m_map);//创建二级目录的树节点 --------地球站 导入的时候
	void AssignmentToLocalVector_DQZ_level3(string parent, string nodeName, map<string, string> m_map);//创建三级目录的树节点 导入的时候--------馈源波形天线

	void AssignmentToLocalVector_DMZD(string nodeName, map<string, string> m_map);//创建一级目录的树节点 --------地面终端
	void AssignmentToLocalVector_DMZD_level2(string parent,string type, map<string, string> m_map);//创建二级目录的树节点 --------地面终端子节点
	void AssignmentToLocalVector_DMZD_Child(string parent, string nodeName, map<string, string> m_map);//创建二级目录的树节点 --------载荷波形天线 导入的时候

	//void AssignmentToLocalVector_DMZD(string nodeName, map<string, string> m_map);//导入的时候创建一级目录的树节点 --------地面终端


	void ImportingLocalData_Tab1();//将tab1界面的本地数据导入界面函数
	void ImportingLocalData_Tab2();
	void ImportingLocalData_Tab3();
	void RefreshTheInterface_tab1();//刷新界面 将数据置0
	void RefreshTheInterface_tab2();
	void RefreshTheInterface_tab3();

	void DisplayInformation(map<string, string> m_map);//显示信息
	void DisplayInformation_2(map<string, string> m_map);//显示电信港地球站信息
	void DisplayInformation_3(map<string, string> m_map);//显示地面终端信息
	void FindTheMap();//查找map
	void FindTheMap_2();//查找map-------界面2
	void FindTheMap_3();//查找map-------界面3
	void FindTheMap_level3(CDuiString cdui_name);//双击3级目录
	void FindTheMap_level4(CDuiString cdui_name);//对星座里面的卫星进行筛选的时候 双击4级目录
	void FindTheMap_level3_2(CDuiString cdui_name);//第二个界面双击三级目录
	void FindTheMap_level4_2(CDuiString cdui_name);//对参数管理界面tab2 双击4级目录
	//void FindTheMap_RailType();//查找map
	map<string, string> GetHoroscopeInform();//从txt获取星座信息
	map<string, string> GetSatelliteInform();//从txt获取卫星信息

	map<string, string> GetSatelliteInform_1();//从txt获取卫星的天线属性信息
	map<string, string> GetSatelliteInform_2();//从txt获取卫星的载荷属性信息

	map<string, string> GetTeleport();//从txt获取电信港信息
	map<string, string> GetEarthStation();//从txt获取地球站信息
	map<string, string> GetEarthStation_1();//从txt获取地球站馈源属性信息 tab2
	map<string, string> GetEarthStation_2();//从txt获取地球站获取波形设备属性信息 tab2
	map<string, string> GetEarthStation_3();//从txt获取地球站天线属性信息 tab2

	map<string, string> GetGroundTerminal();//从txt获取地面终端的信息
	map<string, string> GetGroundTerminal_1();//从txt获取地面终端中载荷属性信息
	map<string, string> GetGroundTerminal_2();//从txt获取地面终端中波形设备属性信息
	map<string, string> GetGroundTerminal_3();//从txt获取地面终端中天线属性信息

	//void NodesContainData(string arrName,string arr,int num);//将数组里的数据名和数据传到成员变量中
	void HideThCcontrol(int num);//隐藏控件
	void HideThCcontrol_2(int num);//隐藏控件
	void HideThCcontrol_3(int num);//隐藏控件 -------界面3
	map<string,string> GetDataFromInterfaces();//将界面1的数据修改后存储成txt
	map<string,string> GetDataFromInterfaces_2();//将界面2的数据修改后存储成txt
	map<string,string> GetDataFromInterfaces_3();//将界面3的数据修改后存储成txt

	void UncheckedTab1();
	void UncheckedTab2();
	void UncheckedTab3();

	void GenerateTheFile(string path,string filename,map<string,string> m_map,string sign);//生成文件 星座、卫星
	void GenerateTheFile(string path, string filename, vector<vector<string>> vecVec);//生成星座和卫星对应的配置文件

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
	CListTextExtElementUI* pItem_File[1024];//依附于创建卫星界面的listEx每一行控件 
	CListTextExtElementUI* pItem_DQZH[1024];//依附于创建地球站界面的listEx每一行控件 
	CListTextExtElementUI* pItem_YH[1024];//依附于创建用户界面的listEX每一行控件

	void ModifyNodeName();//修改节点名称
	void ModifyNodeName_2();//修改节点名称 --电信港
	void ModifyNodeName_3();//修改节点名称 ---地面终端设备
	CTreeNodeUI* pNode_1[1024];//存放根节点
	CTreeNodeUI* pNode_2[1024];//存放二层节点 星座这一层 pNode_2[pNode_2_num]
	CTreeNodeUI* pNode_3[1024];//存放三层节点
	CTreeNodeUI* pNode_4[1024];//存放三层节点

	CTreeNodeUI* pNode_2_1[1024];//存放第二个界面的根节点
	CTreeNodeUI* pNode_2_2[1024];//存放第二个界面的1级节点
	CTreeNodeUI* pNode_2_3[1024];//存放第二个界面的2级节点
	CTreeNodeUI* pNode_2_4[1024];//存放第二个界面的2级节点

	CTreeNodeUI* pNode_3_1[1024];//存放第二个界面的根节点
	CTreeNodeUI* pNode_3_2[1024];//存放第二个界面的1级节点
	CTreeNodeUI* pNode_3_3[1024];//存放第二个界面的2级节点
	//CTreeNodeUI* pNode_4[1024];//存放四层节点
	CLabelUI* plabel[1024];//存放label

	int tab3RootType;//tab3界面存放类型的根节点个数

	int pNode_1_num;//当前根节点在树中的顺序
	int pNode_2_num;//一级节点在树中的顺序
	int pNode_3_num;//二级节点在树中的顺序
	int pNode_4_num;//三级节点在树中的顺序
	int pNode_5_num;//三级节点在树中的顺序

	int pNode_2_1_num;//当前根节点在树中的顺序
	int pNode_2_2_num;//一级节点在树中的顺序
	int pNode_2_3_num;//二级节点在树中的顺序
	int pNode_2_4_num;//三级节点在树中的顺序

	int pNode_3_1_num;//当前根节点在树中的顺序
	int pNode_3_2_num;//一级节点在树中的顺序
	int pNode_3_3_num;//二级节点在树中的顺序
	int pNode_3_4_num;//三级节点在树中的顺序


	vector<string> name_Level3;//存放三级节点--卫星相关天线属性和载荷属性名称
	vector<string> name_Level2;//存放二级节点--卫星名称
	vector<string> name_Level1;//存放一级节点--星座名称

	vector<bool> vec_ExpandFlags;//存放星座节点是否展开(已经筛选标志)
	vector<bool> vec_ExpandFlags2_FT;//存放电信港是否进行了功能类型筛选
	vector<bool> vec_ExpandFlags2_ES;//存放电信港是否进行了地球站类型筛选

	vector<string> name2_Level3;//存放三级节点--地球站下一层
	vector<string> name2_Level2;//存放二级节点--地球站
	vector<string> name2_Level1;//存放一级节点--电信港

	vector<string> name3_Level2;//存放二级节点--地面终端子节点名称
	vector<string> name3_Level1;//存放一级节点--地面终端

	vector<string> name_Level3_type;//存放卫星筛选的时候的节点

	int NumberChild[100];//每个一层节点的二层节点的个数
	int NumberChild_0[100];//每个0层节点的1层节点的个数
	vector<vector<string>> SecondLevelNode;//存储二层节点数据
	int CurrentIndex;//选中的树节点在二层节点的位置
	CTreeViewUI*	m_treeView;//显示星座卫星界面的树控件
	CTreeViewUI*	m_treeView_2;//显示电信港地球站界面的树控件
	CTreeViewUI*	m_treeView_3;//界面场景地面终端界面的树控件

	static unsigned __stdcall ThreadReadtle(void* arglist);
	vector<vector<string>> vecVec_Node1_1;//存放树中包含的数据
	string SelectedNode;
	
	int selectNode[1][4];//存放选中的在树形结构中的位置 [0][0]存放选中的0级节点 [0][1]存放选中的1级节点 [0][2]存放选中的2级节点
	int selectNode_2[1][4];//存放选中的在树形结构中的位置 [0][0]存放选中的0级节点 [0][1]存放选中的1级节点 [0][2]存放选中的2级节点
	int selectNode_3[1][3];//存放选中的在树形结构中的位置 [0][0]存放选中的0级节点 [0][1]存放选中的1级节点 [0][2]存放选中的2级节点
	int ModifyNode[1][3];//要修改的树节点的在树结构中的位置
	vector<string> vec_root;//存放根节点名称
	vector<string> vec_2_root;//存放第二个界面的根节点
	vector<string> vec_3_root;//存放第三个界面的根节点
	//map<string, string> map1_16;
	//map<string, string> map1_17;
	//map<string, string> map1_18;

	bool isLevel0;//选中的是0级节点那一层
	bool isLevel1;//选中的是1级节点那一层
	bool isLevel2;//选中的是2级节点那一层

	bool isLevel_2_0;//选中的是0级节点那一层
	bool isLevel_2_1;//选中的是1级节点那一层
	bool isLevel_2_2;//选中的是2级节点那一层

	bool isLevel_3_0;//选中的是0级节点那一层
	bool isLevel_3_1;//选中的是1级节点那一层



	vector<map<string, string>> m_RecvMap_XZ;//存放星座所包含的数据信息
	//vector<map<string, string>> m_RecvMap_XZ_RailType;//存放轨道类别的vector
	vector<map<string, string>> m_RecvMap;//存放卫星所包含的数据信息
	vector<map<string, string>> m_RecvMap_TxZh;//存放卫星相关的天线和载荷属性信息

	vector<map<string, string>> m_RecvMap_DQZChild;//存放地球站所包含的数据信息
	vector<map<string, string>> m_RecvMap_DQZ;//存放地球站所包含的数据信息
//vector<map<string, string>> m_RecvMap_XZ_RailType;//存放轨道类别的vector
	vector<map<string, string>> m_RecvMap_DXG;//存放电信港所包含的数据信息

	vector<map<string, string>> m_RecvMap_DMZDChild;//存放地面终端所包含的数据信息
	vector<map<string, string>> m_RecvMap_DMZD;//存放地面终端所包含的数据信息
	vector<pair<string, string>> ModifiedData;//修改后的数据

	CpopModifyInterface * m_CpopModifyInterface;
	CPopNewConstellation * m_CPopNewConstellation;//新建星座或者数据
	CpopSelectLevel1 * m_CpopSelectLevel1;//选择要移动到的节点界面
	CDuiString m_modiftName; //要修改的节点名称
	CPopNewAttributeClass * m_CPopNewAttributeClass;//新建星座属性
	CPopDeleteAttClass * m_CPopDeleteAttClass;//删除星座属性
	CDuiString m_modiftName_2; //要修改的节点名称
	CDuiString m_modiftName_3; //要修改的节点名称
	string newName;//修改后的名称
	string newName_2;//修改后的名称
	string newName_3;//修改后的名称
	string newHoroscope;//新建的星座的名称
	string newsatellite;//新建的卫星的名称
	string newTeleport;//新建的电信港名称
	string newEarthStation;//新建的地球站名称
	string newGroundTerminal;//新建的地面终端的名称

	bool RailTypeFlag;//正在轨道分类
	bool FunctionalCategoryFlag;//功能分类 FunctionalCategory
	bool PayloadFlag;//载荷类型分类


};



