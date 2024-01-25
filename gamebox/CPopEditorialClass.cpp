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

int rootNum;//��һ�����ڵ����

int nodeNum;//�����ڵ�
//FILE * lOG;
#define ControlNumber 102  //�ؼ���ֹ�� titles54 
bool boolArray[1024] = {0};//�����Ƿ������������
//vector<string> seleceColumns;
//�������������ʵ��
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

	str += pattern;//��չ�ַ����Է������
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

	wchar_t * pwBuf = new wchar_t[nwLen + 1];//һ��Ҫ��1����Ȼ�����β��  
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

	wchar_t * pwBuf = new wchar_t[nwLen + 1];//һ��Ҫ��1����Ȼ�����β��
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
	
//stringתwstring
}
//wstringתstring
string CPopEditorialClass::wstringToString(wstring wstr)
{
	string str;
	UtilTool::setString(str, wstr.c_str());
	return str;
}
void CPopEditorialClass::creatTreeNodesOfXZ()
{
	//���������ڵ�;
	for (const auto& stData : EnvironmentData::m_mapStXZSJB)
	{
		auto &st = stData.second;
		// �����ڵ�����ӽڵ�
		//CTreeNodeUI* pChildNode = new CTreeNodeUI();
		//UtilTool::setWstring(text_string, st.constellationName.c_str());
		//pChildNode->SetItemText(text_string.c_str());
		//CDuiString strID;
		//strID.Format(_T("%d"),m_iTreeNodeID); // ��intת��Ϊ�ַ���
		//pChildNode->SetUserData(strID);
		//setTreeNodeStyleSheet(pChildNode);
		//pNode_1[0]->AddChildNode(pChildNode);
		//pNode_1[0]->add

		auto newTreeNode = addChildNode(pNode_1[0], st.constellationName);
		
		// �����ӽڵ�
		createTreeNodesOfDX(newTreeNode, st.id);
	}
}

