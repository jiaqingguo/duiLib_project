#include "StdAfx.h"
#include <exdisp.h>
#include <comdef.h>
#include <commdlg.h> 
#include <process.h>
//#include <Event.h>
#include <stdlib.h>
#include <crtdbg.h>
#include <math.h>
#include <queue>
#include <set>
#include "ControlEx.h"
#include "ControlEx1.h"
#include "SearchWnd.h"
#include "resource.h"
//#include "Chart.h"
//#include "WndUI.h"
#include "SplashWnd.h"
#include "DecodeDll.h"
#include "UtilTool.h"
#include "PopWnd.h"
//#include "MysqlAdmin.h"
//#include "Csqlpool.h"
#include <string> 
#include "AddTest.h"
//#include "udp1.h"

#include "Helper.h"
extern vector<string> PreplyData;
extern vector<string> PreplyDataTime;
extern int FirstTab;
extern int data_save_en;
extern bool data_save;
extern CDuiString ZTest_Name;
#define START_YEAR (2006)
#define SECOND_DAY    (86400)    //60*60*24
#define SECOND_HOUR    (3600)    //60*60
#define SECOND_MIN    (60)    //60
//extern UDPAdmin * Udpadmin;
extern DynamicOutData m_dynamic;
const unsigned short int mon_yday[][13] =
{
    /* Normal years.  */
    { 0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365 },
    /* Leap years.  */
    { 0, 31, 60, 91, 121, 152, 182, 213, 244, 274, 305, 335, 366 }
};

class CGameBoxFrame : public WindowImplBase, public CWebBrowserEventHandler
{
public:
	CGameBoxFrame() {

	}

	CDuiString GetSkinFile()
	{
		return _T("skin.xml");
	}
	
	CControlUI* CreateControl(LPCTSTR pstrClass)
	{
		if( _tcsicmp(pstrClass, _T("GameList")) == 0 ){ 
			return new CGameListUI;
		}
		else if( _tcsicmp(pstrClass, _T("GameItem")) == 0 ){
			return new CGameItemUI;
		}
		else if( _tcsicmp(pstrClass, _T("ShortCut")) == 0 ){
			return new CShortCutUI;
		}
		else if( _tcsicmp(pstrClass, _T("LabelMutiline")) == 0 ){
			return new CLabelMutilineUI;
		}
		//else if( _tcsicmp(pstrClass, _T("Wnd")) == 0 ){


		////	CWndUI  *ppUI  = new CWndUI;
		////	CChartWnd * m_ChartWnd=new CChartWnd();
		////	ppUI->SetChartWnd(m_ChartWnd);
		////	HWND hWnd  = CreateWindow(_T("Button"), _T("win32"), WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 0, 0, 0, 0, m_pm.GetPaintWindow(), NULL, NULL, NULL);
		////	ppUI->Attach(hWnd);  
		////	m_ChartWnd->Attach(hWnd);

		////	/*CChart * chart = m_ChartWnd->GetChart();*/
		///////*	chart->ClrPlotData();*/
		////	//chartvector.push_back(m_ChartWnd);

		////	CChart * chart = m_ChartWnd->GetChart();
		////	double pX[100],pY[100];

		////	for(int i=0;i<100;i++){
		////		pX[i]=0;
		////		pY[i]=0;

		////	}
		////	chart->AddCurve(pX, pY, 100);

		//	//string strTime="20151017113000";
		//	//TCHAR *tc = (TCHAR *)(&strTime);

		//	//tc=char2TCAHR(strTime.c_str());
		//	//chart->AddCurve();
		//	//double t1, t2;
		//	//t1 = StringToTime(tc);
		//	//t2 = StringToTime(_T("20151017123512"));
		//	//chart->AddPoint2D(t1,3);
		//	//chart->AddPoint2D(t2,6);
		//	//chart->SetXAutoRange(false);
		//	//chart->SetXRange(t1,t2);


		//	//chart->SetAxisToTime(true, 1);
		//	//chart->SetAxisTimeFormat(_T("%H:%M:%S"), 1);
		//	//chart->SetExactXRange(true);
		//	/*for(int i=100;i<120;i++){
		//	pX[i-100]=i;
		//	pY[i-100]=0;
		//	}

		//	chart->AddCurve(pX, pY, 100);*/





		//		/*chart->SetMarkerShow(true,0);
		//		chart->SetMarkerType(2,0);
		//		chart->SetMarkerFreq(1,0);
		//		chart->SetDataValueShow(true,0);
		//		chart->SetDataValueType(3,0);
		//		chart->SetDataValueFreq(1,0);
		//		chart->SetDataValueFontSize(3,0);*/
		//	
		//	return ppUI;
		//}
		return NULL;
	}

public:

	static unsigned __stdcall ThreadDATAJX(void* arglist);

	BOOL StartThread();

	void InitWindow()
	{

		//high1_chart->SetVisible(false);隐藏曲线

		// 搜索窗口
		m_pSearchWnd  = new CSearchWnd(m_pm.GetRoot());
		m_pSearchWnd->Create(m_hWnd,  _T("searchwnd"), WS_POPUP, WS_EX_TOOLWINDOW | WS_EX_NOACTIVATE);
		::SetWindowPos(m_pSearchWnd->GetHWND(), NULL, 0,0,1,1, SWP_NOACTIVATE);
		m_pSearchWnd->ShowWindow(true);

		SendMessage(WM_SYSCOMMAND, SC_MAXIMIZE, 0);
	
		CListUI* pList3 = static_cast<CListUI*>(m_pm.FindControl(_T("listview3-errdata")));
		/*CListHeaderUI* pList66 = static_cast<CListHeaderUI*>(m_pm.FindControl(_T("listview3-errdata")));
		pList66->SetText(1, _T("p6"));*/
		for(int i = 1; i < 20; i++)
		{
			CListTextElementUI* pItem3  = new CListTextElementUI();
			pItem3->SetFixedHeight(30);
			pList3->Add(pItem3);
			CDuiString html_text;
			html_text.Format(_T("%d"), i);
			pItem3->SetText(0, html_text);
			pItem3->SetText(1, _T("p6"));
			pItem3->SetText(2, _T("通讯状态"));
			pItem3->SetText(3, _T("xx-"));
			pItem3->SetText(4, _T("01"));
			pItem3->SetText(5, _T("通讯故障"));
			pItem3->SetText(6, _T("2019-08-12 16:08:3"));

		}
		m_pCloseBtn = static_cast<CButtonUI*>(m_pm.FindControl(_T("closebtn")));
		precious1_status=(CLabelUI *)static_cast<CLabelUI* >(m_pm.FindControl(L"highpreciouschanel1"));
		pTabDatabase = static_cast<CTabLayoutUI*>(m_pm.FindControl(_T("tab_database")));	
		C7syAdL=static_cast<CWndUI* >(m_pm.FindControl(L"c7-sya-dl"));
		pTabSwitch	 = static_cast<CTabLayoutUI*>(m_pm.FindControl(_T("tab_switch")));
		AddCombo();
		StartThread();
		InitDecodeLib();	
		InitDateTime(L"telelayout");
	}
	TCHAR* NameStringTChar(string name)
	{
		int j=0;TCHAR* chara;
	
		string::iterator it;
		for(it = name.begin();j<14; it++)
		{
			if (*it == '-'||*it == ':'||*it == ' ')
			{
				name.erase(it);          
			}
			j++;
		}
			chara=char2TCAHR(name.c_str());
			return chara;
	}
	TCHAR* char2TCAHR( const char* str )
	{

		int size = MultiByteToWideChar(CP_ACP, 0, str, -1, NULL, 0);

		TCHAR* retStr = new TCHAR[size * sizeof(TCHAR)];

		MultiByteToWideChar(CP_ACP, 0, str, -1, retStr, size);

		return retStr;

	}
	void OnSearchEditChanged()
	{
		CEditUI* pEdit = static_cast<CEditUI*>(m_pm.FindControl(_T("search_edit")));
		RECT rcEdit = pEdit->GetPos();
		POINT ptPos = {rcEdit.left, rcEdit.bottom};
		::ClientToScreen(m_hWnd, &ptPos);
		SetWindowPos(m_pSearchWnd->GetHWND(), NULL, ptPos.x, ptPos.y, rcEdit.right - rcEdit.left, 200, SWP_NOACTIVATE);
		if(!::IsWindowVisible(m_pSearchWnd->GetHWND()))
			m_pSearchWnd->ShowWindow(true, false);
		CDuiString sText = pEdit->GetText();

		m_pSearchWnd->RemoveAll();

		for(int i = 0; i<10; i++)
			m_pSearchWnd->AddItem(sText);
	}

