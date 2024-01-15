#include "stdafx.h"
#include "CPopNetworkManage.h"

#include <sstream>
#include <io.h>
#include <direct.h>
#include <fstream>

#pragma execution_character_set("utf-8") ;

ClueScreenClass * m_ClueScreenClass;
#define ControlNumber_Net 50  //�ؼ���ֹ�� titles50 
#define ControlNumber_Draw1 15  //�ؼ���ֹ�� titles50 
//vector<string> seleceColumns;
//������������ʵ��
DUI_BEGIN_MESSAGE_MAP(CPopNetworkManage, WindowImplBase)
DUI_ON_MSGTYPE(DUI_MSGTYPE_CLICK, OnClick)
DUI_ON_MSGTYPE(DUI_MSGTYPE_ITEMSELECT, OnItemSelect)
DUI_ON_MSGTYPE(DUI_MSGTYPE_SELECTCHANGED, OnSelectChanged)
DUI_END_MESSAGE_MAP()
CPopNetworkManage::CPopNetworkManage(void)
{
	m_pMenu = NULL;
}

CPopNetworkManage::~CPopNetworkManage(void)
{

	CMenuWnd::DestroyMenu();
	if (m_pMenu != NULL) {
		delete m_pMenu;
		m_pMenu = NULL;
	}

}

void CPopNetworkManage::OnFinalMessage(HWND hWnd)
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
template <class Type>
Type stringToNum(const string& str)
{
	istringstream iss(str);
	Type num;
	iss >> num;
	return num;
}
DuiLib::CDuiString CPopNetworkManage::GetSkinFile()
{
	return _T("NetworkManagement.xml");
}

LPCTSTR CPopNetworkManage::GetWindowClassName(void) const
{
	return _T("NetworkManagement");
}

void CPopNetworkManage::OnClick(TNotifyUI &msg)
{
	CDuiString sName = msg.pSender->GetName();
	sName.MakeLower();
	if (msg.pSender == m_pCloseBtn) {
		Close(0);
		return;
	}
	else if (sName.CompareNoCase(_T("bt_save")) == 0)////tab2 ���� bt_save
	{

		string FileName = wstringToString(m_modiftName_2.GetData());

		int sequences;//�����vector�е�˳��
		for (int i = 0; i < vec_level2_1.size(); i++)
		{
			if (FileName == vec_level2_1[i])
			{
				sequences = i;
			}
		}
		
		map<string, string> map_inform = m_mapLevel2_1[sequences];;//�������޸����map 
		//FindTheMap_2();
		int num_map = map_inform.size();

		HideThCcontrol_2(num_map, TRUE);

		map<string, string> map_informRec;//�������޸����map
		map_informRec = GetDataFromInterfaces(num_map);
		m_mapLevel2_1[sequences] = map_informRec;
		//string FileName = wstringToString(m_modiftName_2.GetData());
		//for (int i = 0; i < vec_level2_1.size(); i++)
		//{
		//	if (vec_level2_1[i] == FileName)
		//	{
		//		m_mapLevel2_1[i] = map_inform;
		//	}
		//}
	}
	else if (sName.CompareNoCase(_T("bt_Tree1_SaveAs")) == 0)////tab1 ���Ϊ
	{
		string path;
		path = "D:\\DATA1\\";
		//bool flag = CreateDirectory(_T("D:\\DATA1\\HH"), NULL);
		m_CPopNewConstellation->MessageBox(NULL);
		string name = m_CPopNewConstellation->GetNewName();//	string GetNewName();//��ȡ�޸ĺ������
		path += name;
		bool flag = CreateDirectory(stringToWstring(path).c_str(), NULL);
		vector< vector<string>> vec_XZAndWX;
		vector<string> vec_child;

		for (int i = 0; i < vec_recSatelliteName.size(); i++)//����
		{
			GenerateTheFile(path, vec_recSatelliteName[i], m_RecvSatelliteMap[i],"����");
		}

		for (int i = 0; i < vec_recEarthStationName.size(); i++)//����վ
		{
			GenerateTheFile(path, vec_recEarthStationName[i], m_RecvEarthStationMap[i],"����վ");
		}


		for (int i = 0; i < vec_recGroundTerminalName.size(); i++)//�����ն�
		{
			GenerateTheFile(path, vec_recGroundTerminalName[i], m_RecvGroundTerminalMap[i],"�����ն�");
		}

		/********************���������豸��Ӧ�Ĺ�ϵ�洢********************/
		for (int i = 0; i < pNode_1_num; i++)
		{
			vector<string> vec;
			int childNum = pNode_1[i]->GetCountChild();
			vec.push_back(wstringToString(pNode_1[i]->GetItemText().GetData()));
			for (int j = 0; j < childNum; j++)
			{

				CTreeNodeUI*  node = pNode_1[i]->GetChildNode(j);//GetParentNode
				string childName = wstringToString(node->GetItemText().GetData());
				vec.push_back(childName);
			}
			vec_XZAndWX.push_back(vec);
		}
		GenerateTheFile(path, UTF8_To_string("�����ļ�"), vec_XZAndWX);
	}
	else if (sName.CompareNoCase(_T("bt_Tree1_Import")) == 0)//tab1 ����
	{

		RefreshTheInterface_tab1();



		COptionUI* pControl = static_cast<COptionUI*>(m_pm.FindControl(_T("optical_ScenarioData")));//��������
		if (pControl->IsSelected())
		{

			cout << "tab1��������" << endl;

			RealTimeSceneData_Tab1();//��ʼ����ʵʱ�������� ������ģ�������
			//NotDrawable("tab1", true);
		}
		COptionUI* pControl_1 = static_cast<COptionUI*>(m_pm.FindControl(_T("optical_LocalData")));//��������
		if (pControl_1->IsSelected())
		{

			ImportingLocalData_Tab1();//���뱾������
			//NotDrawable("tab1", false);
		}
	}
	else if (sName.CompareNoCase(_T("bt_Tree1_SaveAs2")) == 0)////tab2 ���Ϊ
	{
		string path;
		path = "D:\\DATA1\\";
		//bool flag = CreateDirectory(_T("D:\\DATA1\\HH"), NULL);
		m_CPopNewConstellation->MessageBox(NULL);
		string name = m_CPopNewConstellation->GetNewName();//	string GetNewName();//��ȡ�޸ĺ������
		path += name;
		bool flag = CreateDirectory(stringToWstring(path).c_str(), NULL);
		vector< vector<string>> vec_XZAndWX;
		vector<string> vec_child;
		for (int i = 0; i < vec_level2_1.size(); i++)
		{
			for (int j = 0; j < vec_recSatelliteName.size(); j++)
			{
				if (vec_level2_1[i] == vec_recSatelliteName[j])
				{
					GenerateTheFile(path, vec_recSatelliteName[j], m_mapLevel2_1[i], UTF8_To_string("����"));
				}
			}

			for (int j = 0; j < vec_recEarthStationName.size(); j++)
			{
				if (vec_level2_1[i] == vec_recEarthStationName[j])
				{
					GenerateTheFile(path, vec_recEarthStationName[j], m_mapLevel2_1[i], UTF8_To_string("����վ"));
				}
			}

			for (int j = 0; j < vec_recGroundTerminalName.size(); j++)
			{
				if (vec_level2_1[i] == vec_recGroundTerminalName[j])
				{
					GenerateTheFile(path, vec_recGroundTerminalName[j], m_mapLevel2_1[i], UTF8_To_string("�����ն�"));
				}
			}
		}


		/********************���������豸��Ӧ�Ĺ�ϵ�洢********************/
		for (int i = 0; i < pNode_2_1_num; i++)
		{
			vector<string> vec;
			int childNum = pNode_2_1[i]->GetCountChild();
			vec.push_back(wstringToString(pNode_2_1[i]->GetItemText().GetData()));
			for (int j = 0; j < childNum; j++)
			{

				CTreeNodeUI*  node = pNode_2_1[i]->GetChildNode(j);//GetParentNode
				string childName = wstringToString(node->GetItemText().GetData());
				vec.push_back(childName);
			}
			vec_XZAndWX.push_back(vec);
		}
		GenerateTheFile(path, UTF8_To_string("�����ļ�"), vec_XZAndWX);

	}
	else if (sName.CompareNoCase(_T("bt_Tree1_Import2")) == 0)//tab2 ����
	{
		RefreshTheInterface_tab2();

		COptionUI* pControl = static_cast<COptionUI*>(m_pm.FindControl(_T("optical_ScenarioData2")));//��������
		if (pControl->IsSelected())
		{

			cout << "tab2��������" << endl;
			RealTimeSceneData_Tab2();
			//NotDrawable("tab2", true);
		}
		COptionUI* pControl_1 = static_cast<COptionUI*>(m_pm.FindControl(_T("optical_LocalData2")));//��������
		if (pControl_1->IsSelected())
		{

			ImportingLocalData_Tab2();//���뱾������
			//NotDrawable("tab2", false);
		}
	}
	else if (sName.CompareNoCase(_T("bt_Tree1_SaveAs3")) == 0)////tab3 ���Ϊ
	{
		string path;
		path = "D:\\DATA1\\";
		//bool flag = CreateDirectory(_T("D:\\DATA1\\HH"), NULL);
		m_CPopNewConstellation->MessageBox(NULL);
		string name = m_CPopNewConstellation->GetNewName();//	string GetNewName();//��ȡ�޸ĺ������
		path += name;
		bool flag = CreateDirectory(stringToWstring(path).c_str(), NULL);
		vector< vector<string>> vec_XZAndWX;
		vector<string> vec_child;
		for (int i = 0; i < vec_level3_1.size(); i++)
		{
			for (int j = 0; j < vec_recSatelliteName.size(); j++)
			{
				if (vec_level3_1[i] == vec_recSatelliteName[j])
				{
					GenerateTheFile(path, vec_recSatelliteName[j], m_mapLevel3_1[i], UTF8_To_string("����"));
				}
			}

			for (int j = 0; j < vec_recEarthStationName.size(); j++)
			{
				if (vec_level3_1[i] == vec_recEarthStationName[j])
				{
					GenerateTheFile(path, vec_recEarthStationName[j], m_mapLevel3_1[i], UTF8_To_string("����վ"));
				}
			}

			for (int j = 0; j < vec_recGroundTerminalName.size(); j++)
			{
				if (vec_level3_1[i] == vec_recGroundTerminalName[j])
				{
					GenerateTheFile(path, vec_recGroundTerminalName[j], m_mapLevel3_1[i], UTF8_To_string("�����ն�"));
				}
			}
		}


		/********************���������豸��Ӧ�Ĺ�ϵ�洢********************/
		for (int i = 0; i < pNode_3_1_num; i++)
		{
			vector<string> vec;
			int childNum = pNode_3_1[i]->GetCountChild();
			vec.push_back(wstringToString(pNode_3_1[i]->GetItemText().GetData()));
			for (int j = 0; j < childNum; j++)
			{

				CTreeNodeUI*  node = pNode_3_1[i]->GetChildNode(j);//GetParentNode
				string childName = wstringToString(node->GetItemText().GetData());
				vec.push_back(childName);
			}
			vec_XZAndWX.push_back(vec);
		}
		GenerateTheFile(path, UTF8_To_string("�����ļ�"), vec_XZAndWX);
	}
	else if (sName.CompareNoCase(_T("bt_Tree1_Import3")) == 0)//tab3 ����
	{
		RefreshTheInterface_tab3();

		COptionUI* pControl = static_cast<COptionUI*>(m_pm.FindControl(_T("optical_ScenarioData3")));//��������
		if (pControl->IsSelected())
		{
			cout << "tab3��������" << endl;
			RealTimeSceneData_Tab3();
			//NotDrawable("tab3", true);
		}
		COptionUI* pControl_1 = static_cast<COptionUI*>(m_pm.FindControl(_T("optical_LocalData3")));//��������
		if (pControl_1->IsSelected())
		{

			ImportingLocalData_Tab3();//���뱾������
			//NotDrawable("tab3", false);
		}
	}
	else if (sName.CompareNoCase(_T("bt_Drawing_tab1")) == 0)//ȷ����ť tab1��
	{
		vec_SequenceDisplayTab1.clear();
		//memset(tab1Group, 0, sizeof(tab1Group));//tab1�������
		vector<string> vec_WX;
		vector<string> vec_DQZ;
		vector<string> vec_DMZD;
		for (int i = 0; i < vec_StartEndPointTab1.size(); i++)
		{
			for (int j = 0; j < vec_recSatelliteName.size(); j++)
			{
				if (vec_StartEndPointTab1[i] == vec_recSatelliteName[j])//ɸѡ����
				{
					vec_WX.push_back(vec_StartEndPointTab1[i]);
				}
			}
			for (int j = 0; j < vec_recEarthStationName.size(); j++)
			{
				if (vec_StartEndPointTab1[i] == vec_recEarthStationName[j])//ɸѡ����վ
				{
					vec_DQZ.push_back(vec_StartEndPointTab1[i]);
				}
			}
			for (int j = 0; j < vec_recGroundTerminalName.size(); j++)
			{
				if (vec_StartEndPointTab1[i] == vec_recGroundTerminalName[j])//ɸѡ�����ն�
				{
					vec_DMZD.push_back(vec_StartEndPointTab1[i]);
				}
			}
		}
		//�����ǡ�����վ�������ն�����
		for (int i = 0; i < vec_WX.size(); i++)
		{
			vec_SequenceDisplayTab1.push_back(vec_WX[i]);
		}
		for (int i = 0; i < vec_DQZ.size(); i++)
		{
			vec_SequenceDisplayTab1.push_back(vec_DQZ[i]);
		}
		for (int i = 0; i < vec_DMZD.size(); i++)
		{
			vec_SequenceDisplayTab1.push_back(vec_DMZD[i]);
		}
		vector<double> vec_pack = AssemblePackageFMU_tab1(vec_WX, vec_DQZ, vec_DMZD);
		//��ȡ��㡢�յ���vec_SequenceDisplayTab1�е�λ��
		string str_path; //D:\0_TwoDimensional\��Ӳ���������_14_23\gamebox
		//wstring wstr;
		//unsigned long size = GetCurrentDirectory(0, NULL);
		//wchar_t *path = new wchar_t[size];
		//if (GetCurrentDirectory(size, path) != 0)
		//{
		//	wstr = path;
		//}

		str_path = CurrentPath + "\\atspaceNet.fmu";
		vector<double> vec_recv = m_CallFmuClass->CallTheFmuInterface(str_path,vec_pack);
		vector<string> vec_DeviceName;
		for (int i = 0; i < vec_recv.size(); i++)//�����ص���Ż�����Ӧ�����ǡ�����վ�������ն�����
		{
			if (vec_recv[i] > 0)
			{
				vec_DeviceName.push_back(vec_SequenceDisplayTab1[vec_recv[i]-1]);
			}
		}
		vector<string> vec_SelectWX;
		vector<string> vec_SelectDQZ;
		vector<string> vec_SelectDMZD;
		for (int i = 0; i < vec_DeviceName.size(); i++)//��ѡ�е��豸�ֿ� ���ǡ�����վ�������ն˷ֿ�
		{
			for (int j = 0; j < vec_recSatelliteName.size(); j++)
			{
				if (vec_DeviceName[i] == vec_recSatelliteName[j])//ɸѡ����
				{
					vec_SelectWX.push_back(vec_DeviceName[i]);
				}
			}
			for (int j = 0; j < vec_recEarthStationName.size(); j++)
			{
				if (vec_DeviceName[i] == vec_recEarthStationName[j])//ɸѡ����վ
				{
					vec_SelectDQZ.push_back(vec_DeviceName[i]);
				}
			}
			for (int j = 0; j < vec_recGroundTerminalName.size(); j++)
			{
				if (vec_DeviceName[i] == vec_recGroundTerminalName[j])//ɸѡ�����ն�
				{
					vec_SelectDMZD.push_back(vec_DeviceName[i]);
				}
			}
		}
		
		m_DrawingInterfaceClass->MessageBox(NULL, vec_WX, vec_DQZ, vec_DMZD, vec_SelectWX, vec_SelectDQZ, vec_SelectDMZD);
	}
	else if (sName.CompareNoCase(_T("bt_AddAttributes_tab2")) == 0)//�������ǡ�����վ�������ն�
	{
		m_CPopNewAttributeClass_Net->MessageBox(NULL);//m_CPopNewAttributeClass

		vector<string> vec = m_CPopNewAttributeClass_Net->GetNewName();//��ȡ�����ӵ�
		if (vec.size() > 0)//�����������Լӵ�������Ե�vector������
		{
	
			for (int i = 0; i < vec_level2_1.size(); i++)
			{
				map<string, string> map_level2 = m_mapLevel2_1[i];
				m_mapLevel2_1[i][vec[0]] = vec[1];
			}
			
		}
		//string FileName = wstringToString(m_modiftName_2.GetData());
		FindTheMap_2();
	}//bt_DeleteAttribute_tab2
	else if (sName.CompareNoCase(_T("bt_DeleteAttribute_tab2")) == 0)//ɾ�����ǡ�����վ�������ն�
	{
		vector<string> vec_name;
		int flag = -1;
		map<string, string> map_inform;//�������޸����map
		string FileName = wstringToString(m_modiftName_2.GetData());
		int sequese;
		for (int i = 0; i < vec_level2_1.size(); i++)
		{
			if (vec_level2_1[i] == FileName)
			{
				map_inform = m_mapLevel2_1[i];//m_RecvMap_XZ[i] = map_inform;
				sequese = i;
				flag = 0;
			}
		}
		for (map<string, string>::iterator iter = map_inform.begin(); iter != map_inform.end(); ++iter)
		{
			cout << "key:" << iter->first << " value:" << iter->second << endl;
			vec_name.push_back(string_To_UTF8(iter->first));
		}
		m_CPopDeleteAttClass->MessageBox(NULL, vec_name);//m_CPopNewAttributeClass
		//map<string, string> map_inform1 = m_RecvMap_DXG[sequese];
		vector<string> vec_deleteName = m_CPopDeleteAttClass->GetDeleteName();

		if (vec_deleteName.size() > 0)//�����������Լӵ�������Ե�vector������
		{

			for (int i = 0; i < vec_level2_1.size(); i++)
			{
				map<string, string> map_level2 = m_mapLevel2_1[i];
				for (map<string, string>::iterator iter = map_inform.begin(); iter != map_inform.end(); ++iter)
				{
					cout << "key:" << iter->first << " value:" << iter->second << endl;
					for (int i = 0; i < vec_deleteName.size(); i++)
					{
						if (string_To_UTF8(iter->first) == vec_deleteName[i])
						{
							//cout << "ѡ��" << endl;
							map_level2.erase(iter->first);
						}

					}
				}
				m_mapLevel2_1[i] = map_level2;
			}

		}
		HideThCcontrol_2(20,FALSE);
		FindTheMap_2();
		cout << "11111111" << endl;

	}
	else if (sName.CompareNoCase(_T("bt_Sift_tab2")) == 0)//tab2 ɸѡ��ť
	{
		CControlUI* pControl_1 = static_cast<CControlUI*>(m_pm.FindControl(stringToWstring("edit_FilterCriteria").c_str()));
		CDuiString cdui_siftType;	//pControl_1->SetText
		cdui_siftType = pControl_1->GetText();
		siftType = wstringToString(cdui_siftType.GetData());//tab2��ѡ���ɸѡ����

		CControlUI* pControl_2 = static_cast<CControlUI*>(m_pm.FindControl(stringToWstring("edit_FilterCriteriaValue").c_str()));
		CDuiString cdui_siftTypeValue;	//pControl_1->SetText
		cdui_siftTypeValue = pControl_2->GetText();
		siftTypeValue = wstringToString(cdui_siftTypeValue.GetData());//tab2��ѡ���ɸѡ����

		vector<string> vec_child;

		vector<string> vec_SheBei;//���Ҫ�����豸

		int num_child = pNode_2_1[selectNode_2[0][0]]->GetCountChild();//GetParentNode
		/******************��ȡ��ѡ������������豸 (��ǰ0���ڵ������һ���ڵ�����)******************/
		for (int i = 0; i < num_child; i++)
		{
			CTreeNodeUI*  node = pNode_2_1[selectNode_2[0][0]]->GetChildNode(i);//GetParentNode
			CDuiString cdui_name = node->GetItemText();
			wstring wstr_name = cdui_name.GetData();
			vec_child.push_back(wstringToString(wstr_name));
		}
		/****************����������������������ɸѡ������*************/
		for (int i = 0; i < vec_child.size(); i++)
		{
			int sequence;
			for (int j = 0; j < vec_level2_1.size(); j++)
			{
				if (vec_child[i] == vec_level2_1[j])
				{
					sequence = j;
				}
			}
			map<string, string> curr_map = m_mapLevel2_1[sequence];
			if (curr_map.count(siftType) > 0)
			{
				string value = curr_map[siftType];//�͹�(LEO)/�й�(MEO)/�߹�(HEO)
				if (value == siftTypeValue)
				{
					vec_SheBei.push_back(vec_child[i]);//���Ҫ�ŵĵ͹�(LEO)/�й�(MEO)/�߹�(HEO)
				}

			}
		}
		vec_StartEndPointTab2.clear();
		//parseElements_2();
		//����ǰ�����ڵ����ǡ�����վ�������ն˸���Combox
		CComboBoxUI* Com_StartingPoint = static_cast<CComboBoxUI*>(m_pm.FindControl(_T("Com_StartingPoint_tab2")));//���
		CComboBoxUI* Com_EndPoint = static_cast<CComboBoxUI*>(m_pm.FindControl(_T("Com_EndPoint_tab2")));;//�յ�
		Com_StartingPoint->RemoveAll();
		Com_EndPoint->RemoveAll();
		for (int i = 0; i < vec_SheBei.size(); i++)//����ѡ�������е��豸������ʾ��Combox�й��û�ѡ�������յ�
		{
			wstring temp;
			CListLabelElementUI * pLabel = new CListLabelElementUI();
			CListLabelElementUI * pLabel_1 = new CListLabelElementUI();
			UtilTool::setWstring(temp, vec_SheBei[i].c_str());
			vec_StartEndPointTab2.push_back(vec_SheBei[i]);
			pLabel->SetText(temp.c_str());
			pLabel->SetBkColor(0xff0000);
			pLabel_1->SetText(temp.c_str());
			pLabel_1->SetBkColor(0xff0000);
			if (Com_StartingPoint)
				Com_StartingPoint->AddAt(pLabel, i);
			if (Com_EndPoint)
				Com_EndPoint->AddAt(pLabel_1, i);
		}
		Com_StartingPoint->SelectItem(0);
		Com_EndPoint->SelectItem(0);
	}
	else if (sName.CompareNoCase(_T("bt_Drawing_tab2")) == 0)//
	{
		vec_SequenceDisplayTab2.clear();
		//memset(tab1Group, 0, sizeof(tab1Group));//tab1�������
		vector<string> vec_WX;
		vector<string> vec_DQZ;
		vector<string> vec_DMZD;
		for (int i = 0; i < vec_StartEndPointTab2.size(); i++)
		{
			for (int j = 0; j < vec_recSatelliteName.size(); j++)
			{
				if (vec_StartEndPointTab2[i] == vec_recSatelliteName[j])//ɸѡ����
				{
					vec_WX.push_back(vec_StartEndPointTab2[i]);
				}
			}
			for (int j = 0; j < vec_recEarthStationName.size(); j++)
			{
				if (vec_StartEndPointTab2[i] == vec_recEarthStationName[j])//ɸѡ����վ
				{
					vec_DQZ.push_back(vec_StartEndPointTab2[i]);
				}
			}
			for (int j = 0; j < vec_recGroundTerminalName.size(); j++)
			{
				if (vec_StartEndPointTab2[i] == vec_recGroundTerminalName[j])//ɸѡ�����ն�
				{
					vec_DMZD.push_back(vec_StartEndPointTab2[i]);
				}
			}
		}
		//�����ǡ�����վ�������ն�����
		for (int i = 0; i < vec_WX.size(); i++)
		{
			vec_SequenceDisplayTab2.push_back(vec_WX[i]);
		}
		for (int i = 0; i < vec_DQZ.size(); i++)
		{
			vec_SequenceDisplayTab2.push_back(vec_DQZ[i]);
		}
		for (int i = 0; i < vec_DMZD.size(); i++)
		{
			vec_SequenceDisplayTab2.push_back(vec_DMZD[i]);
		}
		vector<double> vec_pack = AssemblePackageFMU_tab2(vec_WX, vec_DQZ, vec_DMZD);
		//��ȡ��㡢�յ���vec_SequenceDisplayTab1�е�λ��
		string str_path; //D:\0_TwoDimensional\��Ӳ���������_14_23\gamebox


		str_path = CurrentPath + "\\atspaceNet.fmu";
		vector<double> vec_recv = m_CallFmuClass->CallTheFmuInterface(str_path, vec_pack);
		vector<string> vec_DeviceName;
		for (int i = 0; i < vec_recv.size(); i++)//�����ص���Ż�����Ӧ�����ǡ�����վ�������ն�����
		{
			if (vec_recv[i] > 0)
			{
				vec_DeviceName.push_back(vec_SequenceDisplayTab2[vec_recv[i] - 1]);
			}
		}
		vector<string> vec_SelectWX;
		vector<string> vec_SelectDQZ;
		vector<string> vec_SelectDMZD;
		for (int i = 0; i < vec_DeviceName.size(); i++)//��ѡ�е��豸�ֿ� ���ǡ�����վ�������ն˷ֿ�
		{
			for (int j = 0; j < vec_recSatelliteName.size(); j++)
			{
				if (vec_DeviceName[i] == vec_recSatelliteName[j])//ɸѡ����
				{
					vec_SelectWX.push_back(vec_DeviceName[i]);
				}
			}
			for (int j = 0; j < vec_recEarthStationName.size(); j++)
			{
				if (vec_DeviceName[i] == vec_recEarthStationName[j])//ɸѡ����վ
				{
					vec_SelectDQZ.push_back(vec_DeviceName[i]);
				}
			}
			for (int j = 0; j < vec_recGroundTerminalName.size(); j++)
			{
				if (vec_DeviceName[i] == vec_recGroundTerminalName[j])//ɸѡ�����ն�
				{
					vec_SelectDMZD.push_back(vec_DeviceName[i]);
				}
			}
		}

		m_DrawingInterfaceClass->MessageBox(NULL, vec_WX, vec_DQZ, vec_DMZD, vec_SelectWX, vec_SelectDQZ, vec_SelectDMZD);
	}
	else if (sName.CompareNoCase(_T("bt_AddAttributes_tab3")) == 0)//tab3 ��������
	{
		m_CPopNewAttributeClass_Net->MessageBox(NULL);//m_CPopNewAttributeClass

		vector<string> vec = m_CPopNewAttributeClass_Net->GetNewName();//��ȡ�����ӵ�
		if (vec.size() > 0)//�����������Լӵ�������Ե�vector������
		{

			for (int i = 0; i < vec_level3_1.size(); i++)
			{
				map<string, string> map_level3 = m_mapLevel3_1[i];
				m_mapLevel3_1[i][vec[0]] = vec[1];
			}

		}
		//string FileName = wstringToString(m_modiftName_2.GetData());
		FindTheMap_3();
	}
	else if (sName.CompareNoCase(_T("bt_DeleteAttribute_tab3")) == 0)//ɾ������
	{
		vector<string> vec_name;
		int flag = -1;
		map<string, string> map_inform;//�������޸����map
		string FileName = wstringToString(m_modiftName_3.GetData());
		int sequese;
		for (int i = 0; i < vec_level3_1.size(); i++)
		{
			if (vec_level3_1[i] == FileName)
			{
				map_inform = m_mapLevel3_1[i];//m_RecvMap_XZ[i] = map_inform;
				sequese = i;
				flag = 0;
			}
		}
		for (map<string, string>::iterator iter = map_inform.begin(); iter != map_inform.end(); ++iter)
		{
			cout << "key:" << iter->first << " value:" << iter->second << endl;
			vec_name.push_back(string_To_UTF8(iter->first));
		}
		m_CPopDeleteAttClass->MessageBox(NULL, vec_name);//m_CPopNewAttributeClass
		//map<string, string> map_inform1 = m_RecvMap_DXG[sequese];
		vector<string> vec_deleteName = m_CPopDeleteAttClass->GetDeleteName();

		if (vec_deleteName.size() > 0)//�����������Լӵ�������Ե�vector������
		{

			for (int i = 0; i < vec_level3_1.size(); i++)
			{
				map<string, string> map_level3 = m_mapLevel3_1[i];
				for (map<string, string>::iterator iter = map_inform.begin(); iter != map_inform.end(); ++iter)
				{
					cout << "key:" << iter->first << " value:" << iter->second << endl;
					for (int i = 0; i < vec_deleteName.size(); i++)
					{
						if (string_To_UTF8(iter->first) == vec_deleteName[i])
						{
							//cout << "ѡ��" << endl;
							map_level3.erase(iter->first);
						}

					}
				}
				m_mapLevel3_1[i] = map_level3;
			}

		}
		HideThCcontrol_3(20, FALSE);
		FindTheMap_3();
		cout << "11111111" << endl;
	}
	else if (sName.CompareNoCase(_T("bt_save_tab3")) == 0)////tab3 ����
	{

		string FileName1 = wstringToString(m_modiftName_3.GetData());

		int sequences;//�����vector�е�˳��
		for (int i = 0; i < vec_level3_1.size(); i++)
		{
			if (FileName1 == vec_level3_1[i])
			{
				sequences = i;
			}
		}

		map<string, string> map_inform = m_mapLevel3_1[sequences];;//�������޸����map 
		//FindTheMap_2();
		int num_map = map_inform.size();

		HideThCcontrol_3(num_map, TRUE);

		map<string, string> map_informRec;//�������޸����map
		map_informRec = GetDataFromInterfaces_Tab3(num_map);
		m_mapLevel3_1[sequences] = map_informRec;
		string FileName = wstringToString(m_modiftName_2.GetData());
		for (int i = 0; i < vec_level3_1.size(); i++)
		{
			if (vec_level3_1[i] == FileName)
			{
				m_mapLevel3_1[i] = map_inform;
			}
		}
	}
	else if (sName.CompareNoCase(_T("bt_Sift_tab3")) == 0)
	{
		CControlUI* pControl_1 = static_cast<CControlUI*>(m_pm.FindControl(stringToWstring("edit_FilterCriteria_tab3").c_str()));
		CDuiString cdui_siftType;	//pControl_1->SetText
		cdui_siftType = pControl_1->GetText();
		siftType = wstringToString(cdui_siftType.GetData());//tab2��ѡ���ɸѡ����

		CControlUI* pControl_2 = static_cast<CControlUI*>(m_pm.FindControl(stringToWstring("edit_FilterCriteriaValue_tab3").c_str()));
		CDuiString cdui_siftTypeValue;	//pControl_1->SetText
		cdui_siftTypeValue = pControl_2->GetText();
		siftTypeValue = wstringToString(cdui_siftTypeValue.GetData());//tab2��ѡ���ɸѡ����

		vector<string> vec_child;

		vector<string> vec_SheBei;//���Ҫ�����豸

		int num_child = pNode_3_1[selectNode_3[0][0]]->GetCountChild();//GetParentNode
		/******************��ȡ��ѡ������������豸 (��ǰ0���ڵ������һ���ڵ�����)******************/
		for (int i = 0; i < num_child; i++)
		{
			CTreeNodeUI*  node = pNode_3_1[selectNode_3[0][0]]->GetChildNode(i);//GetParentNode
			CDuiString cdui_name = node->GetItemText();
			wstring wstr_name = cdui_name.GetData();
			vec_child.push_back(wstringToString(wstr_name));
		}
		/****************����������������������ɸѡ������*************/
		for (int i = 0; i < vec_child.size(); i++)
		{
			int sequence;
			for (int j = 0; j < vec_level3_1.size(); j++)
			{
				if (vec_child[i] == vec_level3_1[j])
				{
					sequence = j;
				}
			}
			map<string, string> curr_map = m_mapLevel3_1[sequence];
			if (curr_map.count(siftType) > 0)
			{
				string value = curr_map[siftType];//�͹�(LEO)/�й�(MEO)/�߹�(HEO)
				if (value == siftTypeValue)
				{
					vec_SheBei.push_back(vec_child[i]);//���Ҫ�ŵĵ͹�(LEO)/�й�(MEO)/�߹�(HEO)
				}

			}
		}
		vec_StartEndPointTab3.clear();
		//parseElements_2();
		//����ǰ�����ڵ����ǡ�����վ�������ն˸���Combox
		CComboBoxUI* Com_StartingPoint = static_cast<CComboBoxUI*>(m_pm.FindControl(_T("Com_StartingPoint_tab3")));//���
		CComboBoxUI* Com_EndPoint = static_cast<CComboBoxUI*>(m_pm.FindControl(_T("Com_EndPoint_tab3")));;//�յ�
		Com_StartingPoint->RemoveAll();
		Com_EndPoint->RemoveAll();
		for (int i = 0; i < vec_SheBei.size(); i++)//����ѡ�������е��豸������ʾ��Combox�й��û�ѡ�������յ�
		{
			wstring temp;
			CListLabelElementUI * pLabel = new CListLabelElementUI();
			CListLabelElementUI * pLabel_1 = new CListLabelElementUI();
			UtilTool::setWstring(temp, vec_SheBei[i].c_str());
			vec_StartEndPointTab3.push_back(vec_SheBei[i]);
			pLabel->SetText(temp.c_str());
			pLabel->SetBkColor(0xff0000);
			pLabel_1->SetText(temp.c_str());
			pLabel_1->SetBkColor(0xff0000);
			if (Com_StartingPoint)
				Com_StartingPoint->AddAt(pLabel, i);
			if (Com_EndPoint)
				Com_EndPoint->AddAt(pLabel_1, i);
		}
		Com_StartingPoint->SelectItem(0);
		Com_EndPoint->SelectItem(0);
	}
	else if (sName.CompareNoCase(_T("bt_Drawing_tab3")) == 0)//
	{
		vec_SequenceDisplayTab3.clear();
		//memset(tab1Group, 0, sizeof(tab1Group));//tab1�������
		vector<string> vec_WX;
		vector<string> vec_DQZ;
		vector<string> vec_DMZD;
		for (int i = 0; i < vec_StartEndPointTab3.size(); i++)
		{
			for (int j = 0; j < vec_recSatelliteName.size(); j++)
			{
				if (vec_StartEndPointTab3[i] == vec_recSatelliteName[j])//ɸѡ����
				{
					vec_WX.push_back(vec_StartEndPointTab3[i]);
				}
			}
			for (int j = 0; j < vec_recEarthStationName.size(); j++)
			{
				if (vec_StartEndPointTab3[i] == vec_recEarthStationName[j])//ɸѡ����վ
				{
					vec_DQZ.push_back(vec_StartEndPointTab3[i]);
				}
			}
			for (int j = 0; j < vec_recGroundTerminalName.size(); j++)
			{
				if (vec_StartEndPointTab3[i] == vec_recGroundTerminalName[j])//ɸѡ�����ն�
				{
					vec_DMZD.push_back(vec_StartEndPointTab3[i]);
				}
			}
		}
		//�����ǡ�����վ�������ն�����
		for (int i = 0; i < vec_WX.size(); i++)
		{
			vec_SequenceDisplayTab3.push_back(vec_WX[i]);
		}
		for (int i = 0; i < vec_DQZ.size(); i++)
		{
			vec_SequenceDisplayTab3.push_back(vec_DQZ[i]);
		}
		for (int i = 0; i < vec_DMZD.size(); i++)
		{
			vec_SequenceDisplayTab3.push_back(vec_DMZD[i]);
		}
		vector<double> vec_pack = AssemblePackageFMU_tab3(vec_WX, vec_DQZ, vec_DMZD);
		//��ȡ��㡢�յ���vec_SequenceDisplayTab1�е�λ��
		string str_path; //D:\0_TwoDimensional\��Ӳ���������_14_23\gamebox


		str_path = CurrentPath + "\\atspaceNet.fmu";
		vector<double> vec_recv = m_CallFmuClass->CallTheFmuInterface(str_path, vec_pack);
		vector<string> vec_DeviceName;
		for (int i = 0; i < vec_recv.size(); i++)//�����ص���Ż�����Ӧ�����ǡ�����վ�������ն�����
		{
			if (vec_recv[i] > 0)
			{
				vec_DeviceName.push_back(vec_SequenceDisplayTab3[vec_recv[i] - 1]);
			}
		}
		vector<string> vec_SelectWX;
		vector<string> vec_SelectDQZ;
		vector<string> vec_SelectDMZD;
		for (int i = 0; i < vec_DeviceName.size(); i++)//��ѡ�е��豸�ֿ� ���ǡ�����վ�������ն˷ֿ�
		{
			for (int j = 0; j < vec_recSatelliteName.size(); j++)
			{
				if (vec_DeviceName[i] == vec_recSatelliteName[j])//ɸѡ����
				{
					vec_SelectWX.push_back(vec_DeviceName[i]);
				}
			}
			for (int j = 0; j < vec_recEarthStationName.size(); j++)
			{
				if (vec_DeviceName[i] == vec_recEarthStationName[j])//ɸѡ����վ
				{
					vec_SelectDQZ.push_back(vec_DeviceName[i]);
				}
			}
			for (int j = 0; j < vec_recGroundTerminalName.size(); j++)
			{
				if (vec_DeviceName[i] == vec_recGroundTerminalName[j])//ɸѡ�����ն�
				{
					vec_SelectDMZD.push_back(vec_DeviceName[i]);
				}
			}
		}

		m_DrawingInterfaceClass->MessageBox(NULL, vec_WX, vec_DQZ, vec_DMZD, vec_SelectWX, vec_SelectDQZ, vec_SelectDMZD);
	}

}
wstring CPopNetworkManage::stringToWstring(string str)
{

	wstring w_str;
	UtilTool::setWstring(w_str, str.c_str());
	return w_str;


}
//wstringתstring
string CPopNetworkManage::wstringToString(wstring wstr)
{
	string str;
	UtilTool::setString(str, wstr.c_str());
	return str;
}

