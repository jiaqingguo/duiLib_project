#include "stdafx.h"
#include "GameBox.h"
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <ws2tcpip.h>
#include "winsock2.h"
#include<cstdlib>
#include <string>
#include <cstring>
#include "PopWnds.h"
//#include "AddTest.h"

#pragma comment(lib,"ws2_32.lib")//引用库文件
using namespace std;

BOOL CGameBoxFrame::StartThread()
{
	CreateThread(NULL, 0,(LPTHREAD_START_ROUTINE)ThreadDATAJX, this,0,NULL);
	return TRUE;
}

unsigned __stdcall CGameBoxFrame::ThreadDATAJX(void* arglist)
{

	CGameBoxFrame * pMainWnd = (CGameBoxFrame*)arglist;

	while(1)
	{
	
	    //memcpy(sendBuff,&m_dynamic,88); 结构体给到数组

	    // memcpy(&m_dynamic, &sendBuff, 88);

	    //memset(m_DataBuffer, 0, 30000);

		/*	double DLvalue; char result_buf[100]; string resultc;
		sprintf(result_buf,"%.2f",DLvalue);
		resultc=result_buf;

		std::string str = "123";     string转int
		int n = atoi(str.c_str());


		byte* px = (byte*)str.c_str();

		bitset <8> bitas;            bit转字节
		int a=0;
		bitas.set(0,a);
		bitas.set(1,1);
		bitas.set(2,1);
		bitas.set(3,1);
		bitas.set(4,1);
		bitas.set(5,1);
		bitas.set(6,1);
		bitas.set(7,0);
		unsigned char temp=bitas.to_ulong();


		CDuiString dstr;
		int  t2;
		dstr.Format(L"打印%d\n", t2);
		OutputDebugString(dstr);*/


		//string timestring=	UtilTool::GetTimeString(); 获取时间

		//wstring wstr= parmctrl->GetText();
		//string warn;
		//UtilTool::setString(warn,wstr.c_str());      wstring转String

		//UtilTool::setWstring(temps,valstr.c_str());  String转wstring

		//CEditUI * CCDHDATA = static_cast<CEditUI*>(m_pm.FindControl(_T("AD9739APJ")));
		//CCDHDATA->SetText(temps.c_str());
		//CCDHDATA->SetTextColor(0xff0000);           文本框写入标

		Sleep(1);
	 }
	
	return 0;
}