	void OnLClick(CControlUI *pControl)
	{
		CDuiString sName = pControl->GetName();

		if(sName.CompareNoCase(_T("UPDataLY")) == 0)
		{
		    vector<wstring>szLJNames;
					
	       //读取所选的数据文件（允许多选），将文件名放在szLJNames中返回
			szLJNames=UtilTool::GetOpenFilePaths();
			
			if (szLJNames.size()==0)
			{
				return;
			}
	

			/*	cPopswnd->PSatelliteNum=SatelliteNum;
			cPopswnd->Ptempdata=tempdata;
			cPopswnd->Create(m_hWnd, NULL, WS_POPUP | WS_VISIBLE, WS_EX_TOOLWINDOW , 0, 0, 800, 572);
			cPopswnd->CenterWindow();
			cPopswnd->ShowModal();*/

	
		   
		}else if (sName.CompareNoCase(_T("BackHome"))==0)
		{
			CSplashWnd* pPopWnd = new CSplashWnd();
			pPopWnd->Create(m_hWnd, NULL, WS_POPUP | WS_VISIBLE, WS_EX_TOOLWINDOW , 0, 0, 800, 572);
			pPopWnd->CenterWindow();
				
		}else if (sName.CompareNoCase(_T("start_bc_btn"))==0)
		{
			
		    ZLState=static_cast<CButtonUI*>(m_pm.FindControl(_T("start_bc_btn")));
			
			
			
		}else if (sName.CompareNoCase(_T("search_tele_parm_btn"))==0)
		{

			CListUI * pCListUI=(CListUI *)m_pm.FindControl(L"telelist");
			InitList( pCListUI,resultdata.size());

			for (int i=0;i<resultdata.size();i++)
			{			
				CListTextElementUI* pItem =(CListTextElementUI*)pCListUI->GetItemAt(i);
				wstring text_string;
				UtilTool::setWstring(text_string,resultdata[i][0].c_str());
				pItem->SetText(0,text_string.c_str());
				UtilTool::setWstring(text_string,resultdata[i][1].c_str());
				pItem->SetText(1,text_string.c_str());
				UtilTool::setWstring(text_string,resultdata[i][2].c_str());
				pItem->SetText(2,text_string.c_str());
			}

		}
		else if (sName.CompareNoCase(_T("Drawcurve"))==0)
		{

			//double start_x,end_x;double Curve=0.0;TCHAR*ResultData;	double t1, t2;double XKF,XJF;
			//string limit_string=GetLimitString(L"Curve1Time",pose_string);
			//string parm_name_string1,parm_Name;
			//UtilTool::setString(parm_name_string1,(m_pm.FindControl(L"DataSelect1")->GetText()).GetData());
			//parm_Name=parm_name_string1;
			//parm_name_string1=parm_map_D[parm_name_string1];
			//if(parm_name_string1=="")
			//{
			//	return;
			//}
			//if (parm_map.find(parm_name_string1)==parm_map.end())
			//{
			//	return;
			//}

			//CControlUI * parmctrl=m_pm.FindControl(_T("TEXTDATA"));
			//wstring wstr= parmctrl->GetText();
			//string warn;
			//UtilTool::setString(warn,wstr.c_str());

			//resultdataCC=mp.gettemdata(parm_name_string1,warn+parm_map[parm_name_string1],limit_string,pose_string);

			//if (resultdataCC.size()==0)
			//{
			//	return;
			//}
			//parmctrl=m_pm.FindControl(_T("chare_index"));
			//wstr= parmctrl->GetText();
			//UtilTool::setString(warn,wstr.c_str());
			//int intStr = atoi(warn.c_str());

			//p_select_combox = static_cast<CComboBoxUI*>(m_pm.FindControl(_T("chare_index")));
			//int ide=p_select_combox->GetCurSel();
			//p_select_combox->RemoveAt(ide);//在上面删除

			//p_select_combox = static_cast<CComboBoxUI*>(m_pm.FindControl(_T("chare_indexDl")));
			//parameters[0]=warn;		
			//wstring temp; 
			//CListLabelElementUI * pLabel= new CListLabelElementUI();
			//UtilTool::setWstring(temp,parameters[0].c_str());
			//pLabel->SetText(temp.c_str());
			//pLabel->SetBkColor(0xff0000);
			//if (p_select_combox)
			//	p_select_combox->AddAt(pLabel,0);
			//p_select_combox->SelectItem(0);
			//CEditUI  * CCDHDATA=static_cast<CEditUI*>(m_pm.FindControl(_T("chare_NameDelect")));
			//wstring dataC=m_pm.FindControl(L"DataSelect1")->GetText().GetData();
			//CCDHDATA->SetText(dataC.c_str());

			//chartv_sele[intStr]=parm_Name;

			//CListUI * pCListUI=(CListUI *)m_pm.FindControl(L"listview2-linedata");
			//int RedataSize=resultdataCC.size();
			//InitListSelect( pCListUI,RedataSize,intStr);
			//CChart * temps=((CChartWnd *)(Curve1_chart->GetChartWnd()))->GetChart();
			//temps->ClrSingleData(intStr-1);
			//temps->AddCurve();
			//temps->SetEnableToolTip(true);
			//temps->SetToolTipType(2);
			//temps->SetUseLegend(true);
			//temps->SetAxisToTime(true, 1);
			//temps->SetAxisTimeFormat(_T("%H:%M:%S"), 1);
			//temps->SetXAutoRange(false);
			//temps->SetExactXRange(true);
			//wstring Wparm_Name;
			//UtilTool::setWstring(Wparm_Name,parm_Name.c_str());
			//temps->SetDataTitle(Wparm_Name.c_str(), intStr-1);

			//for (int i=0;i<resultdataCC.size();i++)
			//{			
			//	CListTextElementUI* pItem =(CListTextElementUI*)pCListUI->GetItemAt(i);
			//	wstring text_string;
			//	UtilTool::setWstring(text_string,resultdataCC[i][1].c_str());
			//	pItem->SetText(0,text_string.c_str());
			//	UtilTool::setWstring(text_string,resultdataCC[i][2].c_str());
			//	pItem->SetText(intStr,text_string.c_str());
			//	start_x=_ttof(text_string.c_str());

			//	ResultData=NameStringTChar(resultdataCC[i][1]);
			//
			//	t1 = StringToTime(ResultData);
			//	if(i==0)
			//	{
			//		XKF=t1;
			//	}
			//	if (i==resultdataCC.size()-1)
			//	{
			//		XJF=t1;
			//	}

			//	temps->AddPoint2D(t1, start_x,intStr-1);

			//	Curve+=1;
			//	RECT rc;
			//	GetClientRect(((CChartWnd *)(Curve1_chart->GetChartWnd()))->GetHWnd(),&rc);
			//	InvalidateRect(((CChartWnd *)(Curve1_chart->GetChartWnd()))->GetHWnd(),&rc,true);

			//}
			//temps->SetXRange(XKF,XJF);
			//Curve=0;

		}
		else if (sName.CompareNoCase(_T("QchareDelect"))==0)
		{
		/*	CChart * temps=((CChartWnd *)(Curve1_chart->GetChartWnd()))->GetChart();
			temps->ClrPlotData();
			CListUI * pCListUI=(CListUI *)m_pm.FindControl(L"listview2-linedata");
			int RedataSize=resultdataCC.size();
			InitList(pCListUI,RedataSize);
			RECT rc;
			GetClientRect(((CChartWnd *)(Curve1_chart->GetChartWnd()))->GetHWnd(),&rc);
			InvalidateRect(((CChartWnd *)(Curve1_chart->GetChartWnd()))->GetHWnd(),&rc,true);
			CControlUI * parmctrl=m_pm.FindControl(_T("chare_indexDl"));
			parmctrl->Invalidate();*/
		}
		

	}