string CPopNetworkManage::UTF8_To_string(const std::string & str)
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
string CPopNetworkManage::string_To_UTF8(const std::string & str)
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

vector<string> CPopNetworkManage::my_split(string str, string pattern)
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
void CPopNetworkManage::OnSelectChanged(TNotifyUI &msg)
{
	CDuiString sName = msg.pSender->GetName();
	sName.MakeLower();

}

void CPopNetworkManage::OnItemSelect(TNotifyUI &msg)
{
	CDuiString sName = msg.pSender->GetName();
	sName.MakeLower();
}

LRESULT CPopNetworkManage::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
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

			/*************************����tab 1�һ������߼�**************************/
			if (sMenuName.CompareNoCase(_T("modify_1")) == 0)//�޸� tab1����
			{
				m_CpopModifyInterface->MessageBox(NULL, m_modiftName_1);
				string name = m_CpopModifyInterface->GetNewName();//	string GetNewName();//��ȡ�޸ĺ������
				newName_1 = name;
				if (newName_1 != "")
				{
					ModifyNodeName();
					cout << name << endl;
				}
			}
			else if (sMenuName.CompareNoCase(_T("NewNetwork_1")) == 0)//�½�ͬ������
			{

				m_CPopNewConstellation->MessageBox(NULL);

				string name = m_CPopNewConstellation->GetNewName();//	string GetNewName();//��ȡ�޸ĺ������
				InterfaceNetwork_1 = name;


				if (InterfaceNetwork_1 != "")
				{
					CreateTreeNode(InterfaceNetwork_1);
				}
				//map<string, string> map;
				////map = GetHoroscopeInform();
				//AssignmentToLocalVector_XZ(newHoroscope, map);//�������Լ�Ŀ¼���������

				CTreeViewUI* pTreeView = static_cast<CTreeViewUI*>(m_pm.FindControl(_T("treeview1")));
				pTreeView->SetItemExpand(false, NULL);//�����νṹ��ʼ״̬Ϊ��չ����
				UncheckedTab1();

			}
			else if (sMenuName.CompareNoCase(_T("AddADevice_1")) == 0)//����ڵ���� ����豸
			{

				vector<vector <string>> vec_vecInform;
				vec_vecInform.push_back(vec_recSatelliteName);
				vec_vecInform.push_back(vec_recEarthStationName);
				vec_vecInform.push_back(vec_recGroundTerminalName);
				m_CpopSelectEquipment->MessageBox(NULL, vec_vecInform);

				string FileName = wstringToString(m_modiftName_2.GetData());

				vector<string> vec_Name = m_CpopSelectEquipment->GetNodeName();//	string GetNewName();//��ȡ�޸ĺ������

				int num_child = pNode_1[selectNode[0][0]]->GetCountChild();//GetParentNode
				vector<string> vec_FilterName;//����ǰ���ڵ������Ѿ��е��豸ɾ��
				if (num_child == 0)
				{
					vec_FilterName = vec_Name;
				}
				else if (num_child > 0)
				{
					for (int i = 0; i < vec_Name.size(); i++)
					{

						bool ExistFlag = false;
						for (int j = 0; j < num_child; j++)
						{
							CTreeNodeUI*  node = pNode_1[selectNode[0][0]]->GetChildNode(j);//GetParentNode
							CDuiString cdui_name = node->GetItemText();
							wstring wstr_name = cdui_name.GetData();
							string str_name = wstringToString(wstr_name);
							if (str_name == vec_Name[i])
							{
								ExistFlag = true;

							}
						}
						if (!ExistFlag)
						{
							vec_FilterName.push_back(vec_Name[i]);
						}
					}
				}

				

				if (vec_FilterName.size() > 0)
				{
					Adddisplay(vec_FilterName);
				}
			

				UncheckedTab1();

			}
			else if (sMenuName.CompareNoCase(_T("SelectNetwork_1")) == 0)//��һ��tabҳ--
			{
				vec_StartEndPointTab1.clear();
				int num_child = pNode_1[selectNode[0][0]]->GetCountChild();//GetParentNode
				/******************��ȡ��ѡ������������豸 (��ǰ0���ڵ������һ���ڵ�����)******************/
				for (int i = 0; i < num_child; i++)
				{
					CTreeNodeUI*  node = pNode_1[selectNode[0][0]]->GetChildNode(i);//GetParentNode
					CDuiString cdui_name = node->GetItemText();
					wstring wstr_name = cdui_name.GetData();
					vec_StartEndPointTab1.push_back(wstringToString(wstr_name));
				}
				//Com_StartingPoint
				//����ǰ�����ڵ����ǡ�����վ�������ն˸���Combox
				CComboBoxUI* Com_StartingPoint = static_cast<CComboBoxUI*>(m_pm.FindControl(_T("Com_StartingPoint")));//���
				CComboBoxUI* Com_EndPoint = static_cast<CComboBoxUI*>(m_pm.FindControl(_T("Com_EndPoint")));;//�յ�
				Com_StartingPoint->RemoveAll();
				Com_EndPoint->RemoveAll();
				for (int i = 0; i < vec_StartEndPointTab1.size(); i++)//����ѡ�������е��豸������ʾ��Combox�й��û�ѡ�������յ�
				{
					wstring temp;
					CListLabelElementUI * pLabel = new CListLabelElementUI();
					CListLabelElementUI * pLabel_1 = new CListLabelElementUI();
					UtilTool::setWstring(temp, vec_StartEndPointTab1[i].c_str());
					pLabel->SetText(temp.c_str());
					pLabel->SetBkColor(0xff0000);
					pLabel_1->SetText(temp.c_str());
					pLabel_1->SetBkColor(0xff0000);
					if (Com_StartingPoint)
						Com_StartingPoint->AddAt(pLabel, i);
					if (Com_EndPoint)
						Com_EndPoint->AddAt(pLabel_1, i);
				}
				Com_StartingPoint->SelectItem(0);
				Com_EndPoint->SelectItem(0);
			}

			/*************************����tab 2�һ������߼�**************************/
			if (sMenuName.CompareNoCase(_T("modify_2")) == 0)//�޸� tab2����
			{
				m_CpopModifyInterface->MessageBox(NULL, m_modiftName_2);
				string name = m_CpopModifyInterface->GetNewName();//	string GetNewName();//��ȡ�޸ĺ������
				newName_2 = name;
				if (newName_2 != "")
				{
					ModifyNodeName_2();
					cout << name << endl;
				}

				UncheckedTab2();
			}
			else if (sMenuName.CompareNoCase(_T("NewNetwork_2")) == 0)//�½�ͬ������tab2
			{

				m_CPopNewConstellation->MessageBox(NULL);

				string name = m_CPopNewConstellation->GetNewName();//	string GetNewName();//��ȡ�޸ĺ������
				//InterfaceNetwork_1 = name;


				if (name != "")
				{
					CreateTreeNode_2(name);
					cout << name << endl;
				}

				CTreeViewUI* pTreeView = static_cast<CTreeViewUI*>(m_pm.FindControl(_T("treeview2")));
				pTreeView->SetItemExpand(false, NULL);//�����νṹ��ʼ״̬Ϊ��չ����
				UncheckedTab2();
			}
			else if (sMenuName.CompareNoCase(_T("AddADevice_2")) == 0)//tab2 ����ڵ���� ����豸
			{
				vector<vector <string>> vec_vecInform;
				vec_vecInform.push_back(vec_recSatelliteName);
				vec_vecInform.push_back(vec_recEarthStationName);
				vec_vecInform.push_back(vec_recGroundTerminalName);

				m_CpopSelectEquipment->MessageBox(NULL, vec_vecInform);

				vector<string> vec_Name = m_CpopSelectEquipment->GetNodeName();//	string GetNewName();//��ȡ�޸ĺ������

				int num_child = pNode_2_1[selectNode_2[0][0]]->GetCountChild();//GetParentNode
				vector<string> vec_FilterName;//����ǰ���ڵ������Ѿ��е��豸ɾ��
				if (num_child == 0)
				{
					vec_FilterName = vec_Name;
				}
				else if (num_child > 0)
				{
					for (int i = 0; i < vec_Name.size(); i++)
					{

						bool ExistFlag = false;
						for (int j = 0; j < num_child; j++)
						{
							CTreeNodeUI*  node = pNode_2_1[selectNode_2[0][0]]->GetChildNode(j);//GetParentNode
							CDuiString cdui_name = node->GetItemText();
							wstring wstr_name = cdui_name.GetData();
							string str_name = wstringToString(wstr_name);
							if (str_name == vec_Name[i])
							{
								ExistFlag = true;

							}
						}
						if (!ExistFlag)
						{
							vec_FilterName.push_back(vec_Name[i]);
						}
					}
				}



				if (vec_FilterName.size() > 0)
				{
					Adddisplay_2(vec_FilterName);
				}
				//if (vec_Name.size() > 0)
				//{
				//	Adddisplay_2(vec_Name);
				//}

				UncheckedTab2();

			}
			else if (sMenuName.CompareNoCase(_T("SelectNetwork_2")) == 0)//��2��tabҳ----ѡ���ͼ����
			{

				vec_StartEndPointTab2.clear();
				int num_child = pNode_2_1[selectNode_2[0][0]]->GetCountChild();//GetParentNode
				/******************��ȡ��ѡ������������豸 (��ǰ0���ڵ������һ���ڵ�����)******************/
				for (int i = 0; i < num_child; i++)
				{
					CTreeNodeUI*  node = pNode_2_1[selectNode_2[0][0]]->GetChildNode(i);//GetParentNode
					CDuiString cdui_name = node->GetItemText();
					wstring wstr_name = cdui_name.GetData();
					vec_StartEndPointTab2.push_back(wstringToString(wstr_name));
				}
				//Com_StartingPoint
				//����ǰ�����ڵ����ǡ�����վ�������ն˸���Combox
				CComboBoxUI* Com_StartingPoint = static_cast<CComboBoxUI*>(m_pm.FindControl(_T("Com_StartingPoint_tab2")));//���
				CComboBoxUI* Com_EndPoint = static_cast<CComboBoxUI*>(m_pm.FindControl(_T("Com_EndPoint_tab2")));;//�յ�
				Com_StartingPoint->RemoveAll();
				Com_EndPoint->RemoveAll();
				for (int i = 0; i < vec_StartEndPointTab2.size(); i++)//����ѡ�������е��豸������ʾ��Combox�й��û�ѡ�������յ�
				{
					wstring temp;
					CListLabelElementUI * pLabel = new CListLabelElementUI();
					CListLabelElementUI * pLabel_1 = new CListLabelElementUI();
					UtilTool::setWstring(temp, vec_StartEndPointTab2[i].c_str());
					pLabel->SetText(temp.c_str());
					pLabel->SetBkColor(0xff0000);
					pLabel_1->SetText(temp.c_str());
					pLabel_1->SetBkColor(0xff0000);
					if (Com_StartingPoint)
						Com_StartingPoint->AddAt(pLabel, i);
					if (Com_EndPoint)
						Com_EndPoint->AddAt(pLabel_1, i);
				}
				Com_StartingPoint->SelectItem(0);
				Com_EndPoint->SelectItem(0);
			}

			/*************************����tab3�һ������߼�**********************/
			if (sMenuName.CompareNoCase(_T("modify_3")) == 0)//�޸� tab2����
			{
				m_CpopModifyInterface->MessageBox(NULL, m_modiftName_3);
				string name = m_CpopModifyInterface->GetNewName();//	string GetNewName();//��ȡ�޸ĺ������
				newName_3 = name;
				if (newName_3 != "")
				{
					ModifyNodeName_3();
				}

				//cout << name << endl;
				UncheckedTab3();
			}
			else if (sMenuName.CompareNoCase(_T("NewNetwork_3")) == 0)//�½�ͬ������tab2
			{
				m_CPopNewConstellation->MessageBox(NULL);

				string name = m_CPopNewConstellation->GetNewName();//	string GetNewName();//��ȡ�޸ĺ������
				//InterfaceNetwork_1 = name;
				if (name != "")
				{
					CreateTreeNode_3(name);
				}


				CTreeViewUI* pTreeView = static_cast<CTreeViewUI*>(m_pm.FindControl(_T("treeview3")));
				pTreeView->SetItemExpand(false, NULL);//�����νṹ��ʼ״̬Ϊ��չ����
				UncheckedTab3();
			}
			else if (sMenuName.CompareNoCase(_T("AddADevice_3")) == 0)//tab3 ����ڵ���� ����豸
			{
				vector<vector <string>> vec_vecInform;
				vec_vecInform.push_back(vec_recSatelliteName);
				vec_vecInform.push_back(vec_recEarthStationName);
				vec_vecInform.push_back(vec_recGroundTerminalName);
				m_CpopSelectEquipment->MessageBox(NULL, vec_vecInform);

				vector<string> vec_Name = m_CpopSelectEquipment->GetNodeName();//	string GetNewName();//��ȡ�޸ĺ������

				int num_child = pNode_3_1[selectNode_3[0][0]]->GetCountChild();//GetParentNode
				vector<string> vec_FilterName;//����ǰ���ڵ������Ѿ��е��豸ɾ��
				if (num_child == 0)
				{
					vec_FilterName = vec_Name;
				}
				else if (num_child > 0)
				{
					for (int i = 0; i < vec_Name.size(); i++)
					{

						bool ExistFlag = false;
						for (int j = 0; j < num_child; j++)
						{
							CTreeNodeUI*  node = pNode_3_1[selectNode_3[0][0]]->GetChildNode(j);//GetParentNode
							CDuiString cdui_name = node->GetItemText();
							wstring wstr_name = cdui_name.GetData();
							string str_name = wstringToString(wstr_name);
							if (str_name == vec_Name[i])
							{
								ExistFlag = true;

							}
						}
						if (!ExistFlag)
						{
							vec_FilterName.push_back(vec_Name[i]);
						}
					}
				}



				if (vec_FilterName.size() > 0)
				{
					Adddisplay_3(vec_FilterName);
				}
				//if (vec_Name.size() > 0)
				//{
				//	Adddisplay_3(vec_Name);
				//}

				UncheckedTab3();

			}
			else if (sMenuName.CompareNoCase(_T("SelectNetwork_3")) == 0)//��3��tabҳ��ͼ------ѡ���������
			{
				vec_StartEndPointTab3.clear();
				int num_child = pNode_3_1[selectNode_3[0][0]]->GetCountChild();//GetParentNode
				/******************��ȡ��ѡ������������豸 (��ǰ0���ڵ������һ���ڵ�����)******************/
				for (int i = 0; i < num_child; i++)
				{
					CTreeNodeUI*  node = pNode_3_1[selectNode_3[0][0]]->GetChildNode(i);//GetParentNode
					CDuiString cdui_name = node->GetItemText();
					wstring wstr_name = cdui_name.GetData();
					vec_StartEndPointTab3.push_back(wstringToString(wstr_name));
				}
				//Com_StartingPoint
				//����ǰ�����ڵ����ǡ�����վ�������ն˸���Combox
				CComboBoxUI* Com_StartingPoint = static_cast<CComboBoxUI*>(m_pm.FindControl(_T("Com_StartingPoint_tab3")));//���
				CComboBoxUI* Com_EndPoint = static_cast<CComboBoxUI*>(m_pm.FindControl(_T("Com_EndPoint_tab3")));;//�յ�
				Com_StartingPoint->RemoveAll();
				Com_EndPoint->RemoveAll();
				for (int i = 0; i < vec_StartEndPointTab3.size(); i++)//����ѡ�������е��豸������ʾ��Combox�й��û�ѡ�������յ�
				{
					wstring temp;
					CListLabelElementUI * pLabel = new CListLabelElementUI();
					CListLabelElementUI * pLabel_1 = new CListLabelElementUI();
					UtilTool::setWstring(temp, vec_StartEndPointTab3[i].c_str());
					pLabel->SetText(temp.c_str());
					pLabel->SetBkColor(0xff0000);
					pLabel_1->SetText(temp.c_str());
					pLabel_1->SetBkColor(0xff0000);
					if (Com_StartingPoint)
						Com_StartingPoint->AddAt(pLabel, i);
					if (Com_EndPoint)
						Com_EndPoint->AddAt(pLabel_1, i);
				}
				Com_StartingPoint->SelectItem(0);
				Com_EndPoint->SelectItem(0);
			}


		}
	}
	bHandled = FALSE;
	return 0;
}



