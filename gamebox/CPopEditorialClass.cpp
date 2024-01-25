#include "stdafx.h"

#include <sstream>
#include <vector>
#include <string>
#include <map>
#include <io.h>
#include <direct.h>
#include <fstream>
#include <stdio.h>
#include <cassert>
#include <iostream>
#include <set>

#include "ConnectMysql.h"


#include "CPopEditorialClass.h"
#include "json.hpp"
#include "EnvironmentData.h"

int rootNum;//第一个根节点序号

int nodeNum;//二级节点
//FILE * lOG;
#define ControlNumber 102  //控件截止到 titles54 
bool boolArray[1024] = {0};//星座是否点了卫星排序
//vector<string> seleceColumns;
//参数管理界面类实现
DUI_BEGIN_MESSAGE_MAP(CPopEditorialClass, WindowImplBase)
DUI_ON_MSGTYPE(DUI_MSGTYPE_CLICK, OnClick)
DUI_ON_MSGTYPE(DUI_MSGTYPE_ITEMSELECT, OnItemSelect)
DUI_ON_MSGTYPE(DUI_MSGTYPE_SELECTCHANGED, OnSelectChanged)
DUI_END_MESSAGE_MAP()
CPopEditorialClass::CPopEditorialClass(void)
{
	m_pMenu = NULL;
}

CPopEditorialClass::~CPopEditorialClass(void)
{

	CMenuWnd::DestroyMenu();
	if (m_pMenu != NULL) {
		delete m_pMenu;
		m_pMenu = NULL;
	}


	
}
vector<string> CPopEditorialClass::my_split(string str, string pattern)
{
	string::size_type pos;
	vector<string> result;

	str += pattern;//扩展字符串以方便操作
	int size = str.size();

	for (int i = 0; i < size; i++) {
		pos = str.find(pattern, i);
		if (pos < size) {
			std::string s = str.substr(i, pos - i);
			result.push_back(s);
			i = pos + pattern.size() - 1;
		}
	}
	return result;
}
string UTF8_To_string(const std::string & str)
{
	int nwLen = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, NULL, 0);

	wchar_t * pwBuf = new wchar_t[nwLen + 1];//一定要加1，不然会出现尾巴  
	memset(pwBuf, 0, nwLen * 2 + 2);

	MultiByteToWideChar(CP_UTF8, 0, str.c_str(), str.length(), pwBuf, nwLen);

	int nLen = WideCharToMultiByte(CP_ACP, 0, pwBuf, -1, NULL, NULL, NULL, NULL);

	char * pBuf = new char[nLen + 1];
	memset(pBuf, 0, nLen + 1);

	WideCharToMultiByte(CP_ACP, 0, pwBuf, nwLen, pBuf, nLen, NULL, NULL);

	std::string retStr = pBuf;

	delete[]pBuf;
	delete[]pwBuf;

	pBuf = NULL;
	pwBuf = NULL;

	return retStr;

}
string string_To_UTF8(const std::string & str)
{
	int nwLen = ::MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, NULL, 0);

	wchar_t * pwBuf = new wchar_t[nwLen + 1];//一定要加1，不然会出现尾巴
	ZeroMemory(pwBuf, nwLen * 2 + 2);

	::MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.length(), pwBuf, nwLen);

	int nLen = ::WideCharToMultiByte(CP_UTF8, 0, pwBuf, -1, NULL, NULL, NULL, NULL);

	char * pBuf = new char[nLen + 1];
	ZeroMemory(pBuf, nLen + 1);

	::WideCharToMultiByte(CP_UTF8, 0, pwBuf, nwLen, pBuf, nLen, NULL, NULL);

	std::string retStr(pBuf);

	delete[]pwBuf;
	delete[]pBuf;

	pwBuf = NULL;
	pBuf = NULL;

	return retStr;
}
//
wstring CPopEditorialClass::stringToWstring(string str) 
{

	wstring w_str;
	UtilTool::setWstring(w_str, str.c_str());
	return w_str;
	
//string转wstring
}
//wstring转string
string CPopEditorialClass::wstringToString(wstring wstr)
{
	string str;
	UtilTool::setString(str, wstr.c_str());
	return str;
}
void CPopEditorialClass::creatTreeNodesOfXZ()
{
	//创建星座节点;
	for (const auto& stData : EnvironmentData::m_mapStXZSJB)
	{
		auto &st = stData.second;
		// 给根节点添加子节点
		//CTreeNodeUI* pChildNode = new CTreeNodeUI();
		//UtilTool::setWstring(text_string, st.constellationName.c_str());
		//pChildNode->SetItemText(text_string.c_str());
		//CDuiString strID;
		//strID.Format(_T("%d"),m_iTreeNodeID); // 将int转换为字符串
		//pChildNode->SetUserData(strID);
		//setTreeNodeStyleSheet(pChildNode);
		//pNode_1[0]->AddChildNode(pChildNode);
		//pNode_1[0]->add

		auto newTreeNode = addChildNode(pNode_1[0], st.constellationName);
		
		// 创建子节点
		createTreeNodesOfDX(newTreeNode, st.id);
	}
}

// 创建星座对应的单星子节点
void CPopEditorialClass::createTreeNodesOfDX(CTreeNodeUI* pTreeNodeXZ, const int& sqlID)
{
	// 获取 对应星座下的单星数据;
	std::map<int, stDXSJB>	mapStXZSJBofXZ;
	EnvironmentData::m_iCurSchemeID = 1;
	ConnectMysql::Instance().getDxsjbBySchemeIDAndXzID(mapStXZSJBofXZ, EnvironmentData::m_iCurSchemeID, sqlID);

	for (const auto& stData : mapStXZSJBofXZ)
	{
		auto& st = stData.second;
		addChildNode(pTreeNodeXZ, st.satName);
	}


}

CTreeNodeUI* CPopEditorialClass::addChildNode(CTreeNodeUI* pPrentTreeNode, const std::string strText)
{
	CTreeNodeUI* pChildNode = new CTreeNodeUI();
	wstring text_string;
	UtilTool::setWstring(text_string, strText.c_str());
	pChildNode->SetItemText(text_string.c_str());
	CDuiString strID;
	strID.Format(_T("%d"), m_iTreeNodeID); // 将int转换为字符串
	pChildNode->SetUserData(strID);
	setTreeNodeStyleSheet(pChildNode);
	pPrentTreeNode->AddChildNode(pChildNode);
    m_iTreeNodeID++;
	return pChildNode;
}

void CPopEditorialClass::setTreeNodeStyleSheet(CTreeNodeUI* pTreeNode)
{
	pTreeNode->CheckBoxSelected(false);
	//pTreeNode->SetItemText(stringToWstring(tableName).c_str());
	pTreeNode->SetFixedHeight(30);
	pTreeNode->SetItemTextColor(0xFFFFFFFF);
	pTreeNode->SetItemHotTextColor(0xFFFFFFFF);
	pTreeNode->SetSelItemTextColor(0xFFFFFFFF);
	pTreeNode->SetAttribute(_T("folderattr"), _T("padding=&quot;0,1,0,0&quot; width=&quot;16&quot; height=&quot;16&quot; normalimage=&quot;file='ui/展开.png' &quot; "));
	pTreeNode->SetAttribute(_T("folderattr"), _T("hotimage=&quot;file='ui/展开.png' &quot; selectedimage=&quot;file='ui/未展开.png'&quot; selectedhotimage=&quot;file='ui/未展开.png'&quot;"));
	pTreeNode->SetAttribute(_T("itemattr"), _T("padding=\"16,0,0,0\" align=\"left\" font=\"13\""));//font=&quot;6&quot;
	pTreeNode->SetName(_T("WX1"));

}
void CPopEditorialClass::removeAllNodes(CTreeNodeUI* pNode)
{
	if (pNode == NULL) return;

	// 获取子节点的数量
	int nCount = pNode->GetCount();
	for (int i = nCount - 1; i >= 0; --i)
	{
		// 获取子节点
		CControlUI* pSubItem = pNode->GetItemAt(i);
		CTreeNodeUI* pSubNode = static_cast<CTreeNodeUI*>(pSubItem);
		if (pSubNode != NULL)
		{
			// 递归删除子节点
			removeAllNodes(pSubNode);
		}
	}

	// 现在所有子节点都被删除了，我们可以安全地删除当前节点
	CTreeNodeUI* pParentNode = pNode->GetParentNode();
	if (pParentNode != NULL)
	{
		pParentNode->Remove(pNode); // 从父节点移除当前节点
	}
	else
	{
		// 如果这是根节点，则直接删除
		CTreeViewUI* pTreeView = pNode->GetTreeView();
		if (pTreeView != NULL)
		{
			pTreeView->Remove(pNode); // 从TreeView移除当前节点
		}
	}
	delete pNode; // 删除节点对象
}
void CPopEditorialClass::OnFinalMessage(HWND hWnd)
{
	__super::OnFinalMessage(hWnd);
	delete this;
}
DWORD StringToDword(string val)
{
	DWORD cur_dword;
	sscanf(val.c_str(), "%ul", &cur_dword);
	return cur_dword;
}

DuiLib::CDuiString CPopEditorialClass::GetSkinFile()
{
	return _T("CPopEditorial.xml");
}

LPCTSTR CPopEditorialClass::GetWindowClassName(void) const
{
	return _T("NetworkManagement");
}

void CPopEditorialClass::OnClick(TNotifyUI &msg)
{
	CDuiString sName = msg.pSender->GetName();
	sName.MakeLower();
	if (msg.pSender == m_pCloseBtn) {
		Close(0);
		return;
	}
	else if (sName.CompareNoCase(_T("bt_save")) == 0)////保存Tab1 bt_save
	{

		map<string, string> map_inform;//界面上修改完的map
		map_inform = GetDataFromInterfaces();

		string FileName = wstringToString(m_modiftName.GetData());
		for (int i = 0; i < name_Level1.size(); i++)
		{
			if (name_Level1[i] == FileName)
			{
				m_RecvMap_XZ[i] = map_inform;
			}
		}
		for (int i = 0; i < name_Level2.size(); i++)
		{
			if (name_Level2[i] == FileName)
			{
				m_RecvMap[i] = map_inform;
			}
		}
		for (int i = 0; i < name_Level3.size(); i++)
		{
			if (name_Level3[i] == FileName)
			{
				m_RecvMap_TxZh[i] = map_inform;
			}
		}
		// 为了创建表，字段新增的逻辑;
		for (int i = 0; i < m_vecTxName.size(); i++)
		{
			if (m_vecTxName[i] == FileName)
			{
				m_vecMapTxData[i] = map_inform;
			}
		}
		for (int i = 0; i < m_vecZhName.size(); i++)
		{
			if (m_vecZhName[i] == FileName)
			{
				m_vecMapZhData[i] = map_inform;
			}
		}

	}
	else if (sName.CompareNoCase(_T("bt_save2")) == 0)////保存tab2 bt_save
	{

		map<string, string> map_inform;//界面上修改完的map
		map_inform = GetDataFromInterfaces_2();

		string FileName = wstringToString(m_modiftName_2.GetData());
		for (int i = 0; i < name2_Level1.size(); i++)
		{
			if (name2_Level1[i] == FileName)
			{
				m_RecvMap_DXG[i] = map_inform;
			}
		}

		for (int i = 0; i < name2_Level2.size(); i++)
		{
			if (name2_Level2[i] == FileName)
			{
				m_RecvMap_DQZ[i] = map_inform;
			}
		}
		for (int i = 0; i < name2_Level3.size(); i++)
		{
			if (name2_Level3[i] == FileName)
			{
				m_RecvMap_DQZChild[i] = map_inform;
			}
		}
		// 为了创建表，字段新增的逻辑;
		for (int i = 0; i < m_vecDqzBxName.size(); i++)
		{
			if (m_vecDqzBxName[i] == FileName)
			{
				m_vecMapDqzBxData[i]=map_inform ;
			}
		}
		for (int i = 0; i < m_vecDqzkyName.size(); i++)
		{
			if (m_vecDqzkyName[i] == FileName)
			{
				m_vecMapDqzKyData[i]=map_inform;
			}
		}
		for (int i = 0; i < m_vecDqzTxName.size(); i++)
		{
			if (m_vecDqzTxName[i] == FileName)
			{
				m_vecMapDqzTxData[i]=map_inform;
			}
		}

	}
	else if (sName.CompareNoCase(_T("bt_SaveScheme1")) == 0)  // 保存到数据库;
	{
		
		m_CPopNewScheme->MessageBox(NULL);//
		string strScheme = m_CPopNewScheme->GetNewName();//获取新增加的方案名称;
		if (strScheme.size() > 0)
		{
			saveDatabaseScheme(strScheme);
		}	
	}
	else if (sName.CompareNoCase(_T("bt_SaveScheme2")) == 0)  // 保存到数据库;
	{
		m_CPopNewScheme->MessageBox(NULL);//
		string strScheme = m_CPopNewScheme->GetNewName();//获取新增加的方案名称;
		if (strScheme.size() > 0)
		{
			saveDatabaseScheme(strScheme);
		}	
	}
	else if (sName.CompareNoCase(_T("bt_SaveScheme3")) == 0)  // 保存到数据库;
	{
		m_CPopNewScheme->MessageBox(NULL);//
		string strScheme = m_CPopNewScheme->GetNewName();//获取新增加的方案名称;
		if (strScheme.size() > 0)
		{
			saveDatabaseScheme(strScheme);
		}
	}
	else if (sName.CompareNoCase(_T("bt_AddTo")) == 0)//增加按钮 星座、卫星、卫星下的天线和载荷属性
	{
		m_CPopNewAttributeClass->MessageBox(NULL);//m_CPopNewAttributeClass

		vector<string> vec = m_CPopNewAttributeClass->GetNewName();//获取新增加的
		if (vec.size() > 0)
		{
			map<string, string> map_inform;//界面上修改完的map
			string FileName = wstringToString(m_modiftName.GetData());
			int sequese;
			for (int i = 0; i < name_Level1.size(); i++)//给星座层级添加属性
			{
				if (name_Level1[i] == FileName)
				{
					map_inform = m_RecvMap_XZ[i];//m_RecvMap_XZ[i] = map_inform;
					sequese = i;

					map_inform[string_To_UTF8(vec[0])] = string_To_UTF8(vec[1]);
					m_RecvMap_XZ[sequese] = map_inform;
					DisplayInformation(m_RecvMap_XZ[sequese]);
				}
			}


			for (int i = 0; i < name_Level2.size(); i++)//给卫星层级添加属性
			{
				if (name_Level2[i] == FileName)
				{
					map_inform = m_RecvMap[i];//m_RecvMap_XZ[i] = map_inform;
					sequese = i;

					map_inform[string_To_UTF8(vec[0])] = string_To_UTF8(vec[1]);
					m_RecvMap[sequese] = map_inform;
					DisplayInformation(m_RecvMap[sequese]);
				}
			}

			for (int i = 0; i < name_Level3.size(); i++)
			{
				if (name_Level3[i] == FileName)
				{
					map_inform = m_RecvMap_TxZh[i];//m_RecvMap_XZ[i] = map_inform;
					sequese = i;

					map_inform[string_To_UTF8(vec[0])] = string_To_UTF8(vec[1]);
					m_RecvMap_TxZh[sequese] = map_inform;
					DisplayInformation(m_RecvMap_TxZh[sequese]);
				}
			}
			//map_inform[string_To_UTF8(vec[0])] = string_To_UTF8(vec[1]);
			//m_RecvMap_XZ[sequese] = map_inform;
			//DisplayInformation(m_RecvMap_XZ[sequese]);

			// 为了创建表，字段新增的逻辑;
			for (int i = 0; i < m_vecTxName.size(); i++)
			{
				if (m_vecTxName[i] == FileName)
				{
					map_inform = m_vecMapZhData[i];//m_RecvMap_XZ[i] = map_inform;
					sequese = i;

					map_inform[string_To_UTF8(vec[0])] = string_To_UTF8(vec[1]);
					m_vecMapZhData[sequese] = map_inform;
					
				}
			}
			for (int i = 0; i < m_vecZhName.size(); i++)
			{
				if (m_vecZhName[i] == FileName)
				{
					map_inform = m_vecMapZhData[i];//m_RecvMap_XZ[i] = map_inform;
					sequese = i;

					map_inform[string_To_UTF8(vec[0])] = string_To_UTF8(vec[1]);
					m_vecMapZhData[sequese] = map_inform;

				}
			}
			
			
		}
	}
	else if (sName.CompareNoCase(_T("bt_Delete")) == 0)//删除按钮 星座、卫星、卫星下的天线和载荷属性
	{
		vector<string> vec_name;
		int flag = -1;
		map<string, string> map_inform;//界面上修改完的map
		string FileName = wstringToString(m_modiftName.GetData());
		int sequese;
		for (int i = 0; i < name_Level1.size(); i++)
		{
			if (name_Level1[i] == FileName)
			{
				map_inform = m_RecvMap_XZ[i];//m_RecvMap_XZ[i] = map_inform;
				sequese = i;
				flag = 0;

			}
		}
		for (int i = 0; i < name_Level2.size(); i++)
		{
			if (name_Level2[i] == FileName)
			{
				map_inform = m_RecvMap[i];//m_RecvMap_XZ[i] = map_inform;
				sequese = i;
				flag = 1;
			}
		}

		for (int i = 0; i < name_Level3.size(); i++)
		{
			if (name_Level3[i] == FileName)
			{
				map_inform = m_RecvMap_TxZh[i];//m_RecvMap_XZ[i] = map_inform;
				sequese = i;

				flag = 2;
			}
		}



		for (map<string, string>::iterator iter = map_inform.begin(); iter != map_inform.end(); ++iter)
		{
			cout << "key:" << iter->first << " value:" << iter->second << endl;
			vec_name.push_back(iter->first);
		}
		m_CPopDeleteAttClass->MessageBox(NULL, vec_name);//m_CPopNewAttributeClass

		if (flag == 0)//星座
		{
			map<string, string> map_inform1 = m_RecvMap_XZ[sequese];
			vector<string> vec_deleteName = m_CPopDeleteAttClass->GetDeleteName();
			for (map<string, string>::iterator iter = map_inform.begin(); iter != map_inform.end(); ++iter)
			{
				cout << "key:" << iter->first << " value:" << iter->second << endl;
				for (int i = 0; i < vec_deleteName.size(); i++)
				{
					if (iter->first == vec_deleteName[i])
					{
						//cout << "选择" << endl;
						map_inform1.erase(iter->first);
					}

				}
			}

			m_RecvMap_XZ[sequese] = map_inform1;
			DisplayInformation(m_RecvMap_XZ[sequese]);
		}
		else if (flag == 1)//卫星
		{
			map<string, string> map_inform1 = m_RecvMap[sequese];
			vector<string> vec_deleteName = m_CPopDeleteAttClass->GetDeleteName();
			for (map<string, string>::iterator iter = map_inform.begin(); iter != map_inform.end(); ++iter)
			{
				cout << "key:" << iter->first << " value:" << iter->second << endl;
				for (int i = 0; i < vec_deleteName.size(); i++)
				{
					if (iter->first == vec_deleteName[i])
					{
						//cout << "选择" << endl;
						map_inform1.erase(iter->first);
					}

				}
			}

			m_RecvMap[sequese] = map_inform1;
			DisplayInformation(m_RecvMap[sequese]);
		}
		else if (flag == 2)//卫星的天线和载荷
		{
			map<string, string> map_inform1 = m_RecvMap_TxZh[sequese];
			vector<string> vec_deleteName = m_CPopDeleteAttClass->GetDeleteName();
			for (map<string, string>::iterator iter = map_inform.begin(); iter != map_inform.end(); ++iter)
			{
				cout << "key:" << iter->first << " value:" << iter->second << endl;
				for (int i = 0; i < vec_deleteName.size(); i++)
				{
					if (iter->first == vec_deleteName[i])
					{
						//cout << "选择" << endl;
						map_inform1.erase(iter->first);
					}

				}
			}

			m_RecvMap_TxZh[sequese] = map_inform1;
			DisplayInformation(m_RecvMap_TxZh[sequese]);

			
		}


		// 创建表及字段添加的逻辑; // 删除;
		vector<string> vecDeleteName = m_CPopDeleteAttClass->GetDeleteName();
		for (int i = 0; i < m_vecTxName.size(); i++)
		{
			if (m_vecTxName[i] == FileName)
			{
				map<string, string> &mapData = m_vecMapTxData[i];//m_RecvMap_XZ[i] = map_inform;
				
				for (map<string, string>::iterator iter = mapData.begin(); iter != mapData.end();)
				{
					bool erased = false;
					for (int i = 0; i < vecDeleteName.size(); i++)
					{
						if (iter->first == vecDeleteName[i])
						{
							//cout << "选择" << endl;
							 iter = mapData.erase(iter);
							 erased = true;
						}

					}
					if (!erased)
					{
						++iter;
					}
				}
				break;
				
			}
		}

		for (int i = 0; i < m_vecZhName.size(); i++)
		{
			if (m_vecZhName[i] == FileName)
			{
				map<string, string>& mapData = m_vecMapZhData[i];//m_RecvMap_XZ[i] = map_inform;
		
				for (map<string, string>::iterator iter = mapData.begin(); iter != mapData.end(); ++iter)
				{
					cout << "key:" << iter->first << " value:" << iter->second << endl;
					bool erased = false;
					for (int i = 0; i < vecDeleteName.size(); i++)
					{
						if (iter->first == vecDeleteName[i])
						{
							//cout << "选择" << endl;
							iter=mapData.erase(iter);
							erased = true;
						}

					}
					if (!erased)
					{
						++iter; 
					}
				}
				break;
			}
			
		}
	}
	else if (sName.CompareNoCase(_T("bt_Tree1_Import")) == 0)//导入
	{
		RefreshTheInterface_tab1();
		HideThCcontrol(ControlNumber);

		COptionUI* pControl = static_cast<COptionUI*>(m_pm.FindControl(_T("optical_ScenarioData")));//场景数据
		if (pControl->IsSelected())
		{
			cout << "tab1场景数据" << endl;
			//RefreshTheInterface_tab1();
		}
		COptionUI* pControl_1 = static_cast<COptionUI*>(m_pm.FindControl(_T("optical_LocalData")));//本地数据
		if (pControl_1->IsSelected())
		{

			ImportingLocalData_Tab1();//导入本地数据
			// 加载数据库数据 创建星座等节点;
			//creatTreeNodesOfXZ();
		}



	}
	else if (sName.CompareNoCase(_T("bt_Tree1_SaveAs")) == 0)//另存为
	{
		string path;
		path = "D:\\DATA1\\";
		//bool flag = CreateDirectory(_T("D:\\DATA1\\HH"), NULL);
		m_CPopNewConstellation->MessageBox(NULL);
		string name = m_CPopNewConstellation->GetNewName();//	string GetNewName();//获取修改后的名称
		path += name;
		bool flag = CreateDirectory(stringToWstring(path).c_str(), NULL);
		vector< vector<string>> vec_XZAndWX;
		vector< vector<string>> vec_WXAndChild;
		/*******************将星座的数据存储*****************/
		for (int i = 0; i < name_Level1.size(); i++)
		{
			GenerateTheFile(path, name_Level1[i], m_RecvMap_XZ[i], "星座");

		}
		/*******************将卫星的数据存储********************/
		for (int i = 0; i < name_Level2.size(); i++)
		{
			GenerateTheFile(path, name_Level2[i], m_RecvMap[i], "卫星");
		}
		/*******************将卫星的天线和载荷数据存储**********************************/
		for (int i = 0; i < name_Level3.size(); i++)
		{
			GenerateTheFile(path, name_Level3[i], m_RecvMap_TxZh[i], "天线或载荷");
		}
		/********************将星座对应卫星关系存储********************/
		for (int i = 0; i < pNode_2_num; i++)
		{
			vector<string> vec;
			int childNum = pNode_2[i]->GetCountChild();
			vec.push_back(wstringToString(pNode_2[i]->GetItemText().GetData()));
			for (int j = 0; j < childNum; j++)
			{

				CTreeNodeUI*  node = pNode_2[i]->GetChildNode(j);//GetParentNode
				string childName = wstringToString(node->GetItemText().GetData());
				vec.push_back(childName);
			}
			vec_XZAndWX.push_back(vec);
		}
		GenerateTheFile(path, UTF8_To_string("配置文件_星座"), vec_XZAndWX);

		/**********************将卫星和其子节点关系存储******************************/
		for (int i = 0; i < pNode_3_num; i++)
		{
			vector<string> vec;
			int childNum = pNode_3[i]->GetCountChild();
			vec.push_back(wstringToString(pNode_3[i]->GetItemText().GetData()));
			for (int j = 0; j < childNum; j++)
			{

				CTreeNodeUI*  node = pNode_3[i]->GetChildNode(j);//GetParentNode
				string childName = wstringToString(node->GetItemText().GetData());
				vec.push_back(childName);
			}
			vec_WXAndChild.push_back(vec);
		}
		GenerateTheFile(path, UTF8_To_string("配置文件_卫星"), vec_WXAndChild);
	}
	else if (sName.CompareNoCase(_T("bt_AddTo2")) == 0)//增加 电信港属性 Tab2
	{
		m_CPopNewAttributeClass->MessageBox(NULL);//m_CPopNewAttributeClass

		vector<string> vec = m_CPopNewAttributeClass->GetNewName();//获取新增加的
		if (vec.size() > 0)
		{
			map<string, string> map_inform;//界面上修改完的map
			string FileName = wstringToString(m_modiftName_2.GetData());
			int sequese;
			for (int i = 0; i < name2_Level1.size(); i++)//给电信港增加属性
			{
				if (name2_Level1[i] == FileName)
				{
					map_inform = m_RecvMap_DXG[i];//m_RecvMap_XZ[i] = map_inform;
					sequese = i;

					map_inform[string_To_UTF8(vec[0])] = string_To_UTF8(vec[1]);
					m_RecvMap_DXG[sequese] = map_inform;
					DisplayInformation_2(m_RecvMap_DXG[sequese]);
				}
			}



			for (int i = 0; i < name2_Level2.size(); i++)//给地球站添加属性
			{
				if (name2_Level2[i] == FileName)
				{
					map_inform = m_RecvMap_DQZ[i];//m_RecvMap_XZ[i] = map_inform;
					sequese = i;

					map_inform[string_To_UTF8(vec[0])] = string_To_UTF8(vec[1]);
					m_RecvMap_DQZ[sequese] = map_inform;
					DisplayInformation_2(m_RecvMap_DQZ[sequese]);
				}
			}

			for (int i = 0; i < name2_Level3.size(); i++)
			{
				if (name2_Level3[i] == FileName)
				{
					map_inform = m_RecvMap_DQZChild[i];//m_RecvMap_XZ[i] = map_inform;
					sequese = i;

					map_inform[string_To_UTF8(vec[0])] = string_To_UTF8(vec[1]);
					m_RecvMap_DQZChild[sequese] = map_inform;
					DisplayInformation_2(m_RecvMap_DQZChild[sequese]);
				}
			}
			// 为了创建表，字段新增的逻辑;
			for (int i = 0; i < m_vecDqzBxName.size(); i++)
			{
				if (m_vecDqzBxName[i] == FileName)
				{
					map_inform = m_vecMapDqzBxData[i];
					sequese = i;

					map_inform[string_To_UTF8(vec[0])] = string_To_UTF8(vec[1]);
					m_vecMapDqzBxData[sequese] = map_inform;

				}
			}
			for (int i = 0; i < m_vecDqzkyName.size(); i++)
			{
				if (m_vecDqzkyName[i] == FileName)
				{
					map_inform = m_vecMapDqzKyData[i];
					sequese = i;

					map_inform[string_To_UTF8(vec[0])] = string_To_UTF8(vec[1]);
					m_vecMapDqzKyData[sequese] = map_inform;

				}
			}
			for (int i = 0; i < m_vecDqzTxName.size(); i++)
			{
				if (m_vecDqzTxName[i] == FileName)
				{
					map_inform = m_vecMapDqzTxData[i];
					sequese = i;

					map_inform[string_To_UTF8(vec[0])] = string_To_UTF8(vec[1]);
					m_vecMapDqzTxData[sequese] = map_inform;

				}
			}

		}
	}
	else if (sName.CompareNoCase(_T("bt_Delete2")) == 0)//删除电信港属性 tab2
	{
		vector<string> vec_name;
		int flag = -1;
		map<string, string> map_inform;//界面上修改完的map
		string FileName = wstringToString(m_modiftName_2.GetData());
		int sequese;
		for (int i = 0; i < name2_Level1.size(); i++)
		{
			if (name2_Level1[i] == FileName)
			{
				map_inform = m_RecvMap_DXG[i];//m_RecvMap_XZ[i] = map_inform;
				sequese = i;
				flag = 0;

			}
		}
		for (int i = 0; i < name2_Level2.size(); i++)
		{
			if (name2_Level2[i] == FileName)
			{
				map_inform = m_RecvMap_DQZ[i];//m_RecvMap_XZ[i] = map_inform;
				sequese = i;
				flag = 1;
			}
		}
		for (int i = 0; i < name2_Level3.size(); i++)
		{
			if (name2_Level3[i] == FileName)
			{
				map_inform = m_RecvMap_DQZChild[i];//m_RecvMap_XZ[i] = map_inform;
				sequese = i;
				flag = 2;
			}
		}


		for (map<string, string>::iterator iter = map_inform.begin(); iter != map_inform.end(); ++iter)
		{
			cout << "key:" << iter->first << " value:" << iter->second << endl;
			vec_name.push_back(iter->first);
		}
		m_CPopDeleteAttClass->MessageBox(NULL, vec_name);//m_CPopNewAttributeClass

		if (flag == 0)//电信港
		{
			map<string, string> map_inform1 = m_RecvMap_DXG[sequese];
			vector<string> vec_deleteName = m_CPopDeleteAttClass->GetDeleteName();
			for (map<string, string>::iterator iter = map_inform.begin(); iter != map_inform.end(); ++iter)
			{
				cout << "key:" << iter->first << " value:" << iter->second << endl;
				for (int i = 0; i < vec_deleteName.size(); i++)
				{
					if (iter->first == vec_deleteName[i])
					{
						//cout << "选择" << endl;
						map_inform1.erase(iter->first);
					}

				}
			}

			m_RecvMap_DXG[sequese] = map_inform1;
			DisplayInformation_2(m_RecvMap_DXG[sequese]);
		}
		else if (flag == 1)//地球站
		{
			map<string, string> map_inform1 = m_RecvMap_DQZ[sequese];
			vector<string> vec_deleteName = m_CPopDeleteAttClass->GetDeleteName();
			for (map<string, string>::iterator iter = map_inform.begin(); iter != map_inform.end(); ++iter)
			{
				cout << "key:" << iter->first << " value:" << iter->second << endl;
				for (int i = 0; i < vec_deleteName.size(); i++)
				{
					if (iter->first == vec_deleteName[i])
					{
						//cout << "选择" << endl;
						map_inform1.erase(iter->first);
					}

				}
			}

			m_RecvMap_DQZ[sequese] = map_inform1;
			DisplayInformation_2(m_RecvMap_DQZ[sequese]);
		}
		else if (flag == 2)//地面终端
		{
			map<string, string> map_inform1 = m_RecvMap_DQZChild[sequese];
			vector<string> vec_deleteName = m_CPopDeleteAttClass->GetDeleteName();
			for (map<string, string>::iterator iter = map_inform.begin(); iter != map_inform.end(); ++iter)
			{
				cout << "key:" << iter->first << " value:" << iter->second << endl;
				for (int i = 0; i < vec_deleteName.size(); i++)
				{
					if (iter->first == vec_deleteName[i])
					{
						//cout << "选择" << endl;
						map_inform1.erase(iter->first);
					}

				}
			}

			m_RecvMap_DQZChild[sequese] = map_inform1;
			DisplayInformation_2(m_RecvMap_DQZChild[sequese]);
		}

		// 创建表及字段添加的逻辑; // 删除;
		const vector<string> vecDeleteName = m_CPopDeleteAttClass->GetDeleteName();
		for (int i = 0; i < m_vecDqzkyName.size(); i++)
		{
			if (m_vecDqzkyName[i] == FileName)
			{
				map<string, string>& mapData = m_vecMapDqzKyData[i];//m_RecvMap_XZ[i] = map_inform;
				for (map<string, string>::iterator iter = mapData.begin(); iter != mapData.end();)
				{
					bool erased = false;
					for (int i = 0; i < vecDeleteName.size(); i++)
					{
						if (iter->first == vecDeleteName[i])
						{
							//cout << "选择" << endl;
							iter=mapData.erase(iter);
							erased = true;
						}

					}
					if (!erased)
					{
						++iter;
					}
				}
				break;
			}
		}

		for (int i = 0; i < m_vecDqzBxName.size(); i++)
		{
			if (m_vecDqzBxName[i] == FileName)
			{
				map<string, string>& mapData = m_vecMapDqzBxData[i];//m_RecvMap_XZ[i] = map_inform;
				for (map<string, string>::iterator iter = mapData.begin(); iter != mapData.end();)
				{
					bool erased = false;
					for (int i = 0; i < vecDeleteName.size(); i++)
					{
						if (iter->first == vecDeleteName[i])
						{
							//cout << "选择" << endl;
							iter = mapData.erase(iter);
							erased = true;
						}

					}
					if (!erased)
					{
						++iter;
					}
				}
				break;
			}
		}
		// 地球站 天线
		for (int i = 0; i < m_vecDqzTxName.size(); i++)
		{
			if (m_vecDqzTxName[i] == FileName)
			{
				map<string, string>& mapData = m_vecMapDqzTxData[i];//m_RecvMap_XZ[i] = map_inform;
				//sequese = i;
				for (map<string, string>::iterator iter = mapData.begin(); iter != mapData.end();)
				{
					bool erased = false;
					for (int i = 0; i < vecDeleteName.size(); i++)
					{
						if (iter->first == vecDeleteName[i])
						{
							//cout << "选择" << endl;
							iter = mapData.erase(iter);
							erased = true;
						}

					}
					if (!erased)
					{
						++iter;
					}
				}
				break;
			}
		}

	}
	else if (sName.CompareNoCase(_T("bt_Tree1_Import2")) == 0)//导入
	{
		RefreshTheInterface_tab2();
		HideThCcontrol_2(ControlNumber);
		COptionUI* pControl = static_cast<COptionUI*>(m_pm.FindControl(_T("optical_ScenarioData")));//场景数据
		if (pControl->IsSelected())
		{
			cout << "tab2场景数据" << endl;

			
		}
		COptionUI* pControl_1 = static_cast<COptionUI*>(m_pm.FindControl(_T("optical_LocalData2")));//本地数据
		if (pControl_1->IsSelected())
		{

			ImportingLocalData_Tab2();//导入本地数据
		}
		//ImportingLocalData_Tab2();


	}
	else if (sName.CompareNoCase(_T("bt_Tree1_SaveAs2")) == 0)//tab2 另存为
	{
		vector<vector<string>> vec_WXAndChild;
		string path;
		path = "D:\\DATA1\\";
		//bool flag = CreateDirectory(_T("D:\\DATA1\\HH"), NULL);
		m_CPopNewConstellation->MessageBox(NULL);
		string name = m_CPopNewConstellation->GetNewName();//	string GetNewName();//获取修改后的名称
		path += name;
		bool flag = CreateDirectory(stringToWstring(path).c_str(), NULL);
		vector< vector<string>> vec_DxgAndDqz;
		/*******************将电信港的数据存储*****************/
		for (int i = 0; i < name2_Level1.size(); i++)
		{
			GenerateTheFile(path, name2_Level1[i], m_RecvMap_DXG[i], "电信港");

		}
		/*******************将地球站的数据存********************/
		for (int i = 0; i < name2_Level2.size(); i++)
		{
			GenerateTheFile(path, name2_Level2[i], m_RecvMap_DQZ[i], "地球站");
		}
		/*******************将地球站的馈源波形天线数据存储**********************************/
		for (int i = 0; i < name2_Level3.size(); i++)
		{
			GenerateTheFile(path, name2_Level3[i], m_RecvMap_DQZChild[i], "馈源波形天线");
		}
		/********************将电信港对应地球站关系存储********************/
		for (int i = 0; i < pNode_2_2_num; i++)
		{
			vector<string> vec;
			int childNum = pNode_2_2[i]->GetCountChild();
			vec.push_back(wstringToString(pNode_2_2[i]->GetItemText().GetData()));
			for (int j = 0; j < childNum; j++)
			{

				CTreeNodeUI*  node = pNode_2_2[i]->GetChildNode(j);//GetParentNode
				string childName = wstringToString(node->GetItemText().GetData());
				vec.push_back(childName);
			}
			vec_DxgAndDqz.push_back(vec);
		}
		GenerateTheFile(path, UTF8_To_string("配置文件_电信港"), vec_DxgAndDqz);

		/**********************将地球站和其子节点关系存储******************************/
		for (int i = 0; i < pNode_2_3_num; i++)
		{
			vector<string> vec;
			int childNum = pNode_2_3[i]->GetCountChild();
			vec.push_back(wstringToString(pNode_2_3[i]->GetItemText().GetData()));
			for (int j = 0; j < childNum; j++)
			{

				CTreeNodeUI*  node = pNode_2_3[i]->GetChildNode(j);//GetParentNode
				string childName = wstringToString(node->GetItemText().GetData());
				vec.push_back(childName);
			}
			vec_WXAndChild.push_back(vec);
		}
		GenerateTheFile(path, UTF8_To_string("配置文件_地球站"), vec_WXAndChild);
	}
	else if (sName.CompareNoCase(_T("bt_Tree1_SaveAs3")) == 0)//tab2 另存为
	{
		string path;
		path = "D:\\DATA1\\";
		m_CPopNewConstellation->MessageBox(NULL);
		string name = m_CPopNewConstellation->GetNewName();//	string GetNewName();//获取修改后的名称
		path += name;
		bool flag = CreateDirectory(stringToWstring(path).c_str(), NULL);
		vector< vector<string>> vec_XZAndWX;
		/*******************将星座的数据存储*****************/
		for (int i = 0; i < name3_Level1.size(); i++)
		{
			GenerateTheFile(path, name3_Level1[i], m_RecvMap_DMZD[i], "地面终端");

		}
		/*******************将卫星的数据存********************/
		for (int i = 0; i < name3_Level2.size(); i++)
		{
			GenerateTheFile(path, name3_Level2[i], m_RecvMap_DMZDChild[i], "载荷波形天线");
		}
		/********************将星座对应卫星关系存储********************/
		for (int i = 0; i < pNode_3_2_num; i++)
		{
			vector<string> vec;
			int childNum = pNode_3_2[i]->GetCountChild();
			vec.push_back(wstringToString(pNode_3_2[i]->GetItemText().GetData()));
			for (int j = 0; j < childNum; j++)
			{

				CTreeNodeUI*  node = pNode_3_2[i]->GetChildNode(j);//GetParentNode
				string childName = wstringToString(node->GetItemText().GetData());
				vec.push_back(childName);
			}
			vec_XZAndWX.push_back(vec);
		}
		GenerateTheFile(path, UTF8_To_string("配置文件_地面终端"), vec_XZAndWX);


	}
	else if (sName.CompareNoCase(_T("bt_Tree1_Import3")) == 0)//导入
	{
		RefreshTheInterface_tab3();
		HideThCcontrol_3(ControlNumber);
		COptionUI* pControl = static_cast<COptionUI*>(m_pm.FindControl(_T("optical_ScenarioData3")));//场景数据
		if (pControl->IsSelected())
		{
			cout << "tab3场景数据" << endl;
		}
		COptionUI* pControl_1 = static_cast<COptionUI*>(m_pm.FindControl(_T("optical_LocalData3")));//本地数据
		if (pControl_1->IsSelected())
		{

			ImportingLocalData_Tab3();//导入本地数据
		}
	}
	else if (sName.CompareNoCase(_T("bt_save3")) == 0)////保存tab2 bt_save
	{

		map<string, string> map_inform;//界面上修改完的map
		map_inform = GetDataFromInterfaces_3();

		string FileName = wstringToString(m_modiftName_3.GetData());
		for (int i = 0; i < name3_Level1.size(); i++)
		{
			if (name3_Level1[i] == FileName)
			{
				m_RecvMap_DMZD[i] = map_inform;
			}
		}

		for (int i = 0; i < name3_Level2.size(); i++)
		{
			if (name3_Level2[i] == FileName)
			{
				m_RecvMap_DMZDChild[i] = map_inform;
			}
		}
		// 为了创建表，字段新增的逻辑;
		for (int i = 0; i < m_vecDmzdBxName.size(); i++)
		{
			if (m_vecDmzdBxName[i] == FileName)
			{
				m_vecMapDmzdBxData[i]=map_inform;
			}
		}
		for (int i = 0; i < m_vecDmzdZhName.size(); i++)
		{
			if (m_vecDmzdZhName[i] == FileName)
			{
				m_vecMapDmzdZhData[i]=map_inform;
			}
		}
		for (int i = 0; i < m_vecDmzdTxName.size(); i++)
		{
			if (m_vecDmzdTxName[i] == FileName)
			{
				m_vecMapDmzdTxData[i]=map_inform;
			}
		}


	}
	else if (sName.CompareNoCase(_T("bt_AddTo3")) == 0)//增加 电信港属性 Tab2
	{
		m_CPopNewAttributeClass->MessageBox(NULL);//m_CPopNewAttributeClass

		vector<string> vec = m_CPopNewAttributeClass->GetNewName();//获取新增加的
		if (vec.size() > 0)
		{
			map<string, string> map_inform;//界面上修改完的map
			string FileName = wstringToString(m_modiftName_3.GetData());
			int sequese = -1;
			for (int i = 0; i < name3_Level1.size(); i++)//给电信港增加属性
			{
				if (name3_Level1[i] == FileName)
				{
					map_inform = m_RecvMap_DMZD[i];//m_RecvMap_XZ[i] = map_inform;
					sequese = i;

					map_inform[string_To_UTF8(vec[0])] = string_To_UTF8(vec[1]);
					m_RecvMap_DMZD[sequese] = map_inform;
					DisplayInformation_3(m_RecvMap_DMZD[sequese]);
				}
			}



			for (int i = 0; i < name3_Level2.size(); i++)//给地球站添加属性
			{
				if (name3_Level2[i] == FileName)
				{
					map_inform = m_RecvMap_DMZDChild[i];//m_RecvMap_XZ[i] = map_inform;
					sequese = i;

					map_inform[string_To_UTF8(vec[0])] = string_To_UTF8(vec[1]);
					m_RecvMap_DMZDChild[sequese] = map_inform;
					DisplayInformation_3(m_RecvMap_DMZDChild[sequese]);
				}
			}

			// 为了创建表，字段新增的逻辑;
			for (int i = 0; i < m_vecDmzdBxName.size(); i++)
			{
				if (m_vecDmzdBxName[i] == FileName)
				{
					map_inform = m_vecMapDmzdBxData[i];
					sequese = i;

					map_inform[string_To_UTF8(vec[0])] = string_To_UTF8(vec[1]);
					m_vecMapDmzdBxData[sequese] = map_inform;

				}
			}
			for (int i = 0; i < m_vecDmzdZhName.size(); i++)
			{
				if (m_vecDmzdZhName[i] == FileName)
				{
					map_inform = m_vecMapDmzdZhData[i];
					sequese = i;

					map_inform[string_To_UTF8(vec[0])] = string_To_UTF8(vec[1]);
					m_vecMapDmzdZhData[sequese] = map_inform;

				}
			}
			for (int i = 0; i < m_vecDmzdTxName.size(); i++)
			{
				if (m_vecDmzdTxName[i] == FileName)
				{
					map_inform = m_vecMapDmzdTxData[i];
					sequese = i;

					map_inform[string_To_UTF8(vec[0])] = string_To_UTF8(vec[1]);
					m_vecMapDmzdTxData[sequese] = map_inform;

				}
			}


		}
	}
	else if (sName.CompareNoCase(_T("bt_Delete3")) == 0)//删除电信港属性 tab2
	{
		vector<string> vec_name;
		int flag = -1;
		map<string, string> map_inform;//界面上修改完的map
		string FileName = wstringToString(m_modiftName_3.GetData());
		int sequese;
		for (int i = 0; i < name3_Level1.size(); i++)
		{
			if (name3_Level1[i] == FileName)
			{
				map_inform = m_RecvMap_DMZD[i];//m_RecvMap_XZ[i] = map_inform;
				sequese = i;
				flag = 0;

			}
		}
		for (int i = 0; i < name3_Level2.size(); i++)
		{
			if (name3_Level2[i] == FileName)
			{
				map_inform = m_RecvMap_DMZDChild[i];//m_RecvMap_XZ[i] = map_inform;
				sequese = i;
				flag = 1;
			}
		}


		for (map<string, string>::iterator iter = map_inform.begin(); iter != map_inform.end(); ++iter)
		{
			cout << "key:" << iter->first << " value:" << iter->second << endl;
			vec_name.push_back(iter->first);
		}
		m_CPopDeleteAttClass->MessageBox(NULL, vec_name);//m_CPopNewAttributeClass

		if (flag == 0)//电信港
		{
			map<string, string> map_inform1 = m_RecvMap_DMZD[sequese];
			vector<string> vec_deleteName = m_CPopDeleteAttClass->GetDeleteName();
			for (map<string, string>::iterator iter = map_inform.begin(); iter != map_inform.end(); ++iter)
			{
				cout << "key:" << iter->first << " value:" << iter->second << endl;
				for (int i = 0; i < vec_deleteName.size(); i++)
				{
					if (iter->first == vec_deleteName[i])
					{
						//cout << "选择" << endl;
						map_inform1.erase(iter->first);
					}

				}
			}

			m_RecvMap_DMZD[sequese] = map_inform1;
			DisplayInformation_3(m_RecvMap_DMZD[sequese]);
		}
		else if (flag == 1)//地球站
		{
			map<string, string> map_inform1 = m_RecvMap_DMZDChild[sequese];
			vector<string> vec_deleteName = m_CPopDeleteAttClass->GetDeleteName();
			for (map<string, string>::iterator iter = map_inform.begin(); iter != map_inform.end(); ++iter)
			{
				cout << "key:" << iter->first << " value:" << iter->second << endl;
				for (int i = 0; i < vec_deleteName.size(); i++)
				{
					if (iter->first == vec_deleteName[i])
					{
						//cout << "选择" << endl;
						map_inform1.erase(iter->first);
					}

				}
			}

			m_RecvMap_DMZDChild[sequese] = map_inform1;
			DisplayInformation_3(m_RecvMap_DMZDChild[sequese]);
		}

		// 创建表及字段添加的逻辑; // 删除;
		const vector<string> vecDeleteName = m_CPopDeleteAttClass->GetDeleteName();
		for (int i = 0; i < m_vecDmzdZhName.size(); i++)
		{
			if (m_vecDmzdZhName[i] == FileName)
			{
				map<string, string>& mapData = m_vecMapDmzdZhData[i];//m_RecvMap_XZ[i] = map_inform;
				for (map<string, string>::iterator iter = mapData.begin(); iter != mapData.end();)
				{
					bool erased = false;
					for (int i = 0; i < vecDeleteName.size(); i++)
					{
						if (iter->first == vecDeleteName[i])
						{
							//cout << "选择" << endl;
							iter = mapData.erase(iter);
							erased = true;
						}

					}
					if (!erased)
					{
						++iter;
					}
				}
				break;
			}
		}

		for (int i = 0; i < m_vecDmzdBxName.size(); i++)
		{
			if (m_vecDmzdBxName[i] == FileName)
			{
				map<string, string>& mapData = m_vecMapDmzdBxData[i];//m_RecvMap_XZ[i] = map_inform;
				for (map<string, string>::iterator iter = mapData.begin(); iter != mapData.end();)
				{
					bool erased = false;
					for (int i = 0; i < vecDeleteName.size(); i++)
					{
						if (iter->first == vecDeleteName[i])
						{
							iter = mapData.erase(iter);
							erased = true;
						}

					}
					if (!erased)
					{
						++iter;
					}
				}
				break;
			}
		}
		// 
		for (int i = 0; i < m_vecDmzdTxName.size(); i++)
		{
			if (m_vecDmzdTxName[i] == FileName)
			{
				map<string, string>& mapData = m_vecMapDmzdTxData[i];//m_RecvMap_XZ[i] = map_inform;
				for (map<string, string>::iterator iter = mapData.begin(); iter != mapData.end();)
				{
					bool erased = false;
					for (int i = 0; i < vecDeleteName.size(); i++)
					{
						if (iter->first == vecDeleteName[i])
						{
							iter = mapData.erase(iter);
							erased = true;
						}

					}
					if (!erased)
					{
						++iter;
					}
				}
				break;
			}
		}

	}
	//bt_Tree1_Import3
	//bt_Tree1_SaveAs3
}

void CPopEditorialClass::OnSelectChanged(TNotifyUI &msg)
{
	CDuiString sName = msg.pSender->GetName();
	sName.MakeLower();

}

void CPopEditorialClass::OnItemSelect(TNotifyUI &msg)
{
	CDuiString sName = msg.pSender->GetName();
	sName.MakeLower();
}

LRESULT CPopEditorialClass::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if (uMsg == WM_MENUCLICK)
	{
		MenuCmd* pMenuCmd = (MenuCmd*)wParam;
		if (pMenuCmd != NULL)
		{
			BOOL bChecked = pMenuCmd->bChecked;
			CDuiString sMenuName = pMenuCmd->szName;
			CDuiString sUserData = pMenuCmd->szUserData;
			CDuiString sText = pMenuCmd->szText;
			m_pm.DeletePtr(pMenuCmd);

			if (sMenuName.CompareNoCase(_T("modify")) == 0)
			{
				m_CpopModifyInterface->MessageBox(NULL, m_modiftName);
				string name = m_CpopModifyInterface->GetNewName();//	string GetNewName();//获取修改后的名称
				newName = name;
				if (name != "")
				{
					ModifyNodeName();
					cout << name << endl;
				}

				UncheckedTab1();

			}
			else if (sMenuName.CompareNoCase(_T("NewConstellation")) == 0)//新建星座
			{

				m_CPopNewConstellation->MessageBox(NULL);

				string name = m_CPopNewConstellation->GetNewName();//	string GetNewName();//获取修改后的名称
				newHoroscope = name;

				if (name != "")
				{
					AssignmentToLocalVector_XZ(newHoroscope);//向树的以及目录中添加数据
				}



				CTreeViewUI* pTreeView = static_cast<CTreeViewUI*>(m_pm.FindControl(_T("treeview1")));
				pTreeView->SetItemExpand(false, NULL);//让树形结构初始状态为不展开的
				UncheckedTab1();

			}
			else if (sMenuName.CompareNoCase(_T("RailType")) == 0)//分类---轨道类别 
			{
				if (bChecked)
				{
					if (FunctionalCategoryFlag)//恢复功能类别的分类
					{
						Restore_XZ_Functional();
						FunctionalCategoryFlag = false;

	
					}
					if (PayloadFlag)
					{
						Restore_XZ_LoadType();
						PayloadFlag = false;


					}

					Restore_XZ_classification();
					CMenuWnd::SetMenuItemInfo(_T("FunctionalCategory"), false);
					CMenuWnd::SetMenuItemInfo(_T("Payload"), false);

					RailTypeFlag = true;
					Categorize_XZ_RailType();
					UncheckedTab1();

				}
				else
				{
					RailTypeFlag = false;
					Restore_XZ_RailType();

					CTreeViewUI* pTreeView = static_cast<CTreeViewUI*>(m_pm.FindControl(_T("treeview1")));
					pTreeView->SetItemExpand(false, NULL);//让树形结构初始状态为不展开的
					HideThCcontrol(ControlNumber);
					pNode_1[0]->Select(false);
					UncheckedTab1();

					CControlUI* pControl = static_cast<CControlUI*>(m_pm.FindControl(_T("bt_save")));
					pControl->SetEnabled(TRUE);
					//bt_AddTo
					CControlUI* pControl1 = static_cast<CControlUI*>(m_pm.FindControl(_T("bt_AddTo")));
					pControl1->SetEnabled(TRUE);
					CControlUI* pControl2 = static_cast<CControlUI*>(m_pm.FindControl(_T("bt_Delete")));
					pControl2->SetEnabled(TRUE);
				}
			}
			else if (sMenuName.CompareNoCase(_T("FunctionalCategory")) == 0)//功能类别
			{
				if (bChecked)
				{
					if (RailTypeFlag)
					{
						Restore_XZ_RailType();
						RailTypeFlag = false;

					}
					if (PayloadFlag)
					{
						Restore_XZ_LoadType();
						PayloadFlag = false;

					}
					FunctionalCategoryFlag = true;//功能分类 FunctionalCategory
					CMenuWnd::SetMenuItemInfo(_T("RailType"), false);
					CMenuWnd::SetMenuItemInfo(_T("Payload"), false);
					Categorize_XZ_Functional();
					UncheckedTab1();
				}
				else
				{
					FunctionalCategoryFlag = false;
					Restore_XZ_Functional();
					CTreeViewUI* pTreeView = static_cast<CTreeViewUI*>(m_pm.FindControl(_T("treeview1")));
					pTreeView->SetItemExpand(false, NULL);//让树形结构初始状态为不展开的
					HideThCcontrol(ControlNumber);
					pNode_1[0]->Select(false);
					UncheckedTab1();
				}
			}
			else if (sMenuName.CompareNoCase(_T("Payload")) == 0)//------载荷类型分类
			{
				if (bChecked)
				{
					if (RailTypeFlag)
					{
						Restore_XZ_RailType();
						RailTypeFlag = false;
					}
					if (FunctionalCategoryFlag)//恢复功能类别的分类
					{
						Restore_XZ_Functional();
						FunctionalCategoryFlag = false;
					}

					PayloadFlag = true;//载荷类型分类
					CMenuWnd::SetMenuItemInfo(_T("RailType"), false);
					CMenuWnd::SetMenuItemInfo(_T("FunctionalCategory"), false);

					Categorize_XZ_LoadType();
					UncheckedTab1();


				}
				else
				{
					PayloadFlag = false;

					Restore_XZ_LoadType();
					CTreeViewUI* pTreeView = static_cast<CTreeViewUI*>(m_pm.FindControl(_T("treeview1")));
					pTreeView->SetItemExpand(false, NULL);//让树形结构初始状态为不展开的
					HideThCcontrol(ControlNumber);
					pNode_1[0]->Select(false);
					UncheckedTab1();


				}
			}
			//功能类别 FunctionalCategory
			if (sMenuName.CompareNoCase(_T("modify1")) == 0)
			{
				cout << "111111" << endl;

				m_CpopModifyInterface->MessageBox(NULL, m_modiftName);

				string name = m_CpopModifyInterface->GetNewName();//	string GetNewName();//获取修改后的名称
				newName = name;


				if (name != "")
				{
					ModifyNodeName();
					cout << name << endl;
				}
				cout << name << endl;
				UncheckedTab1();

			}
			else if (sMenuName.CompareNoCase(_T("NewSatellite")) == 0)//新建卫星
			{
				cout << "111111" << endl;
				m_CPopNewConstellation->MessageBox(NULL);

				string name = m_CPopNewConstellation->GetNewName();//	string GetNewName();//获取修改后的名称
				newsatellite = name;

				if (name != "")
				{
					map<string, string> map0;
					map0 = GetSatelliteInform();
					int num = selectNode[0][1];
					AssignmentToLocalVector(newsatellite, map0, selectNode[0][1]);//将卫星新建在树节点上

					map<string, string> map1;
					map1 = GetSatelliteInform_1();//获取天线属性信息
					AssignmentToLocalVector_WXSX(newsatellite, UTF8_To_string("天线属性信息"), map1);
					map<string, string> map2;
					map2 = GetSatelliteInform_2();//获取载荷属性信息
					AssignmentToLocalVector_WXSX(newsatellite, UTF8_To_string("载荷属性信息"), map2);


					
				}


				CTreeViewUI* pTreeView = static_cast<CTreeViewUI*>(m_pm.FindControl(_T("treeview1")));
				pTreeView->SetItemExpand(false, NULL);//让树形结构初始状态为不展开的
				UncheckedTab1();

			}
			else if (sMenuName.CompareNoCase(_T("SatellitePayload")) == 0)//卫星载荷 筛选
			{
				int num = 0;
				int serialNumber = 0;
				string str_name = wstringToString(m_modiftName.GetData());
				for (int i = 0; i < name_Level1.size(); i++)
				{
					if (str_name == name_Level1[i])
					{
						serialNumber = i;
					}
				}
				if (vec_ExpandFlags[serialNumber])
				{
					//CMenuWnd::SetMenuItemInfo(_T("SatellitePayload"), true);
					Restore_WX_LoadFunction(str_name);
					vec_ExpandFlags[serialNumber] = FALSE;
					//CMenuWnd::SetMenuItemInfo(_T("SatellitePayload"), false);
					UncheckedTab1();



				}
				else
				{
					//CMenuWnd::SetMenuItemInfo(_T("SatellitePayload"), false);


					Categorize_WX_LoadFunction(str_name);
					vec_ExpandFlags[serialNumber] = TRUE;


		

					//CMenuWnd::SetMenuItemInfo(_T("SatellitePayload"), false);

					UncheckedTab1();
				}
				//if (bChecked)                                                                                                          
				//{

				//	int num = 0;
				//	string str_name = wstringToString(m_modiftName.GetData());
			 // //   	Categorize_WX_LoadFunction(str_name);
				//	//UncheckedTab1();
				//	/*******************可以把每个星座都展开并且关闭的但是没有对钩显示当前是按照什么分类的********************/
				//	int serialNumber = 0;
				//	for (int i = 0; i < name_Level1.size(); i++)
				//	{
				//		if (str_name == name_Level1[i])
				//		{
				//			serialNumber = i;
				//		}
				//	}
				//	if (vec_ExpandFlags[serialNumber])
				//	{
				//		//CMenuWnd::SetMenuItemInfo(_T("SatellitePayload"), true);
				//		Restore_WX_LoadFunction(str_name);
				//		vec_ExpandFlags[serialNumber] = FALSE;
				//		CMenuWnd::SetMenuItemInfo(_T("SatellitePayload"), false);
				//		UncheckedTab1();
				//	}
				//	else
				//	{
				//		//CMenuWnd::SetMenuItemInfo(_T("SatellitePayload"), false);


				//		Categorize_WX_LoadFunction(str_name);
				//		vec_ExpandFlags[serialNumber] = TRUE;
				//		CMenuWnd::SetMenuItemInfo(_T("SatellitePayload"), false);

				//		UncheckedTab1();
				//	}
					//vec_ExpandFlags[serialNumber] = TRUE;


				//}
				//else
				//{
					//cout << "11111111111111" << endl;
					//int num = 0;
					//string str_name = wstringToString(m_modiftName.GetData());
					//Restore_WX_LoadFunction(str_name);
					//int serialNumber = 0;
					//for (int i = 0; i < name_Level1.size(); i++)
					//{
					//	if (str_name == name_Level1[i])
					//	{
					//		serialNumber = i;
					//	}
					//}
					//pNode_2[serialNumber]->Select(false);
					//UncheckedTab1();
				//}
			}

			//if (sMenuName.CompareNoCase(_T("modify2")) == 0)
			//{
			//	cout << "111111" << endl;

			//	m_CpopModifyInterface->MessageBox(NULL, m_modiftName);

			//	string name = m_CpopModifyInterface->GetNewName();//	string GetNewName();//获取修改后的名称
			//	newName = name;

			//	if (name != "")
			//	{
			//		ModifyNodeName();
			//		cout << name << endl;
			//	}

			//	cout << name << endl;
			//	UncheckedTab1();

			//}//m_CpopSelectLevel1
			//else
			if (sMenuName.CompareNoCase(_T("MoveTo")) == 0)//移动到
			{

				m_CpopSelectLevel1->MessageBox(NULL, name_Level1);

				string name = m_CpopSelectLevel1->GetNodeName();//	string GetNewName();//获取修改后的名称

				cout << name << endl;
				if (name != "")
				{
					Movelevel_2(name, m_modiftName);
				}

				//

				UncheckedTab1();
				//m_modiftName
				//newHoroscope = name;
			}

			//*****************tab2**************************/
			if (sMenuName.CompareNoCase(_T("modify2_0")) == 0)
			{

				m_CpopModifyInterface->MessageBox(NULL, m_modiftName_2);

				string name = m_CpopModifyInterface->GetNewName();//	string GetNewName();//获取修改后的名称
				newName_2 = name;
				
				if (name != "")
				{
					ModifyNodeName_2();
					cout << name << endl;
				}


				UncheckedTab2();

			}//m_CpopSelectLevel1 
			else if (sMenuName.CompareNoCase(_T("NewTeleport")) == 0)//新建电信港
			{
				m_CPopNewConstellation->MessageBox(NULL);

				string name = m_CPopNewConstellation->GetNewName();//	string GetNewName();//获取修改后的名称
				newTeleport = name;

				if (name != "")
				{
					AssignmentToLocalVector_DXG(newTeleport);

				}
				//map<string, string> map;
				//map = GetTeleport();


				//CTreeViewUI* pTreeView = static_cast<CTreeViewUI*>(m_pm.FindControl(_T("treeview2")));
				m_treeView_2->SetItemExpand(false, NULL);//让树形结构初始状态为不展开的
				UncheckedTab2();

			}//
			else if (sMenuName.CompareNoCase(_T("Characteristic")) == 0)//电信港按性质分类
			{
				if (bChecked)
				{

					Restore_DXG_classification();
					Categorize_DXG_characteristic();
					UncheckedTab2();
				}
				else
				{

					Restore_DXG_characteristic();
					m_treeView_2->SetItemExpand(false, NULL);//让树形结构初始状态为不展开的
					HideThCcontrol_2(ControlNumber);
					pNode_2_1[0]->Select(false);
					UncheckedTab2();
				}
			}

			//电信港修改
			if (sMenuName.CompareNoCase(_T("modify2_1")) == 0)
			{
				m_CpopModifyInterface->MessageBox(NULL, m_modiftName_2);

				string name = m_CpopModifyInterface->GetNewName();//	string GetNewName();//获取修改后的名称
				newName_2 = name;
				
				if (name != "")
				{
					ModifyNodeName_2();
					cout << name << endl;
				}


				cout << name << endl;
				UncheckedTab2();

			}//m_CpopSelectLevel1 
			else if (sMenuName.CompareNoCase(_T("NewEarthStation")) == 0)//新建地球站
			{
				cout << "111111" << endl;
				m_CPopNewConstellation->MessageBox(NULL);

				string name = m_CPopNewConstellation->GetNewName();//	string GetNewName();//获取修改后的名称
				newEarthStation = name;

				if (name != "")
				{
					map<string, string> map0;
					map0 = GetEarthStation();
					int num = selectNode_2[0][1];
					AssignmentToLocalVector_DQZ(newEarthStation, map0, selectNode_2[0][1]);//向树的以及目录中添加数据
				
					map<string, string> map1;
					map1 = GetEarthStation_1();//获取天线属性信息
					AssignmentToLocalVector_DQZ_level2(newEarthStation, UTF8_To_string("馈源属性信息"), map1);
					map<string, string> map2;
					map2 = GetEarthStation_2();//获取波形设备属性信息
					AssignmentToLocalVector_DQZ_level2(newEarthStation, UTF8_To_string("波形设备属性信息"), map2);
					map<string, string> map3;
					map3 = GetEarthStation_3();//获取波形设备属性信息
					AssignmentToLocalVector_DQZ_level2(newEarthStation, UTF8_To_string("天线属性信息"), map3);

					
				}



				//CTreeViewUI* pTreeView = static_cast<CTreeViewUI*>(m_pm.FindControl(_T("treeview2")));
				m_treeView_2->SetItemExpand(false, NULL);//让树形结构初始状态为不展开的
				UncheckedTab2();

			}//
			else if (sMenuName.CompareNoCase(_T("FunctionType1_1")) == 0)//地球站按功能类型分类
			{

				int serialNumber;//存放选择的星座名称							
				string str_name = wstringToString(m_modiftName_2.GetData());
				//Categorize_DQZ_FunctionType1(str_name);
				/*******************筛选星座名称在存放星座名称的vector中的位置***************/
				for (int i = 0; i < name2_Level1.size(); i++)
				{
					if (str_name == name2_Level1[i])
					{
						serialNumber = i;
					}
				}

				if (vec_ExpandFlags2_FT[serialNumber])
				{
					Restore_DQZ_FunctionType1(str_name);
					vec_ExpandFlags2_FT[serialNumber] = FALSE;
					
					UncheckedTab2();
				}
				else
				{
					if (vec_ExpandFlags2_ES[serialNumber])
					{
						Restore_DQZ_EarthStation(str_name);
						vec_ExpandFlags2_ES[serialNumber] = FALSE;

						UncheckedTab2();
					}

					Categorize_DQZ_FunctionType1(str_name);
					vec_ExpandFlags2_FT[serialNumber] = TRUE;

					UncheckedTab2();
				}

			}
			else if (sMenuName.CompareNoCase(_T("EarthStation")) == 0)//地球站按地球站类型分类
			{
				int serialNumber;//存放选择的星座名称							
				string str_name = wstringToString(m_modiftName_2.GetData());
				
				/*******************筛选星座名称在存放星座名称的vector中的位置***************/
				for (int i = 0; i < name2_Level1.size(); i++)
				{
					if (str_name == name2_Level1[i])
					{
						serialNumber = i;
					}
				}
				if (vec_ExpandFlags2_ES[serialNumber])
				{
					Restore_DQZ_EarthStation(str_name);
					vec_ExpandFlags2_ES[serialNumber] = FALSE;

					UncheckedTab2();
				}
				else
				{
					if (vec_ExpandFlags2_FT[serialNumber])
					{
						Restore_DQZ_FunctionType1(str_name);
						vec_ExpandFlags2_FT[serialNumber] = FALSE;

						UncheckedTab2();
					}

					Categorize_DQZ_EarthStation(str_name);
					vec_ExpandFlags2_ES[serialNumber] = TRUE;

					UncheckedTab2();
				}

			}


			////地球站修改
			//if (sMenuName.CompareNoCase(_T("modify2_2")) == 0)//
			//{
			//	m_CpopModifyInterface->MessageBox(NULL, m_modiftName_2);

			//	string name = m_CpopModifyInterface->GetNewName();//	string GetNewName();//获取修改后的名称
			//	newName_2 = name;

			//	if (name != "")
			//	{
			//		ModifyNodeName_2();
			//		cout << name << endl;
			//	}


			//	cout << name << endl;
			//	UncheckedTab2();

			//}//m_CpopSelectLevel1 
			//else
			if (sMenuName.CompareNoCase(_T("MoveTo2")) == 0)//移动到某个电信港
			{

				m_CpopSelectLevel1->MessageBox(NULL, name2_Level1);

				string name = m_CpopSelectLevel1->GetNodeName();//	string GetNewName();//获取修改后的名称

				cout << name << endl;
				cout << name << endl;
				if (name != "")
				{
					Movelevel_2_ToDXG(name, m_modiftName_2);
				}
				UncheckedTab2();
			}//
			/*************************tab3********************************/
			//
			if (sMenuName.CompareNoCase(_T("modify3_0")) == 0)
			{
				m_CpopModifyInterface->MessageBox(NULL, m_modiftName_3);

				string name = m_CpopModifyInterface->GetNewName();//	string GetNewName();//获取修改后的名称
				newName_3 = name;
				if (name != "")
				{
					ModifyNodeName_3();
					cout << name << endl;
				}


				cout << name << endl;
				UncheckedTab3();
			}
			else if (sMenuName.CompareNoCase(_T("NewGroundTerminal")) == 0)//新建地面终端
			{
				cout << "111111" << endl;
				m_CPopNewConstellation->MessageBox(NULL);

				string name = m_CPopNewConstellation->GetNewName();//	string GetNewName();//获取修改后的名称
				newGroundTerminal = name;

				if (name != "")
				{
					map<string, string> map0;
					map0 = GetGroundTerminal();
					int num = selectNode_3[0][1];
					AssignmentToLocalVector_DMZD(newGroundTerminal, map0);//向树的以及目录中添加数据

					map<string, string> map1;
					map1 = GetGroundTerminal_1();//获取天线属性信息
					AssignmentToLocalVector_DMZD_level2(newGroundTerminal, UTF8_To_string("载荷属性信息"), map1);
					map<string, string> map2;
					map2 = GetGroundTerminal_2();//获取波形设备属性信息
					AssignmentToLocalVector_DMZD_level2(newGroundTerminal, UTF8_To_string("波形设备属性信息"), map2);
					map<string, string> map3;
					map3 = GetGroundTerminal_3();//获取波形设备属性信息
					AssignmentToLocalVector_DMZD_level2(newGroundTerminal, UTF8_To_string("天线属性信息"), map3);


				}


				//CTreeViewUI* pTreeView = static_cast<CTreeViewUI*>(m_pm.FindControl(_T("treeview2")));
				m_treeView_3->SetItemExpand(false, NULL);//让树形结构初始状态为不展开的
				UncheckedTab3();
			}
			else if (sMenuName.CompareNoCase(_T("Terminaltype")) == 0)
			{
				if (bChecked)
				{
					Categorize_DMZD_TerminalType();
					UncheckedTab3();
				}
				else
				{

					Restore_DMZD_TerminalType();
					pNode_3_1[0]->Select(false);
					m_treeView_3->SetItemExpand(false, NULL);//让树形结构初始状态为不展开的
					HideThCcontrol_3(ControlNumber);
					UncheckedTab3();
				}
			}
			//else if (sMenuName.CompareNoCase(_T("PeerManagement")) == 0)//新建同级地面终端管理
			//{
			//	cout << "111111" << endl;
			//	m_CPopNewConstellation->MessageBox(NULL);

			//	string name = m_CPopNewConstellation->GetNewName();//	string GetNewName();//获取修改后的名称
			//	newGroundTerminal = name;

			//	if (name != "")
			//	{
			//		CreateTreeNode_3(name);
			//		//AssignmentToLocalVector_DMZD(newGroundTerminal, map);//向树的以及目录中添加数据
			//	}


			//	//CTreeViewUI* pTreeView = static_cast<CTreeViewUI*>(m_pm.FindControl(_T("treeview2")));
			//	m_treeView_3->SetItemExpand(false, NULL);//让树形结构初始状态为不展开的
			//	UncheckedTab3();
			//}

			//if (sMenuName.CompareNoCase(_T("modify3_1")) == 0)
			//{
			//	m_CpopModifyInterface->MessageBox(NULL, m_modiftName_3);


			//	string name = m_CpopModifyInterface->GetNewName();//	string GetNewName();//获取修改后的名称
			//	newName_3 = name;
			//	
			//	if (name != "")
			//	{
			//		ModifyNodeName_3();
			//	}


			//	cout << name << endl;
			//	UncheckedTab3();
			//}
			
		}
	}
	bHandled = FALSE;
	return 0;
}

LRESULT CPopEditorialClass::OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	// 有时会在收到WM_NCDESTROY后收到wParam为SC_CLOSE的WM_SYSCOMMAND
	if (wParam == SC_CLOSE) {
		::PostQuitMessage(0L);
		bHandled = TRUE;
		return 0;
	}
	BOOL bZoomed = ::IsZoomed(*this);
	LRESULT lRes = CWindowWnd::HandleMessage(uMsg, wParam, lParam);
	if (::IsZoomed(*this) != bZoomed) {
		if (!bZoomed) {
			CControlUI* pControl = static_cast<CControlUI*>(m_pm.FindControl(_T("maxbtn")));
			if (pControl) pControl->SetVisible(false);
			pControl = static_cast<CControlUI*>(m_pm.FindControl(_T("restorebtn")));
			if (pControl) pControl->SetVisible(true);
		}
		else {
			CControlUI* pControl = static_cast<CControlUI*>(m_pm.FindControl(_T("maxbtn")));
			if (pControl) pControl->SetVisible(true);
			pControl = static_cast<CControlUI*>(m_pm.FindControl(_T("restorebtn")));
			if (pControl) pControl->SetVisible(false);
		}
	}
	return lRes;
}


string  CPopEditorialClass::get_cur_timelogdata()/*获取当前时间字符串*/
{
	string time_string;
	SYSTEMTIME stime;
	char buf[100] = { 0 };
	GetLocalTime(&stime);
	sprintf(buf, "%04d.%02d.%02d.%02d.%02d.%02d", stime.wYear, stime.wMonth, stime.wDay, stime.wHour, stime.wMinute, stime.wSecond);
	time_string += buf;
	return time_string;
}
void CPopEditorialClass::OnLClick(CControlUI *pControl)
{

	CDuiString sName = pControl->GetName();

	//	if(pTabSwitch != NULL)
	//CGameBoxFrame:pTabSwitch->SelectItem(FirstTab);
}
void CPopEditorialClass::Notify(TNotifyUI &msg)
{
	CDuiString strControlName = msg.pSender->GetName();
	if (msg.sType == _T("click"))
	{

		// 按钮消息
		OnLClick(msg.pSender);
	}
	else if (msg.sType == _T("selectchanged"))
	{

		if (strControlName.CompareNoCase(_T("opt_Tab1")) == 0)
		{
			UncheckedTab2();
			UncheckedTab3();
			pTabDatabase->SelectItem(0);

		}
		else if (strControlName.CompareNoCase(_T("opt_Tab2")) == 0) 
		{
			UncheckedTab1();
			UncheckedTab3();
			pTabDatabase->SelectItem(1);
		}
		else if (strControlName.CompareNoCase(_T("opt_Tab3")) == 0) 
		{
			UncheckedTab1();
			UncheckedTab2();
			pTabDatabase->SelectItem(2);
		}
	}
	else if (msg.sType == _T("treeitemdbclick") )//双击树分支 && msg.pSender->GetName() == _T("treeview3")
	{
		GetNumberChild_0();
		GetNumberChild();
		CurrentIndex = 0;
		///*********************界面1 星座卫星**********************************/
		for (int i = 0; i < pNode_1_num; i++)
		{
			//			if(pNode_1[i]->)
			int childNum = pNode_1[i]->GetCountChild();
			if (pNode_1[i]->IsSelected())
			{
				m_modiftName = pNode_1[i]->GetItemText();
				cout << "被选中了" << endl;
				selectNode[0][0] = i;
				selectNode[0][1] = -1;
				selectNode[0][2] = -1;
				selectNode[0][3] = -1;
				//pNode_1[selectNode[0][0]]->Select(false);
				//FindTheMap();
			}
			else
			{
				for (int j = 0; j < childNum; j++)
				{

					CTreeNodeUI*  node = pNode_1[i]->GetChildNode(j);//GetParentNode
					if (node->IsSelected())
					{
						m_modiftName = node->GetItemText();
						selectNode[0][0] = i;
						selectNode[0][1] = j;
						selectNode[0][2] = -1;
						selectNode[0][3] = -1;
						cout << "被选中了!!!" << endl;
						FindTheMap();
						//node->Select(false);
					}
					else
					{
						//CTreeNodeUI*  node1 = pNode_1[i]->GetChildNode(j)->GetChildNode;//GetParentNode
						int num1 = pNode_1[i]->GetChildNode(j)->GetCountChild();
						for (int m = 0; m < num1; m++)
						{
							CTreeNodeUI*  node1 = node->GetChildNode(m);//GetParentNode
							if (node1->IsSelected())
							{
								m_modiftName = node1->GetItemText();
								selectNode[0][0] = i;
								selectNode[0][1] = j;
								selectNode[0][2] = m;
								selectNode[0][3] = -1;
								FindTheMap();
								//node1->Select(false);
							}
							else
							{
								int num3 = node1->GetCountChild();
								for (int n = 0; n < num3; n++)
								{
									CTreeNodeUI*  node3 = node1->GetChildNode(n);//GetParentNode
									if (node3->IsSelected())
									{
										CDuiString cdui_name = node3->GetItemText();
										m_modiftName = node3->GetItemText();
										selectNode[0][0] = i;
										selectNode[0][1] = j;
										selectNode[0][2] = m;
										selectNode[0][3] = n;
										FindTheMap();
										//m_modiftName = node1->GetItemText();
										//FindTheMap_level3(cdui_name);//这是没有天线载荷这一级别之前的操作
									}
									else
									{
										int num4 = node3->GetCountChild();
										for (int q = 0; q < num4; q++)
										{
											CTreeNodeUI*  node4 = node3->GetChildNode(q);//GetParentNode
											if (node4->IsSelected())
											{
												CDuiString cdui_name4 = node4->GetItemText();
												FindTheMap_level4(cdui_name4);
											}
										}

									}
								}
							}
						}
					}
				}
			}

		}
		///*********************界面2 电信港地球站*********************************/
		for (int i = 0; i < pNode_2_1_num; i++)
		{
			//			if(pNode_1[i]->)
			int childNum = pNode_2_1[i]->GetCountChild();
			if (pNode_2_1[i]->IsSelected())
			{
				m_modiftName_2 = pNode_2_1[i]->GetItemText();
				cout << "被选中了" << endl;
				selectNode_2[0][0] = i;
				selectNode_2[0][1] = -1;
				selectNode_2[0][2] = -1;
				selectNode_2[0][3] = -1;
				//pNode_1[selectNode[0][0]]->Select(false);
				//FindTheMap();
			}
			else
			{
				for (int j = 0; j < childNum; j++)
				{

					CTreeNodeUI*  node = pNode_2_1[i]->GetChildNode(j);//GetParentNode
					if (node->IsSelected())
					{
						m_modiftName_2 = node->GetItemText();
						selectNode_2[0][0] = i;
						selectNode_2[0][1] = j;
						selectNode_2[0][2] = -1;
						selectNode_2[0][3] = -1;
						cout << "被选中了!!!" << endl;
						FindTheMap_2();
						//node->Select(false);
					}
					else
					{
						//CTreeNodeUI*  node1 = pNode_1[i]->GetChildNode(j)->GetChildNode;//GetParentNode
						int num1 = pNode_2_1[i]->GetChildNode(j)->GetCountChild();
						for (int m = 0; m < num1; m++)
						{
							CTreeNodeUI*  node1 = node->GetChildNode(m);//GetParentNode
							if (node1->IsSelected())
							{
								m_modiftName_2 = node1->GetItemText();
								selectNode_2[0][0] = i;
								selectNode_2[0][1] = j;
								selectNode_2[0][2] = m;
								selectNode_2[0][3] = -1;
								FindTheMap_2();
								//node1->Select(false);
							}
							else
							{
								int num3 = node1->GetCountChild();
								for (int n = 0; n < num3; n++)
								{
									CTreeNodeUI*  node3 = node1->GetChildNode(n);//GetParentNode
									if (node3->IsSelected())
									{
										CDuiString cdui_name = node3->GetItemText();
										m_modiftName_2 = node3->GetItemText();
									
										selectNode_2[0][0] = i;
										selectNode_2[0][1] = j;
										selectNode_2[0][2] = m;
										selectNode_2[0][3] = n;
										FindTheMap_2();

										//CDuiString cdui_name = node3->GetItemText();
										//FindTheMap_level3_2(cdui_name);
									}
									else
									{
										int num4 = node3->GetCountChild();
										for (int q = 0; q < num4; q++)
										{
											CTreeNodeUI*  node4 = node3->GetChildNode(q);//GetParentNode
											if (node4->IsSelected())
											{
												CDuiString cdui_name4 = node4->GetItemText();
												FindTheMap_level4_2(cdui_name4);
											}
										}

									}
								}

							}
						}
					}
				}
			}

		}
		///*********************界面3 地面终端***********************************/
		for (int i = 0; i < pNode_3_1_num; i++)
		{
			//			if(pNode_1[i]->)
			int childNum = pNode_3_1[i]->GetCountChild();
			if (pNode_3_1[i]->IsSelected())
			{
				m_modiftName_3 = pNode_3_1[i]->GetItemText();
				cout << "被选中了" << endl;
				selectNode_3[0][0] = i;
				selectNode_3[0][1] = -1;
				selectNode_3[0][2] = -1;

				FindTheMap_3();

			}
			else
			{
				for (int j = 0; j < childNum; j++)
				{

					CTreeNodeUI*  node = pNode_3_1[i]->GetChildNode(j);//GetParentNode
					if (node->IsSelected())
					{
						m_modiftName_3 = node->GetItemText();
						selectNode_3[0][0] = i;
						selectNode_3[0][1] = j;
						selectNode_3[0][2] = -1;
						cout << "被选中了!!!" << endl;
						FindTheMap_3();
						//node->Select(false);
					}
					else
					{
						int num1 = pNode_3_1[i]->GetChildNode(j)->GetCountChild();
						for (int m = 0; m < num1; m++)
						{
							CTreeNodeUI*  node1 = node->GetChildNode(m);//GetParentNode
							if (node1->IsSelected())
							{
								m_modiftName_3 = node1->GetItemText();
								selectNode_3[0][0] = i;
								selectNode_3[0][1] = j;
								selectNode_3[0][2] = m;
							
								FindTheMap_3();
								//node1->Select(false);
							}
						}

					}

				}
			}
		}
	}
	else if (msg.sType == _T("itemrclick")) //在树节点上右击鼠标事件 _T("itemrclick")
	{
		/*******************右击场景卫星界面******************/
		for (int i = 0; i < pNode_1_num; i++)
		{
			int childNum = pNode_1[i]->GetCountChild();
			if (pNode_1[i]->IsSelected())
			{
				m_modiftName = pNode_1[i]->GetItemText();
				cout << "被选中了" << endl;
				selectNode[0][0] = i;
				selectNode[0][1] = -1;
				selectNode[0][2] = -1;
				if (i == 0)
				{
					isLevel0 = true;
				}

				pNode_1[i]->Select(false);
			}
			else
			{
				for (int j = 0; j < childNum; j++)
				{

					CTreeNodeUI*  node = pNode_1[i]->GetChildNode(j);//GetParentNode
					if (node->IsSelected())
					{
						m_modiftName = node->GetItemText();
						selectNode[0][0] = i;
						selectNode[0][1] = j;
						selectNode[0][2] = -1;
						cout << "被选中了!!!" << endl;
						if (i == 0)
						{
							isLevel1 = true;
						}
						
						node->Select(false);
					}
					else
					{
						//CTreeNodeUI*  node1 = pNode_1[i]->GetChildNode(j)->GetChildNode;//GetParentNode
						int num1 = pNode_1[i]->GetChildNode(j)->GetCountChild();
						for (int m = 0; m < num1; m++)
						{
							CTreeNodeUI*  node1 = node->GetChildNode(m);//GetParentNode
							if (node1->IsSelected())
							{
								m_modiftName = node1->GetItemText();
								if (i == 0)
								{
									isLevel2 = true;
								}
								
								selectNode[0][0] = i;
								selectNode[0][1] = j;
								selectNode[0][2] = m;
								node1->Select(false);
							}
						}
					}
				}
			}
		}
		/************如果是0级节点被选中**************/
		if (isLevel0)
		{
			CMenuWnd::GetGlobalContextMenuObserver().SetMenuCheckInfo(&m_MenuInfos);

			if (m_pMenu != NULL) {
				delete m_pMenu;
				m_pMenu = NULL;
			}
			m_pMenu = new CMenuWnd();
			CDuiPoint point;
			::GetCursorPos(&point);
			m_pMenu->Init(NULL, _T("menu.xml"), point, &m_pm);
			isLevel0 = false;
		}

		if (isLevel1)
		{
			CMenuWnd::GetGlobalContextMenuObserver().SetMenuCheckInfo(&m_MenuInfos);

			if (m_pMenu != NULL) {
				delete m_pMenu;
				m_pMenu = NULL;
			}
			m_pMenu = new CMenuWnd();
			CDuiPoint point;
			::GetCursorPos(&point);
			m_pMenu->Init(NULL, _T("menu1.xml"), point, &m_pm);
			isLevel1 = false;
		}
		
		if (isLevel2)
		{
			CMenuWnd::GetGlobalContextMenuObserver().SetMenuCheckInfo(&m_MenuInfos);

			if (m_pMenu != NULL) {
				delete m_pMenu;
				m_pMenu = NULL;
			}
			m_pMenu = new CMenuWnd();
			CDuiPoint point;
			::GetCursorPos(&point);
			m_pMenu->Init(NULL, _T("menu2.xml"), point, &m_pm);
			isLevel2 = false;
		}

		/*********************右击电信港地球站界面***********************/
		for (int i = 0; i < pNode_2_1_num; i++)
		{
			int childNum = pNode_2_1[i]->GetCountChild();
			if (pNode_2_1[i]->IsSelected())
			{
				m_modiftName_2 = pNode_2_1[i]->GetItemText();
				cout << "被选中了" << endl;
				selectNode_2[0][0] = i;
				selectNode_2[0][1] = -1;
				selectNode_2[0][2] = -1;
				if (i == 0)
				{
					isLevel_2_0 = true;
				}

				pNode_2_1[i]->Select(false);
			}
			else
			{
				for (int j = 0; j < childNum; j++)
				{

					CTreeNodeUI*  node = pNode_2_1[i]->GetChildNode(j);//GetParentNode
					if (node->IsSelected())
					{
						m_modiftName_2 = node->GetItemText();
						selectNode_2[0][0] = i;
						selectNode_2[0][1] = j;
						selectNode_2[0][2] = -1;
						cout << "被选中了!!!" << endl;
						if (i == 0)
						{
							isLevel_2_1 = true;
						}

						node->Select(false);
					}
					else
					{
						//CTreeNodeUI*  node1 = pNode_1[i]->GetChildNode(j)->GetChildNode;//GetParentNode
						int num1 = pNode_2_1[i]->GetChildNode(j)->GetCountChild();
						for (int m = 0; m < num1; m++)
						{
							CTreeNodeUI*  node1 = node->GetChildNode(m);//GetParentNode
							if (node1->IsSelected())
							{
								m_modiftName_2 = node1->GetItemText();
								if (i == 0)
								{
									isLevel_2_2 = true;
								}

								selectNode_2[0][0] = i;
								selectNode_2[0][1] = j;
								selectNode_2[0][2] = m;
								node1->Select(false);
							}
						}
					}
				}
			}
		}
		/************如果是0级节点被选中**************/
		if (isLevel_2_0)
		{
			CMenuWnd::GetGlobalContextMenuObserver().SetMenuCheckInfo(&m_MenuInfos);

			if (m_pMenu != NULL) {
				delete m_pMenu;
				m_pMenu = NULL;
			}
			m_pMenu = new CMenuWnd();
			CDuiPoint point;
			::GetCursorPos(&point);
			m_pMenu->Init(NULL, _T("menu_1.xml"), point, &m_pm);
			isLevel_2_0 = false;
		}

		if (isLevel_2_1)
		{
			CMenuWnd::GetGlobalContextMenuObserver().SetMenuCheckInfo(&m_MenuInfos);

			if (m_pMenu != NULL) {
				delete m_pMenu;
				m_pMenu = NULL;
			}
			m_pMenu = new CMenuWnd();
			CDuiPoint point;
			::GetCursorPos(&point);
			m_pMenu->Init(NULL, _T("menu1_1.xml"), point, &m_pm);
			isLevel_2_1 = false;
		}

		if (isLevel_2_2)
		{
			CMenuWnd::GetGlobalContextMenuObserver().SetMenuCheckInfo(&m_MenuInfos);

			if (m_pMenu != NULL) {
				delete m_pMenu;
				m_pMenu = NULL;
			}
			m_pMenu = new CMenuWnd();
			CDuiPoint point;
			::GetCursorPos(&point);
			m_pMenu->Init(NULL, _T("menu2_1.xml"), point, &m_pm);
			isLevel_2_2 = false;
		}

		/*******************点击地面终端界面*****************/
		for (int i = 0; i < pNode_3_1_num; i++)
		{
			int childNum = pNode_3_1[i]->GetCountChild();
			if (pNode_3_1[i]->IsSelected())
			{
				m_modiftName_3 = pNode_3_1[i]->GetItemText();
				cout << "被选中了" << endl;
				selectNode_3[0][0] = i;
				selectNode_3[0][1] = -1;
				selectNode_3[0][2] = -1;
				if (i == 0)
				{
					isLevel_3_0 = true;
				}

				pNode_3_1[i]->Select(false);
			}
			else
			{
				for (int j = 0; j < childNum; j++)
				{

					CTreeNodeUI*  node = pNode_3_1[i]->GetChildNode(j);//GetParentNode
					if (node->IsSelected())
					{
						m_modiftName_3 = node->GetItemText();
						selectNode_3[0][0] = i;
						selectNode_3[0][1] = j;
						selectNode_3[0][2] = -1;
						cout << "被选中了!!!" << endl;
						if (i == 0)
						{
							//isLevel_3_1 = true;
						}

						node->Select(false);
					}

				}
			}
		}
		/************如果是0级节点被选中**************/
		if (isLevel_3_0)
		{
			CMenuWnd::GetGlobalContextMenuObserver().SetMenuCheckInfo(&m_MenuInfos);

			if (m_pMenu != NULL) {
				delete m_pMenu;
				m_pMenu = NULL;
			}
			m_pMenu = new CMenuWnd();
			CDuiPoint point;
			::GetCursorPos(&point);
			m_pMenu->Init(NULL, _T("menu_2.xml"), point, &m_pm);
			isLevel_3_0 = false;
		}

		//if (isLevel_3_1)
		//{
		//	CMenuWnd::GetGlobalContextMenuObserver().SetMenuCheckInfo(&m_MenuInfos);

		//	if (m_pMenu != NULL) {
		//		delete m_pMenu;
		//		m_pMenu = NULL;
		//	}
		//	m_pMenu = new CMenuWnd();
		//	CDuiPoint point;
		//	::GetCursorPos(&point);
		//	m_pMenu->Init(NULL, _T("menu1_2.xml"), point, &m_pm);
		//	isLevel_3_1 = false;
		//}


	}

	return WindowImplBase::Notify(msg);
}
unsigned __stdcall CPopEditorialClass::Busys(void* param)
{

	CPopEditorialClass * pWnd = (CPopEditorialClass*)param;
	::PostMessage(pWnd->m_hWnd, WM_CLOSE, 0, 0);
	return 0;
}


unsigned __stdcall CPopEditorialClass::ThreadReadtle(void* arglist)
{
	CPopEditorialClass * pMainWnd = (CPopEditorialClass*)arglist;
	unsigned  char m_DataBuffer[1024] = { 0 };
	int m_DataLength = 0;
	int Order = 0;
	pMainWnd->Udprecvtle.InitServer(22001);

	while (1)
	{
		pMainWnd->Udprecvtle.UDPsRecv((char*)m_DataBuffer,1024);
		// 打开命名共享内存对象
		HANDLE hMapFile = OpenFileMapping(
			FILE_MAP_ALL_ACCESS,
			FALSE,
			L"MySharedMemory"); // 使用与发送进程相同的名称

		if (hMapFile == NULL) {
			std::cerr << "OpenFileMapping failed: " << GetLastError() << std::endl;
			Sleep(100);
			continue;
		}

		// 获取共享内存指针
		LPVOID pBuf = MapViewOfFile(hMapFile, FILE_MAP_READ, 0, 0, sizeof(char) * 200000);

		if (pBuf == NULL) {
			std::cerr << "MapViewOfFile failed: " << GetLastError() << std::endl;
			CloseHandle(hMapFile);
			Sleep(100);
			continue;
		}

		const char* receivedMessage = static_cast<char*>(pBuf);
		string myString(receivedMessage);

		//pMainWnd->Udprecvtle.UDPsSend((char*)m_DataBuffer,100);
		// 清理资源
		UnmapViewOfFile(pBuf);
		CloseHandle(hMapFile);

		// 解析JSON字符串
		Json sceneJsona = Json::parse(myString);

		// 将JSON转换为SceneTLE结构体
		SceneTLE scene;
		scene.Scenename = sceneJsona["Scenename"];

		for (auto it = sceneJsona["SceneTLEdata"].begin(); it != sceneJsona["SceneTLEdata"].end(); ++it) {
			ParameterTLE param;
			param.Parametername = it.value()["Parametername"].get<std::vector<std::string>>();;

			for (const auto& data : it.value()["Parameterdata"]) {
				param.Parameterdata.push_back(data);
			}

			scene.SceneTLEdata[it.key()] = param;
		}

		
		for (auto& it : scene.SceneTLEdata)
		{

			string::size_type idx = UTF8_To_string(it.first).find("#$");
			if (idx != string::npos)
			{
				pMainWnd->CreateTreeNode_livel1(UTF8_To_string(it.second.Parametername[0]));
				pMainWnd->m_RecvMap.push_back(it.second.Parameterdata[0]);
				Order++;
			}
			else {
				pMainWnd->CreateTreeNode_livel1(UTF8_To_string(it.first));

				for (int i = 0; i < it.second.Parametername.size(); i++)
				{
					pMainWnd->AssignmentToLocalVector(UTF8_To_string(it.second.Parametername[i]), it.second.Parameterdata[i], Order);
				}
				Order++;
			}
			
			
		}
	}
	return 0;
}
//初始化
void CPopEditorialClass::InitWindow()
{
	m_pCloseBtn = static_cast<CButtonUI*>(m_pm.FindControl(_T("closebtn")));
	pTabDatabase = static_cast<CTabLayoutUI*>(m_pm.FindControl(_T("tab_switch")));
	

	m_treeView = static_cast<CTreeViewUI*>(m_pm.FindControl(_T("treeview1")));
	m_treeView->SetAttribute(L"itemhotimage", L"ui/文件夹选中.png");
	m_treeView->SetAttribute(L"itemselectedbkcolor", L"0xFF808080");

	m_treeView_2 = static_cast<CTreeViewUI*>(m_pm.FindControl(_T("treeview2")));
	m_treeView_2->SetAttribute(L"itemhotimage", L"ui/文件夹选中.png");
	m_treeView_2->SetAttribute(L"itemselectedbkcolor", L"0xFF808080");
	
	m_treeView_3 = static_cast<CTreeViewUI*>(m_pm.FindControl(_T("treeview3")));
	m_treeView_3->SetAttribute(L"itemhotimage", L"ui/文件夹选中.png");
	m_treeView_3->SetAttribute(L"itemselectedbkcolor", L"0xFF808080");

	pNode_1_num = 0;//当前根节点在树中的顺序
	pNode_2_num = 0;//1层节点在树中的顺序
	pNode_3_num = 0;//2层节点在树中的顺序
	pNode_4_num = 0;//3层节点在树中的顺序

	pNode_2_1_num = 0;//当前根节点在树中的顺序
	pNode_2_2_num = 0;//一级节点在树中的顺序
	pNode_2_3_num = 0;//二级节点在树中的顺序
	pNode_2_4_num = 0;//三级节点在树中的顺序


	pNode_3_1_num = 0;//当前根节点在树中的顺序
	pNode_3_2_num = 0;//一级节点在树中的顺序
	pNode_3_3_num = 0;//二级节点在树中的顺序
	pNode_3_4_num = 0;//三级节点在树中的顺序

	isLevel0 = false;//选中的是0级节点那一层
    isLevel1 = false;//选中的是1级节点那一层
	isLevel2 = false;//选中的是2级节点那一层

	isLevel_2_0 = false;//选中的是0级节点那一层
	isLevel_2_1 = false;//选中的是1级节点那一层
	isLevel_2_2 = false;//选中的是2级节点那一层

	isLevel_3_0 = false;;//选中的是0级节点那一层
	isLevel_3_1 = false;;//选中的是1级节点那一层
	
	tab3RootType = 0;//tab3界面存放类型的根节点格式
	RailTypeFlag = false;
	FunctionalCategoryFlag = false;//功能分类 FunctionalCategory
	PayloadFlag = false;//载荷类型分类

	rootNum = 0;
	selectNode[0][0] = -1;
	selectNode[0][1] = -1;
	selectNode[0][2] = -1;
	selectNode[0][3] = -1;


	CreateTreeNode(UTF8_To_string("星座卫星管理"));//向根节点加名称

	///********************卫星界面的卫星和星座********************/

	CreateTreeNode_2(UTF8_To_string("电信港地球站管理"));//向第二个界面增加根节点
	
	//AssignmentToLocalVector_DXG(UTF8_To_string("电信港2"));//向树的以及目录中添加数据
	/**********************界面3场景-地面终端******************/
	CreateTreeNode_3(UTF8_To_string("地面终端管理"));//向第二个界面增加根节点

	//AssignmentToLocalVector_DMZD_level2(UTF8_To_string("地面终端2"), UTF8_To_string("天线属性信息"), map3_3_3);
	/**********************************************/
	CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ThreadReadtle, this, 0, NULL);
}



void CPopEditorialClass::CreateTreeNode()
{
	///****************************创建网络运营商*****************************/
	pNode_1[pNode_1_num] = new CTreeNodeUI();
	pNode_1[pNode_1_num]->CheckBoxSelected(false);
	pNode_1[pNode_1_num]->SetFixedHeight(30);
	pNode_1[pNode_1_num]->SetItemTextColor(0xFFFFFFFF);
	pNode_1[pNode_1_num]->SetItemHotTextColor(0xFFFFFFFF);
	pNode_1[pNode_1_num]->SetSelItemTextColor(0xFFFFFFFF);
	pNode_1[pNode_1_num]->SetAttribute(_T("folderattr"), _T("padding=&quot;0,1,0,0&quot; width=&quot;18&quot; height=&quot;18&quot; normalimage=&quot;file='ui/展开.png' &quot; "));
	pNode_1[pNode_1_num]->SetAttribute(_T("folderattr"), _T("hotimage=&quot;file='ui/展开.png' &quot; selectedimage=&quot;file='ui/未展开.png'&quot; selectedhotimage=&quot;file='ui/未展开.png'&quot;"));
	pNode_1[pNode_1_num]->SetAttribute(_T("itemattr"), _T("padding=\"16,0,0,0\" align=\"left\" font=\"13\""));//font=&quot;6&quot;
	pNode_1[pNode_1_num]->SetItemText(_T("卫星"));

	/**********************************************/
	m_treeView->Add(pNode_1[pNode_1_num]);			//要先添加父节点,，在添加子节点和样式	
	int TreeNodeItem_Number = 0;
	///****************A********************/
	pNode_2[pNode_2_num] = new CTreeNodeUI();
	pNode_2[pNode_2_num]->CheckBoxSelected(false);
	pNode_2[pNode_2_num]->SetItemText(_T("卫星名称1"));
	pNode_2[pNode_2_num]->SetFixedHeight(30);
	pNode_2[pNode_2_num]->SetItemTextColor(0xFFFFFFFF);
	pNode_2[pNode_2_num]->SetItemHotTextColor(0xFFFFFFFF);
	pNode_2[pNode_2_num]->SetSelItemTextColor(0xFFFFFFFF);
	pNode_2[pNode_2_num]->SetAttribute(_T("folderattr"), _T("padding=&quot;0,1,0,0&quot; width=&quot;16&quot; height=&quot;16&quot; normalimage=&quot;file='ui/展开.png' &quot; "));
	pNode_2[pNode_2_num]->SetAttribute(_T("folderattr"), _T("hotimage=&quot;file='ui/展开.png' &quot; selectedimage=&quot;file='ui/未展开.png'&quot; selectedhotimage=&quot;file='ui/未展开.png'&quot;"));
	pNode_2[pNode_2_num]->SetAttribute(_T("itemattr"), _T("padding=\"16,0,0,0\" align=\"left\" font=\"13\""));//font=&quot;6&quot;
	pNode_2[pNode_2_num]->SetName(_T("WX1"));
	pNode_1[pNode_1_num]->AddChildNode(pNode_2[pNode_2_num]);

}
void CPopEditorialClass::CreateTreeNode(string tableName, string str)
{
	int sequences;//存放根节点的序号
	for (int i = 0; i < vec_root.size(); i++)
	{
		if (tableName == vec_root[i])
		{
			sequences = i;
		}
	}

	pNode_2[pNode_2_num] = new CTreeNodeUI();
	pNode_2[pNode_2_num]->CheckBoxSelected(false);
	pNode_2[pNode_2_num]->SetItemText(stringToWstring(str).c_str());
	pNode_2[pNode_2_num]->SetFixedHeight(30);
	pNode_2[pNode_2_num]->SetItemTextColor(0xFFFFFFFF);
	pNode_2[pNode_2_num]->SetItemHotTextColor(0xFFFFFFFF);
	pNode_2[pNode_2_num]->SetSelItemTextColor(0xFFFFFFFF);
	pNode_2[pNode_2_num]->SetAttribute(_T("folderattr"), _T("padding=&quot;0,1,0,0&quot; width=&quot;16&quot; height=&quot;16&quot; normalimage=&quot;file='ui/展开.png' &quot; "));
	pNode_2[pNode_2_num]->SetAttribute(_T("folderattr"), _T("hotimage=&quot;file='ui/展开.png' &quot; selectedimage=&quot;file='ui/未展开.png'&quot; selectedhotimage=&quot;file='ui/未展开.png'&quot;"));
	pNode_2[pNode_2_num]->SetAttribute(_T("itemattr"), _T("padding=\"16,0,0,0\" align=\"left\" font=\"13\""));//font=&quot;6&quot;
	pNode_2[pNode_2_num]->SetName(_T("WX1"));
	pNode_1[0]->AddChildNode(pNode_2[pNode_2_num]);
	pNode_2_num += 1;


}
//创建星座
void  CPopEditorialClass::CreateTreeNode_livel1(string tableName)
{
	pNode_2[pNode_2_num] = new CTreeNodeUI();
	pNode_2[pNode_2_num]->CheckBoxSelected(false);
	pNode_2[pNode_2_num]->SetItemText(stringToWstring(tableName).c_str());
	pNode_2[pNode_2_num]->SetFixedHeight(30);
	pNode_2[pNode_2_num]->SetItemTextColor(0xFFFFFFFF);
	pNode_2[pNode_2_num]->SetItemHotTextColor(0xFFFFFFFF);
	pNode_2[pNode_2_num]->SetSelItemTextColor(0xFFFFFFFF);
	pNode_2[pNode_2_num]->SetAttribute(_T("folderattr"), _T("padding=&quot;0,1,0,0&quot; width=&quot;16&quot; height=&quot;16&quot; normalimage=&quot;file='ui/展开.png' &quot; "));
	pNode_2[pNode_2_num]->SetAttribute(_T("folderattr"), _T("hotimage=&quot;file='ui/展开.png' &quot; selectedimage=&quot;file='ui/未展开.png'&quot; selectedhotimage=&quot;file='ui/未展开.png'&quot;"));
	pNode_2[pNode_2_num]->SetAttribute(_T("itemattr"), _T("padding=\"16,0,0,0\" align=\"left\" font=\"13\""));//font=&quot;6&quot;
	pNode_2[pNode_2_num]->SetName(_T("WX1"));
	pNode_1[0]->AddChildNode(pNode_2[pNode_2_num]);
	pNode_2_num += 1;
}
//创建卫星
void CPopEditorialClass::CreateTreeNode_livel2(string tableName,int first_level)
{
	pNode_3[pNode_3_num] = new CTreeNodeUI();
	pNode_3[pNode_3_num]->CheckBoxSelected(false);
	pNode_3[pNode_3_num]->SetItemText(stringToWstring(tableName).c_str());
	pNode_3[pNode_3_num]->SetFixedHeight(30);
	pNode_3[pNode_3_num]->SetItemTextColor(0xFFFFFFFF);
	pNode_3[pNode_3_num]->SetItemHotTextColor(0xFFFFFFFF);
	pNode_3[pNode_3_num]->SetSelItemTextColor(0xFFFFFFFF);
	pNode_3[pNode_3_num]->SetAttribute(_T("folderattr"), _T("padding=&quot;0,1,0,0&quot; width=&quot;16&quot; height=&quot;16&quot; normalimage=&quot;file='ui/展开.png' &quot; "));
	pNode_3[pNode_3_num]->SetAttribute(_T("folderattr"), _T("hotimage=&quot;file='ui/展开.png' &quot; selectedimage=&quot;file='ui/未展开.png'&quot; selectedhotimage=&quot;file='ui/未展开.png'&quot;"));
	pNode_3[pNode_3_num]->SetAttribute(_T("itemattr"), _T("padding=\"16,0,0,0\" align=\"left\" font=\"13\""));//font=&quot;6&quot;
	pNode_3[pNode_3_num]->SetName(_T("WX1"));
	pNode_2[first_level]->AddChildNode(pNode_3[pNode_3_num]);
	pNode_3_num += 1;
}
//创建三级节点
void CPopEditorialClass::CreateTreeNode_livel3(string tableName, int first_level)
{
	pNode_4[pNode_4_num] = new CTreeNodeUI();
	pNode_4[pNode_4_num]->CheckBoxSelected(false);
	pNode_4[pNode_4_num]->SetItemText(stringToWstring(tableName).c_str());
	pNode_4[pNode_4_num]->SetFixedHeight(30);
	pNode_4[pNode_4_num]->SetFixedWidth(100);
	pNode_4[pNode_4_num]->SetItemTextColor(0xFFFFFFFF);
	pNode_4[pNode_4_num]->SetItemHotTextColor(0xFFFFFFFF);
	pNode_4[pNode_4_num]->SetSelItemTextColor(0xFFFFFFFF);
	pNode_4[pNode_4_num]->SetAttribute(_T("folderattr"), _T("padding=&quot;0,1,0,0&quot; width=&quot;16&quot; height=&quot;16&quot; normalimage=&quot;file='ui/展开.png' &quot; "));
	pNode_4[pNode_4_num]->SetAttribute(_T("folderattr"), _T("hotimage=&quot;file='ui/展开.png' &quot; selectedimage=&quot;file='ui/未展开.png'&quot; selectedhotimage=&quot;file='ui/未展开.png'&quot;"));
	pNode_4[pNode_4_num]->SetAttribute(_T("itemattr"), _T("padding=\"16,0,0,0\" align=\"left\" font=\"13\""));//font=&quot;6&quot;
	pNode_4[pNode_4_num]->SetName(_T("WX1"));
	pNode_3[first_level]->AddChildNode(pNode_4[pNode_4_num]);
	pNode_4_num += 1;
}
//创建一级节点-----------电信港地球站界面
void CPopEditorialClass::CreateTreeNode_livel2_1(string tableName)
{
	pNode_2_2[pNode_2_2_num] = new CTreeNodeUI();
	pNode_2_2[pNode_2_2_num]->CheckBoxSelected(false);
	pNode_2_2[pNode_2_2_num]->SetItemText(stringToWstring(tableName).c_str());
	pNode_2_2[pNode_2_2_num]->SetFixedHeight(30);
	pNode_2_2[pNode_2_2_num]->SetItemTextColor(0xFFFFFFFF);
	pNode_2_2[pNode_2_2_num]->SetItemHotTextColor(0xFFFFFFFF);
	pNode_2_2[pNode_2_2_num]->SetSelItemTextColor(0xFFFFFFFF);
	pNode_2_2[pNode_2_2_num]->SetAttribute(_T("folderattr"), _T("padding=&quot;0,1,0,0&quot; width=&quot;16&quot; height=&quot;16&quot; normalimage=&quot;file='ui/展开.png' &quot; "));
	pNode_2_2[pNode_2_2_num]->SetAttribute(_T("folderattr"), _T("hotimage=&quot;file='ui/展开.png' &quot; selectedimage=&quot;file='ui/未展开.png'&quot; selectedhotimage=&quot;file='ui/未展开.png'&quot;"));
	pNode_2_2[pNode_2_2_num]->SetAttribute(_T("itemattr"), _T("padding=\"16,0,0,0\" align=\"left\" font=\"13\""));//font=&quot;6&quot;
	pNode_2_2[pNode_2_2_num]->SetName(_T("WX1"));
	pNode_2_1[0]->AddChildNode(pNode_2_2[pNode_2_2_num]);
	pNode_2_2_num += 1;
}
//创建二级节点--------------电信港地球站界面
void CPopEditorialClass::CreateTreeNode_livel2_2(string tableName, int first_level)
{
	pNode_2_3[pNode_2_3_num] = new CTreeNodeUI();
	pNode_2_3[pNode_2_3_num]->CheckBoxSelected(false);
	pNode_2_3[pNode_2_3_num]->SetItemText(stringToWstring(tableName).c_str());
	pNode_2_3[pNode_2_3_num]->SetFixedHeight(30);
	pNode_2_3[pNode_2_3_num]->SetItemTextColor(0xFFFFFFFF);
	pNode_2_3[pNode_2_3_num]->SetItemHotTextColor(0xFFFFFFFF);
	pNode_2_3[pNode_2_3_num]->SetSelItemTextColor(0xFFFFFFFF);
	pNode_2_3[pNode_2_3_num]->SetAttribute(_T("folderattr"), _T("padding=&quot;0,1,0,0&quot; width=&quot;16&quot; height=&quot;16&quot; normalimage=&quot;file='ui/展开.png' &quot; "));
	pNode_2_3[pNode_2_3_num]->SetAttribute(_T("folderattr"), _T("hotimage=&quot;file='ui/展开.png' &quot; selectedimage=&quot;file='ui/未展开.png'&quot; selectedhotimage=&quot;file='ui/未展开.png'&quot;"));
	pNode_2_3[pNode_2_3_num]->SetAttribute(_T("itemattr"), _T("padding=\"16,0,0,0\" align=\"left\" font=\"13\""));//font=&quot;6&quot;
	pNode_2_3[pNode_2_3_num]->SetName(_T("WX1"));
	pNode_2_2[first_level]->AddChildNode(pNode_2_3[pNode_2_3_num]);
	pNode_2_3_num += 1;
}
//创建三级节点 firstlevel是指
void CPopEditorialClass::CreateTreeNode_livel2_3(string tableName, int first_level)
{
	pNode_2_4[pNode_2_4_num] = new CTreeNodeUI();
	pNode_2_4[pNode_2_4_num]->CheckBoxSelected(false);
	pNode_2_4[pNode_2_4_num]->SetItemText(stringToWstring(tableName).c_str());
	pNode_2_4[pNode_2_4_num]->SetFixedHeight(30);
	pNode_2_4[pNode_2_4_num]->SetFixedWidth(100);
	pNode_2_4[pNode_2_4_num]->SetItemTextColor(0xFFFFFFFF);
	pNode_2_4[pNode_2_4_num]->SetItemHotTextColor(0xFFFFFFFF);
	pNode_2_4[pNode_2_4_num]->SetSelItemTextColor(0xFFFFFFFF);
	pNode_2_4[pNode_2_4_num]->SetAttribute(_T("folderattr"), _T("padding=&quot;0,1,0,0&quot; width=&quot;16&quot; height=&quot;16&quot; normalimage=&quot;file='ui/展开.png' &quot; "));
	pNode_2_4[pNode_2_4_num]->SetAttribute(_T("folderattr"), _T("hotimage=&quot;file='ui/展开.png' &quot; selectedimage=&quot;file='ui/未展开.png'&quot; selectedhotimage=&quot;file='ui/未展开.png'&quot;"));
	pNode_2_4[pNode_2_4_num]->SetAttribute(_T("itemattr"), _T("padding=\"16,0,0,0\" align=\"left\" font=\"13\""));//font=&quot;6&quot;
	pNode_2_4[pNode_2_4_num]->SetName(_T("WX1"));
	pNode_2_3[first_level]->AddChildNode(pNode_2_4[pNode_2_4_num]);
	pNode_2_4_num += 1;
}
//创建一级节点-----------地面终端
void CPopEditorialClass::CreateTreeNode_livel3_1(string tableName)
{
	pNode_3_2[pNode_3_2_num] = new CTreeNodeUI();
	pNode_3_2[pNode_3_2_num]->CheckBoxSelected(false);
	pNode_3_2[pNode_3_2_num]->SetItemText(stringToWstring(tableName).c_str());
	pNode_3_2[pNode_3_2_num]->SetFixedHeight(30);
	pNode_3_2[pNode_3_2_num]->SetItemTextColor(0xFFFFFFFF);
	pNode_3_2[pNode_3_2_num]->SetItemHotTextColor(0xFFFFFFFF);
	pNode_3_2[pNode_3_2_num]->SetSelItemTextColor(0xFFFFFFFF);
	pNode_3_2[pNode_3_2_num]->SetAttribute(_T("folderattr"), _T("padding=&quot;0,1,0,0&quot; width=&quot;16&quot; height=&quot;16&quot; normalimage=&quot;file='ui/展开.png' &quot; "));
	pNode_3_2[pNode_3_2_num]->SetAttribute(_T("folderattr"), _T("hotimage=&quot;file='ui/展开.png' &quot; selectedimage=&quot;file='ui/未展开.png'&quot; selectedhotimage=&quot;file='ui/未展开.png'&quot;"));
	pNode_3_2[pNode_3_2_num]->SetAttribute(_T("itemattr"), _T("padding=\"16,0,0,0\" align=\"left\" font=\"13\""));//font=&quot;6&quot;
	pNode_3_2[pNode_3_2_num]->SetName(_T("WX1"));
	pNode_3_1[0]->AddChildNode(pNode_3_2[pNode_3_2_num]);
	pNode_3_2_num += 1;
}
//创建二级节点-----------地面终端子节点
void CPopEditorialClass::CreateTreeNode_livel3_2(string tableName, int first_level)
{
	pNode_3_3[pNode_3_3_num] = new CTreeNodeUI();
	pNode_3_3[pNode_3_3_num]->CheckBoxSelected(false);
	pNode_3_3[pNode_3_3_num]->SetItemText(stringToWstring(tableName).c_str());
	pNode_3_3[pNode_3_3_num]->SetFixedHeight(30);
	pNode_3_3[pNode_3_3_num]->SetItemTextColor(0xFFFFFFFF);
	pNode_3_3[pNode_3_3_num]->SetItemHotTextColor(0xFFFFFFFF);
	pNode_3_3[pNode_3_3_num]->SetSelItemTextColor(0xFFFFFFFF);
	pNode_3_3[pNode_3_3_num]->SetAttribute(_T("folderattr"), _T("padding=&quot;0,1,0,0&quot; width=&quot;16&quot; height=&quot;16&quot; normalimage=&quot;file='ui/展开.png' &quot; "));
	pNode_3_3[pNode_3_3_num]->SetAttribute(_T("folderattr"), _T("hotimage=&quot;file='ui/展开.png' &quot; selectedimage=&quot;file='ui/未展开.png'&quot; selectedhotimage=&quot;file='ui/未展开.png'&quot;"));
	pNode_3_3[pNode_3_3_num]->SetAttribute(_T("itemattr"), _T("padding=\"16,0,0,0\" align=\"left\" font=\"13\""));//font=&quot;6&quot;
	pNode_3_3[pNode_3_3_num]->SetName(_T("WX1"));
	pNode_3_2[first_level]->AddChildNode(pNode_3_3[pNode_3_3_num]);
	pNode_3_3_num += 1;
}
//创建根节点
void CPopEditorialClass::CreateTreeNode(string tableName)
{
	pNode_1[pNode_1_num] = new CTreeNodeUI();
	pNode_1[pNode_1_num]->CheckBoxSelected(false);
	pNode_1[pNode_1_num]->SetFixedHeight(30);
	pNode_1[pNode_1_num]->SetItemTextColor(0xFFFFFFFF);
	pNode_1[pNode_1_num]->SetItemHotTextColor(0xFFFFFFFF);
	pNode_1[pNode_1_num]->SetSelItemTextColor(0xFFFFFFFF);
	pNode_1[pNode_1_num]->SetAttribute(_T("folderattr"), _T("padding=&quot;0,1,0,0&quot; width=&quot;18&quot; height=&quot;18&quot; normalimage=&quot;file='ui/展开.png' &quot; "));
	pNode_1[pNode_1_num]->SetAttribute(_T("folderattr"), _T("hotimage=&quot;file='ui/展开.png' &quot; selectedimage=&quot;file='ui/未展开.png'&quot; selectedhotimage=&quot;file='ui/未展开.png'&quot;"));
	pNode_1[pNode_1_num]->SetAttribute(_T("itemattr"), _T("padding=\"16,0,0,0\" align=\"left\" font=\"13\""));//font=&quot;6&quot;
	pNode_1[pNode_1_num]->SetItemText(stringToWstring(tableName).c_str());
	m_treeView->Add(pNode_1[pNode_1_num]);			//要先添加父节点,，在添加子节点和样式	
	
	vec_root.push_back(tableName);
	pNode_1_num += 1;

}
//创建第二个界面的根节点
void CPopEditorialClass::CreateTreeNode_2(string tableName)
{
	pNode_2_1[pNode_2_1_num] = new CTreeNodeUI();
	pNode_2_1[pNode_2_1_num]->CheckBoxSelected(false);
	pNode_2_1[pNode_2_1_num]->SetFixedHeight(30);
	pNode_2_1[pNode_2_1_num]->SetItemTextColor(0xFFFFFFFF);
	pNode_2_1[pNode_2_1_num]->SetItemHotTextColor(0xFFFFFFFF);
	pNode_2_1[pNode_2_1_num]->SetSelItemTextColor(0xFFFFFFFF);
	pNode_2_1[pNode_2_1_num]->SetAttribute(_T("folderattr"), _T("padding=&quot;0,1,0,0&quot; width=&quot;18&quot; height=&quot;18&quot; normalimage=&quot;file='ui/展开.png' &quot; "));
	pNode_2_1[pNode_2_1_num]->SetAttribute(_T("folderattr"), _T("hotimage=&quot;file='ui/展开.png' &quot; selectedimage=&quot;file='ui/未展开.png'&quot; selectedhotimage=&quot;file='ui/未展开.png'&quot;"));
	pNode_2_1[pNode_2_1_num]->SetAttribute(_T("itemattr"), _T("padding=\"16,0,0,0\" align=\"left\" font=\"13\""));//font=&quot;6&quot;
	pNode_2_1[pNode_2_1_num]->SetItemText(stringToWstring(tableName).c_str());
	m_treeView_2->Add(pNode_2_1[pNode_2_1_num]);			//要先添加父节点,，在添加子节点和样式	

	vec_2_root.push_back(tableName);
	pNode_2_1_num += 1;
}
//创建第三个界面的根节点
void CPopEditorialClass::CreateTreeNode_3(string tableName)
{
	pNode_3_1[pNode_3_1_num] = new CTreeNodeUI();
	pNode_3_1[pNode_3_1_num]->CheckBoxSelected(false);
	pNode_3_1[pNode_3_1_num]->SetFixedHeight(30);
	pNode_3_1[pNode_3_1_num]->SetItemTextColor(0xFFFFFFFF);
	pNode_3_1[pNode_3_1_num]->SetItemHotTextColor(0xFFFFFFFF);
	pNode_3_1[pNode_3_1_num]->SetSelItemTextColor(0xFFFFFFFF);
	pNode_3_1[pNode_3_1_num]->SetAttribute(_T("folderattr"), _T("padding=&quot;0,1,0,0&quot; width=&quot;18&quot; height=&quot;18&quot; normalimage=&quot;file='ui/展开.png' &quot; "));
	pNode_3_1[pNode_3_1_num]->SetAttribute(_T("folderattr"), _T("hotimage=&quot;file='ui/展开.png' &quot; selectedimage=&quot;file='ui/未展开.png'&quot; selectedhotimage=&quot;file='ui/未展开.png'&quot;"));
	pNode_3_1[pNode_3_1_num]->SetAttribute(_T("itemattr"), _T("padding=\"16,0,0,0\" align=\"left\" font=\"13\""));//font=&quot;6&quot;
	pNode_3_1[pNode_3_1_num]->SetItemText(stringToWstring(tableName).c_str());
	m_treeView_3->Add(pNode_3_1[pNode_3_1_num]);			//要先添加父节点,，在添加子节点和样式	

	vec_3_root.push_back(tableName);
	pNode_3_1_num += 1;
}
//创建一级目录的树节点 --------星座
void CPopEditorialClass::AssignmentToLocalVector_XZ(string nodeName)
{
	map<string, string> m_map = GetHoroscopeInform();

	CreateTreeNode_livel1(nodeName);
	name_Level1.push_back(nodeName);
	m_RecvMap_XZ.push_back(m_map);
	vec_ExpandFlags.push_back(FALSE);

	string path1 = "D:\\DATA1";
	if (0 != _access(path1.c_str(), 0))
	{
		_mkdir(path1.c_str());   // 返回 0 表示创建成功，-1 表示失败
	}
	string logdata = path1 + "\\" + nodeName + ".txt";

	ofstream fout(logdata, ios::out);
	for (map<string, string>::iterator iter = m_map.begin(); iter != m_map.end(); ++iter)
	{
		cout << "key:" << iter->first << " value:" << iter->second << endl;
		string str_first = UTF8_To_string(iter->first);
		string str_second = UTF8_To_string(iter->second);
		string str = str_first + ":";
		str += str_second;
		fout << str << endl;
	}

	fout.close();

}
//创建一级目录的树节点 --------星座
void CPopEditorialClass::AssignmentToLocalVector_XZ(string nodeName, map<string, string> m_map)
{
//	map<string, string> m_map = GetHoroscopeInform();

	CreateTreeNode_livel1(nodeName);
	name_Level1.push_back(nodeName);
	m_RecvMap_XZ.push_back(m_map);
	vec_ExpandFlags.push_back(FALSE);
}
//创建二级目录树节点  --------卫星
void CPopEditorialClass::AssignmentToLocalVector(string nodeName, map<string, string> m_map,int first_level)
{
	CreateTreeNode_livel2(nodeName, first_level);
	name_Level2.push_back(nodeName);
	m_RecvMap.push_back(m_map);
}
//创建三级目录的树节点 --------天线属性信息 载荷属性信息
void CPopEditorialClass::AssignmentToLocalVector_WXSX(string wx_name,string type,map<string, string> m_map)
{
	int sequence = -1;
	for (int i = 0; i < name_Level2.size(); i++)
	{
		if (wx_name == name_Level2[i])
		{
			sequence = i;
		}
	}
	string level3Name = wx_name + type;
	CreateTreeNode_livel3(level3Name, sequence);
	name_Level3.push_back(level3Name);
	m_RecvMap_TxZh.push_back(m_map);
}
//创建二级目录的树节点 --------卫星 导入的时候
void CPopEditorialClass::AssignmentToLocalVector(string parent, string nodeName, map<string, string> m_map)
{
	int sequence;
	for (int i = 0; i < name_Level1.size(); i++)
	{
		if (name_Level1[i] == parent)
		{
			sequence = i;
		}
		
	}
	CreateTreeNode_livel2(nodeName, sequence);
	name_Level2.push_back(nodeName);
	m_RecvMap.push_back(m_map);
}
//创建三级目录的树节点 导入的时候--------天线属性信息 载荷属性信息
void CPopEditorialClass::AssignmentToLocalVector_TxZh(string parent, string nodeName, map<string, string> m_map)
{
	int sequence;
	for (int i = 0; i < name_Level2.size(); i++)
	{
		if (name_Level2[i] == parent)
		{
			sequence = i;
		}

	}
	CreateTreeNode_livel3(nodeName, sequence);
	name_Level3.push_back(nodeName);
	m_RecvMap_TxZh.push_back(m_map);
}

// 创建一级目录的树节点--------电信港
void CPopEditorialClass::AssignmentToLocalVector_DXG(string nodeName)
{
	map<string, string> m_map = GetTeleport();
	CreateTreeNode_livel2_1(nodeName);
	name2_Level1.push_back(nodeName);
	m_RecvMap_DXG.push_back(m_map);

	vec_ExpandFlags2_FT.push_back(FALSE);//存放电信港是否进行了功能类型筛选
	vec_ExpandFlags2_ES.push_back(FALSE);//存放电信港是否进行了地球站类型筛选

	string path1 = "D:\\DATA1";
	if (0 != _access(path1.c_str(), 0))
	{
		_mkdir(path1.c_str());   // 返回 0 表示创建成功，-1 表示失败
	}
	string logdata = path1 + "\\" + nodeName + ".txt";

	ofstream fout(logdata, ios::out);
	for (map<string, string>::iterator iter = m_map.begin(); iter != m_map.end(); ++iter)
	{
		cout << "key:" << iter->first << " value:" << iter->second << endl;
		string str_first = UTF8_To_string(iter->first);
		string str_second = UTF8_To_string(iter->second);
		string str = str_first + ":";
		str += str_second;
		fout << str << endl;
	}

	fout.close();
}

void CPopEditorialClass::AssignmentToLocalVector_DXG(string nodeName, map<string, string> m_map)
{
	CreateTreeNode_livel2_1(nodeName);
	name2_Level1.push_back(nodeName);
	m_RecvMap_DXG.push_back(m_map);

	vec_ExpandFlags2_FT.push_back(FALSE);//存放电信港是否进行了功能类型筛选
	vec_ExpandFlags2_ES.push_back(FALSE);//存放电信港是否进行了地球站类型筛选
}
////创建二级目录的树节点 --------地球站
void CPopEditorialClass::AssignmentToLocalVector_DQZ(string nodeName, map<string, string> m_map, int first_level)
{
	CreateTreeNode_livel2_2(nodeName, first_level);
	name2_Level2.push_back(nodeName);
	m_RecvMap_DQZ.push_back(m_map);

	string path1 = "D:\\DATA1";
	if (0 != _access(path1.c_str(), 0))
	{
		_mkdir(path1.c_str());   // 返回 0 表示创建成功，-1 表示失败
	}
	string logdata = path1 + "\\" + nodeName + ".txt";

	ofstream fout(logdata, ios::out);
	for (map<string, string>::iterator iter = m_map.begin(); iter != m_map.end(); ++iter)
	{
		cout << "key:" << iter->first << " value:" << iter->second << endl;
		string str_first = UTF8_To_string(iter->first);
		string str_second = UTF8_To_string(iter->second);
		string str = str_first + ":";
		str += str_second;
		fout << str << endl;
	}

	fout.close();

}
////创建二级目录的树节点 --------地球站 导入的时候
void CPopEditorialClass::AssignmentToLocalVector_DQZ(string parent, string nodeName, map<string, string> m_map)
{
	int sequence;
	for (int i = 0; i < name2_Level1.size(); i++)
	{
		if (name2_Level1[i] == parent)
		{
			sequence = i;
		}

	}

	CreateTreeNode_livel2_2(nodeName, sequence);
	name2_Level2.push_back(nodeName);
	m_RecvMap_DQZ.push_back(m_map);
}

//创建二级目录的树节点 --------地球站
void CPopEditorialClass::AssignmentToLocalVector_DQZ_level2(string parent, string node, map<string, string> m_map)
{
	int sequence = -1;
	for (int i = 0; i < name2_Level2.size(); i++)
	{
		if (name2_Level2[i] == parent)
		{
			sequence = i;
		}

	}
	string level3Name = parent + node;
	CreateTreeNode_livel2_3(level3Name, sequence);
	name2_Level3.push_back(level3Name);
	m_RecvMap_DQZChild.push_back(m_map);
}
//创建三级目录的树节点 导入的时候--------馈源波形天线
void CPopEditorialClass::AssignmentToLocalVector_DQZ_level3(string parent, string nodeName, map<string, string> m_map)
{
	int sequence = -1;
	for (int i = 0; i < name2_Level2.size(); i++)
	{
		if (name2_Level2[i] == parent)
		{
			sequence = i;
		}

	}

	CreateTreeNode_livel2_3(nodeName, sequence);
	name2_Level3.push_back(nodeName);
	m_RecvMap_DQZChild.push_back(m_map);
}
//创建一级目录的树节点 --------地面终端
void CPopEditorialClass::AssignmentToLocalVector_DMZD(string nodeName, map<string, string> m_map)
{
	CreateTreeNode_livel3_1(nodeName);
	name3_Level1.push_back(nodeName);
	m_RecvMap_DMZD.push_back(m_map);
}
//创建二级目录的树节点 --------地面终端子节点
void CPopEditorialClass::AssignmentToLocalVector_DMZD_level2(string parent, string type, map<string, string> m_map)
{
	int sequence = -1;
	for (int i = 0; i < name3_Level1.size(); i++)
	{
		if (name3_Level1[i] == parent)
		{
			sequence = i;
		}

	}
	string name = parent + type;
	CreateTreeNode_livel3_2(name,sequence);
	name3_Level2.push_back(name);
	m_RecvMap_DMZDChild.push_back(m_map);
}
//创建二级目录的树节点 --------载荷波形天线 导入的时候
void CPopEditorialClass::AssignmentToLocalVector_DMZD_Child(string parent, string nodeName, map<string, string> m_map)
{
	int sequence = -1;
	for (int i = 0; i < name3_Level1.size(); i++)
	{
		if (name3_Level1[i] == parent)
		{
			sequence = i;
		}

	}
	
	CreateTreeNode_livel3_2(nodeName, sequence);
	name3_Level2.push_back(nodeName);
	m_RecvMap_DMZDChild.push_back(m_map);
}
//获取0级节点子节点的数量------获取一级节点数量 根节点的子节点
void CPopEditorialClass::GetNumberChild_0()
{
	for (int i = 0; i < pNode_1_num; i++)
	{

		int childNum = pNode_1[i]->GetCountChild();
		NumberChild_0[i] = childNum;
	}
}
//获取一级节点子节点的数量
void CPopEditorialClass::GetNumberChild()
{
	for (int i = 0; i < pNode_1_num; i++)
	{

		int childNum = pNode_1[i]->GetCountChild();

		for (int j = 0; j < childNum; j++)
		{

			CTreeNodeUI*  node = pNode_1[i]->GetChildNode(j);//GetParentNode

			int childNum_1 = node->GetCountChild();

			NumberChild[j] = childNum_1;
		}

	}
}
//查找map并显示
void CPopEditorialClass::FindTheMap()//查找map
{
	int num = 0;

	if (selectNode[0][0] >= 0 && selectNode[0][1] < 0 && selectNode[0][2] < 0 && selectNode[0][3] < 0)//双击0级节点名称
	{
		HideThCcontrol(ControlNumber);//DisplayInformation(m_RecvMap_XZ[selectNode[0][0]]);
	}
	else if (selectNode[0][0] >= 0 && selectNode[0][1] >= 0 && selectNode[0][2] < 0 && selectNode[0][3] < 0)//双击1级节点名称
	{
		CTreeNodeUI*  node = pNode_1[selectNode[0][0]]->GetChildNode(selectNode[0][1]);//GetParentNode
		wstring wstr_name;
		wstr_name = node->GetItemText();
		string str_name = wstringToString(wstr_name);

		int sequences;//存放在vector中的顺序
		for (int i = 0; i < name_Level1.size(); i++)
		{
			if (str_name == name_Level1[i])
			{
				sequences = i;
			}
		}

		DisplayInformation(m_RecvMap_XZ[sequences]);


	}
	else if (selectNode[0][0] >= 0 && selectNode[0][1] >= 0 && selectNode[0][2] >= 0 && selectNode[0][3] < 0)//双击2级节点名称
	{
		CTreeNodeUI*  node = pNode_1[selectNode[0][0]]->GetChildNode(selectNode[0][1])->GetChildNode(selectNode[0][2]);//GetParentNode
		wstring wstr_name;
		wstr_name = node->GetItemText();
		string str_name = wstringToString(wstr_name);
		int sequences;//存放在vector中的顺序
		if (str_name == UTF8_To_string("通信") || str_name == UTF8_To_string("导航") || str_name == UTF8_To_string("遥感"))//|| str_name == "导航" || str_name == "遥感"
		{
			cout << "111111111" << endl;
		}
		else
		{
			cout << "22222222222" << endl;
			for (int i = 0; i < name_Level2.size(); i++)
			{
				if (str_name == name_Level2[i])
				{
					sequences = i;
				}
			}
			DisplayInformation(m_RecvMap[sequences]);
		}
	}
	else if (selectNode[0][0] >= 0 && selectNode[0][1] >= 0 && selectNode[0][2] >= 0 && selectNode[0][3] >= 0)//双击3级节点名称
	{
		CTreeNodeUI*  node0 = pNode_1[selectNode[0][0]]->GetChildNode(selectNode[0][1])->GetChildNode(selectNode[0][2]);//GetParentNode
		wstring wstr_name0;
		wstr_name0 = node0->GetItemText();
		string str_name0 = wstringToString(wstr_name0);
		if (str_name0 == UTF8_To_string("通信")|| str_name0 == UTF8_To_string("导航") || str_name0 == UTF8_To_string("遥感"))
		{
			CTreeNodeUI*  node = pNode_1[selectNode[0][0]]->GetChildNode(selectNode[0][1])->GetChildNode(selectNode[0][2])->GetChildNode(selectNode[0][3]);//GetParentNode
			FindTheMap_level3(node->GetItemText());
		}
		else
		{
			CTreeNodeUI*  node = pNode_1[selectNode[0][0]]->GetChildNode(selectNode[0][1])->GetChildNode(selectNode[0][2])->GetChildNode(selectNode[0][3]);//GetParentNode
			wstring wstr_name;
			wstr_name = node->GetItemText();
			string str_name = wstringToString(wstr_name);

			int sequences;//存放在vector中的顺序
			for (int i = 0; i < name_Level3.size(); i++)
			{
				if (str_name == name_Level3[i])
				{
					sequences = i;
				}
			}

			DisplayInformation(m_RecvMap_TxZh[sequences]);
		}

	}

}
void CPopEditorialClass::FindTheMap_2()
{
	HideThCcontrol_2(ControlNumber);
	int num = 0;

	if (selectNode_2[0][0] >= 0 && selectNode_2[0][1] < 0 && selectNode_2[0][2] < 0 && selectNode_2[0][3] < 0)//修改0级节点名称
	{
		HideThCcontrol_2(ControlNumber);//DisplayInformation(m_RecvMap_XZ[selectNode[0][0]]);
	}
	else if (selectNode_2[0][0] >= 0 && selectNode_2[0][1] >= 0 && selectNode_2[0][2] < 0 && selectNode_2[0][3] < 0)//修改1级节点名称
	{
		CTreeNodeUI*  node = pNode_2_1[selectNode_2[0][0]]->GetChildNode(selectNode_2[0][1]);//GetParentNode
		wstring wstr_name;
		wstr_name = node->GetItemText();
		string str_name = wstringToString(wstr_name);

		int sequences;//存放在vector中的顺序
		for (int i = 0; i < name2_Level1.size(); i++)
		{
			if (str_name == name2_Level1[i])
			{
				sequences = i;
			}
		}

		DisplayInformation_2(m_RecvMap_DXG[sequences]);


	}
	else if (selectNode_2[0][0] >= 0 && selectNode_2[0][1] >= 0 && selectNode_2[0][2] >= 0 && selectNode_2[0][3] < 0)
	{
		CTreeNodeUI*  node = pNode_2_1[selectNode_2[0][0]]->GetChildNode(selectNode_2[0][1])->GetChildNode(selectNode_2[0][2]);//GetParentNode
		wstring wstr_name;
		wstr_name = node->GetItemText();
		string str_name = wstringToString(wstr_name);
		int sequences;//存放在vector中的顺序
		if (str_name == UTF8_To_string("关口站") || str_name == UTF8_To_string("信关站") || str_name == UTF8_To_string("测控站") || str_name == UTF8_To_string("网络枢纽")
			|| str_name == UTF8_To_string("固定") || str_name == UTF8_To_string("移动"))//|| str_name == "导航" || str_name == "遥感"
		{
			cout << "111111111" << endl;
		}
		else
		{
			cout << "22222222222" << endl;
			for (int i = 0; i < name2_Level2.size(); i++)
			{
				if (str_name == name2_Level2[i])
				{
					sequences = i;
				}
			}

			DisplayInformation_2(m_RecvMap_DQZ[sequences]);
		}
	}
	else if (selectNode_2[0][0] >= 0 && selectNode_2[0][1] >= 0 && selectNode_2[0][2] >= 0 && selectNode_2[0][3] >= 0)
	{
		CTreeNodeUI*  node0 = pNode_2_1[selectNode_2[0][0]]->GetChildNode(selectNode_2[0][1])->GetChildNode(selectNode_2[0][2]);//GetParentNode
		wstring wstr_name0;
		wstr_name0 = node0->GetItemText();
		string str_name0 = wstringToString(wstr_name0);
		if (str_name0 == UTF8_To_string("关口站") || str_name0 == UTF8_To_string("信关站") || str_name0 == UTF8_To_string("测控站") || str_name0 == UTF8_To_string("网络枢纽")
			|| str_name0 == UTF8_To_string("固定") || str_name0 == UTF8_To_string("移动"))//|| str_name == "导航" || str_name == "遥感"
		{
			CTreeNodeUI*  node = pNode_2_1[selectNode_2[0][0]]->GetChildNode(selectNode_2[0][1])->GetChildNode(selectNode_2[0][2])->GetChildNode(selectNode_2[0][3]);//GetParentNode
			FindTheMap_level3_2(node->GetItemText());
		}
		else
		{
			CTreeNodeUI*  node = pNode_2_1[selectNode_2[0][0]]->GetChildNode(selectNode_2[0][1])->GetChildNode(selectNode_2[0][2])->GetChildNode(selectNode_2[0][3]);//GetParentNode
			wstring wstr_name;
			wstr_name = node->GetItemText();
			string str_name = wstringToString(wstr_name);

			int sequences;//存放在vector中的顺序
			for (int i = 0; i < name2_Level3.size(); i++)
			{
				if (str_name == name2_Level3[i])
				{
					sequences = i;
				}
			}

			DisplayInformation_2(m_RecvMap_DQZChild[sequences]);
		}
	}

}
//查找map-------界面3
void CPopEditorialClass::FindTheMap_3()
{
	HideThCcontrol_3(ControlNumber);
	int num = 0;

	if (selectNode_3[0][0] >= 0 && selectNode_3[0][1] < 0 && selectNode_3[0][2] < 0)//修改0级节点名称
	{
		HideThCcontrol_3(ControlNumber);//DisplayInformation(m_RecvMap_XZ[selectNode[0][0]]);
	}
	else if (selectNode_3[0][0] >= 0 && selectNode_3[0][1] >= 0 && selectNode_3[0][2] < 0)//修改1级节点名称
	{
		CTreeNodeUI*  node = pNode_3_1[selectNode_3[0][0]]->GetChildNode(selectNode_3[0][1]);//GetParentNode
		wstring wstr_name;
		wstr_name = node->GetItemText();
		string str_name = wstringToString(wstr_name);

		int sequences;//存放在vector中的顺序
		for (int i = 0; i < name3_Level1.size(); i++)
		{
			if (str_name == name3_Level1[i])
			{
				sequences = i;
			}
		}
		DisplayInformation_3(m_RecvMap_DMZD[sequences]);


	}
	else if (selectNode_3[0][0] >= 0 && selectNode_3[0][1] >= 0 && selectNode_3[0][2] >= 0)//修改1级节点名称
	{
		CTreeNodeUI*  node = pNode_3_1[selectNode_3[0][0]]->GetChildNode(selectNode_3[0][1])->GetChildNode(selectNode_3[0][2]);//GetParentNode
		wstring wstr_name;
		wstr_name = node->GetItemText();
		string str_name = wstringToString(wstr_name);
		int sequences;//存放在vector中的顺序
		for (int i = 0; i < name3_Level2.size(); i++)
		{
			if (str_name == name3_Level2[i])
			{
				sequences = i;
			}
		}

		DisplayInformation_3(m_RecvMap_DMZDChild[sequences]);
	}

}

//双击3级目录 tab1
void CPopEditorialClass::FindTheMap_level3(CDuiString cdui_name)
{
	int sequences;//存放在vector中的顺序
	wstring wstr_name = cdui_name;
	string str_name = wstringToString(wstr_name);
	for (int i = 0; i < name_Level2.size(); i++)
	{
		if (str_name == name_Level2[i])
		{
			sequences = i;
		}
	}
	DisplayInformation(m_RecvMap[sequences]);
}
//对星座里面的卫星进行筛选的时候 双击4级目录
void CPopEditorialClass::FindTheMap_level4(CDuiString cdui_name)
{
	int sequences = -1;//存放在vector中的顺序
	wstring wstr_name = cdui_name;
	string str_name = wstringToString(wstr_name);
	for (int i = 0; i < name_Level3.size(); i++)
	{
		if (str_name == name_Level3[i])
		{
			sequences = i;
		}
	}
	DisplayInformation(m_RecvMap_TxZh[sequences]);
}
//对参数管理界面tab2 双击4级目录
void CPopEditorialClass::FindTheMap_level4_2(CDuiString cdui_name)
{
	int sequences;//存放在vector中的顺序
	wstring wstr_name = cdui_name;
	string str_name = wstringToString(wstr_name);
	for (int i = 0; i < name2_Level3.size(); i++)
	{
		if (str_name == name2_Level3[i])
		{
			sequences = i;
		}
	}
	DisplayInformation_2(m_RecvMap_DQZChild[sequences]);
}
void CPopEditorialClass::DisplayInformation(map<string, string> m_map)
{
	HideThCcontrol(ControlNumber);
	map<string, string> recv_map;
	recv_map.insert(m_map.begin(), m_map.end());

	int i = 1;
	for (map<string, string>::iterator iter = recv_map.begin(); iter != recv_map.end(); ++iter) 
	{
		cout << "key:" << iter->first << " value:" << iter->second << endl;

		string vec, StoreOptions, StoreOptions1;
		wstring w, kj;
		std::ostringstream streamsssss;
		streamsssss << i;
		StoreOptions = "titles" + streamsssss.str();
		//UtilTool::setWstring(w, iter->first.c_str());
		UtilTool::setWstring(w, UTF8_To_string(iter->first).c_str());
		UtilTool::setWstring(kj, StoreOptions.c_str());

		CControlUI* pControl = static_cast<CControlUI*>(m_pm.FindControl(kj.c_str()));
		pControl->SetText(w.c_str());
		pControl->SetVisible(TRUE);

		wstring w1, kj1;
		StoreOptions1 = "p" + streamsssss.str();
		//UtilTool::setWstring(w1, iter->second.c_str());
		UtilTool::setWstring(w1, UTF8_To_string(iter->second).c_str());
		UtilTool::setWstring(kj1, StoreOptions1.c_str());
		CControlUI* pControl1 = static_cast<CControlUI*>(m_pm.FindControl(kj1.c_str()));
		pControl1->SetText(w1.c_str());
		pControl1->SetVisible(TRUE);
		i += 1;
	}

}
//显示电信港地球站信息
void CPopEditorialClass::DisplayInformation_2(map<string, string> m_map)
{
	HideThCcontrol_2(ControlNumber);
	map<string, string> recv_map;
	recv_map.insert(m_map.begin(), m_map.end());

	int i = 1;
	for (map<string, string>::iterator iter = recv_map.begin(); iter != recv_map.end(); ++iter)
	{
		cout << "key:" << iter->first << " value:" << iter->second << endl;

		string vec, StoreOptions, StoreOptions1;
		wstring w, kj;
		std::ostringstream streamsssss;
		streamsssss << i;
		StoreOptions = "titles2_" + streamsssss.str();
		//UtilTool::setWstring(w, iter->first.c_str());
		UtilTool::setWstring(w, UTF8_To_string(iter->first).c_str());
		UtilTool::setWstring(kj, StoreOptions.c_str());

		CControlUI* pControl = static_cast<CControlUI*>(m_pm.FindControl(kj.c_str()));
		pControl->SetText(w.c_str());
		pControl->SetVisible(TRUE);

		wstring w1, kj1;
		StoreOptions1 = "p2_" + streamsssss.str();
		//UtilTool::setWstring(w1, iter->second.c_str());
		UtilTool::setWstring(w1, UTF8_To_string(iter->second).c_str());
		UtilTool::setWstring(kj1, StoreOptions1.c_str());
		CControlUI* pControl1 = static_cast<CControlUI*>(m_pm.FindControl(kj1.c_str()));
		pControl1->SetText(w1.c_str());
		pControl1->SetVisible(TRUE);
		i += 1;
	}

}
void CPopEditorialClass::DisplayInformation_3(map<string, string> m_map)
{
	HideThCcontrol_3(ControlNumber);
	map<string, string> recv_map;
	recv_map.insert(m_map.begin(), m_map.end());

	int i = 1;
	for (map<string, string>::iterator iter = recv_map.begin(); iter != recv_map.end(); ++iter)
	{
		cout << "key:" << iter->first << " value:" << iter->second << endl;

		string vec, StoreOptions, StoreOptions1;
		wstring w, kj;
		std::ostringstream streamsssss;
		streamsssss << i;
		StoreOptions = "titles3_" + streamsssss.str();
		//UtilTool::setWstring(w, iter->first.c_str());
		UtilTool::setWstring(w, UTF8_To_string(iter->first).c_str());
		UtilTool::setWstring(kj, StoreOptions.c_str());

		CControlUI* pControl = static_cast<CControlUI*>(m_pm.FindControl(kj.c_str()));
		pControl->SetText(w.c_str());
		pControl->SetVisible(TRUE);

		wstring w1, kj1;
		StoreOptions1 = "p3_" + streamsssss.str();
		//UtilTool::setWstring(w1, iter->second.c_str());
		UtilTool::setWstring(w1, UTF8_To_string(iter->second).c_str());
		UtilTool::setWstring(kj1, StoreOptions1.c_str());
		CControlUI* pControl1 = static_cast<CControlUI*>(m_pm.FindControl(kj1.c_str()));
		pControl1->SetText(w1.c_str());
		pControl1->SetVisible(TRUE);
		i += 1;
	}
}
//隐藏界面上的控件组
void  CPopEditorialClass::HideThCcontrol(int num)//num是我xml里面控件最大序号 也就是个数
{
	for (int i = 0; i < num; i++)
	{
		string vec, StoreOptions, StoreOptions1;
		wstring w, kj;
		std::ostringstream streamsssss;
		streamsssss << i+1;
		StoreOptions = "titles" + streamsssss.str();
		UtilTool::setWstring(kj, StoreOptions.c_str());
		CControlUI* pControl = static_cast<CControlUI*>(m_pm.FindControl(kj.c_str()));
		pControl->SetVisible(false);//False false

		wstring w1, kj1;
		StoreOptions1 = "p" + streamsssss.str();
		UtilTool::setWstring(kj1, StoreOptions1.c_str());
		//UtilTool::setWstring(kj1, StoreOptions1.c_str());
		CControlUI* pControl1 = static_cast<CControlUI*>(m_pm.FindControl(kj1.c_str()));
		pControl1->SetVisible(false);
	}
}
//隐藏控件
void CPopEditorialClass::HideThCcontrol_2(int num)
{

	for (int i = 0; i < num; i++)
	{
		string vec, StoreOptions, StoreOptions1;
		wstring w, kj;
		std::ostringstream streamsssss;
		streamsssss << i + 1;
		StoreOptions = "titles2_" + streamsssss.str();
		UtilTool::setWstring(kj, StoreOptions.c_str());
		CControlUI* pControl = static_cast<CControlUI*>(m_pm.FindControl(kj.c_str()));
		pControl->SetVisible(false);//False false

		wstring w1, kj1;
		StoreOptions1 = "p2_" + streamsssss.str();
		UtilTool::setWstring(kj1, StoreOptions1.c_str());
		//UtilTool::setWstring(kj1, StoreOptions1.c_str());
		CControlUI* pControl1 = static_cast<CControlUI*>(m_pm.FindControl(kj1.c_str()));
		pControl1->SetVisible(false);
	}
}
void CPopEditorialClass::HideThCcontrol_3(int num)
{
	for (int i = 0; i < num; i++)
	{
		string vec, StoreOptions, StoreOptions1;
		wstring w, kj;
		std::ostringstream streamsssss;
		streamsssss << i + 1;
		StoreOptions = "titles3_" + streamsssss.str();
		UtilTool::setWstring(kj, StoreOptions.c_str());
		CControlUI* pControl = static_cast<CControlUI*>(m_pm.FindControl(kj.c_str()));
		pControl->SetVisible(false);//False false

		wstring w1, kj1;
		StoreOptions1 = "p3_" + streamsssss.str();
		UtilTool::setWstring(kj1, StoreOptions1.c_str());
		//UtilTool::setWstring(kj1, StoreOptions1.c_str());
		CControlUI* pControl1 = static_cast<CControlUI*>(m_pm.FindControl(kj1.c_str()));
		pControl1->SetVisible(false);
	}
}
//获取界面修改的数据 存回相应的vector
map<string, string> CPopEditorialClass::GetDataFromInterfaces()
{
	map<string,string> map1;

	for (int i = 0; i < ControlNumber; i++)
	{

		string vec, StoreOptions, StoreOptions1;

		wstring w, kj;
		std::ostringstream streamsssss;
		streamsssss << (i + 1);
		StoreOptions = "titles" + streamsssss.str();
		UtilTool::setWstring(w, vec.c_str());
		UtilTool::setWstring(kj, StoreOptions.c_str());
		CControlUI* pControl = static_cast<CControlUI*>(m_pm.FindControl(kj.c_str()));
		wstring wstr_title = pControl->GetText();


		wstring w1, kj1;
		StoreOptions1 = "p" + streamsssss.str();
		UtilTool::setWstring(w1, vec.c_str());
		UtilTool::setWstring(kj1, StoreOptions1.c_str());
		CControlUI* pControl1 = static_cast<CControlUI*>(m_pm.FindControl(kj1.c_str()));
		wstring wstr_title1 = pControl1->GetText();
		if (pControl->IsVisible() && pControl1->IsVisible())
		{
			//Vec.push_back(make_pair(wstringToString(wstr_title), wstringToString(wstr_title1)));
			map1[string_To_UTF8(wstringToString(wstr_title))] = string_To_UTF8(wstringToString(wstr_title1));
		}

	}
	return map1;
}
//修改节点名称
void CPopEditorialClass::ModifyNodeName()
{
	//selectNode[0][0] = i;
	//selectNode[0][1] = 0;
	if (selectNode[0][0] >= 0 && selectNode[0][1] < 0 && selectNode[0][2] < 0)//修改0级节点名称
	{
		pNode_1[selectNode[0][0]]->SetItemText(stringToWstring(newName).c_str());
	}
	if (selectNode[0][0] >= 0 && selectNode[0][1] >= 0 && selectNode[0][2] < 0)//修改1级节点名称
	{
		//pNode_2[]
		CTreeNodeUI*  node = pNode_1[selectNode[0][0]]->GetChildNode(selectNode[0][1]);//GetParentNode
		wstring wstr_name;
		wstr_name = node->GetItemText();
		string str_name = wstringToString(wstr_name);
		int sequences;//存放在vector中的顺序
		for (int i = 0; i < name_Level1.size(); i++)
		{
			if (str_name == name_Level1[i])
			{
				sequences = i;
			}
		}
		name_Level1[sequences] = newName;
		node->SetItemText(stringToWstring(newName).c_str());
	}

	if (selectNode[0][0] >= 0 && selectNode[0][1] >= 0 && selectNode[0][2] >= 0)//修改2级节点名称
	{
		//pNode_2[]
		CTreeNodeUI*  node = pNode_1[selectNode[0][0]]->GetChildNode(selectNode[0][1])->GetChildNode(selectNode[0][2]);//GetParentNode
		wstring wstr_name;
		wstr_name = node->GetItemText();
		string str_name = wstringToString(wstr_name);
		int sequences;//存放在vector中的顺序
		for (int i = 0; i < name_Level2.size(); i++)
		{
			if (str_name == name_Level2[i])
			{
				sequences = i;
			}
		}
		name_Level2[sequences] = newName;

		node->SetItemText(stringToWstring(newName).c_str());
	}

}
//修改节点名称 --电信港
void  CPopEditorialClass::ModifyNodeName_2()
{
	if (selectNode_2[0][0] >= 0 && selectNode_2[0][1] < 0 && selectNode_2[0][2] < 0)//修改0级节点名称
	{
		pNode_2_1[selectNode_2[0][0]]->SetItemText(stringToWstring(newName_2).c_str());
	}
	if (selectNode_2[0][0] >= 0 && selectNode_2[0][1] >= 0 && selectNode_2[0][2] < 0)//修改1级节点名称
	{
		//pNode_2[]
		CTreeNodeUI*  node = pNode_2_1[selectNode_2[0][0]]->GetChildNode(selectNode_2[0][1]);//GetParentNode
		wstring wstr_name;
		wstr_name = node->GetItemText();
		string str_name = wstringToString(wstr_name);
		int sequences;//存放在vector中的顺序
		for (int i = 0; i < name2_Level1.size(); i++)
		{
			if (str_name == name2_Level1[i])
			{
				sequences = i;
			}
		}
		name2_Level1[sequences] = newName_2;
		node->SetItemText(stringToWstring(newName_2).c_str());
	}

	if (selectNode_2[0][0] >= 0 && selectNode_2[0][1] >= 0 && selectNode_2[0][2] >= 0)//修改2级节点名称
	{
		//pNode_2[]
		CTreeNodeUI*  node = pNode_2_1[selectNode_2[0][0]]->GetChildNode(selectNode_2[0][1])->GetChildNode(selectNode_2[0][2]);//GetParentNode
		wstring wstr_name;
		wstr_name = node->GetItemText();
		string str_name = wstringToString(wstr_name);
		int sequences;//存放在vector中的顺序
		for (int i = 0; i < name2_Level2.size(); i++)
		{
			if (str_name == name2_Level2[i])
			{
				sequences = i;
			}
		}
		name2_Level2[sequences] = newName_2;

		node->SetItemText(stringToWstring(newName_2).c_str());
	}
}
//修改节点名称 ---地面终端设备
void  CPopEditorialClass::ModifyNodeName_3()
{
	if (selectNode_3[0][0] >= 0 && selectNode_3[0][1] < 0 && selectNode_3[0][2] < 0)//修改0级节点名称
	{
		pNode_3_1[selectNode_3[0][0]]->SetItemText(stringToWstring(newName_3).c_str());
	}
	if (selectNode_3[0][0] >= 0 && selectNode_3[0][1] >= 0 && selectNode_3[0][2] < 0)//修改1级节点名称
	{
		//pNode_2[]
		CTreeNodeUI*  node = pNode_3_1[selectNode_3[0][0]]->GetChildNode(selectNode_3[0][1]);//GetParentNode
		wstring wstr_name;
		wstr_name = node->GetItemText();
		string str_name = wstringToString(wstr_name);
		int sequences;//存放在vector中的顺序
		for (int i = 0; i < name3_Level1.size(); i++)
		{
			if (str_name == name3_Level1[i])
			{
				sequences = i;
			}
		}
		name3_Level1[sequences] = newName_3;
		node->SetItemText(stringToWstring(newName_3).c_str());
	}


}
map<string,string> CPopEditorialClass::GetHoroscopeInform()
{
	map<string, string> l_map;

	ifstream infile;
	string path1 = "D:\\DATA1\\Demo\\Horoscope.txt";

	infile.open(path1);   //将文件流对象与文件连接起来 
	//assert(infile.is_open());   //若失败,则输出错误消息,并终止程序运行 
	if (infile)
	{
		string s;
		while (getline(infile, s))
		{
			cout << s.c_str() << endl;
			vector<string> str_;
			string pattern = ":";
			str_ = my_split(s, pattern);
			l_map[string_To_UTF8(str_[0])] = string_To_UTF8(str_[1]);
			
		}
		infile.close();             //关闭文件输入流 
	}

	return l_map;
}
//从txt获取卫星信息
map<string, string> CPopEditorialClass::GetSatelliteInform()
{
	map<string, string> l_map;

	ifstream infile;
	string path1 = "D:\\DATA1\\Demo\\Satellite.txt";

	infile.open(path1);   //将文件流对象与文件连接起来 
	//assert(infile.is_open());   //若失败,则输出错误消息,并终止程序运行 
	if (infile)
	{
		string s;
		while (getline(infile, s))
		{
			cout << s.c_str() << endl;
			vector<string> str_;
			string pattern = ":";
			str_ = my_split(s, pattern);
			l_map[string_To_UTF8(str_[0])] = string_To_UTF8(str_[1]);

		}
		infile.close();             //关闭文件输入流 
	}

	return l_map;
}
//从txt获取卫星的天线属性信息
map<string, string> CPopEditorialClass::GetSatelliteInform_1()
{
	map<string, string> l_map;

	ifstream infile;
	string path1 = "D:\\DATA1\\Demo\\SatelliteAntennaAttribute.txt";

	infile.open(path1);   //将文件流对象与文件连接起来 
	//assert(infile.is_open());   //若失败,则输出错误消息,并终止程序运行 
	if (infile)
	{
		string s;
		while (getline(infile, s))
		{
			cout << s.c_str() << endl;
			vector<string> str_;
			string pattern = ":";
			str_ = my_split(s, pattern);
			l_map[string_To_UTF8(str_[0])] = string_To_UTF8(str_[1]);

		}
		infile.close();             //关闭文件输入流 
	}

	return l_map;
}
//从txt获取卫星的载荷属性信息
map<string, string> CPopEditorialClass::GetSatelliteInform_2()
{
	map<string, string> l_map;

	ifstream infile;
	string path1 = "D:\\DATA1\\Demo\\SatellitePayloadAttribute.txt";

	infile.open(path1);   //将文件流对象与文件连接起来 
	//assert(infile.is_open());   //若失败,则输出错误消息,并终止程序运行 
	if (infile)
	{
		string s;
		while (getline(infile, s))
		{
			cout << s.c_str() << endl;
			vector<string> str_;
			string pattern = ":";
			str_ = my_split(s, pattern);
			l_map[string_To_UTF8(str_[0])] = string_To_UTF8(str_[1]);

		}
		infile.close();             //关闭文件输入流 
	}

	return l_map;
}
//从txt获取电信港信息
map<string, string> CPopEditorialClass::GetTeleport()
{
	map<string, string> l_map;

	ifstream infile;
	string path1 = "D:\\DATA1\\Demo\\Teleport.txt";

	infile.open(path1);   //将文件流对象与文件连接起来 
	//assert(infile.is_open());   //若失败,则输出错误消息,并终止程序运行 
	if (infile)
	{
		string s;
		while (getline(infile, s))
		{
			cout << s.c_str() << endl;
			vector<string> str_;
			string pattern = ":";
			str_ = my_split(s, pattern);
			l_map[string_To_UTF8(str_[0])] = string_To_UTF8(str_[1]);

		}
		infile.close();             //关闭文件输入流 
	}

	return l_map;
}
//从txt获取地球站
map<string, string> CPopEditorialClass::GetEarthStation()
{
	map<string, string> l_map;

	ifstream infile;
	string path1 = "D:\\DATA1\\Demo\\EarthStation.txt";

	infile.open(path1);   //将文件流对象与文件连接起来 
	//assert(infile.is_open());   //若失败,则输出错误消息,并终止程序运行 
	if (infile)
	{
		string s;
		while (getline(infile, s))
		{
			cout << s.c_str() << endl;
			vector<string> str_;
			string pattern = ":";
			str_ = my_split(s, pattern);
			l_map[string_To_UTF8(str_[0])] = string_To_UTF8(str_[1]);

		}
		infile.close();             //关闭文件输入流 
	}

	return l_map;
}
//从txt获取地球站馈源属性信息 tab2
map<string, string> CPopEditorialClass::GetEarthStation_1()
{
	map<string, string> l_map;

	ifstream infile;
	string path1 = "D:\\DATA1\\Demo\\EarthStationCollapse.txt";

	infile.open(path1);   //将文件流对象与文件连接起来 
	//assert(infile.is_open());   //若失败,则输出错误消息,并终止程序运行 
	if (infile)
	{
		string s;
		while (getline(infile, s))
		{
			cout << s.c_str() << endl;
			vector<string> str_;
			string pattern = ":";
			str_ = my_split(s, pattern);
			l_map[string_To_UTF8(str_[0])] = string_To_UTF8(str_[1]);

		}
		infile.close();             //关闭文件输入流 
	}

	return l_map;
}
//从txt获取地球站获取波形设备属性信息 tab2
map<string, string> CPopEditorialClass::GetEarthStation_2()
{
	map<string, string> l_map;

	ifstream infile;
	string path1 = "D:\\DATA1\\Demo\\EarthStationWaveform.txt";

	infile.open(path1);   //将文件流对象与文件连接起来 
	//assert(infile.is_open());   //若失败,则输出错误消息,并终止程序运行 
	if (infile)
	{
		string s;
		while (getline(infile, s))
		{
			cout << s.c_str() << endl;
			vector<string> str_;
			string pattern = ":";
			str_ = my_split(s, pattern);
			l_map[string_To_UTF8(str_[0])] = string_To_UTF8(str_[1]);

		}
		infile.close();             //关闭文件输入流 
	}

	return l_map;
}
//从txt获取地球站天线属性信息 tab2
map<string, string> CPopEditorialClass::GetEarthStation_3()
{
	map<string, string> l_map;

	ifstream infile;
	string path1 = "D:\\DATA1\\Demo\\EarthStationAntenna.txt";

	infile.open(path1);   //将文件流对象与文件连接起来 
	//assert(infile.is_open());   //若失败,则输出错误消息,并终止程序运行 
	if (infile)
	{
		string s;
		while (getline(infile, s))
		{
			cout << s.c_str() << endl;
			vector<string> str_;
			string pattern = ":";
			str_ = my_split(s, pattern);
			l_map[string_To_UTF8(str_[0])] = string_To_UTF8(str_[1]);

		}
		infile.close();             //关闭文件输入流 
	}

	return l_map;
}

//从txt获取地面终端的信息
map<string, string> CPopEditorialClass::GetGroundTerminal()
{
	map<string, string> l_map;

	ifstream infile;
	string path1 = "D:\\DATA1\\Demo\\GroundTerminal.txt";

	infile.open(path1);   //将文件流对象与文件连接起来 
	//assert(infile.is_open());   //若失败,则输出错误消息,并终止程序运行 
	if (infile)
	{
		string s;
		while (getline(infile, s))
		{
			cout << s.c_str() << endl;
			vector<string> str_;
			string pattern = ":";
			str_ = my_split(s, pattern);
			l_map[string_To_UTF8(str_[0])] = string_To_UTF8(str_[1]);

		}
		infile.close();             //关闭文件输入流 
	}

	return l_map;
}
//从txt获取地面终端中载荷属性信息
map<string, string> CPopEditorialClass::GetGroundTerminal_1()
{
	map<string, string> l_map;

	ifstream infile;
	string path1 = "D:\\DATA1\\Demo\\GroundTerminalLoad.txt";

	infile.open(path1);   //将文件流对象与文件连接起来 
	//assert(infile.is_open());   //若失败,则输出错误消息,并终止程序运行 
	if (infile)
	{
		string s;
		while (getline(infile, s))
		{
			cout << s.c_str() << endl;
			vector<string> str_;
			string pattern = ":";
			str_ = my_split(s, pattern);
			l_map[string_To_UTF8(str_[0])] = string_To_UTF8(str_[1]);

		}
		infile.close();             //关闭文件输入流 
	}

	return l_map;
}
//从txt获取地面终端中波形设备属性信息
map<string, string> CPopEditorialClass::GetGroundTerminal_2()
{
	map<string, string> l_map;

	ifstream infile;
	string path1 = "D:\\DATA1\\Demo\\GroundTerminalWaveform.txt";//

	infile.open(path1);   //将文件流对象与文件连接起来 
	//assert(infile.is_open());   //若失败,则输出错误消息,并终止程序运行 
	if (infile)
	{
		string s;
		while (getline(infile, s))
		{
			cout << s.c_str() << endl;
			vector<string> str_;
			string pattern = ":";
			str_ = my_split(s, pattern);
			l_map[string_To_UTF8(str_[0])] = string_To_UTF8(str_[1]);

		}
		infile.close();             //关闭文件输入流 
	}

	return l_map;
}
//从txt获取地面终端中天线属性信息
map<string, string> CPopEditorialClass::GetGroundTerminal_3()
{
	map<string, string> l_map;

	ifstream infile;
	string path1 = "D:\\DATA1\\Demo\\GroundTerminalAntenna.txt";//GroundTerminalAntenna.txt 

	infile.open(path1);   //将文件流对象与文件连接起来 
	//assert(infile.is_open());   //若失败,则输出错误消息,并终止程序运行 
	if (infile)
	{
		string s;
		while (getline(infile, s))
		{
			cout << s.c_str() << endl;
			vector<string> str_;
			string pattern = ":";
			str_ = my_split(s, pattern);
			l_map[string_To_UTF8(str_[0])] = string_To_UTF8(str_[1]);

		}
		infile.close();             //关闭文件输入流 
	}

	return l_map;
}
//移动卫星节点 level 1位星座名称 level2位卫星名称
void CPopEditorialClass::Movelevel_2(string level1, CDuiString level2)//level1为要移动到的星座的名称 level2为要移动的卫星
{	
	vector<string> vec_child;//存放要移动的孩子节点
	string str_nameP;//获取level2所在父节点名称
	int sequences1 = -1;
	int childNum = 0;
	for (int i = 0; i < name_Level2.size(); i++)
	{
		wstring wstr_name,wstr_nameP;
		wstr_name = pNode_3[i]->GetItemText();
		string str_level2 = wstringToString(level2.GetData());

		string str_name = wstringToString(wstr_name);
		
		if (str_level2 == str_name)
		{
			sequences1 = i;//选择的卫星所在name_Level2以及m_RecvMap中的位置
			CTreeNodeUI*  node = pNode_3[i]->GetParentNode();//GetParentNode
			wstr_nameP = node->GetItemText();
			str_nameP = wstringToString(wstr_nameP);
			childNum = pNode_3[i]->GetCountChild();//获取要移动的卫星节点子节点数量
			
		}
		/******************获取卫星包含子节点名称****************/
		if (childNum > 0)
		{
			for (int j = 0; j < childNum; j++)
			{
				wstring str_childName;
				string str_ChildName;
				CTreeNodeUI*  node1 = pNode_3[sequences1]->GetChildNode(j);;//GetParentNode
				str_childName = node1->GetItemText();
				str_ChildName = wstringToString(str_childName);
				vec_child.push_back(str_ChildName);
			}

			childNum = 0;
			
		}

	}

	vector<int> sequences_0;
	vector<int> sequencesFrom_0;
	/****************获取卫星子节点所在name_Level3中的顺序***************/
	for (int i = 0; i < vec_child.size(); i++)
	{
		for (int j = 0; j < name_Level3.size(); j++)
		{
			if (vec_child[i] == name_Level3[j])
			{
				sequences_0.push_back(j);
			}
		}
	}
	/**********************获取卫星父节点和将要移动到的父节点所在数组中的位置*************************/
	int sequences = 0 ;
	int sequencesFrom = 0;
	for (int i = 0; i < name_Level1.size(); i++)
	{
		wstring wstr_name;
		wstr_name = pNode_2[i]->GetItemText();
		string str_name = wstringToString(wstr_name);
		if (level1 == str_name)
		{
			sequences = i;//卫星要移动的目标星座节点在pNode_2中的位置
		}
		if (str_nameP == str_name)
		{
			sequencesFrom = i;//卫星本身所在星座节点在pNode_2中的位置
		}

	}
	/********************删除卫星包含的子节点*********************/
	for (int i = 0; i < sequences_0.size(); i++)
	{
		cout << sequencesFrom << endl;
		pNode_3[sequences1]->Remove(pNode_4[sequences_0[i]]);
	}
	/********************从卫星原来的父节点删除卫星节点 新建卫星节点至移动的目标父节点*********************/
	cout << sequencesFrom << endl;
	pNode_2[sequencesFrom]->Remove(pNode_3[sequences1]);
	pNode_3[sequences1] = new CTreeNodeUI();
	pNode_3[sequences1]->CheckBoxSelected(false);
	pNode_3[sequences1]->SetItemText(level2);
	pNode_3[sequences1]->SetFixedHeight(30);
	pNode_3[sequences1]->SetItemTextColor(0xFFFFFFFF);
	pNode_3[sequences1]->SetItemHotTextColor(0xFFFFFFFF);
	pNode_3[sequences1]->SetSelItemTextColor(0xFFFFFFFF);
	pNode_3[sequences1]->SetAttribute(_T("folderattr"), _T("padding=&quot;0,1,0,0&quot; width=&quot;16&quot; height=&quot;16&quot; normalimage=&quot;file='ui/展开.png' &quot; "));
	pNode_3[sequences1]->SetAttribute(_T("folderattr"), _T("hotimage=&quot;file='ui/展开.png' &quot; selectedimage=&quot;file='ui/未展开.png'&quot; selectedhotimage=&quot;file='ui/未展开.png'&quot;"));
	pNode_3[sequences1]->SetAttribute(_T("itemattr"), _T("padding=\"16,0,0,0\" align=\"left\" font=\"13\""));//font=&quot;6&quot;
	pNode_3[sequences1]->SetName(_T("WX1"));
	pNode_2[sequences]->AddChildNode(pNode_3[sequences1]);

	/************************新建卫星子节点*********************/
	for (int i = 0; i < vec_child.size(); i++)
	{
		pNode_4[sequences_0[i]] = new CTreeNodeUI();
		pNode_4[sequences_0[i]]->CheckBoxSelected(false);
		pNode_4[sequences_0[i]]->SetItemText(stringToWstring(vec_child[i]).c_str());
		pNode_4[sequences_0[i]]->SetFixedHeight(30);
		pNode_4[sequences_0[i]]->SetFixedWidth(100);
		pNode_4[sequences_0[i]]->SetItemTextColor(0xFFFFFFFF);
		pNode_4[sequences_0[i]]->SetItemHotTextColor(0xFFFFFFFF);
		pNode_4[sequences_0[i]]->SetSelItemTextColor(0xFFFFFFFF);
		pNode_4[sequences_0[i]]->SetAttribute(_T("folderattr"), _T("padding=&quot;0,1,0,0&quot; width=&quot;16&quot; height=&quot;16&quot; normalimage=&quot;file='ui/展开.png' &quot; "));
		pNode_4[sequences_0[i]]->SetAttribute(_T("folderattr"), _T("hotimage=&quot;file='ui/展开.png' &quot; selectedimage=&quot;file='ui/未展开.png'&quot; selectedhotimage=&quot;file='ui/未展开.png'&quot;"));
		pNode_4[sequences_0[i]]->SetAttribute(_T("itemattr"), _T("padding=\"16,0,0,0\" align=\"left\" font=\"13\""));//font=&quot;6&quot;
		pNode_4[sequences_0[i]]->SetName(_T("WX1"));
		pNode_3[sequences1]->AddChildNode(pNode_4[sequences_0[i]]);
	}
}
//轨道类别分类
void  CPopEditorialClass::Categorize_XZ_RailType()
{
	vector<vector<string>> vecVec_WxChildName;
	int wxChildSeuque = 0;//卫星子节点顺序
	for (int i = 0; i < name_Level1.size(); i++)
	{
		CDuiString wstr_name = pNode_2[i]->GetItemText();
		string str_name = wstringToString(wstr_name.GetData());
		vector<string> childName;
		//vector<int> 
		/***************获取星座下面的卫星名称********************/
		int childNum = pNode_2[i]->GetCountChild();
		for (int j = 0; j < childNum; j++)
		{
			CDuiString wstr_name1 = pNode_2[i]->GetChildNode(j)->GetItemText();
			string str_name1 = wstringToString(wstr_name1.GetData());
			childName.push_back(str_name1);
			int childNum_wx = pNode_2[i]->GetChildNode(j)->GetCountChild();
			vector<string> vec;
			for (int m = 0; m < childNum_wx; m++)
			{
				CDuiString wstr_name3 = pNode_2[i]->GetChildNode(j)->GetChildNode(m)->GetItemText();
				string str_name3 = wstringToString(wstr_name3.GetData());
				vec.push_back(str_name3);
			}
			vecVec_WxChildName.push_back(vec);
		}
		/***************移除场景里的1级(星座层级)节点******************/
		pNode_1[0]->Remove(pNode_2[i]);
		/***************新建筛选后的2级节点******************/
		pNode_2[i] = new CTreeNodeUI();
		pNode_2[i]->CheckBoxSelected(false);
		pNode_2[i]->SetItemText(stringToWstring(str_name).c_str());
		pNode_2[i]->SetFixedHeight(30);
		pNode_2[i]->SetItemTextColor(0xFFFFFFFF);
		pNode_2[i]->SetItemHotTextColor(0xFFFFFFFF);
		pNode_2[i]->SetSelItemTextColor(0xFFFFFFFF);
		pNode_2[i]->SetAttribute(_T("folderattr"), _T("padding=&quot;0,1,0,0&quot; width=&quot;16&quot; height=&quot;16&quot; normalimage=&quot;file='ui/展开.png' &quot; "));
		pNode_2[i]->SetAttribute(_T("folderattr"), _T("hotimage=&quot;file='ui/展开.png' &quot; selectedimage=&quot;file='ui/未展开.png'&quot; selectedhotimage=&quot;file='ui/未展开.png'&quot;"));
		pNode_2[i]->SetAttribute(_T("itemattr"), _T("padding=\"16,0,0,0\" align=\"left\" font=\"13\""));//font=&quot;6&quot;
		pNode_2[i]->SetName(_T("WX1"));
		map<string, string> curr_map = m_RecvMap_XZ[i];
		if (curr_map.count("轨道类别") > 0)
		{
			string value = curr_map["轨道类别"];
			int sequence = GetNoodLevel(UTF8_To_string(value));
			if (sequence >= 0)
			{
				pNode_1[sequence]->AddChildNode(pNode_2[i]);
			}
			else
			{
				CreateTreeNode(UTF8_To_string(value));//向根节点加名称
				pNode_1[pNode_1_num - 1]->AddChildNode(pNode_2[i]);
			}

		}
		for (int m = 0; m < childNum; m++)//卫星数量
		{
			for (int n = 0; n < name_Level2.size(); n++)
			{
				if (name_Level2[n] == childName[m])
				{
					pNode_3[n]->Remove(pNode_4[n*2]);
					pNode_3[n]->Remove(pNode_4[n*2+1]);

					pNode_2[i]->Remove(pNode_3[n]);
					pNode_3[n] = new CTreeNodeUI();
					pNode_3[n]->CheckBoxSelected(false);
					pNode_3[n]->SetItemText(stringToWstring(name_Level2[n]).c_str());
					pNode_3[n]->SetFixedHeight(30);
					pNode_3[n]->SetItemTextColor(0xFFFFFFFF);
					pNode_3[n]->SetItemHotTextColor(0xFFFFFFFF);
					pNode_3[n]->SetSelItemTextColor(0xFFFFFFFF);
					pNode_3[n]->SetAttribute(_T("folderattr"), _T("padding=&quot;0,1,0,0&quot; width=&quot;16&quot; height=&quot;16&quot; normalimage=&quot;file='ui/展开.png' &quot; "));
					pNode_3[n]->SetAttribute(_T("folderattr"), _T("hotimage=&quot;file='ui/展开.png' &quot; selectedimage=&quot;file='ui/未展开.png'&quot; selectedhotimage=&quot;file='ui/未展开.png'&quot;"));
					pNode_3[n]->SetAttribute(_T("itemattr"), _T("padding=\"16,0,0,0\" align=\"left\" font=\"13\""));//font=&quot;6&quot;
					pNode_3[n]->SetName(_T("WX1"));
					pNode_2[i]->AddChildNode(pNode_3[n]);


					pNode_4[n * 2] = new CTreeNodeUI();
					pNode_4[n * 2]->CheckBoxSelected(false);
					pNode_4[n * 2]->SetItemText(stringToWstring(vecVec_WxChildName[wxChildSeuque][0]).c_str());//wxChildSeuque
					pNode_4[n * 2]->SetFixedHeight(30);
					pNode_4[n * 2]->SetFixedWidth(100);
					pNode_4[n * 2]->SetItemTextColor(0xFFFFFFFF);
					pNode_4[n * 2]->SetItemHotTextColor(0xFFFFFFFF);
					pNode_4[n * 2]->SetSelItemTextColor(0xFFFFFFFF);
					pNode_4[n * 2]->SetAttribute(_T("folderattr"), _T("padding=&quot;0,1,0,0&quot; width=&quot;16&quot; height=&quot;16&quot; normalimage=&quot;file='ui/展开.png' &quot; "));
					pNode_4[n * 2]->SetAttribute(_T("folderattr"), _T("hotimage=&quot;file='ui/展开.png' &quot; selectedimage=&quot;file='ui/未展开.png'&quot; selectedhotimage=&quot;file='ui/未展开.png'&quot;"));
					pNode_4[n * 2]->SetAttribute(_T("itemattr"), _T("padding=\"16,0,0,0\" align=\"left\" font=\"13\""));//font=&quot;6&quot;
					pNode_4[n * 2]->SetName(_T("WX1"));
					pNode_3[n]->AddChildNode(pNode_4[n * 2]);

					pNode_4[n * 2 + 1] = new CTreeNodeUI();
					pNode_4[n * 2 + 1]->CheckBoxSelected(false);
					pNode_4[n * 2 + 1]->SetItemText(stringToWstring(vecVec_WxChildName[wxChildSeuque][1]).c_str());//wxChildSeuque
					pNode_4[n * 2 + 1]->SetFixedHeight(30);
					pNode_4[n * 2 + 1]->SetFixedWidth(100);
					pNode_4[n * 2 + 1]->SetItemTextColor(0xFFFFFFFF);
					pNode_4[n * 2 + 1]->SetItemHotTextColor(0xFFFFFFFF);
					pNode_4[n * 2 + 1]->SetSelItemTextColor(0xFFFFFFFF);
					pNode_4[n * 2 + 1]->SetAttribute(_T("folderattr"), _T("padding=&quot;0,1,0,0&quot; width=&quot;16&quot; height=&quot;16&quot; normalimage=&quot;file='ui/展开.png' &quot; "));
					pNode_4[n * 2 + 1]->SetAttribute(_T("folderattr"), _T("hotimage=&quot;file='ui/展开.png' &quot; selectedimage=&quot;file='ui/未展开.png'&quot; selectedhotimage=&quot;file='ui/未展开.png'&quot;"));
					pNode_4[n * 2 + 1]->SetAttribute(_T("itemattr"), _T("padding=\"16,0,0,0\" align=\"left\" font=\"13\""));//font=&quot;6&quot;
					pNode_4[n * 2 + 1]->SetName(_T("WX1"));
					pNode_3[n]->AddChildNode(pNode_4[n * 2 + 1]);

					wxChildSeuque += 1;
		
				}
			}
		}


	}

}
//轨道类别分类后还原
void CPopEditorialClass::Restore_XZ_RailType()
{
	vector<vector<string>> vecVec_WxChildName;
	int wxChildSeuque = 0;
	for (int i = 0; i < name_Level1.size(); i++)
	{
		vector<string> childName;
		int childNum = pNode_2[i]->GetCountChild();
		for (int j = 0; j < childNum; j++)
		{
			CDuiString wstr_name1 = pNode_2[i]->GetChildNode(j)->GetItemText();
			string str_name1 = wstringToString(wstr_name1.GetData());
			childName.push_back(str_name1);
			int childNum_wx = pNode_2[i]->GetChildNode(j)->GetCountChild();
			vector<string> vec;
			for (int m = 0; m < childNum_wx; m++)
			{
				CDuiString wstr_name3 = pNode_2[i]->GetChildNode(j)->GetChildNode(m)->GetItemText();
				string str_name3 = wstringToString(wstr_name3.GetData());
				vec.push_back(str_name3);
			}
			vecVec_WxChildName.push_back(vec);
		}

		CDuiString wstr_name = pNode_2[i]->GetItemText();
		string str_name = wstringToString(wstr_name.GetData());

		int num = GetNoodLevel_Root(name_Level1[i]);
		cout << num << endl;

		pNode_1[num]->Remove(pNode_2[i]);

		pNode_2[i] = new CTreeNodeUI();
		pNode_2[i]->CheckBoxSelected(false);
		pNode_2[i]->SetItemText(stringToWstring(str_name).c_str());
		pNode_2[i]->SetFixedHeight(30);
		pNode_2[i]->SetItemTextColor(0xFFFFFFFF);
		pNode_2[i]->SetItemHotTextColor(0xFFFFFFFF);
		pNode_2[i]->SetSelItemTextColor(0xFFFFFFFF);
		pNode_2[i]->SetAttribute(_T("folderattr"), _T("padding=&quot;0,1,0,0&quot; width=&quot;16&quot; height=&quot;16&quot; normalimage=&quot;file='ui/展开.png' &quot; "));
		pNode_2[i]->SetAttribute(_T("folderattr"), _T("hotimage=&quot;file='ui/展开.png' &quot; selectedimage=&quot;file='ui/未展开.png'&quot; selectedhotimage=&quot;file='ui/未展开.png'&quot;"));
		pNode_2[i]->SetAttribute(_T("itemattr"), _T("padding=\"16,0,0,0\" align=\"left\" font=\"13\""));//font=&quot;6&quot;
		pNode_2[i]->SetName(_T("WX1"));
		pNode_1[0]->AddChildNode(pNode_2[i]);

		for (int m = 0; m < childNum; m++)
		{
			for (int n = 0; n < name_Level2.size(); n++)
			{
				if (name_Level2[n] == childName[m])
				{

					pNode_3[n]->Remove(pNode_4[n * 2]);
					pNode_3[n]->Remove(pNode_4[n * 2 + 1]);

					pNode_2[i]->Remove(pNode_3[n]);
					pNode_3[n] = new CTreeNodeUI();
					pNode_3[n]->CheckBoxSelected(false);
					pNode_3[n]->SetItemText(stringToWstring(name_Level2[n]).c_str());
					pNode_3[n]->SetFixedHeight(30);
					pNode_3[n]->SetItemTextColor(0xFFFFFFFF);
					pNode_3[n]->SetItemHotTextColor(0xFFFFFFFF);
					pNode_3[n]->SetSelItemTextColor(0xFFFFFFFF);
					pNode_3[n]->SetAttribute(_T("folderattr"), _T("padding=&quot;0,1,0,0&quot; width=&quot;16&quot; height=&quot;16&quot; normalimage=&quot;file='ui/展开.png' &quot; "));
					pNode_3[n]->SetAttribute(_T("folderattr"), _T("hotimage=&quot;file='ui/展开.png' &quot; selectedimage=&quot;file='ui/未展开.png'&quot; selectedhotimage=&quot;file='ui/未展开.png'&quot;"));
					pNode_3[n]->SetAttribute(_T("itemattr"), _T("padding=\"16,0,0,0\" align=\"left\" font=\"13\""));//font=&quot;6&quot;
					pNode_3[n]->SetName(_T("WX1"));
					pNode_2[i]->AddChildNode(pNode_3[n]);

					pNode_4[n * 2] = new CTreeNodeUI();
					pNode_4[n * 2]->CheckBoxSelected(false);
					pNode_4[n * 2]->SetItemText(stringToWstring(vecVec_WxChildName[wxChildSeuque][0]).c_str());//wxChildSeuque
					pNode_4[n * 2]->SetFixedHeight(30);
					pNode_4[n * 2]->SetFixedWidth(100);
					pNode_4[n * 2]->SetItemTextColor(0xFFFFFFFF);
					pNode_4[n * 2]->SetItemHotTextColor(0xFFFFFFFF);
					pNode_4[n * 2]->SetSelItemTextColor(0xFFFFFFFF);
					pNode_4[n * 2]->SetAttribute(_T("folderattr"), _T("padding=&quot;0,1,0,0&quot; width=&quot;16&quot; height=&quot;16&quot; normalimage=&quot;file='ui/展开.png' &quot; "));
					pNode_4[n * 2]->SetAttribute(_T("folderattr"), _T("hotimage=&quot;file='ui/展开.png' &quot; selectedimage=&quot;file='ui/未展开.png'&quot; selectedhotimage=&quot;file='ui/未展开.png'&quot;"));
					pNode_4[n * 2]->SetAttribute(_T("itemattr"), _T("padding=\"16,0,0,0\" align=\"left\" font=\"13\""));//font=&quot;6&quot;
					pNode_4[n * 2]->SetName(_T("WX1"));
					pNode_3[n]->AddChildNode(pNode_4[n * 2]);

					pNode_4[n * 2 + 1] = new CTreeNodeUI();
					pNode_4[n * 2 + 1]->CheckBoxSelected(false);
					pNode_4[n * 2 + 1]->SetItemText(stringToWstring(vecVec_WxChildName[wxChildSeuque][1]).c_str());//wxChildSeuque
					pNode_4[n * 2 + 1]->SetFixedHeight(30);
					pNode_4[n * 2 + 1]->SetFixedWidth(100);
					pNode_4[n * 2 + 1]->SetItemTextColor(0xFFFFFFFF);
					pNode_4[n * 2 + 1]->SetItemHotTextColor(0xFFFFFFFF);
					pNode_4[n * 2 + 1]->SetSelItemTextColor(0xFFFFFFFF);
					pNode_4[n * 2 + 1]->SetAttribute(_T("folderattr"), _T("padding=&quot;0,1,0,0&quot; width=&quot;16&quot; height=&quot;16&quot; normalimage=&quot;file='ui/展开.png' &quot; "));
					pNode_4[n * 2 + 1]->SetAttribute(_T("folderattr"), _T("hotimage=&quot;file='ui/展开.png' &quot; selectedimage=&quot;file='ui/未展开.png'&quot; selectedhotimage=&quot;file='ui/未展开.png'&quot;"));
					pNode_4[n * 2 + 1]->SetAttribute(_T("itemattr"), _T("padding=\"16,0,0,0\" align=\"left\" font=\"13\""));//font=&quot;6&quot;
					pNode_4[n * 2 + 1]->SetName(_T("WX1"));
					pNode_3[n]->AddChildNode(pNode_4[n * 2 + 1]);

					wxChildSeuque += 1;
				}
			}
		}
	}
	RootNodeOnly1();


}
//获取轨道类别的节点类型所在顺序
int CPopEditorialClass::GetNoodLevel(string nodeName)
{
	CDuiString cdui_name;
	string str_name;
	int sequences = -1;
	for (int i = 0; i < pNode_1_num; i++)
	{
		cdui_name = pNode_1[i]->GetItemText();
		str_name = wstringToString(cdui_name.GetData());
		if (str_name == nodeName)
		{
			sequences = i;
		}
	}

	return sequences;
}
//获取轨道类别的根节点所在顺序
int CPopEditorialClass::GetNoodLevel_Root(string nodeName)
{
	CDuiString cdui_name;
	string str_name;
	int sequences = -1;
	for (int i = 0; i < pNode_1_num; i++)
	{
		int childNum = pNode_1[i]->GetCountChild();
		for (int j = 0; j < childNum; j++)
		{
			CTreeNodeUI*  node = pNode_1[i]->GetChildNode(j);//GetParentNode
			cdui_name = node->GetItemText();
			str_name = wstringToString(cdui_name.GetData());
			if (str_name == nodeName)
			{
				sequences = i;
			}
		}
	}
	return sequences;
}
//获取轨道类别的节点类型所在顺序
int CPopEditorialClass::GetNoodLevel_level2(string nodeName,int serialNumber)
{
	//获取卫星类型在节点中的顺序
	CDuiString cdui_name;
	string str_name;
	int sequences = -1;
	//cdui_name = pNode_2[serialNumber]->GetItemText();
	//str_name = wstringToString(cdui_name.GetData());
	int childNum = pNode_2[serialNumber]->GetCountChild();
	for (int i = 0; i < childNum; i++)
	{
		CTreeNodeUI * pNode = pNode_2[serialNumber]->GetChildNode(i);
		cdui_name = pNode->GetItemText();
		str_name = wstringToString(cdui_name.GetData());
		if (str_name == nodeName)
		{
			sequences = i;
		}
	}

	return sequences;
}
//分类复原后只留根节点
void CPopEditorialClass::RootNodeOnly1()
{
	int num = pNode_1_num;
	cout << num << endl;
	for (int i = 0; i < (num-1); i++)
	{
		m_treeView->Remove(pNode_1[i+1]);
		pNode_1_num -= 1;
	}
	int num1 = pNode_1_num;
	cout << num1 << endl;

}
//清空子节点
void CPopEditorialClass::EmptyChildNode_tab1()
{

	name_Level1.clear();
	int num = pNode_2_num;
	cout << num << endl;
	for (int i = 0; i < num ; i++)
	{
		pNode_1[0]->Remove(pNode_2[i]);
		pNode_2_num -= 1;
	}

	memset(pNode_3, 0, sizeof(pNode_3));
	pNode_3_num = 0;

	int num1 = pNode_2_num;
	cout << num1 << endl;
}

void CPopEditorialClass::EmptyChildNode_tab2()
{
	name2_Level1.clear();
	int num = pNode_2_2_num;
	cout << num << endl;
	for (int i = 0; i < num; i++)
	{
		pNode_2_1[0]->Remove(pNode_2_2[i]);
		pNode_2_2_num -= 1;
	}

	memset(pNode_2_3, 0, sizeof(pNode_2_3));
	pNode_2_3_num = 0;




	int num1 = pNode_2_2_num;
	cout << num1 << endl;
}
void CPopEditorialClass::EmptyChildNode_tab3()
{
	name2_Level1.clear();
	int num = pNode_3_2_num;
	cout << num << endl;
	for (int i = 0; i < num; i++)
	{
		pNode_3_1[0]->Remove(pNode_3_2[i]);
		pNode_3_2_num -= 1;
	}

	int num1 = pNode_3_2_num;
	cout << num1 << endl;

}
//功能类别分类
void CPopEditorialClass::Categorize_XZ_Functional()
{
	vector<vector<string>> vecVec_WxChildName;
	int wxChildSeuque = 0;
	for (int i = 0; i < name_Level1.size(); i++)
	{
		CDuiString wstr_name = pNode_2[i]->GetItemText();
		string str_name = wstringToString(wstr_name.GetData());
		vector<string> childName;
		//vector<int> 
		/************************************/
		int childNum = pNode_2[i]->GetCountChild();
		for (int j = 0; j < childNum; j++)
		{
			CDuiString wstr_name1 = pNode_2[i]->GetChildNode(j)->GetItemText();
			string str_name1 = wstringToString(wstr_name1.GetData());
			childName.push_back(str_name1);

			int childNum_wx = pNode_2[i]->GetChildNode(j)->GetCountChild();
			vector<string> vec;
			for (int m = 0; m < childNum_wx; m++)
			{
				CDuiString wstr_name3 = pNode_2[i]->GetChildNode(j)->GetChildNode(m)->GetItemText();
				string str_name3 = wstringToString(wstr_name3.GetData());
				vec.push_back(str_name3);
			}
			vecVec_WxChildName.push_back(vec);
		}
		/***************移除场景里的1级节点******************/
		pNode_1[0]->Remove(pNode_2[i]);
		/***************新建筛选后的2级节点******************/
		pNode_2[i] = new CTreeNodeUI();
		pNode_2[i]->CheckBoxSelected(false);
		pNode_2[i]->SetItemText(stringToWstring(str_name).c_str());
		pNode_2[i]->SetFixedHeight(30);
		pNode_2[i]->SetItemTextColor(0xFFFFFFFF);
		pNode_2[i]->SetItemHotTextColor(0xFFFFFFFF);
		pNode_2[i]->SetSelItemTextColor(0xFFFFFFFF);
		pNode_2[i]->SetAttribute(_T("folderattr"), _T("padding=&quot;0,1,0,0&quot; width=&quot;16&quot; height=&quot;16&quot; normalimage=&quot;file='ui/展开.png' &quot; "));
		pNode_2[i]->SetAttribute(_T("folderattr"), _T("hotimage=&quot;file='ui/展开.png' &quot; selectedimage=&quot;file='ui/未展开.png'&quot; selectedhotimage=&quot;file='ui/未展开.png'&quot;"));
		pNode_2[i]->SetAttribute(_T("itemattr"), _T("padding=\"16,0,0,0\" align=\"left\" font=\"13\""));//font=&quot;6&quot;
		pNode_2[i]->SetName(_T("WX1"));



		map<string, string> curr_map = m_RecvMap_XZ[i];


		if (curr_map.count("功能类别") > 0)
		{
			string value = curr_map["功能类别"];
			int sequence = GetNoodLevel(UTF8_To_string(value));
			if (sequence >= 0)
			{
				pNode_1[sequence]->AddChildNode(pNode_2[i]);
			}
			else
			{
				CreateTreeNode(UTF8_To_string(value));//向根节点加名称
				pNode_1[pNode_1_num - 1]->AddChildNode(pNode_2[i]);
			}

		}
		for (int m = 0; m < childNum; m++)
		{
			for (int n = 0; n < name_Level2.size(); n++)
			{
				if (name_Level2[n] == childName[m])
				{

					pNode_3[n]->Remove(pNode_4[n * 2]);
					pNode_3[n]->Remove(pNode_4[n * 2 + 1]);

					pNode_2[i]->Remove(pNode_3[n]);
					pNode_3[n] = new CTreeNodeUI();
					pNode_3[n]->CheckBoxSelected(false);
					pNode_3[n]->SetItemText(stringToWstring(name_Level2[n]).c_str());
					pNode_3[n]->SetFixedHeight(30);
					pNode_3[n]->SetItemTextColor(0xFFFFFFFF);
					pNode_3[n]->SetItemHotTextColor(0xFFFFFFFF);
					pNode_3[n]->SetSelItemTextColor(0xFFFFFFFF);
					pNode_3[n]->SetAttribute(_T("folderattr"), _T("padding=&quot;0,1,0,0&quot; width=&quot;16&quot; height=&quot;16&quot; normalimage=&quot;file='ui/展开.png' &quot; "));
					pNode_3[n]->SetAttribute(_T("folderattr"), _T("hotimage=&quot;file='ui/展开.png' &quot; selectedimage=&quot;file='ui/未展开.png'&quot; selectedhotimage=&quot;file='ui/未展开.png'&quot;"));
					pNode_3[n]->SetAttribute(_T("itemattr"), _T("padding=\"16,0,0,0\" align=\"left\" font=\"13\""));//font=&quot;6&quot;
					pNode_3[n]->SetName(_T("WX1"));
					pNode_2[i]->AddChildNode(pNode_3[n]);


					pNode_4[n * 2] = new CTreeNodeUI();
					pNode_4[n * 2]->CheckBoxSelected(false);
					pNode_4[n * 2]->SetItemText(stringToWstring(vecVec_WxChildName[wxChildSeuque][0]).c_str());//wxChildSeuque
					pNode_4[n * 2]->SetFixedHeight(30);
					pNode_4[n * 2]->SetFixedWidth(100);
					pNode_4[n * 2]->SetItemTextColor(0xFFFFFFFF);
					pNode_4[n * 2]->SetItemHotTextColor(0xFFFFFFFF);
					pNode_4[n * 2]->SetSelItemTextColor(0xFFFFFFFF);
					pNode_4[n * 2]->SetAttribute(_T("folderattr"), _T("padding=&quot;0,1,0,0&quot; width=&quot;16&quot; height=&quot;16&quot; normalimage=&quot;file='ui/展开.png' &quot; "));
					pNode_4[n * 2]->SetAttribute(_T("folderattr"), _T("hotimage=&quot;file='ui/展开.png' &quot; selectedimage=&quot;file='ui/未展开.png'&quot; selectedhotimage=&quot;file='ui/未展开.png'&quot;"));
					pNode_4[n * 2]->SetAttribute(_T("itemattr"), _T("padding=\"16,0,0,0\" align=\"left\" font=\"13\""));//font=&quot;6&quot;
					pNode_4[n * 2]->SetName(_T("WX1"));
					pNode_3[n]->AddChildNode(pNode_4[n * 2]);

					pNode_4[n * 2 + 1] = new CTreeNodeUI();
					pNode_4[n * 2 + 1]->CheckBoxSelected(false);
					pNode_4[n * 2 + 1]->SetItemText(stringToWstring(vecVec_WxChildName[wxChildSeuque][1]).c_str());//wxChildSeuque
					pNode_4[n * 2 + 1]->SetFixedHeight(30);
					pNode_4[n * 2 + 1]->SetFixedWidth(100);
					pNode_4[n * 2 + 1]->SetItemTextColor(0xFFFFFFFF);
					pNode_4[n * 2 + 1]->SetItemHotTextColor(0xFFFFFFFF);
					pNode_4[n * 2 + 1]->SetSelItemTextColor(0xFFFFFFFF);
					pNode_4[n * 2 + 1]->SetAttribute(_T("folderattr"), _T("padding=&quot;0,1,0,0&quot; width=&quot;16&quot; height=&quot;16&quot; normalimage=&quot;file='ui/展开.png' &quot; "));
					pNode_4[n * 2 + 1]->SetAttribute(_T("folderattr"), _T("hotimage=&quot;file='ui/展开.png' &quot; selectedimage=&quot;file='ui/未展开.png'&quot; selectedhotimage=&quot;file='ui/未展开.png'&quot;"));
					pNode_4[n * 2 + 1]->SetAttribute(_T("itemattr"), _T("padding=\"16,0,0,0\" align=\"left\" font=\"13\""));//font=&quot;6&quot;
					pNode_4[n * 2 + 1]->SetName(_T("WX1"));
					pNode_3[n]->AddChildNode(pNode_4[n * 2 + 1]);

					wxChildSeuque += 1;
				}
			}
		}
	}
}
//功能类别分类后还原
void CPopEditorialClass::Restore_XZ_Functional()
{
	vector<vector<string>> vecVec_WxChildName;
	int wxChildSeuque = 0;
	for (int i = 0; i < name_Level1.size(); i++)
	{
		vector<string> childName;
		int childNum = pNode_2[i]->GetCountChild();
		for (int j = 0; j < childNum; j++)
		{
			CDuiString wstr_name1 = pNode_2[i]->GetChildNode(j)->GetItemText();
			string str_name1 = wstringToString(wstr_name1.GetData());
			childName.push_back(str_name1);

			int childNum_wx = pNode_2[i]->GetChildNode(j)->GetCountChild();
			vector<string> vec;
			for (int m = 0; m < childNum_wx; m++)
			{
				CDuiString wstr_name3 = pNode_2[i]->GetChildNode(j)->GetChildNode(m)->GetItemText();
				string str_name3 = wstringToString(wstr_name3.GetData());
				vec.push_back(str_name3);
			}
			vecVec_WxChildName.push_back(vec);

		}

		CDuiString wstr_name = pNode_2[i]->GetItemText();
		string str_name = wstringToString(wstr_name.GetData());

		int num = GetNoodLevel_Root(name_Level1[i]);
		cout << num << endl;

		pNode_1[num]->Remove(pNode_2[i]);

		pNode_2[i] = new CTreeNodeUI();
		pNode_2[i]->CheckBoxSelected(false);
		pNode_2[i]->SetItemText(stringToWstring(str_name).c_str());
		pNode_2[i]->SetFixedHeight(30);
		pNode_2[i]->SetItemTextColor(0xFFFFFFFF);
		pNode_2[i]->SetItemHotTextColor(0xFFFFFFFF);
		pNode_2[i]->SetSelItemTextColor(0xFFFFFFFF);
		pNode_2[i]->SetAttribute(_T("folderattr"), _T("padding=&quot;0,1,0,0&quot; width=&quot;16&quot; height=&quot;16&quot; normalimage=&quot;file='ui/展开.png' &quot; "));
		pNode_2[i]->SetAttribute(_T("folderattr"), _T("hotimage=&quot;file='ui/展开.png' &quot; selectedimage=&quot;file='ui/未展开.png'&quot; selectedhotimage=&quot;file='ui/未展开.png'&quot;"));
		pNode_2[i]->SetAttribute(_T("itemattr"), _T("padding=\"16,0,0,0\" align=\"left\" font=\"13\""));//font=&quot;6&quot;
		pNode_2[i]->SetName(_T("WX1"));
		pNode_1[0]->AddChildNode(pNode_2[i]);

		for (int m = 0; m < childNum; m++)
		{
			for (int n = 0; n < name_Level2.size(); n++)
			{
				if (name_Level2[n] == childName[m])
				{

					pNode_3[n]->Remove(pNode_4[n * 2]);
					pNode_3[n]->Remove(pNode_4[n * 2 + 1]);

					pNode_2[i]->Remove(pNode_3[n]);
					pNode_3[n] = new CTreeNodeUI();
					pNode_3[n]->CheckBoxSelected(false);
					pNode_3[n]->SetItemText(stringToWstring(name_Level2[n]).c_str());
					pNode_3[n]->SetFixedHeight(30);
					pNode_3[n]->SetItemTextColor(0xFFFFFFFF);
					pNode_3[n]->SetItemHotTextColor(0xFFFFFFFF);
					pNode_3[n]->SetSelItemTextColor(0xFFFFFFFF);
					pNode_3[n]->SetAttribute(_T("folderattr"), _T("padding=&quot;0,1,0,0&quot; width=&quot;16&quot; height=&quot;16&quot; normalimage=&quot;file='ui/展开.png' &quot; "));
					pNode_3[n]->SetAttribute(_T("folderattr"), _T("hotimage=&quot;file='ui/展开.png' &quot; selectedimage=&quot;file='ui/未展开.png'&quot; selectedhotimage=&quot;file='ui/未展开.png'&quot;"));
					pNode_3[n]->SetAttribute(_T("itemattr"), _T("padding=\"16,0,0,0\" align=\"left\" font=\"13\""));//font=&quot;6&quot;
					pNode_3[n]->SetName(_T("WX1"));
					pNode_2[i]->AddChildNode(pNode_3[n]);

					pNode_4[n * 2] = new CTreeNodeUI();
					pNode_4[n * 2]->CheckBoxSelected(false);
					pNode_4[n * 2]->SetItemText(stringToWstring(vecVec_WxChildName[wxChildSeuque][0]).c_str());//wxChildSeuque
					pNode_4[n * 2]->SetFixedHeight(30);
					pNode_4[n * 2]->SetFixedWidth(100);
					pNode_4[n * 2]->SetItemTextColor(0xFFFFFFFF);
					pNode_4[n * 2]->SetItemHotTextColor(0xFFFFFFFF);
					pNode_4[n * 2]->SetSelItemTextColor(0xFFFFFFFF);
					pNode_4[n * 2]->SetAttribute(_T("folderattr"), _T("padding=&quot;0,1,0,0&quot; width=&quot;16&quot; height=&quot;16&quot; normalimage=&quot;file='ui/展开.png' &quot; "));
					pNode_4[n * 2]->SetAttribute(_T("folderattr"), _T("hotimage=&quot;file='ui/展开.png' &quot; selectedimage=&quot;file='ui/未展开.png'&quot; selectedhotimage=&quot;file='ui/未展开.png'&quot;"));
					pNode_4[n * 2]->SetAttribute(_T("itemattr"), _T("padding=\"16,0,0,0\" align=\"left\" font=\"13\""));//font=&quot;6&quot;
					pNode_4[n * 2]->SetName(_T("WX1"));
					pNode_3[n]->AddChildNode(pNode_4[n * 2]);

					pNode_4[n * 2 + 1] = new CTreeNodeUI();
					pNode_4[n * 2 + 1]->CheckBoxSelected(false);
					pNode_4[n * 2 + 1]->SetItemText(stringToWstring(vecVec_WxChildName[wxChildSeuque][1]).c_str());//wxChildSeuque
					pNode_4[n * 2 + 1]->SetFixedHeight(30);
					pNode_4[n * 2 + 1]->SetFixedWidth(100);
					pNode_4[n * 2 + 1]->SetItemTextColor(0xFFFFFFFF);
					pNode_4[n * 2 + 1]->SetItemHotTextColor(0xFFFFFFFF);
					pNode_4[n * 2 + 1]->SetSelItemTextColor(0xFFFFFFFF);
					pNode_4[n * 2 + 1]->SetAttribute(_T("folderattr"), _T("padding=&quot;0,1,0,0&quot; width=&quot;16&quot; height=&quot;16&quot; normalimage=&quot;file='ui/展开.png' &quot; "));
					pNode_4[n * 2 + 1]->SetAttribute(_T("folderattr"), _T("hotimage=&quot;file='ui/展开.png' &quot; selectedimage=&quot;file='ui/未展开.png'&quot; selectedhotimage=&quot;file='ui/未展开.png'&quot;"));
					pNode_4[n * 2 + 1]->SetAttribute(_T("itemattr"), _T("padding=\"16,0,0,0\" align=\"left\" font=\"13\""));//font=&quot;6&quot;
					pNode_4[n * 2 + 1]->SetName(_T("WX1"));
					pNode_3[n]->AddChildNode(pNode_4[n * 2 + 1]);

					wxChildSeuque += 1;
				}
			}
		}
	}
	RootNodeOnly1();
}

//按照载荷类型分类
void CPopEditorialClass::Categorize_XZ_LoadType()
{
	vector<vector<string>> vecVec_WxChildName;
	int wxChildSeuque = 0;
	for (int i = 0; i < name_Level1.size(); i++)
	{
		CDuiString wstr_name = pNode_2[i]->GetItemText();
		string str_name = wstringToString(wstr_name.GetData());
		vector<string> childName;
		//vector<int> 
		/************************************/
		int childNum = pNode_2[i]->GetCountChild();
		for (int j = 0; j < childNum; j++)
		{
			CDuiString wstr_name1 = pNode_2[i]->GetChildNode(j)->GetItemText();
			string str_name1 = wstringToString(wstr_name1.GetData());
			childName.push_back(str_name1);

			int childNum_wx = pNode_2[i]->GetChildNode(j)->GetCountChild();
			vector<string> vec;
			for (int m = 0; m < childNum_wx; m++)
			{
				CDuiString wstr_name3 = pNode_2[i]->GetChildNode(j)->GetChildNode(m)->GetItemText();
				string str_name3 = wstringToString(wstr_name3.GetData());
				vec.push_back(str_name3);
			}
			vecVec_WxChildName.push_back(vec);
		}
		/***************移除场景里的1级节点******************/
		pNode_1[0]->Remove(pNode_2[i]);
		/***************新建筛选后的2级节点******************/
		pNode_2[i] = new CTreeNodeUI();
		pNode_2[i]->CheckBoxSelected(false);
		pNode_2[i]->SetItemText(stringToWstring(str_name).c_str());
		pNode_2[i]->SetFixedHeight(30);
		pNode_2[i]->SetItemTextColor(0xFFFFFFFF);
		pNode_2[i]->SetItemHotTextColor(0xFFFFFFFF);
		pNode_2[i]->SetSelItemTextColor(0xFFFFFFFF);
		pNode_2[i]->SetAttribute(_T("folderattr"), _T("padding=&quot;0,1,0,0&quot; width=&quot;16&quot; height=&quot;16&quot; normalimage=&quot;file='ui/展开.png' &quot; "));
		pNode_2[i]->SetAttribute(_T("folderattr"), _T("hotimage=&quot;file='ui/展开.png' &quot; selectedimage=&quot;file='ui/未展开.png'&quot; selectedhotimage=&quot;file='ui/未展开.png'&quot;"));
		pNode_2[i]->SetAttribute(_T("itemattr"), _T("padding=\"16,0,0,0\" align=\"left\" font=\"13\""));//font=&quot;6&quot;
		pNode_2[i]->SetName(_T("WX1"));
		
		map<string, string> curr_map = m_RecvMap_XZ[i];
		if (curr_map.count("有效载荷") > 0)
		{
			string value = curr_map["有效载荷"];
			int sequence = GetNoodLevel(UTF8_To_string(value));
			if (sequence >= 0)
			{
				pNode_1[sequence]->AddChildNode(pNode_2[i]);
			}
			else
			{
				CreateTreeNode(UTF8_To_string(value));//向根节点加名称
				pNode_1[pNode_1_num - 1]->AddChildNode(pNode_2[i]);
			}

		}
		for (int m = 0; m < childNum; m++)
		{
			for (int n = 0; n < name_Level2.size(); n++)
			{
				if (name_Level2[n] == childName[m])
				{

					pNode_3[n]->Remove(pNode_4[n * 2]);
					pNode_3[n]->Remove(pNode_4[n * 2 + 1]);

					pNode_2[i]->Remove(pNode_3[n]);
					pNode_3[n] = new CTreeNodeUI();
					pNode_3[n]->CheckBoxSelected(false);
					pNode_3[n]->SetItemText(stringToWstring(name_Level2[n]).c_str());
					pNode_3[n]->SetFixedHeight(30);
					pNode_3[n]->SetItemTextColor(0xFFFFFFFF);
					pNode_3[n]->SetItemHotTextColor(0xFFFFFFFF);
					pNode_3[n]->SetSelItemTextColor(0xFFFFFFFF);
					pNode_3[n]->SetAttribute(_T("folderattr"), _T("padding=&quot;0,1,0,0&quot; width=&quot;16&quot; height=&quot;16&quot; normalimage=&quot;file='ui/展开.png' &quot; "));
					pNode_3[n]->SetAttribute(_T("folderattr"), _T("hotimage=&quot;file='ui/展开.png' &quot; selectedimage=&quot;file='ui/未展开.png'&quot; selectedhotimage=&quot;file='ui/未展开.png'&quot;"));
					pNode_3[n]->SetAttribute(_T("itemattr"), _T("padding=\"16,0,0,0\" align=\"left\" font=\"13\""));//font=&quot;6&quot;
					pNode_3[n]->SetName(_T("WX1"));
					pNode_2[i]->AddChildNode(pNode_3[n]);

					pNode_4[n * 2] = new CTreeNodeUI();
					pNode_4[n * 2]->CheckBoxSelected(false);
					pNode_4[n * 2]->SetItemText(stringToWstring(vecVec_WxChildName[wxChildSeuque][0]).c_str());//wxChildSeuque
					pNode_4[n * 2]->SetFixedHeight(30);
					pNode_4[n * 2]->SetFixedWidth(100);
					pNode_4[n * 2]->SetItemTextColor(0xFFFFFFFF);
					pNode_4[n * 2]->SetItemHotTextColor(0xFFFFFFFF);
					pNode_4[n * 2]->SetSelItemTextColor(0xFFFFFFFF);
					pNode_4[n * 2]->SetAttribute(_T("folderattr"), _T("padding=&quot;0,1,0,0&quot; width=&quot;16&quot; height=&quot;16&quot; normalimage=&quot;file='ui/展开.png' &quot; "));
					pNode_4[n * 2]->SetAttribute(_T("folderattr"), _T("hotimage=&quot;file='ui/展开.png' &quot; selectedimage=&quot;file='ui/未展开.png'&quot; selectedhotimage=&quot;file='ui/未展开.png'&quot;"));
					pNode_4[n * 2]->SetAttribute(_T("itemattr"), _T("padding=\"16,0,0,0\" align=\"left\" font=\"13\""));//font=&quot;6&quot;
					pNode_4[n * 2]->SetName(_T("WX1"));
					pNode_3[n]->AddChildNode(pNode_4[n * 2]);

					pNode_4[n * 2 + 1] = new CTreeNodeUI();
					pNode_4[n * 2 + 1]->CheckBoxSelected(false);
					pNode_4[n * 2 + 1]->SetItemText(stringToWstring(vecVec_WxChildName[wxChildSeuque][1]).c_str());//wxChildSeuque
					pNode_4[n * 2 + 1]->SetFixedHeight(30);
					pNode_4[n * 2 + 1]->SetFixedWidth(100);
					pNode_4[n * 2 + 1]->SetItemTextColor(0xFFFFFFFF);
					pNode_4[n * 2 + 1]->SetItemHotTextColor(0xFFFFFFFF);
					pNode_4[n * 2 + 1]->SetSelItemTextColor(0xFFFFFFFF);
					pNode_4[n * 2 + 1]->SetAttribute(_T("folderattr"), _T("padding=&quot;0,1,0,0&quot; width=&quot;16&quot; height=&quot;16&quot; normalimage=&quot;file='ui/展开.png' &quot; "));
					pNode_4[n * 2 + 1]->SetAttribute(_T("folderattr"), _T("hotimage=&quot;file='ui/展开.png' &quot; selectedimage=&quot;file='ui/未展开.png'&quot; selectedhotimage=&quot;file='ui/未展开.png'&quot;"));
					pNode_4[n * 2 + 1]->SetAttribute(_T("itemattr"), _T("padding=\"16,0,0,0\" align=\"left\" font=\"13\""));//font=&quot;6&quot;
					pNode_4[n * 2 + 1]->SetName(_T("WX1"));
					pNode_3[n]->AddChildNode(pNode_4[n * 2 + 1]);

					wxChildSeuque += 1;
				}
			}
		}
	}
}
//载荷类型分类后还原
void CPopEditorialClass::Restore_XZ_LoadType()
{
	vector<vector<string>> vecVec_WxChildName;
	int wxChildSeuque = 0;
	for (int i = 0; i < name_Level1.size(); i++)
	{
		vector<string> childName;
		int childNum = pNode_2[i]->GetCountChild();
		for (int j = 0; j < childNum; j++)
		{
			CDuiString wstr_name1 = pNode_2[i]->GetChildNode(j)->GetItemText();
			string str_name1 = wstringToString(wstr_name1.GetData());
			childName.push_back(str_name1);

			int childNum_wx = pNode_2[i]->GetChildNode(j)->GetCountChild();
			vector<string> vec;
			for (int m = 0; m < childNum_wx; m++)
			{
				CDuiString wstr_name3 = pNode_2[i]->GetChildNode(j)->GetChildNode(m)->GetItemText();
				string str_name3 = wstringToString(wstr_name3.GetData());
				vec.push_back(str_name3);
			}
			vecVec_WxChildName.push_back(vec);
		}

		CDuiString wstr_name = pNode_2[i]->GetItemText();
		string str_name = wstringToString(wstr_name.GetData());

		int num = GetNoodLevel_Root(name_Level1[i]);
		cout << num << endl;

		pNode_1[num]->Remove(pNode_2[i]);

		pNode_2[i] = new CTreeNodeUI();
		pNode_2[i]->CheckBoxSelected(false);
		pNode_2[i]->SetItemText(stringToWstring(str_name).c_str());
		pNode_2[i]->SetFixedHeight(30);
		pNode_2[i]->SetItemTextColor(0xFFFFFFFF);
		pNode_2[i]->SetItemHotTextColor(0xFFFFFFFF);
		pNode_2[i]->SetSelItemTextColor(0xFFFFFFFF);
		pNode_2[i]->SetAttribute(_T("folderattr"), _T("padding=&quot;0,1,0,0&quot; width=&quot;16&quot; height=&quot;16&quot; normalimage=&quot;file='ui/展开.png' &quot; "));
		pNode_2[i]->SetAttribute(_T("folderattr"), _T("hotimage=&quot;file='ui/展开.png' &quot; selectedimage=&quot;file='ui/未展开.png'&quot; selectedhotimage=&quot;file='ui/未展开.png'&quot;"));
		pNode_2[i]->SetAttribute(_T("itemattr"), _T("padding=\"16,0,0,0\" align=\"left\" font=\"13\""));//font=&quot;6&quot;
		pNode_2[i]->SetName(_T("WX1"));
		pNode_1[0]->AddChildNode(pNode_2[i]);

		for (int m = 0; m < childNum; m++)
		{
			for (int n = 0; n < name_Level2.size(); n++)
			{
				if (name_Level2[n] == childName[m])
				{

					pNode_3[n]->Remove(pNode_4[n * 2]);
					pNode_3[n]->Remove(pNode_4[n * 2 + 1]);

					pNode_2[i]->Remove(pNode_3[n]);
					pNode_3[n] = new CTreeNodeUI();
					pNode_3[n]->CheckBoxSelected(false);
					pNode_3[n]->SetItemText(stringToWstring(name_Level2[n]).c_str());
					pNode_3[n]->SetFixedHeight(30);
					pNode_3[n]->SetItemTextColor(0xFFFFFFFF);
					pNode_3[n]->SetItemHotTextColor(0xFFFFFFFF);
					pNode_3[n]->SetSelItemTextColor(0xFFFFFFFF);
					pNode_3[n]->SetAttribute(_T("folderattr"), _T("padding=&quot;0,1,0,0&quot; width=&quot;16&quot; height=&quot;16&quot; normalimage=&quot;file='ui/展开.png' &quot; "));
					pNode_3[n]->SetAttribute(_T("folderattr"), _T("hotimage=&quot;file='ui/展开.png' &quot; selectedimage=&quot;file='ui/未展开.png'&quot; selectedhotimage=&quot;file='ui/未展开.png'&quot;"));
					pNode_3[n]->SetAttribute(_T("itemattr"), _T("padding=\"16,0,0,0\" align=\"left\" font=\"13\""));//font=&quot;6&quot;
					pNode_3[n]->SetName(_T("WX1"));
					pNode_2[i]->AddChildNode(pNode_3[n]);

					pNode_4[n * 2] = new CTreeNodeUI();
					pNode_4[n * 2]->CheckBoxSelected(false);
					pNode_4[n * 2]->SetItemText(stringToWstring(vecVec_WxChildName[wxChildSeuque][0]).c_str());//wxChildSeuque
					pNode_4[n * 2]->SetFixedHeight(30);
					pNode_4[n * 2]->SetFixedWidth(100);
					pNode_4[n * 2]->SetItemTextColor(0xFFFFFFFF);
					pNode_4[n * 2]->SetItemHotTextColor(0xFFFFFFFF);
					pNode_4[n * 2]->SetSelItemTextColor(0xFFFFFFFF);
					pNode_4[n * 2]->SetAttribute(_T("folderattr"), _T("padding=&quot;0,1,0,0&quot; width=&quot;16&quot; height=&quot;16&quot; normalimage=&quot;file='ui/展开.png' &quot; "));
					pNode_4[n * 2]->SetAttribute(_T("folderattr"), _T("hotimage=&quot;file='ui/展开.png' &quot; selectedimage=&quot;file='ui/未展开.png'&quot; selectedhotimage=&quot;file='ui/未展开.png'&quot;"));
					pNode_4[n * 2]->SetAttribute(_T("itemattr"), _T("padding=\"16,0,0,0\" align=\"left\" font=\"13\""));//font=&quot;6&quot;
					pNode_4[n * 2]->SetName(_T("WX1"));
					pNode_3[n]->AddChildNode(pNode_4[n * 2]);

					pNode_4[n * 2 + 1] = new CTreeNodeUI();
					pNode_4[n * 2 + 1]->CheckBoxSelected(false);
					pNode_4[n * 2 + 1]->SetItemText(stringToWstring(vecVec_WxChildName[wxChildSeuque][1]).c_str());//wxChildSeuque
					pNode_4[n * 2 + 1]->SetFixedHeight(30);
					pNode_4[n * 2 + 1]->SetFixedWidth(100);
					pNode_4[n * 2 + 1]->SetItemTextColor(0xFFFFFFFF);
					pNode_4[n * 2 + 1]->SetItemHotTextColor(0xFFFFFFFF);
					pNode_4[n * 2 + 1]->SetSelItemTextColor(0xFFFFFFFF);
					pNode_4[n * 2 + 1]->SetAttribute(_T("folderattr"), _T("padding=&quot;0,1,0,0&quot; width=&quot;16&quot; height=&quot;16&quot; normalimage=&quot;file='ui/展开.png' &quot; "));
					pNode_4[n * 2 + 1]->SetAttribute(_T("folderattr"), _T("hotimage=&quot;file='ui/展开.png' &quot; selectedimage=&quot;file='ui/未展开.png'&quot; selectedhotimage=&quot;file='ui/未展开.png'&quot;"));
					pNode_4[n * 2 + 1]->SetAttribute(_T("itemattr"), _T("padding=\"16,0,0,0\" align=\"left\" font=\"13\""));//font=&quot;6&quot;
					pNode_4[n * 2 + 1]->SetName(_T("WX1"));
					pNode_3[n]->AddChildNode(pNode_4[n * 2 + 1]);

					wxChildSeuque += 1;
				}
			}
		}
	}
	RootNodeOnly1();

}
//查看当前是否有星座在分类
void CPopEditorialClass::Restore_XZ_classification()
{
	int serialNumber = 0;

	for (int i = 0; i < vec_ExpandFlags.size(); i++)
	{
		if (vec_ExpandFlags[i])
		{
			Restore_WX_LoadFunction(name_Level1[i]);
			vec_ExpandFlags[serialNumber] = FALSE;
		}
	}


}
//查看当前是否有电信港在分类
void CPopEditorialClass::Restore_DXG_classification()
{
	int serialNumber = 0;

	for (int i = 0; i < vec_ExpandFlags2_FT.size(); i++)
	{
		if (vec_ExpandFlags2_FT[i])
		{
			Restore_DQZ_FunctionType1(name2_Level1[i]);
			vec_ExpandFlags2_FT[serialNumber] = FALSE;
		}
	}

	for (int i = 0; i < vec_ExpandFlags2_ES.size(); i++)
	{
		if (vec_ExpandFlags2_ES[i])
		{
			Restore_DQZ_FunctionType1(name2_Level1[i]);
			vec_ExpandFlags2_ES[serialNumber] = FALSE;
		}
	}
}
//卫星-----按照载荷功能分类
void CPopEditorialClass::Categorize_WX_LoadFunction(string name)
{
	//string str = name;
	vector<string> childName;//存放子节点名称
	vector<vector<string>> vec_wxchildName;//存放卫星子节点名称
	vector<int> childSerialNum;//子节点所在pNode_3中的序号
	int serialNumber;//存放选择的星座名称
	CTreeNodeUI* pNode[1024];//卫星筛选的时候的三级节点 存放类型的
	CTreeNodeUI* pNode_T4[1024];//存放四层节点

	CTreeNodeUI* pNode_T5[1024];//存放卫星节点子节点
	int level_4 = 0;	
	int level_5 = 0;
	/*******************筛选星座名称在存放星座名称的vector中的位置***************/
	for (int i = 0; i < name_Level1.size(); i++)
	{
		if (name == name_Level1[i])
		{
			serialNumber = i;
		}
	}
	int childNum = pNode_2[serialNumber]->GetCountChild();
	/**********************筛选当前星座下面的卫星名称***********************/
	for (int j = 0; j < childNum; j++)
	{
		CDuiString wstr_name1 = pNode_2[serialNumber]->GetChildNode(j)->GetItemText();
		string str_name1 = wstringToString(wstr_name1.GetData());
		childName.push_back(str_name1);
		/******************筛选当前星座下卫星所在pNode_3中的顺序***********/
		for (int m = 0; m < name_Level2.size(); m++)
		{
			if (str_name1 == name_Level2[m])
			{
				childSerialNum.push_back(m);

			}
		}
		/***********************************************/
		//pNode_1[0]->Remove(pNode_2[i]);
	}
	/***********************筛选卫星下面的子节点名称*****************************/
	for (int i = 0; i < childSerialNum.size(); i++)
	{
		int childN = pNode_3[childSerialNum[i]]->GetCountChild();//->GetChildNode(j)->GetItemText();
		vector<string> childName_1;

		for (int j = 0; j < childN; j++)
		{
			CDuiString wstr_name1 = pNode_3[childSerialNum[i]]->GetChildNode(j)->GetItemText();
			string str_name1 = wstringToString(wstr_name1.GetData());
			childName_1.push_back(str_name1);
		}
		//string str_name1 = wstringToString(wstr_name1.GetData());
		vec_wxchildName.push_back(childName_1);

	}
	/***********************删除卫星下面的子节点******************************/
	for (int j = 0; j < childSerialNum.size(); j++)
	{
		pNode_3[childSerialNum[j]]->Remove(pNode_4[childSerialNum[j]*2+1]);
		pNode_3[childSerialNum[j]]->Remove(pNode_4[childSerialNum[j] * 2]);
	}
	/***********************删除星座下的卫星***********************/
	for (int j = 0; j < childSerialNum.size(); j++)
	{
		pNode_2[serialNumber]->Remove(pNode_3[childSerialNum[j]]);
	}
	/******************************************************/
	for (int j = 0; j < childSerialNum.size(); j++)
	{
		map<string, string> curr_map = m_RecvMap[j];

		if (curr_map.count("卫星载荷名称") > 0)
		{
			string value = curr_map["卫星载荷名称"];
			int sequence = GetNoodLevel_level2(UTF8_To_string(value), serialNumber);
			if (sequence >= 0)
			{
				cout << "111111111111" << endl;
				pNode_T4[level_4] = new CTreeNodeUI();
				pNode_T4[level_4]->CheckBoxSelected(false);
				pNode_T4[level_4]->SetItemText(stringToWstring(childName[j]).c_str());
				pNode_T4[level_4]->SetFixedHeight(30);
				pNode_T4[level_4]->SetItemTextColor(0xFFFFFFFF);
				pNode_T4[level_4]->SetItemHotTextColor(0xFFFFFFFF);
				pNode_T4[level_4]->SetSelItemTextColor(0xFFFFFFFF);
				pNode_T4[level_4]->SetAttribute(_T("folderattr"), _T("padding=&quot;0,1,0,0&quot; width=&quot;16&quot; height=&quot;16&quot; normalimage=&quot;file='ui/展开.png' &quot; "));
				pNode_T4[level_4]->SetAttribute(_T("folderattr"), _T("hotimage=&quot;file='ui/展开.png' &quot; selectedimage=&quot;file='ui/未展开.png'&quot; selectedhotimage=&quot;file='ui/未展开.png'&quot;"));
				pNode_T4[level_4]->SetAttribute(_T("itemattr"), _T("padding=\"16,0,0,0\" align=\"left\" font=\"13\""));//font=&quot;6&quot;
				pNode_T4[level_4]->SetName(_T("WX1"));

				pNode[sequence]->AddChildNode(pNode_T4[level_4]);

				pNode_T5[level_5] = new CTreeNodeUI();
				pNode_T5[level_5]->CheckBoxSelected(false);
				pNode_T5[level_5]->SetItemText(stringToWstring(vec_wxchildName[j][0]).c_str());
				pNode_T5[level_5]->SetFixedHeight(30);
				pNode_T5[level_5]->SetItemTextColor(0xFFFFFFFF);
				pNode_T5[level_5]->SetItemHotTextColor(0xFFFFFFFF);
				pNode_T5[level_4]->SetSelItemTextColor(0xFFFFFFFF);
				pNode_T5[level_5]->SetAttribute(_T("folderattr"), _T("padding=&quot;0,1,0,0&quot; width=&quot;16&quot; height=&quot;16&quot; normalimage=&quot;file='ui/展开.png' &quot; "));
				pNode_T5[level_5]->SetAttribute(_T("folderattr"), _T("hotimage=&quot;file='ui/展开.png' &quot; selectedimage=&quot;file='ui/未展开.png'&quot; selectedhotimage=&quot;file='ui/未展开.png'&quot;"));
				pNode_T5[level_5]->SetAttribute(_T("itemattr"), _T("padding=\"16,0,0,0\" align=\"left\" font=\"13\""));//font=&quot;6&quot;
				pNode_T5[level_5]->SetName(_T("WX1"));

				pNode_T4[level_4]->AddChildNode(pNode_T5[level_5]);
				level_5 += 1;

				pNode_T5[level_5] = new CTreeNodeUI();
				pNode_T5[level_5]->CheckBoxSelected(false);
				pNode_T5[level_5]->SetItemText(stringToWstring(vec_wxchildName[j][1]).c_str());
				pNode_T5[level_5]->SetFixedHeight(30);
				pNode_T5[level_5]->SetItemTextColor(0xFFFFFFFF);
				pNode_T5[level_5]->SetItemHotTextColor(0xFFFFFFFF);
				pNode_T5[level_4]->SetSelItemTextColor(0xFFFFFFFF);
				pNode_T5[level_5]->SetAttribute(_T("folderattr"), _T("padding=&quot;0,1,0,0&quot; width=&quot;16&quot; height=&quot;16&quot; normalimage=&quot;file='ui/展开.png' &quot; "));
				pNode_T5[level_5]->SetAttribute(_T("folderattr"), _T("hotimage=&quot;file='ui/展开.png' &quot; selectedimage=&quot;file='ui/未展开.png'&quot; selectedhotimage=&quot;file='ui/未展开.png'&quot;"));
				pNode_T5[level_5]->SetAttribute(_T("itemattr"), _T("padding=\"16,0,0,0\" align=\"left\" font=\"13\""));//font=&quot;6&quot;
				pNode_T5[level_5]->SetName(_T("WX1"));

				pNode_T4[level_4]->AddChildNode(pNode_T5[level_5]);
				level_5 += 1;
				level_4 += 1;
			}
			else
			{
				pNode[j] = new CTreeNodeUI();
				pNode[j]->CheckBoxSelected(false);
				pNode[j]->SetItemText(stringToWstring(UTF8_To_string(value)).c_str());
				pNode[j]->SetFixedHeight(30);
				pNode[j]->SetItemTextColor(0xFFFFFFFF);
				pNode[j]->SetItemHotTextColor(0xFFFFFFFF);
				pNode[j]->SetSelItemTextColor(0xFFFFFFFF);
				pNode[j]->SetAttribute(_T("folderattr"), _T("padding=&quot;0,1,0,0&quot; width=&quot;16&quot; height=&quot;16&quot; normalimage=&quot;file='ui/展开.png' &quot; "));
				pNode[j]->SetAttribute(_T("folderattr"), _T("hotimage=&quot;file='ui/展开.png' &quot; selectedimage=&quot;file='ui/未展开.png'&quot; selectedhotimage=&quot;file='ui/未展开.png'&quot;"));
				pNode[j]->SetAttribute(_T("itemattr"), _T("padding=\"16,0,0,0\" align=\"left\" font=\"13\""));//font=&quot;6&quot;
				pNode[j]->SetName(_T("WX1"));
				pNode_2[serialNumber]->AddChildNode(pNode[j]);

				pNode_T4[level_4] = new CTreeNodeUI();
				pNode_T4[level_4]->CheckBoxSelected(false);
				pNode_T4[level_4]->SetItemText(stringToWstring(childName[j]).c_str());
				pNode_T4[level_4]->SetFixedHeight(30);
				pNode_T4[level_4]->SetItemTextColor(0xFFFFFFFF);
				pNode_T4[level_4]->SetItemHotTextColor(0xFFFFFFFF);
				pNode_T4[level_4]->SetSelItemTextColor(0xFFFFFFFF);
				pNode_T4[level_4]->SetAttribute(_T("folderattr"), _T("padding=&quot;0,1,0,0&quot; width=&quot;16&quot; height=&quot;16&quot; normalimage=&quot;file='ui/展开.png' &quot; "));
				pNode_T4[level_4]->SetAttribute(_T("folderattr"), _T("hotimage=&quot;file='ui/展开.png' &quot; selectedimage=&quot;file='ui/未展开.png'&quot; selectedhotimage=&quot;file='ui/未展开.png'&quot;"));
				pNode_T4[level_4]->SetAttribute(_T("itemattr"), _T("padding=\"16,0,0,0\" align=\"left\" font=\"13\""));//font=&quot;6&quot;
				pNode_T4[level_4]->SetName(_T("WX1"));

				pNode[j]->AddChildNode(pNode_T4[level_4]);


				pNode_T5[level_5] = new CTreeNodeUI();
				pNode_T5[level_5]->CheckBoxSelected(false);
				pNode_T5[level_5]->SetItemText(stringToWstring(vec_wxchildName[j][0]).c_str());
				pNode_T5[level_5]->SetFixedHeight(30);
				pNode_T5[level_5]->SetItemTextColor(0xFFFFFFFF);
				pNode_T5[level_5]->SetItemHotTextColor(0xFFFFFFFF);
				pNode_T5[level_4]->SetSelItemTextColor(0xFFFFFFFF);
				pNode_T5[level_5]->SetAttribute(_T("folderattr"), _T("padding=&quot;0,1,0,0&quot; width=&quot;16&quot; height=&quot;16&quot; normalimage=&quot;file='ui/展开.png' &quot; "));
				pNode_T5[level_5]->SetAttribute(_T("folderattr"), _T("hotimage=&quot;file='ui/展开.png' &quot; selectedimage=&quot;file='ui/未展开.png'&quot; selectedhotimage=&quot;file='ui/未展开.png'&quot;"));
				pNode_T5[level_5]->SetAttribute(_T("itemattr"), _T("padding=\"16,0,0,0\" align=\"left\" font=\"13\""));//font=&quot;6&quot;
				pNode_T5[level_5]->SetName(_T("WX1"));

				pNode_T4[level_4]->AddChildNode(pNode_T5[level_5]);
				level_5 += 1;

				pNode_T5[level_5] = new CTreeNodeUI();
				pNode_T5[level_5]->CheckBoxSelected(false);
				pNode_T5[level_5]->SetItemText(stringToWstring(vec_wxchildName[j][1]).c_str());
				pNode_T5[level_5]->SetFixedHeight(30);
				pNode_T5[level_5]->SetItemTextColor(0xFFFFFFFF);
				pNode_T5[level_5]->SetItemHotTextColor(0xFFFFFFFF);
				pNode_T5[level_4]->SetSelItemTextColor(0xFFFFFFFF);
				pNode_T5[level_5]->SetAttribute(_T("folderattr"), _T("padding=&quot;0,1,0,0&quot; width=&quot;16&quot; height=&quot;16&quot; normalimage=&quot;file='ui/展开.png' &quot; "));
				pNode_T5[level_5]->SetAttribute(_T("folderattr"), _T("hotimage=&quot;file='ui/展开.png' &quot; selectedimage=&quot;file='ui/未展开.png'&quot; selectedhotimage=&quot;file='ui/未展开.png'&quot;"));
				pNode_T5[level_5]->SetAttribute(_T("itemattr"), _T("padding=\"16,0,0,0\" align=\"left\" font=\"13\""));//font=&quot;6&quot;
				pNode_T5[level_5]->SetName(_T("WX1"));

				pNode_T4[level_4]->AddChildNode(pNode_T5[level_5]);
				level_5 += 1;
				level_4 += 1;

			}

		}
	}

}
//卫星------按载荷功能分类后还原
void CPopEditorialClass::Restore_WX_LoadFunction(string name)
{
	//string str = name;
	vector<string> childName;//存放子节点名称
	vector<vector<string>> vec_wxchildName;//存放卫星子节点名称
	vector<int> childSerialNum;//子节点所在pNode_3中的序号
	int serialNumber;//存放选择的星座名称
	CTreeNodeUI* pNode[1024];
	int serialNum_wx;
	/*******************筛选星座名称在存放星座名称的vector中的位置***************/
	for (int i = 0; i < name_Level1.size(); i++)
	{
		if (name == name_Level1[i])
		{
			serialNumber = i;
		}
		//pNode_2[i]
	}
	/*************************筛选子节点的子节点包含的卫星名称*********************************/
	int childNum = pNode_2[serialNumber]->GetCountChild();
	for (int i = 0; i < childNum; i++)
	{
		CTreeNodeUI* pNode_level3 = pNode_2[serialNumber]->GetChildNode(i);
		int childNum1 = pNode_level3->GetCountChild();
		//pNode_level3->RemoveAll();
		for (int j = 0; j < childNum1; j++)
		{
			vector<string > vec;
			CDuiString cdui_name = pNode_level3->GetChildNode(j)->GetItemText();
			string str_name = wstringToString(cdui_name.GetData());
			childName.push_back(str_name);//存放的是卫星
			int wxChildNum = pNode_level3->GetChildNode(j)->GetCountChild();
			for (int m = 0; m < wxChildNum; m++)
			{
				CDuiString cdui_name1 = pNode_level3->GetChildNode(j)->GetChildNode(m)->GetItemText();
				string str_name1 = wstringToString(cdui_name1.GetData());
				vec.push_back(str_name1);//存放的是卫星
			}
			vec_wxchildName.push_back(vec);
		}
	}
	/********************删除2级节点******************/
	for (int i = childNum; i > 0; i--)
	{
		pNode_2[serialNumber]->Remove(pNode_2[serialNumber]->GetChildNode(i-1));
	}
	/*********************新建恢复为原来的样子**********************/
	int m = 0;
	for (int i = 0; i < childName.size(); i++)
	{
		
		for (int j = 0; j < name_Level2.size(); j++)
		{

			if (childName[i] == name_Level2[j])
			{
				serialNum_wx = j;
				pNode_3[serialNum_wx] = new CTreeNodeUI();
				pNode_3[serialNum_wx]->CheckBoxSelected(false);
				pNode_3[serialNum_wx]->SetItemText(stringToWstring(childName[i]).c_str());
				pNode_3[serialNum_wx]->SetFixedHeight(30);
				pNode_3[serialNum_wx]->SetItemTextColor(0xFFFFFFFF);
				pNode_3[serialNum_wx]->SetItemHotTextColor(0xFFFFFFFF);
				pNode_3[serialNum_wx]->SetSelItemTextColor(0xFFFFFFFF);
				pNode_3[serialNum_wx]->SetAttribute(_T("folderattr"), _T("padding=&quot;0,1,0,0&quot; width=&quot;16&quot; height=&quot;16&quot; normalimage=&quot;file='ui/展开.png' &quot; "));
				pNode_3[serialNum_wx]->SetAttribute(_T("folderattr"), _T("hotimage=&quot;file='ui/展开.png' &quot; selectedimage=&quot;file='ui/未展开.png'&quot; selectedhotimage=&quot;file='ui/未展开.png'&quot;"));
				pNode_3[serialNum_wx]->SetAttribute(_T("itemattr"), _T("padding=\"16,0,0,0\" align=\"left\" font=\"13\""));//font=&quot;6&quot;
				pNode_3[serialNum_wx]->SetName(_T("WX1"));
				pNode_2[serialNumber]->AddChildNode(pNode_3[serialNum_wx]);

				//for (int m = 0; m < vec_wxchildName.size(); m++)
				//{
					pNode_4[serialNum_wx * 2] = new CTreeNodeUI();
					pNode_4[serialNum_wx * 2]->CheckBoxSelected(false);
					pNode_4[serialNum_wx * 2]->SetItemText(stringToWstring(vec_wxchildName[m][0]).c_str());
					pNode_4[serialNum_wx * 2]->SetFixedHeight(30);
					pNode_4[serialNum_wx * 2]->SetItemTextColor(0xFFFFFFFF);
					pNode_4[serialNum_wx * 2]->SetItemHotTextColor(0xFFFFFFFF);
					pNode_4[serialNum_wx * 2]->SetSelItemTextColor(0xFFFFFFFF);
					pNode_4[serialNum_wx * 2]->SetAttribute(_T("folderattr"), _T("padding=&quot;0,1,0,0&quot; width=&quot;16&quot; height=&quot;16&quot; normalimage=&quot;file='ui/展开.png' &quot; "));
					pNode_4[serialNum_wx * 2]->SetAttribute(_T("folderattr"), _T("hotimage=&quot;file='ui/展开.png' &quot; selectedimage=&quot;file='ui/未展开.png'&quot; selectedhotimage=&quot;file='ui/未展开.png'&quot;"));
					pNode_4[serialNum_wx * 2]->SetAttribute(_T("itemattr"), _T("padding=\"16,0,0,0\" align=\"left\" font=\"13\""));//font=&quot;6&quot;
					pNode_4[serialNum_wx * 2]->SetName(_T("WX1"));
					pNode_3[serialNum_wx]->AddChildNode(pNode_4[serialNum_wx * 2]);

					pNode_4[serialNum_wx * 2 + 1] = new CTreeNodeUI();
					pNode_4[serialNum_wx * 2 + 1]->CheckBoxSelected(false);
					pNode_4[serialNum_wx * 2 + 1]->SetItemText(stringToWstring(vec_wxchildName[m][1]).c_str());
					pNode_4[serialNum_wx * 2 + 1]->SetFixedHeight(30);
					pNode_4[serialNum_wx * 2 + 1]->SetItemTextColor(0xFFFFFFFF);
					pNode_4[serialNum_wx * 2 + 1]->SetItemHotTextColor(0xFFFFFFFF);
					pNode_4[serialNum_wx * 2 + 1]->SetSelItemTextColor(0xFFFFFFFF);
					pNode_4[serialNum_wx * 2 + 1]->SetAttribute(_T("folderattr"), _T("padding=&quot;0,1,0,0&quot; width=&quot;16&quot; height=&quot;16&quot; normalimage=&quot;file='ui/展开.png' &quot; "));
					pNode_4[serialNum_wx * 2 + 1]->SetAttribute(_T("folderattr"), _T("hotimage=&quot;file='ui/展开.png' &quot; selectedimage=&quot;file='ui/未展开.png'&quot; selectedhotimage=&quot;file='ui/未展开.png'&quot;"));
					pNode_4[serialNum_wx * 2 + 1]->SetAttribute(_T("itemattr"), _T("padding=\"16,0,0,0\" align=\"left\" font=\"13\""));//font=&quot;6&quot;
					pNode_4[serialNum_wx * 2 + 1]->SetName(_T("WX1"));
					pNode_3[serialNum_wx]->AddChildNode(pNode_4[serialNum_wx * 2 + 1]);
				//}
					m += 1;
			}
	
			
		}
	}
}
//电信港按性质分类
void CPopEditorialClass::Categorize_DXG_characteristic()
{
	vector<vector<string>> vecVec_WxChildName;
	int wxChildSeuque = 0;//卫星子节点顺序
	for (int i = 0; i < name2_Level1.size(); i++)
	{
		CDuiString wstr_name = pNode_2_2[i]->GetItemText();
		string str_name = wstringToString(wstr_name.GetData());
		vector<string> childName;
		//vector<int> 
		/***************获取星座下面的卫星名称********************/
		int childNum = pNode_2_2[i]->GetCountChild();
		for (int j = 0; j < childNum; j++)
		{
			CDuiString wstr_name1 = pNode_2_2[i]->GetChildNode(j)->GetItemText();
			string str_name1 = wstringToString(wstr_name1.GetData());
			childName.push_back(str_name1);
			int childNum_wx = pNode_2_2[i]->GetChildNode(j)->GetCountChild();
			vector<string> vec;
			for (int m = 0; m < childNum_wx; m++)
			{
				CDuiString wstr_name3 = pNode_2_2[i]->GetChildNode(j)->GetChildNode(m)->GetItemText();
				string str_name3 = wstringToString(wstr_name3.GetData());
				vec.push_back(str_name3);
			}
			vecVec_WxChildName.push_back(vec);
		}
		/***************移除场景里的1级(星座层级)节点******************/
		pNode_2_1[0]->Remove(pNode_2_2[i]);
		/***************新建筛选后的2级节点******************/
		pNode_2_2[i] = new CTreeNodeUI();
		pNode_2_2[i]->CheckBoxSelected(false);
		pNode_2_2[i]->SetItemText(stringToWstring(str_name).c_str());
		pNode_2_2[i]->SetFixedHeight(30);
		pNode_2_2[i]->SetItemTextColor(0xFFFFFFFF);
		pNode_2_2[i]->SetItemHotTextColor(0xFFFFFFFF);
		pNode_2_2[i]->SetSelItemTextColor(0xFFFFFFFF);
		pNode_2_2[i]->SetAttribute(_T("folderattr"), _T("padding=&quot;0,1,0,0&quot; width=&quot;16&quot; height=&quot;16&quot; normalimage=&quot;file='ui/展开.png' &quot; "));
		pNode_2_2[i]->SetAttribute(_T("folderattr"), _T("hotimage=&quot;file='ui/展开.png' &quot; selectedimage=&quot;file='ui/未展开.png'&quot; selectedhotimage=&quot;file='ui/未展开.png'&quot;"));
		pNode_2_2[i]->SetAttribute(_T("itemattr"), _T("padding=\"16,0,0,0\" align=\"left\" font=\"13\""));//font=&quot;6&quot;
		pNode_2_2[i]->SetName(_T("WX1"));
		map<string, string> curr_map = m_RecvMap_DXG[i];
		if (curr_map.count("电信港性质") > 0)
		{
			string value = curr_map["电信港性质"];
			int sequence = GetNoodLevel_DXG_XZ(UTF8_To_string(value));
			if (sequence >= 0)
			{
				pNode_2_1[sequence]->AddChildNode(pNode_2_2[i]);
			}
			else
			{
				CreateTreeNode_2(UTF8_To_string(value));//向根节点加名称
				pNode_2_1[pNode_2_1_num - 1]->AddChildNode(pNode_2_2[i]);
			}

		}
		for (int m = 0; m < childNum; m++)//卫星数量
		{
			for (int n = 0; n < name2_Level2.size(); n++)
			{
				if (name2_Level2[n] == childName[m])
				{
					pNode_2_3[n]->Remove(pNode_2_4[n * 2]);
					pNode_2_3[n]->Remove(pNode_2_4[n * 2 + 1]);
					pNode_2_3[n]->Remove(pNode_2_4[n * 2 + 2]);

					pNode_2_2[i]->Remove(pNode_2_3[n]);
					pNode_2_3[n] = new CTreeNodeUI();
					pNode_2_3[n]->CheckBoxSelected(false);
					pNode_2_3[n]->SetItemText(stringToWstring(name2_Level2[n]).c_str());
					pNode_2_3[n]->SetFixedHeight(30);
					pNode_2_3[n]->SetItemTextColor(0xFFFFFFFF);
					pNode_2_3[n]->SetItemHotTextColor(0xFFFFFFFF);
					pNode_2_3[n]->SetSelItemTextColor(0xFFFFFFFF);
					pNode_2_3[n]->SetAttribute(_T("folderattr"), _T("padding=&quot;0,1,0,0&quot; width=&quot;16&quot; height=&quot;16&quot; normalimage=&quot;file='ui/展开.png' &quot; "));
					pNode_2_3[n]->SetAttribute(_T("folderattr"), _T("hotimage=&quot;file='ui/展开.png' &quot; selectedimage=&quot;file='ui/未展开.png'&quot; selectedhotimage=&quot;file='ui/未展开.png'&quot;"));
					pNode_2_3[n]->SetAttribute(_T("itemattr"), _T("padding=\"16,0,0,0\" align=\"left\" font=\"13\""));//font=&quot;6&quot;
					pNode_2_3[n]->SetName(_T("WX1"));
					pNode_2_2[i]->AddChildNode(pNode_2_3[n]);


					pNode_2_4[n * 2] = new CTreeNodeUI();
					pNode_2_4[n * 2]->CheckBoxSelected(false);
					pNode_2_4[n * 2]->SetItemText(stringToWstring(vecVec_WxChildName[wxChildSeuque][0]).c_str());//wxChildSeuque
					pNode_2_4[n * 2]->SetFixedHeight(30);
					pNode_2_4[n * 2]->SetFixedWidth(100);
					pNode_2_4[n * 2]->SetItemTextColor(0xFFFFFFFF);
					pNode_2_4[n * 2]->SetItemHotTextColor(0xFFFFFFFF);
					pNode_2_4[n * 2]->SetSelItemTextColor(0xFFFFFFFF);
					pNode_2_4[n * 2]->SetAttribute(_T("folderattr"), _T("padding=&quot;0,1,0,0&quot; width=&quot;16&quot; height=&quot;16&quot; normalimage=&quot;file='ui/展开.png' &quot; "));
					pNode_2_4[n * 2]->SetAttribute(_T("folderattr"), _T("hotimage=&quot;file='ui/展开.png' &quot; selectedimage=&quot;file='ui/未展开.png'&quot; selectedhotimage=&quot;file='ui/未展开.png'&quot;"));
					pNode_2_4[n * 2]->SetAttribute(_T("itemattr"), _T("padding=\"16,0,0,0\" align=\"left\" font=\"13\""));//font=&quot;6&quot;
					pNode_2_4[n * 2]->SetName(_T("WX1"));
					pNode_2_3[n]->AddChildNode(pNode_2_4[n * 2]);

					pNode_2_4[n * 2 + 1] = new CTreeNodeUI();
					pNode_2_4[n * 2 + 1]->CheckBoxSelected(false);
					pNode_2_4[n * 2 + 1]->SetItemText(stringToWstring(vecVec_WxChildName[wxChildSeuque][1]).c_str());//wxChildSeuque
					pNode_2_4[n * 2 + 1]->SetFixedHeight(30);
					pNode_2_4[n * 2 + 1]->SetFixedWidth(100);
					pNode_2_4[n * 2 + 1]->SetItemTextColor(0xFFFFFFFF);
					pNode_2_4[n * 2 + 1]->SetItemHotTextColor(0xFFFFFFFF);
					pNode_2_4[n * 2 + 1]->SetSelItemTextColor(0xFFFFFFFF);
					pNode_2_4[n * 2 + 1]->SetAttribute(_T("folderattr"), _T("padding=&quot;0,1,0,0&quot; width=&quot;16&quot; height=&quot;16&quot; normalimage=&quot;file='ui/展开.png' &quot; "));
					pNode_2_4[n * 2 + 1]->SetAttribute(_T("folderattr"), _T("hotimage=&quot;file='ui/展开.png' &quot; selectedimage=&quot;file='ui/未展开.png'&quot; selectedhotimage=&quot;file='ui/未展开.png'&quot;"));
					pNode_2_4[n * 2 + 1]->SetAttribute(_T("itemattr"), _T("padding=\"16,0,0,0\" align=\"left\" font=\"13\""));//font=&quot;6&quot;
					pNode_2_4[n * 2 + 1]->SetName(_T("WX1"));
					pNode_2_3[n]->AddChildNode(pNode_2_4[n * 2 + 1]);

					pNode_2_4[n * 2 + 2] = new CTreeNodeUI();
					pNode_2_4[n * 2 + 2]->CheckBoxSelected(false);
					pNode_2_4[n * 2 + 2]->SetItemText(stringToWstring(vecVec_WxChildName[wxChildSeuque][1]).c_str());//wxChildSeuque
					pNode_2_4[n * 2 + 2]->SetFixedHeight(30);
					pNode_2_4[n * 2 + 2]->SetFixedWidth(100);
					pNode_2_4[n * 2 + 2]->SetItemTextColor(0xFFFFFFFF);
					pNode_2_4[n * 2 + 2]->SetItemHotTextColor(0xFFFFFFFF);
					pNode_2_4[n * 2 + 2]->SetSelItemTextColor(0xFFFFFFFF);
					pNode_2_4[n * 2 + 2]->SetAttribute(_T("folderattr"), _T("padding=&quot;0,1,0,0&quot; width=&quot;16&quot; height=&quot;16&quot; normalimage=&quot;file='ui/展开.png' &quot; "));
					pNode_2_4[n * 2 + 2]->SetAttribute(_T("folderattr"), _T("hotimage=&quot;file='ui/展开.png' &quot; selectedimage=&quot;file='ui/未展开.png'&quot; selectedhotimage=&quot;file='ui/未展开.png'&quot;"));
					pNode_2_4[n * 2 + 2]->SetAttribute(_T("itemattr"), _T("padding=\"16,0,0,0\" align=\"left\" font=\"13\""));//font=&quot;6&quot;
					pNode_2_4[n * 2 + 2]->SetName(_T("WX1"));
					pNode_2_3[n]->AddChildNode(pNode_2_4[n * 2 + 2]);

					wxChildSeuque += 1;

				}
			}
		}


	}

}
//按照性质分类后还原
void CPopEditorialClass::Restore_DXG_characteristic()
{
	vector<vector<string>> vecVec_WxChildName;
	int wxChildSeuque = 0;
	for (int i = 0; i < name2_Level1.size(); i++)
	{
		vector<string> childName;
		int childNum = pNode_2_2[i]->GetCountChild();
		for (int j = 0; j < childNum; j++)
		{
			CDuiString wstr_name1 = pNode_2_2[i]->GetChildNode(j)->GetItemText();
			string str_name1 = wstringToString(wstr_name1.GetData());
			childName.push_back(str_name1);
			int childNum_wx = pNode_2_2[i]->GetChildNode(j)->GetCountChild();
			vector<string> vec;
			for (int m = 0; m < childNum_wx; m++)
			{
				CDuiString wstr_name3 = pNode_2_2[i]->GetChildNode(j)->GetChildNode(m)->GetItemText();
				string str_name3 = wstringToString(wstr_name3.GetData());
				vec.push_back(str_name3);
			}
			vecVec_WxChildName.push_back(vec);
		}

		CDuiString wstr_name = pNode_2_2[i]->GetItemText();
		string str_name = wstringToString(wstr_name.GetData());

		int num = GetNoodLevel_Root_DXG(name2_Level1[i]);
		cout << num << endl;

		pNode_2_1[num]->Remove(pNode_2_2[i]);

		pNode_2_2[i] = new CTreeNodeUI();
		pNode_2_2[i]->CheckBoxSelected(false);
		pNode_2_2[i]->SetItemText(stringToWstring(str_name).c_str());
		pNode_2_2[i]->SetFixedHeight(30);
		pNode_2_2[i]->SetItemTextColor(0xFFFFFFFF);
		pNode_2_2[i]->SetItemHotTextColor(0xFFFFFFFF);
		pNode_2_2[i]->SetSelItemTextColor(0xFFFFFFFF);
		pNode_2_2[i]->SetAttribute(_T("folderattr"), _T("padding=&quot;0,1,0,0&quot; width=&quot;16&quot; height=&quot;16&quot; normalimage=&quot;file='ui/展开.png' &quot; "));
		pNode_2_2[i]->SetAttribute(_T("folderattr"), _T("hotimage=&quot;file='ui/展开.png' &quot; selectedimage=&quot;file='ui/未展开.png'&quot; selectedhotimage=&quot;file='ui/未展开.png'&quot;"));
		pNode_2_2[i]->SetAttribute(_T("itemattr"), _T("padding=\"16,0,0,0\" align=\"left\" font=\"13\""));//font=&quot;6&quot;
		pNode_2_2[i]->SetName(_T("WX1"));
		pNode_2_1[0]->AddChildNode(pNode_2_2[i]);

		for (int m = 0; m < childNum; m++)
		{
			for (int n = 0; n < name2_Level2.size(); n++)
			{
				if (name2_Level2[n] == childName[m])
				{

					pNode_2_3[n]->Remove(pNode_2_4[n * 2]);
					pNode_2_3[n]->Remove(pNode_2_4[n * 2 + 1]);
					pNode_2_3[n]->Remove(pNode_2_4[n * 2 + 2]);

					pNode_2_2[i]->Remove(pNode_2_3[n]);
					pNode_2_3[n] = new CTreeNodeUI();
					pNode_2_3[n]->CheckBoxSelected(false);
					pNode_2_3[n]->SetItemText(stringToWstring(name2_Level2[n]).c_str());
					pNode_2_3[n]->SetFixedHeight(30);
					pNode_2_3[n]->SetItemTextColor(0xFFFFFFFF);
					pNode_2_3[n]->SetItemHotTextColor(0xFFFFFFFF);
					pNode_2_3[n]->SetSelItemTextColor(0xFFFFFFFF);
					pNode_2_3[n]->SetAttribute(_T("folderattr"), _T("padding=&quot;0,1,0,0&quot; width=&quot;16&quot; height=&quot;16&quot; normalimage=&quot;file='ui/展开.png' &quot; "));
					pNode_2_3[n]->SetAttribute(_T("folderattr"), _T("hotimage=&quot;file='ui/展开.png' &quot; selectedimage=&quot;file='ui/未展开.png'&quot; selectedhotimage=&quot;file='ui/未展开.png'&quot;"));
					pNode_2_3[n]->SetAttribute(_T("itemattr"), _T("padding=\"16,0,0,0\" align=\"left\" font=\"13\""));//font=&quot;6&quot;
					pNode_2_3[n]->SetName(_T("WX1"));
					pNode_2_2[i]->AddChildNode(pNode_2_3[n]);

					pNode_2_4[n * 2] = new CTreeNodeUI();
					pNode_2_4[n * 2]->CheckBoxSelected(false);
					pNode_2_4[n * 2]->SetItemText(stringToWstring(vecVec_WxChildName[wxChildSeuque][0]).c_str());//wxChildSeuque
					pNode_2_4[n * 2]->SetFixedHeight(30);
					pNode_2_4[n * 2]->SetFixedWidth(100);
					pNode_2_4[n * 2]->SetItemTextColor(0xFFFFFFFF);
					pNode_2_4[n * 2]->SetItemHotTextColor(0xFFFFFFFF);
					pNode_2_4[n * 2]->SetSelItemTextColor(0xFFFFFFFF);
					pNode_2_4[n * 2]->SetAttribute(_T("folderattr"), _T("padding=&quot;0,1,0,0&quot; width=&quot;16&quot; height=&quot;16&quot; normalimage=&quot;file='ui/展开.png' &quot; "));
					pNode_2_4[n * 2]->SetAttribute(_T("folderattr"), _T("hotimage=&quot;file='ui/展开.png' &quot; selectedimage=&quot;file='ui/未展开.png'&quot; selectedhotimage=&quot;file='ui/未展开.png'&quot;"));
					pNode_2_4[n * 2]->SetAttribute(_T("itemattr"), _T("padding=\"16,0,0,0\" align=\"left\" font=\"13\""));//font=&quot;6&quot;
					pNode_2_4[n * 2]->SetName(_T("WX1"));
					pNode_2_3[n]->AddChildNode(pNode_2_4[n * 2]);

					pNode_2_4[n * 2 + 1] = new CTreeNodeUI();
					pNode_2_4[n * 2 + 1]->CheckBoxSelected(false);
					pNode_2_4[n * 2 + 1]->SetItemText(stringToWstring(vecVec_WxChildName[wxChildSeuque][1]).c_str());//wxChildSeuque
					pNode_2_4[n * 2 + 1]->SetFixedHeight(30);
					pNode_2_4[n * 2 + 1]->SetFixedWidth(100);
					pNode_2_4[n * 2 + 1]->SetItemTextColor(0xFFFFFFFF);
					pNode_2_4[n * 2 + 1]->SetItemHotTextColor(0xFFFFFFFF);
					pNode_2_4[n * 2 + 1]->SetSelItemTextColor(0xFFFFFFFF);
					pNode_2_4[n * 2 + 1]->SetAttribute(_T("folderattr"), _T("padding=&quot;0,1,0,0&quot; width=&quot;16&quot; height=&quot;16&quot; normalimage=&quot;file='ui/展开.png' &quot; "));
					pNode_2_4[n * 2 + 1]->SetAttribute(_T("folderattr"), _T("hotimage=&quot;file='ui/展开.png' &quot; selectedimage=&quot;file='ui/未展开.png'&quot; selectedhotimage=&quot;file='ui/未展开.png'&quot;"));
					pNode_2_4[n * 2 + 1]->SetAttribute(_T("itemattr"), _T("padding=\"16,0,0,0\" align=\"left\" font=\"13\""));//font=&quot;6&quot;
					pNode_2_4[n * 2 + 1]->SetName(_T("WX1"));
					pNode_2_3[n]->AddChildNode(pNode_2_4[n * 2 + 1]);

					pNode_2_4[n * 2 + 2] = new CTreeNodeUI();
					pNode_2_4[n * 2 + 2]->CheckBoxSelected(false);
					pNode_2_4[n * 2 + 2]->SetItemText(stringToWstring(vecVec_WxChildName[wxChildSeuque][2]).c_str());//wxChildSeuque
					pNode_2_4[n * 2 + 2]->SetFixedHeight(30);
					pNode_2_4[n * 2 + 2]->SetFixedWidth(100);
					pNode_2_4[n * 2 + 2]->SetItemTextColor(0xFFFFFFFF);
					pNode_2_4[n * 2 + 2]->SetItemHotTextColor(0xFFFFFFFF);
					pNode_2_4[n * 2 + 2]->SetSelItemTextColor(0xFFFFFFFF);
					pNode_2_4[n * 2 + 2]->SetAttribute(_T("folderattr"), _T("padding=&quot;0,1,0,0&quot; width=&quot;16&quot; height=&quot;16&quot; normalimage=&quot;file='ui/展开.png' &quot; "));
					pNode_2_4[n * 2 + 2]->SetAttribute(_T("folderattr"), _T("hotimage=&quot;file='ui/展开.png' &quot; selectedimage=&quot;file='ui/未展开.png'&quot; selectedhotimage=&quot;file='ui/未展开.png'&quot;"));
					pNode_2_4[n * 2 + 2]->SetAttribute(_T("itemattr"), _T("padding=\"16,0,0,0\" align=\"left\" font=\"13\""));//font=&quot;6&quot;
					pNode_2_4[n * 2 + 2]->SetName(_T("WX1"));
					pNode_2_3[n]->AddChildNode(pNode_2_4[n * 2 + 2]);

					wxChildSeuque += 1;
				}
			}
		}
	}
	RootNodeOnly1_DXG();

}
//获取电信港性质的节点类型所在顺序
int CPopEditorialClass::GetNoodLevel_DXG_XZ(string nodeName)
{
	CDuiString cdui_name;
	string str_name;
	int sequences = -1;
	for (int i = 0; i < pNode_2_1_num; i++)
	{
		cdui_name = pNode_2_1[i]->GetItemText();
		str_name = wstringToString(cdui_name.GetData());
		if (str_name == nodeName)
		{
			sequences = i;
		}
	}

	return sequences;
}
//获电信港性质的根节点所在顺序
int CPopEditorialClass::GetNoodLevel_Root_DXG(string nodeName)
{
	CDuiString cdui_name;
	string str_name;
	int sequences = -1;
	for (int i = 0; i < pNode_2_1_num; i++)
	{
		int childNum = pNode_2_1[i]->GetCountChild();
		for (int j = 0; j < childNum; j++)
		{
			CTreeNodeUI*  node = pNode_2_1[i]->GetChildNode(j);//GetParentNode
			cdui_name = node->GetItemText();
			str_name = wstringToString(cdui_name.GetData());
			if (str_name == nodeName)
			{
				sequences = i;
			}
		}
	}
	return sequences;
}
//分类复原后只留根节点
void CPopEditorialClass::RootNodeOnly1_DXG()
{
	int num = pNode_2_1_num;
	cout << num << endl;
	for (int i = 0; i < (num - 1); i++)
	{
		m_treeView_2->Remove(pNode_2_1[i + 1]);
		pNode_2_1_num -= 1;
	}
	int num1 = pNode_2_1_num;
	cout << num1 << endl;
}
//地球站-----按照载荷功能分类
void CPopEditorialClass::Categorize_DQZ_FunctionType1(string name)
{
	//string str = name;
	vector<string> childName;//存放子节点名称
	vector<vector<string>> vec_wxchildName;//存放地球站(卫星)子节点名称
	vector<int> childSerialNum;//子节点所在pNode_3中的序号
	int serialNumber;//存放选择的电信港名称
	CTreeNodeUI* pNode[1024];//卫星筛选的时候的三级节点 存放类型的
	CTreeNodeUI* pNode_T4[1024];//存放四层节点

	CTreeNodeUI* pNode_T5[1024];//存放卫星节点子节点
	int level_4 = 0;
	int level_5 = 0;
	/*******************筛选星座名称在存放星座名称的vector中的位置***************/
	for (int i = 0; i < name2_Level1.size(); i++)
	{
		if (name == name2_Level1[i])
		{
			serialNumber = i;
		}
	}
	int childNum = pNode_2_2[serialNumber]->GetCountChild();
	/**********************筛选当前星座下面的卫星名称***********************/
	for (int j = 0; j < childNum; j++)
	{
		CDuiString wstr_name1 = pNode_2_2[serialNumber]->GetChildNode(j)->GetItemText();
		string str_name1 = wstringToString(wstr_name1.GetData());
		childName.push_back(str_name1);
		/******************筛选当前星座下卫星所在pNode_3中的顺序***********/
		for (int m = 0; m < name2_Level2.size(); m++)
		{
			if (str_name1 == name2_Level2[m])
			{
				childSerialNum.push_back(m);

			}
		}
		/***********************************************/
		//pNode_1[0]->Remove(pNode_2[i]);
	}
	/***********************筛选卫星下面的子节点名称*****************************/
	for (int i = 0; i < childSerialNum.size(); i++)
	{
		int childN = pNode_2_3[childSerialNum[i]]->GetCountChild();//->GetChildNode(j)->GetItemText();
		vector<string> childName_1;

		for (int j = 0; j < childN; j++)
		{
			CDuiString wstr_name1 = pNode_2_3[childSerialNum[i]]->GetChildNode(j)->GetItemText();
			string str_name1 = wstringToString(wstr_name1.GetData());
			childName_1.push_back(str_name1);
		}
		//string str_name1 = wstringToString(wstr_name1.GetData());
		vec_wxchildName.push_back(childName_1);

	}
	/***********************删除卫星下面的子节点******************************/
	for (int j = 0; j < childSerialNum.size(); j++)
	{
		pNode_2_3[childSerialNum[j]]->Remove(pNode_2_4[childSerialNum[j] * 2 + 2]);
		pNode_2_3[childSerialNum[j]]->Remove(pNode_2_4[childSerialNum[j] * 2 + 1]);
		pNode_2_3[childSerialNum[j]]->Remove(pNode_2_4[childSerialNum[j] * 2]);
	}
	/***********************删除星座下的卫星***********************/
	for (int j = 0; j < childSerialNum.size(); j++)
	{
		pNode_2_2[serialNumber]->Remove(pNode_2_3[childSerialNum[j]]);
	}
	/******************************************************/
	for (int j = 0; j < childSerialNum.size(); j++)
	{
		map<string, string> curr_map = m_RecvMap_DQZ[j];

		if (curr_map.count("功能类型") > 0)
		{
			string value = curr_map["功能类型"];
			int sequence = GetNoodLevel_level2_DQZ(UTF8_To_string(value), serialNumber);
			if (sequence >= 0)
			{
				cout << "111111111111" << endl;
				pNode_T4[level_4] = new CTreeNodeUI();
				pNode_T4[level_4]->CheckBoxSelected(false);
				pNode_T4[level_4]->SetItemText(stringToWstring(childName[j]).c_str());
				pNode_T4[level_4]->SetFixedHeight(30);
				pNode_T4[level_4]->SetItemTextColor(0xFFFFFFFF);
				pNode_T4[level_4]->SetItemHotTextColor(0xFFFFFFFF);
				pNode_T4[level_4]->SetSelItemTextColor(0xFFFFFFFF);
				pNode_T4[level_4]->SetAttribute(_T("folderattr"), _T("padding=&quot;0,1,0,0&quot; width=&quot;16&quot; height=&quot;16&quot; normalimage=&quot;file='ui/展开.png' &quot; "));
				pNode_T4[level_4]->SetAttribute(_T("folderattr"), _T("hotimage=&quot;file='ui/展开.png' &quot; selectedimage=&quot;file='ui/未展开.png'&quot; selectedhotimage=&quot;file='ui/未展开.png'&quot;"));
				pNode_T4[level_4]->SetAttribute(_T("itemattr"), _T("padding=\"16,0,0,0\" align=\"left\" font=\"13\""));//font=&quot;6&quot;
				pNode_T4[level_4]->SetName(_T("WX1"));

				pNode[sequence]->AddChildNode(pNode_T4[level_4]);

				pNode_T5[level_5] = new CTreeNodeUI();
				pNode_T5[level_5]->CheckBoxSelected(false);
				pNode_T5[level_5]->SetItemText(stringToWstring(vec_wxchildName[j][0]).c_str());
				pNode_T5[level_5]->SetFixedHeight(30);
				pNode_T5[level_5]->SetItemTextColor(0xFFFFFFFF);
				pNode_T5[level_5]->SetItemHotTextColor(0xFFFFFFFF);
				pNode_T5[level_4]->SetSelItemTextColor(0xFFFFFFFF);
				pNode_T5[level_5]->SetAttribute(_T("folderattr"), _T("padding=&quot;0,1,0,0&quot; width=&quot;16&quot; height=&quot;16&quot; normalimage=&quot;file='ui/展开.png' &quot; "));
				pNode_T5[level_5]->SetAttribute(_T("folderattr"), _T("hotimage=&quot;file='ui/展开.png' &quot; selectedimage=&quot;file='ui/未展开.png'&quot; selectedhotimage=&quot;file='ui/未展开.png'&quot;"));
				pNode_T5[level_5]->SetAttribute(_T("itemattr"), _T("padding=\"16,0,0,0\" align=\"left\" font=\"13\""));//font=&quot;6&quot;
				pNode_T5[level_5]->SetName(_T("WX1"));

				pNode_T4[level_4]->AddChildNode(pNode_T5[level_5]);
				level_5 += 1;

				pNode_T5[level_5] = new CTreeNodeUI();
				pNode_T5[level_5]->CheckBoxSelected(false);
				pNode_T5[level_5]->SetItemText(stringToWstring(vec_wxchildName[j][1]).c_str());
				pNode_T5[level_5]->SetFixedHeight(30);
				pNode_T5[level_5]->SetItemTextColor(0xFFFFFFFF);
				pNode_T5[level_5]->SetItemHotTextColor(0xFFFFFFFF);
				pNode_T5[level_4]->SetSelItemTextColor(0xFFFFFFFF);
				pNode_T5[level_5]->SetAttribute(_T("folderattr"), _T("padding=&quot;0,1,0,0&quot; width=&quot;16&quot; height=&quot;16&quot; normalimage=&quot;file='ui/展开.png' &quot; "));
				pNode_T5[level_5]->SetAttribute(_T("folderattr"), _T("hotimage=&quot;file='ui/展开.png' &quot; selectedimage=&quot;file='ui/未展开.png'&quot; selectedhotimage=&quot;file='ui/未展开.png'&quot;"));
				pNode_T5[level_5]->SetAttribute(_T("itemattr"), _T("padding=\"16,0,0,0\" align=\"left\" font=\"13\""));//font=&quot;6&quot;
				pNode_T5[level_5]->SetName(_T("WX1"));

				pNode_T4[level_4]->AddChildNode(pNode_T5[level_5]);
				level_5 += 1;

				pNode_T5[level_5] = new CTreeNodeUI();
				pNode_T5[level_5]->CheckBoxSelected(false);
				pNode_T5[level_5]->SetItemText(stringToWstring(vec_wxchildName[j][2]).c_str());
				pNode_T5[level_5]->SetFixedHeight(30);
				pNode_T5[level_5]->SetItemTextColor(0xFFFFFFFF);
				pNode_T5[level_5]->SetItemHotTextColor(0xFFFFFFFF);
				pNode_T5[level_4]->SetSelItemTextColor(0xFFFFFFFF);
				pNode_T5[level_5]->SetAttribute(_T("folderattr"), _T("padding=&quot;0,1,0,0&quot; width=&quot;16&quot; height=&quot;16&quot; normalimage=&quot;file='ui/展开.png' &quot; "));
				pNode_T5[level_5]->SetAttribute(_T("folderattr"), _T("hotimage=&quot;file='ui/展开.png' &quot; selectedimage=&quot;file='ui/未展开.png'&quot; selectedhotimage=&quot;file='ui/未展开.png'&quot;"));
				pNode_T5[level_5]->SetAttribute(_T("itemattr"), _T("padding=\"16,0,0,0\" align=\"left\" font=\"13\""));//font=&quot;6&quot;
				pNode_T5[level_5]->SetName(_T("WX1"));

				pNode_T4[level_4]->AddChildNode(pNode_T5[level_5]);
				level_5 += 1;
				level_4 += 1;
			}
			else
			{
				pNode[j] = new CTreeNodeUI();
				pNode[j]->CheckBoxSelected(false);
				pNode[j]->SetItemText(stringToWstring(UTF8_To_string(value)).c_str());
				pNode[j]->SetFixedHeight(30);
				pNode[j]->SetItemTextColor(0xFFFFFFFF);
				pNode[j]->SetItemHotTextColor(0xFFFFFFFF);
				pNode[j]->SetSelItemTextColor(0xFFFFFFFF);
				pNode[j]->SetAttribute(_T("folderattr"), _T("padding=&quot;0,1,0,0&quot; width=&quot;16&quot; height=&quot;16&quot; normalimage=&quot;file='ui/展开.png' &quot; "));
				pNode[j]->SetAttribute(_T("folderattr"), _T("hotimage=&quot;file='ui/展开.png' &quot; selectedimage=&quot;file='ui/未展开.png'&quot; selectedhotimage=&quot;file='ui/未展开.png'&quot;"));
				pNode[j]->SetAttribute(_T("itemattr"), _T("padding=\"16,0,0,0\" align=\"left\" font=\"13\""));//font=&quot;6&quot;
				pNode[j]->SetName(_T("WX1"));
				pNode_2_2[serialNumber]->AddChildNode(pNode[j]);

				pNode_T4[level_4] = new CTreeNodeUI();
				pNode_T4[level_4]->CheckBoxSelected(false);
				pNode_T4[level_4]->SetItemText(stringToWstring(childName[j]).c_str());
				pNode_T4[level_4]->SetFixedHeight(30);
				pNode_T4[level_4]->SetItemTextColor(0xFFFFFFFF);
				pNode_T4[level_4]->SetItemHotTextColor(0xFFFFFFFF);
				pNode_T4[level_4]->SetSelItemTextColor(0xFFFFFFFF);
				pNode_T4[level_4]->SetAttribute(_T("folderattr"), _T("padding=&quot;0,1,0,0&quot; width=&quot;16&quot; height=&quot;16&quot; normalimage=&quot;file='ui/展开.png' &quot; "));
				pNode_T4[level_4]->SetAttribute(_T("folderattr"), _T("hotimage=&quot;file='ui/展开.png' &quot; selectedimage=&quot;file='ui/未展开.png'&quot; selectedhotimage=&quot;file='ui/未展开.png'&quot;"));
				pNode_T4[level_4]->SetAttribute(_T("itemattr"), _T("padding=\"16,0,0,0\" align=\"left\" font=\"13\""));//font=&quot;6&quot;
				pNode_T4[level_4]->SetName(_T("WX1"));

				pNode[j]->AddChildNode(pNode_T4[level_4]);


				pNode_T5[level_5] = new CTreeNodeUI();
				pNode_T5[level_5]->CheckBoxSelected(false);
				pNode_T5[level_5]->SetItemText(stringToWstring(vec_wxchildName[j][0]).c_str());
				pNode_T5[level_5]->SetFixedHeight(30);
				pNode_T5[level_5]->SetItemTextColor(0xFFFFFFFF);
				pNode_T5[level_5]->SetItemHotTextColor(0xFFFFFFFF);
				pNode_T5[level_4]->SetSelItemTextColor(0xFFFFFFFF);
				pNode_T5[level_5]->SetAttribute(_T("folderattr"), _T("padding=&quot;0,1,0,0&quot; width=&quot;16&quot; height=&quot;16&quot; normalimage=&quot;file='ui/展开.png' &quot; "));
				pNode_T5[level_5]->SetAttribute(_T("folderattr"), _T("hotimage=&quot;file='ui/展开.png' &quot; selectedimage=&quot;file='ui/未展开.png'&quot; selectedhotimage=&quot;file='ui/未展开.png'&quot;"));
				pNode_T5[level_5]->SetAttribute(_T("itemattr"), _T("padding=\"16,0,0,0\" align=\"left\" font=\"13\""));//font=&quot;6&quot;
				pNode_T5[level_5]->SetName(_T("WX1"));

				pNode_T4[level_4]->AddChildNode(pNode_T5[level_5]);
				level_5 += 1;

				pNode_T5[level_5] = new CTreeNodeUI();
				pNode_T5[level_5]->CheckBoxSelected(false);
				pNode_T5[level_5]->SetItemText(stringToWstring(vec_wxchildName[j][1]).c_str());
				pNode_T5[level_5]->SetFixedHeight(30);
				pNode_T5[level_5]->SetItemTextColor(0xFFFFFFFF);
				pNode_T5[level_5]->SetItemHotTextColor(0xFFFFFFFF);
				pNode_T5[level_4]->SetSelItemTextColor(0xFFFFFFFF);
				pNode_T5[level_5]->SetAttribute(_T("folderattr"), _T("padding=&quot;0,1,0,0&quot; width=&quot;16&quot; height=&quot;16&quot; normalimage=&quot;file='ui/展开.png' &quot; "));
				pNode_T5[level_5]->SetAttribute(_T("folderattr"), _T("hotimage=&quot;file='ui/展开.png' &quot; selectedimage=&quot;file='ui/未展开.png'&quot; selectedhotimage=&quot;file='ui/未展开.png'&quot;"));
				pNode_T5[level_5]->SetAttribute(_T("itemattr"), _T("padding=\"16,0,0,0\" align=\"left\" font=\"13\""));//font=&quot;6&quot;
				pNode_T5[level_5]->SetName(_T("WX1"));

				pNode_T4[level_4]->AddChildNode(pNode_T5[level_5]);
				level_5 += 1;

				pNode_T5[level_5] = new CTreeNodeUI();
				pNode_T5[level_5]->CheckBoxSelected(false);
				pNode_T5[level_5]->SetItemText(stringToWstring(vec_wxchildName[j][2]).c_str());
				pNode_T5[level_5]->SetFixedHeight(30);
				pNode_T5[level_5]->SetItemTextColor(0xFFFFFFFF);
				pNode_T5[level_5]->SetItemHotTextColor(0xFFFFFFFF);
				pNode_T5[level_4]->SetSelItemTextColor(0xFFFFFFFF);
				pNode_T5[level_5]->SetAttribute(_T("folderattr"), _T("padding=&quot;0,1,0,0&quot; width=&quot;16&quot; height=&quot;16&quot; normalimage=&quot;file='ui/展开.png' &quot; "));
				pNode_T5[level_5]->SetAttribute(_T("folderattr"), _T("hotimage=&quot;file='ui/展开.png' &quot; selectedimage=&quot;file='ui/未展开.png'&quot; selectedhotimage=&quot;file='ui/未展开.png'&quot;"));
				pNode_T5[level_5]->SetAttribute(_T("itemattr"), _T("padding=\"16,0,0,0\" align=\"left\" font=\"13\""));//font=&quot;6&quot;
				pNode_T5[level_5]->SetName(_T("WX1"));

				pNode_T4[level_4]->AddChildNode(pNode_T5[level_5]);
				level_5 += 1;
				level_4 += 1;

			}

		}
	}
}
//地球站------按载荷功能分类后还原
void CPopEditorialClass::Restore_DQZ_FunctionType1(string name)
{

	//string str = name;
	vector<string> childName;//存放子节点名称
	vector<vector<string>> vec_wxchildName;//存放卫星子节点名称
	vector<int> childSerialNum;//子节点所在pNode_3中的序号
	int serialNumber;//存放选择的星座名称
	CTreeNodeUI* pNode[1024];
	int serialNum_wx;
	/*******************筛选星座名称在存放星座名称的vector中的位置***************/
	for (int i = 0; i < name2_Level1.size(); i++)
	{
		if (name == name2_Level1[i])
		{
			serialNumber = i;
		}
		//pNode_2[i]
	}
	/*************************筛选子节点的子节点包含的卫星名称*********************************/
	int childNum = pNode_2_2[serialNumber]->GetCountChild();
	for (int i = 0; i < childNum; i++)
	{
		CTreeNodeUI* pNode_level3 = pNode_2_2[serialNumber]->GetChildNode(i);
		int childNum1 = pNode_level3->GetCountChild();
		//pNode_level3->RemoveAll();
		for (int j = 0; j < childNum1; j++)
		{
			vector<string > vec;
			CDuiString cdui_name = pNode_level3->GetChildNode(j)->GetItemText();
			string str_name = wstringToString(cdui_name.GetData());
			childName.push_back(str_name);//存放的是卫星
			int wxChildNum = pNode_level3->GetChildNode(j)->GetCountChild();
			for (int m = 0; m < wxChildNum; m++)
			{
				CDuiString cdui_name1 = pNode_level3->GetChildNode(j)->GetChildNode(m)->GetItemText();
				string str_name1 = wstringToString(cdui_name1.GetData());
				vec.push_back(str_name1);//存放的是卫星
			}
			vec_wxchildName.push_back(vec);
		}
	}
	/********************删除2级节点******************/
	for (int i = childNum; i > 0; i--)
	{
		pNode_2_2[serialNumber]->Remove(pNode_2_2[serialNumber]->GetChildNode(i - 1));
	}
	/*********************新建恢复为原来的样子**********************/
	int m = 0;
	for (int i = 0; i < childName.size(); i++)
	{

		for (int j = 0; j < name2_Level2.size(); j++)
		{

			if (childName[i] == name2_Level2[j])
			{
				serialNum_wx = j;
				pNode_2_3[serialNum_wx] = new CTreeNodeUI();
				pNode_2_3[serialNum_wx]->CheckBoxSelected(false);
				pNode_2_3[serialNum_wx]->SetItemText(stringToWstring(childName[i]).c_str());
				pNode_2_3[serialNum_wx]->SetFixedHeight(30);
				pNode_2_3[serialNum_wx]->SetItemTextColor(0xFFFFFFFF);
				pNode_2_3[serialNum_wx]->SetItemHotTextColor(0xFFFFFFFF);
				pNode_2_3[serialNum_wx]->SetSelItemTextColor(0xFFFFFFFF);
				pNode_2_3[serialNum_wx]->SetAttribute(_T("folderattr"), _T("padding=&quot;0,1,0,0&quot; width=&quot;16&quot; height=&quot;16&quot; normalimage=&quot;file='ui/展开.png' &quot; "));
				pNode_2_3[serialNum_wx]->SetAttribute(_T("folderattr"), _T("hotimage=&quot;file='ui/展开.png' &quot; selectedimage=&quot;file='ui/未展开.png'&quot; selectedhotimage=&quot;file='ui/未展开.png'&quot;"));
				pNode_2_3[serialNum_wx]->SetAttribute(_T("itemattr"), _T("padding=\"16,0,0,0\" align=\"left\" font=\"13\""));//font=&quot;6&quot;
				pNode_2_3[serialNum_wx]->SetName(_T("WX1"));
				pNode_2_2[serialNumber]->AddChildNode(pNode_2_3[serialNum_wx]);

				//for (int m = 0; m < vec_wxchildName.size(); m++)
				//{
				pNode_2_4[serialNum_wx * 2] = new CTreeNodeUI();
				pNode_2_4[serialNum_wx * 2]->CheckBoxSelected(false);
				pNode_2_4[serialNum_wx * 2]->SetItemText(stringToWstring(vec_wxchildName[m][0]).c_str());
				pNode_2_4[serialNum_wx * 2]->SetFixedHeight(30);
				pNode_2_4[serialNum_wx * 2]->SetItemTextColor(0xFFFFFFFF);
				pNode_2_4[serialNum_wx * 2]->SetItemHotTextColor(0xFFFFFFFF);
				pNode_2_4[serialNum_wx * 2]->SetSelItemTextColor(0xFFFFFFFF);
				pNode_2_4[serialNum_wx * 2]->SetAttribute(_T("folderattr"), _T("padding=&quot;0,1,0,0&quot; width=&quot;16&quot; height=&quot;16&quot; normalimage=&quot;file='ui/展开.png' &quot; "));
				pNode_2_4[serialNum_wx * 2]->SetAttribute(_T("folderattr"), _T("hotimage=&quot;file='ui/展开.png' &quot; selectedimage=&quot;file='ui/未展开.png'&quot; selectedhotimage=&quot;file='ui/未展开.png'&quot;"));
				pNode_2_4[serialNum_wx * 2]->SetAttribute(_T("itemattr"), _T("padding=\"16,0,0,0\" align=\"left\" font=\"13\""));//font=&quot;6&quot;
				pNode_2_4[serialNum_wx * 2]->SetName(_T("WX1"));
				pNode_2_3[serialNum_wx]->AddChildNode(pNode_2_4[serialNum_wx * 2]);

				pNode_2_4[serialNum_wx * 2 + 1] = new CTreeNodeUI();
				pNode_2_4[serialNum_wx * 2 + 1]->CheckBoxSelected(false);
				pNode_2_4[serialNum_wx * 2 + 1]->SetItemText(stringToWstring(vec_wxchildName[m][1]).c_str());
				pNode_2_4[serialNum_wx * 2 + 1]->SetFixedHeight(30);
				pNode_2_4[serialNum_wx * 2 + 1]->SetItemTextColor(0xFFFFFFFF);
				pNode_2_4[serialNum_wx * 2 + 1]->SetItemHotTextColor(0xFFFFFFFF);
				pNode_2_4[serialNum_wx * 2 + 1]->SetSelItemTextColor(0xFFFFFFFF);
				pNode_2_4[serialNum_wx * 2 + 1]->SetAttribute(_T("folderattr"), _T("padding=&quot;0,1,0,0&quot; width=&quot;16&quot; height=&quot;16&quot; normalimage=&quot;file='ui/展开.png' &quot; "));
				pNode_2_4[serialNum_wx * 2 + 1]->SetAttribute(_T("folderattr"), _T("hotimage=&quot;file='ui/展开.png' &quot; selectedimage=&quot;file='ui/未展开.png'&quot; selectedhotimage=&quot;file='ui/未展开.png'&quot;"));
				pNode_2_4[serialNum_wx * 2 + 1]->SetAttribute(_T("itemattr"), _T("padding=\"16,0,0,0\" align=\"left\" font=\"13\""));//font=&quot;6&quot;
				pNode_2_4[serialNum_wx * 2 + 1]->SetName(_T("WX1"));
				pNode_2_3[serialNum_wx]->AddChildNode(pNode_2_4[serialNum_wx * 2 + 1]);

				pNode_2_4[serialNum_wx * 2 + 2] = new CTreeNodeUI();
				pNode_2_4[serialNum_wx * 2 + 2]->CheckBoxSelected(false);
				pNode_2_4[serialNum_wx * 2 + 2]->SetItemText(stringToWstring(vec_wxchildName[m][2]).c_str());
				pNode_2_4[serialNum_wx * 2 + 2]->SetFixedHeight(30);
				pNode_2_4[serialNum_wx * 2 + 2]->SetItemTextColor(0xFFFFFFFF);
				pNode_2_4[serialNum_wx * 2 + 2]->SetItemHotTextColor(0xFFFFFFFF);
				pNode_2_4[serialNum_wx * 2 + 2]->SetSelItemTextColor(0xFFFFFFFF);
				pNode_2_4[serialNum_wx * 2 + 2]->SetAttribute(_T("folderattr"), _T("padding=&quot;0,1,0,0&quot; width=&quot;16&quot; height=&quot;16&quot; normalimage=&quot;file='ui/展开.png' &quot; "));
				pNode_2_4[serialNum_wx * 2 + 2]->SetAttribute(_T("folderattr"), _T("hotimage=&quot;file='ui/展开.png' &quot; selectedimage=&quot;file='ui/未展开.png'&quot; selectedhotimage=&quot;file='ui/未展开.png'&quot;"));
				pNode_2_4[serialNum_wx * 2 + 2]->SetAttribute(_T("itemattr"), _T("padding=\"16,0,0,0\" align=\"left\" font=\"13\""));//font=&quot;6&quot;
				pNode_2_4[serialNum_wx * 2 + 2]->SetName(_T("WX1"));
				pNode_2_3[serialNum_wx]->AddChildNode(pNode_2_4[serialNum_wx * 2 + 2]);
				//}
				m += 1;
			}


		}
	}


}
//获取地球站功能类型所在顺序
int CPopEditorialClass::GetNoodLevel_level2_DQZ(string nodeName, int serialNumber)
{
	//获取卫星类型在节点中的顺序
	CDuiString cdui_name;
	string str_name;
	int sequences = -1;
	//cdui_name = pNode_2[serialNumber]->GetItemText();
	//str_name = wstringToString(cdui_name.GetData());
	int childNum = pNode_2_2[serialNumber]->GetCountChild();
	for (int i = 0; i < childNum; i++)
	{
		CTreeNodeUI * pNode = pNode_2_2[serialNumber]->GetChildNode(i);
		cdui_name = pNode->GetItemText();
		str_name = wstringToString(cdui_name.GetData());
		if (str_name == nodeName)
		{
			sequences = i;
		}
	}

	return sequences;
}
//第二个界面双击三级目录
void CPopEditorialClass::FindTheMap_level3_2(CDuiString cdui_name)
{
	int sequences;//存放在vector中的顺序
	wstring wstr_name = cdui_name;
	string str_name = wstringToString(wstr_name);
	for (int i = 0; i < name2_Level2.size(); i++)
	{
		if (str_name == name2_Level2[i])
		{
			sequences = i;
		}
	}
	DisplayInformation_2(m_RecvMap_DQZ[sequences]);
}
//地球站-------按地球站类型分类
void CPopEditorialClass::Categorize_DQZ_EarthStation(string name)
{
	//string str = name;
	vector<string> childName;//存放子节点名称
	vector<vector<string>> vec_wxchildName;//存放地球站(卫星)子节点名称
	vector<int> childSerialNum;//子节点所在pNode_3中的序号
	int serialNumber;//存放选择的电信港名称
	CTreeNodeUI* pNode[1024];//卫星筛选的时候的三级节点 存放类型的
	CTreeNodeUI* pNode_T4[1024];//存放四层节点

	CTreeNodeUI* pNode_T5[1024];//存放卫星节点子节点
	int level_4 = 0;
	int level_5 = 0;
	/*******************筛选星座名称在存放星座名称的vector中的位置***************/
	for (int i = 0; i < name2_Level1.size(); i++)
	{
		if (name == name2_Level1[i])
		{
			serialNumber = i;
		}
	}
	int childNum = pNode_2_2[serialNumber]->GetCountChild();
	/**********************筛选当前星座下面的卫星名称***********************/
	for (int j = 0; j < childNum; j++)
	{
		CDuiString wstr_name1 = pNode_2_2[serialNumber]->GetChildNode(j)->GetItemText();
		string str_name1 = wstringToString(wstr_name1.GetData());
		childName.push_back(str_name1);
		/******************筛选当前星座下卫星所在pNode_3中的顺序***********/
		for (int m = 0; m < name2_Level2.size(); m++)
		{
			if (str_name1 == name2_Level2[m])
			{
				childSerialNum.push_back(m);

			}
		}
		/***********************************************/
		//pNode_1[0]->Remove(pNode_2[i]);
	}
	/***********************筛选卫星下面的子节点名称*****************************/
	for (int i = 0; i < childSerialNum.size(); i++)
	{
		int childN = pNode_2_3[childSerialNum[i]]->GetCountChild();//->GetChildNode(j)->GetItemText();
		vector<string> childName_1;

		for (int j = 0; j < childN; j++)
		{
			CDuiString wstr_name1 = pNode_2_3[childSerialNum[i]]->GetChildNode(j)->GetItemText();
			string str_name1 = wstringToString(wstr_name1.GetData());
			childName_1.push_back(str_name1);
		}
		//string str_name1 = wstringToString(wstr_name1.GetData());
		vec_wxchildName.push_back(childName_1);

	}
	/***********************删除卫星下面的子节点******************************/
	for (int j = 0; j < childSerialNum.size(); j++)
	{
		pNode_2_3[childSerialNum[j]]->Remove(pNode_2_4[childSerialNum[j] * 2 + 2]);
		pNode_2_3[childSerialNum[j]]->Remove(pNode_2_4[childSerialNum[j] * 2 + 1]);
		pNode_2_3[childSerialNum[j]]->Remove(pNode_2_4[childSerialNum[j] * 2]);
	}
	/***********************删除星座下的卫星***********************/
	for (int j = 0; j < childSerialNum.size(); j++)
	{
		pNode_2_2[serialNumber]->Remove(pNode_2_3[childSerialNum[j]]);
	}
	/******************************************************/
	for (int j = 0; j < childSerialNum.size(); j++)
	{
		map<string, string> curr_map = m_RecvMap_DQZ[j];

		if (curr_map.count("地球站类型") > 0)
		{
			string value = curr_map["地球站类型"];
			int sequence = GetNoodLevel_level2_DQZ(UTF8_To_string(value), serialNumber);
			if (sequence >= 0)
			{
				cout << "111111111111" << endl;
				pNode_T4[level_4] = new CTreeNodeUI();
				pNode_T4[level_4]->CheckBoxSelected(false);
				pNode_T4[level_4]->SetItemText(stringToWstring(childName[j]).c_str());
				pNode_T4[level_4]->SetFixedHeight(30);
				pNode_T4[level_4]->SetItemTextColor(0xFFFFFFFF);
				pNode_T4[level_4]->SetItemHotTextColor(0xFFFFFFFF);
				pNode_T4[level_4]->SetSelItemTextColor(0xFFFFFFFF);
				pNode_T4[level_4]->SetAttribute(_T("folderattr"), _T("padding=&quot;0,1,0,0&quot; width=&quot;16&quot; height=&quot;16&quot; normalimage=&quot;file='ui/展开.png' &quot; "));
				pNode_T4[level_4]->SetAttribute(_T("folderattr"), _T("hotimage=&quot;file='ui/展开.png' &quot; selectedimage=&quot;file='ui/未展开.png'&quot; selectedhotimage=&quot;file='ui/未展开.png'&quot;"));
				pNode_T4[level_4]->SetAttribute(_T("itemattr"), _T("padding=\"16,0,0,0\" align=\"left\" font=\"13\""));//font=&quot;6&quot;
				pNode_T4[level_4]->SetName(_T("WX1"));

				pNode[sequence]->AddChildNode(pNode_T4[level_4]);

				pNode_T5[level_5] = new CTreeNodeUI();
				pNode_T5[level_5]->CheckBoxSelected(false);
				pNode_T5[level_5]->SetItemText(stringToWstring(vec_wxchildName[j][0]).c_str());
				pNode_T5[level_5]->SetFixedHeight(30);
				pNode_T5[level_5]->SetItemTextColor(0xFFFFFFFF);
				pNode_T5[level_5]->SetItemHotTextColor(0xFFFFFFFF);
				pNode_T5[level_4]->SetSelItemTextColor(0xFFFFFFFF);
				pNode_T5[level_5]->SetAttribute(_T("folderattr"), _T("padding=&quot;0,1,0,0&quot; width=&quot;16&quot; height=&quot;16&quot; normalimage=&quot;file='ui/展开.png' &quot; "));
				pNode_T5[level_5]->SetAttribute(_T("folderattr"), _T("hotimage=&quot;file='ui/展开.png' &quot; selectedimage=&quot;file='ui/未展开.png'&quot; selectedhotimage=&quot;file='ui/未展开.png'&quot;"));
				pNode_T5[level_5]->SetAttribute(_T("itemattr"), _T("padding=\"16,0,0,0\" align=\"left\" font=\"13\""));//font=&quot;6&quot;
				pNode_T5[level_5]->SetName(_T("WX1"));

				pNode_T4[level_4]->AddChildNode(pNode_T5[level_5]);
				level_5 += 1;

				pNode_T5[level_5] = new CTreeNodeUI();
				pNode_T5[level_5]->CheckBoxSelected(false);
				pNode_T5[level_5]->SetItemText(stringToWstring(vec_wxchildName[j][1]).c_str());
				pNode_T5[level_5]->SetFixedHeight(30);
				pNode_T5[level_5]->SetItemTextColor(0xFFFFFFFF);
				pNode_T5[level_5]->SetItemHotTextColor(0xFFFFFFFF);
				pNode_T5[level_4]->SetSelItemTextColor(0xFFFFFFFF);
				pNode_T5[level_5]->SetAttribute(_T("folderattr"), _T("padding=&quot;0,1,0,0&quot; width=&quot;16&quot; height=&quot;16&quot; normalimage=&quot;file='ui/展开.png' &quot; "));
				pNode_T5[level_5]->SetAttribute(_T("folderattr"), _T("hotimage=&quot;file='ui/展开.png' &quot; selectedimage=&quot;file='ui/未展开.png'&quot; selectedhotimage=&quot;file='ui/未展开.png'&quot;"));
				pNode_T5[level_5]->SetAttribute(_T("itemattr"), _T("padding=\"16,0,0,0\" align=\"left\" font=\"13\""));//font=&quot;6&quot;
				pNode_T5[level_5]->SetName(_T("WX1"));

				pNode_T4[level_4]->AddChildNode(pNode_T5[level_5]);
				level_5 += 1;

				pNode_T5[level_5] = new CTreeNodeUI();
				pNode_T5[level_5]->CheckBoxSelected(false);
				pNode_T5[level_5]->SetItemText(stringToWstring(vec_wxchildName[j][2]).c_str());
				pNode_T5[level_5]->SetFixedHeight(30);
				pNode_T5[level_5]->SetItemTextColor(0xFFFFFFFF);
				pNode_T5[level_5]->SetItemHotTextColor(0xFFFFFFFF);
				pNode_T5[level_4]->SetSelItemTextColor(0xFFFFFFFF);
				pNode_T5[level_5]->SetAttribute(_T("folderattr"), _T("padding=&quot;0,1,0,0&quot; width=&quot;16&quot; height=&quot;16&quot; normalimage=&quot;file='ui/展开.png' &quot; "));
				pNode_T5[level_5]->SetAttribute(_T("folderattr"), _T("hotimage=&quot;file='ui/展开.png' &quot; selectedimage=&quot;file='ui/未展开.png'&quot; selectedhotimage=&quot;file='ui/未展开.png'&quot;"));
				pNode_T5[level_5]->SetAttribute(_T("itemattr"), _T("padding=\"16,0,0,0\" align=\"left\" font=\"13\""));//font=&quot;6&quot;
				pNode_T5[level_5]->SetName(_T("WX1"));

				pNode_T4[level_4]->AddChildNode(pNode_T5[level_5]);
				level_5 += 1;
				level_4 += 1;
			}
			else
			{
				pNode[j] = new CTreeNodeUI();
				pNode[j]->CheckBoxSelected(false);
				pNode[j]->SetItemText(stringToWstring(UTF8_To_string(value)).c_str());
				pNode[j]->SetFixedHeight(30);
				pNode[j]->SetItemTextColor(0xFFFFFFFF);
				pNode[j]->SetItemHotTextColor(0xFFFFFFFF);
				pNode[j]->SetSelItemTextColor(0xFFFFFFFF);
				pNode[j]->SetAttribute(_T("folderattr"), _T("padding=&quot;0,1,0,0&quot; width=&quot;16&quot; height=&quot;16&quot; normalimage=&quot;file='ui/展开.png' &quot; "));
				pNode[j]->SetAttribute(_T("folderattr"), _T("hotimage=&quot;file='ui/展开.png' &quot; selectedimage=&quot;file='ui/未展开.png'&quot; selectedhotimage=&quot;file='ui/未展开.png'&quot;"));
				pNode[j]->SetAttribute(_T("itemattr"), _T("padding=\"16,0,0,0\" align=\"left\" font=\"13\""));//font=&quot;6&quot;
				pNode[j]->SetName(_T("WX1"));
				pNode_2_2[serialNumber]->AddChildNode(pNode[j]);

				pNode_T4[level_4] = new CTreeNodeUI();
				pNode_T4[level_4]->CheckBoxSelected(false);
				pNode_T4[level_4]->SetItemText(stringToWstring(childName[j]).c_str());
				pNode_T4[level_4]->SetFixedHeight(30);
				pNode_T4[level_4]->SetItemTextColor(0xFFFFFFFF);
				pNode_T4[level_4]->SetItemHotTextColor(0xFFFFFFFF);
				pNode_T4[level_4]->SetSelItemTextColor(0xFFFFFFFF);
				pNode_T4[level_4]->SetAttribute(_T("folderattr"), _T("padding=&quot;0,1,0,0&quot; width=&quot;16&quot; height=&quot;16&quot; normalimage=&quot;file='ui/展开.png' &quot; "));
				pNode_T4[level_4]->SetAttribute(_T("folderattr"), _T("hotimage=&quot;file='ui/展开.png' &quot; selectedimage=&quot;file='ui/未展开.png'&quot; selectedhotimage=&quot;file='ui/未展开.png'&quot;"));
				pNode_T4[level_4]->SetAttribute(_T("itemattr"), _T("padding=\"16,0,0,0\" align=\"left\" font=\"13\""));//font=&quot;6&quot;
				pNode_T4[level_4]->SetName(_T("WX1"));

				pNode[j]->AddChildNode(pNode_T4[level_4]);


				pNode_T5[level_5] = new CTreeNodeUI();
				pNode_T5[level_5]->CheckBoxSelected(false);
				pNode_T5[level_5]->SetItemText(stringToWstring(vec_wxchildName[j][0]).c_str());
				pNode_T5[level_5]->SetFixedHeight(30);
				pNode_T5[level_5]->SetItemTextColor(0xFFFFFFFF);
				pNode_T5[level_5]->SetItemHotTextColor(0xFFFFFFFF);
				pNode_T5[level_4]->SetSelItemTextColor(0xFFFFFFFF);
				pNode_T5[level_5]->SetAttribute(_T("folderattr"), _T("padding=&quot;0,1,0,0&quot; width=&quot;16&quot; height=&quot;16&quot; normalimage=&quot;file='ui/展开.png' &quot; "));
				pNode_T5[level_5]->SetAttribute(_T("folderattr"), _T("hotimage=&quot;file='ui/展开.png' &quot; selectedimage=&quot;file='ui/未展开.png'&quot; selectedhotimage=&quot;file='ui/未展开.png'&quot;"));
				pNode_T5[level_5]->SetAttribute(_T("itemattr"), _T("padding=\"16,0,0,0\" align=\"left\" font=\"13\""));//font=&quot;6&quot;
				pNode_T5[level_5]->SetName(_T("WX1"));

				pNode_T4[level_4]->AddChildNode(pNode_T5[level_5]);
				level_5 += 1;

				pNode_T5[level_5] = new CTreeNodeUI();
				pNode_T5[level_5]->CheckBoxSelected(false);
				pNode_T5[level_5]->SetItemText(stringToWstring(vec_wxchildName[j][1]).c_str());
				pNode_T5[level_5]->SetFixedHeight(30);
				pNode_T5[level_5]->SetItemTextColor(0xFFFFFFFF);
				pNode_T5[level_5]->SetItemHotTextColor(0xFFFFFFFF);
				pNode_T5[level_4]->SetSelItemTextColor(0xFFFFFFFF);
				pNode_T5[level_5]->SetAttribute(_T("folderattr"), _T("padding=&quot;0,1,0,0&quot; width=&quot;16&quot; height=&quot;16&quot; normalimage=&quot;file='ui/展开.png' &quot; "));
				pNode_T5[level_5]->SetAttribute(_T("folderattr"), _T("hotimage=&quot;file='ui/展开.png' &quot; selectedimage=&quot;file='ui/未展开.png'&quot; selectedhotimage=&quot;file='ui/未展开.png'&quot;"));
				pNode_T5[level_5]->SetAttribute(_T("itemattr"), _T("padding=\"16,0,0,0\" align=\"left\" font=\"13\""));//font=&quot;6&quot;
				pNode_T5[level_5]->SetName(_T("WX1"));

				pNode_T4[level_4]->AddChildNode(pNode_T5[level_5]);
				level_5 += 1;

				pNode_T5[level_5] = new CTreeNodeUI();
				pNode_T5[level_5]->CheckBoxSelected(false);
				pNode_T5[level_5]->SetItemText(stringToWstring(vec_wxchildName[j][2]).c_str());
				pNode_T5[level_5]->SetFixedHeight(30);
				pNode_T5[level_5]->SetItemTextColor(0xFFFFFFFF);
				pNode_T5[level_5]->SetItemHotTextColor(0xFFFFFFFF);
				pNode_T5[level_4]->SetSelItemTextColor(0xFFFFFFFF);
				pNode_T5[level_5]->SetAttribute(_T("folderattr"), _T("padding=&quot;0,1,0,0&quot; width=&quot;16&quot; height=&quot;16&quot; normalimage=&quot;file='ui/展开.png' &quot; "));
				pNode_T5[level_5]->SetAttribute(_T("folderattr"), _T("hotimage=&quot;file='ui/展开.png' &quot; selectedimage=&quot;file='ui/未展开.png'&quot; selectedhotimage=&quot;file='ui/未展开.png'&quot;"));
				pNode_T5[level_5]->SetAttribute(_T("itemattr"), _T("padding=\"16,0,0,0\" align=\"left\" font=\"13\""));//font=&quot;6&quot;
				pNode_T5[level_5]->SetName(_T("WX1"));

				pNode_T4[level_4]->AddChildNode(pNode_T5[level_5]);
				level_5 += 1;
				level_4 += 1;

			}

		}
	}
	////string str = name;
	//vector<string> childName;//存放子节点名称
	//vector<int> childSerialNum;//子节点所在pNode_3中的序号
	//int serialNumber;//存放选择的星座名称
	//CTreeNodeUI* pNode[1024];//卫星筛选的时候的三级节点 存放类型的
	//CTreeNodeUI* pNode_4[1024];//存放四层节点
	///*******************筛选星座名称在存放星座名称的vector中的位置***************/
	//for (int i = 0; i < name2_Level1.size(); i++)
	//{
	//	if (name == name2_Level1[i])
	//	{
	//		serialNumber = i;
	//	}
	//	//pNode_2[i]
	//}
	//int childNum = pNode_2_2[serialNumber]->GetCountChild();
	///**********************筛选当前星座下面的卫星名称***********************/
	//for (int j = 0; j < childNum; j++)
	//{
	//	CDuiString wstr_name1 = pNode_2_2[serialNumber]->GetChildNode(j)->GetItemText();
	//	string str_name1 = wstringToString(wstr_name1.GetData());
	//	childName.push_back(str_name1);
	//	/******************筛选当前星座下卫星所在pNode_3中的顺序***********/
	//	for (int m = 0; m < name2_Level2.size(); m++)
	//	{
	//		if (str_name1 == name2_Level2[m])
	//		{
	//			childSerialNum.push_back(m);

	//		}
	//	}
	//	//pNode_1[0]->Remove(pNode_2[i]);
	//}
	///***********************删除星座下的卫星***********************/
	//for (int j = 0; j < childSerialNum.size(); j++)
	//{
	//	pNode_2_2[serialNumber]->Remove(pNode_2_3[childSerialNum[j]]);
	//}
	//for (int j = 0; j < childSerialNum.size(); j++)
	//{
	//	map<string, string> curr_map = m_RecvMap_DQZ[j];

	//	if (curr_map.count("地球站类型") > 0)
	//	{
	//		string value = curr_map["地球站类型"];
	//		int sequence = GetNoodLevel_level2_DQZ(UTF8_To_string(value), serialNumber);
	//		if (sequence >= 0)
	//		{
	//			cout << "111111111111" << endl;
	//			pNode_4[pNode_2_4_num] = new CTreeNodeUI();
	//			pNode_4[pNode_2_4_num]->CheckBoxSelected(false);
	//			pNode_4[pNode_2_4_num]->SetItemText(stringToWstring(childName[j]).c_str());
	//			pNode_4[pNode_2_4_num]->SetFixedHeight(30);
	//			pNode_4[pNode_2_4_num]->SetItemTextColor(0xFFFFFFFF);
	//			pNode_4[pNode_2_4_num]->SetItemHotTextColor(0xFFFFFFFF);
	//			pNode_4[pNode_2_4_num]->SetSelItemTextColor(0xFFFFFFFF);
	//			pNode_4[pNode_2_4_num]->SetAttribute(_T("folderattr"), _T("padding=&quot;0,1,0,0&quot; width=&quot;16&quot; height=&quot;16&quot; normalimage=&quot;file='ui/展开.png' &quot; "));
	//			pNode_4[pNode_2_4_num]->SetAttribute(_T("folderattr"), _T("hotimage=&quot;file='ui/展开.png' &quot; selectedimage=&quot;file='ui/未展开.png'&quot; selectedhotimage=&quot;file='ui/未展开.png'&quot;"));
	//			pNode_4[pNode_2_4_num]->SetAttribute(_T("itemattr"), _T("padding=\"16,0,0,0\" align=\"left\" font=\"13\""));//font=&quot;6&quot;
	//			pNode_4[pNode_2_4_num]->SetName(_T("WX1"));

	//			pNode[sequence]->AddChildNode(pNode_4[pNode_2_4_num]);
	//			pNode_2_4_num += 1;
	//		}
	//		else
	//		{
	//			pNode[j] = new CTreeNodeUI();
	//			pNode[j]->CheckBoxSelected(false);
	//			pNode[j]->SetItemText(stringToWstring(UTF8_To_string(value)).c_str());
	//			pNode[j]->SetFixedHeight(30);
	//			pNode[j]->SetItemTextColor(0xFFFFFFFF);
	//			pNode[j]->SetItemHotTextColor(0xFFFFFFFF);
	//			pNode[j]->SetSelItemTextColor(0xFFFFFFFF);
	//			pNode[j]->SetAttribute(_T("folderattr"), _T("padding=&quot;0,1,0,0&quot; width=&quot;16&quot; height=&quot;16&quot; normalimage=&quot;file='ui/展开.png' &quot; "));
	//			pNode[j]->SetAttribute(_T("folderattr"), _T("hotimage=&quot;file='ui/展开.png' &quot; selectedimage=&quot;file='ui/未展开.png'&quot; selectedhotimage=&quot;file='ui/未展开.png'&quot;"));
	//			pNode[j]->SetAttribute(_T("itemattr"), _T("padding=\"16,0,0,0\" align=\"left\" font=\"13\""));//font=&quot;6&quot;
	//			pNode[j]->SetName(_T("WX1"));
	//			pNode_2_2[serialNumber]->AddChildNode(pNode[j]);

	//			pNode_4[pNode_2_4_num] = new CTreeNodeUI();
	//			pNode_4[pNode_2_4_num]->CheckBoxSelected(false);
	//			pNode_4[pNode_2_4_num]->SetItemText(stringToWstring(childName[j]).c_str());
	//			pNode_4[pNode_2_4_num]->SetFixedHeight(30);
	//			pNode_4[pNode_2_4_num]->SetItemTextColor(0xFFFFFFFF);
	//			pNode_4[pNode_2_4_num]->SetItemHotTextColor(0xFFFFFFFF);
	//			pNode_4[pNode_2_4_num]->SetSelItemTextColor(0xFFFFFFFF);
	//			pNode_4[pNode_2_4_num]->SetAttribute(_T("folderattr"), _T("padding=&quot;0,1,0,0&quot; width=&quot;16&quot; height=&quot;16&quot; normalimage=&quot;file='ui/展开.png' &quot; "));
	//			pNode_4[pNode_2_4_num]->SetAttribute(_T("folderattr"), _T("hotimage=&quot;file='ui/展开.png' &quot; selectedimage=&quot;file='ui/未展开.png'&quot; selectedhotimage=&quot;file='ui/未展开.png'&quot;"));
	//			pNode_4[pNode_2_4_num]->SetAttribute(_T("itemattr"), _T("padding=\"16,0,0,0\" align=\"left\" font=\"13\""));//font=&quot;6&quot;
	//			pNode_4[pNode_2_4_num]->SetName(_T("WX1"));

	//			pNode[j]->AddChildNode(pNode_4[pNode_2_4_num]);
	//			pNode_2_4_num += 1;
	//		}

	//	}
	//}
}
//地球站-------按地球站类型分类后还原
void CPopEditorialClass::Restore_DQZ_EarthStation(string name)
{
	//string str = name;
	vector<string> childName;//存放子节点名称
	vector<vector<string>> vec_wxchildName;//存放卫星子节点名称
	vector<int> childSerialNum;//子节点所在pNode_3中的序号
	int serialNumber;//存放选择的星座名称
	CTreeNodeUI* pNode[1024];
	int serialNum_wx;
	/*******************筛选星座名称在存放星座名称的vector中的位置***************/
	for (int i = 0; i < name2_Level1.size(); i++)
	{
		if (name == name2_Level1[i])
		{
			serialNumber = i;
		}
		//pNode_2[i]
	}
	/*************************筛选子节点的子节点包含的卫星名称*********************************/
	int childNum = pNode_2_2[serialNumber]->GetCountChild();
	for (int i = 0; i < childNum; i++)
	{
		CTreeNodeUI* pNode_level3 = pNode_2_2[serialNumber]->GetChildNode(i);
		int childNum1 = pNode_level3->GetCountChild();
		//pNode_level3->RemoveAll();
		for (int j = 0; j < childNum1; j++)
		{
			vector<string > vec;
			CDuiString cdui_name = pNode_level3->GetChildNode(j)->GetItemText();
			string str_name = wstringToString(cdui_name.GetData());
			childName.push_back(str_name);//存放的是卫星
			int wxChildNum = pNode_level3->GetChildNode(j)->GetCountChild();
			for (int m = 0; m < wxChildNum; m++)
			{
				CDuiString cdui_name1 = pNode_level3->GetChildNode(j)->GetChildNode(m)->GetItemText();
				string str_name1 = wstringToString(cdui_name1.GetData());
				vec.push_back(str_name1);//存放的是卫星
			}
			vec_wxchildName.push_back(vec);
		}
	}
	/********************删除2级节点******************/
	for (int i = childNum; i > 0; i--)
	{
		pNode_2_2[serialNumber]->Remove(pNode_2_2[serialNumber]->GetChildNode(i - 1));
	}
	/*********************新建恢复为原来的样子**********************/
	int m = 0;
	for (int i = 0; i < childName.size(); i++)
	{

		for (int j = 0; j < name2_Level2.size(); j++)
		{

			if (childName[i] == name2_Level2[j])
			{
				serialNum_wx = j;
				pNode_2_3[serialNum_wx] = new CTreeNodeUI();
				pNode_2_3[serialNum_wx]->CheckBoxSelected(false);
				pNode_2_3[serialNum_wx]->SetItemText(stringToWstring(childName[i]).c_str());
				pNode_2_3[serialNum_wx]->SetFixedHeight(30);
				pNode_2_3[serialNum_wx]->SetItemTextColor(0xFFFFFFFF);
				pNode_2_3[serialNum_wx]->SetItemHotTextColor(0xFFFFFFFF);
				pNode_2_3[serialNum_wx]->SetSelItemTextColor(0xFFFFFFFF);
				pNode_2_3[serialNum_wx]->SetAttribute(_T("folderattr"), _T("padding=&quot;0,1,0,0&quot; width=&quot;16&quot; height=&quot;16&quot; normalimage=&quot;file='ui/展开.png' &quot; "));
				pNode_2_3[serialNum_wx]->SetAttribute(_T("folderattr"), _T("hotimage=&quot;file='ui/展开.png' &quot; selectedimage=&quot;file='ui/未展开.png'&quot; selectedhotimage=&quot;file='ui/未展开.png'&quot;"));
				pNode_2_3[serialNum_wx]->SetAttribute(_T("itemattr"), _T("padding=\"16,0,0,0\" align=\"left\" font=\"13\""));//font=&quot;6&quot;
				pNode_2_3[serialNum_wx]->SetName(_T("WX1"));
				pNode_2_2[serialNumber]->AddChildNode(pNode_2_3[serialNum_wx]);

				//for (int m = 0; m < vec_wxchildName.size(); m++)
				//{
				pNode_2_4[serialNum_wx * 2] = new CTreeNodeUI();
				pNode_2_4[serialNum_wx * 2]->CheckBoxSelected(false);
				pNode_2_4[serialNum_wx * 2]->SetItemText(stringToWstring(vec_wxchildName[m][0]).c_str());
				pNode_2_4[serialNum_wx * 2]->SetFixedHeight(30);
				pNode_2_4[serialNum_wx * 2]->SetItemTextColor(0xFFFFFFFF);
				pNode_2_4[serialNum_wx * 2]->SetItemHotTextColor(0xFFFFFFFF);
				pNode_2_4[serialNum_wx * 2]->SetSelItemTextColor(0xFFFFFFFF);
				pNode_2_4[serialNum_wx * 2]->SetAttribute(_T("folderattr"), _T("padding=&quot;0,1,0,0&quot; width=&quot;16&quot; height=&quot;16&quot; normalimage=&quot;file='ui/展开.png' &quot; "));
				pNode_2_4[serialNum_wx * 2]->SetAttribute(_T("folderattr"), _T("hotimage=&quot;file='ui/展开.png' &quot; selectedimage=&quot;file='ui/未展开.png'&quot; selectedhotimage=&quot;file='ui/未展开.png'&quot;"));
				pNode_2_4[serialNum_wx * 2]->SetAttribute(_T("itemattr"), _T("padding=\"16,0,0,0\" align=\"left\" font=\"13\""));//font=&quot;6&quot;
				pNode_2_4[serialNum_wx * 2]->SetName(_T("WX1"));
				pNode_2_3[serialNum_wx]->AddChildNode(pNode_2_4[serialNum_wx * 2]);

				pNode_2_4[serialNum_wx * 2 + 1] = new CTreeNodeUI();
				pNode_2_4[serialNum_wx * 2 + 1]->CheckBoxSelected(false);
				pNode_2_4[serialNum_wx * 2 + 1]->SetItemText(stringToWstring(vec_wxchildName[m][1]).c_str());
				pNode_2_4[serialNum_wx * 2 + 1]->SetFixedHeight(30);
				pNode_2_4[serialNum_wx * 2 + 1]->SetItemTextColor(0xFFFFFFFF);
				pNode_2_4[serialNum_wx * 2 + 1]->SetItemHotTextColor(0xFFFFFFFF);
				pNode_2_4[serialNum_wx * 2 + 1]->SetSelItemTextColor(0xFFFFFFFF);
				pNode_2_4[serialNum_wx * 2 + 1]->SetAttribute(_T("folderattr"), _T("padding=&quot;0,1,0,0&quot; width=&quot;16&quot; height=&quot;16&quot; normalimage=&quot;file='ui/展开.png' &quot; "));
				pNode_2_4[serialNum_wx * 2 + 1]->SetAttribute(_T("folderattr"), _T("hotimage=&quot;file='ui/展开.png' &quot; selectedimage=&quot;file='ui/未展开.png'&quot; selectedhotimage=&quot;file='ui/未展开.png'&quot;"));
				pNode_2_4[serialNum_wx * 2 + 1]->SetAttribute(_T("itemattr"), _T("padding=\"16,0,0,0\" align=\"left\" font=\"13\""));//font=&quot;6&quot;
				pNode_2_4[serialNum_wx * 2 + 1]->SetName(_T("WX1"));
				pNode_2_3[serialNum_wx]->AddChildNode(pNode_2_4[serialNum_wx * 2 + 1]);

				pNode_2_4[serialNum_wx * 2 + 2] = new CTreeNodeUI();
				pNode_2_4[serialNum_wx * 2 + 2]->CheckBoxSelected(false);
				pNode_2_4[serialNum_wx * 2 + 2]->SetItemText(stringToWstring(vec_wxchildName[m][2]).c_str());
				pNode_2_4[serialNum_wx * 2 + 2]->SetFixedHeight(30);
				pNode_2_4[serialNum_wx * 2 + 2]->SetItemTextColor(0xFFFFFFFF);
				pNode_2_4[serialNum_wx * 2 + 2]->SetItemHotTextColor(0xFFFFFFFF);
				pNode_2_4[serialNum_wx * 2 + 2]->SetSelItemTextColor(0xFFFFFFFF);
				pNode_2_4[serialNum_wx * 2 + 2]->SetAttribute(_T("folderattr"), _T("padding=&quot;0,1,0,0&quot; width=&quot;16&quot; height=&quot;16&quot; normalimage=&quot;file='ui/展开.png' &quot; "));
				pNode_2_4[serialNum_wx * 2 + 2]->SetAttribute(_T("folderattr"), _T("hotimage=&quot;file='ui/展开.png' &quot; selectedimage=&quot;file='ui/未展开.png'&quot; selectedhotimage=&quot;file='ui/未展开.png'&quot;"));
				pNode_2_4[serialNum_wx * 2 + 2]->SetAttribute(_T("itemattr"), _T("padding=\"16,0,0,0\" align=\"left\" font=\"13\""));//font=&quot;6&quot;
				pNode_2_4[serialNum_wx * 2 + 2]->SetName(_T("WX1"));
				pNode_2_3[serialNum_wx]->AddChildNode(pNode_2_4[serialNum_wx * 2 + 2]);
				//}
				m += 1;
			}


		}
	}
	////string str = name;
	//vector<string> childName;//存放子节点名称
	//vector<int> childSerialNum;//子节点所在pNode_3中的序号
	//int serialNumber;//存放选择的星座名称
	//CTreeNodeUI* pNode[1024];
	//int serialNum_wx;
	///*******************筛选星座名称在存放星座名称的vector中的位置***************/
	//for (int i = 0; i < name2_Level1.size(); i++)
	//{
	//	if (name == name2_Level1[i])
	//	{
	//		serialNumber = i;
	//	}
	//	//pNode_2[i]
	//}
	///*************************筛选子节点的子节点包含的卫星名称*********************************/
	//int childNum = pNode_2_2[serialNumber]->GetCountChild();
	//for (int i = 0; i < childNum; i++)
	//{
	//	CTreeNodeUI* pNode_level3 = pNode_2_2[serialNumber]->GetChildNode(i);
	//	int childNum1 = pNode_level3->GetCountChild();
	//	//pNode_level3->RemoveAll();
	//	for (int j = 0; j < childNum1; j++)
	//	{
	//		CDuiString cdui_name = pNode_level3->GetChildNode(j)->GetItemText();
	//		string str_name = wstringToString(cdui_name.GetData());
	//		childName.push_back(str_name);
	//	}
	//}
	///********************删除2级节点******************/
	//for (int i = childNum; i > 0; i--)
	//{
	//	pNode_2_2[serialNumber]->Remove(pNode_2_2[serialNumber]->GetChildNode(i - 1));
	//}
	///*********************新建恢复为原来的样子**********************/
	//for (int i = 0; i < childName.size(); i++)
	//{
	//	for (int j = 0; j < name2_Level2.size(); j++)
	//	{
	//		if (childName[i] == name2_Level2[j])
	//		{
	//			serialNum_wx = j;
	//			pNode_2_3[serialNum_wx] = new CTreeNodeUI();
	//			pNode_2_3[serialNum_wx]->CheckBoxSelected(false);
	//			pNode_2_3[serialNum_wx]->SetItemText(stringToWstring(childName[i]).c_str());
	//			pNode_2_3[serialNum_wx]->SetFixedHeight(30);
	//			pNode_2_3[serialNum_wx]->SetItemTextColor(0xFFFFFFFF);
	//			pNode_2_3[serialNum_wx]->SetItemHotTextColor(0xFFFFFFFF);
	//			pNode_2_3[serialNum_wx]->SetSelItemTextColor(0xFFFFFFFF);
	//			pNode_2_3[serialNum_wx]->SetAttribute(_T("folderattr"), _T("padding=&quot;0,1,0,0&quot; width=&quot;16&quot; height=&quot;16&quot; normalimage=&quot;file='ui/展开.png' &quot; "));
	//			pNode_2_3[serialNum_wx]->SetAttribute(_T("folderattr"), _T("hotimage=&quot;file='ui/展开.png' &quot; selectedimage=&quot;file='ui/未展开.png'&quot; selectedhotimage=&quot;file='ui/未展开.png'&quot;"));
	//			pNode_2_3[serialNum_wx]->SetAttribute(_T("itemattr"), _T("padding=\"16,0,0,0\" align=\"left\" font=\"13\""));//font=&quot;6&quot;
	//			pNode_2_3[serialNum_wx]->SetName(_T("WX1"));
	//			pNode_2_2[serialNumber]->AddChildNode(pNode_2_3[serialNum_wx]);
	//		}
	//	}
	//}
}
//移动地球站到某个电信港节点
void CPopEditorialClass::Movelevel_2_ToDXG(string level1, CDuiString level2)
{
	vector<string> vec_child;//存放要移动的孩子节点
	string str_nameP;//获取level2所在父节点名称
	int sequences1 = -1;
	int childNum = 0;

	for (int i = 0; i < name2_Level2.size(); i++)
	{
		wstring wstr_name, wstr_nameP;
		wstr_name = pNode_2_3[i]->GetItemText();
		string str_level2 = wstringToString(level2.GetData());

		string str_name = wstringToString(wstr_name);

		if (str_level2 == str_name)
		{
			sequences1 = i;//选择的卫星所在name_Level2以及m_RecvMap中的位置
			CTreeNodeUI*  node = pNode_2_3[i]->GetParentNode();//GetParentNode
			wstr_nameP = node->GetItemText();
			str_nameP = wstringToString(wstr_nameP);
			childNum = pNode_2_3[i]->GetCountChild();//获取要移动的卫星节点子节点数量

		}
		/******************获取卫星包含子节点名称****************/
		if (childNum > 0)
		{
			for (int j = 0; j < childNum; j++)
			{
				wstring str_childName;
				string str_ChildName;
				CTreeNodeUI*  node1 = pNode_2_3[sequences1]->GetChildNode(j);;//GetParentNode
				str_childName = node1->GetItemText();
				str_ChildName = wstringToString(str_childName);
				vec_child.push_back(str_ChildName);
			}

			childNum = 0;

		}

	}

	vector<int> sequences_0;
	vector<int> sequencesFrom_0;
	/****************获取卫星子节点所在name_Level3中的顺序***************/
	for (int i = 0; i < vec_child.size(); i++)
	{
		for (int j = 0; j < name2_Level3.size(); j++)
		{
			if (vec_child[i] == name2_Level3[j])
			{
				sequences_0.push_back(j);
			}
		}
	}
	/**********************获取卫星父节点和将要移动到的父节点所在数组中的位置*************************/
	int sequences = 0;
	int sequencesFrom = 0;
	for (int i = 0; i < name2_Level1.size(); i++)
	{
		wstring wstr_name;
		wstr_name = pNode_2_2[i]->GetItemText();
		string str_name = wstringToString(wstr_name);
		if (level1 == str_name)
		{
			sequences = i;//卫星要移动的目标星座节点在pNode_2中的位置
		}
		if (str_nameP == str_name)
		{
			sequencesFrom = i;//卫星本身所在星座节点在pNode_2中的位置
		}

	}
	/********************删除卫星包含的子节点*********************/
	for (int i = 0; i < sequences_0.size(); i++)
	{
		cout << sequencesFrom << endl;
		pNode_2_3[sequences1]->Remove(pNode_2_4[sequences_0[i]]);
	}
	/********************从卫星原来的父节点删除卫星节点 新建卫星节点至移动的目标父节点*********************/
	cout << sequencesFrom << endl;
	pNode_2_2[sequencesFrom]->Remove(pNode_2_3[sequences1]);
	pNode_2_3[sequences1] = new CTreeNodeUI();
	pNode_2_3[sequences1]->CheckBoxSelected(false);
	pNode_2_3[sequences1]->SetItemText(level2);
	pNode_2_3[sequences1]->SetFixedHeight(30);
	pNode_2_3[sequences1]->SetItemTextColor(0xFFFFFFFF);
	pNode_2_3[sequences1]->SetItemHotTextColor(0xFFFFFFFF);
	pNode_2_3[sequences1]->SetSelItemTextColor(0xFFFFFFFF);
	pNode_2_3[sequences1]->SetAttribute(_T("folderattr"), _T("padding=&quot;0,1,0,0&quot; width=&quot;16&quot; height=&quot;16&quot; normalimage=&quot;file='ui/展开.png' &quot; "));
	pNode_2_3[sequences1]->SetAttribute(_T("folderattr"), _T("hotimage=&quot;file='ui/展开.png' &quot; selectedimage=&quot;file='ui/未展开.png'&quot; selectedhotimage=&quot;file='ui/未展开.png'&quot;"));
	pNode_2_3[sequences1]->SetAttribute(_T("itemattr"), _T("padding=\"16,0,0,0\" align=\"left\" font=\"13\""));//font=&quot;6&quot;
	pNode_2_3[sequences1]->SetName(_T("WX1"));
	pNode_2_2[sequences]->AddChildNode(pNode_2_3[sequences1]);

	/************************新建卫星子节点*********************/
	for (int i = 0; i < vec_child.size(); i++)
	{
		pNode_2_4[sequences_0[i]] = new CTreeNodeUI();
		pNode_2_4[sequences_0[i]]->CheckBoxSelected(false);
		pNode_2_4[sequences_0[i]]->SetItemText(stringToWstring(vec_child[i]).c_str());
		pNode_2_4[sequences_0[i]]->SetFixedHeight(30);
		pNode_2_4[sequences_0[i]]->SetFixedWidth(100);
		pNode_2_4[sequences_0[i]]->SetItemTextColor(0xFFFFFFFF);
		pNode_2_4[sequences_0[i]]->SetItemHotTextColor(0xFFFFFFFF);
		pNode_2_4[sequences_0[i]]->SetSelItemTextColor(0xFFFFFFFF);
		pNode_2_4[sequences_0[i]]->SetAttribute(_T("folderattr"), _T("padding=&quot;0,1,0,0&quot; width=&quot;16&quot; height=&quot;16&quot; normalimage=&quot;file='ui/展开.png' &quot; "));
		pNode_2_4[sequences_0[i]]->SetAttribute(_T("folderattr"), _T("hotimage=&quot;file='ui/展开.png' &quot; selectedimage=&quot;file='ui/未展开.png'&quot; selectedhotimage=&quot;file='ui/未展开.png'&quot;"));
		pNode_2_4[sequences_0[i]]->SetAttribute(_T("itemattr"), _T("padding=\"16,0,0,0\" align=\"left\" font=\"13\""));//font=&quot;6&quot;
		pNode_2_4[sequences_0[i]]->SetName(_T("WX1"));
		pNode_2_3[sequences1]->AddChildNode(pNode_2_4[sequences_0[i]]);
	}

}
//获取界面修改的数据 存回相应的vector
map<string, string> CPopEditorialClass::GetDataFromInterfaces_2()
{
	map<string, string> map1;

	for (int i = 0; i < ControlNumber; i++)
	{

		string vec, StoreOptions, StoreOptions1;

		wstring w, kj;
		std::ostringstream streamsssss;
		streamsssss << (i + 1);
		StoreOptions = "titles2_" + streamsssss.str();
		UtilTool::setWstring(w, vec.c_str());
		UtilTool::setWstring(kj, StoreOptions.c_str());
		CControlUI* pControl = static_cast<CControlUI*>(m_pm.FindControl(kj.c_str()));
		wstring wstr_title = pControl->GetText();


		wstring w1, kj1;
		StoreOptions1 = "p2_" + streamsssss.str();
		UtilTool::setWstring(w1, vec.c_str());
		UtilTool::setWstring(kj1, StoreOptions1.c_str());
		CControlUI* pControl1 = static_cast<CControlUI*>(m_pm.FindControl(kj1.c_str()));
		wstring wstr_title1 = pControl1->GetText();
		if (pControl->IsVisible() && pControl1->IsVisible())
		{
			//Vec.push_back(make_pair(wstringToString(wstr_title), wstringToString(wstr_title1)));
			map1[string_To_UTF8(wstringToString(wstr_title))] = string_To_UTF8(wstringToString(wstr_title1));
		}

	}
	return map1;
}
//地面终端按性质分类
void CPopEditorialClass::Categorize_DMZD_TerminalType()
{
	for (int i = 0; i < name3_Level1.size(); i++)
	{
		CDuiString wstr_name = pNode_3_2[i]->GetItemText();
		string str_name = wstringToString(wstr_name.GetData());
		vector<string> childName;
		//vector<int> 
		/************************************/
		int childNum = pNode_3_2[i]->GetCountChild();
		for (int j = 0; j < childNum; j++)
		{
			CDuiString wstr_name1 = pNode_3_2[i]->GetChildNode(j)->GetItemText();
			string str_name1 = wstringToString(wstr_name1.GetData());
			childName.push_back(str_name1);
		}
		/***************移除场景里的1级节点******************/
		pNode_3_1[0]->Remove(pNode_3_2[i]);
		/***************新建筛选后的2级节点******************/
		pNode_3_2[i] = new CTreeNodeUI();
		pNode_3_2[i]->CheckBoxSelected(false);
		pNode_3_2[i]->SetItemText(stringToWstring(str_name).c_str());
		pNode_3_2[i]->SetFixedHeight(30);
		pNode_3_2[i]->SetItemTextColor(0xFFFFFFFF);
		pNode_3_2[i]->SetItemHotTextColor(0xFFFFFFFF);
		pNode_3_2[i]->SetSelItemTextColor(0xFFFFFFFF);
		pNode_3_2[i]->SetAttribute(_T("folderattr"), _T("padding=&quot;0,1,0,0&quot; width=&quot;16&quot; height=&quot;16&quot; normalimage=&quot;file='ui/展开.png' &quot; "));
		pNode_3_2[i]->SetAttribute(_T("folderattr"), _T("hotimage=&quot;file='ui/展开.png' &quot; selectedimage=&quot;file='ui/未展开.png'&quot; selectedhotimage=&quot;file='ui/未展开.png'&quot;"));
		pNode_3_2[i]->SetAttribute(_T("itemattr"), _T("padding=\"16,0,0,0\" align=\"left\" font=\"13\""));//font=&quot;6&quot;
		pNode_3_2[i]->SetName(_T("WX1"));
		map<string, string> curr_map = m_RecvMap_DMZD[i];
		if (curr_map.count("终端类型") > 0)
		{
			string value = curr_map["终端类型"];
			int sequence = GetNoodLevel_DMZD_ZDLX(UTF8_To_string(value));
			if (sequence >= 0)
			{
				pNode_3_1[sequence]->AddChildNode(pNode_3_2[i]);
			}
			else
			{
				CreateTreeNode_3(UTF8_To_string(value));//向根节点加名称
				pNode_3_1[pNode_3_1_num - 1]->AddChildNode(pNode_3_2[i]);
			}

		}
		for (int m = 0; m < childNum; m++)
		{
			for (int n = 0; n < name3_Level2.size(); n++)
			{
				if (name3_Level2[n] == childName[m])
				{
					pNode_3_2[i]->Remove(pNode_3_3[n]);
					pNode_3_3[n] = new CTreeNodeUI();
					pNode_3_3[n]->CheckBoxSelected(false);
					pNode_3_3[n]->SetItemText(stringToWstring(name3_Level2[n]).c_str());
					pNode_3_3[n]->SetFixedHeight(30);
					pNode_3_3[n]->SetItemTextColor(0xFFFFFFFF);
					pNode_3_3[n]->SetItemHotTextColor(0xFFFFFFFF);
					pNode_3_3[n]->SetSelItemTextColor(0xFFFFFFFF);
					pNode_3_3[n]->SetAttribute(_T("folderattr"), _T("padding=&quot;0,1,0,0&quot; width=&quot;16&quot; height=&quot;16&quot; normalimage=&quot;file='ui/展开.png' &quot; "));
					pNode_3_3[n]->SetAttribute(_T("folderattr"), _T("hotimage=&quot;file='ui/展开.png' &quot; selectedimage=&quot;file='ui/未展开.png'&quot; selectedhotimage=&quot;file='ui/未展开.png'&quot;"));
					pNode_3_3[n]->SetAttribute(_T("itemattr"), _T("padding=\"16,0,0,0\" align=\"left\" font=\"13\""));//font=&quot;6&quot;
					pNode_3_3[n]->SetName(_T("WX1"));
					pNode_3_2[i]->AddChildNode(pNode_3_3[n]);
				}
			}
		}


	}
}
//按照性质分类后还原
void CPopEditorialClass::Restore_DMZD_TerminalType()
{
	for (int i = 0; i < name3_Level1.size(); i++)
	{
		vector<string> childName;
		int childNum = pNode_3_2[i]->GetCountChild();
		for (int j = 0; j < childNum; j++)
		{
			CDuiString wstr_name1 = pNode_3_2[i]->GetChildNode(j)->GetItemText();
			string str_name1 = wstringToString(wstr_name1.GetData());
			childName.push_back(str_name1);
		}

		CDuiString wstr_name = pNode_3_2[i]->GetItemText();
		string str_name = wstringToString(wstr_name.GetData());

		int num = GetNoodLevel_Root_DMZD(name3_Level1[i]);
		cout << num << endl;

		pNode_3_1[num]->Remove(pNode_3_2[i]);

		pNode_3_2[i] = new CTreeNodeUI();
		pNode_3_2[i]->CheckBoxSelected(false);
		pNode_3_2[i]->SetItemText(stringToWstring(str_name).c_str());
		pNode_3_2[i]->SetFixedHeight(30);
		pNode_3_2[i]->SetItemTextColor(0xFFFFFFFF);
		pNode_3_2[i]->SetItemHotTextColor(0xFFFFFFFF);
		pNode_3_2[i]->SetSelItemTextColor(0xFFFFFFFF);
		pNode_3_2[i]->SetAttribute(_T("folderattr"), _T("padding=&quot;0,1,0,0&quot; width=&quot;16&quot; height=&quot;16&quot; normalimage=&quot;file='ui/展开.png' &quot; "));
		pNode_3_2[i]->SetAttribute(_T("folderattr"), _T("hotimage=&quot;file='ui/展开.png' &quot; selectedimage=&quot;file='ui/未展开.png'&quot; selectedhotimage=&quot;file='ui/未展开.png'&quot;"));
		pNode_3_2[i]->SetAttribute(_T("itemattr"), _T("padding=\"16,0,0,0\" align=\"left\" font=\"13\""));//font=&quot;6&quot;
		pNode_3_2[i]->SetName(_T("WX1"));
		pNode_3_1[0]->AddChildNode(pNode_3_2[i]);

		for (int m = 0; m < childNum; m++)
		{
			for (int n = 0; n < name3_Level2.size(); n++)
			{
				if (name3_Level2[n] == childName[m])
				{
					pNode_3_2[i]->Remove(pNode_3_3[n]);
					pNode_3_3[n] = new CTreeNodeUI();
					pNode_3_3[n]->CheckBoxSelected(false);
					pNode_3_3[n]->SetItemText(stringToWstring(name3_Level2[n]).c_str());
					pNode_3_3[n]->SetFixedHeight(30);
					pNode_3_3[n]->SetItemTextColor(0xFFFFFFFF);
					pNode_3_3[n]->SetItemHotTextColor(0xFFFFFFFF);
					pNode_3_3[n]->SetSelItemTextColor(0xFFFFFFFF);
					pNode_3_3[n]->SetAttribute(_T("folderattr"), _T("padding=&quot;0,1,0,0&quot; width=&quot;16&quot; height=&quot;16&quot; normalimage=&quot;file='ui/展开.png' &quot; "));
					pNode_3_3[n]->SetAttribute(_T("folderattr"), _T("hotimage=&quot;file='ui/展开.png' &quot; selectedimage=&quot;file='ui/未展开.png'&quot; selectedhotimage=&quot;file='ui/未展开.png'&quot;"));
					pNode_3_3[n]->SetAttribute(_T("itemattr"), _T("padding=\"16,0,0,0\" align=\"left\" font=\"13\""));//font=&quot;6&quot;
					pNode_3_3[n]->SetName(_T("WX1"));
					pNode_3_2[i]->AddChildNode(pNode_3_3[n]);
				}
			}
		}
	}
	RootNodeOnly1_DMZD();

	//for (int i = 0; i < name3_Level1.size(); i++)
	//{
	//	vector<string> childName;
	//	int childNum = pNode_3_2[i]->GetCountChild();
	//	for (int j = 0; j < childNum; j++)
	//	{
	//		CDuiString wstr_name1 = pNode_3_2[i]->GetChildNode(j)->GetItemText();
	//		string str_name1 = wstringToString(wstr_name1.GetData());
	//		childName.push_back(str_name1);
	//	}

	//	CDuiString wstr_name = pNode_3_2[i]->GetItemText();
	//	string str_name = wstringToString(wstr_name.GetData());

	//	int num = GetNoodLevel_Root_DMZD(name3_Level1[i]);
	//	cout << num << endl;

	//	pNode_3_1[num]->Remove(pNode_3_2[i]);

	//	pNode_3_2[i] = new CTreeNodeUI();
	//	pNode_3_2[i]->CheckBoxSelected(false);
	//	pNode_3_2[i]->SetItemText(stringToWstring(str_name).c_str());
	//	pNode_3_2[i]->SetFixedHeight(30);
	//	pNode_3_2[i]->SetItemTextColor(0xFFFFFFFF);
	//	pNode_3_2[i]->SetItemHotTextColor(0xFFFFFFFF);
	//	pNode_3_2[i]->SetSelItemTextColor(0xFFFFFFFF);
	//	pNode_3_2[i]->SetAttribute(_T("folderattr"), _T("padding=&quot;0,1,0,0&quot; width=&quot;16&quot; height=&quot;16&quot; normalimage=&quot;file='ui/展开.png' &quot; "));
	//	pNode_3_2[i]->SetAttribute(_T("folderattr"), _T("hotimage=&quot;file='ui/展开.png' &quot; selectedimage=&quot;file='ui/未展开.png'&quot; selectedhotimage=&quot;file='ui/未展开.png'&quot;"));
	//	pNode_3_2[i]->SetAttribute(_T("itemattr"), _T("padding=\"16,0,0,0\" align=\"left\" font=\"13\""));//font=&quot;6&quot;
	//	pNode_3_2[i]->SetName(_T("WX1"));
	//	pNode_3_1[0]->AddChildNode(pNode_3_2[i]);

	//	//for (int m = 0; m < childNum; m++)
	//	//{
	//	//	for (int n = 0; n < name2_Level2.size(); n++)
	//	//	{
	//	//		if (name2_Level2[n] == childName[m])
	//	//		{
	//	//			pNode_2_2[i]->Remove(pNode_2_3[n]);
	//	//			pNode_2_3[n] = new CTreeNodeUI();
	//	//			pNode_2_3[n]->CheckBoxSelected(false);
	//	//			pNode_2_3[n]->SetItemText(stringToWstring(name2_Level2[n]).c_str());
	//	//			pNode_2_3[n]->SetFixedHeight(30);
	//	//			pNode_2_3[n]->SetItemTextColor(0xFFFFFFFF);
	//	//			pNode_2_3[n]->SetItemHotTextColor(0xFFFFFFFF);
	//	//			pNode_2_3[n]->SetSelItemTextColor(0xFFFFFFFF);
	//	//			pNode_2_3[n]->SetAttribute(_T("folderattr"), _T("padding=&quot;0,1,0,0&quot; width=&quot;16&quot; height=&quot;16&quot; normalimage=&quot;file='ui/展开.png' &quot; "));
	//	//			pNode_2_3[n]->SetAttribute(_T("folderattr"), _T("hotimage=&quot;file='ui/展开.png' &quot; selectedimage=&quot;file='ui/未展开.png'&quot; selectedhotimage=&quot;file='ui/未展开.png'&quot;"));
	//	//			pNode_2_3[n]->SetAttribute(_T("itemattr"), _T("padding=\"16,0,0,0\" align=\"left\" font=\"13\""));//font=&quot;6&quot;
	//	//			pNode_2_3[n]->SetName(_T("WX1"));
	//	//			pNode_2_2[i]->AddChildNode(pNode_2_3[n]);
	//	//		}
	//	//	}
	//	//}
	//}
	//RootNodeOnly1_DMZD();
}
//获取地面终端的终端类型的节点类型所在顺序
int CPopEditorialClass::GetNoodLevel_DMZD_ZDLX(string nodeName)
{
	CDuiString cdui_name;
	string str_name;
	int sequences = -1;
	for (int i = 0; i < pNode_3_1_num; i++)
	{
		cdui_name = pNode_3_1[i]->GetItemText();
		str_name = wstringToString(cdui_name.GetData());
		if (str_name == nodeName)
		{
			sequences = i;
		}
	}

	return sequences;
}
//获地面终端类型的根节点所在顺序
int CPopEditorialClass::GetNoodLevel_Root_DMZD(string nodeName)
{
	CDuiString cdui_name;
	string str_name;
	int sequences = -1;
	for (int i = 0; i < pNode_3_1_num; i++)
	{
		int childNum = pNode_3_1[i]->GetCountChild();
		for (int j = 0; j < childNum; j++)
		{
			CTreeNodeUI*  node = pNode_3_1[i]->GetChildNode(j);//GetParentNode
			cdui_name = node->GetItemText();
			str_name = wstringToString(cdui_name.GetData());
			if (str_name == nodeName)
			{
				sequences = i;
			}
		}
	}
	return sequences;
}
//地面终端分类复原后只留根节点
void CPopEditorialClass::RootNodeOnly1_DMZD()
{
	int num = pNode_3_1_num;
	cout << num << endl;
	for (int i = 0; i < (num - 1); i++)
	{
		m_treeView_3->Remove(pNode_3_1[i + 1]);
		pNode_3_1_num -= 1;
	}
	int num1 = pNode_3_1_num;
	cout << num1 << endl;
}
//将界面3的数据修改后存储成txt
map<string, string> CPopEditorialClass::GetDataFromInterfaces_3()
{
	map<string, string> map1;

	for (int i = 0; i < ControlNumber; i++)
	{

		string vec, StoreOptions, StoreOptions1;

		wstring w, kj;
		std::ostringstream streamsssss;
		streamsssss << (i + 1);
		StoreOptions = "titles3_" + streamsssss.str();
		UtilTool::setWstring(w, vec.c_str());
		UtilTool::setWstring(kj, StoreOptions.c_str());
		CControlUI* pControl = static_cast<CControlUI*>(m_pm.FindControl(kj.c_str()));
		wstring wstr_title = pControl->GetText();


		wstring w1, kj1;
		StoreOptions1 = "p3_" + streamsssss.str();
		UtilTool::setWstring(w1, vec.c_str());
		UtilTool::setWstring(kj1, StoreOptions1.c_str());
		CControlUI* pControl1 = static_cast<CControlUI*>(m_pm.FindControl(kj1.c_str()));
		wstring wstr_title1 = pControl1->GetText();
		if (pControl->IsVisible() && pControl1->IsVisible())
		{
			//Vec.push_back(make_pair(wstringToString(wstr_title), wstringToString(wstr_title1)));
			map1[string_To_UTF8(wstringToString(wstr_title))] = string_To_UTF8(wstringToString(wstr_title1));
		}

	}
	return map1;
}
//取消tab1所有选择
void CPopEditorialClass::UncheckedTab1()
{
	for (int i = 0; i < pNode_1_num; i++)
	{

		int childNum = pNode_1[i]->GetCountChild();
		if (pNode_1[i]->IsSelected())
		{
			pNode_1[i]->Select(false);
		}
		else
		{
			for (int j = 0; j < childNum; j++)
			{

				CTreeNodeUI*  node = pNode_1[i]->GetChildNode(j);//GetParentNode
				if (node->IsSelected())
				{
					node->Select(false);
				}
				else
				{
					//CTreeNodeUI*  node1 = pNode_1[i]->GetChildNode(j)->GetChildNode;//GetParentNode
					int num1 = pNode_1[i]->GetChildNode(j)->GetCountChild();
					for (int m = 0; m < num1; m++)
					{
						CTreeNodeUI*  node1 = node->GetChildNode(m);//GetParentNode
						if (node1->IsSelected())
						{
							node1->Select(false);
						}
						else
						{
							int num3 = node1->GetCountChild();
							for (int n = 0; n < num3; n++)
							{
								CTreeNodeUI*  node3 = node1->GetChildNode(n);//GetParentNode
								if (node3->IsSelected())
								{
									node3->Select(false);
								}
								else
								{
									int num4 = node3->GetCountChild();
									for (int q = 0; q < num4; q++)
									{
										CTreeNodeUI*  node4 = node3->GetChildNode(q);//GetParentNode
										if (node4->IsSelected())
										{
											node4->Select(false);
										}
									}
								}
							}
						}
					}
				}
			}
		}

	}


}
void CPopEditorialClass::UncheckedTab2()
{
	for (int i = 0; i < pNode_2_1_num; i++)
	{
		//			if(pNode_1[i]->)
		int childNum = pNode_2_1[i]->GetCountChild();
		if (pNode_2_1[i]->IsSelected())
		{

			pNode_2_1[i]->Select(false);
			//FindTheMap();
		}
		else
		{
			for (int j = 0; j < childNum; j++)
			{

				CTreeNodeUI*  node = pNode_2_1[i]->GetChildNode(j);//GetParentNode
				if (node->IsSelected())
				{
					node->Select(false);
				}
				else
				{
					//CTreeNodeUI*  node1 = pNode_1[i]->GetChildNode(j)->GetChildNode;//GetParentNode
					int num1 = pNode_2_1[i]->GetChildNode(j)->GetCountChild();
					for (int m = 0; m < num1; m++)
					{
						CTreeNodeUI*  node1 = node->GetChildNode(m);//GetParentNode
						if (node1->IsSelected())
						{
							node1->Select(false);
						}
						else
						{
							int num3 = node1->GetCountChild();
							for (int n = 0; n < num3; n++)
							{
								CTreeNodeUI*  node3 = node1->GetChildNode(n);//GetParentNode
								if (node3->IsSelected())
								{
									node3->Select(false);
								}
							}
						}
					}
				}
			}
		}

	}
}
void CPopEditorialClass::UncheckedTab3()
{
	for (int i = 0; i < pNode_3_1_num; i++)
	{
		//			if(pNode_1[i]->)
		int childNum = pNode_3_1[i]->GetCountChild();
		if (pNode_3_1[i]->IsSelected())
		{
			pNode_3_1[i]->Select(false);

		}
		else
		{
			for (int j = 0; j < childNum; j++)
			{

				CTreeNodeUI*  node = pNode_3_1[i]->GetChildNode(j);//GetParentNode
				if (node->IsSelected())
				{
					node->Select(false);
				}

			}
		}
	}
}
//生成文件
void CPopEditorialClass::GenerateTheFile(string path, string filename, map<string, string> m_map,string sign)
{
	//string path1 = "D:\\DATA1";
	if (0 != _access(path.c_str(), 0))
	{
		_mkdir(path.c_str());   // 返回 0 表示创建成功，-1 表示失败
	}
	string logdata = path + "\\" + filename + ".txt";

	ofstream fout(logdata, ios::out);

	fout << sign << endl;
	for (map<string, string>::iterator iter = m_map.begin(); iter != m_map.end(); ++iter)
	{
		cout << "key:" << iter->first << " value:" << iter->second << endl;
		string str_first = iter->first;
		string str_second = iter->second;
		string str = str_first + ":";
		str += str_second;
		fout << str << endl;
	}
}

//生成文件
void CPopEditorialClass::GenerateTheFile(string path, string filename, vector<vector<string>> vecVec)
{
	if (0 != _access(path.c_str(), 0))
	{
		_mkdir(path.c_str());   // 返回 0 表示创建成功，-1 表示失败
	}
	string logdata = path + "\\" + filename + ".txt";

	ofstream fout(logdata, ios::out);
	for (int i = 0; i < vecVec.size(); i++)
	{
		string line;
		for (int j = 0; j < vecVec[i].size(); j++)
		{
			if (j == 0)
			{
				line += vecVec[i][j];
				line += ":";
			}
			else
			{
				line += vecVec[i][j];
				line += "-";
			}

		}
		fout << line << endl;
	}
}
void CPopEditorialClass::saveDatabaseScheme(const std::string& strScheme)
{
	std::set<std::string> filelds;
	std::vector<std::string> vecFilelds;
	std::vector<std::string> vecFileldsValue;
	/*******************新建星座表及字段********************/                        //////// ?????? 考量 星座字段不一致的问题
	

	
	if (name_Level1.size() > 0)
	{
		//std::vector<std::string> filelds;
		std::string strTableNameXZ = strScheme + UTF8_To_string("_星座");
		filelds.clear();
		filelds.insert("Name");
		
		for (int i = 0; i < m_RecvMap_XZ.size(); i++)
		{
			for (auto map : m_RecvMap_XZ[i])
			{
				filelds.insert(UTF8_To_string(map.first));
			}

		}
		//	strTable = UTF8_To_string(strTable);
		ConnectMysql::Instance().createTableAndFields(strTableNameXZ, filelds);

		// 插入数据
		for (int i = 0; i < m_RecvMap_XZ.size(); i++)
		{
			vecFilelds.clear();
			vecFileldsValue.clear();

			vecFilelds.push_back(UTF8_To_string("Name"));
			vecFileldsValue.push_back((name_Level1[i]));
			for (auto map : m_RecvMap_XZ[i])
			{
				vecFilelds.push_back(UTF8_To_string(map.first));
				vecFileldsValue.push_back(UTF8_To_string(map.second));
			}

			ConnectMysql::Instance().insertTableData(strTableNameXZ, vecFilelds, vecFileldsValue);

		}
	}
	/*******************新建卫星表及字段********************/// 简单插入数据 或者根据字段名插入值;
	
	if (name_Level2.size() > 0)
	{
		//std::set<std::string> filelds;
		filelds.clear();
		std::string strTableNameWX = strScheme + UTF8_To_string("_卫星");
		filelds.insert("Name");
		for (int i = 0; i < m_RecvMap.size(); i++)
		{
			for (auto map : m_RecvMap[i])
			{
				filelds.insert(UTF8_To_string(map.first));
			}

		}

		ConnectMysql::Instance().createTableAndFields(strTableNameWX, filelds);

		// 插入数据
		for (int i = 0; i < m_RecvMap.size(); i++)
		{
			vecFilelds.clear();
			vecFileldsValue.clear();

			vecFilelds.push_back(UTF8_To_string("Name"));
			vecFileldsValue.push_back((name_Level2[i]));
			for (auto map : m_RecvMap[i])
			{
				vecFilelds.push_back(UTF8_To_string(map.first));
				vecFileldsValue.push_back(UTF8_To_string(map.second));
			}

			ConnectMysql::Instance().insertTableData(strTableNameWX, vecFilelds, vecFileldsValue);
		}
	}

	/*******************新建卫星天线表及字段********************/
	if (m_vecTxName.size() > 0)
	{
		
		filelds.clear();
		std::string strTableNameWXTX = strScheme + UTF8_To_string("_卫星天线");
		filelds.insert("Name");
		for (int i = 0; i < m_vecMapTxData.size(); i++)
		{
			for (auto map : m_vecMapTxData[i])
			{
				filelds.insert(UTF8_To_string(map.first));
			}

		}

		ConnectMysql::Instance().createTableAndFields(strTableNameWXTX, filelds);

		// 插入数据
		for (int i = 0; i < m_vecMapTxData.size(); i++)
		{
			vecFilelds.clear();
			vecFileldsValue.clear();

			vecFilelds.push_back(UTF8_To_string("Name"));
			vecFileldsValue.push_back((m_vecTxName[i]));
			for (auto map : m_vecMapTxData[i])
			{
				vecFilelds.push_back(UTF8_To_string(map.first));
				vecFileldsValue.push_back(UTF8_To_string(map.second));
			}

			ConnectMysql::Instance().insertTableData(strTableNameWXTX, vecFilelds, vecFileldsValue);
		}
	}

	/*******************新建卫星载荷表及字段********************/
	if (m_vecZhName.size() > 0)
	{

		filelds.clear();
		std::string strTableNameWXZH = strScheme + UTF8_To_string("_卫星载荷");
		filelds.insert("Name");
		for (int i = 0; i < m_vecMapZhData.size(); i++)
		{
			for (auto map : m_vecMapZhData[i])
			{
				filelds.insert(UTF8_To_string(map.first));
			}

		}

		ConnectMysql::Instance().createTableAndFields(strTableNameWXZH, filelds);

		// 插入数据
		for (int i = 0; i < m_vecMapZhData.size(); i++)
		{
			vecFilelds.clear();
			vecFileldsValue.clear();

			vecFilelds.push_back(UTF8_To_string("Name"));
			vecFileldsValue.push_back((m_vecZhName[i]));
			for (auto map : m_vecMapZhData[i])
			{
				vecFilelds.push_back(UTF8_To_string(map.first));
				vecFileldsValue.push_back(UTF8_To_string(map.second));
			}

			ConnectMysql::Instance().insertTableData(strTableNameWXZH, vecFilelds, vecFileldsValue);
		}
	}

	/*******************新建电信港表及字段********************/
	if (name2_Level1.size() > 0)
	{
		filelds.clear();
		std::string strTableNameDXG = strScheme + UTF8_To_string("_电信港");
		filelds.insert("Name");
		for (int i = 0; i < m_RecvMap_DXG.size(); i++)
		{
			for (auto map : m_RecvMap_DXG[i])
			{
				filelds.insert(UTF8_To_string(map.first));
			}
		}

		ConnectMysql::Instance().createTableAndFields(strTableNameDXG, filelds);

		// 插入数据
		for (int i = 0; i < m_RecvMap_DXG.size(); i++)
		{
			vecFilelds.clear();
			vecFileldsValue.clear();

			vecFilelds.push_back(UTF8_To_string("Name"));
			vecFileldsValue.push_back((name2_Level1[i]));
			for (auto map : m_RecvMap_DXG[i])
			{
				vecFilelds.push_back(UTF8_To_string(map.first));
				vecFileldsValue.push_back(UTF8_To_string(map.second));
			}

			ConnectMysql::Instance().insertTableData(strTableNameDXG, vecFilelds, vecFileldsValue);
		}
	}

	/*******************新建地球站表及字段********************/
	if (name2_Level2.size() > 0)
	{

		filelds.clear();
		std::string strTableNameDQZ = strScheme + UTF8_To_string("_地球站");
		filelds.insert("Name");
		for (int i = 0; i < m_RecvMap_DQZ.size(); i++)
		{
			for (auto map : m_RecvMap_DQZ[i])
			{
				filelds.insert(UTF8_To_string(map.first));
			}

		}

		ConnectMysql::Instance().createTableAndFields(strTableNameDQZ, filelds);

		// 插入数据
		for (int i = 0; i < m_RecvMap_DQZ.size(); i++)
		{
			vecFilelds.clear();
			vecFileldsValue.clear();

			vecFilelds.push_back(UTF8_To_string("Name"));
			vecFileldsValue.push_back((name2_Level2[i]));
			for (auto map : m_RecvMap_DQZ[i])
			{
				vecFilelds.push_back(UTF8_To_string(map.first));
				vecFileldsValue.push_back(UTF8_To_string(map.second));
			}

			ConnectMysql::Instance().insertTableData(strTableNameDQZ, vecFilelds, vecFileldsValue);
		}
	}

	/*******************新建地球站波形表及字段********************/
	if (m_vecDqzBxName.size() > 0)
	{

		filelds.clear();
		std::string strTableNameDQZBX = strScheme + UTF8_To_string("_地球站波形");
		filelds.insert("Name");
		for (int i = 0; i < m_vecMapDqzBxData.size(); i++)
		{
			for (auto map : m_vecMapDqzBxData[i])
			{
				filelds.insert(UTF8_To_string(map.first));
			}

		}
		ConnectMysql::Instance().createTableAndFields(strTableNameDQZBX, filelds);
		// 插入数据
		for (int i = 0; i < m_vecMapDqzBxData.size(); i++)
		{
			vecFilelds.clear();
			vecFileldsValue.clear();

			vecFilelds.push_back(UTF8_To_string("Name"));
			vecFileldsValue.push_back((m_vecDqzBxName[i]));
			for (auto map : m_vecMapDqzBxData[i])
			{
				vecFilelds.push_back(UTF8_To_string(map.first));
				vecFileldsValue.push_back(UTF8_To_string(map.second));
			}

			ConnectMysql::Instance().insertTableData(strTableNameDQZBX, vecFilelds, vecFileldsValue);
		}
	}
	/*******************新建地球站溃源表及字段********************/
	if (m_vecDqzkyName.size() > 0)
	{

		filelds.clear();
		std::string strTableNameDQZKY = strScheme + UTF8_To_string("_地球站溃源");
		filelds.insert("Name");
		for (int i = 0; i < m_vecMapDqzKyData.size(); i++)
		{
			for (auto map : m_vecMapDqzKyData[i])
			{
				filelds.insert(UTF8_To_string(map.first));
			}

		}
		ConnectMysql::Instance().createTableAndFields(strTableNameDQZKY, filelds);
		// 插入数据
		for (int i = 0; i < m_vecMapDqzKyData.size(); i++)
		{
			vecFilelds.clear();
			vecFileldsValue.clear();

			vecFilelds.push_back(UTF8_To_string("Name"));
			vecFileldsValue.push_back((m_vecDqzkyName[i]));
			for (auto map : m_vecMapDqzKyData[i])
			{
				vecFilelds.push_back(UTF8_To_string(map.first));
				vecFileldsValue.push_back(UTF8_To_string(map.second));
			}

			ConnectMysql::Instance().insertTableData(strTableNameDQZKY, vecFilelds, vecFileldsValue);
		}
	}

	/*******************新建地球站天线表及字段********************/
	if (m_vecDqzTxName.size() > 0)
	{

		filelds.clear();
		std::string strTableNameDQZTX = strScheme + UTF8_To_string("_地球站天线");
		filelds.insert("Name");
		for (int i = 0; i < m_vecMapDqzTxData.size(); i++)
		{
			for (auto map : m_vecMapDqzTxData[i])
			{
				filelds.insert(UTF8_To_string(map.first));
			}

		}
		ConnectMysql::Instance().createTableAndFields(strTableNameDQZTX, filelds);
		// 插入数据
		for (int i = 0; i < m_vecMapDqzTxData.size(); i++)
		{
			vecFilelds.clear();
			vecFileldsValue.clear();

			vecFilelds.push_back(UTF8_To_string("Name"));
			vecFileldsValue.push_back((m_vecDqzTxName[i]));
			for (auto map : m_vecMapDqzTxData[i])
			{
				vecFilelds.push_back(UTF8_To_string(map.first));
				vecFileldsValue.push_back(UTF8_To_string(map.second));
			}

			ConnectMysql::Instance().insertTableData(strTableNameDQZTX, vecFilelds, vecFileldsValue);
		}
	}

	/*******************新建地面终端表及字段********************/
	if (name3_Level1.size() > 0)
	{

		filelds.clear();
		std::string strTableNameDMZD = strScheme + UTF8_To_string("_地面终端");
		filelds.insert("Name");
		for (int i = 0; i < m_RecvMap_DMZD.size(); i++)
		{
			for (auto map : m_RecvMap_DMZD[i])
			{
				filelds.insert(UTF8_To_string(map.first));
			}

		}

		ConnectMysql::Instance().createTableAndFields(strTableNameDMZD, filelds);

		// 插入数据
		for (int i = 0; i < m_RecvMap_DMZD.size(); i++)
		{
			vecFilelds.clear();
			vecFileldsValue.clear();

			vecFilelds.push_back(UTF8_To_string("Name"));
			vecFileldsValue.push_back((name3_Level1[i]));
			for (auto map : m_RecvMap_DMZD[i])
			{
				vecFilelds.push_back(UTF8_To_string(map.first));
				vecFileldsValue.push_back(UTF8_To_string(map.second));
			}

			ConnectMysql::Instance().insertTableData(strTableNameDMZD, vecFilelds, vecFileldsValue);
		}
	}

	/*******************新建地面终端表波形及字段********************/
	if (m_vecDmzdBxName.size() > 0)
	{

		filelds.clear();
		std::string strTableNameDMZDBX = strScheme + UTF8_To_string("_地面终端波形");
		filelds.insert("Name");
		for (int i = 0; i < m_vecMapDmzdBxData.size(); i++)
		{
			for (auto map : m_vecMapDmzdBxData[i])
			{
				filelds.insert(UTF8_To_string(map.first));
			}
		}

		ConnectMysql::Instance().createTableAndFields(strTableNameDMZDBX, filelds);
		// 插入数据
		for (int i = 0; i < m_vecMapDmzdBxData.size(); i++)
		{
			vecFilelds.clear();
			vecFileldsValue.clear();

			vecFilelds.push_back(UTF8_To_string("Name"));
			vecFileldsValue.push_back((m_vecDmzdBxName[i]));
			for (auto map : m_vecMapDmzdBxData[i])
			{
				vecFilelds.push_back(UTF8_To_string(map.first));
				vecFileldsValue.push_back(UTF8_To_string(map.second));
			}
			// 插入数据;
			ConnectMysql::Instance().insertTableData(strTableNameDMZDBX, vecFilelds, vecFileldsValue);
		}
	}

	/*******************新建地面终端载荷表及字段********************/
	if (m_vecDmzdZhName.size() > 0)
	{

		filelds.clear();
		std::string strTableNameDMZDZH = strScheme + UTF8_To_string("_地面终端载荷");
		filelds.insert("Name");
		for (int i = 0; i < m_vecMapDmzdZhData.size(); i++)
		{
			for (auto map : m_vecMapDmzdZhData[i])
			{
				filelds.insert(UTF8_To_string(map.first));
			}
		}

		ConnectMysql::Instance().createTableAndFields(strTableNameDMZDZH, filelds);
		// 插入数据
		for (int i = 0; i < m_vecMapDmzdZhData.size(); i++)
		{
			vecFilelds.clear();
			vecFileldsValue.clear();

			vecFilelds.push_back(UTF8_To_string("Name"));
			vecFileldsValue.push_back((m_vecDmzdZhName[i]));
			for (auto map : m_vecMapDmzdZhData[i])
			{
				vecFilelds.push_back(UTF8_To_string(map.first));
				vecFileldsValue.push_back(UTF8_To_string(map.second));
			}
			// 插入数据;
			ConnectMysql::Instance().insertTableData(strTableNameDMZDZH, vecFilelds, vecFileldsValue);
		}
	}

	/*******************新建地面终端天线表及字段********************/
	if (m_vecDmzdTxName.size() > 0)
	{

		filelds.clear();
		std::string strTableNameDMZDTX = strScheme + UTF8_To_string("_地面终端天线");
		filelds.insert("Name");
		for (int i = 0; i < m_vecMapDmzdTxData.size(); i++)
		{
			for (auto map : m_vecMapDmzdTxData[i])
			{
				filelds.insert(UTF8_To_string(map.first));
			}
		}

		ConnectMysql::Instance().createTableAndFields(strTableNameDMZDTX, filelds);
		// 插入数据
		for (int i = 0; i < m_vecMapDmzdTxData.size(); i++)
		{
			vecFilelds.clear();
			vecFileldsValue.clear();

			vecFilelds.push_back(UTF8_To_string("Name"));
			vecFileldsValue.push_back((m_vecDmzdTxName[i]));
			for (auto map : m_vecMapDmzdTxData[i])
			{
				vecFilelds.push_back(UTF8_To_string(map.first));
				vecFileldsValue.push_back(UTF8_To_string(map.second));
			}
			// 插入数据;
			ConnectMysql::Instance().insertTableData(strTableNameDMZDTX, vecFilelds, vecFileldsValue);
		}
	}
}
//将tab1界面的本地数据导入界面函数
void CPopEditorialClass::ImportingLocalData_Tab1()
{
	m_vecTxName.clear();
	m_vecZhName.clear();
	m_vecMapTxData.clear();
	m_vecMapZhData.clear();

	vector<string> vec_TxZhName;//卫星的子节点 --- 天线载荷名称
	vector<map<string, string>> vec_TxZhData;//卫星的子节点 --- 天线载荷数据

	vector<string> vec_WxName;
	vector<map<string, string>> vec_wxData;//遍历文件并将卫星数据存储下来

	vector<vector<string>> vecXZAndWX;//每一行都存放 星座:卫星-卫星-。。。
	vector<vector<string>> vecWXAndTxZh;//每一行都存放 卫星:天线-载荷-。。。
	vector<wstring> upload_fileName;
	upload_fileName = UtilTool::GetOpenFilePaths_txt();
	/*********************先把星座的筛选出来显示在界面上*********************/
	for (int i = 0; i < upload_fileName.size(); i++)
	{
		string fileN;
		UtilTool::setString(fileN, upload_fileName[i].c_str());

		vector<string> str_xzFileName;
		string fgf = "\\";
		str_xzFileName = my_split(fileN, fgf);
		string fileName = str_xzFileName[str_xzFileName.size() - 1];

		vector<string> str_xzName;
		fgf = ".";
		str_xzName = my_split(fileName, fgf);
		string Name = str_xzName[str_xzName.size() - 1];

		ifstream fin(fileN, ios::binary);
		unsigned char  s2;
		fin.read((char*)&s2, sizeof(s2));//读取第一个字节，然后左移8位
		int p = s2 << 8;
		fin.read((char*)&s2, sizeof(s2));//读取第二个字节
		p += s2;

		string code;
		fin.close();

		ifstream infile;
		infile.open(fileN);   //将文件流对象与文件连接起来 
		//assert(infile.is_open());   //若失败,则输出错误消息,并终止程序运行 
		if (infile)
		{
			string s;
			map<string, string> l_map;
			bool flag = true;
			string type;
			while (getline(infile, s))
			{
				if (flag)//如果是第一行 第一行写着
				{
					cout << "第一行" << endl;
					type = s;

					flag = false;
				}
				else
				{
					cout << "第二行" << endl;
					cout << s.c_str() << endl;
					vector<string> str_;
					string pattern = ":";
					str_ = my_split(s, pattern);
					if (p == 58760 || p == 59032)//文件是UTF-8类型
					{
						l_map[str_[0]] = str_[1];
					}
					else if (p == 46260)//文件是其他类型
					{

						l_map[string_To_UTF8(str_[0])] = string_To_UTF8(str_[1]);
					}
					else
					{
						l_map[str_[0]] = str_[1];
					}
				}


			}
			if (type == "星座")
			{
				cout << "星座" << endl;
				AssignmentToLocalVector_XZ(str_xzName[0], l_map);//将星座相关信息显示在界面上

			//	std::vector<std::string> filelds;
			//	for (auto map : l_map)
			//	{
			//		filelds.push_back(UTF8_To_string(map.first));
			//	}
			//	std::string strTable = UTF8_To_string("方案1_") + str_xzName[0];
			////	strTable = UTF8_To_string(strTable);
			//	ConnectMysql::Instance().createTableAndFields(strTable, filelds);
			}
			
			infile.close();             //关闭文件输入流 
		}
		//m_FtpClientClass.execute_putFile(fileN);//put 上传文件
	}
	/*********************把星座配置文件筛选出来**********************************/
	for (int i = 0; i < upload_fileName.size(); i++)
	{
		//vector<vector<string>> vecVec_XzWx;
		string fileN;
		UtilTool::setString(fileN, upload_fileName[i].c_str());

		vector<string> str_xzFileName;
		string fgf = "\\";
		str_xzFileName = my_split(fileN, fgf);
		string fileName = str_xzFileName[str_xzFileName.size() - 1];

		vector<string> str_xzName;
		fgf = ".";
		str_xzName = my_split(fileName, fgf);
		string Name = str_xzName[0];
		cout << Name << endl;
		if (str_xzName[0] == UTF8_To_string("配置文件_星座"))
		{
			ifstream fin(fileN, ios::binary);
			unsigned char  s2;
			fin.read((char*)&s2, sizeof(s2));//读取第一个字节，然后左移8位
			int p = s2 << 8;
			fin.read((char*)&s2, sizeof(s2));//读取第二个字节
			p += s2;

			string code;
			fin.close();

			ifstream infile;
			infile.open(fileN);   //将文件流对象与文件连接起来 
			//assert(infile.is_open());   //若失败,则输出错误消息,并终止程序运行 
			if (infile)
			{
				string s;
				map<string, string> l_map;

				while (getline(infile, s))
				{
					vector<string> line;
					cout << "第二行" << endl;
					cout << s.c_str() << endl;
					vector<string> str_;
					string pattern = ":";
					str_ = my_split(s, pattern);
					if (p == 58760 || p == 59032)//文件是UTF-8类型
					{
						line.push_back(str_[0]);

						vector<string> str_wx;
						pattern = "-";
						str_wx = my_split(str_[1], pattern);
						for (int i = 0; i < str_wx.size(); i++)
						{
							line.push_back(str_wx[i]);
						}
						//l_map[str_[0]] = str_[1];
					}
					else if (p == 46260 || p == 21364)//文件是其他类型
					{
						line.push_back(str_[0]);

						vector<string> str_wx;
						pattern = "-";
						str_wx = my_split(str_[1], pattern);
						for (int i = 0; i < str_wx.size(); i++)
						{
							line.push_back(string_To_UTF8(str_wx[i]));
						}
						//l_map[string_To_UTF8(str_[0])] = string_To_UTF8(str_[1]);
					}
					else
					{
						line.push_back(str_[0]);

						vector<string> str_wx;
						pattern = "-";
						str_wx = my_split(str_[1], pattern);
						for (int i = 0; i < str_wx.size(); i++)
						{
							line.push_back(str_wx[i]);
						}
					}
					vecXZAndWX.push_back(line);
				}


			}

			cout << "11111" << endl;
		}
	}
	/*********************把卫星的数据筛选出来*************************************/
	for (int i = 0; i < upload_fileName.size(); i++)
	{
		string fileN;
		UtilTool::setString(fileN, upload_fileName[i].c_str());

		vector<string> str_xzFileName;
		string fgf = "\\";
		str_xzFileName = my_split(fileN, fgf);
		string fileName = str_xzFileName[str_xzFileName.size() - 1];

		vector<string> str_xzName;
		fgf = ".";
		str_xzName = my_split(fileName, fgf);
		string Name = str_xzName[str_xzName.size() - 1];

		ifstream fin(fileN, ios::binary);
		unsigned char  s2;
		fin.read((char*)&s2, sizeof(s2));//读取第一个字节，然后左移8位
		int p = s2 << 8;
		fin.read((char*)&s2, sizeof(s2));//读取第二个字节
		p += s2;

		string code;
		fin.close();

		ifstream infile;
		infile.open(fileN);   //将文件流对象与文件连接起来 
		//assert(infile.is_open());   //若失败,则输出错误消息,并终止程序运行 
		if (infile)
		{
			string s;
			map<string, string> l_map;
			bool flag = true;
			string type;
			while (getline(infile, s))
			{
				if (flag)//如果是第一行 第一行写着
				{
					cout << "第一行" << endl;
					type = s;

					flag = false;
				}
				else
				{
					cout << "第二行" << endl;
					cout << s.c_str() << endl;
					vector<string> str_;
					string pattern = ":";
					str_ = my_split(s, pattern);
					if (p == 58760 || p == 59032 || p == 58765)//文件是UTF-8类型
					{
						l_map[str_[0]] = str_[1];
					}
					else if (p == 46260)//文件是其他类型
					{

						l_map[string_To_UTF8(str_[0])] = string_To_UTF8(str_[1]);
					}
					else
					{
						l_map[str_[0]] = str_[1];
					}
				}


			}
			if (type == "卫星")
			{
				cout << "卫星" << endl;
				vec_WxName.push_back(str_xzName[0]);
				vec_wxData.push_back(l_map);
				//AssignmentToLocalVector_XZ(str_xzName[0], l_map);
			}


			infile.close();             //关闭文件输入流 
		}
	}
	/*********************把天线和载荷的数据筛选出来******************************/
	for (int i = 0; i < upload_fileName.size(); i++)
	{
		string fileN;
		UtilTool::setString(fileN, upload_fileName[i].c_str());

		vector<string> str_xzFileName;
		string fgf = "\\";
		str_xzFileName = my_split(fileN, fgf);
		string fileName = str_xzFileName[str_xzFileName.size() - 1];

		vector<string> str_xzName;
		fgf = ".";
		str_xzName = my_split(fileName, fgf);
		string Name = str_xzName[str_xzName.size() - 1];

		ifstream fin(fileN, ios::binary);
		unsigned char  s2;
		fin.read((char*)&s2, sizeof(s2));//读取第一个字节，然后左移8位
		int p = s2 << 8;
		fin.read((char*)&s2, sizeof(s2));//读取第二个字节
		p += s2;

		string code;
		fin.close();

		ifstream infile;
		infile.open(fileN);   //将文件流对象与文件连接起来 
		//assert(infile.is_open());   //若失败,则输出错误消息,并终止程序运行 
		if (infile)
		{
			string s;
			map<string, string> l_map;
			bool flag = true;
			string type;
			while (getline(infile, s))
			{
				if (flag)//如果是第一行 第一行写着
				{
					cout << "第一行" << endl;
					type = s;

					flag = false;
				}
				else
				{
					cout << "第二行" << endl;
					cout << s.c_str() << endl;
					vector<string> str_;
					string pattern = ":";
					str_ = my_split(s, pattern);
					if (p == 58760 || p == 59032 || p == 58765)//文件是UTF-8类型
					{
						l_map[str_[0]] = str_[1];
					}
					else if (p == 46260)//文件是其他类型
					{

						l_map[string_To_UTF8(str_[0])] = string_To_UTF8(str_[1]);
					}
					else
					{
						l_map[str_[0]] = str_[1];
					}
				}


			}
			//if (type == "天线或载荷")
			//{
			//	cout << "卫星" << endl;
			//	vec_TxZhName.push_back(str_xzName[0]);
			//	vec_TxZhData.push_back(l_map);
			//	//AssignmentToLocalVector_XZ(str_xzName[0], l_map);
			//}

			if (type == "天线")
			{
				cout << "卫星" << endl;
				vec_TxZhName.push_back(str_xzName[0]);
				vec_TxZhData.push_back(l_map);

				
				m_vecTxName.push_back(str_xzName[0]);
				m_vecMapTxData.push_back(l_map);
			}
			else if(type == "载荷")
			{
				vec_TxZhName.push_back(str_xzName[0]);
			    vec_TxZhData.push_back(l_map);

				m_vecZhName.push_back(str_xzName[0]);
				m_vecMapZhData.push_back(l_map);
			}


			infile.close();             //关闭文件输入流 
		}
	}
	/*********************把卫星配置文件筛选出来**********************************/
	for (int i = 0; i < upload_fileName.size(); i++)
	{
		//vector<vector<string>> vecVec_XzWx;
		string fileN;
		UtilTool::setString(fileN, upload_fileName[i].c_str());

		vector<string> str_xzFileName;
		string fgf = "\\";
		str_xzFileName = my_split(fileN, fgf);
		string fileName = str_xzFileName[str_xzFileName.size() - 1];

		vector<string> str_xzName;
		fgf = ".";
		str_xzName = my_split(fileName, fgf);
		string Name = str_xzName[0];
		cout << Name << endl;
		if (str_xzName[0] == UTF8_To_string("配置文件_卫星"))
		{
			ifstream fin(fileN, ios::binary);
			unsigned char  s2;
			fin.read((char*)&s2, sizeof(s2));//读取第一个字节，然后左移8位
			int p = s2 << 8;
			fin.read((char*)&s2, sizeof(s2));//读取第二个字节
			p += s2;

			string code;
			fin.close();

			ifstream infile;
			infile.open(fileN);   //将文件流对象与文件连接起来 
			//assert(infile.is_open());   //若失败,则输出错误消息,并终止程序运行 
			if (infile)
			{
				string s;
				map<string, string> l_map;

				while (getline(infile, s))
				{
					vector<string> line;
					cout << "第二行" << endl;
					cout << s.c_str() << endl;
					vector<string> str_;
					string pattern = ":";
					str_ = my_split(s, pattern);
					if (p == 58760 || p == 59032)//文件是UTF-8类型
					{
						line.push_back(str_[0]);

						vector<string> str_wx;
						pattern = "-";
						str_wx = my_split(str_[1], pattern);
						for (int i = 0; i < str_wx.size(); i++)
						{
							line.push_back(str_wx[i]);
						}
						//l_map[str_[0]] = str_[1];
					}
					else if (p == 46260 || p == 21364)//文件是其他类型
					{
						line.push_back(str_[0]);

						vector<string> str_wx;
						pattern = "-";
						str_wx = my_split(str_[1], pattern);
						for (int i = 0; i < str_wx.size(); i++)
						{
							line.push_back(string_To_UTF8(str_wx[i]));
						}
						//l_map[string_To_UTF8(str_[0])] = string_To_UTF8(str_[1]);
					}
					else
					{
						line.push_back(str_[0]);

						vector<string> str_wx;
						pattern = "-";
						str_wx = my_split(str_[1], pattern);
						for (int i = 0; i < str_wx.size(); i++)
						{
							line.push_back(str_wx[i]);
						}
					}
					vecWXAndTxZh.push_back(line);
				}


			}

			cout << "11111" << endl;
		}
	}
	/*********************创建卫星节点在treeView中的一级节点下面*******************/
	for (int i = 0; i < vec_WxName.size(); i++)
	{
		int seques = 0;
		string xzName;//星座名称
		for (int j = 0; j < vecXZAndWX.size(); j++)
		{
			for (int m = 0; m < vecXZAndWX[j].size(); m++)
			{
				if (UTF8_To_string(vecXZAndWX[j][m]) == vec_WxName[i])
				{
					seques = j;
					xzName = vecXZAndWX[j][0];
					AssignmentToLocalVector(xzName, vec_WxName[i], vec_wxData[i]);
				}
				else if (vecXZAndWX[j][m] == vec_WxName[i])
				{
					seques = j;
					xzName = vecXZAndWX[j][0];
					AssignmentToLocalVector(xzName, vec_WxName[i], vec_wxData[i]);
				}
			}
		}
	}
	/***********************创建卫星的天线和载荷在treeview*************************************/
	for (int i = 0; i < vec_TxZhName.size(); i++)
	{
		int seques = 0;
		string xzName;//星座名称
		for (int j = 0; j < vecWXAndTxZh.size(); j++)
		{
			for (int m = 0; m < vecWXAndTxZh[j].size(); m++)
			{
				if (UTF8_To_string(vecWXAndTxZh[j][m]) == vec_TxZhName[i])
				{
					seques = j;
					xzName = vecWXAndTxZh[j][0];
					AssignmentToLocalVector_TxZh(xzName, vec_TxZhName[i], vec_TxZhData[i]);
				}
				if (vecWXAndTxZh[j][m] == vec_TxZhName[i])
				{
					seques = j;
					xzName = vecWXAndTxZh[j][0];
					AssignmentToLocalVector_TxZh(xzName, vec_TxZhName[i], vec_TxZhData[i]);
				}
			}
		}
	}
}
void CPopEditorialClass::ImportingLocalData_Tab2()
{
	m_vecDqzBxName.clear();
	m_vecDqzTxName.clear();
	m_vecDqzkyName.clear();
	m_vecMapDqzBxData.clear();
	m_vecMapDqzTxData.clear();
	m_vecMapDqzKyData.clear();

	vector<string> vec_TxZhName;//地球站的子节点 --- 馈源波形天线名称
	vector<map<string, string>> vec_TxZhData;//卫星的子节点 --- 天线载荷数据

	vector<string> vec_WxName;
	vector<map<string, string>> vec_wxData;//遍历文件并将卫星数据存储下来

	vector<vector<string>> vecXZAndWX;//每一行都存放星座:卫星-卫星-。。。
	vector<vector<string>> vecWXAndTxZh;//每一行都存放 卫星:天线-载荷-。。。

	vector<wstring> upload_fileName;
	upload_fileName = UtilTool::GetOpenFilePaths_txt();
	/*********************先把电信港的筛选出来显示在界面上*********************/
	for (int i = 0; i < upload_fileName.size(); i++)
	{
		string fileN;
		UtilTool::setString(fileN, upload_fileName[i].c_str());

		vector<string> str_xzFileName;
		string fgf = "\\";
		str_xzFileName = my_split(fileN, fgf);
		string fileName = str_xzFileName[str_xzFileName.size() - 1];

		vector<string> str_xzName;
		fgf = ".";
		str_xzName = my_split(fileName, fgf);
		string Name = str_xzName[str_xzName.size() - 1];

		ifstream fin(fileN, ios::binary);
		unsigned char  s2;
		fin.read((char*)&s2, sizeof(s2));//读取第一个字节，然后左移8位
		int p = s2 << 8;
		fin.read((char*)&s2, sizeof(s2));//读取第二个字节
		p += s2;

		string code;
		fin.close();

		ifstream infile;
		infile.open(fileN);   //将文件流对象与文件连接起来 
		//assert(infile.is_open());   //若失败,则输出错误消息,并终止程序运行 
		if (infile)
		{
			string s;
			map<string, string> l_map;
			bool flag = true;
			string type;
			while (getline(infile, s))
			{
				if (flag)//如果是第一行 第一行写着
				{
					cout << "第一行" << endl;
					type = s;

					flag = false;
				}
				else
				{
					cout << "第二行" << endl;
					cout << s.c_str() << endl;
					vector<string> str_;
					string pattern = ":";
					str_ = my_split(s, pattern);
					//if (p == 58760 || p == 59032)//文件是UTF-8类型
					//{
					//	l_map[str_[0]] = str_[1];
					//}
					if (p == 46260)//文件是其他类型
					{

						l_map[string_To_UTF8(str_[0])] = string_To_UTF8(str_[1]);
					}
					else
					{
						l_map[str_[0]] = str_[1];
					}
				}


			}
			if (type == "电信港")
			{
				cout << "电信港" << endl;
				AssignmentToLocalVector_DXG(str_xzName[0], l_map);//将星座相关信息显示在界面上
			}


			infile.close();             //关闭文件输入流 
		}

	}
	/*********************把电信港配置文件筛选出来**********************************/
	for (int i = 0; i < upload_fileName.size(); i++)
	{
		//vector<vector<string>> vecVec_XzWx;
		string fileN;
		UtilTool::setString(fileN, upload_fileName[i].c_str());

		vector<string> str_xzFileName;
		string fgf = "\\";
		str_xzFileName = my_split(fileN, fgf);
		string fileName = str_xzFileName[str_xzFileName.size() - 1];

		vector<string> str_xzName;
		fgf = ".";
		str_xzName = my_split(fileName, fgf);
		string Name = str_xzName[0];
		cout << Name << endl;
		if (str_xzName[0] == UTF8_To_string("配置文件_电信港"))
		{
			ifstream fin(fileN, ios::binary);
			unsigned char  s2;
			fin.read((char*)&s2, sizeof(s2));//读取第一个字节，然后左移8位
			int p = s2 << 8;
			fin.read((char*)&s2, sizeof(s2));//读取第二个字节
			p += s2;

			string code;
			fin.close();

			ifstream infile;
			infile.open(fileN);   //将文件流对象与文件连接起来 
			//assert(infile.is_open());   //若失败,则输出错误消息,并终止程序运行 
			if (infile)
			{
				string s;
				map<string, string> l_map;

				while (getline(infile, s))
				{
					vector<string> line;
					cout << "第二行" << endl;
					cout << s.c_str() << endl;
					vector<string> str_;
					string pattern = ":";
					str_ = my_split(s, pattern);
					if (p == 58760 || p == 59032)//文件是UTF-8类型
					{
						line.push_back(str_[0]);

						vector<string> str_wx;
						pattern = "-";
						str_wx = my_split(str_[1], pattern);
						for (int i = 0; i < str_wx.size(); i++)
						{
							line.push_back(str_wx[i]);
						}
						//l_map[str_[0]] = str_[1];
					}
					else if (p == 46260 || p == 21364)//文件是其他类型
					{
						line.push_back(str_[0]);

						vector<string> str_wx;
						pattern = "-";
						str_wx = my_split(str_[1], pattern);
						for (int i = 0; i < str_wx.size(); i++)
						{
							line.push_back(string_To_UTF8(str_wx[i]));
						}
						//l_map[string_To_UTF8(str_[0])] = string_To_UTF8(str_[1]);
					}
					else
					{
						line.push_back(str_[0]);

						vector<string> str_wx;
						pattern = "-";
						str_wx = my_split(str_[1], pattern);
						for (int i = 0; i < str_wx.size(); i++)
						{
							line.push_back(str_wx[i]);
						}
					}
					vecXZAndWX.push_back(line);
				}


			}

			cout << "11111" << endl;
		}
	}
	///*********************把地球站的数据筛选出来*************************************/
	for (int i = 0; i < upload_fileName.size(); i++)
	{
		string fileN;
		UtilTool::setString(fileN, upload_fileName[i].c_str());

		vector<string> str_xzFileName;
		string fgf = "\\";
		str_xzFileName = my_split(fileN, fgf);
		string fileName = str_xzFileName[str_xzFileName.size() - 1];

		vector<string> str_xzName;
		fgf = ".";
		str_xzName = my_split(fileName, fgf);
		string Name = str_xzName[str_xzName.size() - 1];

		ifstream fin(fileN, ios::binary);
		unsigned char  s2;
		fin.read((char*)&s2, sizeof(s2));//读取第一个字节，然后左移8位
		int p = s2 << 8;
		fin.read((char*)&s2, sizeof(s2));//读取第二个字节
		p += s2;

		string code;
		fin.close();

		ifstream infile;
		infile.open(fileN);   //将文件流对象与文件连接起来 
		//assert(infile.is_open());   //若失败,则输出错误消息,并终止程序运行 
		if (infile)
		{
			string s;
			map<string, string> l_map;
			bool flag = true;
			string type;
			while (getline(infile, s))
			{
				if (flag)//如果是第一行 第一行写着
				{
					cout << "第一行" << endl;
					type = s;

					flag = false;
				}
				else
				{
					cout << "第二行" << endl;
					cout << s.c_str() << endl;
					vector<string> str_;
					string pattern = ":";
					str_ = my_split(s, pattern);
					if (p == 58760 || p == 59032 || p == 58765)//文件是UTF-8类型
					{
						l_map[str_[0]] = str_[1];
					}
					else if (p == 46260)//文件是其他类型
					{

						l_map[string_To_UTF8(str_[0])] = string_To_UTF8(str_[1]);
					}
					else
					{
						l_map[str_[0]] = str_[1];
					}
				}


			}
			if (type == "地球站")
			{
				cout << "地球站" << endl;
				vec_WxName.push_back(str_xzName[0]);
				vec_wxData.push_back(l_map);
				//AssignmentToLocalVector_XZ(str_xzName[0], l_map);
			}


			infile.close();             //关闭文件输入流 
		}
	}
	/*********************把天线和载荷的数据筛选出来******************************/
	for (int i = 0; i < upload_fileName.size(); i++)
	{
		string fileN;
		UtilTool::setString(fileN, upload_fileName[i].c_str());

		vector<string> str_xzFileName;
		string fgf = "\\";
		str_xzFileName = my_split(fileN, fgf);
		string fileName = str_xzFileName[str_xzFileName.size() - 1];

		vector<string> str_xzName;
		fgf = ".";
		str_xzName = my_split(fileName, fgf);
		string Name = str_xzName[str_xzName.size() - 1];

		ifstream fin(fileN, ios::binary);
		unsigned char  s2;
		fin.read((char*)&s2, sizeof(s2));//读取第一个字节，然后左移8位
		int p = s2 << 8;
		fin.read((char*)&s2, sizeof(s2));//读取第二个字节
		p += s2;

		string code;
		fin.close();

		ifstream infile;
		infile.open(fileN);   //将文件流对象与文件连接起来 
		//assert(infile.is_open());   //若失败,则输出错误消息,并终止程序运行 
		if (infile)
		{
			string s;
			map<string, string> l_map;
			bool flag = true;
			string type;
			while (getline(infile, s))
			{
				if (flag)//如果是第一行 第一行写着
				{
					cout << "第一行" << endl;
					type = s;

					flag = false;
				}
				else
				{
					cout << "第二行" << endl;
					cout << s.c_str() << endl;
					vector<string> str_;
					string pattern = ":";
					str_ = my_split(s, pattern);
					if (p == 58760 || p == 59032 || p == 58765)//文件是UTF-8类型
					{
						l_map[str_[0]] = str_[1];
					}
					else if (p == 46260)//文件是其他类型
					{

						l_map[string_To_UTF8(str_[0])] = string_To_UTF8(str_[1]);
					}
					else
					{
						l_map[str_[0]] = str_[1];
					}
				}


			}
			if (type == "馈源波形天线")
			{
				cout << "馈源波形天线" << endl;
				vec_TxZhName.push_back(str_xzName[0]);
				vec_TxZhData.push_back(l_map);
				//AssignmentToLocalVector_XZ(str_xzName[0], l_map);
			}

			if (type == "地球站馈源")
			{
				vec_TxZhName.push_back(str_xzName[0]);
				vec_TxZhData.push_back(l_map);

				m_vecDqzkyName.push_back(str_xzName[0]);
				m_vecMapDqzKyData.push_back(l_map);
			}
			else if (type == "地球站波形")
			{
				vec_TxZhName.push_back(str_xzName[0]);
				vec_TxZhData.push_back(l_map);

				m_vecDqzBxName.push_back(str_xzName[0]);
				m_vecMapDqzBxData.push_back(l_map);
			}
			else if (type == "地球站天线")
			{
				vec_TxZhName.push_back(str_xzName[0]);
				vec_TxZhData.push_back(l_map);

				m_vecDqzTxName.push_back(str_xzName[0]);
				m_vecMapDqzTxData.push_back(l_map);
			}

			infile.close();             //关闭文件输入流 
		}
	}
	/*********************把地面站配置文件筛选出来**********************************/
	for (int i = 0; i < upload_fileName.size(); i++)
	{
		//vector<vector<string>> vecVec_XzWx;
		string fileN;
		UtilTool::setString(fileN, upload_fileName[i].c_str());

		vector<string> str_xzFileName;
		string fgf = "\\";
		str_xzFileName = my_split(fileN, fgf);
		string fileName = str_xzFileName[str_xzFileName.size() - 1];

		vector<string> str_xzName;
		fgf = ".";
		str_xzName = my_split(fileName, fgf);
		string Name = str_xzName[0];
		cout << Name << endl;
		if (str_xzName[0] == UTF8_To_string("配置文件_地球站"))
		{
			ifstream fin(fileN, ios::binary);
			unsigned char  s2;
			fin.read((char*)&s2, sizeof(s2));//读取第一个字节，然后左移8位
			int p = s2 << 8;
			fin.read((char*)&s2, sizeof(s2));//读取第二个字节
			p += s2;

			string code;
			fin.close();

			ifstream infile;
			infile.open(fileN);   //将文件流对象与文件连接起来 
			//assert(infile.is_open());   //若失败,则输出错误消息,并终止程序运行 
			if (infile)
			{
				string s;
				map<string, string> l_map;

				while (getline(infile, s))
				{
					vector<string> line;
					cout << "第二行" << endl;
					cout << s.c_str() << endl;
					vector<string> str_;
					string pattern = ":";
					str_ = my_split(s, pattern);
					if (p == 58760 || p == 59032)//文件是UTF-8类型
					{
						line.push_back(str_[0]);

						vector<string> str_wx;
						pattern = "-";
						str_wx = my_split(str_[1], pattern);
						for (int i = 0; i < str_wx.size(); i++)
						{
							line.push_back(str_wx[i]);
						}
						//l_map[str_[0]] = str_[1];
					}
					else if (p == 46260 || p == 21364)//文件是其他类型
					{
						line.push_back(str_[0]);

						vector<string> str_wx;
						pattern = "-";
						str_wx = my_split(str_[1], pattern);
						for (int i = 0; i < str_wx.size(); i++)
						{
							line.push_back(string_To_UTF8(str_wx[i]));
						}
						//l_map[string_To_UTF8(str_[0])] = string_To_UTF8(str_[1]);
					}
					else
					{
						line.push_back(str_[0]);

						vector<string> str_wx;
						pattern = "-";
						str_wx = my_split(str_[1], pattern);
						for (int i = 0; i < str_wx.size(); i++)
						{
							line.push_back(str_wx[i]);
						}
					}
					vecWXAndTxZh.push_back(line);
				}


			}

			cout << "11111" << endl;
		}
	}
	///*********************创建电信港、地球站在treeView种的一级节点下面*******************/
	for (int i = 0; i < vec_WxName.size(); i++)
	{
		int seques = 0;
		string xzName;//星座名称
		for (int j = 0; j < vecXZAndWX.size(); j++)
		{
			for (int m = 0; m < vecXZAndWX[j].size(); m++)
			{
				if (UTF8_To_string(vecXZAndWX[j][m]) == vec_WxName[i])
				{
					seques = j;
					xzName = vecXZAndWX[j][0];
					AssignmentToLocalVector_DQZ(xzName, vec_WxName[i], vec_wxData[i]);
				}
				else if (vecXZAndWX[j][m] == vec_WxName[i])
				{
					seques = j;
					xzName = vecXZAndWX[j][0];
					AssignmentToLocalVector_DQZ(xzName, vec_WxName[i], vec_wxData[i]);
				}
			}
		}


	}
	/***********************创建地球站的馈源波形天线在treeview*************************************/
	for (int i = 0; i < vec_TxZhName.size(); i++)
	{
		int seques = 0;
		string xzName;//星座名称
		for (int j = 0; j < vecWXAndTxZh.size(); j++)
		{
			for (int m = 0; m < vecWXAndTxZh[j].size(); m++)
			{
				if (UTF8_To_string(vecWXAndTxZh[j][m]) == vec_TxZhName[i])
				{
					seques = j;
					xzName = vecWXAndTxZh[j][0];
					AssignmentToLocalVector_DQZ_level3(xzName, vec_TxZhName[i], vec_TxZhData[i]);
				}
				if (vecWXAndTxZh[j][m] == vec_TxZhName[i])
				{
					seques = j;
					xzName = vecWXAndTxZh[j][0];
					AssignmentToLocalVector_DQZ_level3(xzName, vec_TxZhName[i], vec_TxZhData[i]);
				}
			}
		}
	}
}
void CPopEditorialClass::ImportingLocalData_Tab3()
{
	m_vecDmzdBxName.clear();
	m_vecDmzdZhName.clear();
	m_vecDmzdTxName.clear();
	m_vecMapDmzdBxData.clear();
	m_vecMapDmzdTxData.clear();
	m_vecMapDmzdZhData.clear();

	vector<string> vec_WxName;
	vector<map<string, string>> vec_wxData;//遍历文件并将卫星数据存储下来
	vector<vector<string>> vecXZAndWX;//每一行都存放星座:卫星-卫星-。。。
	vector<wstring> upload_fileName;
	upload_fileName = UtilTool::GetOpenFilePaths_txt();
	/*********************先把地面终端的筛选出来显示在界面上*********************/
	for (int i = 0; i < upload_fileName.size(); i++)
	{
		string fileN;
		UtilTool::setString(fileN, upload_fileName[i].c_str());

		vector<string> str_xzFileName;
		string fgf = "\\";
		str_xzFileName = my_split(fileN, fgf);
		string fileName = str_xzFileName[str_xzFileName.size() - 1];

		vector<string> str_xzName;
		fgf = ".";
		str_xzName = my_split(fileName, fgf);
		string Name = str_xzName[str_xzName.size() - 1];

		ifstream fin(fileN, ios::binary);
		unsigned char  s2;
		fin.read((char*)&s2, sizeof(s2));//读取第一个字节，然后左移8位
		int p = s2 << 8;
		fin.read((char*)&s2, sizeof(s2));//读取第二个字节
		p += s2;

		string code;
		fin.close();

		ifstream infile;
		infile.open(fileN);   //将文件流对象与文件连接起来 
		//assert(infile.is_open());   //若失败,则输出错误消息,并终止程序运行 
		if (infile)
		{
			string s;
			map<string, string> l_map;
			bool flag = true;
			string type;
			while (getline(infile, s))
			{
				if (flag)//如果是第一行 第一行写着
				{
					cout << "第一行" << endl;
					type = s;

					flag = false;
				}
				else
				{
					cout << "第二行" << endl;
					cout << s.c_str() << endl;
					vector<string> str_;
					string pattern = ":";
					str_ = my_split(s, pattern);
					if (p == 58760 || p == 59032)//文件是UTF-8类型
					{
						l_map[str_[0]] = str_[1];
					}
					else if (p == 46260)//文件是其他类型
					{

						l_map[string_To_UTF8(str_[0])] = string_To_UTF8(str_[1]);
					}
					else
					{
						l_map[str_[0]] = str_[1];
					}
				}


			}
			if (type == "地面终端")
			{
				cout << "地面终端" << endl;
				AssignmentToLocalVector_DMZD(str_xzName[0], l_map);//将星座相关信息显示在界面上
			}

			infile.close();             //关闭文件输入流 
		}
		//m_FtpClientClass.execute_putFile(fileN);//put 上传文件
	}
	/*********************把配置文件筛选出来**********************************/
	for (int i = 0; i < upload_fileName.size(); i++)
	{
		//vector<vector<string>> vecVec_XzWx;
		string fileN;
		UtilTool::setString(fileN, upload_fileName[i].c_str());

		vector<string> str_xzFileName;
		string fgf = "\\";
		str_xzFileName = my_split(fileN, fgf);
		string fileName = str_xzFileName[str_xzFileName.size() - 1];

		vector<string> str_xzName;
		fgf = ".";
		str_xzName = my_split(fileName, fgf);
		string Name = str_xzName[0];
		cout << Name << endl;
		if (str_xzName[0] == UTF8_To_string("配置文件_地面终端"))
		{
			ifstream fin(fileN, ios::binary);
			unsigned char  s2;
			fin.read((char*)&s2, sizeof(s2));//读取第一个字节，然后左移8位
			int p = s2 << 8;
			fin.read((char*)&s2, sizeof(s2));//读取第二个字节
			p += s2;

			string code;
			fin.close();

			ifstream infile;
			infile.open(fileN);   //将文件流对象与文件连接起来 
			//assert(infile.is_open());   //若失败,则输出错误消息,并终止程序运行 
			if (infile)
			{
				string s;
				map<string, string> l_map;

				while (getline(infile, s))
				{
					vector<string> line;
					cout << "第二行" << endl;
					cout << s.c_str() << endl;
					vector<string> str_;
					string pattern = ":";
					str_ = my_split(s, pattern);
					if (p == 58760 || p == 59032)//文件是UTF-8类型
					{
						line.push_back(str_[0]);

						vector<string> str_wx;
						pattern = "-";
						str_wx = my_split(str_[1], pattern);
						for (int i = 0; i < str_wx.size(); i++)
						{
							line.push_back(str_wx[i]);
						}
						//l_map[str_[0]] = str_[1];
					}
					else if (p == 46260 || p == 21364)//文件是其他类型
					{
						line.push_back(str_[0]);

						vector<string> str_wx;
						pattern = "-";
						str_wx = my_split(str_[1], pattern);
						for (int i = 0; i < str_wx.size(); i++)
						{
							line.push_back(string_To_UTF8(str_wx[i]));
						}
						//l_map[string_To_UTF8(str_[0])] = string_To_UTF8(str_[1]);
					}
					else
					{
						line.push_back(str_[0]);

						vector<string> str_wx;
						pattern = "-";
						str_wx = my_split(str_[1], pattern);
						for (int i = 0; i < str_wx.size(); i++)
						{
							line.push_back(str_wx[i]);
						}
					}
					vecXZAndWX.push_back(line);
				}


			}

			cout << "11111" << endl;
		}
	}
	/*********************把地面终端子节点的载荷、波形、天线数据筛选出来*************************************/
	for (int i = 0; i < upload_fileName.size(); i++)
	{
		string fileN;
		UtilTool::setString(fileN, upload_fileName[i].c_str());

		vector<string> str_xzFileName;
		string fgf = "\\";
		str_xzFileName = my_split(fileN, fgf);
		string fileName = str_xzFileName[str_xzFileName.size() - 1];

		vector<string> str_xzName;
		fgf = ".";
		str_xzName = my_split(fileName, fgf);
		string Name = str_xzName[str_xzName.size() - 1];

		ifstream fin(fileN, ios::binary);
		unsigned char  s2;
		fin.read((char*)&s2, sizeof(s2));//读取第一个字节，然后左移8位
		int p = s2 << 8;
		fin.read((char*)&s2, sizeof(s2));//读取第二个字节
		p += s2;

		string code;
		fin.close();

		ifstream infile;
		infile.open(fileN);   //将文件流对象与文件连接起来 
		//assert(infile.is_open());   //若失败,则输出错误消息,并终止程序运行 
		if (infile)
		{
			string s;
			map<string, string> l_map;
			bool flag = true;
			string type;
			while (getline(infile, s))
			{
				if (flag)//如果是第一行 第一行写着
				{
					cout << "第一行" << endl;
					type = s;

					flag = false;
				}
				else
				{
					cout << "第二行" << endl;
					cout << s.c_str() << endl;
					vector<string> str_;
					string pattern = ":";
					str_ = my_split(s, pattern);
					if (p == 58760 || p == 59032 || p == 58765)//文件是UTF-8类型
					{
						l_map[str_[0]] = str_[1];
					}
					else if (p == 46260)//文件是其他类型
					{

						l_map[string_To_UTF8(str_[0])] = string_To_UTF8(str_[1]);
					}
					else
					{
						l_map[str_[0]] = str_[1];
					}
				}


			}
			if (type == "载荷波形天线")
			{
				cout << "载荷波形天线" << endl;
				vec_WxName.push_back(str_xzName[0]);
				vec_wxData.push_back(l_map);
				//AssignmentToLocalVector_XZ(str_xzName[0], l_map);
			}
			if (type == "地面终端载荷")
			{
				vec_WxName.push_back(str_xzName[0]);
				vec_wxData.push_back(l_map);
				
				m_vecDmzdZhName.push_back(str_xzName[0]);
				m_vecMapDmzdZhData.push_back(l_map);
			}
			else if (type == "地面终端波形")
			{
				vec_WxName.push_back(str_xzName[0]);
				vec_wxData.push_back(l_map);

				m_vecDmzdBxName.push_back(str_xzName[0]);
				m_vecMapDmzdBxData.push_back(l_map);
			}
			else if (type == "地面终端天线")
			{
				vec_WxName.push_back(str_xzName[0]);
				vec_wxData.push_back(l_map);

				m_vecDmzdTxName.push_back(str_xzName[0]);
				m_vecMapDmzdTxData.push_back(l_map);
			}


			infile.close();             //关闭文件输入流 
		}
	}
	/*********************创建卫星节点在treeView种的一级节点下面*******************/
	for (int i = 0; i < vec_WxName.size(); i++)
	{
		int seques = 0;
		string xzName;//星座名称
		for (int j = 0; j < vecXZAndWX.size(); j++)
		{
			for (int m = 0; m < vecXZAndWX[j].size(); m++)
			{
				if (UTF8_To_string(vecXZAndWX[j][m]) == vec_WxName[i])
				{
					seques = j;
					xzName = vecXZAndWX[j][0];
					AssignmentToLocalVector_DMZD_Child(xzName, vec_WxName[i], vec_wxData[i]);
				}

				if (vecXZAndWX[j][m] == vec_WxName[i])
				{
					seques = j;
					xzName = vecXZAndWX[j][0];
					AssignmentToLocalVector_DMZD_Child(xzName, vec_WxName[i], vec_wxData[i]);
				}
			}
		}
		//xzName


	}


}
//刷新界面 将数据置0
void CPopEditorialClass::RefreshTheInterface_tab1()
{
	EmptyChildNode_tab1();
	//CreateTreeNode_livel2(nodeName, sequence);
	name_Level2.clear();
	m_RecvMap.clear();
}
void CPopEditorialClass::RefreshTheInterface_tab2()
{
	EmptyChildNode_tab2();
	//CreateTreeNode_livel2(nodeName, sequence);
	name2_Level2.clear();
	m_RecvMap_DQZ.clear();
}
void CPopEditorialClass::RefreshTheInterface_tab3()
{
	EmptyChildNode_tab3();
	name3_Level1.clear();
	m_RecvMap_DMZD.clear();
}