	virtual HRESULT STDMETHODCALLTYPE UpdateUI( void)
	{
		m_pSearchWnd->ShowWindow(false);
		return S_OK;
	}
	
	void OnCategorySelect(CGameListUI *pList, int nCurSel, int nOldSel)
	{
		CListLabelElementUI *pItem = (CListLabelElementUI*)pList->GetItemAt(nCurSel);
		if( _tcscmp(pItem->GetClass(), _T("ListLabelElementUI")) == 0 ) {
			CGameListUI::Node* node = (CGameListUI::Node*)pItem->GetTag();
			if( node->data()._level == 0 ) {
				/*if(node->data()._expand) pList->SelectItem(nCurSel + 1);*/
				if (node->data()._text==_T("{x 4}{i fuqi.png }{x 4}XX-20"))
				{
					BJselect=0;
					pTabPanel = static_cast<CControlUI*>(m_pm.FindControl(_T("xx20-basic_tab_panel")));
				}else if (node->data()._text==_T("{x 4}{i fuqi.png }{x 4}C7"))
				{
					BJselect=16;
					pTabPanel = static_cast<CControlUI*>(m_pm.FindControl(_T("c7-basic_tab_panel")));
				}

				
			}
			else if( node->data()._level == 1 )
			{
				if (node->data()._text==_T("{x 4}{i zi.png 3.5 1}{x 4}B28F256器件"))
				{
					BJselect=2;
					pTabPanel = static_cast<CControlUI*>(m_pm.FindControl(_T("xx20-B28256")));	
				}else if(node->data()._text==_T("{x 4}{i zi.png 3.5 1}{x 4}SM28F256器件"))
				{
					BJselect=3;
					pTabPanel = static_cast<CControlUI*>(m_pm.FindControl(_T("xx20-SM28F256")));   
				}else if(node->data()._text==_T("{x 4}{i zi.png 3.5 1}{x 4}JMR28F256器件"))
				{
					BJselect=4;
					pTabPanel = static_cast<CControlUI*>(m_pm.FindControl(_T("xx20-JMR28F256")));   
				}else if(node->data()._text==_T("{x 4}{i zi.png 3.5 1}{x 4}XY28F256器件"))
				{
					BJselect=5;
					pTabPanel = static_cast<CControlUI*>(m_pm.FindControl(_T("xx20-XY28F256")));
				}else if(node->data()._text==_T("{x 4}{i zi.png 3.5 1}{x 4}IS61WV25616B器件"))
				{
					BJselect=6;
					pTabPanel = static_cast<CControlUI*>(m_pm.FindControl(_T("xx20-IS61WV25616B")));
				}else if(node->data()._text==_T("{x 4}{i zi.png 3.5 1}{x 4}MR4A08BC器件"))
				{
					BJselect=7;
					pTabPanel = static_cast<CControlUI*>(m_pm.FindControl(_T("xx20-MR4A08BC")));
				}else if(node->data()._text==_T("{x 4}{i zi.png 3.5 1}{x 4}MB85AS4MT器件"))
				{
					BJselect=8;
					pTabPanel = static_cast<CControlUI*>(m_pm.FindControl(_T("xx20-MB85AS4MT")));
				}else if(node->data()._text==_T("{x 4}{i zi.png 3.5 1}{x 4}B7156RH器件"))
				{
					BJselect=9;
					pTabPanel = static_cast<CControlUI*>(m_pm.FindControl(_T("xx20-B7156RH")));
				}
				else if(node->data()._text==_T("{x 4}{i zi.png 3.5 1}{x 4}B8CR256K32器件"))
				{
					BJselect=10;
					pTabPanel = static_cast<CControlUI*>(m_pm.FindControl(_T("xx20-B8CR256K32RH")));
				}
				else if(node->data()._text==_T("{x 4}{i zi.png 3.5 1}{x 4}V7 FPGA器件"))
				{
					BJselect=10;
					pTabPanel = static_cast<CControlUI*>(m_pm.FindControl(_T("xx20-V7")));
				}
				else if(node->data()._text==_T("{x 4}{i zi.png 3.5 1}{x 4}AD9739器件"))
				{
					BJselect=10;
					pTabPanel = static_cast<CControlUI*>(m_pm.FindControl(_T("xx20-AD9739")));
				}
				else if(node->data()._text==_T("{x 4}{i zi.png 3.5 1}{x 4}AD430器件"))
				{
					BJselect=10;
					pTabPanel = static_cast<CControlUI*>(m_pm.FindControl(_T("xx20-AD430")));
				}
				else if(node->data()._text==_T("{x 4}{i zi.png 3.5 1}{x 4}总控"))
				{
					BJselect=11;
					pTabPanel = static_cast<CControlUI*>(m_pm.FindControl(_T("c7-zk")));
				}
				else if(node->data()._text==_T("{x 4}{i zi.png 3.5 1}{x 4}试验单元D"))
				{
					BJselect=12;
					pTabPanel = static_cast<CControlUI*>(m_pm.FindControl(_T("c7-syd")));
				}
				else if(node->data()._text==_T("{x 4}{i zi.png 3.5 1}{x 4}试验单元C"))
				{
					BJselect=13;
					pTabPanel = static_cast<CControlUI*>(m_pm.FindControl(_T("c7-syc")));
				}
				else if(node->data()._text==_T("{x 4}{i zi.png 3.5 1}{x 4}试验单元B"))
				{
					BJselect=14;
					pTabPanel = static_cast<CControlUI*>(m_pm.FindControl(_T("c7-syb")));
				}
				else if(node->data()._text==_T("{x 4}{i zi.png 3.5 1}{x 4}试验单元A"))
				{
					BJselect=15;
					pTabPanel = static_cast<CControlUI*>(m_pm.FindControl(_T("c7-sya")));
				}

			}
		}

		pTabSwitch->SelectItem(pTabPanel);

	}
public:
	CGameListUI::Node* AddCategoryNode(CDuiString sText, int nID)
	{
		CGameListUI* pGameList = static_cast<CGameListUI*>(m_pm.FindControl(_T("categorylist")));
		if(!pGameList) return NULL;
		CDuiString sIcon = _T("fuqi.png");
		CDuiString sFormat;
		sFormat.Format(_T("{x 4}{i %s }{x 4}%s"), sIcon.GetData(), sText.GetData());
		CGameListUI::Node* pCategoryNode = pGameList->AddNode(sFormat, nID);

		return pCategoryNode;
	}

