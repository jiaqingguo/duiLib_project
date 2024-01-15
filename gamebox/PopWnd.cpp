#include "StdAfx.h"
#include "PopWnd.h"
#include <ShellAPI.h>
#include <process.h>
#include <Windows.h>
#include <iostream>
#include <fstream>
#include "UtilTool.h"
#include "direct.h"
#include "io.h"
//////////////////////////////////////////////////////////////////////////
///
vector<string> PreplyData;
vector<string> PreplyDataTime;
int data_save_en;

DUI_BEGIN_MESSAGE_MAP(CPopWnd, WindowImplBase)
	DUI_ON_MSGTYPE(DUI_MSGTYPE_CLICK,OnClick)
	DUI_ON_MSGTYPE(DUI_MSGTYPE_ITEMSELECT,OnItemSelect)
	DUI_ON_MSGTYPE(DUI_MSGTYPE_SELECTCHANGED,OnSelectChanged)
	DUI_END_MESSAGE_MAP()

CPopWnd::CPopWnd(void)
{
}

CPopWnd::~CPopWnd(void)
{
}

void CPopWnd::OnFinalMessage( HWND hWnd)
{
	__super::OnFinalMessage(hWnd);
	delete this;
}

DuiLib::CDuiString CPopWnd::GetSkinFile()
{
	return _T("Loading.xml");
}

LPCTSTR CPopWnd::GetWindowClassName( void ) const
{
	return _T("PopWnd");
}

void CPopWnd::OnClick( TNotifyUI &msg )
{
	CDuiString sName = msg.pSender->GetName();
	sName.MakeLower();

	if( msg.pSender == m_pCloseBtn ) {
		Close(0);
		return; 
	}
	else if( msg.pSender == m_pMinBtn ) { 
		//CMsgWnd::MessageBox(m_hWnd, NULL, _T("子子窗口"));

		SendMessage(WM_SYSCOMMAND, SC_MINIMIZE, 0); return; }
	else if( msg.pSender == m_pMaxBtn ) { 
		SendMessage(WM_SYSCOMMAND, SC_MAXIMIZE, 0); return; }
	else if( msg.pSender == m_pRestoreBtn ) { 
		SendMessage(WM_SYSCOMMAND, SC_RESTORE, 0); return; }
	else if( msg.pSender == m_pMenuBtn ) {
	}
	else if(sName.CompareNoCase(_T("homepage_btn")) == 0)
	{
		ShellExecute(NULL, _T("open"), _T("https://github.com/qdtroy"), NULL, NULL, SW_SHOW);
	}
}

void CPopWnd::OnSelectChanged( TNotifyUI &msg )
{
	CDuiString sName = msg.pSender->GetName();
	sName.MakeLower();

}

void CPopWnd::OnItemSelect( TNotifyUI &msg )
{
	CDuiString sName = msg.pSender->GetName();
	sName.MakeLower();
}

LRESULT CPopWnd::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;
	return 0;
}

void CPopWnd::Notify( TNotifyUI &msg )
{
	return WindowImplBase::Notify(msg);
}

LRESULT CPopWnd::OnSysCommand( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
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


string  CPopWnd::get_cur_timelogdata()/*获取当前时间字符串*/
{
	string time_string;
	SYSTEMTIME stime;
	char buf[100]={0};
	GetLocalTime(&stime);
	sprintf(buf,"%04d.%02d.%02d.%02d.%02d.%02d",stime.wYear,stime.wMonth,stime.wDay,stime.wHour,stime.wMinute,stime.wSecond);
	time_string+=buf;
	return time_string;
}

unsigned __stdcall CPopWnd:: Busys( void* param )  
{  
	
	FILE * lOG;
	string logdata,path1,path2;
	vector<string> replyDatas,datatime;
	string newname,tempstr,tempstr1;
	CPopWnd * pWnd = (CPopWnd*)param;
	pWnd->Complement=true;
	pWnd->Id_Complement=true;
	unsigned char ReplyData;
	logdata= pWnd->get_cur_timelogdata();
	path1 = "E:\\DATA";

	
	if (pWnd->PSatelliteNum==1)
	{

		if(data_save_en==1)
		{
			if (0 != _access(path1.c_str(), 0))
				{
					_mkdir(path1.c_str());   // 返回 0 表示创建成功，-1 表示失败
				}
			logdata=path1+"\\"+logdata+".txt";
			ofstream fout(logdata,ios::app);
			fout.close();
			lOG=fopen(logdata.c_str(),"a");
		}

		for(int i=0;i<pWnd->Ptempdata.size();i++)
		{
			replyDatas=UtilTool::split_string(pWnd->Ptempdata[i].c_str(),' ');
			/*replyDatas=pWnd->split_string_XX20(pWnd->Ptempdata[i]);*/
			
			if (0<replyDatas.size())
			{
				//datatime = UtilTool::get_datatime(pWnd->Ptempdata[i].c_str());
				if(data_save_en==1)
				{
					//fprintf(lOG,"%s  ",datatime[0].c_str());
					fprintf(lOG,"%s ",replyDatas[0].c_str());
					fprintf(lOG,"\n");
				}

				tempstr+=replyDatas[0];
				//tempstr1+=datatime[0];
				PreplyData.push_back(tempstr);
				//PreplyDataTime.push_back(tempstr1);
			}

			tempstr="";
			//tempstr1="";
		}

	}
	
	fclose(lOG);
	
	::PostMessage(pWnd->m_hWnd, WM_CLOSE, 0, 0);
	return 0;
}  

void CPopWnd::InitWindow()
{
	
	m_pCloseBtn = static_cast<CButtonUI*>(m_pm.FindControl(_T("closebtn")));
	m_pMaxBtn = static_cast<CButtonUI*>(m_pm.FindControl(_T("maxbtn")));
	m_pRestoreBtn = static_cast<CButtonUI*>(m_pm.FindControl(_T("restorebtn")));
	m_pMinBtn = static_cast<CButtonUI*>(m_pm.FindControl(_T("minbtn")));
	m_pMenuBtn = static_cast<CButtonUI*>(m_pm.FindControl(_T("menubtn")));
	
	CreateThread(NULL, 0,(LPTHREAD_START_ROUTINE)Busys, this,0,NULL);

	//_beginthread(Busys, 0, m_hWnd);
}