LRESULT CPopNetworkManage::OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
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


string  CPopNetworkManage::get_cur_timelogdata()/*��ȡ��ǰʱ���ַ���*/
{
	string time_string;
	SYSTEMTIME stime;
	char buf[100] = { 0 };
	GetLocalTime(&stime);
	sprintf(buf, "%04d.%02d.%02d.%02d.%02d.%02d", stime.wYear, stime.wMonth, stime.wDay, stime.wHour, stime.wMinute, stime.wSecond);
	time_string += buf;
	return time_string;
}
void CPopNetworkManage::OnLClick(CControlUI *pControl)
{

	CDuiString sName = pControl->GetName();

	//	if(pTabSwitch != NULL)
	//CGameBoxFrame:pTabSwitch->SelectItem(FirstTab);
}
void CPopNetworkManage::Notify(TNotifyUI &msg)
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
	}//˫�����νṹ
	else if (msg.sType == _T("treeitemdbclick"))
	{
		/******************** tab1����˫���¼�*******************/
		for (int i = 0; i < pNode_1_num; i++)
		{
			int childNum = pNode_1[i]->GetCountChild();
			if (pNode_1[i]->IsSelected())
			{
				m_modiftName_1 = pNode_1[i]->GetItemText();
				cout << "��ѡ����" << endl;
				selectNode[0][0] = i;
				selectNode[0][1] = -1;
				selectNode[0][2] = -1;
				FindTheMap();
				pNode_1[i]->Select(false);
			}
			else
			{
				for (int j = 0; j < childNum; j++)
				{

					CTreeNodeUI*  node = pNode_1[i]->GetChildNode(j);//GetParentNode
					if (node->IsSelected())
					{
						m_modiftName_1 = node->GetItemText();
						selectNode[0][0] = i;
						selectNode[0][1] = j;
						selectNode[0][2] = -1;
						FindTheMap();
						cout << "��ѡ����!!!" << endl;

						//node->Select(false);
					}

				}
			}
		}
		/******************** tab2����˫���¼�*******************/
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
				FindTheMap_2();
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
						FindTheMap_2();
						cout << "��ѡ����!!!" << endl;

						//node->Select(false);
					}

				}
			}
		}
		/******************** tab3����˫���¼�*******************/
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
				FindTheMap_3();
				//pNode_3_1[i]->Select(false);
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
						FindTheMap_3();
						cout << "��ѡ����!!!" << endl;

						//node->Select(false);
					}

				}
			}
		}

	}
	else if (msg.sType == _T("itemrclick")) //�����ڵ����һ�����¼� _T("itemrclick")
	{
	/***********************tab1�Ĳ���*********************/
		for (int i = 0; i < pNode_1_num; i++)
		{
			int childNum = pNode_1[i]->GetCountChild();
			if (pNode_1[i]->IsSelected())
			{
				m_modiftName_1 = pNode_1[i]->GetItemText();
				cout << "��ѡ����" << endl;
				selectNode[0][0] = i;
				selectNode[0][1] = -1;
				selectNode[0][2] = -1;
				isLevel0_1 = true;
				pNode_1[i]->Select(false);
			}
			else
			{
				for (int j = 0; j < childNum; j++)
				{

					CTreeNodeUI*  node = pNode_1[i]->GetChildNode(j);//GetParentNode
					if (node->IsSelected())
					{
						m_modiftName_1 = node->GetItemText();
						selectNode[0][0] = i;
						selectNode[0][1] = j;
						selectNode[0][2] = -1;
						cout << "��ѡ����!!!" << endl;

						node->Select(false);
					}

				}
			}
		}
		if (isLevel0_1)
		{
			CMenuWnd::GetGlobalContextMenuObserver().SetMenuCheckInfo(&m_MenuInfos);

			if (m_pMenu != NULL) {
				delete m_pMenu;
				m_pMenu = NULL;
			}
			m_pMenu = new CMenuWnd();
			CDuiPoint point;
			::GetCursorPos(&point);
			m_pMenu->Init(NULL, _T("menu_net1.xml"), point, &m_pm);
			isLevel0_1 = false;
		}

	
	/***********************tab2�Ĳ���*********************/
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
			isLevel0_2 = true;
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

					node->Select(false);
				}

			}
		}
	}
	if (isLevel0_2)
	{
		CMenuWnd::GetGlobalContextMenuObserver().SetMenuCheckInfo(&m_MenuInfos);

		if (m_pMenu != NULL) {
			delete m_pMenu;
			m_pMenu = NULL;
		}
		m_pMenu = new CMenuWnd();
		CDuiPoint point;
		::GetCursorPos(&point);
		m_pMenu->Init(NULL, _T("menu_net2.xml"), point, &m_pm);
		isLevel0_2 = false;
	}
	/***********************tab3����********************/
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
			isLevel0_3 = true;
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

					node->Select(false);
				}

			}
		}
	}
	if (isLevel0_3)
	{
		CMenuWnd::GetGlobalContextMenuObserver().SetMenuCheckInfo(&m_MenuInfos);

		if (m_pMenu != NULL) {
			delete m_pMenu;
			m_pMenu = NULL;
		}
		m_pMenu = new CMenuWnd();
		CDuiPoint point;
		::GetCursorPos(&point);
		m_pMenu->Init(NULL, _T("menu_net3.xml"), point, &m_pm);
		isLevel0_3 = false;
	}
}

	return WindowImplBase::Notify(msg);
}
unsigned __stdcall CPopNetworkManage::Busys(void* param)
{

	CPopNetworkManage * pWnd = (CPopNetworkManage*)param;
	::PostMessage(pWnd->m_hWnd, WM_CLOSE, 0, 0);
	return 0;
}

//��ʼ��
void CPopNetworkManage::InitWindow()
{
	m_pCloseBtn = static_cast<CButtonUI*>(m_pm.FindControl(_T("closebtn")));
	pTabDatabase = static_cast<CTabLayoutUI*>(m_pm.FindControl(_T("tab_switch")));
	//select_BusinessType = static_cast<CComboBoxUI*>(m_pm.FindControl(_T("Com_BusinessType")));//ҵ������
	//select_BusinessType_1 = static_cast<CComboBoxUI*>(m_pm.FindControl(_T("Com_Selct_BusinessType")));;//��ͼҪѡ���ҵ�����Ϳؼ�

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
	pNode_2_num = 0;//����ڵ������е�˳��
	pNode_3_num = 0;//����ڵ������е�˳��

	pNode_2_1_num = 0;//��ǰ���ڵ������е�˳��
	pNode_2_2_num = 0;//1��ڵ������е�˳��
	pNode_2_3_num = 0;//2��ڵ������е�˳��

	pNode_3_1_num = 0;//��ǰ���ڵ������е�˳��
	pNode_3_2_num = 0;//1��ڵ������е�˳��
	pNode_3_3_num = 0;//2��ڵ������е�˳��

	selectNode[0][0] = -1;
	selectNode[0][1] = -1;
	selectNode[0][2] = -1;

	selectNode_2[0][0] = -1;
	selectNode_2[0][1] = -1;
	selectNode_2[0][2] = -1;


	selectNode_3[0][0] = -1;
	selectNode_3[0][1] = -1;
	selectNode_3[0][2] = -1;


	isLevel0_1 = false;
	isLevel0_2 = false;//�ڶ���Tab��������ѡ�����0��
	isLevel0_3 = false;//������Tab��������ѡ�����0��

	HideThCcontrol_2(20, FALSE);
	HideThCcontrol_3(20, FALSE);
	//AddComboBox();//�ϸ��汾��tab3�е�combox�ӳ�ʼֵ

	m_Demo_Tab2[UTF8_To_string("IP")] = UTF8_To_string("127.0.0.1");
	m_Demo_Tab2[UTF8_To_string("·����")] = UTF8_To_string("·����");

	m_Demo_Tab3[UTF8_To_string("ҵ������")] = UTF8_To_string("����");

	//CreateTreeNode();
	CreateTreeNode(UTF8_To_string("�����·����"));//����ڵ������
	CreateTreeNode_2(UTF8_To_string("ͨ�ŷ�������"));//��tab2������Ӹ��ڵ�
	CreateTreeNode_3(UTF8_To_string("�û���ϵ����"));//��tab3������Ӹ��ڵ�

	m_CallFmuClass = new CallFmuClass();//����fmu��

	string str_path; //D:\0_TwoDimensional\��Ӳ���������_14_23\gamebox
	wstring wstr;
	unsigned long size = GetCurrentDirectory(0, NULL);
	wchar_t *path = new wchar_t[size];
	if (GetCurrentDirectory(size, path) != 0)
	{
		wstr = path;
	}

	CurrentPath = wstringToString(wstr);

	//��ʼʱ�� ��fmu�ĳ�ʼʱ�����ʱ
	intTime[0] = 2022; intTime[1] = 10; intTime[2] = 27; intTime[3] = 7; intTime[4] = 40; intTime[5] = 0; 
	ts = 1;


}
//��tab3��com���ӱ���
void CPopNetworkManage::AddComboBox()
{
	string parameters[5];//
	parameters[0] = UTF8_To_string("����"); parameters[1] = UTF8_To_string("�㲥"); parameters[2] = UTF8_To_string("ͼ��"); parameters[3] = UTF8_To_string("��Ƶ"); parameters[4] = UTF8_To_string("����");
	for (int i = 0; i < 5; i++)
	{
		wstring temp;
		CListLabelElementUI * pLabel = new CListLabelElementUI();
		CListLabelElementUI * pLabel_1 = new CListLabelElementUI();
		UtilTool::setWstring(temp, parameters[i].c_str());
		pLabel->SetText(temp.c_str());
		pLabel->SetBkColor(0xff0000);
		pLabel_1->SetText(temp.c_str());
		pLabel_1->SetBkColor(0xff0000);
		if (select_BusinessType)
			select_BusinessType->AddAt(pLabel, i);
		if (select_BusinessType_1)
			select_BusinessType_1->AddAt(pLabel_1, i);
	}



	select_BusinessType->SelectItem(0);
	select_BusinessType_1->SelectItem(0);
	str_BusinessType = "����";
}
//������ʹ��UDP����
UINT WINAPI CPopNetworkManage::UDPSendDataThread(void* pParam)
{
	CPopNetworkManage * pMainWnd = (CPopNetworkManage*)pParam;
	unsigned  char m_DataBuffer[1024] = { 0 };
	pMainWnd->m_UDPAdmin1 = new UDPAdmin1();

	pMainWnd->m_UDPAdmin1->InitClient(8002,"127.0.0.1");


	return 0;
}
void CPopNetworkManage::CreateTreeNode(CDuiString name1, CDuiString time)
{
	/****************************����������Ӫ��*****************************/

	pNode_1[pNode_1_num] = new CTreeNodeUI();
	pNode_1[pNode_1_num]->CheckBoxSelected(false);
	pNode_1[pNode_1_num]->SetFixedHeight(30);
	pNode_1[pNode_1_num]->SetItemTextColor(0xFFFFFFFF);
	pNode_1[pNode_1_num]->SetItemHotTextColor(0xFFFFFFFF);
	pNode_1[pNode_1_num]->SetSelItemTextColor(0xFFFFFFFF);
	pNode_1[pNode_1_num]->SetAttribute(_T("folderattr"), _T("padding=&quot;0,1,0,0&quot; width=&quot;18&quot; height=&quot;18&quot; normalimage=&quot;file='ui/չ��.png' &quot; "));
	pNode_1[pNode_1_num]->SetAttribute(_T("folderattr"), _T("hotimage=&quot;file='ui/չ��.png' &quot; selectedimage=&quot;file='ui/δչ��.png'&quot; selectedhotimage=&quot;file='ui/δչ��.png'&quot;"));
	pNode_1[pNode_1_num]->SetAttribute(_T("itemattr"), _T("padding=\"16,0,0,0\" align=\"left\" font=\"13\""));//font=&quot;6&quot;
	pNode_1[pNode_1_num]->SetItemText(name1);
	m_treeView->Add(pNode_1[pNode_1_num]);			//Ҫ����Ӹ��ڵ�,��������ӽڵ����ʽ	


	string str_name;
	UtilTool::setString(str_name, name1.GetData());
	vec_OperatorName.push_back(str_name);

	CLabelUI* plabel = new CLabelUI();

	plabel->SetText(time);
	pNode_1[pNode_1_num]->Add(plabel);
	plabel->SetTextColor(0xFFFFFFFF);// align="left" font="13"  width="130" 
	plabel->SetFont(13);
	/******************************************/

	pNode_2[pNode_2_num] = new CTreeNodeUI();
	pNode_2[pNode_2_num]->CheckBoxSelected(false);
	pNode_2[pNode_2_num]->SetItemText(_T("����"));
	pNode_2[pNode_2_num]->SetFixedHeight(30);
	pNode_2[pNode_2_num]->SetItemTextColor(0xFFFFFFFF);
	pNode_2[pNode_2_num]->SetItemHotTextColor(0xFFFFFFFF);
	pNode_2[pNode_2_num]->SetSelItemTextColor(0xFFFFFFFF);
	pNode_2[pNode_2_num]->SetAttribute(_T("folderattr"), _T("padding=&quot;0,1,0,0&quot; width=&quot;16&quot; height=&quot;16&quot; normalimage=&quot;file='ui/չ��.png' &quot; "));
	pNode_2[pNode_2_num]->SetAttribute(_T("folderattr"), _T("hotimage=&quot;file='ui/չ��.png' &quot; selectedimage=&quot;file='ui/δչ��.png'&quot; selectedhotimage=&quot;file='ui/δչ��.png'&quot;"));
	pNode_2[pNode_2_num]->SetAttribute(_T("itemattr"), _T("padding=\"16,0,0,0\" align=\"left\" font=\"13\""));//font=&quot;6&quot;
	pNode_2[pNode_2_num]->SetName(_T("WX"));
	pNode_1[pNode_1_num]->AddChildNode(pNode_2[pNode_2_num]);
	pNode_2_num += 1;
	pNode_2[pNode_2_num] = new CTreeNodeUI();
	pNode_2[pNode_2_num]->CheckBoxSelected(false);
	pNode_2[pNode_2_num]->SetItemText(_T("����վ"));
	pNode_2[pNode_2_num]->SetFixedHeight(30);
	pNode_2[pNode_2_num]->SetItemTextColor(0xFFFFFFFF);
	pNode_2[pNode_2_num]->SetItemHotTextColor(0xFFFFFFFF);
	pNode_2[pNode_2_num]->SetSelItemTextColor(0xFFFFFFFF);
	pNode_2[pNode_2_num]->SetAttribute(_T("folderattr"), _T("padding=&quot;0,1,0,0&quot; width=&quot;16&quot; height=&quot;16&quot; normalimage=&quot;file='ui/չ��.png' &quot; "));
	pNode_2[pNode_2_num]->SetAttribute(_T("folderattr"), _T("hotimage=&quot;file='ui/չ��.png' &quot; selectedimage=&quot;file='ui/δչ��.png'&quot; selectedhotimage=&quot;file='ui/δչ��.png'&quot;"));
	pNode_2[pNode_2_num]->SetAttribute(_T("itemattr"), _T("padding=\"16,0,0,0\" align=\"left\" font=\"13\""));//font=&quot;6&quot;
	pNode_2[pNode_2_num]->SetName(_T("DQZ"));

	pNode_1[pNode_1_num]->AddChildNode(pNode_2[pNode_2_num]);

	pNode_2_num += 1;
	pNode_2[pNode_2_num] = new CTreeNodeUI();
	pNode_2[pNode_2_num]->CheckBoxSelected(false);
	pNode_2[pNode_2_num]->SetItemText(_T("�û�"));
	pNode_2[pNode_2_num]->SetFixedHeight(30);
	pNode_2[pNode_2_num]->SetItemTextColor(0xFFFFFFFF);
	pNode_2[pNode_2_num]->SetItemHotTextColor(0xFFFFFFFF);
	pNode_2[pNode_2_num]->SetSelItemTextColor(0xFFFFFFFF);
	pNode_2[pNode_2_num]->SetAttribute(_T("folderattr"), _T("padding=&quot;0,1,0,0&quot; width=&quot;16&quot; height=&quot;16&quot; normalimage=&quot;file='ui/չ��.png' &quot; "));
	pNode_2[pNode_2_num]->SetAttribute(_T("folderattr"), _T("hotimage=&quot;file='ui/չ��.png' &quot; selectedimage=&quot;file='ui/δչ��.png'&quot; selectedhotimage=&quot;file='ui/δչ��.png'&quot;"));
	pNode_2[pNode_2_num]->SetAttribute(_T("itemattr"), _T("padding=\"16,0,0,0\" align=\"left\" font=\"13\""));//font=&quot;6&quot;
	pNode_2[pNode_2_num]->SetName(_T("YH"));

	pNode_1[pNode_1_num]->AddChildNode(pNode_2[pNode_2_num]);
	pNode_2_num += 1;
	//vec_OperatorName.push_back("������Ӫ��1");
	pNode_1_num += 1;

}

void CPopNetworkManage::CreateTreeNode(string tableName)
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

	vec_root_1.push_back(tableName);
	pNode_1_num += 1;


}
//�������ڵ�------------tab2
void CPopNetworkManage::CreateTreeNode_2(string tableName)
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


	vec_root_2.push_back(tableName);
	pNode_2_1_num += 1;
}
//�������ڵ�------------tab3
void CPopNetworkManage::CreateTreeNode_3(string tableName)
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


	vec_root_3.push_back(tableName);
	pNode_3_1_num += 1;
}
//�����������ڵ�
void CPopNetworkManage::CreateTreeNode_3(int num,int node,string name)
{
	wstring wst_name;
	UtilTool::setWstring(wst_name,name.c_str());
	pNode_3_num += 1;
	pNode_3[pNode_3_num] = new CTreeNodeUI();
	pNode_3[pNode_3_num]->CheckBoxSelected(false);
	pNode_3[pNode_3_num]->SetItemText(wst_name.c_str());
	pNode_3[pNode_3_num]->SetFixedHeight(30);
	pNode_3[pNode_3_num]->SetItemTextColor(0xFFFFFFFF);
	pNode_3[pNode_3_num]->SetItemHotTextColor(0xFFFFFFFF);
	pNode_3[pNode_3_num]->SetSelItemTextColor(0xFFFFFFFF);
	pNode_3[pNode_3_num]->SetAttribute(_T("folderattr"), _T("padding=&quot;0,1,0,0&quot; width=&quot;16&quot; height=&quot;16&quot; normalimage=&quot;file='ui/չ��.png' &quot; "));
	pNode_3[pNode_3_num]->SetAttribute(_T("folderattr"), _T("hotimage=&quot;file='ui/չ��.png' &quot; selectedimage=&quot;file='ui/δչ��.png'&quot; selectedhotimage=&quot;file='ui/δչ��.png'&quot;"));
	pNode_3[pNode_3_num]->SetAttribute(_T("itemattr"), _T("padding=\"16,0,0,0\" align=\"left\" font=\"13\""));//font=&quot;6&quot;
	pNode_3[pNode_3_num]->SetName(_T("YH"));
	pNode_2[num*3 + node]->AddChildNode(pNode_3[pNode_3_num]);
}

void  CPopNetworkManage::CreateTreeNode_livel1(string tableName,int num)
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
	pNode_1[num]->AddChildNode(pNode_2[pNode_2_num]);
	pNode_2_num += 1;
}
//����1���ڵ�--------tab2
void CPopNetworkManage::CreateTreeNode_livel1_2(string tableName, int num)
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
	pNode_2_1[num]->AddChildNode(pNode_2_2[pNode_2_2_num]);



	pNode_2_2_num += 1;
}
//����1���ڵ�--------tab3
void CPopNetworkManage::CreateTreeNode_livel1_3(string tableName, int num)
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
	pNode_3_1[num]->AddChildNode(pNode_3_2[pNode_3_2_num]);



	pNode_3_2_num += 1;
}
void CPopNetworkManage::ModifyNodeName()
{
	//selectNode[0][0] = i;
	//selectNode[0][1] = 0;
	if (selectNode[0][0] >= 0 && selectNode[0][1] < 0 && selectNode[0][2] < 0)//�޸�0���ڵ�����
	{
		pNode_1[selectNode[0][0]]->SetItemText(stringToWstring(newName_1).c_str());
	}
	if (selectNode[0][0] >= 0 && selectNode[0][1] >= 0 && selectNode[0][2] < 0)//�޸�1���ڵ�����
	{
		//pNode_2[]
		//CTreeNodeUI*  node = pNode_1[selectNode[0][0]]->GetChildNode(selectNode[0][1]);//GetParentNode
		//wstring wstr_name;
		//wstr_name = node->GetItemText();
		//string str_name = wstringToString(wstr_name);
		//int sequences;//�����vector�е�˳��
		//for (int i = 0; i < name_Level1.size(); i++)
		//{
		//	if (str_name == name_Level1[i])
		//	{
		//		sequences = i;
		//	}
		//}
		//name_Level1[sequences] = newName;
		//node->SetItemText(stringToWstring(newName).c_str());
	}



}
void CPopNetworkManage::ModifyNodeName_2()
{
	if (selectNode_2[0][0] >= 0 && selectNode_2[0][1] < 0 && selectNode_2[0][2] < 0)//�޸�0���ڵ�����
	{
		pNode_2_1[selectNode_2[0][0]]->SetItemText(stringToWstring(newName_2).c_str());
	}

}

void CPopNetworkManage::ModifyNodeName_3()
{
	if (selectNode_3[0][0] >= 0 && selectNode_3[0][1] < 0 && selectNode_3[0][2] < 0)//�޸�0���ڵ�����
	{
		pNode_3_1[selectNode_3[0][0]]->SetItemText(stringToWstring(newName_3).c_str());
	}

}
void CPopNetworkManage::saveDataLocally(string noodName, map<string, string> m_map)
{
	vec_recEquipmentName.push_back(noodName);//���յ����豸���� ���ǡ�����վ���ն�
	m_RecvMap.push_back(m_map);//��������豸��������Ϣ ���ǡ�����վ���ն�
}