	CGameListUI::Node* AddGameNode(CDuiString sText, CGameListUI::Node *Category, int nID)
	{
		CGameListUI* pGameList = (CGameListUI*)Category->data()._pListElement->GetOwner();
		if(!pGameList) return NULL;
	
		CDuiString sIcon = _T("zi.png");
		CDuiString sFormat;
		sFormat.Format(_T("{x 4}{i %s 3.5 1}{x 4}%s"), sIcon.GetData(), sText.GetData());

		CGameListUI::Node* pGameNode = pGameList->AddNode(sFormat, nID, Category);

		return pGameNode;
	}

public:
	LPCTSTR GetWindowClassName() const { return _T("UIMainFrame"); };
	UINT GetClassStyle() const { return CS_DBLCLKS; };
	void OnFinalMessage(HWND /*hWnd*/) { delete this; };

	void Notify(TNotifyUI& msg)
	{
		CDuiString strControlName = msg.pSender->GetName();

		if( msg.sType == _T("click") ) {
			// 按钮消息
			if( msg.pSender == m_pCloseBtn ) 
			{
				if(IDYES == MessageBox(m_hWnd, _T("确定要退出平台吗？"), _T("天地数据分析平台"), MB_YESNO))
				{
						PostQuitMessage(0);
				}
				return;
			}
			OnLClick(msg.pSender);
		}
		else if( msg.sType == _T("itemclick") ) {

			CGameListUI* pGameList = static_cast<CGameListUI*>(m_pm.FindControl(_T("categorylist")));
			if( pGameList->GetItemIndex(msg.pSender) != -1 )
			{
				if( _tcscmp(msg.pSender->GetClass(), _T("ListLabelElementUI")) == 0 ) {
					CGameListUI::Node* node = (CGameListUI::Node*)msg.pSender->GetTag();

					POINT pt = { 0 };
					::GetCursorPos(&pt);
					::ScreenToClient(m_pm.GetPaintWindow(), &pt);
					pt.x -= msg.pSender->GetX();
					SIZE sz = pGameList->GetExpanderSizeX(node);
					if( pt.x >= sz.cx && pt.x < sz.cy )                     
						pGameList->ExpandNode(node, !node->data()._expand);
				}
			}
		}

		else if(msg.sType == _T("itemselect")&&msg.pSender->GetName()==_T("TEXTDATA"))
		{
			if (msg.pSender->GetName()!=_T("TEXTDATA"))
			{
				return;
			}
			

		}
		else if(msg.sType == _T("itemselect")&&msg.pSender->GetName()==_T("chare_indexDl"))
		{
			if (msg.pSender->GetName()!=_T("chare_indexDl"))
			{
				return;
			}
			CControlUI * parmctrl=m_pm.FindControl(_T("chare_indexDl"));
			wstring wstr= parmctrl->GetText();
			string warn;wstring chartv_seleN;
			UtilTool::setString(warn,wstr.c_str());
			int intStr = atoi(warn.c_str());
			
			UtilTool::setWstring(chartv_seleN,chartv_sele[intStr].c_str());

			CEditUI  * CCDHDATA=static_cast<CEditUI*>(m_pm.FindControl(_T("chare_NameDelect")));
			CCDHDATA->SetText(chartv_seleN.c_str());

		}
		else if(msg.sType == _T("itemselect")&&msg.pSender->GetName()==_T("QTypeSelect1"))
		{
			
		}
		
		else if(msg.sType == _T("itemselect")&&msg.pSender->GetName()==_T("pTypeSelect"))
		{
			
		}
		else if(msg.sType == _T("itemselect"))
		{
			CGameListUI* pGameList = static_cast<CGameListUI*>(m_pm.FindControl(_T("categorylist")));
			if( pGameList == msg.pSender )
			{
				OnCategorySelect(pGameList, (int)msg.wParam, (int)msg.lParam);
				CurParam = (int)msg.wParam;
				OldParam = (int)msg.lParam;
			//点击左侧树状栏，标题页对应切换到数据分析页面，防止再次点击曲线绘制或报警提示失效
				COptionUI* pOptionParam = static_cast<COptionUI*>(m_pm.FindControl(_T("basic_tab")));
				pOptionParam->Selected(true,true);//刷新更新
			}
		}
		else if( msg.sType == _T("selectchanged") ) 
		{	

			if(strControlName.CompareNoCase(_T("basic_tab")) == 0){
				
				//pTabSwitch->SelectItem(0);  //--yww
				CGameListUI* pGameList = static_cast<CGameListUI*>(m_pm.FindControl(_T("categorylist")));
				OnCategorySelect(pGameList, CurParam, OldParam);
				
			}else if(strControlName.CompareNoCase(_T("rich_tab")) == 0){
			
				pTabSwitch->SelectItem(1);
			
			}else if (strControlName.CompareNoCase(_T("Alarm_tab")) == 0)
			{
				if (BJselect<=10)
				{
					pTabSwitch->SelectItem(2);
				}else{
					pTabSwitch->SelectItem(21);
				}
				
			}else if (strControlName.CompareNoCase(_T("ani_tab")) == 0)
			{
				pTabSwitch->SelectItem(3);
			}else if (strControlName.CompareNoCase(_T("split_tab")) == 0)
			{
				pTabSwitch->SelectItem(4);
			}else if (strControlName.CompareNoCase(_T("test_info_tab")) == 0)
			{
				pTabDatabase->SelectItem(1);
			}else if (strControlName.CompareNoCase(_T("search_tele_tab")) == 0)
			{
				pTabDatabase->SelectItem(2);
			}else if (strControlName.CompareNoCase(_T("search_curve_tab")) == 0)
			{
				pTabDatabase->SelectItem(0);
			}
			 		
		}
		else if( msg.sType == _T("itemactivate") ) {
	
		}else if(msg.sType==_T("killfocus")){		
				
		}

		return WindowImplBase::Notify(msg);
	}