// ����������Ӧ�ĵ����ӽڵ�
void CPopEditorialClass::createTreeNodesOfDX(CTreeNodeUI* pTreeNodeXZ, const int& sqlID)
{
	// ��ȡ ��Ӧ�����µĵ�������;
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
	strID.Format(_T("%d"), m_iTreeNodeID); // ��intת��Ϊ�ַ���
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
	pTreeNode->SetAttribute(_T("folderattr"), _T("padding=&quot;0,1,0,0&quot; width=&quot;16&quot; height=&quot;16&quot; normalimage=&quot;file='ui/չ��.png' &quot; "));
	pTreeNode->SetAttribute(_T("folderattr"), _T("hotimage=&quot;file='ui/չ��.png' &quot; selectedimage=&quot;file='ui/δչ��.png'&quot; selectedhotimage=&quot;file='ui/δչ��.png'&quot;"));
	pTreeNode->SetAttribute(_T("itemattr"), _T("padding=\"16,0,0,0\" align=\"left\" font=\"13\""));//font=&quot;6&quot;
	pTreeNode->SetName(_T("WX1"));

}
void CPopEditorialClass::removeAllNodes(CTreeNodeUI* pNode)
{
	if (pNode == NULL) return;

	// ��ȡ�ӽڵ������
	int nCount = pNode->GetCount();
	for (int i = nCount - 1; i >= 0; --i)
	{
		// ��ȡ�ӽڵ�
		CControlUI* pSubItem = pNode->GetItemAt(i);
		CTreeNodeUI* pSubNode = static_cast<CTreeNodeUI*>(pSubItem);
		if (pSubNode != NULL)
		{
			// �ݹ�ɾ���ӽڵ�
			removeAllNodes(pSubNode);
		}
	}

	// ���������ӽڵ㶼��ɾ���ˣ����ǿ��԰�ȫ��ɾ����ǰ�ڵ�
	CTreeNodeUI* pParentNode = pNode->GetParentNode();
	if (pParentNode != NULL)
	{
		pParentNode->Remove(pNode); // �Ӹ��ڵ��Ƴ���ǰ�ڵ�
	}
	else
	{
		// ������Ǹ��ڵ㣬��ֱ��ɾ��
		CTreeViewUI* pTreeView = pNode->GetTreeView();
		if (pTreeView != NULL)
		{
			pTreeView->Remove(pNode); // ��TreeView�Ƴ���ǰ�ڵ�
		}
	}
	delete pNode; // ɾ���ڵ����
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
	else if (sName.CompareNoCase(_T("bt_save")) == 0)////����Tab1 bt_save
	{

		map<string, string> map_inform;//�������޸����map
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
		// Ϊ�˴������ֶ��������߼�;
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
	else if (sName.CompareNoCase(_T("bt_save2")) == 0)////����tab2 bt_save
	{

		map<string, string> map_inform;//�������޸����map
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
		// Ϊ�˴������ֶ��������߼�;
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
	else if (sName.CompareNoCase(_T("bt_SaveScheme1")) == 0)  // ���浽���ݿ�;
	{
		
		m_CPopNewScheme->MessageBox(NULL);//
		string strScheme = m_CPopNewScheme->GetNewName();//��ȡ�����ӵķ�������;
		if (strScheme.size() > 0)
		{
			saveDatabaseScheme(strScheme);
		}	
	}
	else if (sName.CompareNoCase(_T("bt_SaveScheme2")) == 0)  // ���浽���ݿ�;
	{
		m_CPopNewScheme->MessageBox(NULL);//
		string strScheme = m_CPopNewScheme->GetNewName();//��ȡ�����ӵķ�������;
		if (strScheme.size() > 0)
		{
			saveDatabaseScheme(strScheme);
		}	
	}
	else if (sName.CompareNoCase(_T("bt_SaveScheme3")) == 0)  // ���浽���ݿ�;
	{
		m_CPopNewScheme->MessageBox(NULL);//
		string strScheme = m_CPopNewScheme->GetNewName();//��ȡ�����ӵķ�������;
		if (strScheme.size() > 0)
		{
			saveDatabaseScheme(strScheme);
		}
	}
	else if (sName.CompareNoCase(_T("bt_AddTo")) == 0)//���Ӱ�ť ���������ǡ������µ����ߺ��غ�����
	{
		m_CPopNewAttributeClass->MessageBox(NULL);//m_CPopNewAttributeClass

		vector<string> vec = m_CPopNewAttributeClass->GetNewName();//��ȡ�����ӵ�
		if (vec.size() > 0)
		{
			map<string, string> map_inform;//�������޸����map
			string FileName = wstringToString(m_modiftName.GetData());
			int sequese;
			for (int i = 0; i < name_Level1.size(); i++)//�������㼶�������
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


			for (int i = 0; i < name_Level2.size(); i++)//�����ǲ㼶�������
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

			// Ϊ�˴������ֶ��������߼�;
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
	else if (sName.CompareNoCase(_T("bt_Delete")) == 0)//ɾ����ť ���������ǡ������µ����ߺ��غ�����
	{
		vector<string> vec_name;
		int flag = -1;
		map<string, string> map_inform;//�������޸����map
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

		if (flag == 0)//����
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
						//cout << "ѡ��" << endl;
						map_inform1.erase(iter->first);
					}

				}
			}

			m_RecvMap_XZ[sequese] = map_inform1;
			DisplayInformation(m_RecvMap_XZ[sequese]);
		}
		else if (flag == 1)//����
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
						//cout << "ѡ��" << endl;
						map_inform1.erase(iter->first);
					}

				}
			}

			m_RecvMap[sequese] = map_inform1;
			DisplayInformation(m_RecvMap[sequese]);
		}
		else if (flag == 2)//���ǵ����ߺ��غ�
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
						//cout << "ѡ��" << endl;
						map_inform1.erase(iter->first);
					}

				}
			}

			m_RecvMap_TxZh[sequese] = map_inform1;
			DisplayInformation(m_RecvMap_TxZh[sequese]);

			
		}


		// �������ֶ���ӵ��߼�; // ɾ��;
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
							//cout << "ѡ��" << endl;
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
							//cout << "ѡ��" << endl;
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
	else if (sName.CompareNoCase(_T("bt_Tree1_Import")) == 0)//����
	{
		RefreshTheInterface_tab1();
		HideThCcontrol(ControlNumber);

		COptionUI* pControl = static_cast<COptionUI*>(m_pm.FindControl(_T("optical_ScenarioData")));//��������
		if (pControl->IsSelected())
		{
			cout << "tab1��������" << endl;
			//RefreshTheInterface_tab1();
		}
		COptionUI* pControl_1 = static_cast<COptionUI*>(m_pm.FindControl(_T("optical_LocalData")));//��������
		if (pControl_1->IsSelected())
		{

			ImportingLocalData_Tab1();//���뱾������
			// �������ݿ����� ���������Ƚڵ�;
			//creatTreeNodesOfXZ();
		}



	}
	else if (sName.CompareNoCase(_T("bt_Tree1_SaveAs")) == 0)//���Ϊ
	{
		string path;
		path = "D:\\DATA1\\";
		//bool flag = CreateDirectory(_T("D:\\DATA1\\HH"), NULL);
		m_CPopNewConstellation->MessageBox(NULL);
		string name = m_CPopNewConstellation->GetNewName();//	string GetNewName();//��ȡ�޸ĺ������
		path += name;
		bool flag = CreateDirectory(stringToWstring(path).c_str(), NULL);
		vector< vector<string>> vec_XZAndWX;
		vector< vector<string>> vec_WXAndChild;
		/*******************�����������ݴ洢*****************/
		for (int i = 0; i < name_Level1.size(); i++)
		{
			GenerateTheFile(path, name_Level1[i], m_RecvMap_XZ[i], "����");

		}
		/*******************�����ǵ����ݴ洢********************/
		for (int i = 0; i < name_Level2.size(); i++)
		{
			GenerateTheFile(path, name_Level2[i], m_RecvMap[i], "����");
		}
		/*******************�����ǵ����ߺ��غ����ݴ洢**********************************/
		for (int i = 0; i < name_Level3.size(); i++)
		{
			GenerateTheFile(path, name_Level3[i], m_RecvMap_TxZh[i], "���߻��غ�");
		}
		/********************��������Ӧ���ǹ�ϵ�洢********************/
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
		GenerateTheFile(path, UTF8_To_string("�����ļ�_����"), vec_XZAndWX);

		/**********************�����Ǻ����ӽڵ��ϵ�洢******************************/
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
		GenerateTheFile(path, UTF8_To_string("�����ļ�_����"), vec_WXAndChild);
	}
	else if (sName.CompareNoCase(_T("bt_AddTo2")) == 0)//���� ���Ÿ����� Tab2
	{
		m_CPopNewAttributeClass->MessageBox(NULL);//m_CPopNewAttributeClass

		vector<string> vec = m_CPopNewAttributeClass->GetNewName();//��ȡ�����ӵ�
		if (vec.size() > 0)
		{
			map<string, string> map_inform;//�������޸����map
			string FileName = wstringToString(m_modiftName_2.GetData());
			int sequese;
			for (int i = 0; i < name2_Level1.size(); i++)//�����Ÿ���������
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



			for (int i = 0; i < name2_Level2.size(); i++)//������վ�������
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
			// Ϊ�˴������ֶ��������߼�;
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
	else if (sName.CompareNoCase(_T("bt_Delete2")) == 0)//ɾ�����Ÿ����� tab2
	{
		vector<string> vec_name;
		int flag = -1;
		map<string, string> map_inform;//�������޸����map
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

		if (flag == 0)//���Ÿ�
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
						//cout << "ѡ��" << endl;
						map_inform1.erase(iter->first);
					}

				}
			}

			m_RecvMap_DXG[sequese] = map_inform1;
			DisplayInformation_2(m_RecvMap_DXG[sequese]);
		}
		else if (flag == 1)//����վ
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
						//cout << "ѡ��" << endl;
						map_inform1.erase(iter->first);
					}

				}
			}

			m_RecvMap_DQZ[sequese] = map_inform1;
			DisplayInformation_2(m_RecvMap_DQZ[sequese]);
		}
		else if (flag == 2)//�����ն�
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
						//cout << "ѡ��" << endl;
						map_inform1.erase(iter->first);
					}

				}
			}

			m_RecvMap_DQZChild[sequese] = map_inform1;
			DisplayInformation_2(m_RecvMap_DQZChild[sequese]);
		}

		// �������ֶ���ӵ��߼�; // ɾ��;
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
							//cout << "ѡ��" << endl;
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
							//cout << "ѡ��" << endl;
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
		// ����վ ����
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
							//cout << "ѡ��" << endl;
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
	else if (sName.CompareNoCase(_T("bt_Tree1_Import2")) == 0)//����
	{
		RefreshTheInterface_tab2();
		HideThCcontrol_2(ControlNumber);
		COptionUI* pControl = static_cast<COptionUI*>(m_pm.FindControl(_T("optical_ScenarioData")));//��������
		if (pControl->IsSelected())
		{
			cout << "tab2��������" << endl;

			
		}
		COptionUI* pControl_1 = static_cast<COptionUI*>(m_pm.FindControl(_T("optical_LocalData2")));//��������
		if (pControl_1->IsSelected())
		{

			ImportingLocalData_Tab2();//���뱾������
		}
		//ImportingLocalData_Tab2();


	}
	else if (sName.CompareNoCase(_T("bt_Tree1_SaveAs2")) == 0)//tab2 ���Ϊ
	{
		vector<vector<string>> vec_WXAndChild;
		string path;
		path = "D:\\DATA1\\";
		//bool flag = CreateDirectory(_T("D:\\DATA1\\HH"), NULL);
		m_CPopNewConstellation->MessageBox(NULL);
		string name = m_CPopNewConstellation->GetNewName();//	string GetNewName();//��ȡ�޸ĺ������
		path += name;
		bool flag = CreateDirectory(stringToWstring(path).c_str(), NULL);
		vector< vector<string>> vec_DxgAndDqz;
		/*******************�����Ÿ۵����ݴ洢*****************/
		for (int i = 0; i < name2_Level1.size(); i++)
		{
			GenerateTheFile(path, name2_Level1[i], m_RecvMap_DXG[i], "���Ÿ�");

		}
		/*******************������վ�����ݴ�********************/
		for (int i = 0; i < name2_Level2.size(); i++)
		{
			GenerateTheFile(path, name2_Level2[i], m_RecvMap_DQZ[i], "����վ");
		}
		/*******************������վ����Դ�����������ݴ洢**********************************/
		for (int i = 0; i < name2_Level3.size(); i++)
		{
			GenerateTheFile(path, name2_Level3[i], m_RecvMap_DQZChild[i], "��Դ��������");
		}
		/********************�����Ÿ۶�Ӧ����վ��ϵ�洢********************/
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
		GenerateTheFile(path, UTF8_To_string("�����ļ�_���Ÿ�"), vec_DxgAndDqz);

		/**********************������վ�����ӽڵ��ϵ�洢******************************/
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
		GenerateTheFile(path, UTF8_To_string("�����ļ�_����վ"), vec_WXAndChild);
	}
	else if (sName.CompareNoCase(_T("bt_Tree1_SaveAs3")) == 0)//tab2 ���Ϊ
	{
		string path;
		path = "D:\\DATA1\\";
		m_CPopNewConstellation->MessageBox(NULL);
		string name = m_CPopNewConstellation->GetNewName();//	string GetNewName();//��ȡ�޸ĺ������
		path += name;
		bool flag = CreateDirectory(stringToWstring(path).c_str(), NULL);
		vector< vector<string>> vec_XZAndWX;
		/*******************�����������ݴ洢*****************/
		for (int i = 0; i < name3_Level1.size(); i++)
		{
			GenerateTheFile(path, name3_Level1[i], m_RecvMap_DMZD[i], "�����ն�");

		}
		/*******************�����ǵ����ݴ�********************/
		for (int i = 0; i < name3_Level2.size(); i++)
		{
			GenerateTheFile(path, name3_Level2[i], m_RecvMap_DMZDChild[i], "�غɲ�������");
		}
		/********************��������Ӧ���ǹ�ϵ�洢********************/
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
		GenerateTheFile(path, UTF8_To_string("�����ļ�_�����ն�"), vec_XZAndWX);


	}
	else if (sName.CompareNoCase(_T("bt_Tree1_Import3")) == 0)//����
	{
		RefreshTheInterface_tab3();
		HideThCcontrol_3(ControlNumber);
		COptionUI* pControl = static_cast<COptionUI*>(m_pm.FindControl(_T("optical_ScenarioData3")));//��������
		if (pControl->IsSelected())
		{
			cout << "tab3��������" << endl;
		}
		COptionUI* pControl_1 = static_cast<COptionUI*>(m_pm.FindControl(_T("optical_LocalData3")));//��������
		if (pControl_1->IsSelected())
		{

			ImportingLocalData_Tab3();//���뱾������
		}
	}
	else if (sName.CompareNoCase(_T("bt_save3")) == 0)////����tab2 bt_save
	{

		map<string, string> map_inform;//�������޸����map
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
		// Ϊ�˴������ֶ��������߼�;
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
	else if (sName.CompareNoCase(_T("bt_AddTo3")) == 0)//���� ���Ÿ����� Tab2
	{
		m_CPopNewAttributeClass->MessageBox(NULL);//m_CPopNewAttributeClass

		vector<string> vec = m_CPopNewAttributeClass->GetNewName();//��ȡ�����ӵ�
		if (vec.size() > 0)
		{
			map<string, string> map_inform;//�������޸����map
			string FileName = wstringToString(m_modiftName_3.GetData());
			int sequese = -1;
			for (int i = 0; i < name3_Level1.size(); i++)//�����Ÿ���������
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



			for (int i = 0; i < name3_Level2.size(); i++)//������վ�������
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

			// Ϊ�˴������ֶ��������߼�;
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
	else if (sName.CompareNoCase(_T("bt_Delete3")) == 0)//ɾ�����Ÿ����� tab2
	{
		vector<string> vec_name;
		int flag = -1;
		map<string, string> map_inform;//�������޸����map
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

		if (flag == 0)//���Ÿ�
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
						//cout << "ѡ��" << endl;
						map_inform1.erase(iter->first);
					}

				}
			}

			m_RecvMap_DMZD[sequese] = map_inform1;
			DisplayInformation_3(m_RecvMap_DMZD[sequese]);
		}
		else if (flag == 1)//����վ
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
						//cout << "ѡ��" << endl;
						map_inform1.erase(iter->first);
					}

				}
			}

			m_RecvMap_DMZDChild[sequese] = map_inform1;
			DisplayInformation_3(m_RecvMap_DMZDChild[sequese]);
		}

		// �������ֶ���ӵ��߼�; // ɾ��;
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
							//cout << "ѡ��" << endl;
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
				string name = m_CpopModifyInterface->GetNewName();//	string GetNewName();//��ȡ�޸ĺ������
				newName = name;
				if (name != "")
				{
					ModifyNodeName();
					cout << name << endl;
				}

				UncheckedTab1();

			}
			else if (sMenuName.CompareNoCase(_T("NewConstellation")) == 0)//�½�����
			{

				m_CPopNewConstellation->MessageBox(NULL);

				string name = m_CPopNewConstellation->GetNewName();//	string GetNewName();//��ȡ�޸ĺ������
				newHoroscope = name;

				if (name != "")
				{
					AssignmentToLocalVector_XZ(newHoroscope);//�������Լ�Ŀ¼���������
				}



				CTreeViewUI* pTreeView = static_cast<CTreeViewUI*>(m_pm.FindControl(_T("treeview1")));
				pTreeView->SetItemExpand(false, NULL);//�����νṹ��ʼ״̬Ϊ��չ����
				UncheckedTab1();

			}
			else if (sMenuName.CompareNoCase(_T("RailType")) == 0)//����---������ 
			{
				if (bChecked)
				{
					if (FunctionalCategoryFlag)//�ָ��������ķ���
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
					pTreeView->SetItemExpand(false, NULL);//�����νṹ��ʼ״̬Ϊ��չ����
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
			else if (sMenuName.CompareNoCase(_T("FunctionalCategory")) == 0)//�������
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
					FunctionalCategoryFlag = true;//���ܷ��� FunctionalCategory
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
					pTreeView->SetItemExpand(false, NULL);//�����νṹ��ʼ״̬Ϊ��չ����
					HideThCcontrol(ControlNumber);
					pNode_1[0]->Select(false);
					UncheckedTab1();
				}
			}
			else if (sMenuName.CompareNoCase(_T("Payload")) == 0)//------�غ����ͷ���
			{
				if (bChecked)
				{
					if (RailTypeFlag)
					{
						Restore_XZ_RailType();
						RailTypeFlag = false;
					}
					if (FunctionalCategoryFlag)//�ָ��������ķ���
					{
						Restore_XZ_Functional();
						FunctionalCategoryFlag = false;
					}

					PayloadFlag = true;//�غ����ͷ���
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
					pTreeView->SetItemExpand(false, NULL);//�����νṹ��ʼ״̬Ϊ��չ����
					HideThCcontrol(ControlNumber);
					pNode_1[0]->Select(false);
					UncheckedTab1();


				}
			}
			//������� FunctionalCategory
			if (sMenuName.CompareNoCase(_T("modify1")) == 0)
			{
				cout << "111111" << endl;

				m_CpopModifyInterface->MessageBox(NULL, m_modiftName);

				string name = m_CpopModifyInterface->GetNewName();//	string GetNewName();//��ȡ�޸ĺ������
				newName = name;


				if (name != "")
				{
					ModifyNodeName();
					cout << name << endl;
				}
				cout << name << endl;
				UncheckedTab1();

			}
			else if (sMenuName.CompareNoCase(_T("NewSatellite")) == 0)//�½�����
			{
				cout << "111111" << endl;
				m_CPopNewConstellation->MessageBox(NULL);

				string name = m_CPopNewConstellation->GetNewName();//	string GetNewName();//��ȡ�޸ĺ������
				newsatellite = name;

				if (name != "")
				{
					map<string, string> map0;
					map0 = GetSatelliteInform();
					int num = selectNode[0][1];
					AssignmentToLocalVector(newsatellite, map0, selectNode[0][1]);//�������½������ڵ���

					map<string, string> map1;
					map1 = GetSatelliteInform_1();//��ȡ����������Ϣ
					AssignmentToLocalVector_WXSX(newsatellite, UTF8_To_string("����������Ϣ"), map1);
					map<string, string> map2;
					map2 = GetSatelliteInform_2();//��ȡ�غ�������Ϣ
					AssignmentToLocalVector_WXSX(newsatellite, UTF8_To_string("�غ�������Ϣ"), map2);


					
				}


				CTreeViewUI* pTreeView = static_cast<CTreeViewUI*>(m_pm.FindControl(_T("treeview1")));
				pTreeView->SetItemExpand(false, NULL);//�����νṹ��ʼ״̬Ϊ��չ����
				UncheckedTab1();

			}
			else if (sMenuName.CompareNoCase(_T("SatellitePayload")) == 0)//�����غ� ɸѡ
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
				//	/*******************���԰�ÿ��������չ�����ҹرյĵ���û�жԹ���ʾ��ǰ�ǰ���ʲô�����********************/
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

			//	string name = m_CpopModifyInterface->GetNewName();//	string GetNewName();//��ȡ�޸ĺ������
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
			if (sMenuName.CompareNoCase(_T("MoveTo")) == 0)//�ƶ���
			{

				m_CpopSelectLevel1->MessageBox(NULL, name_Level1);

				string name = m_CpopSelectLevel1->GetNodeName();//	string GetNewName();//��ȡ�޸ĺ������

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

				string name = m_CpopModifyInterface->GetNewName();//	string GetNewName();//��ȡ�޸ĺ������
				newName_2 = name;
				
				if (name != "")
				{
					ModifyNodeName_2();
					cout << name << endl;
				}


				UncheckedTab2();

			}//m_CpopSelectLevel1 
			else if (sMenuName.CompareNoCase(_T("NewTeleport")) == 0)//�½����Ÿ�
			{
				m_CPopNewConstellation->MessageBox(NULL);

				string name = m_CPopNewConstellation->GetNewName();//	string GetNewName();//��ȡ�޸ĺ������
				newTeleport = name;

				if (name != "")
				{
					AssignmentToLocalVector_DXG(newTeleport);

				}
				//map<string, string> map;
				//map = GetTeleport();


				//CTreeViewUI* pTreeView = static_cast<CTreeViewUI*>(m_pm.FindControl(_T("treeview2")));
				m_treeView_2->SetItemExpand(false, NULL);//�����νṹ��ʼ״̬Ϊ��չ����
				UncheckedTab2();

			}//
			else if (sMenuName.CompareNoCase(_T("Characteristic")) == 0)//���Ÿ۰����ʷ���
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
					m_treeView_2->SetItemExpand(false, NULL);//�����νṹ��ʼ״̬Ϊ��չ����
					HideThCcontrol_2(ControlNumber);
					pNode_2_1[0]->Select(false);
					UncheckedTab2();
				}
			}

			//���Ÿ��޸�
			if (sMenuName.CompareNoCase(_T("modify2_1")) == 0)
			{
				m_CpopModifyInterface->MessageBox(NULL, m_modiftName_2);

				string name = m_CpopModifyInterface->GetNewName();//	string GetNewName();//��ȡ�޸ĺ������
				newName_2 = name;
				
				if (name != "")
				{
					ModifyNodeName_2();
					cout << name << endl;
				}


				cout << name << endl;
				UncheckedTab2();

			}//m_CpopSelectLevel1 
			else if (sMenuName.CompareNoCase(_T("NewEarthStation")) == 0)//�½�����վ
			{
				cout << "111111" << endl;
				m_CPopNewConstellation->MessageBox(NULL);

				string name = m_CPopNewConstellation->GetNewName();//	string GetNewName();//��ȡ�޸ĺ������
				newEarthStation = name;

				if (name != "")
				{
					map<string, string> map0;
					map0 = GetEarthStation();
					int num = selectNode_2[0][1];
					AssignmentToLocalVector_DQZ(newEarthStation, map0, selectNode_2[0][1]);//�������Լ�Ŀ¼���������
				
					map<string, string> map1;
					map1 = GetEarthStation_1();//��ȡ����������Ϣ
					AssignmentToLocalVector_DQZ_level2(newEarthStation, UTF8_To_string("��Դ������Ϣ"), map1);
					map<string, string> map2;
					map2 = GetEarthStation_2();//��ȡ�����豸������Ϣ
					AssignmentToLocalVector_DQZ_level2(newEarthStation, UTF8_To_string("�����豸������Ϣ"), map2);
					map<string, string> map3;
					map3 = GetEarthStation_3();//��ȡ�����豸������Ϣ
					AssignmentToLocalVector_DQZ_level2(newEarthStation, UTF8_To_string("����������Ϣ"), map3);

					
				}



				//CTreeViewUI* pTreeView = static_cast<CTreeViewUI*>(m_pm.FindControl(_T("treeview2")));
				m_treeView_2->SetItemExpand(false, NULL);//�����νṹ��ʼ״̬Ϊ��չ����
				UncheckedTab2();

			}//
			else if (sMenuName.CompareNoCase(_T("FunctionType1_1")) == 0)//����վ���������ͷ���
			{

				int serialNumber;//���ѡ�����������							
				string str_name = wstringToString(m_modiftName_2.GetData());
				//Categorize_DQZ_FunctionType1(str_name);
				/*******************ɸѡ���������ڴ���������Ƶ�vector�е�λ��***************/
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
			else if (sMenuName.CompareNoCase(_T("EarthStation")) == 0)//����վ������վ���ͷ���
			{
				int serialNumber;//���ѡ�����������							
				string str_name = wstringToString(m_modiftName_2.GetData());
				
				/*******************ɸѡ���������ڴ���������Ƶ�vector�е�λ��***************/
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


			////����վ�޸�
			//if (sMenuName.CompareNoCase(_T("modify2_2")) == 0)//
			//{
			//	m_CpopModifyInterface->MessageBox(NULL, m_modiftName_2);

			//	string name = m_CpopModifyInterface->GetNewName();//	string GetNewName();//��ȡ�޸ĺ������
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
			if (sMenuName.CompareNoCase(_T("MoveTo2")) == 0)//�ƶ���ĳ�����Ÿ�
			{

				m_CpopSelectLevel1->MessageBox(NULL, name2_Level1);

				string name = m_CpopSelectLevel1->GetNodeName();//	string GetNewName();//��ȡ�޸ĺ������

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

				string name = m_CpopModifyInterface->GetNewName();//	string GetNewName();//��ȡ�޸ĺ������
				newName_3 = name;
				if (name != "")
				{
					ModifyNodeName_3();
					cout << name << endl;
				}


				cout << name << endl;
				UncheckedTab3();
			}
			else if (sMenuName.CompareNoCase(_T("NewGroundTerminal")) == 0)//�½������ն�
			{
				cout << "111111" << endl;
				m_CPopNewConstellation->MessageBox(NULL);

				string name = m_CPopNewConstellation->GetNewName();//	string GetNewName();//��ȡ�޸ĺ������
				newGroundTerminal = name;

				if (name != "")
				{
					map<string, string> map0;
					map0 = GetGroundTerminal();
					int num = selectNode_3[0][1];
					AssignmentToLocalVector_DMZD(newGroundTerminal, map0);//�������Լ�Ŀ¼���������

					map<string, string> map1;
					map1 = GetGroundTerminal_1();//��ȡ����������Ϣ
					AssignmentToLocalVector_DMZD_level2(newGroundTerminal, UTF8_To_string("�غ�������Ϣ"), map1);
					map<string, string> map2;
					map2 = GetGroundTerminal_2();//��ȡ�����豸������Ϣ
					AssignmentToLocalVector_DMZD_level2(newGroundTerminal, UTF8_To_string("�����豸������Ϣ"), map2);
					map<string, string> map3;
					map3 = GetGroundTerminal_3();//��ȡ�����豸������Ϣ
					AssignmentToLocalVector_DMZD_level2(newGroundTerminal, UTF8_To_string("����������Ϣ"), map3);


				}


				//CTreeViewUI* pTreeView = static_cast<CTreeViewUI*>(m_pm.FindControl(_T("treeview2")));
				m_treeView_3->SetItemExpand(false, NULL);//�����νṹ��ʼ״̬Ϊ��չ����
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
					m_treeView_3->SetItemExpand(false, NULL);//�����νṹ��ʼ״̬Ϊ��չ����
					HideThCcontrol_3(ControlNumber);
					UncheckedTab3();
				}
			}
			//else if (sMenuName.CompareNoCase(_T("PeerManagement")) == 0)//�½�ͬ�������ն˹���
			//{
			//	cout << "111111" << endl;
			//	m_CPopNewConstellation->MessageBox(NULL);

			//	string name = m_CPopNewConstellation->GetNewName();//	string GetNewName();//��ȡ�޸ĺ������
			//	newGroundTerminal = name;

			//	if (name != "")
			//	{
			//		CreateTreeNode_3(name);
			//		//AssignmentToLocalVector_DMZD(newGroundTerminal, map);//�������Լ�Ŀ¼���������
			//	}


			//	//CTreeViewUI* pTreeView = static_cast<CTreeViewUI*>(m_pm.FindControl(_T("treeview2")));
			//	m_treeView_3->SetItemExpand(false, NULL);//�����νṹ��ʼ״̬Ϊ��չ����
			//	UncheckedTab3();
			//}

			//if (sMenuName.CompareNoCase(_T("modify3_1")) == 0)
			//{
			//	m_CpopModifyInterface->MessageBox(NULL, m_modiftName_3);


			//	string name = m_CpopModifyInterface->GetNewName();//	string GetNewName();//��ȡ�޸ĺ������
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
	// ��ʱ�����յ�WM_NCDESTROY���յ�wParamΪSC_CLOSE��WM_SYSCOMMAND
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


string  CPopEditorialClass::get_cur_timelogdata()/*��ȡ��ǰʱ���ַ���*/
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

		// ��ť��Ϣ
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
	else if (msg.sType == _T("treeitemdbclick") )//˫������֧ && msg.pSender->GetName() == _T("treeview3")
	{
		GetNumberChild_0();
		GetNumberChild();
		CurrentIndex = 0;
		///*********************����1 ��������**********************************/
		for (int i = 0; i < pNode_1_num; i++)
		{
			//			if(pNode_1[i]->)
			int childNum = pNode_1[i]->GetCountChild();
			if (pNode_1[i]->IsSelected())
			{
				m_modiftName = pNode_1[i]->GetItemText();
				cout << "��ѡ����" << endl;
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
						cout << "��ѡ����!!!" << endl;
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
										//FindTheMap_level3(cdui_name);//����û�������غ���һ����֮ǰ�Ĳ���
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
		///*********************����2 ���Ÿ۵���վ*********************************/
		for (int i = 0; i < pNode_2_1_num; i++)
		{
			//			if(pNode_1[i]->)
			int childNum = pNode_2_1[i]->GetCountChild();
			if (pNode_2_1[i]->IsSelected())
			{
				m_modiftName_2 = pNode_2_1[i]->GetItemText();
				cout << "��ѡ����" << endl;
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
						cout << "��ѡ����!!!" << endl;
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
		///*********************����3 �����ն�***********************************/
		for (int i = 0; i < pNode_3_1_num; i++)
		{
			//			if(pNode_1[i]->)
			int childNum = pNode_3_1[i]->GetCountChild();
			if (pNode_3_1[i]->IsSelected())
			{
				m_modiftName_3 = pNode_3_1[i]->GetItemText();
				cout << "��ѡ����" << endl;
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
						cout << "��ѡ����!!!" << endl;
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
	else if (msg.sType == _T("itemrclick")) //�����ڵ����һ�����¼� _T("itemrclick")
	{
		/*******************�һ��������ǽ���******************/
		for (int i = 0; i < pNode_1_num; i++)
		{
			int childNum = pNode_1[i]->GetCountChild();
			if (pNode_1[i]->IsSelected())
			{
				m_modiftName = pNode_1[i]->GetItemText();
				cout << "��ѡ����" << endl;
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
						cout << "��ѡ����!!!" << endl;
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
		/************�����0���ڵ㱻ѡ��**************/
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

		/*********************�һ����Ÿ۵���վ����***********************/
		for (int i = 0; i < pNode_2_1_num; i++)
		{
			int childNum = pNode_2_1[i]->GetCountChild();
			if (pNode_2_1[i]->IsSelected())
			{
				m_modiftName_2 = pNode_2_1[i]->GetItemText();
				cout << "��ѡ����" << endl;
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
						cout << "��ѡ����!!!" << endl;
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
		/************�����0���ڵ㱻ѡ��**************/
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

		/*******************��������ն˽���*****************/
		for (int i = 0; i < pNode_3_1_num; i++)
		{
			int childNum = pNode_3_1[i]->GetCountChild();
			if (pNode_3_1[i]->IsSelected())
			{
				m_modiftName_3 = pNode_3_1[i]->GetItemText();
				cout << "��ѡ����" << endl;
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
						cout << "��ѡ����!!!" << endl;
						if (i == 0)
						{
							//isLevel_3_1 = true;
						}

						node->Select(false);
					}

				}
			}
		}
		/************�����0���ڵ㱻ѡ��**************/
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
		// �����������ڴ����
		HANDLE hMapFile = OpenFileMapping(
			FILE_MAP_ALL_ACCESS,
			FALSE,
			L"MySharedMemory"); // ʹ���뷢�ͽ�����ͬ������

		if (hMapFile == NULL) {
			std::cerr << "OpenFileMapping failed: " << GetLastError() << std::endl;
			Sleep(100);
			continue;
		}

		// ��ȡ�����ڴ�ָ��
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
		// ������Դ
		UnmapViewOfFile(pBuf);
		CloseHandle(hMapFile);

		// ����JSON�ַ���
		Json sceneJsona = Json::parse(myString);

		// ��JSONת��ΪSceneTLE�ṹ��
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
//��ʼ��
void CPopEditorialClass::InitWindow()
{
	m_pCloseBtn = static_cast<CButtonUI*>(m_pm.FindControl(_T("closebtn")));
	pTabDatabase = static_cast<CTabLayoutUI*>(m_pm.FindControl(_T("tab_switch")));
	

	m_treeView = static_cast<CTreeViewUI*>(m_pm.FindControl(_T("treeview1")));
	m_treeView->SetAttribute(L"itemhotimage", L"ui/�ļ���ѡ��.png");
	m_treeView->SetAttribute(L"itemselectedbkcolor", L"0xFF808080");

	m_treeView_2 = static_cast<CTreeViewUI*>(m_pm.FindControl(_T("treeview2")));
	m_treeView_2->SetAttribute(L"itemhotimage", L"ui/�ļ���ѡ��.png");
	m_treeView_2->SetAttribute(L"itemselectedbkcolor", L"0xFF808080");
	
	m_treeView_3 = static_cast<CTreeViewUI*>(m_pm.FindControl(_T("treeview3")));
	m_treeView_3->SetAttribute(L"itemhotimage", L"ui/�ļ���ѡ��.png");
	m_treeView_3->SetAttribute(L"itemselectedbkcolor", L"0xFF808080");

	pNode_1_num = 0;//��ǰ���ڵ������е�˳��
	pNode_2_num = 0;//1��ڵ������е�˳��
	pNode_3_num = 0;//2��ڵ������е�˳��
	pNode_4_num = 0;//3��ڵ������е�˳��

	pNode_2_1_num = 0;//��ǰ���ڵ������е�˳��
	pNode_2_2_num = 0;//һ���ڵ������е�˳��
	pNode_2_3_num = 0;//�����ڵ������е�˳��
	pNode_2_4_num = 0;//�����ڵ������е�˳��


	pNode_3_1_num = 0;//��ǰ���ڵ������е�˳��
	pNode_3_2_num = 0;//һ���ڵ������е�˳��
	pNode_3_3_num = 0;//�����ڵ������е�˳��
	pNode_3_4_num = 0;//�����ڵ������е�˳��

	isLevel0 = false;//ѡ�е���0���ڵ���һ��
    isLevel1 = false;//ѡ�е���1���ڵ���һ��
	isLevel2 = false;//ѡ�е���2���ڵ���һ��

	isLevel_2_0 = false;//ѡ�е���0���ڵ���һ��
	isLevel_2_1 = false;//ѡ�е���1���ڵ���һ��
	isLevel_2_2 = false;//ѡ�е���2���ڵ���һ��

	isLevel_3_0 = false;;//ѡ�е���0���ڵ���һ��
	isLevel_3_1 = false;;//ѡ�е���1���ڵ���һ��
	
	tab3RootType = 0;//tab3���������͵ĸ��ڵ��ʽ
	RailTypeFlag = false;
	FunctionalCategoryFlag = false;//���ܷ��� FunctionalCategory
	PayloadFlag = false;//�غ����ͷ���

	rootNum = 0;
	selectNode[0][0] = -1;
	selectNode[0][1] = -1;
	selectNode[0][2] = -1;
	selectNode[0][3] = -1;


	CreateTreeNode(UTF8_To_string("�������ǹ���"));//����ڵ������

	///********************���ǽ�������Ǻ�����********************/

	CreateTreeNode_2(UTF8_To_string("���Ÿ۵���վ����"));//��ڶ����������Ӹ��ڵ�
	
	//AssignmentToLocalVector_DXG(UTF8_To_string("���Ÿ�2"));//�������Լ�Ŀ¼���������
	/**********************����3����-�����ն�******************/
	CreateTreeNode_3(UTF8_To_string("�����ն˹���"));//��ڶ����������Ӹ��ڵ�

	//AssignmentToLocalVector_DMZD_level2(UTF8_To_string("�����ն�2"), UTF8_To_string("����������Ϣ"), map3_3_3);
	/**********************************************/
	CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ThreadReadtle, this, 0, NULL);
}



void CPopEditorialClass::CreateTreeNode()
{
	///****************************����������Ӫ��*****************************/
	pNode_1[pNode_1_num] = new CTreeNodeUI();
	pNode_1[pNode_1_num]->CheckBoxSelected(false);
	pNode_1[pNode_1_num]->SetFixedHeight(30);
	pNode_1[pNode_1_num]->SetItemTextColor(0xFFFFFFFF);
	pNode_1[pNode_1_num]->SetItemHotTextColor(0xFFFFFFFF);
	pNode_1[pNode_1_num]->SetSelItemTextColor(0xFFFFFFFF);
	pNode_1[pNode_1_num]->SetAttribute(_T("folderattr"), _T("padding=&quot;0,1,0,0&quot; width=&quot;18&quot; height=&quot;18&quot; normalimage=&quot;file='ui/չ��.png' &quot; "));
	pNode_1[pNode_1_num]->SetAttribute(_T("folderattr"), _T("hotimage=&quot;file='ui/չ��.png' &quot; selectedimage=&quot;file='ui/δչ��.png'&quot; selectedhotimage=&quot;file='ui/δչ��.png'&quot;"));
	pNode_1[pNode_1_num]->SetAttribute(_T("itemattr"), _T("padding=\"16,0,0,0\" align=\"left\" font=\"13\""));//font=&quot;6&quot;
	pNode_1[pNode_1_num]->SetItemText(_T("����"));

	/**********************************************/
	m_treeView->Add(pNode_1[pNode_1_num]);			//Ҫ����Ӹ��ڵ�,��������ӽڵ����ʽ	
	int TreeNodeItem_Number = 0;
	///****************A********************/
	pNode_2[pNode_2_num] = new CTreeNodeUI();
	pNode_2[pNode_2_num]->CheckBoxSelected(false);
	pNode_2[pNode_2_num]->SetItemText(_T("��������1"));
	pNode_2[pNode_2_num]->SetFixedHeight(30);
	pNode_2[pNode_2_num]->SetItemTextColor(0xFFFFFFFF);
	pNode_2[pNode_2_num]->SetItemHotTextColor(0xFFFFFFFF);
	pNode_2[pNode_2_num]->SetSelItemTextColor(0xFFFFFFFF);
	pNode_2[pNode_2_num]->SetAttribute(_T("folderattr"), _T("padding=&quot;0,1,0,0&quot; width=&quot;16&quot; height=&quot;16&quot; normalimage=&quot;file='ui/չ��.png' &quot; "));
	pNode_2[pNode_2_num]->SetAttribute(_T("folderattr"), _T("hotimage=&quot;file='ui/չ��.png' &quot; selectedimage=&quot;file='ui/δչ��.png'&quot; selectedhotimage=&quot;file='ui/δչ��.png'&quot;"));
	pNode_2[pNode_2_num]->SetAttribute(_T("itemattr"), _T("padding=\"16,0,0,0\" align=\"left\" font=\"13\""));//font=&quot;6&quot;
	pNode_2[pNode_2_num]->SetName(_T("WX1"));
	pNode_1[pNode_1_num]->AddChildNode(pNode_2[pNode_2_num]);

}
void CPopEditorialClass::CreateTreeNode(string tableName, string str)
{
	int sequences;//��Ÿ��ڵ�����
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
	pNode_2[pNode_2_num]->SetAttribute(_T("folderattr"), _T("padding=&quot;0,1,0,0&quot; width=&quot;16&quot; height=&quot;16&quot; normalimage=&quot;file='ui/չ��.png' &quot; "));
	pNode_2[pNode_2_num]->SetAttribute(_T("folderattr"), _T("hotimage=&quot;file='ui/չ��.png' &quot; selectedimage=&quot;file='ui/δչ��.png'&quot; selectedhotimage=&quot;file='ui/δչ��.png'&quot;"));
	pNode_2[pNode_2_num]->SetAttribute(_T("itemattr"), _T("padding=\"16,0,0,0\" align=\"left\" font=\"13\""));//font=&quot;6&quot;
	pNode_2[pNode_2_num]->SetName(_T("WX1"));
	pNode_1[0]->AddChildNode(pNode_2[pNode_2_num]);
	pNode_2_num += 1;


}
//��������
void  CPopEditorialClass::CreateTreeNode_livel1(string tableName)
{
	pNode_2[pNode_2_num] = new CTreeNodeUI();
	pNode_2[pNode_2_num]->CheckBoxSelected(false);
	pNode_2[pNode_2_num]->SetItemText(stringToWstring(tableName).c_str());
	pNode_2[pNode_2_num]->SetFixedHeight(30);
	pNode_2[pNode_2_num]->SetItemTextColor(0xFFFFFFFF);
	pNode_2[pNode_2_num]->SetItemHotTextColor(0xFFFFFFFF);
	pNode_2[pNode_2_num]->SetSelItemTextColor(0xFFFFFFFF);
	pNode_2[pNode_2_num]->SetAttribute(_T("folderattr"), _T("padding=&quot;0,1,0,0&quot; width=&quot;16&quot; height=&quot;16&quot; normalimage=&quot;file='ui/չ��.png' &quot; "));
	pNode_2[pNode_2_num]->SetAttribute(_T("folderattr"), _T("hotimage=&quot;file='ui/չ��.png' &quot; selectedimage=&quot;file='ui/δչ��.png'&quot; selectedhotimage=&quot;file='ui/δչ��.png'&quot;"));
	pNode_2[pNode_2_num]->SetAttribute(_T("itemattr"), _T("padding=\"16,0,0,0\" align=\"left\" font=\"13\""));//font=&quot;6&quot;
	pNode_2[pNode_2_num]->SetName(_T("WX1"));
	pNode_1[0]->AddChildNode(pNode_2[pNode_2_num]);
	pNode_2_num += 1;
}
//��������
void CPopEditorialClass::CreateTreeNode_livel2(string tableName,int first_level)
{
	pNode_3[pNode_3_num] = new CTreeNodeUI();
	pNode_3[pNode_3_num]->CheckBoxSelected(false);
	pNode_3[pNode_3_num]->SetItemText(stringToWstring(tableName).c_str());
	pNode_3[pNode_3_num]->SetFixedHeight(30);
	pNode_3[pNode_3_num]->SetItemTextColor(0xFFFFFFFF);
	pNode_3[pNode_3_num]->SetItemHotTextColor(0xFFFFFFFF);
	pNode_3[pNode_3_num]->SetSelItemTextColor(0xFFFFFFFF);
	pNode_3[pNode_3_num]->SetAttribute(_T("folderattr"), _T("padding=&quot;0,1,0,0&quot; width=&quot;16&quot; height=&quot;16&quot; normalimage=&quot;file='ui/չ��.png' &quot; "));
	pNode_3[pNode_3_num]->SetAttribute(_T("folderattr"), _T("hotimage=&quot;file='ui/չ��.png' &quot; selectedimage=&quot;file='ui/δչ��.png'&quot; selectedhotimage=&quot;file='ui/δչ��.png'&quot;"));
	pNode_3[pNode_3_num]->SetAttribute(_T("itemattr"), _T("padding=\"16,0,0,0\" align=\"left\" font=\"13\""));//font=&quot;6&quot;
	pNode_3[pNode_3_num]->SetName(_T("WX1"));
	pNode_2[first_level]->AddChildNode(pNode_3[pNode_3_num]);
	pNode_3_num += 1;
}
//���������ڵ�
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
	pNode_4[pNode_4_num]->SetAttribute(_T("folderattr"), _T("padding=&quot;0,1,0,0&quot; width=&quot;16&quot; height=&quot;16&quot; normalimage=&quot;file='ui/չ��.png' &quot; "));
	pNode_4[pNode_4_num]->SetAttribute(_T("folderattr"), _T("hotimage=&quot;file='ui/չ��.png' &quot; selectedimage=&quot;file='ui/δչ��.png'&quot; selectedhotimage=&quot;file='ui/δչ��.png'&quot;"));
	pNode_4[pNode_4_num]->SetAttribute(_T("itemattr"), _T("padding=\"16,0,0,0\" align=\"left\" font=\"13\""));//font=&quot;6&quot;
	pNode_4[pNode_4_num]->SetName(_T("WX1"));
	pNode_3[first_level]->AddChildNode(pNode_4[pNode_4_num]);
	pNode_4_num += 1;
}
//����һ���ڵ�-----------���Ÿ۵���վ����
void CPopEditorialClass::CreateTreeNode_livel2_1(string tableName)
{
	pNode_2_2[pNode_2_2_num] = new CTreeNodeUI();
	pNode_2_2[pNode_2_2_num]->CheckBoxSelected(false);
	pNode_2_2[pNode_2_2_num]->SetItemText(stringToWstring(tableName).c_str());
	pNode_2_2[pNode_2_2_num]->SetFixedHeight(30);
	pNode_2_2[pNode_2_2_num]->SetItemTextColor(0xFFFFFFFF);
	pNode_2_2[pNode_2_2_num]->SetItemHotTextColor(0xFFFFFFFF);
	pNode_2_2[pNode_2_2_num]->SetSelItemTextColor(0xFFFFFFFF);
	pNode_2_2[pNode_2_2_num]->SetAttribute(_T("folderattr"), _T("padding=&quot;0,1,0,0&quot; width=&quot;16&quot; height=&quot;16&quot; normalimage=&quot;file='ui/չ��.png' &quot; "));
	pNode_2_2[pNode_2_2_num]->SetAttribute(_T("folderattr"), _T("hotimage=&quot;file='ui/չ��.png' &quot; selectedimage=&quot;file='ui/δչ��.png'&quot; selectedhotimage=&quot;file='ui/δչ��.png'&quot;"));
	pNode_2_2[pNode_2_2_num]->SetAttribute(_T("itemattr"), _T("padding=\"16,0,0,0\" align=\"left\" font=\"13\""));//font=&quot;6&quot;
	pNode_2_2[pNode_2_2_num]->SetName(_T("WX1"));
	pNode_2_1[0]->AddChildNode(pNode_2_2[pNode_2_2_num]);
	pNode_2_2_num += 1;
}
//���������ڵ�--------------���Ÿ۵���վ����
void CPopEditorialClass::CreateTreeNode_livel2_2(string tableName, int first_level)
{
	pNode_2_3[pNode_2_3_num] = new CTreeNodeUI();
	pNode_2_3[pNode_2_3_num]->CheckBoxSelected(false);
	pNode_2_3[pNode_2_3_num]->SetItemText(stringToWstring(tableName).c_str());
	pNode_2_3[pNode_2_3_num]->SetFixedHeight(30);
	pNode_2_3[pNode_2_3_num]->SetItemTextColor(0xFFFFFFFF);
	pNode_2_3[pNode_2_3_num]->SetItemHotTextColor(0xFFFFFFFF);
	pNode_2_3[pNode_2_3_num]->SetSelItemTextColor(0xFFFFFFFF);
	pNode_2_3[pNode_2_3_num]->SetAttribute(_T("folderattr"), _T("padding=&quot;0,1,0,0&quot; width=&quot;16&quot; height=&quot;16&quot; normalimage=&quot;file='ui/չ��.png' &quot; "));
	pNode_2_3[pNode_2_3_num]->SetAttribute(_T("folderattr"), _T("hotimage=&quot;file='ui/չ��.png' &quot; selectedimage=&quot;file='ui/δչ��.png'&quot; selectedhotimage=&quot;file='ui/δչ��.png'&quot;"));
	pNode_2_3[pNode_2_3_num]->SetAttribute(_T("itemattr"), _T("padding=\"16,0,0,0\" align=\"left\" font=\"13\""));//font=&quot;6&quot;
	pNode_2_3[pNode_2_3_num]->SetName(_T("WX1"));
	pNode_2_2[first_level]->AddChildNode(pNode_2_3[pNode_2_3_num]);
	pNode_2_3_num += 1;
}
//���������ڵ� firstlevel��ָ
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
	pNode_2_4[pNode_2_4_num]->SetAttribute(_T("folderattr"), _T("padding=&quot;0,1,0,0&quot; width=&quot;16&quot; height=&quot;16&quot; normalimage=&quot;file='ui/չ��.png' &quot; "));
	pNode_2_4[pNode_2_4_num]->SetAttribute(_T("folderattr"), _T("hotimage=&quot;file='ui/չ��.png' &quot; selectedimage=&quot;file='ui/δչ��.png'&quot; selectedhotimage=&quot;file='ui/δչ��.png'&quot;"));
	pNode_2_4[pNode_2_4_num]->SetAttribute(_T("itemattr"), _T("padding=\"16,0,0,0\" align=\"left\" font=\"13\""));//font=&quot;6&quot;
	pNode_2_4[pNode_2_4_num]->SetName(_T("WX1"));
	pNode_2_3[first_level]->AddChildNode(pNode_2_4[pNode_2_4_num]);
	pNode_2_4_num += 1;
}
//����һ���ڵ�-----------�����ն�
void CPopEditorialClass::CreateTreeNode_livel3_1(string tableName)
{
	pNode_3_2[pNode_3_2_num] = new CTreeNodeUI();
	pNode_3_2[pNode_3_2_num]->CheckBoxSelected(false);
	pNode_3_2[pNode_3_2_num]->SetItemText(stringToWstring(tableName).c_str());
	pNode_3_2[pNode_3_2_num]->SetFixedHeight(30);
	pNode_3_2[pNode_3_2_num]->SetItemTextColor(0xFFFFFFFF);
	pNode_3_2[pNode_3_2_num]->SetItemHotTextColor(0xFFFFFFFF);
	pNode_3_2[pNode_3_2_num]->SetSelItemTextColor(0xFFFFFFFF);
	pNode_3_2[pNode_3_2_num]->SetAttribute(_T("folderattr"), _T("padding=&quot;0,1,0,0&quot; width=&quot;16&quot; height=&quot;16&quot; normalimage=&quot;file='ui/չ��.png' &quot; "));
	pNode_3_2[pNode_3_2_num]->SetAttribute(_T("folderattr"), _T("hotimage=&quot;file='ui/չ��.png' &quot; selectedimage=&quot;file='ui/δչ��.png'&quot; selectedhotimage=&quot;file='ui/δչ��.png'&quot;"));
	pNode_3_2[pNode_3_2_num]->SetAttribute(_T("itemattr"), _T("padding=\"16,0,0,0\" align=\"left\" font=\"13\""));//font=&quot;6&quot;
	pNode_3_2[pNode_3_2_num]->SetName(_T("WX1"));
	pNode_3_1[0]->AddChildNode(pNode_3_2[pNode_3_2_num]);
	pNode_3_2_num += 1;
}
//���������ڵ�-----------�����ն��ӽڵ�
void CPopEditorialClass::CreateTreeNode_livel3_2(string tableName, int first_level)
{
	pNode_3_3[pNode_3_3_num] = new CTreeNodeUI();
	pNode_3_3[pNode_3_3_num]->CheckBoxSelected(false);
	pNode_3_3[pNode_3_3_num]->SetItemText(stringToWstring(tableName).c_str());
	pNode_3_3[pNode_3_3_num]->SetFixedHeight(30);
	pNode_3_3[pNode_3_3_num]->SetItemTextColor(0xFFFFFFFF);
	pNode_3_3[pNode_3_3_num]->SetItemHotTextColor(0xFFFFFFFF);
	pNode_3_3[pNode_3_3_num]->SetSelItemTextColor(0xFFFFFFFF);
	pNode_3_3[pNode_3_3_num]->SetAttribute(_T("folderattr"), _T("padding=&quot;0,1,0,0&quot; width=&quot;16&quot; height=&quot;16&quot; normalimage=&quot;file='ui/չ��.png' &quot; "));
	pNode_3_3[pNode_3_3_num]->SetAttribute(_T("folderattr"), _T("hotimage=&quot;file='ui/չ��.png' &quot; selectedimage=&quot;file='ui/δչ��.png'&quot; selectedhotimage=&quot;file='ui/δչ��.png'&quot;"));
	pNode_3_3[pNode_3_3_num]->SetAttribute(_T("itemattr"), _T("padding=\"16,0,0,0\" align=\"left\" font=\"13\""));//font=&quot;6&quot;
	pNode_3_3[pNode_3_3_num]->SetName(_T("WX1"));
	pNode_3_2[first_level]->AddChildNode(pNode_3_3[pNode_3_3_num]);
	pNode_3_3_num += 1;
}
//�������ڵ�
void CPopEditorialClass::CreateTreeNode(string tableName)
{
	pNode_1[pNode_1_num] = new CTreeNodeUI();
	pNode_1[pNode_1_num]->CheckBoxSelected(false);
	pNode_1[pNode_1_num]->SetFixedHeight(30);
	pNode_1[pNode_1_num]->SetItemTextColor(0xFFFFFFFF);
	pNode_1[pNode_1_num]->SetItemHotTextColor(0xFFFFFFFF);
	pNode_1[pNode_1_num]->SetSelItemTextColor(0xFFFFFFFF);
	pNode_1[pNode_1_num]->SetAttribute(_T("folderattr"), _T("padding=&quot;0,1,0,0&quot; width=&quot;18&quot; height=&quot;18&quot; normalimage=&quot;file='ui/չ��.png' &quot; "));
	pNode_1[pNode_1_num]->SetAttribute(_T("folderattr"), _T("hotimage=&quot;file='ui/չ��.png' &quot; selectedimage=&quot;file='ui/δչ��.png'&quot; selectedhotimage=&quot;file='ui/δչ��.png'&quot;"));
	pNode_1[pNode_1_num]->SetAttribute(_T("itemattr"), _T("padding=\"16,0,0,0\" align=\"left\" font=\"13\""));//font=&quot;6&quot;
	pNode_1[pNode_1_num]->SetItemText(stringToWstring(tableName).c_str());
	m_treeView->Add(pNode_1[pNode_1_num]);			//Ҫ����Ӹ��ڵ�,��������ӽڵ����ʽ	
	
	vec_root.push_back(tableName);
	pNode_1_num += 1;

}
//�����ڶ�������ĸ��ڵ�
void CPopEditorialClass::CreateTreeNode_2(string tableName)
{
	pNode_2_1[pNode_2_1_num] = new CTreeNodeUI();
	pNode_2_1[pNode_2_1_num]->CheckBoxSelected(false);
	pNode_2_1[pNode_2_1_num]->SetFixedHeight(30);
	pNode_2_1[pNode_2_1_num]->SetItemTextColor(0xFFFFFFFF);
	pNode_2_1[pNode_2_1_num]->SetItemHotTextColor(0xFFFFFFFF);
	pNode_2_1[pNode_2_1_num]->SetSelItemTextColor(0xFFFFFFFF);
	pNode_2_1[pNode_2_1_num]->SetAttribute(_T("folderattr"), _T("padding=&quot;0,1,0,0&quot; width=&quot;18&quot; height=&quot;18&quot; normalimage=&quot;file='ui/չ��.png' &quot; "));
	pNode_2_1[pNode_2_1_num]->SetAttribute(_T("folderattr"), _T("hotimage=&quot;file='ui/չ��.png' &quot; selectedimage=&quot;file='ui/δչ��.png'&quot; selectedhotimage=&quot;file='ui/δչ��.png'&quot;"));
	pNode_2_1[pNode_2_1_num]->SetAttribute(_T("itemattr"), _T("padding=\"16,0,0,0\" align=\"left\" font=\"13\""));//font=&quot;6&quot;
	pNode_2_1[pNode_2_1_num]->SetItemText(stringToWstring(tableName).c_str());
	m_treeView_2->Add(pNode_2_1[pNode_2_1_num]);			//Ҫ����Ӹ��ڵ�,��������ӽڵ����ʽ	

	vec_2_root.push_back(tableName);
	pNode_2_1_num += 1;
}
//��������������ĸ��ڵ�
void CPopEditorialClass::CreateTreeNode_3(string tableName)
{
	pNode_3_1[pNode_3_1_num] = new CTreeNodeUI();
	pNode_3_1[pNode_3_1_num]->CheckBoxSelected(false);
	pNode_3_1[pNode_3_1_num]->SetFixedHeight(30);
	pNode_3_1[pNode_3_1_num]->SetItemTextColor(0xFFFFFFFF);
	pNode_3_1[pNode_3_1_num]->SetItemHotTextColor(0xFFFFFFFF);
	pNode_3_1[pNode_3_1_num]->SetSelItemTextColor(0xFFFFFFFF);
	pNode_3_1[pNode_3_1_num]->SetAttribute(_T("folderattr"), _T("padding=&quot;0,1,0,0&quot; width=&quot;18&quot; height=&quot;18&quot; normalimage=&quot;file='ui/չ��.png' &quot; "));
	pNode_3_1[pNode_3_1_num]->SetAttribute(_T("folderattr"), _T("hotimage=&quot;file='ui/չ��.png' &quot; selectedimage=&quot;file='ui/δչ��.png'&quot; selectedhotimage=&quot;file='ui/δչ��.png'&quot;"));
	pNode_3_1[pNode_3_1_num]->SetAttribute(_T("itemattr"), _T("padding=\"16,0,0,0\" align=\"left\" font=\"13\""));//font=&quot;6&quot;
	pNode_3_1[pNode_3_1_num]->SetItemText(stringToWstring(tableName).c_str());
	m_treeView_3->Add(pNode_3_1[pNode_3_1_num]);			//Ҫ����Ӹ��ڵ�,��������ӽڵ����ʽ	

	vec_3_root.push_back(tableName);
	pNode_3_1_num += 1;
}
//����һ��Ŀ¼�����ڵ� --------����
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
		_mkdir(path1.c_str());   // ���� 0 ��ʾ�����ɹ���-1 ��ʾʧ��
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
//����һ��Ŀ¼�����ڵ� --------����
void CPopEditorialClass::AssignmentToLocalVector_XZ(string nodeName, map<string, string> m_map)
{
//	map<string, string> m_map = GetHoroscopeInform();

	CreateTreeNode_livel1(nodeName);
	name_Level1.push_back(nodeName);
	m_RecvMap_XZ.push_back(m_map);
	vec_ExpandFlags.push_back(FALSE);
}
//��������Ŀ¼���ڵ�  --------����
void CPopEditorialClass::AssignmentToLocalVector(string nodeName, map<string, string> m_map,int first_level)
{
	CreateTreeNode_livel2(nodeName, first_level);
	name_Level2.push_back(nodeName);
	m_RecvMap.push_back(m_map);
}
//��������Ŀ¼�����ڵ� --------����������Ϣ �غ�������Ϣ
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
//��������Ŀ¼�����ڵ� --------���� �����ʱ��
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
//��������Ŀ¼�����ڵ� �����ʱ��--------����������Ϣ �غ�������Ϣ
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

// ����һ��Ŀ¼�����ڵ�--------���Ÿ�
void CPopEditorialClass::AssignmentToLocalVector_DXG(string nodeName)
{
	map<string, string> m_map = GetTeleport();
	CreateTreeNode_livel2_1(nodeName);
	name2_Level1.push_back(nodeName);
	m_RecvMap_DXG.push_back(m_map);

	vec_ExpandFlags2_FT.push_back(FALSE);//��ŵ��Ÿ��Ƿ�����˹�������ɸѡ
	vec_ExpandFlags2_ES.push_back(FALSE);//��ŵ��Ÿ��Ƿ�����˵���վ����ɸѡ

	string path1 = "D:\\DATA1";
	if (0 != _access(path1.c_str(), 0))
	{
		_mkdir(path1.c_str());   // ���� 0 ��ʾ�����ɹ���-1 ��ʾʧ��
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

	vec_ExpandFlags2_FT.push_back(FALSE);//��ŵ��Ÿ��Ƿ�����˹�������ɸѡ
	vec_ExpandFlags2_ES.push_back(FALSE);//��ŵ��Ÿ��Ƿ�����˵���վ����ɸѡ
}
////��������Ŀ¼�����ڵ� --------����վ
void CPopEditorialClass::AssignmentToLocalVector_DQZ(string nodeName, map<string, string> m_map, int first_level)
{
	CreateTreeNode_livel2_2(nodeName, first_level);
	name2_Level2.push_back(nodeName);
	m_RecvMap_DQZ.push_back(m_map);

	string path1 = "D:\\DATA1";
	if (0 != _access(path1.c_str(), 0))
	{
		_mkdir(path1.c_str());   // ���� 0 ��ʾ�����ɹ���-1 ��ʾʧ��
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
////��������Ŀ¼�����ڵ� --------����վ �����ʱ��
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

//��������Ŀ¼�����ڵ� --------����վ
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
//��������Ŀ¼�����ڵ� �����ʱ��--------��Դ��������
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
//����һ��Ŀ¼�����ڵ� --------�����ն�
void CPopEditorialClass::AssignmentToLocalVector_DMZD(string nodeName, map<string, string> m_map)
{
	CreateTreeNode_livel3_1(nodeName);
	name3_Level1.push_back(nodeName);
	m_RecvMap_DMZD.push_back(m_map);
}
//��������Ŀ¼�����ڵ� --------�����ն��ӽڵ�
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
//��������Ŀ¼�����ڵ� --------�غɲ������� �����ʱ��
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
//��ȡ0���ڵ��ӽڵ������------��ȡһ���ڵ����� ���ڵ���ӽڵ�
void CPopEditorialClass::GetNumberChild_0()
{
	for (int i = 0; i < pNode_1_num; i++)
	{

		int childNum = pNode_1[i]->GetCountChild();
		NumberChild_0[i] = childNum;
	}
}
//��ȡһ���ڵ��ӽڵ������
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
//����map����ʾ
void CPopEditorialClass::FindTheMap()//����map
{
	int num = 0;

	if (selectNode[0][0] >= 0 && selectNode[0][1] < 0 && selectNode[0][2] < 0 && selectNode[0][3] < 0)//˫��0���ڵ�����
	{
		HideThCcontrol(ControlNumber);//DisplayInformation(m_RecvMap_XZ[selectNode[0][0]]);
	}
	else if (selectNode[0][0] >= 0 && selectNode[0][1] >= 0 && selectNode[0][2] < 0 && selectNode[0][3] < 0)//˫��1���ڵ�����
	{
		CTreeNodeUI*  node = pNode_1[selectNode[0][0]]->GetChildNode(selectNode[0][1]);//GetParentNode
		wstring wstr_name;
		wstr_name = node->GetItemText();
		string str_name = wstringToString(wstr_name);

		int sequences;//�����vector�е�˳��
		for (int i = 0; i < name_Level1.size(); i++)
		{
			if (str_name == name_Level1[i])
			{
				sequences = i;
			}
		}

		DisplayInformation(m_RecvMap_XZ[sequences]);


	}
	else if (selectNode[0][0] >= 0 && selectNode[0][1] >= 0 && selectNode[0][2] >= 0 && selectNode[0][3] < 0)//˫��2���ڵ�����
	{
		CTreeNodeUI*  node = pNode_1[selectNode[0][0]]->GetChildNode(selectNode[0][1])->GetChildNode(selectNode[0][2]);//GetParentNode
		wstring wstr_name;
		wstr_name = node->GetItemText();
		string str_name = wstringToString(wstr_name);
		int sequences;//�����vector�е�˳��
		if (str_name == UTF8_To_string("ͨ��") || str_name == UTF8_To_string("����") || str_name == UTF8_To_string("ң��"))//|| str_name == "����" || str_name == "ң��"
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
	else if (selectNode[0][0] >= 0 && selectNode[0][1] >= 0 && selectNode[0][2] >= 0 && selectNode[0][3] >= 0)//˫��3���ڵ�����
	{
		CTreeNodeUI*  node0 = pNode_1[selectNode[0][0]]->GetChildNode(selectNode[0][1])->GetChildNode(selectNode[0][2]);//GetParentNode
		wstring wstr_name0;
		wstr_name0 = node0->GetItemText();
		string str_name0 = wstringToString(wstr_name0);
		if (str_name0 == UTF8_To_string("ͨ��")|| str_name0 == UTF8_To_string("����") || str_name0 == UTF8_To_string("ң��"))
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

			int sequences;//�����vector�е�˳��
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

	if (selectNode_2[0][0] >= 0 && selectNode_2[0][1] < 0 && selectNode_2[0][2] < 0 && selectNode_2[0][3] < 0)//�޸�0���ڵ�����
	{
		HideThCcontrol_2(ControlNumber);//DisplayInformation(m_RecvMap_XZ[selectNode[0][0]]);
	}
	else if (selectNode_2[0][0] >= 0 && selectNode_2[0][1] >= 0 && selectNode_2[0][2] < 0 && selectNode_2[0][3] < 0)//�޸�1���ڵ�����
	{
		CTreeNodeUI*  node = pNode_2_1[selectNode_2[0][0]]->GetChildNode(selectNode_2[0][1]);//GetParentNode
		wstring wstr_name;
		wstr_name = node->GetItemText();
		string str_name = wstringToString(wstr_name);

		int sequences;//�����vector�е�˳��
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
		int sequences;//�����vector�е�˳��
		if (str_name == UTF8_To_string("�ؿ�վ") || str_name == UTF8_To_string("�Ź�վ") || str_name == UTF8_To_string("���վ") || str_name == UTF8_To_string("������Ŧ")
			|| str_name == UTF8_To_string("�̶�") || str_name == UTF8_To_string("�ƶ�"))//|| str_name == "����" || str_name == "ң��"
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
		if (str_name0 == UTF8_To_string("�ؿ�վ") || str_name0 == UTF8_To_string("�Ź�վ") || str_name0 == UTF8_To_string("���վ") || str_name0 == UTF8_To_string("������Ŧ")
			|| str_name0 == UTF8_To_string("�̶�") || str_name0 == UTF8_To_string("�ƶ�"))//|| str_name == "����" || str_name == "ң��"
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

			int sequences;//�����vector�е�˳��
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
//����map-------����3
void CPopEditorialClass::FindTheMap_3()
{
	HideThCcontrol_3(ControlNumber);
	int num = 0;

	if (selectNode_3[0][0] >= 0 && selectNode_3[0][1] < 0 && selectNode_3[0][2] < 0)//�޸�0���ڵ�����
	{
		HideThCcontrol_3(ControlNumber);//DisplayInformation(m_RecvMap_XZ[selectNode[0][0]]);
	}
	else if (selectNode_3[0][0] >= 0 && selectNode_3[0][1] >= 0 && selectNode_3[0][2] < 0)//�޸�1���ڵ�����
	{
		CTreeNodeUI*  node = pNode_3_1[selectNode_3[0][0]]->GetChildNode(selectNode_3[0][1]);//GetParentNode
		wstring wstr_name;
		wstr_name = node->GetItemText();
		string str_name = wstringToString(wstr_name);

		int sequences;//�����vector�е�˳��
		for (int i = 0; i < name3_Level1.size(); i++)
		{
			if (str_name == name3_Level1[i])
			{
				sequences = i;
			}
		}
		DisplayInformation_3(m_RecvMap_DMZD[sequences]);


	}
	else if (selectNode_3[0][0] >= 0 && selectNode_3[0][1] >= 0 && selectNode_3[0][2] >= 0)//�޸�1���ڵ�����
	{
		CTreeNodeUI*  node = pNode_3_1[selectNode_3[0][0]]->GetChildNode(selectNode_3[0][1])->GetChildNode(selectNode_3[0][2]);//GetParentNode
		wstring wstr_name;
		wstr_name = node->GetItemText();
		string str_name = wstringToString(wstr_name);
		int sequences;//�����vector�е�˳��
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

//˫��3��Ŀ¼ tab1
void CPopEditorialClass::FindTheMap_level3(CDuiString cdui_name)
{
	int sequences;//�����vector�е�˳��
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
//��������������ǽ���ɸѡ��ʱ�� ˫��4��Ŀ¼
void CPopEditorialClass::FindTheMap_level4(CDuiString cdui_name)
{
	int sequences = -1;//�����vector�е�˳��
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
//�Բ����������tab2 ˫��4��Ŀ¼
void CPopEditorialClass::FindTheMap_level4_2(CDuiString cdui_name)
{
	int sequences;//�����vector�е�˳��
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
//��ʾ���Ÿ۵���վ��Ϣ
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
//���ؽ����ϵĿؼ���
void  CPopEditorialClass::HideThCcontrol(int num)//num����xml����ؼ������� Ҳ���Ǹ���
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
//���ؿؼ�
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
//��ȡ�����޸ĵ����� �����Ӧ��vector
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
//�޸Ľڵ�����
void CPopEditorialClass::ModifyNodeName()
{
	//selectNode[0][0] = i;
	//selectNode[0][1] = 0;
	if (selectNode[0][0] >= 0 && selectNode[0][1] < 0 && selectNode[0][2] < 0)//�޸�0���ڵ�����
	{
		pNode_1[selectNode[0][0]]->SetItemText(stringToWstring(newName).c_str());
	}
	if (selectNode[0][0] >= 0 && selectNode[0][1] >= 0 && selectNode[0][2] < 0)//�޸�1���ڵ�����
	{
		//pNode_2[]
		CTreeNodeUI*  node = pNode_1[selectNode[0][0]]->GetChildNode(selectNode[0][1]);//GetParentNode
		wstring wstr_name;
		wstr_name = node->GetItemText();
		string str_name = wstringToString(wstr_name);
		int sequences;//�����vector�е�˳��
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

	if (selectNode[0][0] >= 0 && selectNode[0][1] >= 0 && selectNode[0][2] >= 0)//�޸�2���ڵ�����
	{
		//pNode_2[]
		CTreeNodeUI*  node = pNode_1[selectNode[0][0]]->GetChildNode(selectNode[0][1])->GetChildNode(selectNode[0][2]);//GetParentNode
		wstring wstr_name;
		wstr_name = node->GetItemText();
		string str_name = wstringToString(wstr_name);
		int sequences;//�����vector�е�˳��
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
//�޸Ľڵ����� --���Ÿ�
void  CPopEditorialClass::ModifyNodeName_2()
{
	if (selectNode_2[0][0] >= 0 && selectNode_2[0][1] < 0 && selectNode_2[0][2] < 0)//�޸�0���ڵ�����
	{
		pNode_2_1[selectNode_2[0][0]]->SetItemText(stringToWstring(newName_2).c_str());
	}
	if (selectNode_2[0][0] >= 0 && selectNode_2[0][1] >= 0 && selectNode_2[0][2] < 0)//�޸�1���ڵ�����
	{
		//pNode_2[]
		CTreeNodeUI*  node = pNode_2_1[selectNode_2[0][0]]->GetChildNode(selectNode_2[0][1]);//GetParentNode
		wstring wstr_name;
		wstr_name = node->GetItemText();
		string str_name = wstringToString(wstr_name);
		int sequences;//�����vector�е�˳��
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

	if (selectNode_2[0][0] >= 0 && selectNode_2[0][1] >= 0 && selectNode_2[0][2] >= 0)//�޸�2���ڵ�����
	{
		//pNode_2[]
		CTreeNodeUI*  node = pNode_2_1[selectNode_2[0][0]]->GetChildNode(selectNode_2[0][1])->GetChildNode(selectNode_2[0][2]);//GetParentNode
		wstring wstr_name;
		wstr_name = node->GetItemText();
		string str_name = wstringToString(wstr_name);
		int sequences;//�����vector�е�˳��
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
//�޸Ľڵ����� ---�����ն��豸
void  CPopEditorialClass::ModifyNodeName_3()
{
	if (selectNode_3[0][0] >= 0 && selectNode_3[0][1] < 0 && selectNode_3[0][2] < 0)//�޸�0���ڵ�����
	{
		pNode_3_1[selectNode_3[0][0]]->SetItemText(stringToWstring(newName_3).c_str());
	}
	if (selectNode_3[0][0] >= 0 && selectNode_3[0][1] >= 0 && selectNode_3[0][2] < 0)//�޸�1���ڵ�����
	{
		//pNode_2[]
		CTreeNodeUI*  node = pNode_3_1[selectNode_3[0][0]]->GetChildNode(selectNode_3[0][1]);//GetParentNode
		wstring wstr_name;
		wstr_name = node->GetItemText();
		string str_name = wstringToString(wstr_name);
		int sequences;//�����vector�е�˳��
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

	infile.open(path1);   //���ļ����������ļ��������� 
	//assert(infile.is_open());   //��ʧ��,�����������Ϣ,����ֹ�������� 
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
		infile.close();             //�ر��ļ������� 
	}

	return l_map;
}
//��txt��ȡ������Ϣ
map<string, string> CPopEditorialClass::GetSatelliteInform()
{
	map<string, string> l_map;

	ifstream infile;
	string path1 = "D:\\DATA1\\Demo\\Satellite.txt";

	infile.open(path1);   //���ļ����������ļ��������� 
	//assert(infile.is_open());   //��ʧ��,�����������Ϣ,����ֹ�������� 
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
		infile.close();             //�ر��ļ������� 
	}

	return l_map;
}
//��txt��ȡ���ǵ�����������Ϣ
map<string, string> CPopEditorialClass::GetSatelliteInform_1()
{
	map<string, string> l_map;

	ifstream infile;
	string path1 = "D:\\DATA1\\Demo\\SatelliteAntennaAttribute.txt";

	infile.open(path1);   //���ļ����������ļ��������� 
	//assert(infile.is_open());   //��ʧ��,�����������Ϣ,����ֹ�������� 
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
		infile.close();             //�ر��ļ������� 
	}

	return l_map;
}
//��txt��ȡ���ǵ��غ�������Ϣ
map<string, string> CPopEditorialClass::GetSatelliteInform_2()
{
	map<string, string> l_map;

	ifstream infile;
	string path1 = "D:\\DATA1\\Demo\\SatellitePayloadAttribute.txt";

	infile.open(path1);   //���ļ����������ļ��������� 
	//assert(infile.is_open());   //��ʧ��,�����������Ϣ,����ֹ�������� 
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
		infile.close();             //�ر��ļ������� 
	}

	return l_map;
}
//��txt��ȡ���Ÿ���Ϣ
map<string, string> CPopEditorialClass::GetTeleport()
{
	map<string, string> l_map;

	ifstream infile;
	string path1 = "D:\\DATA1\\Demo\\Teleport.txt";

	infile.open(path1);   //���ļ����������ļ��������� 
	//assert(infile.is_open());   //��ʧ��,�����������Ϣ,����ֹ�������� 
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
		infile.close();             //�ر��ļ������� 
	}

	return l_map;
}
//��txt��ȡ����վ
map<string, string> CPopEditorialClass::GetEarthStation()
{
	map<string, string> l_map;

	ifstream infile;
	string path1 = "D:\\DATA1\\Demo\\EarthStation.txt";

	infile.open(path1);   //���ļ����������ļ��������� 
	//assert(infile.is_open());   //��ʧ��,�����������Ϣ,����ֹ�������� 
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
		infile.close();             //�ر��ļ������� 
	}

	return l_map;
}
//��txt��ȡ����վ��Դ������Ϣ tab2
map<string, string> CPopEditorialClass::GetEarthStation_1()
{
	map<string, string> l_map;

	ifstream infile;
	string path1 = "D:\\DATA1\\Demo\\EarthStationCollapse.txt";

	infile.open(path1);   //���ļ����������ļ��������� 
	//assert(infile.is_open());   //��ʧ��,�����������Ϣ,����ֹ�������� 
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
		infile.close();             //�ر��ļ������� 
	}

	return l_map;
}
//��txt��ȡ����վ��ȡ�����豸������Ϣ tab2
map<string, string> CPopEditorialClass::GetEarthStation_2()
{
	map<string, string> l_map;

	ifstream infile;
	string path1 = "D:\\DATA1\\Demo\\EarthStationWaveform.txt";

	infile.open(path1);   //���ļ����������ļ��������� 
	//assert(infile.is_open());   //��ʧ��,�����������Ϣ,����ֹ�������� 
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
		infile.close();             //�ر��ļ������� 
	}

	return l_map;
}
//��txt��ȡ����վ����������Ϣ tab2
map<string, string> CPopEditorialClass::GetEarthStation_3()
{
	map<string, string> l_map;

	ifstream infile;
	string path1 = "D:\\DATA1\\Demo\\EarthStationAntenna.txt";

	infile.open(path1);   //���ļ����������ļ��������� 
	//assert(infile.is_open());   //��ʧ��,�����������Ϣ,����ֹ�������� 
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
		infile.close();             //�ر��ļ������� 
	}

	return l_map;
}

//��txt��ȡ�����ն˵���Ϣ
map<string, string> CPopEditorialClass::GetGroundTerminal()
{
	map<string, string> l_map;

	ifstream infile;
	string path1 = "D:\\DATA1\\Demo\\GroundTerminal.txt";

	infile.open(path1);   //���ļ����������ļ��������� 
	//assert(infile.is_open());   //��ʧ��,�����������Ϣ,����ֹ�������� 
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
		infile.close();             //�ر��ļ������� 
	}

	return l_map;
}
//��txt��ȡ�����ն����غ�������Ϣ
map<string, string> CPopEditorialClass::GetGroundTerminal_1()
{
	map<string, string> l_map;

	ifstream infile;
	string path1 = "D:\\DATA1\\Demo\\GroundTerminalLoad.txt";

	infile.open(path1);   //���ļ����������ļ��������� 
	//assert(infile.is_open());   //��ʧ��,�����������Ϣ,����ֹ�������� 
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
		infile.close();             //�ر��ļ������� 
	}

	return l_map;
}
//��txt��ȡ�����ն��в����豸������Ϣ
map<string, string> CPopEditorialClass::GetGroundTerminal_2()
{
	map<string, string> l_map;

	ifstream infile;
	string path1 = "D:\\DATA1\\Demo\\GroundTerminalWaveform.txt";//

	infile.open(path1);   //���ļ����������ļ��������� 
	//assert(infile.is_open());   //��ʧ��,�����������Ϣ,����ֹ�������� 
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
		infile.close();             //�ر��ļ������� 
	}

	return l_map;
}
//��txt��ȡ�����ն�������������Ϣ
map<string, string> CPopEditorialClass::GetGroundTerminal_3()
{
	map<string, string> l_map;

	ifstream infile;
	string path1 = "D:\\DATA1\\Demo\\GroundTerminalAntenna.txt";//GroundTerminalAntenna.txt 

	infile.open(path1);   //���ļ����������ļ��������� 
	//assert(infile.is_open());   //��ʧ��,�����������Ϣ,����ֹ�������� 
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
		infile.close();             //�ر��ļ������� 
	}

	return l_map;
}
//�ƶ����ǽڵ� level 1λ�������� level2λ��������
void CPopEditorialClass::Movelevel_2(string level1, CDuiString level2)//level1ΪҪ�ƶ��������������� level2ΪҪ�ƶ�������
{	
	vector<string> vec_child;//���Ҫ�ƶ��ĺ��ӽڵ�
	string str_nameP;//��ȡlevel2���ڸ��ڵ�����
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
			sequences1 = i;//ѡ�����������name_Level2�Լ�m_RecvMap�е�λ��
			CTreeNodeUI*  node = pNode_3[i]->GetParentNode();//GetParentNode
			wstr_nameP = node->GetItemText();
			str_nameP = wstringToString(wstr_nameP);
			childNum = pNode_3[i]->GetCountChild();//��ȡҪ�ƶ������ǽڵ��ӽڵ�����
			
		}
		/******************��ȡ���ǰ����ӽڵ�����****************/
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
	/****************��ȡ�����ӽڵ�����name_Level3�е�˳��***************/
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
	/**********************��ȡ���Ǹ��ڵ�ͽ�Ҫ�ƶ����ĸ��ڵ����������е�λ��*************************/
	int sequences = 0 ;
	int sequencesFrom = 0;
	for (int i = 0; i < name_Level1.size(); i++)
	{
		wstring wstr_name;
		wstr_name = pNode_2[i]->GetItemText();
		string str_name = wstringToString(wstr_name);
		if (level1 == str_name)
		{
			sequences = i;//����Ҫ�ƶ���Ŀ�������ڵ���pNode_2�е�λ��
		}
		if (str_nameP == str_name)
		{
			sequencesFrom = i;//���Ǳ������������ڵ���pNode_2�е�λ��
		}

	}
	/********************ɾ�����ǰ������ӽڵ�*********************/
	for (int i = 0; i < sequences_0.size(); i++)
	{
		cout << sequencesFrom << endl;
		pNode_3[sequences1]->Remove(pNode_4[sequences_0[i]]);
	}
	/********************������ԭ���ĸ��ڵ�ɾ�����ǽڵ� �½����ǽڵ����ƶ���Ŀ�길�ڵ�*********************/
	cout << sequencesFrom << endl;
	pNode_2[sequencesFrom]->Remove(pNode_3[sequences1]);
	pNode_3[sequences1] = new CTreeNodeUI();
	pNode_3[sequences1]->CheckBoxSelected(false);
	pNode_3[sequences1]->SetItemText(level2);
	pNode_3[sequences1]->SetFixedHeight(30);
	pNode_3[sequences1]->SetItemTextColor(0xFFFFFFFF);
	pNode_3[sequences1]->SetItemHotTextColor(0xFFFFFFFF);
	pNode_3[sequences1]->SetSelItemTextColor(0xFFFFFFFF);
	pNode_3[sequences1]->SetAttribute(_T("folderattr"), _T("padding=&quot;0,1,0,0&quot; width=&quot;16&quot; height=&quot;16&quot; normalimage=&quot;file='ui/չ��.png' &quot; "));
	pNode_3[sequences1]->SetAttribute(_T("folderattr"), _T("hotimage=&quot;file='ui/չ��.png' &quot; selectedimage=&quot;file='ui/δչ��.png'&quot; selectedhotimage=&quot;file='ui/δչ��.png'&quot;"));
	pNode_3[sequences1]->SetAttribute(_T("itemattr"), _T("padding=\"16,0,0,0\" align=\"left\" font=\"13\""));//font=&quot;6&quot;
	pNode_3[sequences1]->SetName(_T("WX1"));
	pNode_2[sequences]->AddChildNode(pNode_3[sequences1]);

	/************************�½������ӽڵ�*********************/
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
		pNode_4[sequences_0[i]]->SetAttribute(_T("folderattr"), _T("padding=&quot;0,1,0,0&quot; width=&quot;16&quot; height=&quot;16&quot; normalimage=&quot;file='ui/չ��.png' &quot; "));
		pNode_4[sequences_0[i]]->SetAttribute(_T("folderattr"), _T("hotimage=&quot;file='ui/չ��.png' &quot; selectedimage=&quot;file='ui/δչ��.png'&quot; selectedhotimage=&quot;file='ui/δչ��.png'&quot;"));
		pNode_4[sequences_0[i]]->SetAttribute(_T("itemattr"), _T("padding=\"16,0,0,0\" align=\"left\" font=\"13\""));//font=&quot;6&quot;
		pNode_4[sequences_0[i]]->SetName(_T("WX1"));
		pNode_3[sequences1]->AddChildNode(pNode_4[sequences_0[i]]);
	}
}
//���������
void  CPopEditorialClass::Categorize_XZ_RailType()
{
	vector<vector<string>> vecVec_WxChildName;
	int wxChildSeuque = 0;//�����ӽڵ�˳��
	for (int i = 0; i < name_Level1.size(); i++)
	{
		CDuiString wstr_name = pNode_2[i]->GetItemText();
		string str_name = wstringToString(wstr_name.GetData());
		vector<string> childName;
		//vector<int> 
		/***************��ȡ�����������������********************/
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
		/***************�Ƴ��������1��(�����㼶)�ڵ�******************/
		pNode_1[0]->Remove(pNode_2[i]);
		/***************�½�ɸѡ���2���ڵ�******************/
		pNode_2[i] = new CTreeNodeUI();
		pNode_2[i]->CheckBoxSelected(false);
		pNode_2[i]->SetItemText(stringToWstring(str_name).c_str());
		pNode_2[i]->SetFixedHeight(30);
		pNode_2[i]->SetItemTextColor(0xFFFFFFFF);
		pNode_2[i]->SetItemHotTextColor(0xFFFFFFFF);
		pNode_2[i]->SetSelItemTextColor(0xFFFFFFFF);
		pNode_2[i]->SetAttribute(_T("folderattr"), _T("padding=&quot;0,1,0,0&quot; width=&quot;16&quot; height=&quot;16&quot; normalimage=&quot;file='ui/չ��.png' &quot; "));
		pNode_2[i]->SetAttribute(_T("folderattr"), _T("hotimage=&quot;file='ui/չ��.png' &quot; selectedimage=&quot;file='ui/δչ��.png'&quot; selectedhotimage=&quot;file='ui/δչ��.png'&quot;"));
		pNode_2[i]->SetAttribute(_T("itemattr"), _T("padding=\"16,0,0,0\" align=\"left\" font=\"13\""));//font=&quot;6&quot;
		pNode_2[i]->SetName(_T("WX1"));
		map<string, string> curr_map = m_RecvMap_XZ[i];
		if (curr_map.count("������") > 0)
		{
			string value = curr_map["������"];
			int sequence = GetNoodLevel(UTF8_To_string(value));
			if (sequence >= 0)
			{
				pNode_1[sequence]->AddChildNode(pNode_2[i]);
			}
			else
			{
				CreateTreeNode(UTF8_To_string(value));//����ڵ������
				pNode_1[pNode_1_num - 1]->AddChildNode(pNode_2[i]);
			}

		}
		for (int m = 0; m < childNum; m++)//��������
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
					pNode_3[n]->SetAttribute(_T("folderattr"), _T("padding=&quot;0,1,0,0&quot; width=&quot;16&quot; height=&quot;16&quot; normalimage=&quot;file='ui/չ��.png' &quot; "));
					pNode_3[n]->SetAttribute(_T("folderattr"), _T("hotimage=&quot;file='ui/չ��.png' &quot; selectedimage=&quot;file='ui/δչ��.png'&quot; selectedhotimage=&quot;file='ui/δչ��.png'&quot;"));
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
					pNode_4[n * 2]->SetAttribute(_T("folderattr"), _T("padding=&quot;0,1,0,0&quot; width=&quot;16&quot; height=&quot;16&quot; normalimage=&quot;file='ui/չ��.png' &quot; "));
					pNode_4[n * 2]->SetAttribute(_T("folderattr"), _T("hotimage=&quot;file='ui/չ��.png' &quot; selectedimage=&quot;file='ui/δչ��.png'&quot; selectedhotimage=&quot;file='ui/δչ��.png'&quot;"));
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
					pNode_4[n * 2 + 1]->SetAttribute(_T("folderattr"), _T("padding=&quot;0,1,0,0&quot; width=&quot;16&quot; height=&quot;16&quot; normalimage=&quot;file='ui/չ��.png' &quot; "));
					pNode_4[n * 2 + 1]->SetAttribute(_T("folderattr"), _T("hotimage=&quot;file='ui/չ��.png' &quot; selectedimage=&quot;file='ui/δչ��.png'&quot; selectedhotimage=&quot;file='ui/δչ��.png'&quot;"));
					pNode_4[n * 2 + 1]->SetAttribute(_T("itemattr"), _T("padding=\"16,0,0,0\" align=\"left\" font=\"13\""));//font=&quot;6&quot;
					pNode_4[n * 2 + 1]->SetName(_T("WX1"));
					pNode_3[n]->AddChildNode(pNode_4[n * 2 + 1]);

					wxChildSeuque += 1;
		
				}
			}
		}


	}

}
//����������ԭ
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
		pNode_2[i]->SetAttribute(_T("folderattr"), _T("padding=&quot;0,1,0,0&quot; width=&quot;16&quot; height=&quot;16&quot; normalimage=&quot;file='ui/չ��.png' &quot; "));
		pNode_2[i]->SetAttribute(_T("folderattr"), _T("hotimage=&quot;file='ui/չ��.png' &quot; selectedimage=&quot;file='ui/δչ��.png'&quot; selectedhotimage=&quot;file='ui/δչ��.png'&quot;"));
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
					pNode_3[n]->SetAttribute(_T("folderattr"), _T("padding=&quot;0,1,0,0&quot; width=&quot;16&quot; height=&quot;16&quot; normalimage=&quot;file='ui/չ��.png' &quot; "));
					pNode_3[n]->SetAttribute(_T("folderattr"), _T("hotimage=&quot;file='ui/չ��.png' &quot; selectedimage=&quot;file='ui/δչ��.png'&quot; selectedhotimage=&quot;file='ui/δչ��.png'&quot;"));
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
					pNode_4[n * 2]->SetAttribute(_T("folderattr"), _T("padding=&quot;0,1,0,0&quot; width=&quot;16&quot; height=&quot;16&quot; normalimage=&quot;file='ui/չ��.png' &quot; "));
					pNode_4[n * 2]->SetAttribute(_T("folderattr"), _T("hotimage=&quot;file='ui/չ��.png' &quot; selectedimage=&quot;file='ui/δչ��.png'&quot; selectedhotimage=&quot;file='ui/δչ��.png'&quot;"));
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
					pNode_4[n * 2 + 1]->SetAttribute(_T("folderattr"), _T("padding=&quot;0,1,0,0&quot; width=&quot;16&quot; height=&quot;16&quot; normalimage=&quot;file='ui/չ��.png' &quot; "));
					pNode_4[n * 2 + 1]->SetAttribute(_T("folderattr"), _T("hotimage=&quot;file='ui/չ��.png' &quot; selectedimage=&quot;file='ui/δչ��.png'&quot; selectedhotimage=&quot;file='ui/δչ��.png'&quot;"));
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
//��ȡ������Ľڵ���������˳��
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
//��ȡ������ĸ��ڵ�����˳��
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
//��ȡ������Ľڵ���������˳��
int CPopEditorialClass::GetNoodLevel_level2(string nodeName,int serialNumber)
{
	//��ȡ���������ڽڵ��е�˳��
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
//���ิԭ��ֻ�����ڵ�
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
//����ӽڵ�
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
//����������
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
		/***************�Ƴ��������1���ڵ�******************/
		pNode_1[0]->Remove(pNode_2[i]);
		/***************�½�ɸѡ���2���ڵ�******************/
		pNode_2[i] = new CTreeNodeUI();
		pNode_2[i]->CheckBoxSelected(false);
		pNode_2[i]->SetItemText(stringToWstring(str_name).c_str());
		pNode_2[i]->SetFixedHeight(30);
		pNode_2[i]->SetItemTextColor(0xFFFFFFFF);
		pNode_2[i]->SetItemHotTextColor(0xFFFFFFFF);
		pNode_2[i]->SetSelItemTextColor(0xFFFFFFFF);
		pNode_2[i]->SetAttribute(_T("folderattr"), _T("padding=&quot;0,1,0,0&quot; width=&quot;16&quot; height=&quot;16&quot; normalimage=&quot;file='ui/չ��.png' &quot; "));
		pNode_2[i]->SetAttribute(_T("folderattr"), _T("hotimage=&quot;file='ui/չ��.png' &quot; selectedimage=&quot;file='ui/δչ��.png'&quot; selectedhotimage=&quot;file='ui/δչ��.png'&quot;"));
		pNode_2[i]->SetAttribute(_T("itemattr"), _T("padding=\"16,0,0,0\" align=\"left\" font=\"13\""));//font=&quot;6&quot;
		pNode_2[i]->SetName(_T("WX1"));



		map<string, string> curr_map = m_RecvMap_XZ[i];


		if (curr_map.count("�������") > 0)
		{
			string value = curr_map["�������"];
			int sequence = GetNoodLevel(UTF8_To_string(value));
			if (sequence >= 0)
			{
				pNode_1[sequence]->AddChildNode(pNode_2[i]);
			}
			else
			{
				CreateTreeNode(UTF8_To_string(value));//����ڵ������
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
					pNode_3[n]->SetAttribute(_T("folderattr"), _T("padding=&quot;0,1,0,0&quot; width=&quot;16&quot; height=&quot;16&quot; normalimage=&quot;file='ui/չ��.png' &quot; "));
					pNode_3[n]->SetAttribute(_T("folderattr"), _T("hotimage=&quot;file='ui/չ��.png' &quot; selectedimage=&quot;file='ui/δչ��.png'&quot; selectedhotimage=&quot;file='ui/δչ��.png'&quot;"));
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
					pNode_4[n * 2]->SetAttribute(_T("folderattr"), _T("padding=&quot;0,1,0,0&quot; width=&quot;16&quot; height=&quot;16&quot; normalimage=&quot;file='ui/չ��.png' &quot; "));
					pNode_4[n * 2]->SetAttribute(_T("folderattr"), _T("hotimage=&quot;file='ui/չ��.png' &quot; selectedimage=&quot;file='ui/δչ��.png'&quot; selectedhotimage=&quot;file='ui/δչ��.png'&quot;"));
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
					pNode_4[n * 2 + 1]->SetAttribute(_T("folderattr"), _T("padding=&quot;0,1,0,0&quot; width=&quot;16&quot; height=&quot;16&quot; normalimage=&quot;file='ui/չ��.png' &quot; "));
					pNode_4[n * 2 + 1]->SetAttribute(_T("folderattr"), _T("hotimage=&quot;file='ui/չ��.png' &quot; selectedimage=&quot;file='ui/δչ��.png'&quot; selectedhotimage=&quot;file='ui/δչ��.png'&quot;"));
					pNode_4[n * 2 + 1]->SetAttribute(_T("itemattr"), _T("padding=\"16,0,0,0\" align=\"left\" font=\"13\""));//font=&quot;6&quot;
					pNode_4[n * 2 + 1]->SetName(_T("WX1"));
					pNode_3[n]->AddChildNode(pNode_4[n * 2 + 1]);

					wxChildSeuque += 1;
				}
			}
		}
	}
}
//�����������ԭ
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
		pNode_2[i]->SetAttribute(_T("folderattr"), _T("padding=&quot;0,1,0,0&quot; width=&quot;16&quot; height=&quot;16&quot; normalimage=&quot;file='ui/չ��.png' &quot; "));
		pNode_2[i]->SetAttribute(_T("folderattr"), _T("hotimage=&quot;file='ui/չ��.png' &quot; selectedimage=&quot;file='ui/δչ��.png'&quot; selectedhotimage=&quot;file='ui/δչ��.png'&quot;"));
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
					pNode_3[n]->SetAttribute(_T("folderattr"), _T("padding=&quot;0,1,0,0&quot; width=&quot;16&quot; height=&quot;16&quot; normalimage=&quot;file='ui/չ��.png' &quot; "));
					pNode_3[n]->SetAttribute(_T("folderattr"), _T("hotimage=&quot;file='ui/չ��.png' &quot; selectedimage=&quot;file='ui/δչ��.png'&quot; selectedhotimage=&quot;file='ui/δչ��.png'&quot;"));
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
					pNode_4[n * 2]->SetAttribute(_T("folderattr"), _T("padding=&quot;0,1,0,0&quot; width=&quot;16&quot; height=&quot;16&quot; normalimage=&quot;file='ui/չ��.png' &quot; "));
					pNode_4[n * 2]->SetAttribute(_T("folderattr"), _T("hotimage=&quot;file='ui/չ��.png' &quot; selectedimage=&quot;file='ui/δչ��.png'&quot; selectedhotimage=&quot;file='ui/δչ��.png'&quot;"));
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
					pNode_4[n * 2 + 1]->SetAttribute(_T("folderattr"), _T("padding=&quot;0,1,0,0&quot; width=&quot;16&quot; height=&quot;16&quot; normalimage=&quot;file='ui/չ��.png' &quot; "));
					pNode_4[n * 2 + 1]->SetAttribute(_T("folderattr"), _T("hotimage=&quot;file='ui/չ��.png' &quot; selectedimage=&quot;file='ui/δչ��.png'&quot; selectedhotimage=&quot;file='ui/δչ��.png'&quot;"));
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

//�����غ����ͷ���
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
		/***************�Ƴ��������1���ڵ�******************/
		pNode_1[0]->Remove(pNode_2[i]);
		/***************�½�ɸѡ���2���ڵ�******************/
		pNode_2[i] = new CTreeNodeUI();
		pNode_2[i]->CheckBoxSelected(false);
		pNode_2[i]->SetItemText(stringToWstring(str_name).c_str());
		pNode_2[i]->SetFixedHeight(30);
		pNode_2[i]->SetItemTextColor(0xFFFFFFFF);
		pNode_2[i]->SetItemHotTextColor(0xFFFFFFFF);
		pNode_2[i]->SetSelItemTextColor(0xFFFFFFFF);
		pNode_2[i]->SetAttribute(_T("folderattr"), _T("padding=&quot;0,1,0,0&quot; width=&quot;16&quot; height=&quot;16&quot; normalimage=&quot;file='ui/չ��.png' &quot; "));
		pNode_2[i]->SetAttribute(_T("folderattr"), _T("hotimage=&quot;file='ui/չ��.png' &quot; selectedimage=&quot;file='ui/δչ��.png'&quot; selectedhotimage=&quot;file='ui/δչ��.png'&quot;"));
		pNode_2[i]->SetAttribute(_T("itemattr"), _T("padding=\"16,0,0,0\" align=\"left\" font=\"13\""));//font=&quot;6&quot;
		pNode_2[i]->SetName(_T("WX1"));
		
		map<string, string> curr_map = m_RecvMap_XZ[i];
		if (curr_map.count("��Ч�غ�") > 0)
		{
			string value = curr_map["��Ч�غ�"];
			int sequence = GetNoodLevel(UTF8_To_string(value));
			if (sequence >= 0)
			{
				pNode_1[sequence]->AddChildNode(pNode_2[i]);
			}
			else
			{
				CreateTreeNode(UTF8_To_string(value));//����ڵ������
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
					pNode_3[n]->SetAttribute(_T("folderattr"), _T("padding=&quot;0,1,0,0&quot; width=&quot;16&quot; height=&quot;16&quot; normalimage=&quot;file='ui/չ��.png' &quot; "));
					pNode_3[n]->SetAttribute(_T("folderattr"), _T("hotimage=&quot;file='ui/չ��.png' &quot; selectedimage=&quot;file='ui/δչ��.png'&quot; selectedhotimage=&quot;file='ui/δչ��.png'&quot;"));
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
					pNode_4[n * 2]->SetAttribute(_T("folderattr"), _T("padding=&quot;0,1,0,0&quot; width=&quot;16&quot; height=&quot;16&quot; normalimage=&quot;file='ui/չ��.png' &quot; "));
					pNode_4[n * 2]->SetAttribute(_T("folderattr"), _T("hotimage=&quot;file='ui/չ��.png' &quot; selectedimage=&quot;file='ui/δչ��.png'&quot; selectedhotimage=&quot;file='ui/δչ��.png'&quot;"));
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
					pNode_4[n * 2 + 1]->SetAttribute(_T("folderattr"), _T("padding=&quot;0,1,0,0&quot; width=&quot;16&quot; height=&quot;16&quot; normalimage=&quot;file='ui/չ��.png' &quot; "));
					pNode_4[n * 2 + 1]->SetAttribute(_T("folderattr"), _T("hotimage=&quot;file='ui/չ��.png' &quot; selectedimage=&quot;file='ui/δչ��.png'&quot; selectedhotimage=&quot;file='ui/δչ��.png'&quot;"));
					pNode_4[n * 2 + 1]->SetAttribute(_T("itemattr"), _T("padding=\"16,0,0,0\" align=\"left\" font=\"13\""));//font=&quot;6&quot;
					pNode_4[n * 2 + 1]->SetName(_T("WX1"));
					pNode_3[n]->AddChildNode(pNode_4[n * 2 + 1]);

					wxChildSeuque += 1;
				}
			}
		}
	}
}
//�غ����ͷ����ԭ
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
		pNode_2[i]->SetAttribute(_T("folderattr"), _T("padding=&quot;0,1,0,0&quot; width=&quot;16&quot; height=&quot;16&quot; normalimage=&quot;file='ui/չ��.png' &quot; "));
		pNode_2[i]->SetAttribute(_T("folderattr"), _T("hotimage=&quot;file='ui/չ��.png' &quot; selectedimage=&quot;file='ui/δչ��.png'&quot; selectedhotimage=&quot;file='ui/δչ��.png'&quot;"));
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
					pNode_3[n]->SetAttribute(_T("folderattr"), _T("padding=&quot;0,1,0,0&quot; width=&quot;16&quot; height=&quot;16&quot; normalimage=&quot;file='ui/չ��.png' &quot; "));
					pNode_3[n]->SetAttribute(_T("folderattr"), _T("hotimage=&quot;file='ui/չ��.png' &quot; selectedimage=&quot;file='ui/δչ��.png'&quot; selectedhotimage=&quot;file='ui/δչ��.png'&quot;"));
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
					pNode_4[n * 2]->SetAttribute(_T("folderattr"), _T("padding=&quot;0,1,0,0&quot; width=&quot;16&quot; height=&quot;16&quot; normalimage=&quot;file='ui/չ��.png' &quot; "));
					pNode_4[n * 2]->SetAttribute(_T("folderattr"), _T("hotimage=&quot;file='ui/չ��.png' &quot; selectedimage=&quot;file='ui/δչ��.png'&quot; selectedhotimage=&quot;file='ui/δչ��.png'&quot;"));
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
					pNode_4[n * 2 + 1]->SetAttribute(_T("folderattr"), _T("padding=&quot;0,1,0,0&quot; width=&quot;16&quot; height=&quot;16&quot; normalimage=&quot;file='ui/չ��.png' &quot; "));
					pNode_4[n * 2 + 1]->SetAttribute(_T("folderattr"), _T("hotimage=&quot;file='ui/չ��.png' &quot; selectedimage=&quot;file='ui/δչ��.png'&quot; selectedhotimage=&quot;file='ui/δչ��.png'&quot;"));
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
//�鿴��ǰ�Ƿ��������ڷ���
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
//�鿴��ǰ�Ƿ��е��Ÿ��ڷ���
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
//����-----�����غɹ��ܷ���
void CPopEditorialClass::Categorize_WX_LoadFunction(string name)
{
	//string str = name;
	vector<string> childName;//����ӽڵ�����
	vector<vector<string>> vec_wxchildName;//��������ӽڵ�����
	vector<int> childSerialNum;//�ӽڵ�����pNode_3�е����
	int serialNumber;//���ѡ�����������
	CTreeNodeUI* pNode[1024];//����ɸѡ��ʱ��������ڵ� ������͵�
	CTreeNodeUI* pNode_T4[1024];//����Ĳ�ڵ�

	CTreeNodeUI* pNode_T5[1024];//������ǽڵ��ӽڵ�
	int level_4 = 0;	
	int level_5 = 0;
	/*******************ɸѡ���������ڴ���������Ƶ�vector�е�λ��***************/
	for (int i = 0; i < name_Level1.size(); i++)
	{
		if (name == name_Level1[i])
		{
			serialNumber = i;
		}
	}
	int childNum = pNode_2[serialNumber]->GetCountChild();
	/**********************ɸѡ��ǰ�����������������***********************/
	for (int j = 0; j < childNum; j++)
	{
		CDuiString wstr_name1 = pNode_2[serialNumber]->GetChildNode(j)->GetItemText();
		string str_name1 = wstringToString(wstr_name1.GetData());
		childName.push_back(str_name1);
		/******************ɸѡ��ǰ��������������pNode_3�е�˳��***********/
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
	/***********************ɸѡ����������ӽڵ�����*****************************/
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
	/***********************ɾ������������ӽڵ�******************************/
	for (int j = 0; j < childSerialNum.size(); j++)
	{
		pNode_3[childSerialNum[j]]->Remove(pNode_4[childSerialNum[j]*2+1]);
		pNode_3[childSerialNum[j]]->Remove(pNode_4[childSerialNum[j] * 2]);
	}
	/***********************ɾ�������µ�����***********************/
	for (int j = 0; j < childSerialNum.size(); j++)
	{
		pNode_2[serialNumber]->Remove(pNode_3[childSerialNum[j]]);
	}
	/******************************************************/
	for (int j = 0; j < childSerialNum.size(); j++)
	{
		map<string, string> curr_map = m_RecvMap[j];

		if (curr_map.count("�����غ�����") > 0)
		{
			string value = curr_map["�����غ�����"];
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
				pNode_T4[level_4]->SetAttribute(_T("folderattr"), _T("padding=&quot;0,1,0,0&quot; width=&quot;16&quot; height=&quot;16&quot; normalimage=&quot;file='ui/չ��.png' &quot; "));
				pNode_T4[level_4]->SetAttribute(_T("folderattr"), _T("hotimage=&quot;file='ui/չ��.png' &quot; selectedimage=&quot;file='ui/δչ��.png'&quot; selectedhotimage=&quot;file='ui/δչ��.png'&quot;"));
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
				pNode_T5[level_5]->SetAttribute(_T("folderattr"), _T("padding=&quot;0,1,0,0&quot; width=&quot;16&quot; height=&quot;16&quot; normalimage=&quot;file='ui/չ��.png' &quot; "));
				pNode_T5[level_5]->SetAttribute(_T("folderattr"), _T("hotimage=&quot;file='ui/չ��.png' &quot; selectedimage=&quot;file='ui/δչ��.png'&quot; selectedhotimage=&quot;file='ui/δչ��.png'&quot;"));
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
				pNode_T5[level_5]->SetAttribute(_T("folderattr"), _T("padding=&quot;0,1,0,0&quot; width=&quot;16&quot; height=&quot;16&quot; normalimage=&quot;file='ui/չ��.png' &quot; "));
				pNode_T5[level_5]->SetAttribute(_T("folderattr"), _T("hotimage=&quot;file='ui/չ��.png' &quot; selectedimage=&quot;file='ui/δչ��.png'&quot; selectedhotimage=&quot;file='ui/δչ��.png'&quot;"));
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
				pNode[j]->SetAttribute(_T("folderattr"), _T("padding=&quot;0,1,0,0&quot; width=&quot;16&quot; height=&quot;16&quot; normalimage=&quot;file='ui/չ��.png' &quot; "));
				pNode[j]->SetAttribute(_T("folderattr"), _T("hotimage=&quot;file='ui/չ��.png' &quot; selectedimage=&quot;file='ui/δչ��.png'&quot; selectedhotimage=&quot;file='ui/δչ��.png'&quot;"));
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
				pNode_T4[level_4]->SetAttribute(_T("folderattr"), _T("padding=&quot;0,1,0,0&quot; width=&quot;16&quot; height=&quot;16&quot; normalimage=&quot;file='ui/չ��.png' &quot; "));
				pNode_T4[level_4]->SetAttribute(_T("folderattr"), _T("hotimage=&quot;file='ui/չ��.png' &quot; selectedimage=&quot;file='ui/δչ��.png'&quot; selectedhotimage=&quot;file='ui/δչ��.png'&quot;"));
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
				pNode_T5[level_5]->SetAttribute(_T("folderattr"), _T("padding=&quot;0,1,0,0&quot; width=&quot;16&quot; height=&quot;16&quot; normalimage=&quot;file='ui/չ��.png' &quot; "));
				pNode_T5[level_5]->SetAttribute(_T("folderattr"), _T("hotimage=&quot;file='ui/չ��.png' &quot; selectedimage=&quot;file='ui/δչ��.png'&quot; selectedhotimage=&quot;file='ui/δչ��.png'&quot;"));
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
				pNode_T5[level_5]->SetAttribute(_T("folderattr"), _T("padding=&quot;0,1,0,0&quot; width=&quot;16&quot; height=&quot;16&quot; normalimage=&quot;file='ui/չ��.png' &quot; "));
				pNode_T5[level_5]->SetAttribute(_T("folderattr"), _T("hotimage=&quot;file='ui/չ��.png' &quot; selectedimage=&quot;file='ui/δչ��.png'&quot; selectedhotimage=&quot;file='ui/δչ��.png'&quot;"));
				pNode_T5[level_5]->SetAttribute(_T("itemattr"), _T("padding=\"16,0,0,0\" align=\"left\" font=\"13\""));//font=&quot;6&quot;
				pNode_T5[level_5]->SetName(_T("WX1"));

				pNode_T4[level_4]->AddChildNode(pNode_T5[level_5]);
				level_5 += 1;
				level_4 += 1;

			}

		}
	}

}
//����------���غɹ��ܷ����ԭ
void CPopEditorialClass::Restore_WX_LoadFunction(string name)
{
	//string str = name;
	vector<string> childName;//����ӽڵ�����
	vector<vector<string>> vec_wxchildName;//��������ӽڵ�����
	vector<int> childSerialNum;//�ӽڵ�����pNode_3�е����
	int serialNumber;//���ѡ�����������
	CTreeNodeUI* pNode[1024];
	int serialNum_wx;
	/*******************ɸѡ���������ڴ���������Ƶ�vector�е�λ��***************/
	for (int i = 0; i < name_Level1.size(); i++)
	{
		if (name == name_Level1[i])
		{
			serialNumber = i;
		}
		//pNode_2[i]
	}
	/*************************ɸѡ�ӽڵ���ӽڵ��������������*********************************/
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
			childName.push_back(str_name);//��ŵ�������
			int wxChildNum = pNode_level3->GetChildNode(j)->GetCountChild();
			for (int m = 0; m < wxChildNum; m++)
			{
				CDuiString cdui_name1 = pNode_level3->GetChildNode(j)->GetChildNode(m)->GetItemText();
				string str_name1 = wstringToString(cdui_name1.GetData());
				vec.push_back(str_name1);//��ŵ�������
			}
			vec_wxchildName.push_back(vec);
		}
	}
	/********************ɾ��2���ڵ�******************/
	for (int i = childNum; i > 0; i--)
	{
		pNode_2[serialNumber]->Remove(pNode_2[serialNumber]->GetChildNode(i-1));
	}
	/*********************�½��ָ�Ϊԭ��������**********************/
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
				pNode_3[serialNum_wx]->SetAttribute(_T("folderattr"), _T("padding=&quot;0,1,0,0&quot; width=&quot;16&quot; height=&quot;16&quot; normalimage=&quot;file='ui/չ��.png' &quot; "));
				pNode_3[serialNum_wx]->SetAttribute(_T("folderattr"), _T("hotimage=&quot;file='ui/չ��.png' &quot; selectedimage=&quot;file='ui/δչ��.png'&quot; selectedhotimage=&quot;file='ui/δչ��.png'&quot;"));
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
					pNode_4[serialNum_wx * 2]->SetAttribute(_T("folderattr"), _T("padding=&quot;0,1,0,0&quot; width=&quot;16&quot; height=&quot;16&quot; normalimage=&quot;file='ui/չ��.png' &quot; "));
					pNode_4[serialNum_wx * 2]->SetAttribute(_T("folderattr"), _T("hotimage=&quot;file='ui/չ��.png' &quot; selectedimage=&quot;file='ui/δչ��.png'&quot; selectedhotimage=&quot;file='ui/δչ��.png'&quot;"));
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
					pNode_4[serialNum_wx * 2 + 1]->SetAttribute(_T("folderattr"), _T("padding=&quot;0,1,0,0&quot; width=&quot;16&quot; height=&quot;16&quot; normalimage=&quot;file='ui/չ��.png' &quot; "));
					pNode_4[serialNum_wx * 2 + 1]->SetAttribute(_T("folderattr"), _T("hotimage=&quot;file='ui/չ��.png' &quot; selectedimage=&quot;file='ui/δչ��.png'&quot; selectedhotimage=&quot;file='ui/δչ��.png'&quot;"));
					pNode_4[serialNum_wx * 2 + 1]->SetAttribute(_T("itemattr"), _T("padding=\"16,0,0,0\" align=\"left\" font=\"13\""));//font=&quot;6&quot;
					pNode_4[serialNum_wx * 2 + 1]->SetName(_T("WX1"));
					pNode_3[serialNum_wx]->AddChildNode(pNode_4[serialNum_wx * 2 + 1]);
				//}
					m += 1;
			}
	
			
		}
	}
}
//���Ÿ۰����ʷ���
void CPopEditorialClass::Categorize_DXG_characteristic()
{
	vector<vector<string>> vecVec_WxChildName;
	int wxChildSeuque = 0;//�����ӽڵ�˳��
	for (int i = 0; i < name2_Level1.size(); i++)
	{
		CDuiString wstr_name = pNode_2_2[i]->GetItemText();
		string str_name = wstringToString(wstr_name.GetData());
		vector<string> childName;
		//vector<int> 
		/***************��ȡ�����������������********************/
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
		/***************�Ƴ��������1��(�����㼶)�ڵ�******************/
		pNode_2_1[0]->Remove(pNode_2_2[i]);
		/***************�½�ɸѡ���2���ڵ�******************/
		pNode_2_2[i] = new CTreeNodeUI();
		pNode_2_2[i]->CheckBoxSelected(false);
		pNode_2_2[i]->SetItemText(stringToWstring(str_name).c_str());
		pNode_2_2[i]->SetFixedHeight(30);
		pNode_2_2[i]->SetItemTextColor(0xFFFFFFFF);
		pNode_2_2[i]->SetItemHotTextColor(0xFFFFFFFF);
		pNode_2_2[i]->SetSelItemTextColor(0xFFFFFFFF);
		pNode_2_2[i]->SetAttribute(_T("folderattr"), _T("padding=&quot;0,1,0,0&quot; width=&quot;16&quot; height=&quot;16&quot; normalimage=&quot;file='ui/չ��.png' &quot; "));
		pNode_2_2[i]->SetAttribute(_T("folderattr"), _T("hotimage=&quot;file='ui/չ��.png' &quot; selectedimage=&quot;file='ui/δչ��.png'&quot; selectedhotimage=&quot;file='ui/δչ��.png'&quot;"));
		pNode_2_2[i]->SetAttribute(_T("itemattr"), _T("padding=\"16,0,0,0\" align=\"left\" font=\"13\""));//font=&quot;6&quot;
		pNode_2_2[i]->SetName(_T("WX1"));
		map<string, string> curr_map = m_RecvMap_DXG[i];
		if (curr_map.count("���Ÿ�����") > 0)
		{
			string value = curr_map["���Ÿ�����"];
			int sequence = GetNoodLevel_DXG_XZ(UTF8_To_string(value));
			if (sequence >= 0)
			{
				pNode_2_1[sequence]->AddChildNode(pNode_2_2[i]);
			}
			else
			{
				CreateTreeNode_2(UTF8_To_string(value));//����ڵ������
				pNode_2_1[pNode_2_1_num - 1]->AddChildNode(pNode_2_2[i]);
			}

		}
		for (int m = 0; m < childNum; m++)//��������
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
					pNode_2_3[n]->SetAttribute(_T("folderattr"), _T("padding=&quot;0,1,0,0&quot; width=&quot;16&quot; height=&quot;16&quot; normalimage=&quot;file='ui/չ��.png' &quot; "));
					pNode_2_3[n]->SetAttribute(_T("folderattr"), _T("hotimage=&quot;file='ui/չ��.png' &quot; selectedimage=&quot;file='ui/δչ��.png'&quot; selectedhotimage=&quot;file='ui/δչ��.png'&quot;"));
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
					pNode_2_4[n * 2]->SetAttribute(_T("folderattr"), _T("padding=&quot;0,1,0,0&quot; width=&quot;16&quot; height=&quot;16&quot; normalimage=&quot;file='ui/չ��.png' &quot; "));
					pNode_2_4[n * 2]->SetAttribute(_T("folderattr"), _T("hotimage=&quot;file='ui/չ��.png' &quot; selectedimage=&quot;file='ui/δչ��.png'&quot; selectedhotimage=&quot;file='ui/δչ��.png'&quot;"));
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
					pNode_2_4[n * 2 + 1]->SetAttribute(_T("folderattr"), _T("padding=&quot;0,1,0,0&quot; width=&quot;16&quot; height=&quot;16&quot; normalimage=&quot;file='ui/չ��.png' &quot; "));
					pNode_2_4[n * 2 + 1]->SetAttribute(_T("folderattr"), _T("hotimage=&quot;file='ui/չ��.png' &quot; selectedimage=&quot;file='ui/δչ��.png'&quot; selectedhotimage=&quot;file='ui/δչ��.png'&quot;"));
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
					pNode_2_4[n * 2 + 2]->SetAttribute(_T("folderattr"), _T("padding=&quot;0,1,0,0&quot; width=&quot;16&quot; height=&quot;16&quot; normalimage=&quot;file='ui/չ��.png' &quot; "));
					pNode_2_4[n * 2 + 2]->SetAttribute(_T("folderattr"), _T("hotimage=&quot;file='ui/չ��.png' &quot; selectedimage=&quot;file='ui/δչ��.png'&quot; selectedhotimage=&quot;file='ui/δչ��.png'&quot;"));
					pNode_2_4[n * 2 + 2]->SetAttribute(_T("itemattr"), _T("padding=\"16,0,0,0\" align=\"left\" font=\"13\""));//font=&quot;6&quot;
					pNode_2_4[n * 2 + 2]->SetName(_T("WX1"));
					pNode_2_3[n]->AddChildNode(pNode_2_4[n * 2 + 2]);

					wxChildSeuque += 1;

				}
			}
		}


	}

}
//�������ʷ����ԭ
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
		pNode_2_2[i]->SetAttribute(_T("folderattr"), _T("padding=&quot;0,1,0,0&quot; width=&quot;16&quot; height=&quot;16&quot; normalimage=&quot;file='ui/չ��.png' &quot; "));
		pNode_2_2[i]->SetAttribute(_T("folderattr"), _T("hotimage=&quot;file='ui/չ��.png' &quot; selectedimage=&quot;file='ui/δչ��.png'&quot; selectedhotimage=&quot;file='ui/δչ��.png'&quot;"));
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
					pNode_2_3[n]->SetAttribute(_T("folderattr"), _T("padding=&quot;0,1,0,0&quot; width=&quot;16&quot; height=&quot;16&quot; normalimage=&quot;file='ui/չ��.png' &quot; "));
					pNode_2_3[n]->SetAttribute(_T("folderattr"), _T("hotimage=&quot;file='ui/չ��.png' &quot; selectedimage=&quot;file='ui/δչ��.png'&quot; selectedhotimage=&quot;file='ui/δչ��.png'&quot;"));
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
					pNode_2_4[n * 2]->SetAttribute(_T("folderattr"), _T("padding=&quot;0,1,0,0&quot; width=&quot;16&quot; height=&quot;16&quot; normalimage=&quot;file='ui/չ��.png' &quot; "));
					pNode_2_4[n * 2]->SetAttribute(_T("folderattr"), _T("hotimage=&quot;file='ui/չ��.png' &quot; selectedimage=&quot;file='ui/δչ��.png'&quot; selectedhotimage=&quot;file='ui/δչ��.png'&quot;"));
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
					pNode_2_4[n * 2 + 1]->SetAttribute(_T("folderattr"), _T("padding=&quot;0,1,0,0&quot; width=&quot;16&quot; height=&quot;16&quot; normalimage=&quot;file='ui/չ��.png' &quot; "));
					pNode_2_4[n * 2 + 1]->SetAttribute(_T("folderattr"), _T("hotimage=&quot;file='ui/չ��.png' &quot; selectedimage=&quot;file='ui/δչ��.png'&quot; selectedhotimage=&quot;file='ui/δչ��.png'&quot;"));
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
					pNode_2_4[n * 2 + 2]->SetAttribute(_T("folderattr"), _T("padding=&quot;0,1,0,0&quot; width=&quot;16&quot; height=&quot;16&quot; normalimage=&quot;file='ui/չ��.png' &quot; "));
					pNode_2_4[n * 2 + 2]->SetAttribute(_T("folderattr"), _T("hotimage=&quot;file='ui/չ��.png' &quot; selectedimage=&quot;file='ui/δչ��.png'&quot; selectedhotimage=&quot;file='ui/δչ��.png'&quot;"));
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
//��ȡ���Ÿ����ʵĽڵ���������˳��
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
//����Ÿ����ʵĸ��ڵ�����˳��
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
//���ิԭ��ֻ�����ڵ�
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
//����վ-----�����غɹ��ܷ���
void CPopEditorialClass::Categorize_DQZ_FunctionType1(string name)
{
	//string str = name;
	vector<string> childName;//����ӽڵ�����
	vector<vector<string>> vec_wxchildName;//��ŵ���վ(����)�ӽڵ�����
	vector<int> childSerialNum;//�ӽڵ�����pNode_3�е����
	int serialNumber;//���ѡ��ĵ��Ÿ�����
	CTreeNodeUI* pNode[1024];//����ɸѡ��ʱ��������ڵ� ������͵�
	CTreeNodeUI* pNode_T4[1024];//����Ĳ�ڵ�

	CTreeNodeUI* pNode_T5[1024];//������ǽڵ��ӽڵ�
	int level_4 = 0;
	int level_5 = 0;
	/*******************ɸѡ���������ڴ���������Ƶ�vector�е�λ��***************/
	for (int i = 0; i < name2_Level1.size(); i++)
	{
		if (name == name2_Level1[i])
		{
			serialNumber = i;
		}
	}
	int childNum = pNode_2_2[serialNumber]->GetCountChild();
	/**********************ɸѡ��ǰ�����������������***********************/
	for (int j = 0; j < childNum; j++)
	{
		CDuiString wstr_name1 = pNode_2_2[serialNumber]->GetChildNode(j)->GetItemText();
		string str_name1 = wstringToString(wstr_name1.GetData());
		childName.push_back(str_name1);
		/******************ɸѡ��ǰ��������������pNode_3�е�˳��***********/
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
	/***********************ɸѡ����������ӽڵ�����*****************************/
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
	/***********************ɾ������������ӽڵ�******************************/
	for (int j = 0; j < childSerialNum.size(); j++)
	{
		pNode_2_3[childSerialNum[j]]->Remove(pNode_2_4[childSerialNum[j] * 2 + 2]);
		pNode_2_3[childSerialNum[j]]->Remove(pNode_2_4[childSerialNum[j] * 2 + 1]);
		pNode_2_3[childSerialNum[j]]->Remove(pNode_2_4[childSerialNum[j] * 2]);
	}
	/***********************ɾ�������µ�����***********************/
	for (int j = 0; j < childSerialNum.size(); j++)
	{
		pNode_2_2[serialNumber]->Remove(pNode_2_3[childSerialNum[j]]);
	}
	/******************************************************/
	for (int j = 0; j < childSerialNum.size(); j++)
	{
		map<string, string> curr_map = m_RecvMap_DQZ[j];

		if (curr_map.count("��������") > 0)
		{
			string value = curr_map["��������"];
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
				pNode_T4[level_4]->SetAttribute(_T("folderattr"), _T("padding=&quot;0,1,0,0&quot; width=&quot;16&quot; height=&quot;16&quot; normalimage=&quot;file='ui/չ��.png' &quot; "));
				pNode_T4[level_4]->SetAttribute(_T("folderattr"), _T("hotimage=&quot;file='ui/չ��.png' &quot; selectedimage=&quot;file='ui/δչ��.png'&quot; selectedhotimage=&quot;file='ui/δչ��.png'&quot;"));
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
				pNode_T5[level_5]->SetAttribute(_T("folderattr"), _T("padding=&quot;0,1,0,0&quot; width=&quot;16&quot; height=&quot;16&quot; normalimage=&quot;file='ui/չ��.png' &quot; "));
				pNode_T5[level_5]->SetAttribute(_T("folderattr"), _T("hotimage=&quot;file='ui/չ��.png' &quot; selectedimage=&quot;file='ui/δչ��.png'&quot; selectedhotimage=&quot;file='ui/δչ��.png'&quot;"));
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
				pNode_T5[level_5]->SetAttribute(_T("folderattr"), _T("padding=&quot;0,1,0,0&quot; width=&quot;16&quot; height=&quot;16&quot; normalimage=&quot;file='ui/չ��.png' &quot; "));
				pNode_T5[level_5]->SetAttribute(_T("folderattr"), _T("hotimage=&quot;file='ui/չ��.png' &quot; selectedimage=&quot;file='ui/δչ��.png'&quot; selectedhotimage=&quot;file='ui/δչ��.png'&quot;"));
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
				pNode_T5[level_5]->SetAttribute(_T("folderattr"), _T("padding=&quot;0,1,0,0&quot; width=&quot;16&quot; height=&quot;16&quot; normalimage=&quot;file='ui/չ��.png' &quot; "));
				pNode_T5[level_5]->SetAttribute(_T("folderattr"), _T("hotimage=&quot;file='ui/չ��.png' &quot; selectedimage=&quot;file='ui/δչ��.png'&quot; selectedhotimage=&quot;file='ui/δչ��.png'&quot;"));
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
				pNode[j]->SetAttribute(_T("folderattr"), _T("padding=&quot;0,1,0,0&quot; width=&quot;16&quot; height=&quot;16&quot; normalimage=&quot;file='ui/չ��.png' &quot; "));
				pNode[j]->SetAttribute(_T("folderattr"), _T("hotimage=&quot;file='ui/չ��.png' &quot; selectedimage=&quot;file='ui/δչ��.png'&quot; selectedhotimage=&quot;file='ui/δչ��.png'&quot;"));
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
				pNode_T4[level_4]->SetAttribute(_T("folderattr"), _T("padding=&quot;0,1,0,0&quot; width=&quot;16&quot; height=&quot;16&quot; normalimage=&quot;file='ui/չ��.png' &quot; "));
				pNode_T4[level_4]->SetAttribute(_T("folderattr"), _T("hotimage=&quot;file='ui/չ��.png' &quot; selectedimage=&quot;file='ui/δչ��.png'&quot; selectedhotimage=&quot;file='ui/δչ��.png'&quot;"));
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
				pNode_T5[level_5]->SetAttribute(_T("folderattr"), _T("padding=&quot;0,1,0,0&quot; width=&quot;16&quot; height=&quot;16&quot; normalimage=&quot;file='ui/չ��.png' &quot; "));
				pNode_T5[level_5]->SetAttribute(_T("folderattr"), _T("hotimage=&quot;file='ui/չ��.png' &quot; selectedimage=&quot;file='ui/δչ��.png'&quot; selectedhotimage=&quot;file='ui/δչ��.png'&quot;"));
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
				pNode_T5[level_5]->SetAttribute(_T("folderattr"), _T("padding=&quot;0,1,0,0&quot; width=&quot;16&quot; height=&quot;16&quot; normalimage=&quot;file='ui/չ��.png' &quot; "));
				pNode_T5[level_5]->SetAttribute(_T("folderattr"), _T("hotimage=&quot;file='ui/չ��.png' &quot; selectedimage=&quot;file='ui/δչ��.png'&quot; selectedhotimage=&quot;file='ui/δչ��.png'&quot;"));
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
				pNode_T5[level_5]->SetAttribute(_T("folderattr"), _T("padding=&quot;0,1,0,0&quot; width=&quot;16&quot; height=&quot;16&quot; normalimage=&quot;file='ui/չ��.png' &quot; "));
				pNode_T5[level_5]->SetAttribute(_T("folderattr"), _T("hotimage=&quot;file='ui/չ��.png' &quot; selectedimage=&quot;file='ui/δչ��.png'&quot; selectedhotimage=&quot;file='ui/δչ��.png'&quot;"));
				pNode_T5[level_5]->SetAttribute(_T("itemattr"), _T("padding=\"16,0,0,0\" align=\"left\" font=\"13\""));//font=&quot;6&quot;
				pNode_T5[level_5]->SetName(_T("WX1"));

				pNode_T4[level_4]->AddChildNode(pNode_T5[level_5]);
				level_5 += 1;
				level_4 += 1;

			}

		}
	}
}
//����վ------���غɹ��ܷ����ԭ
void CPopEditorialClass::Restore_DQZ_FunctionType1(string name)
{

	//string str = name;
	vector<string> childName;//����ӽڵ�����
	vector<vector<string>> vec_wxchildName;//��������ӽڵ�����
	vector<int> childSerialNum;//�ӽڵ�����pNode_3�е����
	int serialNumber;//���ѡ�����������
	CTreeNodeUI* pNode[1024];
	int serialNum_wx;
	/*******************ɸѡ���������ڴ���������Ƶ�vector�е�λ��***************/
	for (int i = 0; i < name2_Level1.size(); i++)
	{
		if (name == name2_Level1[i])
		{
			serialNumber = i;
		}
		//pNode_2[i]
	}
	/*************************ɸѡ�ӽڵ���ӽڵ��������������*********************************/
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
			childName.push_back(str_name);//��ŵ�������
			int wxChildNum = pNode_level3->GetChildNode(j)->GetCountChild();
			for (int m = 0; m < wxChildNum; m++)
			{
				CDuiString cdui_name1 = pNode_level3->GetChildNode(j)->GetChildNode(m)->GetItemText();
				string str_name1 = wstringToString(cdui_name1.GetData());
				vec.push_back(str_name1);//��ŵ�������
			}
			vec_wxchildName.push_back(vec);
		}
	}
	/********************ɾ��2���ڵ�******************/
	for (int i = childNum; i > 0; i--)
	{
		pNode_2_2[serialNumber]->Remove(pNode_2_2[serialNumber]->GetChildNode(i - 1));
	}
	/*********************�½��ָ�Ϊԭ��������**********************/
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
				pNode_2_3[serialNum_wx]->SetAttribute(_T("folderattr"), _T("padding=&quot;0,1,0,0&quot; width=&quot;16&quot; height=&quot;16&quot; normalimage=&quot;file='ui/չ��.png' &quot; "));
				pNode_2_3[serialNum_wx]->SetAttribute(_T("folderattr"), _T("hotimage=&quot;file='ui/չ��.png' &quot; selectedimage=&quot;file='ui/δչ��.png'&quot; selectedhotimage=&quot;file='ui/δչ��.png'&quot;"));
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
				pNode_2_4[serialNum_wx * 2]->SetAttribute(_T("folderattr"), _T("padding=&quot;0,1,0,0&quot; width=&quot;16&quot; height=&quot;16&quot; normalimage=&quot;file='ui/չ��.png' &quot; "));
				pNode_2_4[serialNum_wx * 2]->SetAttribute(_T("folderattr"), _T("hotimage=&quot;file='ui/չ��.png' &quot; selectedimage=&quot;file='ui/δչ��.png'&quot; selectedhotimage=&quot;file='ui/δչ��.png'&quot;"));
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
				pNode_2_4[serialNum_wx * 2 + 1]->SetAttribute(_T("folderattr"), _T("padding=&quot;0,1,0,0&quot; width=&quot;16&quot; height=&quot;16&quot; normalimage=&quot;file='ui/չ��.png' &quot; "));
				pNode_2_4[serialNum_wx * 2 + 1]->SetAttribute(_T("folderattr"), _T("hotimage=&quot;file='ui/չ��.png' &quot; selectedimage=&quot;file='ui/δչ��.png'&quot; selectedhotimage=&quot;file='ui/δչ��.png'&quot;"));
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
				pNode_2_4[serialNum_wx * 2 + 2]->SetAttribute(_T("folderattr"), _T("padding=&quot;0,1,0,0&quot; width=&quot;16&quot; height=&quot;16&quot; normalimage=&quot;file='ui/չ��.png' &quot; "));
				pNode_2_4[serialNum_wx * 2 + 2]->SetAttribute(_T("folderattr"), _T("hotimage=&quot;file='ui/չ��.png' &quot; selectedimage=&quot;file='ui/δչ��.png'&quot; selectedhotimage=&quot;file='ui/δչ��.png'&quot;"));
				pNode_2_4[serialNum_wx * 2 + 2]->SetAttribute(_T("itemattr"), _T("padding=\"16,0,0,0\" align=\"left\" font=\"13\""));//font=&quot;6&quot;
				pNode_2_4[serialNum_wx * 2 + 2]->SetName(_T("WX1"));
				pNode_2_3[serialNum_wx]->AddChildNode(pNode_2_4[serialNum_wx * 2 + 2]);
				//}
				m += 1;
			}


		}
	}


}
//��ȡ����վ������������˳��
int CPopEditorialClass::GetNoodLevel_level2_DQZ(string nodeName, int serialNumber)
{
	//��ȡ���������ڽڵ��е�˳��
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
//�ڶ�������˫������Ŀ¼
void CPopEditorialClass::FindTheMap_level3_2(CDuiString cdui_name)
{
	int sequences;//�����vector�е�˳��
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
//����վ-------������վ���ͷ���
void CPopEditorialClass::Categorize_DQZ_EarthStation(string name)
{
	//string str = name;
	vector<string> childName;//����ӽڵ�����
	vector<vector<string>> vec_wxchildName;//��ŵ���վ(����)�ӽڵ�����
	vector<int> childSerialNum;//�ӽڵ�����pNode_3�е����
	int serialNumber;//���ѡ��ĵ��Ÿ�����
	CTreeNodeUI* pNode[1024];//����ɸѡ��ʱ��������ڵ� ������͵�
	CTreeNodeUI* pNode_T4[1024];//����Ĳ�ڵ�

	CTreeNodeUI* pNode_T5[1024];//������ǽڵ��ӽڵ�
	int level_4 = 0;
	int level_5 = 0;
	/*******************ɸѡ���������ڴ���������Ƶ�vector�е�λ��***************/
	for (int i = 0; i < name2_Level1.size(); i++)
	{
		if (name == name2_Level1[i])
		{
			serialNumber = i;
		}
	}
	int childNum = pNode_2_2[serialNumber]->GetCountChild();
	/**********************ɸѡ��ǰ�����������������***********************/
	for (int j = 0; j < childNum; j++)
	{
		CDuiString wstr_name1 = pNode_2_2[serialNumber]->GetChildNode(j)->GetItemText();
		string str_name1 = wstringToString(wstr_name1.GetData());
		childName.push_back(str_name1);
		/******************ɸѡ��ǰ��������������pNode_3�е�˳��***********/
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
	/***********************ɸѡ����������ӽڵ�����*****************************/
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
	/***********************ɾ������������ӽڵ�******************************/
	for (int j = 0; j < childSerialNum.size(); j++)
	{
		pNode_2_3[childSerialNum[j]]->Remove(pNode_2_4[childSerialNum[j] * 2 + 2]);
		pNode_2_3[childSerialNum[j]]->Remove(pNode_2_4[childSerialNum[j] * 2 + 1]);
		pNode_2_3[childSerialNum[j]]->Remove(pNode_2_4[childSerialNum[j] * 2]);
	}
	/***********************ɾ�������µ�����***********************/
	for (int j = 0; j < childSerialNum.size(); j++)
	{
		pNode_2_2[serialNumber]->Remove(pNode_2_3[childSerialNum[j]]);
	}
	/******************************************************/
	for (int j = 0; j < childSerialNum.size(); j++)
	{
		map<string, string> curr_map = m_RecvMap_DQZ[j];

		if (curr_map.count("����վ����") > 0)
		{
			string value = curr_map["����վ����"];
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
				pNode_T4[level_4]->SetAttribute(_T("folderattr"), _T("padding=&quot;0,1,0,0&quot; width=&quot;16&quot; height=&quot;16&quot; normalimage=&quot;file='ui/չ��.png' &quot; "));
				pNode_T4[level_4]->SetAttribute(_T("folderattr"), _T("hotimage=&quot;file='ui/չ��.png' &quot; selectedimage=&quot;file='ui/δչ��.png'&quot; selectedhotimage=&quot;file='ui/δչ��.png'&quot;"));
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
				pNode_T5[level_5]->SetAttribute(_T("folderattr"), _T("padding=&quot;0,1,0,0&quot; width=&quot;16&quot; height=&quot;16&quot; normalimage=&quot;file='ui/չ��.png' &quot; "));
				pNode_T5[level_5]->SetAttribute(_T("folderattr"), _T("hotimage=&quot;file='ui/չ��.png' &quot; selectedimage=&quot;file='ui/δչ��.png'&quot; selectedhotimage=&quot;file='ui/δչ��.png'&quot;"));
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
				pNode_T5[level_5]->SetAttribute(_T("folderattr"), _T("padding=&quot;0,1,0,0&quot; width=&quot;16&quot; height=&quot;16&quot; normalimage=&quot;file='ui/չ��.png' &quot; "));
				pNode_T5[level_5]->SetAttribute(_T("folderattr"), _T("hotimage=&quot;file='ui/չ��.png' &quot; selectedimage=&quot;file='ui/δչ��.png'&quot; selectedhotimage=&quot;file='ui/δչ��.png'&quot;"));
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
				pNode_T5[level_5]->SetAttribute(_T("folderattr"), _T("padding=&quot;0,1,0,0&quot; width=&quot;16&quot; height=&quot;16&quot; normalimage=&quot;file='ui/չ��.png' &quot; "));
				pNode_T5[level_5]->SetAttribute(_T("folderattr"), _T("hotimage=&quot;file='ui/չ��.png' &quot; selectedimage=&quot;file='ui/δչ��.png'&quot; selectedhotimage=&quot;file='ui/δչ��.png'&quot;"));
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
				pNode[j]->SetAttribute(_T("folderattr"), _T("padding=&quot;0,1,0,0&quot; width=&quot;16&quot; height=&quot;16&quot; normalimage=&quot;file='ui/չ��.png' &quot; "));
				pNode[j]->SetAttribute(_T("folderattr"), _T("hotimage=&quot;file='ui/չ��.png' &quot; selectedimage=&quot;file='ui/δչ��.png'&quot; selectedhotimage=&quot;file='ui/δչ��.png'&quot;"));
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
				pNode_T4[level_4]->SetAttribute(_T("folderattr"), _T("padding=&quot;0,1,0,0&quot; width=&quot;16&quot; height=&quot;16&quot; normalimage=&quot;file='ui/չ��.png' &quot; "));
				pNode_T4[level_4]->SetAttribute(_T("folderattr"), _T("hotimage=&quot;file='ui/չ��.png' &quot; selectedimage=&quot;file='ui/δչ��.png'&quot; selectedhotimage=&quot;file='ui/δչ��.png'&quot;"));
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
				pNode_T5[level_5]->SetAttribute(_T("folderattr"), _T("padding=&quot;0,1,0,0&quot; width=&quot;16&quot; height=&quot;16&quot; normalimage=&quot;file='ui/չ��.png' &quot; "));
				pNode_T5[level_5]->SetAttribute(_T("folderattr"), _T("hotimage=&quot;file='ui/չ��.png' &quot; selectedimage=&quot;file='ui/δչ��.png'&quot; selectedhotimage=&quot;file='ui/δչ��.png'&quot;"));
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
				pNode_T5[level_5]->SetAttribute(_T("folderattr"), _T("padding=&quot;0,1,0,0&quot; width=&quot;16&quot; height=&quot;16&quot; normalimage=&quot;file='ui/չ��.png' &quot; "));
				pNode_T5[level_5]->SetAttribute(_T("folderattr"), _T("hotimage=&quot;file='ui/չ��.png' &quot; selectedimage=&quot;file='ui/δչ��.png'&quot; selectedhotimage=&quot;file='ui/δչ��.png'&quot;"));
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
				pNode_T5[level_5]->SetAttribute(_T("folderattr"), _T("padding=&quot;0,1,0,0&quot; width=&quot;16&quot; height=&quot;16&quot; normalimage=&quot;file='ui/չ��.png' &quot; "));
				pNode_T5[level_5]->SetAttribute(_T("folderattr"), _T("hotimage=&quot;file='ui/չ��.png' &quot; selectedimage=&quot;file='ui/δչ��.png'&quot; selectedhotimage=&quot;file='ui/δչ��.png'&quot;"));
				pNode_T5[level_5]->SetAttribute(_T("itemattr"), _T("padding=\"16,0,0,0\" align=\"left\" font=\"13\""));//font=&quot;6&quot;
				pNode_T5[level_5]->SetName(_T("WX1"));

				pNode_T4[level_4]->AddChildNode(pNode_T5[level_5]);
				level_5 += 1;
				level_4 += 1;

			}

		}
	}
	////string str = name;
	//vector<string> childName;//����ӽڵ�����
	//vector<int> childSerialNum;//�ӽڵ�����pNode_3�е����
	//int serialNumber;//���ѡ�����������
	//CTreeNodeUI* pNode[1024];//����ɸѡ��ʱ��������ڵ� ������͵�
	//CTreeNodeUI* pNode_4[1024];//����Ĳ�ڵ�
	///*******************ɸѡ���������ڴ���������Ƶ�vector�е�λ��***************/
	//for (int i = 0; i < name2_Level1.size(); i++)
	//{
	//	if (name == name2_Level1[i])
	//	{
	//		serialNumber = i;
	//	}
	//	//pNode_2[i]
	//}
	//int childNum = pNode_2_2[serialNumber]->GetCountChild();
	///**********************ɸѡ��ǰ�����������������***********************/
	//for (int j = 0; j < childNum; j++)
	//{
	//	CDuiString wstr_name1 = pNode_2_2[serialNumber]->GetChildNode(j)->GetItemText();
	//	string str_name1 = wstringToString(wstr_name1.GetData());
	//	childName.push_back(str_name1);
	//	/******************ɸѡ��ǰ��������������pNode_3�е�˳��***********/
	//	for (int m = 0; m < name2_Level2.size(); m++)
	//	{
	//		if (str_name1 == name2_Level2[m])
	//		{
	//			childSerialNum.push_back(m);

	//		}
	//	}
	//	//pNode_1[0]->Remove(pNode_2[i]);
	//}
	///***********************ɾ�������µ�����***********************/
	//for (int j = 0; j < childSerialNum.size(); j++)
	//{
	//	pNode_2_2[serialNumber]->Remove(pNode_2_3[childSerialNum[j]]);
	//}
	//for (int j = 0; j < childSerialNum.size(); j++)
	//{
	//	map<string, string> curr_map = m_RecvMap_DQZ[j];

	//	if (curr_map.count("����վ����") > 0)
	//	{
	//		string value = curr_map["����վ����"];
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
	//			pNode_4[pNode_2_4_num]->SetAttribute(_T("folderattr"), _T("padding=&quot;0,1,0,0&quot; width=&quot;16&quot; height=&quot;16&quot; normalimage=&quot;file='ui/չ��.png' &quot; "));
	//			pNode_4[pNode_2_4_num]->SetAttribute(_T("folderattr"), _T("hotimage=&quot;file='ui/չ��.png' &quot; selectedimage=&quot;file='ui/δչ��.png'&quot; selectedhotimage=&quot;file='ui/δչ��.png'&quot;"));
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
	//			pNode[j]->SetAttribute(_T("folderattr"), _T("padding=&quot;0,1,0,0&quot; width=&quot;16&quot; height=&quot;16&quot; normalimage=&quot;file='ui/չ��.png' &quot; "));
	//			pNode[j]->SetAttribute(_T("folderattr"), _T("hotimage=&quot;file='ui/չ��.png' &quot; selectedimage=&quot;file='ui/δչ��.png'&quot; selectedhotimage=&quot;file='ui/δչ��.png'&quot;"));
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
	//			pNode_4[pNode_2_4_num]->SetAttribute(_T("folderattr"), _T("padding=&quot;0,1,0,0&quot; width=&quot;16&quot; height=&quot;16&quot; normalimage=&quot;file='ui/չ��.png' &quot; "));
	//			pNode_4[pNode_2_4_num]->SetAttribute(_T("folderattr"), _T("hotimage=&quot;file='ui/չ��.png' &quot; selectedimage=&quot;file='ui/δչ��.png'&quot; selectedhotimage=&quot;file='ui/δչ��.png'&quot;"));
	//			pNode_4[pNode_2_4_num]->SetAttribute(_T("itemattr"), _T("padding=\"16,0,0,0\" align=\"left\" font=\"13\""));//font=&quot;6&quot;
	//			pNode_4[pNode_2_4_num]->SetName(_T("WX1"));

	//			pNode[j]->AddChildNode(pNode_4[pNode_2_4_num]);
	//			pNode_2_4_num += 1;
	//		}

	//	}
	//}
}
//����վ-------������վ���ͷ����ԭ
void CPopEditorialClass::Restore_DQZ_EarthStation(string name)
{
	//string str = name;
	vector<string> childName;//����ӽڵ�����
	vector<vector<string>> vec_wxchildName;//��������ӽڵ�����
	vector<int> childSerialNum;//�ӽڵ�����pNode_3�е����
	int serialNumber;//���ѡ�����������
	CTreeNodeUI* pNode[1024];
	int serialNum_wx;
	/*******************ɸѡ���������ڴ���������Ƶ�vector�е�λ��***************/
	for (int i = 0; i < name2_Level1.size(); i++)
	{
		if (name == name2_Level1[i])
		{
			serialNumber = i;
		}
		//pNode_2[i]
	}
	/*************************ɸѡ�ӽڵ���ӽڵ��������������*********************************/
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
			childName.push_back(str_name);//��ŵ�������
			int wxChildNum = pNode_level3->GetChildNode(j)->GetCountChild();
			for (int m = 0; m < wxChildNum; m++)
			{
				CDuiString cdui_name1 = pNode_level3->GetChildNode(j)->GetChildNode(m)->GetItemText();
				string str_name1 = wstringToString(cdui_name1.GetData());
				vec.push_back(str_name1);//��ŵ�������
			}
			vec_wxchildName.push_back(vec);
		}
	}
	/********************ɾ��2���ڵ�******************/
	for (int i = childNum; i > 0; i--)
	{
		pNode_2_2[serialNumber]->Remove(pNode_2_2[serialNumber]->GetChildNode(i - 1));
	}
	/*********************�½��ָ�Ϊԭ��������**********************/
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
				pNode_2_3[serialNum_wx]->SetAttribute(_T("folderattr"), _T("padding=&quot;0,1,0,0&quot; width=&quot;16&quot; height=&quot;16&quot; normalimage=&quot;file='ui/չ��.png' &quot; "));
				pNode_2_3[serialNum_wx]->SetAttribute(_T("folderattr"), _T("hotimage=&quot;file='ui/չ��.png' &quot; selectedimage=&quot;file='ui/δչ��.png'&quot; selectedhotimage=&quot;file='ui/δչ��.png'&quot;"));
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
				pNode_2_4[serialNum_wx * 2]->SetAttribute(_T("folderattr"), _T("padding=&quot;0,1,0,0&quot; width=&quot;16&quot; height=&quot;16&quot; normalimage=&quot;file='ui/չ��.png' &quot; "));
				pNode_2_4[serialNum_wx * 2]->SetAttribute(_T("folderattr"), _T("hotimage=&quot;file='ui/չ��.png' &quot; selectedimage=&quot;file='ui/δչ��.png'&quot; selectedhotimage=&quot;file='ui/δչ��.png'&quot;"));
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
				pNode_2_4[serialNum_wx * 2 + 1]->SetAttribute(_T("folderattr"), _T("padding=&quot;0,1,0,0&quot; width=&quot;16&quot; height=&quot;16&quot; normalimage=&quot;file='ui/չ��.png' &quot; "));
				pNode_2_4[serialNum_wx * 2 + 1]->SetAttribute(_T("folderattr"), _T("hotimage=&quot;file='ui/չ��.png' &quot; selectedimage=&quot;file='ui/δչ��.png'&quot; selectedhotimage=&quot;file='ui/δչ��.png'&quot;"));
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
				pNode_2_4[serialNum_wx * 2 + 2]->SetAttribute(_T("folderattr"), _T("padding=&quot;0,1,0,0&quot; width=&quot;16&quot; height=&quot;16&quot; normalimage=&quot;file='ui/չ��.png' &quot; "));
				pNode_2_4[serialNum_wx * 2 + 2]->SetAttribute(_T("folderattr"), _T("hotimage=&quot;file='ui/չ��.png' &quot; selectedimage=&quot;file='ui/δչ��.png'&quot; selectedhotimage=&quot;file='ui/δչ��.png'&quot;"));
				pNode_2_4[serialNum_wx * 2 + 2]->SetAttribute(_T("itemattr"), _T("padding=\"16,0,0,0\" align=\"left\" font=\"13\""));//font=&quot;6&quot;
				pNode_2_4[serialNum_wx * 2 + 2]->SetName(_T("WX1"));
				pNode_2_3[serialNum_wx]->AddChildNode(pNode_2_4[serialNum_wx * 2 + 2]);
				//}
				m += 1;
			}


		}
	}
	////string str = name;
	//vector<string> childName;//����ӽڵ�����
	//vector<int> childSerialNum;//�ӽڵ�����pNode_3�е����
	//int serialNumber;//���ѡ�����������
	//CTreeNodeUI* pNode[1024];
	//int serialNum_wx;
	///*******************ɸѡ���������ڴ���������Ƶ�vector�е�λ��***************/
	//for (int i = 0; i < name2_Level1.size(); i++)
	//{
	//	if (name == name2_Level1[i])
	//	{
	//		serialNumber = i;
	//	}
	//	//pNode_2[i]
	//}
	///*************************ɸѡ�ӽڵ���ӽڵ��������������*********************************/
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
	///********************ɾ��2���ڵ�******************/
	//for (int i = childNum; i > 0; i--)
	//{
	//	pNode_2_2[serialNumber]->Remove(pNode_2_2[serialNumber]->GetChildNode(i - 1));
	//}
	///*********************�½��ָ�Ϊԭ��������**********************/
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
	//			pNode_2_3[serialNum_wx]->SetAttribute(_T("folderattr"), _T("padding=&quot;0,1,0,0&quot; width=&quot;16&quot; height=&quot;16&quot; normalimage=&quot;file='ui/չ��.png' &quot; "));
	//			pNode_2_3[serialNum_wx]->SetAttribute(_T("folderattr"), _T("hotimage=&quot;file='ui/չ��.png' &quot; selectedimage=&quot;file='ui/δչ��.png'&quot; selectedhotimage=&quot;file='ui/δչ��.png'&quot;"));
	//			pNode_2_3[serialNum_wx]->SetAttribute(_T("itemattr"), _T("padding=\"16,0,0,0\" align=\"left\" font=\"13\""));//font=&quot;6&quot;
	//			pNode_2_3[serialNum_wx]->SetName(_T("WX1"));
	//			pNode_2_2[serialNumber]->AddChildNode(pNode_2_3[serialNum_wx]);
	//		}
	//	}
	//}
}
//�ƶ�����վ��ĳ�����Ÿ۽ڵ�
void CPopEditorialClass::Movelevel_2_ToDXG(string level1, CDuiString level2)
{
	vector<string> vec_child;//���Ҫ�ƶ��ĺ��ӽڵ�
	string str_nameP;//��ȡlevel2���ڸ��ڵ�����
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
			sequences1 = i;//ѡ�����������name_Level2�Լ�m_RecvMap�е�λ��
			CTreeNodeUI*  node = pNode_2_3[i]->GetParentNode();//GetParentNode
			wstr_nameP = node->GetItemText();
			str_nameP = wstringToString(wstr_nameP);
			childNum = pNode_2_3[i]->GetCountChild();//��ȡҪ�ƶ������ǽڵ��ӽڵ�����

		}
		/******************��ȡ���ǰ����ӽڵ�����****************/
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
	/****************��ȡ�����ӽڵ�����name_Level3�е�˳��***************/
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
	/**********************��ȡ���Ǹ��ڵ�ͽ�Ҫ�ƶ����ĸ��ڵ����������е�λ��*************************/
	int sequences = 0;
	int sequencesFrom = 0;
	for (int i = 0; i < name2_Level1.size(); i++)
	{
		wstring wstr_name;
		wstr_name = pNode_2_2[i]->GetItemText();
		string str_name = wstringToString(wstr_name);
		if (level1 == str_name)
		{
			sequences = i;//����Ҫ�ƶ���Ŀ�������ڵ���pNode_2�е�λ��
		}
		if (str_nameP == str_name)
		{
			sequencesFrom = i;//���Ǳ������������ڵ���pNode_2�е�λ��
		}

	}
	/********************ɾ�����ǰ������ӽڵ�*********************/
	for (int i = 0; i < sequences_0.size(); i++)
	{
		cout << sequencesFrom << endl;
		pNode_2_3[sequences1]->Remove(pNode_2_4[sequences_0[i]]);
	}
	/********************������ԭ���ĸ��ڵ�ɾ�����ǽڵ� �½����ǽڵ����ƶ���Ŀ�길�ڵ�*********************/
	cout << sequencesFrom << endl;
	pNode_2_2[sequencesFrom]->Remove(pNode_2_3[sequences1]);
	pNode_2_3[sequences1] = new CTreeNodeUI();
	pNode_2_3[sequences1]->CheckBoxSelected(false);
	pNode_2_3[sequences1]->SetItemText(level2);
	pNode_2_3[sequences1]->SetFixedHeight(30);
	pNode_2_3[sequences1]->SetItemTextColor(0xFFFFFFFF);
	pNode_2_3[sequences1]->SetItemHotTextColor(0xFFFFFFFF);
	pNode_2_3[sequences1]->SetSelItemTextColor(0xFFFFFFFF);
	pNode_2_3[sequences1]->SetAttribute(_T("folderattr"), _T("padding=&quot;0,1,0,0&quot; width=&quot;16&quot; height=&quot;16&quot; normalimage=&quot;file='ui/չ��.png' &quot; "));
	pNode_2_3[sequences1]->SetAttribute(_T("folderattr"), _T("hotimage=&quot;file='ui/չ��.png' &quot; selectedimage=&quot;file='ui/δչ��.png'&quot; selectedhotimage=&quot;file='ui/δչ��.png'&quot;"));
	pNode_2_3[sequences1]->SetAttribute(_T("itemattr"), _T("padding=\"16,0,0,0\" align=\"left\" font=\"13\""));//font=&quot;6&quot;
	pNode_2_3[sequences1]->SetName(_T("WX1"));
	pNode_2_2[sequences]->AddChildNode(pNode_2_3[sequences1]);

	/************************�½������ӽڵ�*********************/
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
		pNode_2_4[sequences_0[i]]->SetAttribute(_T("folderattr"), _T("padding=&quot;0,1,0,0&quot; width=&quot;16&quot; height=&quot;16&quot; normalimage=&quot;file='ui/չ��.png' &quot; "));
		pNode_2_4[sequences_0[i]]->SetAttribute(_T("folderattr"), _T("hotimage=&quot;file='ui/չ��.png' &quot; selectedimage=&quot;file='ui/δչ��.png'&quot; selectedhotimage=&quot;file='ui/δչ��.png'&quot;"));
		pNode_2_4[sequences_0[i]]->SetAttribute(_T("itemattr"), _T("padding=\"16,0,0,0\" align=\"left\" font=\"13\""));//font=&quot;6&quot;
		pNode_2_4[sequences_0[i]]->SetName(_T("WX1"));
		pNode_2_3[sequences1]->AddChildNode(pNode_2_4[sequences_0[i]]);
	}

}
//��ȡ�����޸ĵ����� �����Ӧ��vector
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
//�����ն˰����ʷ���
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
		/***************�Ƴ��������1���ڵ�******************/
		pNode_3_1[0]->Remove(pNode_3_2[i]);
		/***************�½�ɸѡ���2���ڵ�******************/
		pNode_3_2[i] = new CTreeNodeUI();
		pNode_3_2[i]->CheckBoxSelected(false);
		pNode_3_2[i]->SetItemText(stringToWstring(str_name).c_str());
		pNode_3_2[i]->SetFixedHeight(30);
		pNode_3_2[i]->SetItemTextColor(0xFFFFFFFF);
		pNode_3_2[i]->SetItemHotTextColor(0xFFFFFFFF);
		pNode_3_2[i]->SetSelItemTextColor(0xFFFFFFFF);
		pNode_3_2[i]->SetAttribute(_T("folderattr"), _T("padding=&quot;0,1,0,0&quot; width=&quot;16&quot; height=&quot;16&quot; normalimage=&quot;file='ui/չ��.png' &quot; "));
		pNode_3_2[i]->SetAttribute(_T("folderattr"), _T("hotimage=&quot;file='ui/չ��.png' &quot; selectedimage=&quot;file='ui/δչ��.png'&quot; selectedhotimage=&quot;file='ui/δչ��.png'&quot;"));
		pNode_3_2[i]->SetAttribute(_T("itemattr"), _T("padding=\"16,0,0,0\" align=\"left\" font=\"13\""));//font=&quot;6&quot;
		pNode_3_2[i]->SetName(_T("WX1"));
		map<string, string> curr_map = m_RecvMap_DMZD[i];
		if (curr_map.count("�ն�����") > 0)
		{
			string value = curr_map["�ն�����"];
			int sequence = GetNoodLevel_DMZD_ZDLX(UTF8_To_string(value));
			if (sequence >= 0)
			{
				pNode_3_1[sequence]->AddChildNode(pNode_3_2[i]);
			}
			else
			{
				CreateTreeNode_3(UTF8_To_string(value));//����ڵ������
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
					pNode_3_3[n]->SetAttribute(_T("folderattr"), _T("padding=&quot;0,1,0,0&quot; width=&quot;16&quot; height=&quot;16&quot; normalimage=&quot;file='ui/չ��.png' &quot; "));
					pNode_3_3[n]->SetAttribute(_T("folderattr"), _T("hotimage=&quot;file='ui/չ��.png' &quot; selectedimage=&quot;file='ui/δչ��.png'&quot; selectedhotimage=&quot;file='ui/δչ��.png'&quot;"));
					pNode_3_3[n]->SetAttribute(_T("itemattr"), _T("padding=\"16,0,0,0\" align=\"left\" font=\"13\""));//font=&quot;6&quot;
					pNode_3_3[n]->SetName(_T("WX1"));
					pNode_3_2[i]->AddChildNode(pNode_3_3[n]);
				}
			}
		}


	}
}
//�������ʷ����ԭ
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
		pNode_3_2[i]->SetAttribute(_T("folderattr"), _T("padding=&quot;0,1,0,0&quot; width=&quot;16&quot; height=&quot;16&quot; normalimage=&quot;file='ui/չ��.png' &quot; "));
		pNode_3_2[i]->SetAttribute(_T("folderattr"), _T("hotimage=&quot;file='ui/չ��.png' &quot; selectedimage=&quot;file='ui/δչ��.png'&quot; selectedhotimage=&quot;file='ui/δչ��.png'&quot;"));
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
					pNode_3_3[n]->SetAttribute(_T("folderattr"), _T("padding=&quot;0,1,0,0&quot; width=&quot;16&quot; height=&quot;16&quot; normalimage=&quot;file='ui/չ��.png' &quot; "));
					pNode_3_3[n]->SetAttribute(_T("folderattr"), _T("hotimage=&quot;file='ui/չ��.png' &quot; selectedimage=&quot;file='ui/δչ��.png'&quot; selectedhotimage=&quot;file='ui/δչ��.png'&quot;"));
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
	//	pNode_3_2[i]->SetAttribute(_T("folderattr"), _T("padding=&quot;0,1,0,0&quot; width=&quot;16&quot; height=&quot;16&quot; normalimage=&quot;file='ui/չ��.png' &quot; "));
	//	pNode_3_2[i]->SetAttribute(_T("folderattr"), _T("hotimage=&quot;file='ui/չ��.png' &quot; selectedimage=&quot;file='ui/δչ��.png'&quot; selectedhotimage=&quot;file='ui/δչ��.png'&quot;"));
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
	//	//			pNode_2_3[n]->SetAttribute(_T("folderattr"), _T("padding=&quot;0,1,0,0&quot; width=&quot;16&quot; height=&quot;16&quot; normalimage=&quot;file='ui/չ��.png' &quot; "));
	//	//			pNode_2_3[n]->SetAttribute(_T("folderattr"), _T("hotimage=&quot;file='ui/չ��.png' &quot; selectedimage=&quot;file='ui/δչ��.png'&quot; selectedhotimage=&quot;file='ui/δչ��.png'&quot;"));
	//	//			pNode_2_3[n]->SetAttribute(_T("itemattr"), _T("padding=\"16,0,0,0\" align=\"left\" font=\"13\""));//font=&quot;6&quot;
	//	//			pNode_2_3[n]->SetName(_T("WX1"));
	//	//			pNode_2_2[i]->AddChildNode(pNode_2_3[n]);
	//	//		}
	//	//	}
	//	//}
	//}
	//RootNodeOnly1_DMZD();
}
//��ȡ�����ն˵��ն����͵Ľڵ���������˳��
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
//������ն����͵ĸ��ڵ�����˳��
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
//�����ն˷��ิԭ��ֻ�����ڵ�
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
//������3�������޸ĺ�洢��txt
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
//ȡ��tab1����ѡ��
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
//�����ļ�
void CPopEditorialClass::GenerateTheFile(string path, string filename, map<string, string> m_map,string sign)
{
	//string path1 = "D:\\DATA1";
	if (0 != _access(path.c_str(), 0))
	{
		_mkdir(path.c_str());   // ���� 0 ��ʾ�����ɹ���-1 ��ʾʧ��
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

//�����ļ�
void CPopEditorialClass::GenerateTheFile(string path, string filename, vector<vector<string>> vecVec)
{
	if (0 != _access(path.c_str(), 0))
	{
		_mkdir(path.c_str());   // ���� 0 ��ʾ�����ɹ���-1 ��ʾʧ��
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
	/*******************�½��������ֶ�********************/                        //////// ?????? ���� �����ֶβ�һ�µ�����
	

	
	if (name_Level1.size() > 0)
	{
		//std::vector<std::string> filelds;
		std::string strTableNameXZ = strScheme + UTF8_To_string("_����");
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

		// ��������
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
	/*******************�½����Ǳ��ֶ�********************/// �򵥲������� ���߸����ֶ�������ֵ;
	
	if (name_Level2.size() > 0)
	{
		//std::set<std::string> filelds;
		filelds.clear();
		std::string strTableNameWX = strScheme + UTF8_To_string("_����");
		filelds.insert("Name");
		for (int i = 0; i < m_RecvMap.size(); i++)
		{
			for (auto map : m_RecvMap[i])
			{
				filelds.insert(UTF8_To_string(map.first));
			}

		}

		ConnectMysql::Instance().createTableAndFields(strTableNameWX, filelds);

		// ��������
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

	/*******************�½��������߱��ֶ�********************/
	if (m_vecTxName.size() > 0)
	{
		
		filelds.clear();
		std::string strTableNameWXTX = strScheme + UTF8_To_string("_��������");
		filelds.insert("Name");
		for (int i = 0; i < m_vecMapTxData.size(); i++)
		{
			for (auto map : m_vecMapTxData[i])
			{
				filelds.insert(UTF8_To_string(map.first));
			}

		}

		ConnectMysql::Instance().createTableAndFields(strTableNameWXTX, filelds);

		// ��������
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

	/*******************�½������غɱ��ֶ�********************/
	if (m_vecZhName.size() > 0)
	{

		filelds.clear();
		std::string strTableNameWXZH = strScheme + UTF8_To_string("_�����غ�");
		filelds.insert("Name");
		for (int i = 0; i < m_vecMapZhData.size(); i++)
		{
			for (auto map : m_vecMapZhData[i])
			{
				filelds.insert(UTF8_To_string(map.first));
			}

		}

		ConnectMysql::Instance().createTableAndFields(strTableNameWXZH, filelds);

		// ��������
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

	/*******************�½����Ÿ۱��ֶ�********************/
	if (name2_Level1.size() > 0)
	{
		filelds.clear();
		std::string strTableNameDXG = strScheme + UTF8_To_string("_���Ÿ�");
		filelds.insert("Name");
		for (int i = 0; i < m_RecvMap_DXG.size(); i++)
		{
			for (auto map : m_RecvMap_DXG[i])
			{
				filelds.insert(UTF8_To_string(map.first));
			}
		}

		ConnectMysql::Instance().createTableAndFields(strTableNameDXG, filelds);

		// ��������
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

	/*******************�½�����վ���ֶ�********************/
	if (name2_Level2.size() > 0)
	{

		filelds.clear();
		std::string strTableNameDQZ = strScheme + UTF8_To_string("_����վ");
		filelds.insert("Name");
		for (int i = 0; i < m_RecvMap_DQZ.size(); i++)
		{
			for (auto map : m_RecvMap_DQZ[i])
			{
				filelds.insert(UTF8_To_string(map.first));
			}

		}

		ConnectMysql::Instance().createTableAndFields(strTableNameDQZ, filelds);

		// ��������
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

	/*******************�½�����վ���α��ֶ�********************/
	if (m_vecDqzBxName.size() > 0)
	{

		filelds.clear();
		std::string strTableNameDQZBX = strScheme + UTF8_To_string("_����վ����");
		filelds.insert("Name");
		for (int i = 0; i < m_vecMapDqzBxData.size(); i++)
		{
			for (auto map : m_vecMapDqzBxData[i])
			{
				filelds.insert(UTF8_To_string(map.first));
			}

		}
		ConnectMysql::Instance().createTableAndFields(strTableNameDQZBX, filelds);
		// ��������
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
	/*******************�½�����վ��Դ���ֶ�********************/
	if (m_vecDqzkyName.size() > 0)
	{

		filelds.clear();
		std::string strTableNameDQZKY = strScheme + UTF8_To_string("_����վ��Դ");
		filelds.insert("Name");
		for (int i = 0; i < m_vecMapDqzKyData.size(); i++)
		{
			for (auto map : m_vecMapDqzKyData[i])
			{
				filelds.insert(UTF8_To_string(map.first));
			}

		}
		ConnectMysql::Instance().createTableAndFields(strTableNameDQZKY, filelds);
		// ��������
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

	/*******************�½�����վ���߱��ֶ�********************/
	if (m_vecDqzTxName.size() > 0)
	{

		filelds.clear();
		std::string strTableNameDQZTX = strScheme + UTF8_To_string("_����վ����");
		filelds.insert("Name");
		for (int i = 0; i < m_vecMapDqzTxData.size(); i++)
		{
			for (auto map : m_vecMapDqzTxData[i])
			{
				filelds.insert(UTF8_To_string(map.first));
			}

		}
		ConnectMysql::Instance().createTableAndFields(strTableNameDQZTX, filelds);
		// ��������
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

	/*******************�½������ն˱��ֶ�********************/
	if (name3_Level1.size() > 0)
	{

		filelds.clear();
		std::string strTableNameDMZD = strScheme + UTF8_To_string("_�����ն�");
		filelds.insert("Name");
		for (int i = 0; i < m_RecvMap_DMZD.size(); i++)
		{
			for (auto map : m_RecvMap_DMZD[i])
			{
				filelds.insert(UTF8_To_string(map.first));
			}

		}

		ConnectMysql::Instance().createTableAndFields(strTableNameDMZD, filelds);

		// ��������
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

	/*******************�½������ն˱��μ��ֶ�********************/
	if (m_vecDmzdBxName.size() > 0)
	{

		filelds.clear();
		std::string strTableNameDMZDBX = strScheme + UTF8_To_string("_�����ն˲���");
		filelds.insert("Name");
		for (int i = 0; i < m_vecMapDmzdBxData.size(); i++)
		{
			for (auto map : m_vecMapDmzdBxData[i])
			{
				filelds.insert(UTF8_To_string(map.first));
			}
		}

		ConnectMysql::Instance().createTableAndFields(strTableNameDMZDBX, filelds);
		// ��������
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
			// ��������;
			ConnectMysql::Instance().insertTableData(strTableNameDMZDBX, vecFilelds, vecFileldsValue);
		}
	}

	/*******************�½������ն��غɱ��ֶ�********************/
	if (m_vecDmzdZhName.size() > 0)
	{

		filelds.clear();
		std::string strTableNameDMZDZH = strScheme + UTF8_To_string("_�����ն��غ�");
		filelds.insert("Name");
		for (int i = 0; i < m_vecMapDmzdZhData.size(); i++)
		{
			for (auto map : m_vecMapDmzdZhData[i])
			{
				filelds.insert(UTF8_To_string(map.first));
			}
		}

		ConnectMysql::Instance().createTableAndFields(strTableNameDMZDZH, filelds);
		// ��������
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
			// ��������;
			ConnectMysql::Instance().insertTableData(strTableNameDMZDZH, vecFilelds, vecFileldsValue);
		}
	}

	/*******************�½������ն����߱��ֶ�********************/
	if (m_vecDmzdTxName.size() > 0)
	{

		filelds.clear();
		std::string strTableNameDMZDTX = strScheme + UTF8_To_string("_�����ն�����");
		filelds.insert("Name");
		for (int i = 0; i < m_vecMapDmzdTxData.size(); i++)
		{
			for (auto map : m_vecMapDmzdTxData[i])
			{
				filelds.insert(UTF8_To_string(map.first));
			}
		}

		ConnectMysql::Instance().createTableAndFields(strTableNameDMZDTX, filelds);
		// ��������
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
			// ��������;
			ConnectMysql::Instance().insertTableData(strTableNameDMZDTX, vecFilelds, vecFileldsValue);
		}
	}
}
//��tab1����ı������ݵ�����溯��
void CPopEditorialClass::ImportingLocalData_Tab1()
{
	m_vecTxName.clear();
	m_vecZhName.clear();
	m_vecMapTxData.clear();
	m_vecMapZhData.clear();

	vector<string> vec_TxZhName;//���ǵ��ӽڵ� --- �����غ�����
	vector<map<string, string>> vec_TxZhData;//���ǵ��ӽڵ� --- �����غ�����

	vector<string> vec_WxName;
	vector<map<string, string>> vec_wxData;//�����ļ������������ݴ洢����

	vector<vector<string>> vecXZAndWX;//ÿһ�ж���� ����:����-����-������
	vector<vector<string>> vecWXAndTxZh;//ÿһ�ж���� ����:����-�غ�-������
	vector<wstring> upload_fileName;
	upload_fileName = UtilTool::GetOpenFilePaths_txt();
	/*********************�Ȱ�������ɸѡ������ʾ�ڽ�����*********************/
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
		fin.read((char*)&s2, sizeof(s2));//��ȡ��һ���ֽڣ�Ȼ������8λ
		int p = s2 << 8;
		fin.read((char*)&s2, sizeof(s2));//��ȡ�ڶ����ֽ�
		p += s2;

		string code;
		fin.close();

		ifstream infile;
		infile.open(fileN);   //���ļ����������ļ��������� 
		//assert(infile.is_open());   //��ʧ��,�����������Ϣ,����ֹ�������� 
		if (infile)
		{
			string s;
			map<string, string> l_map;
			bool flag = true;
			string type;
			while (getline(infile, s))
			{
				if (flag)//����ǵ�һ�� ��һ��д��
				{
					cout << "��һ��" << endl;
					type = s;

					flag = false;
				}
				else
				{
					cout << "�ڶ���" << endl;
					cout << s.c_str() << endl;
					vector<string> str_;
					string pattern = ":";
					str_ = my_split(s, pattern);
					if (p == 58760 || p == 59032)//�ļ���UTF-8����
					{
						l_map[str_[0]] = str_[1];
					}
					else if (p == 46260)//�ļ�����������
					{

						l_map[string_To_UTF8(str_[0])] = string_To_UTF8(str_[1]);
					}
					else
					{
						l_map[str_[0]] = str_[1];
					}
				}


			}
			if (type == "����")
			{
				cout << "����" << endl;
				AssignmentToLocalVector_XZ(str_xzName[0], l_map);//�����������Ϣ��ʾ�ڽ�����

			//	std::vector<std::string> filelds;
			//	for (auto map : l_map)
			//	{
			//		filelds.push_back(UTF8_To_string(map.first));
			//	}
			//	std::string strTable = UTF8_To_string("����1_") + str_xzName[0];
			////	strTable = UTF8_To_string(strTable);
			//	ConnectMysql::Instance().createTableAndFields(strTable, filelds);
			}
			
			infile.close();             //�ر��ļ������� 
		}
		//m_FtpClientClass.execute_putFile(fileN);//put �ϴ��ļ�
	}
	/*********************�����������ļ�ɸѡ����**********************************/
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
		if (str_xzName[0] == UTF8_To_string("�����ļ�_����"))
		{
			ifstream fin(fileN, ios::binary);
			unsigned char  s2;
			fin.read((char*)&s2, sizeof(s2));//��ȡ��һ���ֽڣ�Ȼ������8λ
			int p = s2 << 8;
			fin.read((char*)&s2, sizeof(s2));//��ȡ�ڶ����ֽ�
			p += s2;

			string code;
			fin.close();

			ifstream infile;
			infile.open(fileN);   //���ļ����������ļ��������� 
			//assert(infile.is_open());   //��ʧ��,�����������Ϣ,����ֹ�������� 
			if (infile)
			{
				string s;
				map<string, string> l_map;

				while (getline(infile, s))
				{
					vector<string> line;
					cout << "�ڶ���" << endl;
					cout << s.c_str() << endl;
					vector<string> str_;
					string pattern = ":";
					str_ = my_split(s, pattern);
					if (p == 58760 || p == 59032)//�ļ���UTF-8����
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
					else if (p == 46260 || p == 21364)//�ļ�����������
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
	/*********************�����ǵ�����ɸѡ����*************************************/
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
		fin.read((char*)&s2, sizeof(s2));//��ȡ��һ���ֽڣ�Ȼ������8λ
		int p = s2 << 8;
		fin.read((char*)&s2, sizeof(s2));//��ȡ�ڶ����ֽ�
		p += s2;

		string code;
		fin.close();

		ifstream infile;
		infile.open(fileN);   //���ļ����������ļ��������� 
		//assert(infile.is_open());   //��ʧ��,�����������Ϣ,����ֹ�������� 
		if (infile)
		{
			string s;
			map<string, string> l_map;
			bool flag = true;
			string type;
			while (getline(infile, s))
			{
				if (flag)//����ǵ�һ�� ��һ��д��
				{
					cout << "��һ��" << endl;
					type = s;

					flag = false;
				}
				else
				{
					cout << "�ڶ���" << endl;
					cout << s.c_str() << endl;
					vector<string> str_;
					string pattern = ":";
					str_ = my_split(s, pattern);
					if (p == 58760 || p == 59032 || p == 58765)//�ļ���UTF-8����
					{
						l_map[str_[0]] = str_[1];
					}
					else if (p == 46260)//�ļ�����������
					{

						l_map[string_To_UTF8(str_[0])] = string_To_UTF8(str_[1]);
					}
					else
					{
						l_map[str_[0]] = str_[1];
					}
				}


			}
			if (type == "����")
			{
				cout << "����" << endl;
				vec_WxName.push_back(str_xzName[0]);
				vec_wxData.push_back(l_map);
				//AssignmentToLocalVector_XZ(str_xzName[0], l_map);
			}


			infile.close();             //�ر��ļ������� 
		}
	}
	/*********************�����ߺ��غɵ�����ɸѡ����******************************/
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
		fin.read((char*)&s2, sizeof(s2));//��ȡ��һ���ֽڣ�Ȼ������8λ
		int p = s2 << 8;
		fin.read((char*)&s2, sizeof(s2));//��ȡ�ڶ����ֽ�
		p += s2;

		string code;
		fin.close();

		ifstream infile;
		infile.open(fileN);   //���ļ����������ļ��������� 
		//assert(infile.is_open());   //��ʧ��,�����������Ϣ,����ֹ�������� 
		if (infile)
		{
			string s;
			map<string, string> l_map;
			bool flag = true;
			string type;
			while (getline(infile, s))
			{
				if (flag)//����ǵ�һ�� ��һ��д��
				{
					cout << "��һ��" << endl;
					type = s;

					flag = false;
				}
				else
				{
					cout << "�ڶ���" << endl;
					cout << s.c_str() << endl;
					vector<string> str_;
					string pattern = ":";
					str_ = my_split(s, pattern);
					if (p == 58760 || p == 59032 || p == 58765)//�ļ���UTF-8����
					{
						l_map[str_[0]] = str_[1];
					}
					else if (p == 46260)//�ļ�����������
					{

						l_map[string_To_UTF8(str_[0])] = string_To_UTF8(str_[1]);
					}
					else
					{
						l_map[str_[0]] = str_[1];
					}
				}


			}
			//if (type == "���߻��غ�")
			//{
			//	cout << "����" << endl;
			//	vec_TxZhName.push_back(str_xzName[0]);
			//	vec_TxZhData.push_back(l_map);
			//	//AssignmentToLocalVector_XZ(str_xzName[0], l_map);
			//}

			if (type == "����")
			{
				cout << "����" << endl;
				vec_TxZhName.push_back(str_xzName[0]);
				vec_TxZhData.push_back(l_map);

				
				m_vecTxName.push_back(str_xzName[0]);
				m_vecMapTxData.push_back(l_map);
			}
			else if(type == "�غ�")
			{
				vec_TxZhName.push_back(str_xzName[0]);
			    vec_TxZhData.push_back(l_map);

				m_vecZhName.push_back(str_xzName[0]);
				m_vecMapZhData.push_back(l_map);
			}


			infile.close();             //�ر��ļ������� 
		}
	}
	/*********************�����������ļ�ɸѡ����**********************************/
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
		if (str_xzName[0] == UTF8_To_string("�����ļ�_����"))
		{
			ifstream fin(fileN, ios::binary);
			unsigned char  s2;
			fin.read((char*)&s2, sizeof(s2));//��ȡ��һ���ֽڣ�Ȼ������8λ
			int p = s2 << 8;
			fin.read((char*)&s2, sizeof(s2));//��ȡ�ڶ����ֽ�
			p += s2;

			string code;
			fin.close();

			ifstream infile;
			infile.open(fileN);   //���ļ����������ļ��������� 
			//assert(infile.is_open());   //��ʧ��,�����������Ϣ,����ֹ�������� 
			if (infile)
			{
				string s;
				map<string, string> l_map;

				while (getline(infile, s))
				{
					vector<string> line;
					cout << "�ڶ���" << endl;
					cout << s.c_str() << endl;
					vector<string> str_;
					string pattern = ":";
					str_ = my_split(s, pattern);
					if (p == 58760 || p == 59032)//�ļ���UTF-8����
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
					else if (p == 46260 || p == 21364)//�ļ�����������
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
	/*********************�������ǽڵ���treeView�е�һ���ڵ�����*******************/
	for (int i = 0; i < vec_WxName.size(); i++)
	{
		int seques = 0;
		string xzName;//��������
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
	/***********************�������ǵ����ߺ��غ���treeview*************************************/
	for (int i = 0; i < vec_TxZhName.size(); i++)
	{
		int seques = 0;
		string xzName;//��������
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

	vector<string> vec_TxZhName;//����վ���ӽڵ� --- ��Դ������������
	vector<map<string, string>> vec_TxZhData;//���ǵ��ӽڵ� --- �����غ�����

	vector<string> vec_WxName;
	vector<map<string, string>> vec_wxData;//�����ļ������������ݴ洢����

	vector<vector<string>> vecXZAndWX;//ÿһ�ж��������:����-����-������
	vector<vector<string>> vecWXAndTxZh;//ÿһ�ж���� ����:����-�غ�-������

	vector<wstring> upload_fileName;
	upload_fileName = UtilTool::GetOpenFilePaths_txt();
	/*********************�Ȱѵ��Ÿ۵�ɸѡ������ʾ�ڽ�����*********************/
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
		fin.read((char*)&s2, sizeof(s2));//��ȡ��һ���ֽڣ�Ȼ������8λ
		int p = s2 << 8;
		fin.read((char*)&s2, sizeof(s2));//��ȡ�ڶ����ֽ�
		p += s2;

		string code;
		fin.close();

		ifstream infile;
		infile.open(fileN);   //���ļ����������ļ��������� 
		//assert(infile.is_open());   //��ʧ��,�����������Ϣ,����ֹ�������� 
		if (infile)
		{
			string s;
			map<string, string> l_map;
			bool flag = true;
			string type;
			while (getline(infile, s))
			{
				if (flag)//����ǵ�һ�� ��һ��д��
				{
					cout << "��һ��" << endl;
					type = s;

					flag = false;
				}
				else
				{
					cout << "�ڶ���" << endl;
					cout << s.c_str() << endl;
					vector<string> str_;
					string pattern = ":";
					str_ = my_split(s, pattern);
					//if (p == 58760 || p == 59032)//�ļ���UTF-8����
					//{
					//	l_map[str_[0]] = str_[1];
					//}
					if (p == 46260)//�ļ�����������
					{

						l_map[string_To_UTF8(str_[0])] = string_To_UTF8(str_[1]);
					}
					else
					{
						l_map[str_[0]] = str_[1];
					}
				}


			}
			if (type == "���Ÿ�")
			{
				cout << "���Ÿ�" << endl;
				AssignmentToLocalVector_DXG(str_xzName[0], l_map);//�����������Ϣ��ʾ�ڽ�����
			}


			infile.close();             //�ر��ļ������� 
		}

	}
	/*********************�ѵ��Ÿ������ļ�ɸѡ����**********************************/
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
		if (str_xzName[0] == UTF8_To_string("�����ļ�_���Ÿ�"))
		{
			ifstream fin(fileN, ios::binary);
			unsigned char  s2;
			fin.read((char*)&s2, sizeof(s2));//��ȡ��һ���ֽڣ�Ȼ������8λ
			int p = s2 << 8;
			fin.read((char*)&s2, sizeof(s2));//��ȡ�ڶ����ֽ�
			p += s2;

			string code;
			fin.close();

			ifstream infile;
			infile.open(fileN);   //���ļ����������ļ��������� 
			//assert(infile.is_open());   //��ʧ��,�����������Ϣ,����ֹ�������� 
			if (infile)
			{
				string s;
				map<string, string> l_map;

				while (getline(infile, s))
				{
					vector<string> line;
					cout << "�ڶ���" << endl;
					cout << s.c_str() << endl;
					vector<string> str_;
					string pattern = ":";
					str_ = my_split(s, pattern);
					if (p == 58760 || p == 59032)//�ļ���UTF-8����
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
					else if (p == 46260 || p == 21364)//�ļ�����������
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
	///*********************�ѵ���վ������ɸѡ����*************************************/
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
		fin.read((char*)&s2, sizeof(s2));//��ȡ��һ���ֽڣ�Ȼ������8λ
		int p = s2 << 8;
		fin.read((char*)&s2, sizeof(s2));//��ȡ�ڶ����ֽ�
		p += s2;

		string code;
		fin.close();

		ifstream infile;
		infile.open(fileN);   //���ļ����������ļ��������� 
		//assert(infile.is_open());   //��ʧ��,�����������Ϣ,����ֹ�������� 
		if (infile)
		{
			string s;
			map<string, string> l_map;
			bool flag = true;
			string type;
			while (getline(infile, s))
			{
				if (flag)//����ǵ�һ�� ��һ��д��
				{
					cout << "��һ��" << endl;
					type = s;

					flag = false;
				}
				else
				{
					cout << "�ڶ���" << endl;
					cout << s.c_str() << endl;
					vector<string> str_;
					string pattern = ":";
					str_ = my_split(s, pattern);
					if (p == 58760 || p == 59032 || p == 58765)//�ļ���UTF-8����
					{
						l_map[str_[0]] = str_[1];
					}
					else if (p == 46260)//�ļ�����������
					{

						l_map[string_To_UTF8(str_[0])] = string_To_UTF8(str_[1]);
					}
					else
					{
						l_map[str_[0]] = str_[1];
					}
				}


			}
			if (type == "����վ")
			{
				cout << "����վ" << endl;
				vec_WxName.push_back(str_xzName[0]);
				vec_wxData.push_back(l_map);
				//AssignmentToLocalVector_XZ(str_xzName[0], l_map);
			}


			infile.close();             //�ر��ļ������� 
		}
	}
	/*********************�����ߺ��غɵ�����ɸѡ����******************************/
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
		fin.read((char*)&s2, sizeof(s2));//��ȡ��һ���ֽڣ�Ȼ������8λ
		int p = s2 << 8;
		fin.read((char*)&s2, sizeof(s2));//��ȡ�ڶ����ֽ�
		p += s2;

		string code;
		fin.close();

		ifstream infile;
		infile.open(fileN);   //���ļ����������ļ��������� 
		//assert(infile.is_open());   //��ʧ��,�����������Ϣ,����ֹ�������� 
		if (infile)
		{
			string s;
			map<string, string> l_map;
			bool flag = true;
			string type;
			while (getline(infile, s))
			{
				if (flag)//����ǵ�һ�� ��һ��д��
				{
					cout << "��һ��" << endl;
					type = s;

					flag = false;
				}
				else
				{
					cout << "�ڶ���" << endl;
					cout << s.c_str() << endl;
					vector<string> str_;
					string pattern = ":";
					str_ = my_split(s, pattern);
					if (p == 58760 || p == 59032 || p == 58765)//�ļ���UTF-8����
					{
						l_map[str_[0]] = str_[1];
					}
					else if (p == 46260)//�ļ�����������
					{

						l_map[string_To_UTF8(str_[0])] = string_To_UTF8(str_[1]);
					}
					else
					{
						l_map[str_[0]] = str_[1];
					}
				}


			}
			if (type == "��Դ��������")
			{
				cout << "��Դ��������" << endl;
				vec_TxZhName.push_back(str_xzName[0]);
				vec_TxZhData.push_back(l_map);
				//AssignmentToLocalVector_XZ(str_xzName[0], l_map);
			}

			if (type == "����վ��Դ")
			{
				vec_TxZhName.push_back(str_xzName[0]);
				vec_TxZhData.push_back(l_map);

				m_vecDqzkyName.push_back(str_xzName[0]);
				m_vecMapDqzKyData.push_back(l_map);
			}
			else if (type == "����վ����")
			{
				vec_TxZhName.push_back(str_xzName[0]);
				vec_TxZhData.push_back(l_map);

				m_vecDqzBxName.push_back(str_xzName[0]);
				m_vecMapDqzBxData.push_back(l_map);
			}
			else if (type == "����վ����")
			{
				vec_TxZhName.push_back(str_xzName[0]);
				vec_TxZhData.push_back(l_map);

				m_vecDqzTxName.push_back(str_xzName[0]);
				m_vecMapDqzTxData.push_back(l_map);
			}

			infile.close();             //�ر��ļ������� 
		}
	}
	/*********************�ѵ���վ�����ļ�ɸѡ����**********************************/
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
		if (str_xzName[0] == UTF8_To_string("�����ļ�_����վ"))
		{
			ifstream fin(fileN, ios::binary);
			unsigned char  s2;
			fin.read((char*)&s2, sizeof(s2));//��ȡ��һ���ֽڣ�Ȼ������8λ
			int p = s2 << 8;
			fin.read((char*)&s2, sizeof(s2));//��ȡ�ڶ����ֽ�
			p += s2;

			string code;
			fin.close();

			ifstream infile;
			infile.open(fileN);   //���ļ����������ļ��������� 
			//assert(infile.is_open());   //��ʧ��,�����������Ϣ,����ֹ�������� 
			if (infile)
			{
				string s;
				map<string, string> l_map;

				while (getline(infile, s))
				{
					vector<string> line;
					cout << "�ڶ���" << endl;
					cout << s.c_str() << endl;
					vector<string> str_;
					string pattern = ":";
					str_ = my_split(s, pattern);
					if (p == 58760 || p == 59032)//�ļ���UTF-8����
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
					else if (p == 46260 || p == 21364)//�ļ�����������
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
	///*********************�������Ÿۡ�����վ��treeView�ֵ�һ���ڵ�����*******************/
	for (int i = 0; i < vec_WxName.size(); i++)
	{
		int seques = 0;
		string xzName;//��������
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
	/***********************��������վ����Դ����������treeview*************************************/
	for (int i = 0; i < vec_TxZhName.size(); i++)
	{
		int seques = 0;
		string xzName;//��������
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
	vector<map<string, string>> vec_wxData;//�����ļ������������ݴ洢����
	vector<vector<string>> vecXZAndWX;//ÿһ�ж��������:����-����-������
	vector<wstring> upload_fileName;
	upload_fileName = UtilTool::GetOpenFilePaths_txt();
	/*********************�Ȱѵ����ն˵�ɸѡ������ʾ�ڽ�����*********************/
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
		fin.read((char*)&s2, sizeof(s2));//��ȡ��һ���ֽڣ�Ȼ������8λ
		int p = s2 << 8;
		fin.read((char*)&s2, sizeof(s2));//��ȡ�ڶ����ֽ�
		p += s2;

		string code;
		fin.close();

		ifstream infile;
		infile.open(fileN);   //���ļ����������ļ��������� 
		//assert(infile.is_open());   //��ʧ��,�����������Ϣ,����ֹ�������� 
		if (infile)
		{
			string s;
			map<string, string> l_map;
			bool flag = true;
			string type;
			while (getline(infile, s))
			{
				if (flag)//����ǵ�һ�� ��һ��д��
				{
					cout << "��һ��" << endl;
					type = s;

					flag = false;
				}
				else
				{
					cout << "�ڶ���" << endl;
					cout << s.c_str() << endl;
					vector<string> str_;
					string pattern = ":";
					str_ = my_split(s, pattern);
					if (p == 58760 || p == 59032)//�ļ���UTF-8����
					{
						l_map[str_[0]] = str_[1];
					}
					else if (p == 46260)//�ļ�����������
					{

						l_map[string_To_UTF8(str_[0])] = string_To_UTF8(str_[1]);
					}
					else
					{
						l_map[str_[0]] = str_[1];
					}
				}


			}
			if (type == "�����ն�")
			{
				cout << "�����ն�" << endl;
				AssignmentToLocalVector_DMZD(str_xzName[0], l_map);//�����������Ϣ��ʾ�ڽ�����
			}

			infile.close();             //�ر��ļ������� 
		}
		//m_FtpClientClass.execute_putFile(fileN);//put �ϴ��ļ�
	}
	/*********************�������ļ�ɸѡ����**********************************/
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
		if (str_xzName[0] == UTF8_To_string("�����ļ�_�����ն�"))
		{
			ifstream fin(fileN, ios::binary);
			unsigned char  s2;
			fin.read((char*)&s2, sizeof(s2));//��ȡ��һ���ֽڣ�Ȼ������8λ
			int p = s2 << 8;
			fin.read((char*)&s2, sizeof(s2));//��ȡ�ڶ����ֽ�
			p += s2;

			string code;
			fin.close();

			ifstream infile;
			infile.open(fileN);   //���ļ����������ļ��������� 
			//assert(infile.is_open());   //��ʧ��,�����������Ϣ,����ֹ�������� 
			if (infile)
			{
				string s;
				map<string, string> l_map;

				while (getline(infile, s))
				{
					vector<string> line;
					cout << "�ڶ���" << endl;
					cout << s.c_str() << endl;
					vector<string> str_;
					string pattern = ":";
					str_ = my_split(s, pattern);
					if (p == 58760 || p == 59032)//�ļ���UTF-8����
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
					else if (p == 46260 || p == 21364)//�ļ�����������
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
	/*********************�ѵ����ն��ӽڵ���غɡ����Ρ���������ɸѡ����*************************************/
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
		fin.read((char*)&s2, sizeof(s2));//��ȡ��һ���ֽڣ�Ȼ������8λ
		int p = s2 << 8;
		fin.read((char*)&s2, sizeof(s2));//��ȡ�ڶ����ֽ�
		p += s2;

		string code;
		fin.close();

		ifstream infile;
		infile.open(fileN);   //���ļ����������ļ��������� 
		//assert(infile.is_open());   //��ʧ��,�����������Ϣ,����ֹ�������� 
		if (infile)
		{
			string s;
			map<string, string> l_map;
			bool flag = true;
			string type;
			while (getline(infile, s))
			{
				if (flag)//����ǵ�һ�� ��һ��д��
				{
					cout << "��һ��" << endl;
					type = s;

					flag = false;
				}
				else
				{
					cout << "�ڶ���" << endl;
					cout << s.c_str() << endl;
					vector<string> str_;
					string pattern = ":";
					str_ = my_split(s, pattern);
					if (p == 58760 || p == 59032 || p == 58765)//�ļ���UTF-8����
					{
						l_map[str_[0]] = str_[1];
					}
					else if (p == 46260)//�ļ�����������
					{

						l_map[string_To_UTF8(str_[0])] = string_To_UTF8(str_[1]);
					}
					else
					{
						l_map[str_[0]] = str_[1];
					}
				}


			}
			if (type == "�غɲ�������")
			{
				cout << "�غɲ�������" << endl;
				vec_WxName.push_back(str_xzName[0]);
				vec_wxData.push_back(l_map);
				//AssignmentToLocalVector_XZ(str_xzName[0], l_map);
			}
			if (type == "�����ն��غ�")
			{
				vec_WxName.push_back(str_xzName[0]);
				vec_wxData.push_back(l_map);
				
				m_vecDmzdZhName.push_back(str_xzName[0]);
				m_vecMapDmzdZhData.push_back(l_map);
			}
			else if (type == "�����ն˲���")
			{
				vec_WxName.push_back(str_xzName[0]);
				vec_wxData.push_back(l_map);

				m_vecDmzdBxName.push_back(str_xzName[0]);
				m_vecMapDmzdBxData.push_back(l_map);
			}
			else if (type == "�����ն�����")
			{
				vec_WxName.push_back(str_xzName[0]);
				vec_wxData.push_back(l_map);

				m_vecDmzdTxName.push_back(str_xzName[0]);
				m_vecMapDmzdTxData.push_back(l_map);
			}


			infile.close();             //�ر��ļ������� 
		}
	}
	/*********************�������ǽڵ���treeView�ֵ�һ���ڵ�����*******************/
	for (int i = 0; i < vec_WxName.size(); i++)
	{
		int seques = 0;
		string xzName;//��������
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
//ˢ�½��� ��������0
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