void CPopNetworkManage::saveDataLocally_WX(string noodName, map<string, string> m_map)
{
	vec_recSatelliteName.push_back(noodName);//���յ����豸���� ����
	m_RecvSatelliteMap.push_back(m_map);//��������豸��������Ϣ ����
}
void CPopNetworkManage::saveDataLocally_DQZ(string noodName, map<string, string> m_map)
{
	vec_recEarthStationName.push_back(noodName);//���յ����豸���� ����վ
	m_RecvEarthStationMap.push_back(m_map);//��������豸��������Ϣ ����վ
}

void CPopNetworkManage::saveDataLocally_DMZD(string noodName, map<string, string> m_map)
{
	vec_recGroundTerminalName.push_back(noodName);//���յ����豸���� �����ն�
	m_RecvGroundTerminalMap.push_back(m_map);//��������豸��������Ϣ �����ն�
}
void CPopNetworkManage::Adddisplay(vector<string> vec)
{
	if (selectNode[0][0] >= 0 && selectNode[0][1] < 0 && selectNode[0][2] < 0)
	{
		for (int i = 0; i < vec.size(); i++)
		{
			CreateTreeNode_livel1(vec[i],selectNode[0][0]);
		}
	}
}
//������tab2ҳ
void CPopNetworkManage::Adddisplay_2(vector<string> vec)
{
	if (selectNode_2[0][0] >= 0 && selectNode_2[0][1] < 0 && selectNode_2[0][2] < 0)
	{
		for (int i = 0; i < vec.size(); i++)
		{
			CreateTreeNode_livel1_2(vec[i], selectNode_2[0][0]);
			bool flag = false;
			if (vec_level2_1.size() > 0)
			{
				for (int j = 0; j < vec_level2_1.size(); j++)
				{
					if (vec_level2_1[j] == vec[i])
					{
						cout << "�Ѿ��������豸��" << endl;
						flag = true;
					}
				}
				if (!flag)
				{
					m_mapLevel2_1.push_back(m_Demo_Tab2);
					vec_level2_1.push_back(vec[i]);
				}
			}
			else
			{
				m_mapLevel2_1.push_back(m_Demo_Tab2);
				vec_level2_1.push_back(vec[i]);
			}
		}
	}
	cout << "11111111" << endl;
}

//������tab3ҳ
void CPopNetworkManage::Adddisplay_3(vector<string> vec)
{
	if (selectNode_3[0][0] >= 0 && selectNode_3[0][1] < 0 && selectNode_3[0][2] < 0)
	{
		for (int i = 0; i < vec.size(); i++)
		{
			CreateTreeNode_livel1_3(vec[i], selectNode_3[0][0]);
			bool flag = false;
			if (vec_level3_1.size() > 0)
			{
				for (int j = 0; j < vec_level3_1.size(); j++)
				{
					if (vec_level3_1[j] == vec[i])
					{
						cout << "�Ѿ��������豸��" << endl;
						flag = true;
					}
				}
				if (!flag)
				{
					m_mapLevel3_1.push_back(m_Demo_Tab3);
					vec_level3_1.push_back(vec[i]);
				}
			}
			else
			{
				m_mapLevel3_1.push_back(m_Demo_Tab3);
				vec_level3_1.push_back(vec[i]);
			}
		}
	}
	cout << "11111111" << endl;
}
void CPopNetworkManage::FindTheMap()//����map
{
	int num = 0;

	if (selectNode[0][0] >= 0 && selectNode[0][1] < 0 && selectNode[0][2] < 0)//�޸�0���ڵ�����
	{
		HideThCcontrol(ControlNumber_Net);//DisplayInformation(m_RecvMap_XZ[selectNode[0][0]]);
	}
	else if (selectNode[0][0] >= 0 && selectNode[0][1] >= 0 && selectNode[0][2] < 0)//�޸�1���ڵ�����
	{
		CTreeNodeUI*  node = pNode_1[selectNode[0][0]]->GetChildNode(selectNode[0][1]);//GetParentNode
		wstring wstr_name;
		wstr_name = node->GetItemText();
		string str_name = wstringToString(wstr_name);

		int sequences_WX = -1;//�����vector�е�˳��
		int sequences_DQZ = -1;//�����vector�е�˳��
		int sequences_DMZD = -1;//�����vector�е�˳��
		for (int i = 0; i < vec_recSatelliteName.size(); i++)//�鿴�Ƕ������� �������vector�е�λ��
		{
			if (str_name == vec_recSatelliteName[i])
			{
				sequences_WX = i;
			}
		}
		for (int i = 0; i < vec_recEarthStationName.size(); i++)//�鿴�Ƕ��ǵ���վ �������vector�е�λ��
		{
			if (str_name == vec_recEarthStationName[i])
			{
				sequences_DQZ = i;
			}
		}
		for (int i = 0; i < vec_recGroundTerminalName.size(); i++)//�鿴�Ƕ��ǵ����ն� �������vector�е�λ��
		{
			if (str_name == vec_recGroundTerminalName[i])
			{
				sequences_DMZD = i;
			}
		}
		if (sequences_WX > -1)
		{
			DisplayInformation(m_RecvSatelliteMap[sequences_WX]);
		}
		else if (sequences_DQZ > -1)
		{
			DisplayInformation(m_RecvEarthStationMap[sequences_DQZ]);
		}
		else if (sequences_DMZD > -1)
		{
			DisplayInformation(m_RecvGroundTerminalMap[sequences_DMZD]);
		}
		
	}


}
void CPopNetworkManage::FindTheMap_2()//����map
{
	int num = 0;

	if (selectNode_2[0][0] >= 0 && selectNode_2[0][1] < 0 && selectNode_2[0][2] < 0)//�޸�0���ڵ�����
	{
		HideThCcontrol_2(20,FALSE);//DisplayInformation(m_RecvMap_XZ[selectNode[0][0]]);
	}
	else if (selectNode_2[0][0] >= 0 && selectNode_2[0][1] >= 0 && selectNode_2[0][2] < 0)//�޸�1���ڵ�����
	{
		CTreeNodeUI*  node = pNode_2_1[selectNode_2[0][0]]->GetChildNode(selectNode_2[0][1]);//GetParentNode
		wstring wstr_name;
		wstr_name = node->GetItemText();
		string str_name = wstringToString(wstr_name);

		int sequences;//�����vector�е�˳��
		for (int i = 0; i < vec_level2_1.size(); i++)
		{
			if (str_name == vec_level2_1[i])
			{
				sequences = i;
			}
		}
		DisplayInformation_2(m_mapLevel2_1[sequences]);
	}
}

void CPopNetworkManage::FindTheMap_3()
{
	int num = 0;

	if (selectNode_3[0][0] >= 0 && selectNode_3[0][1] < 0 && selectNode_3[0][2] < 0)//�޸�0���ڵ�����
	{
		HideThCcontrol_3(20,FALSE);//DisplayInformation(m_RecvMap_XZ[selectNode[0][0]]);
	}
	else if (selectNode_3[0][0] >= 0 && selectNode_3[0][1] >= 0 && selectNode_3[0][2] < 0)//�޸�1���ڵ�����
	{
		CTreeNodeUI*  node = pNode_3_1[selectNode_3[0][0]]->GetChildNode(selectNode_3[0][1]);//GetParentNode
		wstring wstr_name;
		wstr_name = node->GetItemText();
		string str_name = wstringToString(wstr_name);

		int sequences;//�����vector�е�˳��
		for (int i = 0; i < vec_level3_1.size(); i++)
		{
			if (str_name == vec_level3_1[i])
			{
				sequences = i;
			}
		}
		DisplayInformation_3(m_mapLevel3_1[sequences]);
	}
}
//���ؽ����ϵĿؼ���
void  CPopNetworkManage::HideThCcontrol(int num)//num����xml����ؼ������� Ҳ���Ǹ���
{
	for (int i = 0; i < num; i++)
	{
		string vec, StoreOptions, StoreOptions1;
		wstring w, kj;
		std::ostringstream streamsssss;
		streamsssss << i + 1;
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
//���ؿؼ� Tab2
void CPopNetworkManage::HideThCcontrol_2(int num,bool flag)
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
		pControl->SetVisible(flag);//False false

		wstring w1, kj1;
		StoreOptions1 = "p2_" + streamsssss.str();
		UtilTool::setWstring(kj1, StoreOptions1.c_str());
		//UtilTool::setWstring(kj1, StoreOptions1.c_str());
		CControlUI* pControl1 = static_cast<CControlUI*>(m_pm.FindControl(kj1.c_str()));
		pControl1->SetVisible(flag);
	}
	CControlUI* pControAdd = static_cast<CControlUI*>(m_pm.FindControl(stringToWstring("bt_AddAttributes_tab2").c_str()));
	pControAdd->SetVisible(flag);//False false
	CControlUI* pControlDelete = static_cast<CControlUI*>(m_pm.FindControl(stringToWstring("bt_DeleteAttribute_tab2").c_str()));
	pControlDelete->SetVisible(flag);//False false
	CControlUI* pControl = static_cast<CControlUI*>(m_pm.FindControl(stringToWstring("bt_save").c_str()));
	pControl->SetVisible(flag);//False false
	
}
void CPopNetworkManage::HideThCcontrol_3(int num,bool flag)
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
		pControl->SetVisible(flag);//False false

		wstring w1, kj1;
		StoreOptions1 = "p3_" + streamsssss.str();
		UtilTool::setWstring(kj1, StoreOptions1.c_str());
		//UtilTool::setWstring(kj1, StoreOptions1.c_str());
		CControlUI* pControl1 = static_cast<CControlUI*>(m_pm.FindControl(kj1.c_str()));
		pControl1->SetVisible(flag);
	}
	CControlUI* pControAdd = static_cast<CControlUI*>(m_pm.FindControl(stringToWstring("bt_AddAttributes_tab3").c_str()));
	pControAdd->SetVisible(flag);//False false
	CControlUI* pControlDelete = static_cast<CControlUI*>(m_pm.FindControl(stringToWstring("bt_DeleteAttribute_tab3").c_str()));
	pControlDelete->SetVisible(flag);//False false
	CControlUI* pControl = static_cast<CControlUI*>(m_pm.FindControl(stringToWstring("bt_save_tab3").c_str()));
	pControl->SetVisible(flag);//False false

}
void CPopNetworkManage::DisplayInformation(map<string, string> m_map)
{
	HideThCcontrol(ControlNumber_Net);
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
void CPopNetworkManage::DisplayInformation_2(map<string, string> m_map)
{
	int num = m_map.size();
	HideThCcontrol_2(num,TRUE);
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
		UtilTool::setWstring(w, iter->first.c_str());
		UtilTool::setWstring(kj, StoreOptions.c_str());

		CControlUI* pControl = static_cast<CControlUI*>(m_pm.FindControl(kj.c_str()));
		pControl->SetText(w.c_str());
		pControl->SetVisible(TRUE);

		wstring w1, kj1;
		StoreOptions1 = "p2_" + streamsssss.str();
		//UtilTool::setWstring(w1, iter->second.c_str());
		UtilTool::setWstring(w1, iter->second.c_str());
		UtilTool::setWstring(kj1, StoreOptions1.c_str());
		CControlUI* pControl1 = static_cast<CControlUI*>(m_pm.FindControl(kj1.c_str()));
		pControl1->SetText(w1.c_str());
		pControl1->SetVisible(TRUE);
		i += 1;
	}

}
//tab3���Ͻǽ�����ʾ
void  CPopNetworkManage::DisplayInformation_3(map<string, string> m_map)
{
	int num = m_map.size();
	HideThCcontrol_3(num, TRUE);
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
		UtilTool::setWstring(w, iter->first.c_str());
		UtilTool::setWstring(kj, StoreOptions.c_str());

		CControlUI* pControl = static_cast<CControlUI*>(m_pm.FindControl(kj.c_str()));
		pControl->SetText(w.c_str());
		pControl->SetVisible(TRUE);

		wstring w1, kj1;
		StoreOptions1 = "p3_" + streamsssss.str();
		//UtilTool::setWstring(w1, iter->second.c_str());
		UtilTool::setWstring(w1, iter->second.c_str());
		UtilTool::setWstring(kj1, StoreOptions1.c_str());
		CControlUI* pControl1 = static_cast<CControlUI*>(m_pm.FindControl(kj1.c_str()));
		pControl1->SetText(w1.c_str());
		pControl1->SetVisible(TRUE);
		i += 1;
		//cout << "key:" << iter->first << " value:" << iter->second << endl;

		//string vec, StoreOptions, StoreOptions1;
		//wstring w, kj;
		//std::ostringstream streamsssss;
		//streamsssss << i;
		//StoreOptions = "titles3_" + streamsssss.str();
		////UtilTool::setWstring(w, iter->first.c_str());
		//UtilTool::setWstring(w, iter->first.c_str());
		//UtilTool::setWstring(kj, StoreOptions.c_str());

		//CControlUI* pControl = static_cast<CControlUI*>(m_pm.FindControl(kj.c_str()));
		//pControl->SetText(w.c_str());
		//pControl->SetVisible(TRUE);

		//wstring w1, kj1;
		//StoreOptions1 = "Com_BusinessType";
		////UtilTool::setWstring(w1, iter->second.c_str());
		//UtilTool::setWstring(w1, iter->second.c_str());
		//UtilTool::setWstring(kj1, StoreOptions1.c_str());
		//CControlUI* pControl1 = static_cast<CControlUI*>(m_pm.FindControl(kj1.c_str()));
		//if (iter->second == UTF8_To_string("����"))//���ӡ��㲥��ͼ����Ƶ������
		//{
		//	select_BusinessType->SelectItem(0);
		//}
		//else if (iter->second == UTF8_To_string("�㲥"))
		//{
		//	select_BusinessType->SelectItem(1);
		//}
		//else if (iter->second == UTF8_To_string("ͼ��"))
		//{
		//	select_BusinessType->SelectItem(2);
		//}
		//else if (iter->second == UTF8_To_string("��Ƶ"))
		//{
		//	select_BusinessType->SelectItem(3);
		//}
		//else if (iter->second == UTF8_To_string("����"))
		//{
		//	select_BusinessType->SelectItem(4);
		//}
		////pControl1->SetText(w1.c_str());
		//pControl1->SetVisible(TRUE);
		//i += 1;
	}
}

void CPopNetworkManage::parseElements()
{
	vector<string> vec_child;

	vector<string> wx_LEO;//���Ҫ�ŵĵ͹�(LEO)/�й�(MEO)/�߹�(HEO)
	vector<string> wx_MEO;//���Ҫ�����й�
	vector<string> wx_HEO;//���Ҫ���ĸ߹�
	vector<string> vec_SheBei;//���Ҫ�����豸

	int num_child = pNode_1[selectNode[0][0]]->GetCountChild();//GetParentNode
	/******************��ȡ��ѡ������������豸 (��ǰ0���ڵ������һ���ڵ�����)******************/
	for (int i = 0; i < num_child; i++)
	{
		CTreeNodeUI*  node = pNode_1[selectNode[0][0]]->GetChildNode(i);//GetParentNode
		CDuiString cdui_name = node->GetItemText();
		wstring wstr_name = cdui_name.GetData();
		vec_child.push_back(wstringToString(wstr_name));
	}
	/****************����������������������ɸѡ������*************/
	for (int i = 0; i < vec_child.size(); i++)
	{
		int sequence;
		for (int j = 0; j < vec_recEquipmentName.size(); j++)
		{
			if (vec_child[i] == vec_recEquipmentName[j])
			{
				sequence = j;
			}
		}
		map<string, string> curr_map = m_RecvMap[sequence];
		if (curr_map.count("����������") > 0)
		{
			string value = curr_map["����������"];//�͹�(LEO)/�й�(MEO)/�߹�(HEO)
			if (value == "�͹�(LEO)")
			{
				wx_LEO.push_back(vec_child[i]);//���Ҫ�ŵĵ͹�(LEO)/�й�(MEO)/�߹�(HEO)
			}
			else if (value == "�й�(MEO)")
			{
				wx_MEO.push_back(vec_child[i]);
			}
			else if (value == "�߹�(HEO)")
			{
				wx_HEO.push_back(vec_child[i]);
			}
		}
		else
		{
			vec_SheBei.push_back(vec_child[i]);
		}
	}
	DrawAPicture(wx_HEO, wx_MEO,wx_LEO, vec_SheBei);

}

//����1��ͼ
void CPopNetworkManage::DrawAPicture(vector<string> vec_line1, vector<string> vec_line2, vector<string> vec_line3, vector<string> vec_line4)
{
	HideThCcontrol_Draw(ControlNumber_Draw1);
	/*******************����һ�и߹��ͼ***************/
	int num_Start_line1 = (15 - vec_line1.size())/2;
	string StoreOptions_line1[15];
	string StoreOptions_line1_1[15];
	for (int i = 0; i < vec_line1.size(); i++)
	{
		wstring w, kj;
		std::ostringstream streamsssss;
		streamsssss << (i + 1 + num_Start_line1);
		StoreOptions_line1[i] = "BJ1_" + streamsssss.str();
		StoreOptions_line1_1[i] = "lable1_" + streamsssss.str();

		//UtilTool::setWstring(w, vec[i].c_str());
		UtilTool::setWstring(kj, StoreOptions_line1[i].c_str());
		CControlUI* pControl = static_cast<CControlUI*>(m_pm.FindControl(kj.c_str()));
		pControl->SetForeImage(_T("ui/����.png"));

		CControlUI* pControl_1 = static_cast<CControlUI*>(m_pm.FindControl(stringToWstring(StoreOptions_line1_1[i]).c_str()));
		pControl_1->SetText(stringToWstring(vec_line1[i]).c_str());
		//pControl_1->SetVisible(TRUE); 




		streamsssss.clear(); //clear flags.�����ת��ʱ�������ȵ����clear()
		streamsssss.str("");
	}
	/**********************���й��ͼ***************************/
	int num_Start_line2 = (15 - vec_line2.size()) / 2;
	string StoreOptions_line2[15];
	string StoreOptions_line2_1[15];
	string StoreOptions_JT1[15];

	for (int i = 0; i < vec_line2.size(); i++)
	{
		wstring w, kj;
		std::ostringstream streamsssss;
		streamsssss << (i + 1 + num_Start_line2);
		StoreOptions_line2[i] = "BJ2_" + streamsssss.str();
		StoreOptions_line2_1[i] = "lable2_" + streamsssss.str();
		StoreOptions_JT1[i] = "JT1_" + streamsssss.str();

		//UtilTool::setWstring(w, vec[i].c_str());
		UtilTool::setWstring(kj, StoreOptions_line2[i].c_str());
		CControlUI* pControl = static_cast<CControlUI*>(m_pm.FindControl(kj.c_str()));
		pControl->SetForeImage(_T("ui/����.png"));

		CControlUI* pControl_1 = static_cast<CControlUI*>(m_pm.FindControl(stringToWstring(StoreOptions_line2_1[i]).c_str()));
		pControl_1->SetText(stringToWstring(vec_line2[i]).c_str());


		CControlUI* pControl_2 = static_cast<CControlUI*>(m_pm.FindControl(stringToWstring(StoreOptions_JT1[i]).c_str()));
		//pControl_2->SetVisible(TRUE);
		pControl_2->SetForeImage(_T("ui/���¼�ͷ��.png"));


		streamsssss.clear(); //clear flags.�����ת��ʱ�������ȵ����clear()
		streamsssss.str("");
	}
	/**********************���͹��ͼ***************************/
	int num_Start_line3 = (15 - vec_line3.size()) / 2;
	string StoreOptions_line3[15];
	string StoreOptions_line3_1[15];
	string StoreOptions_JT2[15];

	for (int i = 0; i < vec_line3.size(); i++)
	{
		wstring w, kj;
		std::ostringstream streamsssss;
		streamsssss << (i + 1 + num_Start_line3);
		StoreOptions_line3[i] = "BJ3_" + streamsssss.str();
		StoreOptions_line3_1[i] = "lable3_" + streamsssss.str();
		StoreOptions_JT2[i] = "JT2_" + streamsssss.str();

		//UtilTool::setWstring(w, vec[i].c_str());
		UtilTool::setWstring(kj, StoreOptions_line3[i].c_str());
		CControlUI* pControl = static_cast<CControlUI*>(m_pm.FindControl(kj.c_str()));
		pControl->SetForeImage(_T("ui/����.png"));

		CControlUI* pControl_1 = static_cast<CControlUI*>(m_pm.FindControl(stringToWstring(StoreOptions_line3_1[i]).c_str()));
		pControl_1->SetText(stringToWstring(vec_line3[i]).c_str());

		CControlUI* pControl_2 = static_cast<CControlUI*>(m_pm.FindControl(stringToWstring(StoreOptions_JT2[i]).c_str()));
		pControl_2->SetForeImage(_T("ui/���¼�ͷ��.png"));


		streamsssss.clear(); //clear flags.�����ת��ʱ�������ȵ����clear()
		streamsssss.str("");
	}
	/**********************���豸��ͼ***************************/
	int num_Start_line4 = (15 - vec_line4.size()) / 2;
	string StoreOptions_line4[15];
	string StoreOptions_line4_1[15];
	string StoreOptions_JT3[15];
	for (int i = 0; i < vec_line4.size(); i++)
	{
		wstring w, kj;
		std::ostringstream streamsssss;
		streamsssss << (i + 1 + num_Start_line4);
		StoreOptions_line4[i] = "BJ4_" + streamsssss.str();
		StoreOptions_line4_1[i] = "lable4_" + streamsssss.str();
		StoreOptions_JT3[i] = "JT3_" + streamsssss.str();
		//UtilTool::setWstring(w, vec[i].c_str());
		UtilTool::setWstring(kj, StoreOptions_line4[i].c_str());
		CControlUI* pControl = static_cast<CControlUI*>(m_pm.FindControl(kj.c_str()));
		pControl->SetForeImage(_T("ui/�豸.png"));

		CControlUI* pControl_1 = static_cast<CControlUI*>(m_pm.FindControl(stringToWstring(StoreOptions_line4_1[i]).c_str()));
		pControl_1->SetText(stringToWstring(vec_line4[i]).c_str());

		CControlUI* pControl_2 = static_cast<CControlUI*>(m_pm.FindControl(stringToWstring(StoreOptions_JT3[i]).c_str()));
		pControl_2->SetForeImage(_T("ui/���¼�ͷ��.png"));

		streamsssss.clear(); //clear flags.�����ת��ʱ�������ȵ����clear()
		streamsssss.str("");
	}
}