	 void InitDecodeLib(){

		string filepath=UtilTool::getPath();

		filepath+="config\\";

		decode=Decode::GetInstance("servercfg.xml",filepath.c_str());

		filepath+="servercfg.xml";

	}
	 void New_Packet()
	{
		wstring postfix=L"";
		if (pMessageTitle)
			msgmap[priviousname]=pMessageTitle;
		string newname;
		UtilTool::setString(newname,postfix.c_str());
		newname=msgName+newname;
		pMessageTitle=msgmap[newname];
		if (pMessageTitle==NULL)
		{
			pMessageTitle=decode->create_msg_by_ShowName(msgName.c_str());
			decode->InitMessageRaw(pMessageTitle);

			msgmap[newname]=pMessageTitle;
		}

		InitMessageDisplay(pMessageTitle,postfix);

		UpDataMessageDisplay(pMessageTitle,postfix);


	}

	 void InitMessageDisplay(Message * pMessage,wstring postfix=L""){

		memset(pMessage->msg_raw, 0, RAWBYTE_SIZE);
		ParameterDescriptor *parameters=pMessage->msg_descriptor->parameters;
		CListTextElementUI* pItem=NULL;
		int elementcounts=pMessage->msg_descriptor->param_cnt;
		//m_pParmTitle->RemoveAllButNotFree();
		string PKname=pMessage->msg_descriptor->cshowname;
		int SelectXx=0;int SelectC7=0;
		if(PKname=="遥测源包PK135格式")
		{
			SelectXx=0;
			PKname="PK135";
		}else if (PKname=="PK136易失型数据包")
		{
			SelectXx=1;
			PKname="PK136Volatile";
		}else if (PKname=="PK136非易失型数据包")
		{
			SelectXx=2;
			PKname="PK136NoVolatile";
		}else if (PKname=="PK136高速测量数据包")
		{
			SelectXx=3;
			PKname="PK136Speed";
		}else if (PKname=="PK136高精度测量数据包")
		{
			SelectXx=4;
			PKname="PK136Precision";
		}else if (PKname=="PK136V7试验数据包")
		{
			SelectXx=5;
			PKname="PK136V7";
		}else if (PKname=="主控单元")
		{
			SelectC7=0;
			PKname="ZKDY";
		}else if (PKname=="试验单元A")
		{
			SelectC7=1;
			PKname="SYDYA";
		}else if (PKname=="试验单元B")
		{
			SelectC7=2;
			PKname="SYDYB";
		}else if (PKname=="试验单元C")
		{
			SelectC7=3;
			PKname="SYDYC";

		}else if (PKname=="试验单元D")
		{
			SelectC7=4;
			PKname="SYDYD";
		}
		for(int i=0;i<elementcounts;i++)
		{					
			parm_map[parameters[i].paramID]=PKname;
			if (PKname=="PK135"||PKname=="PK136Volatile"||PKname=="PK136Speed"||PKname=="PK136NoVolatile"||PKname=="PK136Precision"||PKname=="PK136V7")
			{
				parm_map_Xx[SelectXx][i]=parameters[i].paramName;
			}else if (PKname=="ZKDY"||PKname=="SYDYA"||PKname=="SYDYB"||PKname=="SYDYC"||PKname=="SYDYD")
			{
				parm_map_c7[SelectC7][i]=parameters[i].paramName;
			}
			
			parm_map_D[parameters[i].paramName]=parameters[i].paramID;
		}


		string ErrLink[][5]={"p70","p42","p12","","",
						     "p71","p43","p13","","",
							 "p60","p40","","","",
							 "p61","p41","","","",
							 "p62","p54","","","",
							 "p63","p55","","","",
							 "p64","p56","","","",
							 "p65","p57","","","",
							 "p66","p58","","","",
							 "p67","p59","","","",
							 "p39","","","","",
							 "p37","p52","","","",
							 "p38","p53","","","",
							 "p21","p44","","","",
							 "p23","p45","","","",
							 "p25","p46","","","",
							 "p27","p47","","","",
							 "p29","p48","","","",
							 "p31","p49","","","",
							 "p33","p50","","","",
							 "p35","p51","","","",
							 "p15","p16","p68","p37","",
							 "p17","p18","p69","p38","",
							 "p22","","","","",
							 "p24","","","","",
							 "p26","","","",""};
		int row=sizeof(ErrLink)/sizeof(ErrLink[0]);
		vector <string> err_link;

			for(int i=0;i<row;i++)
			{
				err_link.clear();
				for(int j=0;j<4;j++)
				{
					if(ErrLink[i][j+1] != "")
					err_link.push_back(ErrLink[i][j+1]);
				}
				err_link_map[ErrLink[i][0]]=err_link;
			}
	}
	 void UpDataMessageDisplay( Message * pmessage,wstring postfix=L"")
	{
		string YCName;
		char temps1[800];
		char Temp_ts[800];
		memset(Temp_ts,0,800);
		if (pmessage==NULL)
		{
			return;
		}
		//bool_enable_notify=false;

		CListTextElementUI* pItem=NULL;
		string result;
		int elementcounts=pmessage->msg_descriptor->param_cnt;
		//YCName=pmessage->msg_descriptor->cshowname;
		wstring valstr;
		//char buf[50];
		if (pMessageTitle==NULL)
		{
			return;
		}
		for(size_t i=0;i<pmessage->msg_descriptor->param_cnt;i++)
		{	
			ParameterValue *param_value=&pmessage->param_value[i];
			ParameterDescriptor *parameters=&pmessage->msg_descriptor->parameters[i];
			YCName=pmessage->msg_descriptor->cshowname;
			wstring wparmname;int Frame_count;double a_value,b_value;
			UtilTool::setWstring(wparmname,decode->get_markid(pmessage,i));
			wparmname+=postfix;
			if (select_control_name==wparmname)
			{
				continue;
			}

			CControlUI * parmctrl=m_pm.FindControl(wparmname.c_str());
			if (parmctrl == NULL)
				continue;
			CLabelUI * parmlable=static_cast<CLabelUI*>(static_cast<CVerticalLayoutUI*>(parmctrl->GetParent())->FindSubControl(_T("name")))	;
			
			int start=0,count=0;
			decode->get_parmrawpos(pMessageTitle,i,&start,&count);
			

			if (decode->getAnnotation_cnt(pmessage,i))
			{
				if((parameters->annotation_cnt>0))
				{
	
					int iAnnotationIndex=decode->getAnnotation_index(pmessage,i);
					static_cast<CComboBoxUI*>(parmctrl)->SelectItem(iAnnotationIndex);
					
					continue;
				}
			}
			else{
				bool change=false;
				int len=0;

				char result_buf[200]={0};	
				char * result_string_ptr=NULL;
				//	if (strlen(parameters->formulaName.c_str()))
				{

					uint64_t rawdata=param_value->v.rawvalue;
					float value=0;	

					result_string_ptr=decode->getParmentResult(pmessage,i,&len);
					result=result_string_ptr;
					delete result_string_ptr;

				}


				//if (YCkg==true)
				//{
				//	if (YCName=="主控单元")
				//	{
				//		double 	DL=0;
				//		double 	Dvalue=0;
				//		float DLvalue=0;
				//		uint64_t Dldata=param_value->v.rawvalue;

				//		if (parameters->paramID=="Z28"||parameters->paramID=="Z29"||parameters->paramID=="Z30"||parameters->paramID=="Z31")
				//		{
				//			DL=Dldata;
				//			DLvalue=(DL*5/256)*2;			

				//			Dvalue=DLvalue;

				//			CDuiString str;
				//			str.Format(L"Voltage values:%.2fV  "    ,Dvalue);
				//			if (parameters->paramID=="Z28")
				//			{
				//			
				//				C7syAYdata->SetText(str);
				//				CChart * temp=((CChartWnd *)(C7syAdY->GetChartWnd()))->GetChart();
				//				temp->ClrPlotData();					
				//				temp->AddCurve();						
				//				temp->SetEnableToolTip(true);
				//				temp->SetToolTipType(2);
				//				temp->SetUseLegend(true);
				//				temp->SetTitle(_T("试验单元A电压数据显示"));	
				//				temp->SetDataTitle(_T("SYA电压数据值"), 0);				
				//				temp->AddPoint2D((double)C7ADY, Dvalue,0);
				//				C7ADY+=1;

				//			}
				//			sprintf(result_buf,"%.2f",DLvalue);
				//			result=result_buf;
				//		}
				//	
				//	
				//

				//	

				//	}

				//UtilTool::setWstring(valstr,result.c_str());
				//parmctrl->SetText(valstr.c_str());
				//continue;
			//}

			//if (parmctrl)
			//{
			//	if (decode->isDefault(pMessageTitle,i))
			//	{
			//		static_cast<CEditUI*>(parmctrl)->SetTextColor(0x000000);
			//		parmlable->SetTextColor(0x000000);	
			//		//pcHexList->MarkHexSelect(start,count,0x000000);
			//	}
			//	else
			//	{
			//		static_cast<CEditUI*>(parmctrl)->SetTextColor(0xff0000);
			//		parmlable->SetTextColor(0xff0000);		
			//		//pcHexList->MarkHexSelect(start,count,0xff0000);
			//	}
			//	parmctrl->SetText(valstr.c_str());
			//}
			//else 
			
		} 
	}
	
}
		