//���ؽ���1��ͼ�ؼ�
void CPopNetworkManage::HideThCcontrol_Draw(int num)
{
	string StoreOptions_line1[15];
	string StoreOptions_line1_1[15];
	string StoreOptions_JT1[15];
	for (int i = 0; i < num; i++)
	{
		wstring w, kj;
		std::ostringstream streamsssss;
		streamsssss << (i + 1);
		StoreOptions_line1[i] = "BJ1_" + streamsssss.str();
		StoreOptions_line1_1[i] = "lable1_" + streamsssss.str();
		StoreOptions_JT1[i] = "JT1_" + streamsssss.str();
		UtilTool::setWstring(kj, StoreOptions_line1[i].c_str());
		CControlUI* pControl = static_cast<CControlUI*>(m_pm.FindControl(kj.c_str()));
		pControl->SetForeImage(_T("ui/�յ�.png"));
		//pControl->SetVisible(flag);

		CControlUI* pControl_1 = static_cast<CControlUI*>(m_pm.FindControl(stringToWstring(StoreOptions_line1_1[i]).c_str()));
		pControl_1->SetText(_T(" "));
		//pControl_1->SetVisible(flag);

		CControlUI* pControl_2 = static_cast<CControlUI*>(m_pm.FindControl(stringToWstring(StoreOptions_JT1[i]).c_str()));
		pControl_2->SetForeImage(_T("ui/�յ�.png"));
		//pControl_2->SetVisible(flag);
	}
	string StoreOptions_line2[15];
	string StoreOptions_line2_1[15];
	string StoreOptions_JT2[15];
	for (int i = 0; i < num; i++)
	{
		wstring w, kj;
		std::ostringstream streamsssss;
		streamsssss << (i + 1);
		StoreOptions_line2[i] = "BJ2_" + streamsssss.str();
		StoreOptions_line2_1[i] = "lable2_" + streamsssss.str();
		StoreOptions_JT2[i] = "JT2_" + streamsssss.str();
		UtilTool::setWstring(kj, StoreOptions_line2[i].c_str());
		CControlUI* pControl = static_cast<CControlUI*>(m_pm.FindControl(kj.c_str()));
		pControl->SetForeImage(_T("ui/�յ�.png"));

		CControlUI* pControl_1 = static_cast<CControlUI*>(m_pm.FindControl(stringToWstring(StoreOptions_line2_1[i]).c_str()));
		pControl_1->SetText(_T(" "));

		CControlUI* pControl_2 = static_cast<CControlUI*>(m_pm.FindControl(stringToWstring(StoreOptions_JT2[i]).c_str()));
		pControl_2->SetForeImage(_T("ui/�յ�.png"));

		streamsssss.clear(); //clear flags.�����ת��ʱ�������ȵ����clear()
		streamsssss.str("");
	}

	string StoreOptions_line3[15];
	string StoreOptions_line3_1[15];
	string StoreOptions_JT3[15];
	for (int i = 0; i < num; i++)
	{
		wstring w, kj;
		std::ostringstream streamsssss;
		streamsssss << (i + 1);
		StoreOptions_line3[i] = "BJ3_" + streamsssss.str();
		StoreOptions_line3_1[i] = "lable3_" + streamsssss.str();
		StoreOptions_JT3[i] = "JT3_" + streamsssss.str();
		//UtilTool::setWstring(w, vec[i].c_str());
		UtilTool::setWstring(kj, StoreOptions_line3[i].c_str());
		CControlUI* pControl = static_cast<CControlUI*>(m_pm.FindControl(kj.c_str()));
		pControl->SetForeImage(_T("ui/�յ�.png"));

		CControlUI* pControl_1 = static_cast<CControlUI*>(m_pm.FindControl(stringToWstring(StoreOptions_line3_1[i]).c_str()));
		pControl_1->SetText(_T(" "));

		CControlUI* pControl_2 = static_cast<CControlUI*>(m_pm.FindControl(stringToWstring(StoreOptions_JT3[i]).c_str()));
		pControl_2->SetForeImage(_T("ui/�յ�.png"));

		streamsssss.clear(); //clear flags.�����ת��ʱ�������ȵ����clear()
		streamsssss.str("");
	}
	string StoreOptions_line4[15];
	string StoreOptions_line4_1[15];
	for (int i = 0; i < num; i++)
	{
		wstring w, kj;
		std::ostringstream streamsssss;
		streamsssss << (i + 1);
		StoreOptions_line4[i] = "BJ4_" + streamsssss.str();
		StoreOptions_line4_1[i] = "lable4_" + streamsssss.str();
		//UtilTool::setWstring(w, vec[i].c_str());
		UtilTool::setWstring(kj, StoreOptions_line4[i].c_str());
		CControlUI* pControl = static_cast<CControlUI*>(m_pm.FindControl(kj.c_str()));
		pControl->SetForeImage(_T("ui/�յ�.png"));

		CControlUI* pControl_1 = static_cast<CControlUI*>(m_pm.FindControl(stringToWstring(StoreOptions_line4_1[i]).c_str()));
		pControl_1->SetText(_T(" "));

		streamsssss.clear(); //clear flags.�����ת��ʱ�������ȵ����clear()
		streamsssss.str("");
	}
}
//���ؽ���2��ͼ�ؼ�
void CPopNetworkManage::HideThCcontro2_Draw(int num)
{
	string StoreOptions_line1[15];
	string StoreOptions_line1_1[15];
	string StoreOptions_JT1[15];
	for (int i = 0; i < num; i++)
	{
		wstring w, kj;
		std::ostringstream streamsssss;
		streamsssss << (i + 1);
		StoreOptions_line1[i] = "BJ_2_1_" + streamsssss.str();
		StoreOptions_line1_1[i] = "lable_2_1_" + streamsssss.str();
		StoreOptions_JT1[i] = "JT_2_1_" + streamsssss.str();
		UtilTool::setWstring(kj, StoreOptions_line1[i].c_str());
		CControlUI* pControl = static_cast<CControlUI*>(m_pm.FindControl(kj.c_str()));
		pControl->SetForeImage(_T("ui/�յ�.png"));
		//pControl->SetVisible(flag);

		CControlUI* pControl_1 = static_cast<CControlUI*>(m_pm.FindControl(stringToWstring(StoreOptions_line1_1[i]).c_str()));
		pControl_1->SetText(_T(" "));
		//pControl_1->SetVisible(flag);

		CControlUI* pControl_2 = static_cast<CControlUI*>(m_pm.FindControl(stringToWstring(StoreOptions_JT1[i]).c_str()));
		pControl_2->SetForeImage(_T("ui/�յ�.png"));
		//pControl_2->SetVisible(flag);
	}

	CControlUI* pControl_1 = static_cast<CControlUI*>(m_pm.FindControl(stringToWstring("BJ_2_2_1").c_str()));
	//pControl_1->SetVisible(flag);
	string StoreOptions_line2[15];
	string StoreOptions_line2_1[15];
	string StoreOptions_JT2[15];
	for (int i = 0; i < num; i++)
	{
		wstring w, kj;
		std::ostringstream streamsssss;
		streamsssss << (i + 1);
		StoreOptions_JT2[i] = "JT_2_2_" + streamsssss.str();


		CControlUI* pControl_2 = static_cast<CControlUI*>(m_pm.FindControl(stringToWstring(StoreOptions_JT2[i]).c_str()));
		pControl_2->SetForeImage(_T("ui/�յ�.png"));
		//pControl_2->SetVisible(flag);

		streamsssss.clear(); //clear flags.�����ת��ʱ�������ȵ����clear()
		streamsssss.str("");
	}

	string StoreOptions_line3[15];
	string StoreOptions_line3_1[15];
	string StoreOptions_JT3[15];
	for (int i = 0; i < num; i++)
	{
		wstring w, kj;
		std::ostringstream streamsssss;
		streamsssss << (i + 1);
		StoreOptions_line3[i] = "BJ_2_3_" + streamsssss.str();
		StoreOptions_line3_1[i] = "lable_2_3_" + streamsssss.str();
		//StoreOptions_JT3[i] = "JT2_3_" + streamsssss.str();
		//UtilTool::setWstring(w, vec[i].c_str());
		UtilTool::setWstring(kj, StoreOptions_line3[i].c_str());
		CControlUI* pControl = static_cast<CControlUI*>(m_pm.FindControl(kj.c_str()));
		pControl->SetForeImage(_T("ui/�յ�.png"));
		//pControl->SetVisible(flag);

		CControlUI* pControl_1 = static_cast<CControlUI*>(m_pm.FindControl(stringToWstring(StoreOptions_line3_1[i]).c_str()));
		pControl_1->SetText(_T(" "));
		//pControl_1->SetVisible(flag);


		streamsssss.clear(); //clear flags.�����ת��ʱ�������ȵ����clear()
		streamsssss.str("");
	}

}

//���ؽ���3��ͼ�ؼ�
void CPopNetworkManage::HideThCcontro3_Draw(int num)
{
	string StoreOptions_line1[15];
	string StoreOptions_line1_1[15];
	string StoreOptions_JT1[15];
	for (int i = 0; i < num; i++)
	{
		wstring w, kj;
		std::ostringstream streamsssss;
		streamsssss << (i + 1);
		StoreOptions_line1[i] = "BJ_3_1_" + streamsssss.str();
		StoreOptions_line1_1[i] = "lable_3_1_" + streamsssss.str();
		StoreOptions_JT1[i] = "JT_3_1_" + streamsssss.str();
		UtilTool::setWstring(kj, StoreOptions_line1[i].c_str());
		CControlUI* pControl = static_cast<CControlUI*>(m_pm.FindControl(kj.c_str()));
		pControl->SetForeImage(_T("ui/�յ�.png"));
		//pControl->SetVisible(flag);

		CControlUI* pControl_1 = static_cast<CControlUI*>(m_pm.FindControl(stringToWstring(StoreOptions_line1_1[i]).c_str()));
		pControl_1->SetText(_T(" "));
		//pControl_1->SetVisible(flag);

		CControlUI* pControl_2 = static_cast<CControlUI*>(m_pm.FindControl(stringToWstring(StoreOptions_JT1[i]).c_str()));
		pControl_2->SetForeImage(_T("ui/�յ�.png"));
		//pControl_2->SetVisible(flag);
	}

	CControlUI* pControl_1 = static_cast<CControlUI*>(m_pm.FindControl(stringToWstring("BJ_2_2_1").c_str()));
	//pControl_1->SetVisible(flag);
	string StoreOptions_line2[15];
	string StoreOptions_line2_1[15];
	string StoreOptions_JT2[15];
	for (int i = 0; i < num; i++)
	{
		wstring w, kj;
		std::ostringstream streamsssss;
		streamsssss << (i + 1);
		StoreOptions_JT2[i] = "JT_3_2_" + streamsssss.str();


		CControlUI* pControl_2 = static_cast<CControlUI*>(m_pm.FindControl(stringToWstring(StoreOptions_JT2[i]).c_str()));
		pControl_2->SetForeImage(_T("ui/�յ�.png"));
		//pControl_2->SetVisible(flag);

		streamsssss.clear(); //clear flags.�����ת��ʱ�������ȵ����clear()
		streamsssss.str("");
	}

	string StoreOptions_line3[15];
	string StoreOptions_line3_1[15];
	string StoreOptions_JT3[15];
	for (int i = 0; i < num; i++)
	{
		wstring w, kj;
		std::ostringstream streamsssss;
		streamsssss << (i + 1);
		StoreOptions_line3[i] = "BJ_3_3_" + streamsssss.str();
		StoreOptions_line3_1[i] = "lable_3_3_" + streamsssss.str();
		//StoreOptions_JT3[i] = "JT2_3_" + streamsssss.str();
		//UtilTool::setWstring(w, vec[i].c_str());
		UtilTool::setWstring(kj, StoreOptions_line3[i].c_str());
		CControlUI* pControl = static_cast<CControlUI*>(m_pm.FindControl(kj.c_str()));
		pControl->SetForeImage(_T("ui/�յ�.png"));
		//pControl->SetVisible(flag);

		CControlUI* pControl_1 = static_cast<CControlUI*>(m_pm.FindControl(stringToWstring(StoreOptions_line3_1[i]).c_str()));
		pControl_1->SetText(_T(" "));
		//pControl_1->SetVisible(flag);


		streamsssss.clear(); //clear flags.�����ת��ʱ�������ȵ����clear()
		streamsssss.str("");
	}

}

//��ȡ�����޸ĵ����� �����Ӧ��vector
map<string, string> CPopNetworkManage::GetDataFromInterfaces(int num)
{
	map<string, string> map1;

	for (int i = 0; i < num; i++)
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
			map1[wstringToString(wstr_title)] = wstringToString(wstr_title1);
		}

	}
	return map1;
}

map<string, string> CPopNetworkManage::GetDataFromInterfaces_Tab3(int num)
{
	map<string, string> map1;


	for (int i = 0; i < num; i++)
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
			map1[wstringToString(wstr_title)] = wstringToString(wstr_title1);
		}

	}
	return map1;
	return map1;
}
void CPopNetworkManage::parseElements_2()
{
	/************��ȡ�û�ɸѡ������ɸѡֵ************/
	CControlUI* pControl_1 = static_cast<CControlUI*>(m_pm.FindControl(stringToWstring("edit_FilterCriteria").c_str()));
	CDuiString cdui_siftType;	//pControl_1->SetText
	cdui_siftType = pControl_1->GetText();
	siftType = wstringToString(cdui_siftType.GetData());//tab2��ѡ���ɸѡ����

	CControlUI* pControl_2 = static_cast<CControlUI*>(m_pm.FindControl(stringToWstring("edit_FilterCriteriaValue").c_str()));
	CDuiString cdui_siftTypeValue;	//pControl_1->SetText
	cdui_siftTypeValue = pControl_2->GetText();
	siftTypeValue = wstringToString(cdui_siftTypeValue.GetData());//tab2��ѡ���ɸѡ����

	vector<string> vec_child;

	vector<string> vec_SheBei;//���Ҫ�����豸

	int num_child = pNode_2_1[selectNode_2[0][0]]->GetCountChild();//GetParentNode
	/******************��ȡ��ѡ������������豸 (��ǰ0���ڵ������һ���ڵ�����)******************/
	for (int i = 0; i < num_child; i++)
	{
		CTreeNodeUI*  node = pNode_2_1[selectNode_2[0][0]]->GetChildNode(i);//GetParentNode
		CDuiString cdui_name = node->GetItemText();
		wstring wstr_name = cdui_name.GetData();
		vec_child.push_back(wstringToString(wstr_name));
	}
	/****************����������������������ɸѡ������*************/
	for (int i = 0; i < vec_child.size(); i++)
	{
		int sequence;
		for (int j = 0; j < vec_level2_1.size(); j++)
		{
			if (vec_child[i] == vec_level2_1[j])
			{
				sequence = j;
			}
		}
		map<string, string> curr_map = m_mapLevel2_1[sequence];
		if (curr_map.count(siftType) > 0)
		{
			string value = curr_map[siftType];//�͹�(LEO)/�й�(MEO)/�߹�(HEO)
			if (value == siftTypeValue)
			{
				vec_SheBei.push_back(vec_child[i]);//���Ҫ�ŵĵ͹�(LEO)/�й�(MEO)/�߹�(HEO)
			}

		}
	}
	cout << "1111111111" << endl;
	DrawAPicture_Tab2(vec_SheBei);
}
//����2�Ļ�ͼ
void CPopNetworkManage::DrawAPicture_Tab2(vector<string> vec_line1)
{
	HideThCcontro2_Draw(ControlNumber_Draw1);


	vector<string> line_1;//��ŵ�һ�е��豸����
	vector<string> line_2;//��ŵڶ��е��豸����
	if (vec_line1.size() > 0)
	{
		bool flag = false;
		for (int i = 0; i < vec_line1.size(); i++)
		{
			if ((i+1)%2)
			{
				line_1.push_back(vec_line1[i]);
			}
			else
			{
				line_2.push_back(vec_line1[i]);
			}
		}
	}
	/*******************��������һ��ͼ********************/
	string StoreOptions_line1[15];
	string StoreOptions_line1_1[15];
	string StoreOptions_JT1[15];
	string StoreOptions_LineMid[15];//�м����
	int num_Start_line1 = (15 - line_1.size()) / 2;
	for (int i = 0; i < line_1.size(); i++)
	{
		wstring w, kj;
		std::ostringstream streamsssss;
		streamsssss << (i + 1 + num_Start_line1);
		StoreOptions_line1[i] = "BJ_2_1_" + streamsssss.str();
		StoreOptions_line1_1[i] = "lable_2_1_" + streamsssss.str();
		StoreOptions_JT1[i] = "JT_2_1_" + streamsssss.str();
		StoreOptions_LineMid[i] = "BJ_2_2_" + streamsssss.str();
		UtilTool::setWstring(kj, StoreOptions_line1[i].c_str());
		CControlUI* pControl = static_cast<CControlUI*>(m_pm.FindControl(kj.c_str()));
		pControl->SetForeImage(_T("ui/�豸.png"));

		CControlUI* pControl_1 = static_cast<CControlUI*>(m_pm.FindControl(stringToWstring(StoreOptions_line1_1[i]).c_str()));
		pControl_1->SetText(stringToWstring(line_1[i]).c_str());


		CControlUI* pControl_2 = static_cast<CControlUI*>(m_pm.FindControl(stringToWstring(StoreOptions_JT1[i]).c_str()));
		pControl_2->SetForeImage(_T("ui/��ͷ2.png"));

		CControlUI* pControl_3 = static_cast<CControlUI*>(m_pm.FindControl(stringToWstring(StoreOptions_LineMid[i]).c_str()));
		pControl_3->SetForeImage(_T("ui/��.png"));

	
	}


	string StoreOptions_JT2[15];

	string StoreOptions_line3[15];
	string StoreOptions_line3_1[15];
	string StoreOptions_JT3[15];

	int num_Start_line2 = (15 - line_2.size()) / 2;
	for (int i = 0; i < line_2.size(); i++)
	{
		wstring w, kj;
		std::ostringstream streamsssss;
		streamsssss << (i + 1 + num_Start_line2);
		StoreOptions_JT2[i] = "JT_2_2_" + streamsssss.str();
		StoreOptions_line3[i] = "BJ_2_3_" + streamsssss.str();
		StoreOptions_line3_1[i] = "lable_2_3_" + streamsssss.str();
		UtilTool::setWstring(kj, StoreOptions_line3[i].c_str());
		CControlUI* pControl = static_cast<CControlUI*>(m_pm.FindControl(kj.c_str()));
		pControl->SetForeImage(_T("ui/�豸.png"));
		//pControl->SetVisible(flag);

		CControlUI* pControl_1 = static_cast<CControlUI*>(m_pm.FindControl(stringToWstring(StoreOptions_line3_1[i]).c_str()));
		pControl_1->SetText(stringToWstring(line_2[i]).c_str());

		CControlUI* pControl_2 = static_cast<CControlUI*>(m_pm.FindControl(stringToWstring(StoreOptions_JT2[i]).c_str()));
		pControl_2->SetForeImage(_T("ui/��ͷ2.png"));


		streamsssss.clear(); //clear flags.�����ת��ʱ�������ȵ����clear()
		streamsssss.str("");
	}

}
void CPopNetworkManage::parseElements_3()
{

	CControlUI* pControl1 = static_cast<CControlUI*>(m_pm.FindControl(stringToWstring("Com_Selct_BusinessType").c_str()));
	CDuiString cui_name = pControl1->GetText();
	str_BusinessType = wstringToString(cui_name.GetData());

	vector<string> vec_child;

	vector<string> vec_SheBei;//���Ҫ�����豸

	int num_child = pNode_3_1[selectNode_3[0][0]]->GetCountChild();//GetParentNode
	/******************��ȡ��ѡ������������豸 (��ǰ0���ڵ������һ���ڵ�����)******************/
	for (int i = 0; i < num_child; i++)
	{
		CTreeNodeUI*  node = pNode_3_1[selectNode_3[0][0]]->GetChildNode(i);//GetParentNode
		CDuiString cdui_name = node->GetItemText();
		wstring wstr_name = cdui_name.GetData();
		vec_child.push_back(wstringToString(wstr_name));
	}
	/****************����������������������ɸѡ������*************/
	for (int i = 0; i < vec_child.size(); i++)
	{
		int sequence;
		for (int j = 0; j < vec_level3_1.size(); j++)
		{
			if (vec_child[i] == vec_level3_1[j])
			{
				sequence = j;
			}
		}
		map<string, string> curr_map = m_mapLevel3_1[sequence];
		if (curr_map.count(UTF8_To_string("ҵ������")) > 0)
		{
			string value = curr_map[UTF8_To_string("ҵ������")];//�����ӡ��㲥��ͼ����Ƶ��������
			if (value == str_BusinessType)
			{
				vec_SheBei.push_back(vec_child[i]);//���Ҫ�ŵĵ͹�(LEO)/�й�(MEO)/�߹�(HEO)
			}

		}
	}
	cout << "1111111111" << endl;
	DrawAPicture_Tab3(vec_SheBei);
}
//����3�Ļ�ͼ
void CPopNetworkManage::DrawAPicture_Tab3(vector<string> vec_line1)
{
	HideThCcontro3_Draw(ControlNumber_Draw1);


	vector<string> line_1;//��ŵ�һ�е��豸����
	vector<string> line_2;//��ŵڶ��е��豸����
	if (vec_line1.size() > 0)
	{
		bool flag = false;
		for (int i = 0; i < vec_line1.size(); i++)
		{
			if ((i + 1) % 2)
			{
				line_1.push_back(vec_line1[i]);
			}
			else
			{
				line_2.push_back(vec_line1[i]);
			}
		}
	}
	/*******************��������һ��ͼ********************/
	string StoreOptions_line1[15];
	string StoreOptions_line1_1[15];
	string StoreOptions_JT1[15];
	string StoreOptions_LineMid[15];//�м����
	int num_Start_line1 = (15 - line_1.size()) / 2;
	for (int i = 0; i < line_1.size(); i++)
	{
		wstring w, kj;
		std::ostringstream streamsssss;
		streamsssss << (i + 1 + num_Start_line1);
		StoreOptions_line1[i] = "BJ_3_1_" + streamsssss.str();
		StoreOptions_line1_1[i] = "lable_3_1_" + streamsssss.str();
		StoreOptions_JT1[i] = "JT_3_1_" + streamsssss.str();
		StoreOptions_LineMid[i] = "BJ_3_2_" + streamsssss.str();
		UtilTool::setWstring(kj, StoreOptions_line1[i].c_str());
		CControlUI* pControl = static_cast<CControlUI*>(m_pm.FindControl(kj.c_str()));
		pControl->SetForeImage(_T("ui/�豸.png"));

		CControlUI* pControl_1 = static_cast<CControlUI*>(m_pm.FindControl(stringToWstring(StoreOptions_line1_1[i]).c_str()));
		pControl_1->SetText(stringToWstring(line_1[i]).c_str());


		CControlUI* pControl_2 = static_cast<CControlUI*>(m_pm.FindControl(stringToWstring(StoreOptions_JT1[i]).c_str()));
		pControl_2->SetForeImage(_T("ui/��ͷ2.png"));

		CControlUI* pControl_3 = static_cast<CControlUI*>(m_pm.FindControl(stringToWstring(StoreOptions_LineMid[i]).c_str()));
		pControl_3->SetForeImage(_T("ui/��.png"));


	}


	string StoreOptions_JT2[15];

	string StoreOptions_line3[15];
	string StoreOptions_line3_1[15];
	string StoreOptions_JT3[15];

	int num_Start_line2 = (15 - line_2.size()) / 2;
	for (int i = 0; i < line_2.size(); i++)
	{
		wstring w, kj;
		std::ostringstream streamsssss;
		streamsssss << (i + 1 + num_Start_line2);
		StoreOptions_JT2[i] = "JT_3_2_" + streamsssss.str();
		StoreOptions_line3[i] = "BJ_3_3_" + streamsssss.str();
		StoreOptions_line3_1[i] = "lable_3_3_" + streamsssss.str();
		UtilTool::setWstring(kj, StoreOptions_line3[i].c_str());
		CControlUI* pControl = static_cast<CControlUI*>(m_pm.FindControl(kj.c_str()));
		pControl->SetForeImage(_T("ui/�豸.png"));
		//pControl->SetVisible(flag);

		CControlUI* pControl_1 = static_cast<CControlUI*>(m_pm.FindControl(stringToWstring(StoreOptions_line3_1[i]).c_str()));
		pControl_1->SetText(stringToWstring(line_2[i]).c_str());

		CControlUI* pControl_2 = static_cast<CControlUI*>(m_pm.FindControl(stringToWstring(StoreOptions_JT2[i]).c_str()));
		pControl_2->SetForeImage(_T("ui/��ͷ2.png"));


		streamsssss.clear(); //clear flags.�����ת��ʱ�������ȵ����clear()
		streamsssss.str("");
	}

}
//��Tab1ѡ�е����ڵ�ȡ��ѡ��
void CPopNetworkManage::UncheckedTab1()
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

			}
		}
	}
}
//��Tab2ѡ�е����ڵ�ȡ��ѡ��
void CPopNetworkManage::UncheckedTab2()
{
	for (int i = 0; i < pNode_2_1_num; i++)
	{
		int childNum = pNode_2_1[i]->GetCountChild();
		if (pNode_2_1[i]->IsSelected())
		{
			pNode_2_1[i]->Select(false);
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

			}
		}
	}
}
//��Tab3ѡ�е����ڵ�ȡ��ѡ��
void CPopNetworkManage::UncheckedTab3()
{
	for (int i = 0; i < pNode_3_1_num; i++)
	{
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
void CPopNetworkManage::GenerateTheFile(string path, string filename, map<string, string> m_map)
{
	//string path1 = "D:\\DATA1";
	if (0 != _access(path.c_str(), 0))
	{
		_mkdir(path.c_str());   // ���� 0 ��ʾ�����ɹ���-1 ��ʾʧ��
	}
	string logdata = path + "\\" + filename + ".txt";

	ofstream fout(logdata, ios::out);


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
void CPopNetworkManage::GenerateTheFile(string path, string filename, vector<vector<string>> vecVec)
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

//�����ļ�
void CPopNetworkManage::GenerateTheFile(string path, string filename, map<string, string> m_map, string sign)
{
	//string path1 = "D:\\DATA1";
	if (0 != _access(path.c_str(), 0))
	{
		_mkdir(path.c_str());   // ���� 0 ��ʾ�����ɹ���-1 ��ʾʧ��
	}
	string logdata = path + "\\" + filename + ".txt";

	ofstream fout(logdata, ios::out);

	fout << sign << endl;for (map<string, string>::iterator iter = m_map.begin(); iter != m_map.end(); ++iter)
	{
		cout << "key:" << iter->first << " value:" << iter->second << endl;
		string str_first = iter->first;
		string str_second = iter->second;
		string str = str_first + ":";
		str += str_second;
		fout << str << endl;
	}
}
//��ȡtab1ҳ��ǰ�豸��Ϣ
map<string, string> CPopNetworkManage::GetCurrentEquipInform(string name)
{
	//map <string, string> recv_map;

	int sequences;//�����vector�е�˳��
	for (int i = 0; i < vec_recEquipmentName.size(); i++)
	{
		if (name == vec_recEquipmentName[i])
		{
			sequences = i;
		}
	}
	//DisplayInformation(m_RecvMap[sequences]);

	return m_RecvMap[sequences];
}
//��ȡtab2ҳ��ǰ�豸��Ϣ
map<string, string> CPopNetworkManage::GetCurrentEquipInform_tab2(string name)
{
	//map <string, string> recv_map;

	int sequences;//�����vector�е�˳��
	for (int i = 0; i < vec_level2_1.size(); i++)
	{
		if (name == vec_level2_1[i])
		{
			sequences = i;
		}
	}


	//DisplayInformation(m_RecvMap[sequences]);

	return m_mapLevel2_1[sequences];
}

//��ȡtab2ҳ��ǰ�豸��Ϣ
map<string, string> CPopNetworkManage::GetCurrentEquipInform_tab3(string name)
{
	//map <string, string> recv_map;

	int sequences;//�����vector�е�˳��
	for (int i = 0; i < vec_level3_1.size(); i++)
	{
		if (name == vec_level3_1[i])
		{
			sequences = i;
		}
	}


	//DisplayInformation(m_RecvMap[sequences]);

	return m_mapLevel3_1[sequences];
}
//tab1 ʵʱ�������ݵ��� ������ģ������
void CPopNetworkManage::RealTimeSceneData_Tab1()
{
	map<string, string> map11;
	map11["��������"] = "����1"; //, , 52.7, -80, 29, 120, 6.2, 29, 120, 0.1, 0.63, 0.1, 0.1;...
	map11["��������"] = "StarLink";
	map11["��������"] = "����";
	map11["��������"] = "SpaceZ";
	map11["�������"] = "ͨ������";
	map11["����������"] = "�͹�(LEO)"; //"�͹�(LEO)";
	map11["NORAD���"] = "44914U";
	map11["COPAR���"] = "20001A";
	map11["�Ƿ񹫿���Ϣ��ʶ"] = "5";
	map11["����볤��"] = "42912767.35";	//1 -------------����е�λ��
	map11["���ƫ����"] = "0.0001477";		//2  
	map11["������"] = "53.0535";			//3
	map11["������ྭ"] = "292.5393";		//4
	map11["���ص����"] = "71.8141";		//5
	map11["ֱ�����"] = "288.3008";			//6
	map11["������������"] = "�������������";

	map11["�����غ�����"] = "ͨ��";
	map11["��������"] = "����1";

	map11["EIRP"] = "52.7";					//7
	map11["SFD"] = " -80";					//8
	map11["����Ƶ��"] = "29";				//9
	map11["�������"] = "120";			//10
	map11["GTֵ"] = "6.2";					//11
	map11["����Ƶ��"] = "29";			//12
	map11["���մ���"] = "120";				//13
	map11["���߿ھ�"] = "0.1";			//14
	map11["����Ч��"] = "0.63";				//15
	map11["ָ�����"] = "0.1";			//16
	map11["�������"] = "0.1";				//17

	map11["������������"] = "�������������";
	saveDataLocally_WX(UTF8_To_string("����1"), map11);
	map<string, string> map12;
	map12["��������"] = "����2";
	map12["��������"] = "SpaceY";
	map12["����볤��"] = "42912800.794";	//1 -------------����е�λ��  
	map12["���ƫ����"] = "0.0001805";		//2 
	map12["������"] = "53.0525";			//3
	map12["������ྭ"] = "294.2545";		//4
	map12["���ص����"] = "64.6475";		//5
	map12["ֱ�����"] = "295.4701";			//6
	map12["������������"] = "�������������";

	map12["�����غ�����"] = "ͨ��";
	map12["��������"] = "����1";

	map12["EIRP"] = "52.7";					//7
	map12["SFD"] = " -80";					//8
	map12["����Ƶ��"] = "29";				//9
	map12["�������"] = "120";			//10
	map12["GTֵ"] = "6.2";					//11
	map12["����Ƶ��"] = "29";			//12
	map12["���մ���"] = "120";				//13
	map12["���߿ھ�"] = "0.1";			//14
	map12["����Ч��"] = "0.63";				//15
	map12["ָ�����"] = "0.1";			//16
	map12["�������"] = "0.1";				//17
	saveDataLocally_WX(UTF8_To_string("����2"), map12);
	map<string, string> map21;
	map21["��������"] = "����3";
	map21["��������"] = "StarLink";
	map21["��������"] = "����";
	map21["��������"] = "SpaceY";
	map21["�������"] = "ͨ������";
	map21["����������"] = "�߹�(HEO)";//"�߹�(HEO)";
	map21["�����غ�����"] = "ͨ��";
	map21["��������"] = "����0";

	map21["����볤��"] = "42912772.755";	//1 -------------����е�λ��  
	map21["���ƫ����"] = "0.0001554";		//2  
	map21["������"] = "53.0549";			//3
	map21["������ྭ"] = "294.2266";		//4
	map21["���ص����"] = "67.9407";		//5
	map21["ֱ�����"] = "292.1747";			//6
	map21["������������"] = "�������������";

	map21["�����غ�����"] = "ͨ��";
	map21["��������"] = "����1";

	map21["EIRP"] = "52.7";					//7
	map21["SFD"] = " -80";					//8
	map21["����Ƶ��"] = "29";				//9
	map21["�������"] = "120";			//10
	map21["GTֵ"] = "6.2";					//11
	map21["����Ƶ��"] = "29";			//12
	map21["���մ���"] = "120";				//13
	map21["���߿ھ�"] = "0.1";			//14
	map21["����Ч��"] = "0.63";				//15
	map21["ָ�����"] = "0.1";			//16
	map21["�������"] = "0.1";				//17
	saveDataLocally_WX(UTF8_To_string("����3"), map21);

	map<string, string> map22;
	map22["��������"] = "����4";
	map22["��������"] = "StarLink";
	map22["��������"] = "����";
	map22["��������"] = "SpaceX";
	map22["�������"] = "ͨ������";
	map22["����������"] = "�߹�(HEO)";//"�߹�(HEO)";
	map22["�����غ�����"] = "ͨ��";
	map22["��������"] = "����0";

	map22["����볤��"] = "42878846.391";	//1 -------------����е�λ��,
	map22["���ƫ����"] = "0.0001637";		//2  4, , , 53.0018, 262.1977, 73.6559, 286.4614, 52.7, -80, 29, 120, 6.2, 29, 120, 0.1, 0.63, 0.1, 0.1;...
	map22["������"] = "53.0018";			//3
	map22["������ྭ"] = "262.1977";		//4
	map22["���ص����"] = "73.6559";		//5
	map22["ֱ�����"] = "286.4614";			//6
	map22["������������"] = "�������������";

	map22["�����غ�����"] = "ͨ��";
	map22["��������"] = "����1";

	map22["EIRP"] = "52.7";					//7
	map22["SFD"] = " -80";					//8
	map22["����Ƶ��"] = "29";				//9
	map22["�������"] = "120";			//10
	map22["GTֵ"] = "6.2";					//11
	map22["����Ƶ��"] = "29";			//12
	map22["���մ���"] = "120";				//13
	map22["���߿ھ�"] = "0.1";			//14
	map22["����Ч��"] = "0.63";				//15
	map22["ָ�����"] = "0.1";			//16
	map22["�������"] = "0.1";				//17
	saveDataLocally_WX(UTF8_To_string("����4"), map22);

	map<string, string> map25;
	map25["��������"] = "����5";
	map25["��������"] = "StarLink";
	map25["��������"] = "����";
	map25["��������"] = "SpaceZ";
	map25["�������"] = "ͨ������";
	map25["����������"] = "�߹�(HEO)";//"�߹�(HEO)";
	map25["�����غ�����"] = "ͨ��";
	map25["��������"] = "����0";

	map25["����볤��"] = "42912811.548";	//1 -------------����е�λ��,
	map25["���ƫ����"] = "0.0001501";		//2   52.7, -80, 29, 120, 6.2, 29, 120, 0.1, 0.63, 0.1, 0.1;...
	map25["������"] = "53.0548";			//3
	map25["������ྭ"] = "293.813";		//4
	map25["���ص����"] = "60.3975";		//5
	map25["ֱ�����"] = "299.7163";			//6
	map25["������������"] = "�������������";

	map25["�����غ�����"] = "ͨ��";
	map25["��������"] = "����1";

	map25["EIRP"] = "52.7";					//7
	map25["SFD"] = " -80";					//8
	map25["����Ƶ��"] = "29";				//9
	map25["�������"] = "120";			//10
	map25["GTֵ"] = "6.2";					//11
	map25["����Ƶ��"] = "29";			//12
	map25["���մ���"] = "120";				//13
	map25["���߿ھ�"] = "0.1";			//14
	map25["����Ч��"] = "0.63";				//15
	map25["ָ�����"] = "0.1";			//16
	map25["�������"] = "0.1";				//17
	saveDataLocally_WX(UTF8_To_string("����5"), map25);

	map<string, string> map26;
	map26["��������"] = "����6";
	map26["��������"] = "StarLink";
	map26["��������"] = "����";
	map26["��������"] = "SpaceY";
	map26["�������"] = "ͨ������";
	map26["����������"] = "�߹�(HEO)";//"�߹�(HEO)";
	map26["�����غ�����"] = "ͨ��";
	map26["��������"] = "����0";

	map26["����볤��"] = "42705713.283";	//1 -------------����е�λ��,
	map26["���ƫ����"] = "0.0006317";		//2   , , , , , , 52.7, -80, 29, 120, 6.2, 29, 120, 0.1, 0.63, 0.1, 0.1;...
	map26["������"] = "53.0479";			//3
	map26["������ྭ"] = "153.0687";		//4
	map26["���ص����"] = "310.1842";		//5
	map26["ֱ�����"] = "49.8624";			//6
	map26["������������"] = "�������������";

	map26["�����غ�����"] = "ͨ��";
	map26["��������"] = "����1";

	map26["EIRP"] = "52.7";					//7
	map26["SFD"] = " -80";					//8
	map26["����Ƶ��"] = "29";				//9
	map26["�������"] = "120";			//10
	map26["GTֵ"] = "6.2";					//11
	map26["����Ƶ��"] = "29";			//12
	map26["���մ���"] = "120";				//13
	map26["���߿ھ�"] = "0.1";			//14
	map26["����Ч��"] = "0.63";				//15
	map26["ָ�����"] = "0.1";			//16
	map26["�������"] = "0.1";				//17
	saveDataLocally_WX(UTF8_To_string("����6"), map26);

	map<string, string> map27;
	map27["��������"] = "����7";
	map27["��������"] = "StarLink";
	map27["��������"] = "����";
	map27["��������"] = "SpaceX";
	map27["�������"] = "ͨ������";
	map27["����������"] = "�߹�(HEO)";//"�߹�(HEO)";
	map27["�����غ�����"] = "ͨ��";
	map27["��������"] = "����0";

	map27["����볤��"] = "42912752.693";	//1 -------------����е�λ��,
	map27["���ƫ����"] = "0.0001733";		//2  ,  , , ,, 52.7, -80, 29, 120, 6.2, 29, 120, 0.1, 0.63, 0.1, 0.1;...
	map27["������"] = "53.0522";			//3
	map27["������ྭ"] = "292.4151";		//4
	map27["���ص����"] = "89.8962";		//5
	map27["ֱ�����"] = "270.2225";			//6
	map27["������������"] = "�������������";

	map27["�����غ�����"] = "ͨ��";
	map27["��������"] = "����1";

	map27["EIRP"] = "52.7";					//7
	map27["SFD"] = " -80";					//8
	map27["����Ƶ��"] = "29";				//9
	map27["�������"] = "120";			//10
	map27["GTֵ"] = "6.2";					//11
	map27["����Ƶ��"] = "29";			//12
	map27["���մ���"] = "120";				//13
	map27["���߿ھ�"] = "0.1";			//14
	map27["����Ч��"] = "0.63";				//15
	map27["ָ�����"] = "0.1";			//16
	map27["�������"] = "0.1";				//17
	saveDataLocally_WX(UTF8_To_string("����7"), map27);

	map<string, string> map28;
	map28["��������"] = "����8";
	map28["��������"] = "StarLink";
	map28["��������"] = "����";
	map28["��������"] = "SpaceY";
	map28["�������"] = "ͨ������";
	map28["����������"] = "�߹�(HEO)";//"�߹�(HEO)";
	map28["�����غ�����"] = "ͨ��";
	map28["��������"] = "����0";

	map28["����볤��"] = "42912807.724";	//1 -------------����е�λ��,
	map28["���ƫ����"] = "0.0001789";		//2 42912807.724, 0.0001789, 53.0533, 292.9965, , 310.1595, 
	map28["������"] = "53.0533";			//3
	map28["������ྭ"] = "292.9965";		//4
	map28["���ص����"] = "49.9551";		//5
	map28["ֱ�����"] = "310.1595";			//6
	map28["������������"] = "�������������";

	map28["�����غ�����"] = "ͨ��";
	map28["��������"] = "����1";

	map28["EIRP"] = "52.7";					//7
	map28["SFD"] = " -80";					//8
	map28["����Ƶ��"] = "29";				//9
	map28["�������"] = "120";			//10
	map28["GTֵ"] = "6.2";					//11
	map28["����Ƶ��"] = "29";			//12
	map28["���մ���"] = "120";				//13
	map28["���߿ھ�"] = "0.1";			//14
	map28["����Ч��"] = "0.63";				//15
	map28["ָ�����"] = "0.1";			//16
	map28["�������"] = "0.1";				//17
	saveDataLocally_WX(UTF8_To_string("����8"), map28);

	map<string, string> map29;
	map29["��������"] = "����9";
	map29["��������"] = "StarLink";
	map29["��������"] = "����";
	map29["��������"] = "SpaceY";
	map29["�������"] = "ͨ������";
	map29["����������"] = "�߹�(HEO)";//"�߹�(HEO)";
	map29["�����غ�����"] = "ͨ��";
	map29["��������"] = "����0";

	map29["����볤��"] = "42912764.633";	//1 -------------����е�λ��,
	map29["���ƫ����"] = "0.0001357";		//2 42912764.633, 0.0001357, 53.0548, 292.8573, 72.5682, 287.5455, 52.7, -80, 29, 120, 6.2, 29, 120, 0.1, 0.63, 0.1, 0.1;...
	map29["������"] = "53.0548";			//3
	map29["������ྭ"] = "292.8573";		//4
	map29["���ص����"] = "72.5682";		//5
	map29["ֱ�����"] = "287.5455";			//6
	map29["������������"] = "�������������";

	map29["�����غ�����"] = "ͨ��";
	map29["��������"] = "����1";

	map29["EIRP"] = "52.7";					//7
	map29["SFD"] = " -80";					//8
	map29["����Ƶ��"] = "29";				//9
	map29["�������"] = "120";			//10
	map29["GTֵ"] = "6.2";					//11
	map29["����Ƶ��"] = "29";			//12
	map29["���մ���"] = "120";				//13
	map29["���߿ھ�"] = "0.1";			//14
	map29["����Ч��"] = "0.63";				//15
	map29["ָ�����"] = "0.1";			//16
	map29["�������"] = "0.1";				//17
	saveDataLocally_WX(UTF8_To_string("����9"), map29);

	map<string, string> map210;
	map210["��������"] = "����10";
	map210["��������"] = "StarLink";
	map210["��������"] = "����";
	map210["��������"] = "SpaceY";
	map210["�������"] = "ͨ������";
	map210["����������"] = "�߹�(HEO)";//"�߹�(HEO)";
	map210["�����غ�����"] = "ͨ��";
	map210["��������"] = "����0";

	map210["����볤��"] = "42912770.62";	//1 -------------����е�λ��,
	map210["���ƫ����"] = "0.0001622";		//2  42912770.62, , 53.0551, 295.3636, 57.0431, 303.0714, 52.7, -80, 29, 120, 6.2, 29, 120, 0.1, 0.63, 0.1, 0.1;...
	map210["������"] = "53.0551";			//3
	map210["������ྭ"] = "295.3636";		//4
	map210["���ص����"] = "57.0431";		//5
	map210["ֱ�����"] = "303.0714";			//6
	map210["������������"] = "�������������";

	map210["�����غ�����"] = "ͨ��";
	map210["��������"] = "����1";

	map210["EIRP"] = "52.7";					//7
	map210["SFD"] = " -80";					//8
	map210["����Ƶ��"] = "29";				//9
	map210["�������"] = "120";			//10
	map210["GTֵ"] = "6.2";					//11
	map210["����Ƶ��"] = "29";			//12
	map210["���մ���"] = "120";				//13
	map210["���߿ھ�"] = "0.1";			//14
	map210["����Ч��"] = "0.63";				//15
	map210["ָ�����"] = "0.1";			//16
	map210["�������"] = "0.1";				//17
	saveDataLocally_WX(UTF8_To_string("����10"), map210);

	map<string, string> map2_01;
	map2_01["����վ����"] = "����վ1";
	map2_01["��������"] = "�ؿ�վ";
	map2_01["����վ����"] = "�̶�";
	map2_01["��������"] = "SpaceY";

	map2_01["����"] = "-126";			// 1  
	map2_01["γ��"] = "39";				// 2
	map2_01["�߶�"] = "0";				// 3
	map2_01["EIRP"] = "20";			// 7
	map2_01["����Ƶ��"] = "-80";		// 8
	map2_01["�������"] = "29";		// 9
	map2_01["FEC"] = "120";			// 10
	map2_01["GTֵ"] = "6.2";			// 11
	map2_01["����Ƶ��"] = "29";		// 12
	map2_01["���մ���"] = "120";		//13
	map2_01["���߿ھ�"] = "0.1";		//14
	map2_01["����Ч��"] = "0.63";		//15
	map2_01["ָ�����"] = "0.1";		//16
	map2_01["�������"] = "0.1";		//17
	saveDataLocally_DQZ(UTF8_To_string("����վ1"), map2_01);
	map<string, string> map2_02;
	map2_02["����վ����"] = "����վ2";
	map2_02["��������"] = "�Ź�վ";
	map2_02["����վ����"] = "�ƶ�";
	map2_02["��������"] = "SpaceZ";

	map2_02["����"] = "-120";			// 1  
	map2_02["γ��"] = "30";				// 2
	map2_02["�߶�"] = "0";				// 3
	map2_02["EIRP"] = "20";			// 7
	map2_02["����Ƶ��"] = "-80";		// 8
	map2_02["�������"] = "29";		// 9
	map2_02["FEC"] = "120";			// 10
	map2_02["GTֵ"] = "6.2";			// 11
	map2_02["����Ƶ��"] = "29";		// 12
	map2_02["���մ���"] = "120";		//13
	map2_02["���߿ھ�"] = "0.1";		//14
	map2_02["����Ч��"] = "0.63";		//15
	map2_02["ָ�����"] = "0.1";		//16
	map2_02["�������"] = "0.1";		//17
	saveDataLocally_DQZ(UTF8_To_string("����վ2"), map2_02);
	map<string, string> map2_03;
	map2_03["����վ����"] = "����վ3";
	map2_03["��������"] = "���վ";
	map2_03["����վ����"] = "�̶�";
	map2_03["��������"] = "SpaceX";

	map2_03["����"] = "-118";			// 1  
	map2_03["γ��"] = "50";				// 2
	map2_03["�߶�"] = "0";				// 3
	map2_03["EIRP"] = "20";			// 7
	map2_03["����Ƶ��"] = "-80";		// 8
	map2_03["�������"] = "29";		// 9
	map2_03["FEC"] = "120";			// 10
	map2_03["GTֵ"] = "6.2";			// 11
	map2_03["����Ƶ��"] = "29";		// 12
	map2_03["���մ���"] = "120";		//13
	map2_03["���߿ھ�"] = "0.1";		//14
	map2_03["����Ч��"] = "0.63";		//15
	map2_03["ָ�����"] = "0.1";		//16
	map2_03["�������"] = "0.1";		//17
	saveDataLocally_DQZ(UTF8_To_string("����վ3"), map2_03);

	map<string, string> map2_04;
	map2_04["����վ����"] = "����վ4";
	map2_04["��������"] = "���վ";
	map2_04["����վ����"] = "�̶�";
	map2_04["��������"] = "SpaceX";

	map2_04["����"] = "-130";			// 1  
	map2_04["γ��"] = "55";				// 2
	map2_04["�߶�"] = "0";				// 3
	map2_04["EIRP"] = "20";			// 7
	map2_04["����Ƶ��"] = "-80";		// 8
	map2_04["�������"] = "29";		// 9
	map2_04["FEC"] = "120";			// 10
	map2_04["GTֵ"] = "6.2";			// 11
	map2_04["����Ƶ��"] = "29";		// 12
	map2_04["���մ���"] = "120";		//13
	map2_04["���߿ھ�"] = "0.1";		//14
	map2_04["����Ч��"] = "0.63";		//15
	map2_04["ָ�����"] = "0.1";		//16
	map2_04["�������"] = "0.1";		//17
	saveDataLocally_DQZ(UTF8_To_string("����վ4"), map2_04);


	map<string, string> map3_1;
	map3_1["�����ն�����"] = "�����ն�1";
	map3_1["�ն�����"] = "�ֻ�";
	map3_1["��������"] = "SpaceZ";

	map3_1["����"] = "-110";			// 1     
	map3_1["γ��"] = "45";				// 2
	map3_1["�߶�"] = "0";				// 3
	map3_1["EIRP"] = "20";			// 7
	map3_1["����Ƶ��"] = "-80";		// 8
	map3_1["�������"] = "29";		// 9
	map3_1["FEC"] = "120";			// 10
	map3_1["GTֵ"] = "6.2";			// 11
	map3_1["����Ƶ��"] = "29";		// 12
	map3_1["���մ���"] = "120";		//13
	map3_1["���߿ھ�"] = "0.1";		//14
	map3_1["����Ч��"] = "0.63";		//15
	map3_1["ָ�����"] = "0.1";		//16
	map3_1["�������"] = "0.1";		//17
	saveDataLocally_DMZD(UTF8_To_string("�����ն�1"), map3_1);
	map<string, string> map3_2;
	map3_2["�����ն�����"] = "�����ն�2";
	map3_2["�ն�����"] = "���ǵ绰";
	map3_2["��������"] = "SpaceX";

	map3_2["����"] = "-90";			// 1      20, -80, 29, 120, 6.2, 29, 120, 0.1, 0.63, 0.1, 0.1;...
	map3_2["γ��"] = "30";				// 2
	map3_2["�߶�"] = "0";				// 3
	map3_2["EIRP"] = "20";			// 7
	map3_2["����Ƶ��"] = "-80";		// 8
	map3_2["�������"] = "29";		// 9
	map3_2["FEC"] = "120";			// 10
	map3_2["GTֵ"] = "6.2";			// 11
	map3_2["����Ƶ��"] = "29";		// 12
	map3_2["���մ���"] = "120";		//13
	map3_2["���߿ھ�"] = "0.1";		//14
	map3_2["����Ч��"] = "0.63";		//15
	map3_2["ָ�����"] = "0.1";		//16
	map3_2["�������"] = "0.1";		//17
	saveDataLocally_DMZD(UTF8_To_string("�����ն�2"), map3_2);
	//map<string, string> map3_3;
	//map3_3["�����ն�����"] = "�����ն�3";
	//map3_3["�ն�����"] = "���ǵ绰1";
	//map3_3["��������"] = "SpaceY";

	//map3_3["����"] = "-100";			// 1     
	//map3_3["γ��"] = "50";				// 2
	//map3_3["�߶�"] = "0";				// 3
	//map3_3["EIRP"] = "20";			// 7
	//map3_3["����Ƶ��"] = "-80";		// 8
	//map3_3["�������"] = "29";		// 9
	//map3_3["FEC"] = "120";			// 10
	//map3_3["GTֵ"] = "6.2";			// 11
	//map3_3["����Ƶ��"] = "29";		// 12
	//map3_3["���մ���"] = "120";		//13
	//map3_3["���߿ھ�"] = "0.1";		//14
	//map3_3["����Ч��"] = "0.63";		//15
	//map3_3["ָ�����"] = "0.1";		//16
	//map3_3["�������"] = "0.1";		//17
	//saveDataLocally_DMZD(UTF8_To_string("�����ն�3"), map3_3);
}
//tab2 ʵʱ�������ݵ��� ������ģ������
void CPopNetworkManage::RealTimeSceneData_Tab2()
{

}
//tab3 ʵʱ�������ݵ��� ������ģ������
void CPopNetworkManage::RealTimeSceneData_Tab3()
{

}

//�������ݵĻ������Ի�ͼ ���������Ǹ����пؼ����óɲ��ɼ�
void CPopNetworkManage::NotDrawable(string tabName, bool flag)
{
	if (tabName == "tab1")
	{
		CControlUI* pControlableS = static_cast<CControlUI*>(m_pm.FindControl(stringToWstring("lable_StartingPoint").c_str()));//���lable
		pControlableS->SetVisible(flag);//False false
		CControlUI* pControlComboxS = static_cast<CControlUI*>(m_pm.FindControl(stringToWstring("Com_StartingPoint").c_str()));//���combox
		pControlComboxS->SetVisible(flag);//False false 
		CControlUI* pControlableE = static_cast<CControlUI*>(m_pm.FindControl(stringToWstring("lable_EndPoint").c_str()));//���lable
		pControlableE->SetVisible(flag);//False false
		CControlUI* pControlComboxE = static_cast<CControlUI*>(m_pm.FindControl(stringToWstring("Com_EndPoint").c_str()));//���combox
		pControlComboxE->SetVisible(flag);//False false
		CControlUI* pControl = static_cast<CControlUI*>(m_pm.FindControl(stringToWstring("bt_Drawing_tab1").c_str()));
		pControl->SetVisible(flag);//False false
	}
	else if (tabName == "tab2")
	{
		CControlUI* pControl1 = static_cast<CControlUI*>(m_pm.FindControl(stringToWstring("lable_SX").c_str()));//���lable
		pControl1->SetVisible(flag);//False false
		CControlUI* pControl2 = static_cast<CControlUI*>(m_pm.FindControl(stringToWstring("edit_FilterCriteria").c_str()));//���combox
		pControl2->SetVisible(flag);//False false 
		CControlUI* pControl3 = static_cast<CControlUI*>(m_pm.FindControl(stringToWstring("lable_Sx_Value").c_str()));//���lable
		pControl3->SetVisible(flag);//False false
		CControlUI* pControl4 = static_cast<CControlUI*>(m_pm.FindControl(stringToWstring("edit_FilterCriteriaValue").c_str()));//���combox
		pControl4->SetVisible(flag);//False false
		CControlUI* pControl5 = static_cast<CControlUI*>(m_pm.FindControl(stringToWstring("bt_Sift_tab2").c_str()));
		pControl5->SetVisible(flag);//False false
		/********************��ͼ*****************/
		CControlUI* pControlableS = static_cast<CControlUI*>(m_pm.FindControl(stringToWstring("lable_StartingPoint_tab2").c_str()));//���lable
		pControlableS->SetVisible(flag);//False false
		CControlUI* pControlComboxS = static_cast<CControlUI*>(m_pm.FindControl(stringToWstring("Com_StartingPoint_tab2").c_str()));//���combox
		pControlComboxS->SetVisible(flag);//False false 
		CControlUI* pControlableE = static_cast<CControlUI*>(m_pm.FindControl(stringToWstring("lable_EndPoint_tab2").c_str()));//���lable
		pControlableE->SetVisible(flag);//False false
		CControlUI* pControlComboxE = static_cast<CControlUI*>(m_pm.FindControl(stringToWstring("Com_EndPoint_tab2").c_str()));//���combox
		pControlComboxE->SetVisible(flag);//False false
		CControlUI* pControl = static_cast<CControlUI*>(m_pm.FindControl(stringToWstring("bt_Drawing_tab2").c_str()));
		pControl->SetVisible(flag);//False false
	}
	else if (tabName == "tab3")
	{
		CControlUI* pControl1 = static_cast<CControlUI*>(m_pm.FindControl(stringToWstring("lable_SX_tab3").c_str()));//���lable
		pControl1->SetVisible(flag);//False false
		CControlUI* pControl2 = static_cast<CControlUI*>(m_pm.FindControl(stringToWstring("edit_FilterCriteria_tab3").c_str()));//���combox
		pControl2->SetVisible(flag);//False false 
		CControlUI* pControl3 = static_cast<CControlUI*>(m_pm.FindControl(stringToWstring("lable_Sx_Value_tab3").c_str()));//���lable
		pControl3->SetVisible(flag);//False false
		CControlUI* pControl4 = static_cast<CControlUI*>(m_pm.FindControl(stringToWstring("edit_FilterCriteriaValue_tab3").c_str()));//���combox
		pControl4->SetVisible(flag);//False false
		CControlUI* pControl5 = static_cast<CControlUI*>(m_pm.FindControl(stringToWstring("bt_Sift_tab3").c_str()));
		pControl5->SetVisible(flag);//False false
		/********************��ͼ*****************/
		CControlUI* pControlableS = static_cast<CControlUI*>(m_pm.FindControl(stringToWstring("lable_StartingPoint_tab3").c_str()));//���lable
		pControlableS->SetVisible(flag);//False false
		CControlUI* pControlComboxS = static_cast<CControlUI*>(m_pm.FindControl(stringToWstring("Com_StartingPoint_tab3").c_str()));//���combox
		pControlComboxS->SetVisible(flag);//False false 
		CControlUI* pControlableE = static_cast<CControlUI*>(m_pm.FindControl(stringToWstring("lable_EndPoint_tab3").c_str()));//���lable
		pControlableE->SetVisible(flag);//False false
		CControlUI* pControlComboxE = static_cast<CControlUI*>(m_pm.FindControl(stringToWstring("Com_EndPoint_tab3").c_str()));//���combox
		pControlComboxE->SetVisible(flag);//False false
		CControlUI* pControl = static_cast<CControlUI*>(m_pm.FindControl(stringToWstring("bt_Drawing_tab3").c_str()));
		pControl->SetVisible(flag);//False false
	}
}
//��tab1����ı������ݵ�����溯��
void CPopNetworkManage::ImportingLocalData_Tab1()
{
	vector<string> vec_WxName;
	vector<map<string, string>> vec_wxData;//�����ļ������������ݴ洢����
	vector<vector<string>> vecXZAndWX;//ÿһ�ж��������:����-����-������
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
		if (str_xzName[0] == UTF8_To_string("�����ļ�"))
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
							if (str_wx[i] != "")
							{
								line.push_back(str_wx[i]);
							}
							//line.push_back(str_wx[i]);
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
							if (str_wx[i] != "")
							{
								line.push_back(str_wx[i]);
							}
							//line.push_back(string_To_UTF8(str_wx[i]));
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
							if (str_wx[i] != "")
							{
								line.push_back(str_wx[i]);
							}

							
						}
					}
					vecXZAndWX.push_back(line);
				}


			}

			cout << "11111" << endl;
			infile.close();
		}
		else
		{
			ifstream fin(fileN, ios::binary);
			unsigned char  s2;
			fin.read((char*)&s2, sizeof(s2));//��ȡ��һ���ֽڣ�Ȼ������8λ
			int p = s2 << 8;
			fin.read((char*)&s2, sizeof(s2));//��ȡ�ڶ����ֽ�
			p += s2;

			string code;
			fin.close();
			map<string, string> l_map;
			ifstream infile;
			infile.open(fileN);   //���ļ����������ļ��������� 
			//assert(infile.is_open());   //��ʧ��,�����������Ϣ,����ֹ�������� 
			if (infile)
			{
				string s;

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
					saveDataLocally_WX(str_xzName[0], l_map);
				}
				else if (type == "����վ")
				{
					saveDataLocally_DQZ(str_xzName[0], l_map);
				}
				else if (type == "�����ն�")
				{
					saveDataLocally_DMZD(str_xzName[0], l_map);
				}

			}


			//vec_recEquipmentName.push_back(str_xzName[0]);//���յ����豸���� ���ǡ�����վ���ն�
			//m_RecvMap.push_back(l_map);//��������豸��������Ϣ ���ǡ�����վ���ն�


			infile.close();
		}
          //�ر��ļ������� 
	}
	///*********************�������ڵ�---��������********************/
	for (int i = 0; i < vecXZAndWX.size(); i++)
	{
		if (i == 0)
		{
			pNode_1[0]->SetItemText(stringToWstring(vecXZAndWX[0][0]).c_str());
		}
		else
		{
			CreateTreeNode(vecXZAndWX[i][0]);//����ڵ������ UTF8_To_string("�����·����")
		}
	}
	/***********************�����豸���ڽڵ�1���ڵ�******************/
	for (int i = 0; i < vecXZAndWX.size(); i++)
	{

		for (int j = 0; j < vecXZAndWX[i].size(); j++)
		{
			if (j > 0)
			{
				int sequese = -1;
				sequese = getRootNodeOrder(vecXZAndWX[i][0]);
				CreateTreeNode_livel1(vecXZAndWX[i][j], sequese);


			}

		}
	}
	cout << "11111111" << endl;
}
void CPopNetworkManage::ImportingLocalData_Tab2()
{
	vector<string> vec_WxName;
	vector<map<string, string>> vec_wxData;//�����ļ������������ݴ洢����
	vector<vector<string>> vecXZAndWX;//ÿһ�ж��������:����-����-������
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
		if (str_xzName[0] == UTF8_To_string("�����ļ�"))
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
							if (str_wx[i] != "")
							{
								line.push_back(str_wx[i]);
							}
							//line.push_back(str_wx[i]);
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
							if (str_wx[i] != "")
							{
								line.push_back(str_wx[i]);
							}
							//line.push_back(string_To_UTF8(str_wx[i]));
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
							if (str_wx[i] != "")
							{
								line.push_back(str_wx[i]);
							}


						}
					}
					vecXZAndWX.push_back(line);
				}


			}

			cout << "11111" << endl;
			infile.close();
		}
		else
		{
			ifstream fin(fileN, ios::binary);
			unsigned char  s2;
			fin.read((char*)&s2, sizeof(s2));//��ȡ��һ���ֽڣ�Ȼ������8λ
			int p = s2 << 8;
			fin.read((char*)&s2, sizeof(s2));//��ȡ�ڶ����ֽ�
			p += s2;

			string code;
			fin.close();
			map<string, string> l_map;
			ifstream infile;
			infile.open(fileN);   //���ļ����������ļ��������� 
			//assert(infile.is_open());   //��ʧ��,�����������Ϣ,����ֹ�������� 
			if (infile)
			{
				string s;

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
				vec_level2_1.push_back(str_xzName[0]);
				m_mapLevel2_1.push_back(l_map);
			}
			infile.close();
		}
		//�ر��ļ������� 
	}