	// void UpdataDisplays(Message * pMessage, unsigned char * msg_raw,int length){
	//	memcpy(pMessage->msg_raw, msg_raw, length);
	//	decode->decode_msg(pMessage, msg_raw);

	//	UpDataMessageDisplay(pMessage);	

	//}


	 void UpdataDisplay(Message * pMessage, unsigned char * msg_raw,string msg_time,int length){
		 memcpy(pMessage->msg_raw, msg_raw, length);
		 decode->decode_msg(pMessage, msg_raw);

		 UpDataMessageDisplay(pMessage);
		// UpDataMessageDisplay_Initial(pMessage,msg_time);	

	 }

	
	
	 void AddCombo(){

		 p_select_combox = static_cast<CComboBoxUI*>(m_pm.FindControl(_T("p14")));
		 parameters[0]="不到25%";parameters[1]="26%到50%";parameters[2]="51%到75%";parameters[3]="76%到99%";parameters[4]="100%";
		 for(int i=0;i<5;i++)
		 {
			 wstring temp; 
			 CListLabelElementUI * pLabel= new CListLabelElementUI();
			 UtilTool::setWstring(temp,parameters[i].c_str());
			 pLabel->SetText(temp.c_str());
			 pLabel->SetBkColor(0xff0000);
			 if (p_select_combox)
				 p_select_combox->AddAt(pLabel,i);
		 }

		 p_select_combox->SelectItem(0);
//		 p_select_combox->SetBoolShowControl(true);

	
	 }

	 //void InitDatabase(int type){
		// string time_string=mp.get_cur_timestring();
		// CDuiString bm_test_name;
		// wstring time_wstring;
		// UtilTool::setWstring(time_wstring,time_string.c_str());
		// if (type==1)
		// {
		//	  bm_test_name+=_T("XX20-");
		// }else if (type==2)
		// {
		//	  bm_test_name+=_T("C7-");
		// }
		//
		// bm_test_name+=time_wstring.c_str();

		// //if (CAddTest::MessageBox(NULL,L"启动数据存储",L"",bm_test_name,)!=IDOK)
		// //{
		//	// return;
		// //}

		// UtilTool::setString(testname,ZTest_Name.GetData());

		// //mp.CreateTables(testname);
		//// mp.CreateParmTables(decode,testname,bm_insert_parm_string_map);
	 //}

	 void InitCommbox(CComboBoxUI * pComboParm,CDuiString prefix,int n){
		 for (int i=0;i<n;i++)
		 {
			 CListLabelElementUI * pLabel= new CListLabelElementUI();
			 CDuiString temp;
			 temp.Format(L"%d",i);
			 temp+=prefix;
			 pLabel->SetText(temp);

			 if (pComboParm)
				 pComboParm->AddAt(pLabel,i);
		 }
		 pComboParm->SelectItem(0);
	 }

	 void InitDateTime(CDuiString name){
		 CHorizontalLayoutUI * pCHorizontalLayoutUI=(CHorizontalLayoutUI *)m_pm.FindControl(name);
		 if (pCHorizontalLayoutUI==nullptr)
		 {
			 return;
		 }
		 CComboBoxUI * pComboParm=static_cast<CComboBoxUI*>(pCHorizontalLayoutUI->FindSubControl(L"start_hour"));
		 InitCommbox(pComboParm,L"时",24);
		 pComboParm=static_cast<CComboBoxUI*>(pCHorizontalLayoutUI->FindSubControl(L"end_hour"));
		 InitCommbox(pComboParm,L"时",24);

		 pComboParm=static_cast<CComboBoxUI*>(pCHorizontalLayoutUI->FindSubControl(L"start_min"));
		 InitCommbox(pComboParm,L"分",60);
		 pComboParm=static_cast<CComboBoxUI*>(pCHorizontalLayoutUI->FindSubControl(L"end_min"));
		 InitCommbox(pComboParm,L"分",60);

		 pComboParm=static_cast<CComboBoxUI*>(pCHorizontalLayoutUI->FindSubControl(L"start_second"));
		 InitCommbox(pComboParm,L"秒",60);
		 pComboParm=static_cast<CComboBoxUI*>(pCHorizontalLayoutUI->FindSubControl(L"end_second"));
		 InitCommbox(pComboParm,L"秒",60);
	 }

	 string GetLimitString(CDuiString name,string & pose_string){
		 CHorizontalLayoutUI * pCHorizontalLayoutUI=(CHorizontalLayoutUI *)m_pm.FindControl(name);
		 wstring limit_string;
		 string limit_result_string;
		 if (pCHorizontalLayoutUI==nullptr)
		 {
			 return limit_result_string;
		 }	

		 CDuiString str;
		 CComboBoxUI * pComboParm=NULL;
		 limit_string=L"time > '";
		 CDuiString lower_str,upper_str;
		 str=pCHorizontalLayoutUI->FindSubControl(L"start_time")->GetText();
		 limit_string+=str;
		 lower_str+=str;
		 limit_string+=L" ";
		 pComboParm=static_cast<CComboBoxUI*>(pCHorizontalLayoutUI->FindSubControl(L"start_hour"));
		 str.Format(L"%02d:", pComboParm->GetCurSel());	
		 limit_string+=str.GetData();
		 lower_str+=str;
		 pComboParm=static_cast<CComboBoxUI*>(pCHorizontalLayoutUI->FindSubControl(L"start_min"));
		 str.Format(L"%02d:", pComboParm->GetCurSel());
		 lower_str+=str;
		 limit_string+=str.GetData();
		 pComboParm=static_cast<CComboBoxUI*>(pCHorizontalLayoutUI->FindSubControl(L"start_second"));
		 str.Format(L"%02d", pComboParm->GetCurSel());
		 lower_str+=str;
		 limit_string+=str.GetData();
		 limit_string+=L".000";
		 limit_string+=L"' and time<'";

		 str=pCHorizontalLayoutUI->FindSubControl(L"end_time")->GetText();
		 limit_string+=str;
		 upper_str+=str;
		 limit_string+=L" ";
		 pComboParm=static_cast<CComboBoxUI*>(pCHorizontalLayoutUI->FindSubControl(L"end_hour"));
		 str.Format(L"%02d:", pComboParm->GetCurSel());
		 upper_str+=str;
		 limit_string+=str.GetData();
		 pComboParm=static_cast<CComboBoxUI*>(pCHorizontalLayoutUI->FindSubControl(L"end_min"));
		 str.Format(L"%02d:", pComboParm->GetCurSel());
		 upper_str+=str;
		 limit_string+=str.GetData();
		 pComboParm=static_cast<CComboBoxUI*>(pCHorizontalLayoutUI->FindSubControl(L"end_second"));
		 str.Format(L"%02d", pComboParm->GetCurSel());
		 if (pCHorizontalLayoutUI->FindSubControl(L"begin_line")->GetText()==L"")
		 {
			 pCHorizontalLayoutUI->FindSubControl(L"begin_line")->SetText(L"0");
		 }	if (pCHorizontalLayoutUI->FindSubControl(L"search_line")->GetText()==L"")
		 {
			 pCHorizontalLayoutUI->FindSubControl(L"begin_line")->SetText(L"1000");
		 }
		 upper_str+=str;
		 limit_string+=str.GetData();
		  limit_string+=L".000";
		 limit_string+=L"'";		
		 str=L" LIMIT ";
		 str+=pCHorizontalLayoutUI->FindSubControl(L"begin_line")->GetText();
		 str+=L",";
		 str+=pCHorizontalLayoutUI->FindSubControl(L"search_line")->GetText();
		 UtilTool::setString(pose_string,str.GetData());
		 if (lower_str==upper_str)
		 {
			 return "";
		 }
		 UtilTool::setString(limit_result_string,limit_string.c_str());	
		 return limit_result_string;
	 }

	 inline CDuiString untostr(unsigned char* orig,int uclen)
	{
		CDuiString str_temp,retur;
		int i=0;
		
		str_temp=L"";
		retur=L"";
		/*if (uclen>10000)
		{
			return L"";
		}*/
		if(orig!=NULL)
		{
			for(i=0;i<uclen;i++)
			{
				retur.Format(L"%02X ",(unsigned char)orig[i]);
				str_temp+=retur;
			}
			return str_temp;
		}
		else
		{
             return L"";
		}	 
		
		
		
		

	}

	LRESULT OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
	{
		bHandled = TRUE;
		PostQuitMessage(0);
		return 0;
	}
	LRESULT OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		// 有时会在收到WM_NCDESTROY后收到wParam为SC_CLOSE的WM_SYSCOMMAND
		if( wParam == SC_CLOSE ) {
			::PostQuitMessage(0L);
			bHandled = TRUE;
			return 0;
		}
		BOOL bZoomed = ::IsZoomed(*this);
		LRESULT lRes = CWindowWnd::HandleMessage(uMsg, wParam, lParam);
		if( ::IsZoomed(*this) != bZoomed ) {
			if( !bZoomed ) {
				CControlUI* pControl = static_cast<CControlUI*>(m_pm.FindControl(_T("maxbtn")));
				if( pControl ) pControl->SetVisible(false);
				pControl = static_cast<CControlUI*>(m_pm.FindControl(_T("restorebtn")));
				if( pControl ) pControl->SetVisible(true);
			}
			else {
				CControlUI* pControl = static_cast<CControlUI*>(m_pm.FindControl(_T("maxbtn")));
				if( pControl ) pControl->SetVisible(true);
				pControl = static_cast<CControlUI*>(m_pm.FindControl(_T("restorebtn")));
				if( pControl ) pControl->SetVisible(false);
			}
		}
		return lRes;
	}

	LRESULT HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		LRESULT lRes = 0;
		switch( uMsg ) {
		case WM_LBUTTONDOWN:
		case WM_NCLBUTTONDOWN:
			{
				m_pSearchWnd->ShowWindow(false);
				bHandled = FALSE;
				break;
			}
		case WM_ACTIVATEAPP:
			{
				if((BOOL)wParam == FALSE && IsWindow(m_pSearchWnd->GetHWND()))
				{
					m_pSearchWnd->ShowWindow(false);
				}
				bHandled = FALSE;
				break;
			}
		default:
			bHandled = FALSE;
		}

		return lRes;
	}

	inline double max_double(double array[],int  count)
	{
		double maxdou=0.0;
		for(int i=0;i<count;i++)
		{if(i==0)
		{
			maxdou=array[0];
		}
		if(maxdou<=array[i])
			maxdou=array[i];

		}
		return maxdou;
	}
	inline double min_double(double array[],int  count)
	{
		double mindou=0.0;
		for(int i=0;i<count;i++)
		{
			if(i==0)
			{
				mindou=array[0];
			}
			if(mindou>=array[i])
				mindou=array[i];

		}
		return mindou;
	}
	inline double maxvec_double(vector<double> datas)
	{
		double maxdou=0.0;
		for(int i=0;i<datas.size();i++)
		{if(i==0)
		{
			maxdou=datas[0];
		}
		if(maxdou<=datas[i])
			maxdou=datas[i];

		}
		return maxdou;
	}
	inline double minvec_double(vector<double> datas)
	{
		double mindou=0.0;
		for(int i=0;i<datas.size();i++)
		{
			if(i==0)
			{
				mindou=datas[0];
			}
			if(mindou>=datas[i])
				mindou=datas[i];

		}
		return mindou;
	}



	inline void InitList(CListUI * pList,int rowCount){
		while (pList->GetCount()<rowCount)
		{
		/*	CListContainerElementUI* pItem  = new CListContainerElementUI();*/
			CListTextElementUI* pItem  = new CListTextElementUI();	
			pList->Add(pItem);
		}
		while (pList->GetCount()>rowCount)
		{
			pList->RemoveAt(0);
		}
	}

	inline void InitListSelect(CListUI * pList,int rowCount,int index){
		while (pList->GetCount()<rowCount)
		{
			/*	CListContainerElementUI* pItem  = new CListContainerElementUI();*/
			CListTextElementUI* pItem  = new CListTextElementUI();	
			pList->Add(pItem);
		}
		while (pList->GetCount()>rowCount)
		{
			pList->RemoveAt(index);
		}
	}

	inline void InitListS(CListUI * pList,int index){
		
		for (int i=0;i<pList->GetCount();i++)
		{
			pList->RemoveAt(index);
		}
		/*while (pList->GetCount()>rowCount)
		{
			pList->RemoveAt(index);
		}*/
	}