///*********************�������ڵ�---��������********************/
	for (int i = 0; i < vecXZAndWX.size(); i++)
	{
		if (i == 0)
		{
			pNode_2_1[0]->SetItemText(stringToWstring(vecXZAndWX[0][0]).c_str());
		}
		else
		{
			CreateTreeNode_2(vecXZAndWX[i][0]);//����ڵ������ UTF8_To_string("�����·����")
		}
	}
	/***********************�����豸���ڽڵ�1���ڵ�******************/
	for (int i = 0; i < vecXZAndWX.size(); i++)
	{

		for (int j = 0; j < vecXZAndWX[i].size(); j++)
		{
			if (j > 0)
			{
				int sequese = -1;
				sequese = getRootNodeOrder_2(vecXZAndWX[i][0]);
				CreateTreeNode_livel1_2(vecXZAndWX[i][j], sequese);
			}

			//CreateTreeNode_livel1();
		}
	}

}
void CPopNetworkManage::ImportingLocalData_Tab3()
{
	vector<string> vec_WxName;
	vector<map<string, string>> vec_wxData;//�����ļ������������ݴ洢����
	vector<vector<string>> vecXZAndWX;//ÿһ�ж��������:����-����-������
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
		if (str_xzName[0] == UTF8_To_string("�����ļ�"))
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
							if (str_wx[i] != "")
							{
								line.push_back(str_wx[i]);
							}
							//line.push_back(str_wx[i]);
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
							if (str_wx[i] != "")
							{
								line.push_back(str_wx[i]);
							}
							//line.push_back(string_To_UTF8(str_wx[i]));
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
							if (str_wx[i] != "")
							{
								line.push_back(str_wx[i]);
							}


						}
					}
					vecXZAndWX.push_back(line);
				}


			}

			cout << "11111" << endl;
			infile.close();
		}
		else
		{
			ifstream fin(fileN, ios::binary);
			unsigned char  s2;
			fin.read((char*)&s2, sizeof(s2));//��ȡ��һ���ֽڣ�Ȼ������8λ
			int p = s2 << 8;
			fin.read((char*)&s2, sizeof(s2));//��ȡ�ڶ����ֽ�
			p += s2;

			string code;
			fin.close();
			map<string, string> l_map;
			ifstream infile;
			infile.open(fileN);   //���ļ����������ļ��������� 
			//assert(infile.is_open());   //��ʧ��,�����������Ϣ,����ֹ�������� 
			if (infile)
			{
				string s;

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
				vec_level3_1.push_back(str_xzName[0]);
				m_mapLevel3_1.push_back(l_map);
			}
			infile.close();
		}
		//�ر��ļ������� 
	}
	///*********************�������ڵ�---��������********************/
	for (int i = 0; i < vecXZAndWX.size(); i++)
	{
		if (i == 0)
		{
			pNode_3_1[0]->SetItemText(stringToWstring(vecXZAndWX[0][0]).c_str());
		}
		else
		{
			CreateTreeNode_3(vecXZAndWX[i][0]);//����ڵ������ UTF8_To_string("�����·����")
		}
	}
	/***********************�����豸���ڽڵ�1���ڵ�******************/
	for (int i = 0; i < vecXZAndWX.size(); i++)
	{

		for (int j = 0; j < vecXZAndWX[i].size(); j++)
		{
			if (j > 0)
			{
				int sequese = -1;
				sequese = getRootNodeOrder_3(vecXZAndWX[i][0]);
				CreateTreeNode_livel1_3(vecXZAndWX[i][j], sequese);
			}

			//CreateTreeNode_livel1();
		}
	}
}
//tab1 ��ȡ���豸���ڵĸ��ڵ�λ��
int CPopNetworkManage::getRootNodeOrder(string name)
{
	int sequese = -1;
	for (int i = 0; i < pNode_1_num; i++)
	{
		//CTreeNodeUI*  node = pNode_1[i]->GetChildNode(j);//GetParentNode
		string RootName = wstringToString(pNode_1[i]->GetItemText().GetData());
		if (name == RootName)
		{
			sequese = i;
		}
	}
	return sequese;
}
//tab2 ��ȡ���豸���ڵĸ��ڵ�λ��
int CPopNetworkManage::getRootNodeOrder_2(string name)
{
	int sequese = -1;
	for (int i = 0; i < pNode_2_1_num; i++)
	{
		//CTreeNodeUI*  node = pNode_1[i]->GetChildNode(j);//GetParentNode
		string RootName = wstringToString(pNode_2_1[i]->GetItemText().GetData());
		if (name == RootName)
		{
			sequese = i;
		}
	}
	return sequese;
}
//tab2 ��ȡ���豸���ڵĸ��ڵ�λ��
int CPopNetworkManage::getRootNodeOrder_3(string name)
{
	int sequese = -1;
	for (int i = 0; i < pNode_3_1_num; i++)
	{
		//CTreeNodeUI*  node = pNode_1[i]->GetChildNode(j);//GetParentNode
		string RootName = wstringToString(pNode_3_1[i]->GetItemText().GetData());
		if (name == RootName)
		{
			sequese = i;
		}
	}
	return sequese;
}
//tab1 ��ȡ��ǰ�豸�Ƿ��Ѿ��洢
bool CPopNetworkManage::getWhetherDeviceStored(string name)
{
	bool flag;
	if (vec_recEquipmentName.size() > 0)
	{
		for (int i = 0; i < vec_recEquipmentName.size(); i++)
		{
			if (name == vec_recEquipmentName[i])
			{
				flag = true;
				break;
			}
			else
			{
				flag = false;
			}
		}
	}
	else
	{
		flag = false;
	}

	return flag;
}
//ˢ�½��� ��������0
void CPopNetworkManage::RefreshTheInterface_tab1()
{
	vec_recEquipmentName.clear();
	m_RecvMap.clear();

	/***************�����ڵ����***************/
	for (int i = 0; i < pNode_1_num; i++)
	{
		int childNum = pNode_1[i]->GetCountChild();
		for (int j = 0; j < childNum; j++)
		{
			pNode_1[i]->Remove(pNode_2[j]);
		}
	}
	memset(pNode_2, 0, sizeof(pNode_2));
	pNode_2_num = 0;


	int num = pNode_1_num;
	cout << num << endl;
	for (int i = 0; i < (num - 1); i++)
	{
		m_treeView->Remove(pNode_1[i + 1]);
		pNode_1_num -= 1;
	}
	/***********��������ǡ�����վ�������ն����ƺ���Ϣ��vector���***********/
	vec_recSatelliteName.clear();
	m_RecvSatelliteMap.clear();

	vec_recEarthStationName.clear();
	m_RecvEarthStationMap.clear();

	vec_recGroundTerminalName.clear();
	m_RecvGroundTerminalMap.clear();
	/***********��ѡ�������յ��Combox���************/
	CComboBoxUI* Com_StartingPoint = static_cast<CComboBoxUI*>(m_pm.FindControl(_T("Com_StartingPoint")));//���
	CComboBoxUI* Com_EndPoint = static_cast<CComboBoxUI*>(m_pm.FindControl(_T("Com_EndPoint")));;//�յ�
	Com_StartingPoint->RemoveAll();
	Com_EndPoint->RemoveAll();


}
void CPopNetworkManage::RefreshTheInterface_tab2()
{
	vec_level2_1.clear();
	m_mapLevel2_1.clear();


	for (int i = 0; i < pNode_2_1_num; i++)
	{
		int childNum = pNode_2_1[i]->GetCountChild();
		for (int j = 0; j < childNum; j++)
		{
			pNode_2_1[i]->Remove(pNode_2_2[j]);
		}
	}
	memset(pNode_2_2, 0, sizeof(pNode_2_2));
	pNode_2_2_num = 0;


	int num = pNode_2_1_num;
	cout << num << endl;
	for (int i = 0; i < (num - 1); i++)
	{
		m_treeView_2->Remove(pNode_2_1[i + 1]);
		pNode_2_1_num -= 1;
	}
	cout << "1111111111" << endl;
}
void CPopNetworkManage::RefreshTheInterface_tab3()
{
	vec_level3_1.clear();
	m_mapLevel3_1.clear();


	for (int i = 0; i < pNode_3_1_num; i++)
	{
		int childNum = pNode_3_1[i]->GetCountChild();
		for (int j = 0; j < childNum; j++)
		{
			pNode_3_1[i]->Remove(pNode_3_2[j]);
		}
	}
	memset(pNode_3_2, 0, sizeof(pNode_3_2));
	pNode_3_2_num = 0;


	int num = pNode_3_1_num;
	cout << num << endl;
	for (int i = 0; i < (num - 1); i++)
	{
		m_treeView_3->Remove(pNode_3_1[i + 1]);
		pNode_3_1_num -= 1;
	}
	cout << "1111111111" << endl;
}
//��ɵ���fmu�İ� tab1
vector<double> CPopNetworkManage::AssemblePackageFMU_tab1(vector<string> vec_WX, vector<string> vec_DQZ, vector<string> vec_DMZD)
{
	vector<vector<double>> vec_WXValue;
	vector<vector<double>> vec_DQZValue;
	vector<vector<double>> vec_DMZDValue;

	vector<double> vec_AllDevice;
	for (int i = 0; i < 6; i++)//����ʼʱ��Ž�����
	{
		vec_AllDevice.push_back(intTime[i]);
	}
	vec_AllDevice.push_back(ts);//����ʱ�Ž�����
	vec_AllDevice.push_back(vec_WX.size()); //�����ǷŽ�����
	vec_AllDevice.push_back(vec_DQZ.size()); //������վ�Ž�����
	vec_AllDevice.push_back(vec_DMZD.size());//�������ն˷Ž�����


	//m_NetworkPackets.startPoint = startSeque;
	//m_NetworkPackets.endPoint = endSeque;
	int serialNumber = 0;//�豸��������
	for (int i = 0; i < vec_WX.size(); i++)//�����ǵ����ݷ������
	{
		for (int j = 0; j < vec_recSatelliteName.size(); j++)
		{
			vector<double> vec_WXInformation;//���������Ϣ
			if (vec_WX[i] == vec_recSatelliteName[j])//����ѡ�е������ڽڵ��е�λ��
			{
				serialNumber += 1;
				//vec_WXInformation.push_back(serialNumber);
				//vec_WXInformation.push_back(stringToNum<double>(m_RecvSatelliteMap[j]["����볤��"]));
				//vec_WXInformation.push_back(stringToNum<double>(m_RecvSatelliteMap[j]["���ƫ����"]));
				//vec_WXInformation.push_back(stringToNum<double>(m_RecvSatelliteMap[j]["������"]));
				//vec_WXInformation.push_back(stringToNum<double>(m_RecvSatelliteMap[j]["������ྭ"]));
				//vec_WXInformation.push_back(stringToNum<double>(m_RecvSatelliteMap[j]["���ص����"]));
				//vec_WXInformation.push_back(stringToNum<double>(m_RecvSatelliteMap[j]["ֱ�����"]));
				//vec_WXInformation.push_back(stringToNum<double>(m_RecvSatelliteMap[j]["EIRP"]));

				//vec_WXInformation.push_back(stringToNum<double>(m_RecvSatelliteMap[j]["SFD"]));
				//vec_WXInformation.push_back(stringToNum<double>(m_RecvSatelliteMap[j]["����Ƶ��"]));
				//vec_WXInformation.push_back(stringToNum<double>(m_RecvSatelliteMap[j]["�������"]));
				//vec_WXInformation.push_back(stringToNum<double>(m_RecvSatelliteMap[j]["GTֵ"]));
				//vec_WXInformation.push_back(stringToNum<double>(m_RecvSatelliteMap[j]["����Ƶ��"]));
				//vec_WXInformation.push_back(stringToNum<double>(m_RecvSatelliteMap[j]["���մ���"]));
				//vec_WXInformation.push_back(stringToNum<double>(m_RecvSatelliteMap[j]["���߿ھ�"]));


				//vec_WXInformation.push_back(stringToNum<double>(m_RecvSatelliteMap[j]["����Ч��"]));
				//vec_WXInformation.push_back(stringToNum<double>(m_RecvSatelliteMap[j]["ָ�����"]));
				//vec_WXInformation.push_back(stringToNum<double>(m_RecvSatelliteMap[j]["�������"]));


				vec_AllDevice.push_back(serialNumber);
				vec_AllDevice.push_back(stringToNum<double>(m_RecvSatelliteMap[j]["����볤��"]));
				vec_AllDevice.push_back(stringToNum<double>(m_RecvSatelliteMap[j]["���ƫ����"]));
				vec_AllDevice.push_back(stringToNum<double>(m_RecvSatelliteMap[j]["������"]));
				vec_AllDevice.push_back(stringToNum<double>(m_RecvSatelliteMap[j]["������ྭ"]));
				vec_AllDevice.push_back(stringToNum<double>(m_RecvSatelliteMap[j]["���ص����"]));
				vec_AllDevice.push_back(stringToNum<double>(m_RecvSatelliteMap[j]["ֱ�����"]));
				vec_AllDevice.push_back(stringToNum<double>(m_RecvSatelliteMap[j]["EIRP"]));

				vec_AllDevice.push_back(stringToNum<double>(m_RecvSatelliteMap[j]["SFD"]));
				vec_AllDevice.push_back(stringToNum<double>(m_RecvSatelliteMap[j]["����Ƶ��"]));
				vec_AllDevice.push_back(stringToNum<double>(m_RecvSatelliteMap[j]["�������"]));
				vec_AllDevice.push_back(stringToNum<double>(m_RecvSatelliteMap[j]["GTֵ"]));
				vec_AllDevice.push_back(stringToNum<double>(m_RecvSatelliteMap[j]["����Ƶ��"]));
				vec_AllDevice.push_back(stringToNum<double>(m_RecvSatelliteMap[j]["���մ���"]));
				vec_AllDevice.push_back(stringToNum<double>(m_RecvSatelliteMap[j]["���߿ھ�"]));


				vec_AllDevice.push_back(stringToNum<double>(m_RecvSatelliteMap[j]["����Ч��"]));
				vec_AllDevice.push_back(stringToNum<double>(m_RecvSatelliteMap[j]["ָ�����"]));
				vec_AllDevice.push_back(stringToNum<double>(m_RecvSatelliteMap[j]["�������"]));
			}
			
		}
	}

	for (int i = 0; i < vec_DQZ.size(); i++)//������վ�����ݷ������
	{
		for (int j = 0; j < vec_recEarthStationName.size(); j++)
		{
			vector<double> vec_DQZInformation;//���������Ϣ
			if (vec_DQZ[i] == vec_recEarthStationName[j])//����ѡ�е������ڽڵ��е�λ��
			{
				serialNumber += 1;
				vec_AllDevice.push_back(serialNumber);

				vec_AllDevice.push_back(stringToNum<double>(m_RecvEarthStationMap[j]["����"]));
				vec_AllDevice.push_back(stringToNum<double>(m_RecvEarthStationMap[j]["γ��"]));
				vec_AllDevice.push_back(stringToNum<double>(m_RecvEarthStationMap[j]["�߶�"]));
				vec_AllDevice.push_back(0);
				vec_AllDevice.push_back(0);
				vec_AllDevice.push_back(0);

				vec_AllDevice.push_back(stringToNum<double>(m_RecvEarthStationMap[j]["EIRP"]));
				vec_AllDevice.push_back(stringToNum<double>(m_RecvEarthStationMap[j]["����Ƶ��"]));
				vec_AllDevice.push_back(stringToNum<double>(m_RecvEarthStationMap[j]["�������"]));
				vec_AllDevice.push_back(stringToNum<double>(m_RecvEarthStationMap[j]["FEC"]));
				vec_AllDevice.push_back(stringToNum<double>(m_RecvEarthStationMap[j]["GTֵ"]));
				vec_AllDevice.push_back(stringToNum<double>(m_RecvEarthStationMap[j]["����Ƶ��"]));
				vec_AllDevice.push_back(stringToNum<double>(m_RecvEarthStationMap[j]["���մ���"]));


				vec_AllDevice.push_back(stringToNum<double>(m_RecvEarthStationMap[j]["���߿ھ�"]));
				vec_AllDevice.push_back(stringToNum<double>(m_RecvEarthStationMap[j]["����Ч��"]));
				vec_AllDevice.push_back(stringToNum<double>(m_RecvEarthStationMap[j]["ָ�����"]));
				vec_AllDevice.push_back(stringToNum<double>(m_RecvEarthStationMap[j]["�������"]));

				//vec_DQZInformation.push_back(stringToNum<double>(m_RecvEarthStationMap[j]["����"]));
				//vec_DQZInformation.push_back(stringToNum<double>(m_RecvEarthStationMap[j]["γ��"]));
				//vec_DQZInformation.push_back(stringToNum<double>(m_RecvEarthStationMap[j]["�߶�"]));
				//vec_DQZInformation.push_back(0);
				//vec_DQZInformation.push_back(0);
				//vec_DQZInformation.push_back(0);

				//vec_DQZInformation.push_back(stringToNum<double>(m_RecvEarthStationMap[j]["EIRP"]));
				//vec_DQZInformation.push_back(stringToNum<double>(m_RecvEarthStationMap[j]["����Ƶ��"]));
				//vec_DQZInformation.push_back(stringToNum<double>(m_RecvEarthStationMap[j]["�������"]));
				//vec_DQZInformation.push_back(stringToNum<double>(m_RecvEarthStationMap[j]["FEC"]));
				//vec_DQZInformation.push_back(stringToNum<double>(m_RecvEarthStationMap[j]["GTֵ"]));
				//vec_DQZInformation.push_back(stringToNum<double>(m_RecvEarthStationMap[j]["����Ƶ��"]));
				//vec_DQZInformation.push_back(stringToNum<double>(m_RecvEarthStationMap[j]["���մ���"]));


				//vec_DQZInformation.push_back(stringToNum<double>(m_RecvEarthStationMap[j]["���߿ھ�"]));
				//vec_DQZInformation.push_back(stringToNum<double>(m_RecvEarthStationMap[j]["����Ч��"]));
				//vec_DQZInformation.push_back(stringToNum<double>(m_RecvEarthStationMap[j]["ָ�����"]));
				//vec_DQZInformation.push_back(stringToNum<double>(m_RecvEarthStationMap[j]["�������"]));

				//vec_WXValue.push_back(vec_DQZInformation);
			}
			
		}
	}


	for (int i = 0; i < vec_DMZD.size(); i++)//������վ�����ݷ������
	{
		for (int j = 0; j < vec_recGroundTerminalName.size(); j++)
		{
			vector<double> vec_DMZDInformation;//���������Ϣ
			if (vec_DMZD[i] == vec_recGroundTerminalName[j])//����ѡ�е������ڽڵ��е�λ��
			{
				serialNumber += 1;
				//vec_DMZDInformation.push_back(serialNumber);

				//vec_DMZDInformation.push_back(stringToNum<double>(m_RecvGroundTerminalMap[j]["����"]));
				//vec_DMZDInformation.push_back(stringToNum<double>(m_RecvGroundTerminalMap[j]["γ��"]));
				//vec_DMZDInformation.push_back(stringToNum<double>(m_RecvGroundTerminalMap[j]["�߶�"]));
				//vec_DMZDInformation.push_back(0);
				//vec_DMZDInformation.push_back(0);
				//vec_DMZDInformation.push_back(0);

				//vec_DMZDInformation.push_back(stringToNum<double>(m_RecvGroundTerminalMap[j]["EIRP"]));
				//vec_DMZDInformation.push_back(stringToNum<double>(m_RecvGroundTerminalMap[j]["����Ƶ��"]));
				//vec_DMZDInformation.push_back(stringToNum<double>(m_RecvGroundTerminalMap[j]["�������"]));
				//vec_DMZDInformation.push_back(stringToNum<double>(m_RecvGroundTerminalMap[j]["FEC"]));
				//vec_DMZDInformation.push_back(stringToNum<double>(m_RecvGroundTerminalMap[j]["GTֵ"]));
				//vec_DMZDInformation.push_back(stringToNum<double>(m_RecvGroundTerminalMap[j]["����Ƶ��"]));
				//vec_DMZDInformation.push_back(stringToNum<double>(m_RecvGroundTerminalMap[j]["���մ���"]));


				//vec_DMZDInformation.push_back(stringToNum<double>(m_RecvGroundTerminalMap[j]["���߿ھ�"]));
				//vec_DMZDInformation.push_back(stringToNum<double>(m_RecvGroundTerminalMap[j]["����Ч��"]));
				//vec_DMZDInformation.push_back(stringToNum<double>(m_RecvGroundTerminalMap[j]["ָ�����"]));
				//vec_DMZDInformation.push_back(stringToNum<double>(m_RecvGroundTerminalMap[j]["�������"]));
				//vec_WXValue.push_back(vec_DMZDInformation);
				vec_AllDevice.push_back(serialNumber);
		
				vec_AllDevice.push_back(stringToNum<double>(m_RecvGroundTerminalMap[j]["����"]));
				vec_AllDevice.push_back(stringToNum<double>(m_RecvGroundTerminalMap[j]["γ��"]));
				vec_AllDevice.push_back(stringToNum<double>(m_RecvGroundTerminalMap[j]["�߶�"]));
				vec_AllDevice.push_back(0);
				vec_AllDevice.push_back(0);
				vec_AllDevice.push_back(0);

				vec_AllDevice.push_back(stringToNum<double>(m_RecvGroundTerminalMap[j]["EIRP"]));
				vec_AllDevice.push_back(stringToNum<double>(m_RecvGroundTerminalMap[j]["����Ƶ��"]));
				vec_AllDevice.push_back(stringToNum<double>(m_RecvGroundTerminalMap[j]["�������"]));
				vec_AllDevice.push_back(stringToNum<double>(m_RecvGroundTerminalMap[j]["FEC"]));
				vec_AllDevice.push_back(stringToNum<double>(m_RecvGroundTerminalMap[j]["GTֵ"]));
				vec_AllDevice.push_back(stringToNum<double>(m_RecvGroundTerminalMap[j]["����Ƶ��"]));
				vec_AllDevice.push_back(stringToNum<double>(m_RecvGroundTerminalMap[j]["���մ���"]));


				vec_AllDevice.push_back(stringToNum<double>(m_RecvGroundTerminalMap[j]["���߿ھ�"]));
				vec_AllDevice.push_back(stringToNum<double>(m_RecvGroundTerminalMap[j]["����Ч��"]));
				vec_AllDevice.push_back(stringToNum<double>(m_RecvGroundTerminalMap[j]["ָ�����"]));
				vec_AllDevice.push_back(stringToNum<double>(m_RecvGroundTerminalMap[j]["�������"]));
			}
			
		}
	}
	int NumComplete = (100 - vec_WX.size() - vec_DQZ.size() - vec_DMZD.size()) * 18;
	for (int i = 0; i < NumComplete; i++)
	{
		vec_AllDevice.push_back(0);
	}
	//vec_SequenceDisplayTab1�ǰ��հ������������ٵ���վ�ٵ����ն˵�˳������
	CControlUI* pControl_Start = static_cast<CControlUI*>(m_pm.FindControl(_T("Com_StartingPoint")));
	wstring wstr_start = pControl_Start->GetText();
	string str_startText = wstringToString(wstr_start);
	CControlUI* pControl_End = static_cast<CControlUI*>(m_pm.FindControl(_T("Com_EndPoint")));
	wstring wstr_end = pControl_End->GetText();
	string str_endText = wstringToString(wstr_end);
	cout << str_startText << str_endText << endl;

	double startSeque;		//������
	double endSeque;		//�յ����
	for (int i = 0; i < vec_SequenceDisplayTab1.size(); i++)
	{
		if (vec_SequenceDisplayTab1[i] == str_startText)
		{
			startSeque = i + 1;

		}
		if (vec_SequenceDisplayTab1[i] == str_endText)
		{
			endSeque = i + 1;
		}
	}
	vec_AllDevice.push_back(startSeque);//��������ŷŽ�����
	vec_AllDevice.push_back(endSeque);//���յ����ŷŽ�����

	return vec_AllDevice;
}
//��ɵ���fmu�İ� tab2
vector<double> CPopNetworkManage::AssemblePackageFMU_tab2(vector<string> vec_WX, vector<string> vec_DQZ, vector<string> vec_DMZD)
{
	vector<vector<double>> vec_WXValue;
	vector<vector<double>> vec_DQZValue;
	vector<vector<double>> vec_DMZDValue;

	vector<double> vec_AllDevice;
	for (int i = 0; i < 6; i++)//����ʼʱ��Ž�����
	{
		vec_AllDevice.push_back(intTime[i]);
	}
	vec_AllDevice.push_back(ts);//����ʱ�Ž�����
	vec_AllDevice.push_back(vec_WX.size()); //�����ǷŽ�����
	vec_AllDevice.push_back(vec_DQZ.size()); //������վ�Ž�����
	vec_AllDevice.push_back(vec_DMZD.size());//�������ն˷Ž�����


	//m_NetworkPackets.startPoint = startSeque;
	//m_NetworkPackets.endPoint = endSeque;
	int serialNumber = 0;//�豸��������
	for (int i = 0; i < vec_WX.size(); i++)//�����ǵ����ݷ������
	{
		for (int j = 0; j < vec_recSatelliteName.size(); j++)
		{
			vector<double> vec_WXInformation;//���������Ϣ
			if (vec_WX[i] == vec_recSatelliteName[j])//����ѡ�е������ڽڵ��е�λ��
			{
				serialNumber += 1;

				vec_AllDevice.push_back(serialNumber);
				vec_AllDevice.push_back(stringToNum<double>(m_RecvSatelliteMap[j]["����볤��"]));
				vec_AllDevice.push_back(stringToNum<double>(m_RecvSatelliteMap[j]["���ƫ����"]));
				vec_AllDevice.push_back(stringToNum<double>(m_RecvSatelliteMap[j]["������"]));
				vec_AllDevice.push_back(stringToNum<double>(m_RecvSatelliteMap[j]["������ྭ"]));
				vec_AllDevice.push_back(stringToNum<double>(m_RecvSatelliteMap[j]["���ص����"]));
				vec_AllDevice.push_back(stringToNum<double>(m_RecvSatelliteMap[j]["ֱ�����"]));
				vec_AllDevice.push_back(stringToNum<double>(m_RecvSatelliteMap[j]["EIRP"]));

				vec_AllDevice.push_back(stringToNum<double>(m_RecvSatelliteMap[j]["SFD"]));
				vec_AllDevice.push_back(stringToNum<double>(m_RecvSatelliteMap[j]["����Ƶ��"]));
				vec_AllDevice.push_back(stringToNum<double>(m_RecvSatelliteMap[j]["�������"]));
				vec_AllDevice.push_back(stringToNum<double>(m_RecvSatelliteMap[j]["GTֵ"]));
				vec_AllDevice.push_back(stringToNum<double>(m_RecvSatelliteMap[j]["����Ƶ��"]));
				vec_AllDevice.push_back(stringToNum<double>(m_RecvSatelliteMap[j]["���մ���"]));
				vec_AllDevice.push_back(stringToNum<double>(m_RecvSatelliteMap[j]["���߿ھ�"]));


				vec_AllDevice.push_back(stringToNum<double>(m_RecvSatelliteMap[j]["����Ч��"]));
				vec_AllDevice.push_back(stringToNum<double>(m_RecvSatelliteMap[j]["ָ�����"]));
				vec_AllDevice.push_back(stringToNum<double>(m_RecvSatelliteMap[j]["�������"]));
			}

		}
	}

	for (int i = 0; i < vec_DQZ.size(); i++)//������վ�����ݷ������
	{
		for (int j = 0; j < vec_recEarthStationName.size(); j++)
		{
			vector<double> vec_DQZInformation;//���������Ϣ
			if (vec_DQZ[i] == vec_recEarthStationName[j])//����ѡ�е������ڽڵ��е�λ��
			{
				serialNumber += 1;
				vec_AllDevice.push_back(serialNumber);

				vec_AllDevice.push_back(stringToNum<double>(m_RecvEarthStationMap[j]["����"]));
				vec_AllDevice.push_back(stringToNum<double>(m_RecvEarthStationMap[j]["γ��"]));
				vec_AllDevice.push_back(stringToNum<double>(m_RecvEarthStationMap[j]["�߶�"]));
				vec_AllDevice.push_back(0);
				vec_AllDevice.push_back(0);
				vec_AllDevice.push_back(0);

				vec_AllDevice.push_back(stringToNum<double>(m_RecvEarthStationMap[j]["EIRP"]));
				vec_AllDevice.push_back(stringToNum<double>(m_RecvEarthStationMap[j]["����Ƶ��"]));
				vec_AllDevice.push_back(stringToNum<double>(m_RecvEarthStationMap[j]["�������"]));
				vec_AllDevice.push_back(stringToNum<double>(m_RecvEarthStationMap[j]["FEC"]));
				vec_AllDevice.push_back(stringToNum<double>(m_RecvEarthStationMap[j]["GTֵ"]));
				vec_AllDevice.push_back(stringToNum<double>(m_RecvEarthStationMap[j]["����Ƶ��"]));
				vec_AllDevice.push_back(stringToNum<double>(m_RecvEarthStationMap[j]["���մ���"]));


				vec_AllDevice.push_back(stringToNum<double>(m_RecvEarthStationMap[j]["���߿ھ�"]));
				vec_AllDevice.push_back(stringToNum<double>(m_RecvEarthStationMap[j]["����Ч��"]));
				vec_AllDevice.push_back(stringToNum<double>(m_RecvEarthStationMap[j]["ָ�����"]));
				vec_AllDevice.push_back(stringToNum<double>(m_RecvEarthStationMap[j]["�������"]));

			}

		}
	}


	for (int i = 0; i < vec_DMZD.size(); i++)//������վ�����ݷ������
	{
		for (int j = 0; j < vec_recGroundTerminalName.size(); j++)
		{
			vector<double> vec_DMZDInformation;//���������Ϣ
			if (vec_DMZD[i] == vec_recGroundTerminalName[j])//����ѡ�е������ڽڵ��е�λ��
			{
				serialNumber += 1;

				vec_AllDevice.push_back(serialNumber);

				vec_AllDevice.push_back(stringToNum<double>(m_RecvGroundTerminalMap[j]["����"]));
				vec_AllDevice.push_back(stringToNum<double>(m_RecvGroundTerminalMap[j]["γ��"]));
				vec_AllDevice.push_back(stringToNum<double>(m_RecvGroundTerminalMap[j]["�߶�"]));
				vec_AllDevice.push_back(0);
				vec_AllDevice.push_back(0);
				vec_AllDevice.push_back(0);

				vec_AllDevice.push_back(stringToNum<double>(m_RecvGroundTerminalMap[j]["EIRP"]));
				vec_AllDevice.push_back(stringToNum<double>(m_RecvGroundTerminalMap[j]["����Ƶ��"]));
				vec_AllDevice.push_back(stringToNum<double>(m_RecvGroundTerminalMap[j]["�������"]));
				vec_AllDevice.push_back(stringToNum<double>(m_RecvGroundTerminalMap[j]["FEC"]));
				vec_AllDevice.push_back(stringToNum<double>(m_RecvGroundTerminalMap[j]["GTֵ"]));
				vec_AllDevice.push_back(stringToNum<double>(m_RecvGroundTerminalMap[j]["����Ƶ��"]));
				vec_AllDevice.push_back(stringToNum<double>(m_RecvGroundTerminalMap[j]["���մ���"]));


				vec_AllDevice.push_back(stringToNum<double>(m_RecvGroundTerminalMap[j]["���߿ھ�"]));
				vec_AllDevice.push_back(stringToNum<double>(m_RecvGroundTerminalMap[j]["����Ч��"]));
				vec_AllDevice.push_back(stringToNum<double>(m_RecvGroundTerminalMap[j]["ָ�����"]));
				vec_AllDevice.push_back(stringToNum<double>(m_RecvGroundTerminalMap[j]["�������"]));
			}

		}
	}
	int NumComplete = (100 - vec_WX.size() - vec_DQZ.size() - vec_DMZD.size()) * 18;
	for (int i = 0; i < NumComplete; i++)
	{
		vec_AllDevice.push_back(0);
	}
	//vec_SequenceDisplayTab2�ǰ��հ������������ٵ���վ�ٵ����ն˵�˳������
	CControlUI* pControl_Start = static_cast<CControlUI*>(m_pm.FindControl(_T("Com_StartingPoint_tab2")));
	wstring wstr_start = pControl_Start->GetText();
	string str_startText = wstringToString(wstr_start);
	CControlUI* pControl_End = static_cast<CControlUI*>(m_pm.FindControl(_T("Com_EndPoint_tab2")));
	wstring wstr_end = pControl_End->GetText();
	string str_endText = wstringToString(wstr_end);
	cout << str_startText << str_endText << endl;

	double startSeque;		//������
	double endSeque;		//�յ����
	for (int i = 0; i < vec_SequenceDisplayTab2.size(); i++)
	{
		if (vec_SequenceDisplayTab2[i] == str_startText)
		{
			startSeque = i + 1;

		}
		if (vec_SequenceDisplayTab2[i] == str_endText)
		{
			endSeque = i + 1;
		}
	}
	vec_AllDevice.push_back(startSeque);//��������ŷŽ�����
	vec_AllDevice.push_back(endSeque);//���յ����ŷŽ�����

	return vec_AllDevice;
}