public:
	CSearchWnd *m_pSearchWnd;
	Decode * decode;
//	Csqlpool *mypool;
	COptionUI * pdata;
	vector<string> replyData;
	vector<string> replyDataTime;
	Message * pMessage[2],* pMessageTitle;
	map<string ,Message*> msgmap;
	string priviousname;
	string msgName;string rtname;
	map<CDuiString,double> SirenData;
	map<CDuiString, double>::iterator iter;
	map<CDuiString,double> SirenDatac;
	map<CDuiString, double>::iterator iterc;
	wstring select_control_name;
	CComboBoxUI  * p_select_combox;	
	string parameters[9];
	CLabelUI *precious1_status;
	CWndUI * high1_chart,*precious1_chart;
	CWndUI * C7syAdL;
	CButtonUI* m_pCloseBtn;
	CTabLayoutUI* pTabSwitch;
	CTabLayoutUI* pTabDatabase;
	CControlUI* pTabPanel;
	int CurParam,OldParam;//yww
	vector<string> tempdata;
	int SatelliteNum;
	int BJselect;
	CButtonUI * ZLState;
	string Pathorg;
	unsigned char  tempS[300];
	string spaceDate,spaceTime,FrameTime;
	map<string,string> bm_insert_parm_string_map;
	string	 pose_string;
	//MysqlPool mp;
	map<string,string> parm_map;
	map<string,string> parm_map_D;
	map<string,vector<string>> err_link_map;
	string parm_map_Xx[30][300];
	string parm_map_c7[30][300];
	string testname;//数据库名称
	bool data_save_KG;
	bool LastStep;
	bool NextStep;
	vector<vector<string>> resultdata;
	vector<vector<string>> resultC7data;
	map<int,string>chartv_sele;
	vector<vector<string>> resultdataCC;
	int curHour,curMinute,curSecond;
};

void InitResource()
{	
	// 资源类型
#ifdef _DEBUG
	CPaintManagerUI::SetResourceType(UILIB_FILE);
#else
	CPaintManagerUI::SetResourceType(UILIB_ZIPRESOURCE);
#endif
	// 资源路径
	CDuiString strResourcePath = CPaintManagerUI::GetInstancePath();
	// 加载资源
	switch(CPaintManagerUI::GetResourceType())
	{
	case UILIB_FILE:
		{
			strResourcePath += _T("skin\\gamebox\\");
			CPaintManagerUI::SetResourcePath(strResourcePath.GetData());
			// 加载资源管理器
			CResourceManager::GetInstance()->LoadResource(_T("res.xml"), NULL);
			break;
		}
	case UILIB_RESOURCE:
		{
			strResourcePath += _T("skin\\gamebox\\");
			CPaintManagerUI::SetResourcePath(strResourcePath.GetData());
			// 加载资源管理器
			CResourceManager::GetInstance()->LoadResource(_T("IDR_RES"), _T("xml"));
			break;
		}
	case UILIB_ZIP:
		{
			strResourcePath += _T("skin\\");
			CPaintManagerUI::SetResourcePath(strResourcePath.GetData());
			CPaintManagerUI::SetResourceZip(_T("gamebox.zip"), true);
			// 加载资源管理器
			CResourceManager::GetInstance()->LoadResource(_T("res.xml"), NULL);
			break;
		}
	case UILIB_ZIPRESOURCE:
		{
			strResourcePath += _T("skin\\gamebox\\");
			CPaintManagerUI::SetResourcePath(strResourcePath.GetData());

			HRSRC hResource = ::FindResource(CPaintManagerUI::GetResourceDll(), _T("IDR_ZIPRES"), _T("ZIPRES"));
			if( hResource != NULL ) {
				DWORD dwSize = 0;
				HGLOBAL hGlobal = ::LoadResource(CPaintManagerUI::GetResourceDll(), hResource);
				if( hGlobal != NULL ) {
					dwSize = ::SizeofResource(CPaintManagerUI::GetResourceDll(), hResource);
					if( dwSize > 0 ) {
						CPaintManagerUI::SetResourceZip((LPBYTE)::LockResource(hGlobal), dwSize);
						// 加载资源管理器
						CResourceManager::GetInstance()->LoadResource(_T("res.xml"), NULL);
					}
				}
				::FreeResource(hResource);
			}
		}
		break;
	}


	// 注册控件
	//REGIST_DUICONTROL(CLabelIconUI);
	//REGIST_DUICONTROL(CGameItemUI);
	//REGIST_DUICONTROL(CGameListUI);
	//REGIST_DUICONTROL(CLabelMutilineUI);
	//REGIST_DUICONTROL(CCircleProgressUI);
	REGIST_DUICONTROL(CMyComboUI);
	REGIST_DUICONTROL(CChartViewUI);
}

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPSTR /*lpCmdLine*/, int nCmdShow)
{
	HRESULT Hr = ::CoInitialize(NULL);
	if( FAILED(Hr) ) return 0;
	// 初始化UI管理器
	CPaintManagerUI::SetInstance(hInstance);
	// 初始化资源
	InitResource();

	if (RTXInstalled())
	{
		return -1;
	}

	if (!LoadDLL())
	{
		cout << "系统没有安装 Real-Time Extension RunTime, 应用程序退出!";
		//AfxMessageBox(_T("系统没有安装 Real-Time Extension RunTime, 应用程序退出!"));
		return -2;
	}


	// 初始化数据库;

	if (!ConnectMysql::Instance().StartConnectMysql())
	{
		cout << "StartConnectMysql Failure!!!" << endl;
		string msg = "数据库连接失败!";

	}
	else
	{
		cout << "StartConnectMysql Successful!!!" << endl;
		string msg = "数据库链接成功!";

	}

	CSplashWnd::MessageBox(NULL);

	/*CGameBoxFrame* pFrame = new CGameBoxFrame();
	if( pFrame == NULL ) return 0;
	pFrame->Create(NULL, _T("天地数据分析平台"), UI_WNDSTYLE_FRAME, 0L, 0, 0, 800, 572);
	pFrame->CenterWindow();
	::ShowWindow(*pFrame, SW_SHOW);

	CPaintManagerUI::MessageLoop();

	::CoUninitialize();*/
	return 0;
}