//��ɵ���fmu�İ� tab3
vector<double> CPopNetworkManage::AssemblePackageFMU_tab3(vector<string> vec_WX, vector<string> vec_DQZ, vector<string> vec_DMZD)
{
	vector<vector<double>> vec_WXValue;
	vector<vector<double>> vec_DQZValue;
	vector<vector<double>> vec_DMZDValue;

	vector<double> vec_AllDevice;
	for (int i = 0; i < 6; i++)//����ʼʱ��Ž�����
	{
		vec_AllDevice.push_back(intTime[i]);
	}
	vec_AllDevice.push_back(ts);//����ʱ�Ž�����
	vec_AllDevice.push_back(vec_WX.size()); //�����ǷŽ�����
	vec_AllDevice.push_back(vec_DQZ.size()); //������վ�Ž�����
	vec_AllDevice.push_back(vec_DMZD.size());//�������ն˷Ž�����


	//m_NetworkPackets.startPoint = startSeque;
	//m_NetworkPackets.endPoint = endSeque;
	int serialNumber = 0;//�豸��������
	for (int i = 0; i < vec_WX.size(); i++)//�����ǵ����ݷ������
	{
		for (int j = 0; j < vec_recSatelliteName.size(); j++)
		{
			vector<double> vec_WXInformation;//���������Ϣ
			if (vec_WX[i] == vec_recSatelliteName[j])//����ѡ�е������ڽڵ��е�λ��
			{
				serialNumber += 1;

				vec_AllDevice.push_back(serialNumber);
				vec_AllDevice.push_back(stringToNum<double>(m_RecvSatelliteMap[j]["����볤��"]));
				vec_AllDevice.push_back(stringToNum<double>(m_RecvSatelliteMap[j]["���ƫ����"]));
				vec_AllDevice.push_back(stringToNum<double>(m_RecvSatelliteMap[j]["������"]));
				vec_AllDevice.push_back(stringToNum<double>(m_RecvSatelliteMap[j]["������ྭ"]));
				vec_AllDevice.push_back(stringToNum<double>(m_RecvSatelliteMap[j]["���ص����"]));
				vec_AllDevice.push_back(stringToNum<double>(m_RecvSatelliteMap[j]["ֱ�����"]));
				vec_AllDevice.push_back(stringToNum<double>(m_RecvSatelliteMap[j]["EIRP"]));

				vec_AllDevice.push_back(stringToNum<double>(m_RecvSatelliteMap[j]["SFD"]));
				vec_AllDevice.push_back(stringToNum<double>(m_RecvSatelliteMap[j]["����Ƶ��"]));
				vec_AllDevice.push_back(stringToNum<double>(m_RecvSatelliteMap[j]["�������"]));
				vec_AllDevice.push_back(stringToNum<double>(m_RecvSatelliteMap[j]["GTֵ"]));
				vec_AllDevice.push_back(stringToNum<double>(m_RecvSatelliteMap[j]["����Ƶ��"]));
				vec_AllDevice.push_back(stringToNum<double>(m_RecvSatelliteMap[j]["���մ���"]));
				vec_AllDevice.push_back(stringToNum<double>(m_RecvSatelliteMap[j]["���߿ھ�"]));


				vec_AllDevice.push_back(stringToNum<double>(m_RecvSatelliteMap[j]["����Ч��"]));
				vec_AllDevice.push_back(stringToNum<double>(m_RecvSatelliteMap[j]["ָ�����"]));
				vec_AllDevice.push_back(stringToNum<double>(m_RecvSatelliteMap[j]["�������"]));
			}

		}
	}

	for (int i = 0; i < vec_DQZ.size(); i++)//������վ�����ݷ������
	{
		for (int j = 0; j < vec_recEarthStationName.size(); j++)
		{
			vector<double> vec_DQZInformation;//���������Ϣ
			if (vec_DQZ[i] == vec_recEarthStationName[j])//����ѡ�е������ڽڵ��е�λ��
			{
				serialNumber += 1;
				vec_AllDevice.push_back(serialNumber);

				vec_AllDevice.push_back(stringToNum<double>(m_RecvEarthStationMap[j]["����"]));
				vec_AllDevice.push_back(stringToNum<double>(m_RecvEarthStationMap[j]["γ��"]));
				vec_AllDevice.push_back(stringToNum<double>(m_RecvEarthStationMap[j]["�߶�"]));
				vec_AllDevice.push_back(0);
				vec_AllDevice.push_back(0);
				vec_AllDevice.push_back(0);

				vec_AllDevice.push_back(stringToNum<double>(m_RecvEarthStationMap[j]["EIRP"]));
				vec_AllDevice.push_back(stringToNum<double>(m_RecvEarthStationMap[j]["����Ƶ��"]));
				vec_AllDevice.push_back(stringToNum<double>(m_RecvEarthStationMap[j]["�������"]));
				vec_AllDevice.push_back(stringToNum<double>(m_RecvEarthStationMap[j]["FEC"]));
				vec_AllDevice.push_back(stringToNum<double>(m_RecvEarthStationMap[j]["GTֵ"]));
				vec_AllDevice.push_back(stringToNum<double>(m_RecvEarthStationMap[j]["����Ƶ��"]));
				vec_AllDevice.push_back(stringToNum<double>(m_RecvEarthStationMap[j]["���մ���"]));


				vec_AllDevice.push_back(stringToNum<double>(m_RecvEarthStationMap[j]["���߿ھ�"]));
				vec_AllDevice.push_back(stringToNum<double>(m_RecvEarthStationMap[j]["����Ч��"]));
				vec_AllDevice.push_back(stringToNum<double>(m_RecvEarthStationMap[j]["ָ�����"]));
				vec_AllDevice.push_back(stringToNum<double>(m_RecvEarthStationMap[j]["�������"]));

			}

		}
	}


	for (int i = 0; i < vec_DMZD.size(); i++)//������վ�����ݷ������
	{
		for (int j = 0; j < vec_recGroundTerminalName.size(); j++)
		{
			vector<double> vec_DMZDInformation;//���������Ϣ
			if (vec_DMZD[i] == vec_recGroundTerminalName[j])//����ѡ�е������ڽڵ��е�λ��
			{
				serialNumber += 1;

				vec_AllDevice.push_back(serialNumber);

				vec_AllDevice.push_back(stringToNum<double>(m_RecvGroundTerminalMap[j]["����"]));
				vec_AllDevice.push_back(stringToNum<double>(m_RecvGroundTerminalMap[j]["γ��"]));
				vec_AllDevice.push_back(stringToNum<double>(m_RecvGroundTerminalMap[j]["�߶�"]));
				vec_AllDevice.push_back(0);
				vec_AllDevice.push_back(0);
				vec_AllDevice.push_back(0);

				vec_AllDevice.push_back(stringToNum<double>(m_RecvGroundTerminalMap[j]["EIRP"]));
				vec_AllDevice.push_back(stringToNum<double>(m_RecvGroundTerminalMap[j]["����Ƶ��"]));
				vec_AllDevice.push_back(stringToNum<double>(m_RecvGroundTerminalMap[j]["�������"]));
				vec_AllDevice.push_back(stringToNum<double>(m_RecvGroundTerminalMap[j]["FEC"]));
				vec_AllDevice.push_back(stringToNum<double>(m_RecvGroundTerminalMap[j]["GTֵ"]));
				vec_AllDevice.push_back(stringToNum<double>(m_RecvGroundTerminalMap[j]["����Ƶ��"]));
				vec_AllDevice.push_back(stringToNum<double>(m_RecvGroundTerminalMap[j]["���մ���"]));


				vec_AllDevice.push_back(stringToNum<double>(m_RecvGroundTerminalMap[j]["���߿ھ�"]));
				vec_AllDevice.push_back(stringToNum<double>(m_RecvGroundTerminalMap[j]["����Ч��"]));
				vec_AllDevice.push_back(stringToNum<double>(m_RecvGroundTerminalMap[j]["ָ�����"]));
				vec_AllDevice.push_back(stringToNum<double>(m_RecvGroundTerminalMap[j]["�������"]));
			}

		}
	}
	int NumComplete = (100 - vec_WX.size() - vec_DQZ.size() - vec_DMZD.size()) * 18;
	for (int i = 0; i < NumComplete; i++)
	{
		vec_AllDevice.push_back(0);
	}
	//vec_SequenceDisplayTab2�ǰ��հ������������ٵ���վ�ٵ����ն˵�˳������
	CControlUI* pControl_Start = static_cast<CControlUI*>(m_pm.FindControl(_T("Com_StartingPoint_tab3")));
	wstring wstr_start = pControl_Start->GetText();
	string str_startText = wstringToString(wstr_start);
	CControlUI* pControl_End = static_cast<CControlUI*>(m_pm.FindControl(_T("Com_EndPoint_tab3")));
	wstring wstr_end = pControl_End->GetText();
	string str_endText = wstringToString(wstr_end);
	cout << str_startText << str_endText << endl;

	double startSeque;		//������
	double endSeque;		//�յ����
	for (int i = 0; i < vec_SequenceDisplayTab3.size(); i++)
	{
		if (vec_SequenceDisplayTab3[i] == str_startText)
		{
			startSeque = i + 1;

		}
		if (vec_SequenceDisplayTab3[i] == str_endText)
		{
			endSeque = i + 1;
		}
	}
	vec_AllDevice.push_back(startSeque);//��������ŷŽ�����
	vec_AllDevice.push_back(endSeque);//���յ����ŷŽ�����

	return vec_AllDevice;
}
