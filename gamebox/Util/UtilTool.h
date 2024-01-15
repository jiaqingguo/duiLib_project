/*!
* 工具方法
*/

#pragma comment(lib,"shlwapi.lib")
#pragma once
#include <string>
#include <WinBase.h>
#include <windows.h>  
#include <ShellAPI.h>
#include <shlwapi.h>
#include <TlHelp32.h>
#include <commdlg.h>
//#include "tinyxml.h"
#include <vector>
#include "libxl.h"
using namespace libxl;

using namespace std;
#define  TEMSIZE 100
typedef void (*Func)(void * parm,void * result);


class UtilTool
{


private: UtilTool(void)
		 {
		 }

		 ~UtilTool(void)
		 {
		 }

public: 
	static char * getPath(){
		wchar_t chpath[MAX_PATH*2];  
		GetModuleFileName(NULL,chpath,sizeof(chpath));  
		wcscat(chpath,L"\\..\\");
		return	UtilTool::wcharToChar(chpath);
	}

	static string getExeName(){
		wchar_t chpath[MAX_PATH*2];  
		GetModuleFileName(NULL,chpath,sizeof(chpath));  
		string file_path;
		UtilTool::setString(file_path,chpath);
		replace_all(file_path,"\\"," ");
		vector<string> file_path_exe=UtilTool::split_string((char *)file_path.c_str(),' ');
		return file_path_exe[file_path_exe.size()-1];
	}

	//static	short GetSum(ADT_L1_1553_CDP & myCdp,unsigned short* data,int len)
	//	{
	//		short sum(0);
	//		for (int ii = 0; ii < len; ++ii)
	//		{
	//			myCdp.DATAinfo[ii] =htons( data[ii]);
	//			if(ii<11)
	//				sum += myCdp.DATAinfo[ii] & 0x0000FFFF;
	//		}
	//		sum = ~sum;
	//		myCdp.DATAinfo[len] = sum;
	//		return	sum;
	//	}

	//static	string GetConfigFileName(string FileName){
	//	//  <xlsfilename filename="Packets.xlsx"/>
	//	string xls_file_name;
	//	TiXmlElement *space, *data;
	//	TiXmlDocument doc(FileName.c_str());
	//	if (!doc.LoadFile()) return xls_file_name;
	//	TiXmlElement *root = doc.RootElement();
	//	space = root->FirstChildElement();
	//	xls_file_name=space->Attribute("filename");
	//	return xls_file_name;
	//}

	inline static string& trim(string& text)
	{
		if(!text.empty())
		{
			text.erase(0, text.find_first_not_of((" \n\r\t")));
			text.erase(text.find_last_not_of((" \n\r\t")) + 1);
		}
		return text;
	}

	inline static int split(const string& str, vector<string>& ret_, string sep = ",")
	{
		if (str.empty())
		{
			return 0;
		}

		string tmp;
		string::size_type pos_begin = str.find_first_not_of(sep);
		string::size_type comma_pos = 0;

		while (pos_begin != string::npos)
		{
			comma_pos = str.find(sep, pos_begin);
			if (comma_pos != string::npos)
			{
				tmp = str.substr(pos_begin, comma_pos - pos_begin);
				pos_begin = comma_pos + sep.length();
			}
			else
			{
				tmp = str.substr(pos_begin);
				pos_begin = comma_pos;
			}

			if (!tmp.empty())
			{
				ret_.push_back(tmp);
				tmp.clear();
			}
		}
		return 0;
	}


	static void TrimZero(char * buf,int len){
		int i;
		for ( i=0;i<len;i++)
		{
			if (buf[i]=='.')
			{
				break;
			}
		}
		if (i==len)
		{
			return;
		}

		for(int i=0;i<len;i++){
			if (buf[len-i-1]=='0')
			{
				buf[len-i-1]=0;
			}else if(buf[len-i-1]=='.')
			{
				buf[len-i-1]=0;
				return;
			}else
				return;
		}
	}

	static inline void SaveExcel(vector<vector<string>> & value_string,string type)
	{
		wstring wfilepath;wstring wfoldpath;
		string configpath=(const char *)UtilTool::getPath();
		configpath+="path.txt";
		string path=(char *)UtilTool::readfile(configpath.c_str());
		vector<string> vs=UtilTool::split_string_multi_row((char *)path.c_str(),'\n');
		UtilTool::setWstring(wfoldpath,vs[1].c_str());
		string filepath;
		filepath=UtilTool::GetTimeString();
		filepath+="-";
		filepath+=type;
		filepath+=".xls";
		UtilTool::setWstring(wfilepath,filepath.c_str());
		string bcdpath;
		if (UtilTool::GetSaveFilePath(wfilepath,wfoldpath,L"")==NULL)
		{
			return;
		}
		UtilTool::setString(filepath,wfilepath.c_str());		

		path=vs[0];
		path+="\n";
		path+=filepath;
		path+="\\..\\";
		Book* book = xlCreateBook();
		book->setKey(L"Halil Kural", L"windows-2723210a07c4e90162b26966a8jcdboe");



		if(book) 
		{                
			wstring wstr_temp;
			Sheet* sheet = book->addSheet(L"Data");
			if(sheet)
			{
				for (int i=0;i<value_string.size();i++)
				{
					for (int j=0;j<value_string[i].size();j++)
					{
						UtilTool::setWstring(wstr_temp,value_string[i][j].c_str());
						sheet->writeStr(i,j,wstr_temp.c_str());
					}
				}
			}
			book->save(wfilepath.c_str()); 
		}

		return ;

	}

	static inline void SaveExcel(CListUI * pList,string filepath)
	{
		wstring wfilepath;wstring wfoldpath;
		string configpath=(const char *)UtilTool::getPath();
		configpath+="path.txt";
		string path=(char *)UtilTool::readfile(configpath.c_str());
		vector<string> vs=UtilTool::split_string_multi_row((char *)path.c_str(),'\n');
		UtilTool::setWstring(wfoldpath,vs[1].c_str());
		UtilTool::setWstring(wfilepath,filepath.c_str());
		string bcdpath;

		if (UtilTool::GetSaveFilePath(wfilepath,wfoldpath,L"")==NULL)
		{
			return;
		}	
		UtilTool::setString(filepath,wfilepath.c_str());	

		path=vs[0];
		path+="\n";
		path+=filepath;
		path+="\\..\\";
		Book* book = xlCreateBook();
		book->setKey(L"Halil Kural", L"windows-2723210a07c4e90162b26966a8jcdboe");
		if(book->addSheet(L"data"))
			if(book) 
			{                
				wstring wstr_temp;
				Sheet* sheet = book->getSheet(0);
				if (sheet==NULL)
				{
					return;
				}
				int col=pList->GetHeader()->GetCount();
				for (int i=0;i<col;i++)
					sheet->writeStr(0,i,pList->GetHeader()->GetItemAt(i)->GetText().GetData());
				for (int i=0;i<pList->GetCount();i++)
				{
					CControlUI * pItem=pList->GetItemAt(i);
					/*	if (pItem->GetClass()==L"ListTextElementUI")
					{	*/
						CListTextElementUI * pCListTextElementUI=(CListTextElementUI*)pItem;
						for (int j=0;j<col;j++)
							sheet->writeStr(i+1,j,pCListTextElementUI->GetText(j));
				/*	}else{
						CListContainerElementUI * pCListContainerElementUI=(CListContainerElementUI*)pItem;
						for (int j=0;j<col;j++)
							sheet->writeStr(i+1,j,pCListContainerElementUI->GetItemAt(j)->GetText());
					}*/
				}
				book->save(wfilepath.c_str()); 
			}
			return ;
	}

		static vector<string>  readfiles(const char * filepath){
		
		char szData[1000]={0};
		vector<string> retData;
		FILE *fp=fopen(filepath,"rt");
		while(!feof(fp))
		{
			memset(szData,0,1000);
			fgets(szData,sizeof(szData)-1,fp);
			retData.push_back(szData);
		}
		fclose(fp);
		return retData;
		/*fseek(fp,0L,SEEK_END);  
		int size=ftell(fp);  
		unsigned char * data=new unsigned char[size];
		memset(data,0,size);
		fseek(fp, 0, SEEK_SET);
		size=fread(data,sizeof(char),size,fp);
		fclose(fp);
		if (size)
		{	
			return data;
		}
		return NULL;*/
	}

		static void StrToHex(BYTE *pbDest, BYTE *pbSrc, int nLen)
		{
			char h1,h2;
			BYTE s1,s2;
			int i;

			for (i=0; i<nLen; i++)
			{
				h1 = pbSrc[2*i];
				h2 = pbSrc[2*i+1];

				s1 = toupper(h1) - 0x30;
				if (s1 > 9) 
					s1 -= 7;

				s2 = toupper(h2) - 0x30;
				if (s2 > 9) 
					s2 -= 7;

				pbDest[i] = s1*16 + s2;
			}
		}
	static inline	vector<unsigned int> LoadFilterConfig(const string& path)
	{
		Book* book = xlCreateBook();
		vector<unsigned int> parminfo_vector;
		book->setKey(L"Halil Kural", L"windows-2723210a07c4e90162b26966a8jcdboe");
		string content_string;
		if(book)
		{
			if(book->load(UtilTool::charToWchar(path.c_str())))
			{
				for (size_t cnt = 0;  cnt < book->sheetCount(); ++cnt) {
					Sheet* sheet = book->getSheet(cnt);
					int rows = sheet->lastRow();
					int cols = sheet->lastCol();

					//设备名称	包序号	RT地址	RT子地址	数据域长度(字节)
					//	7V加热器	18	5	9	124		

					for(int i = 0; i < rows; ++i) {
						for (int j=0;j<cols;j++)
						{
							std::string string_value;
							if(sheet){
								libxl::CellType celltype = sheet->cellType(i,j);
								switch(celltype)
								{
								case CELLTYPE_NUMBER:
									{
										double intmy =sheet->readNum(i,j);
										//		cellcontent.double_value=intmy;
										char csNum[200]={};
										if(abs(intmy-(int)intmy)<0.00000000000000001)
											sprintf(csNum,"%d",(int)intmy);
										else
											sprintf(csNum,"%g",intmy);
										string_value=csNum;
										break;
									}
								case CELLTYPE_STRING:
									{
										const wchar_t * t = sheet->readStr(i,j);
										if (t)
										{
											UtilTool::setString(string_value,t);
											break;
										}
									}
								default:

									break;
								}
							}        

							unsigned int value;
							sscanf(string_value.c_str(),"%x",&value);
							parminfo_vector.push_back(value);
						}
					}
				}
			}
			else
			{
				//			std::cout << book->errorMessage() << std::endl;
			}
		}
		book->release();
		return parminfo_vector;
	}

	static inline void SaveExcel(vector<unsigned int> filter_flag,string filepath)
	{
		wstring wfilepath;wstring wfoldpath;
		string configpath=(const char *)UtilTool::getPath();
		configpath+="path.txt";
		string path=(char *)UtilTool::readfile(configpath.c_str());
		vector<string> vs=UtilTool::split_string_multi_row((char *)path.c_str(),'\n');
		UtilTool::setWstring(wfoldpath,vs[1].c_str());
		UtilTool::setWstring(wfilepath,filepath.c_str());
		string bcdpath;

		if (UtilTool::GetSaveFilePath(wfilepath,wfoldpath,L"")==NULL)
		{
			return;
		}	
		UtilTool::setString(filepath,wfilepath.c_str());	

		path=vs[0];
		path+="\n";
		path+=filepath;
		path+="\\..\\";
		Book* book = xlCreateBook();
		book->setKey(L"Halil Kural", L"windows-2723210a07c4e90162b26966a8jcdboe");
		if(book->addSheet(L"data"))
			if(book) 
			{                
				wstring wstr_temp;
				Sheet* sheet = book->getSheet(0);
				if (sheet==NULL)
				{
					return;
				}
				for (int i=0;i<filter_flag.size();i++)
				{
					char buf[100]={0};
					sprintf(buf,"%x",filter_flag[i]);
					wstring value;
					UtilTool::setWstring(value,buf);
					sheet->writeStr(0,i,value.c_str());
				}
			}
			book->save(wfilepath.c_str()); 
			return ;
	}
	static inline void SaveExcel(vector<map<string,string>> & value_string,vector<string> & value_index,string template_path,string filepath,bool bool_direct=false)
	{
		wstring wfilepath;wstring wfoldpath;
		string configpath=(const char *)UtilTool::getPath();
		configpath+="path.txt";
		string path=(char *)UtilTool::readfile(configpath.c_str());
		vector<string> vs=UtilTool::split_string_multi_row((char *)path.c_str(),'\n');
		UtilTool::setWstring(wfoldpath,vs[1].c_str());
		UtilTool::setWstring(wfilepath,filepath.c_str());
		string bcdpath;
		if (!bool_direct)
		{
			if (UtilTool::GetSaveFilePath(wfilepath,wfoldpath,L"")==NULL)
			{
				return;
			}	
			UtilTool::setString(filepath,wfilepath.c_str());	
		}
		path=vs[0];
		path+="\n";
		path+=filepath;
		path+="\\..\\";

		wstring file_path_wstring;
		template_path=UtilTool::getPath()+template_path;
		UtilTool::setWstring(file_path_wstring,template_path.c_str());

		Book* book = xlCreateBook();
		book->setKey(L"Halil Kural", L"windows-2723210a07c4e90162b26966a8jcdboe");

		if(book->load(file_path_wstring.c_str()))

			if(book) 
			{                
				wstring wstr_temp;
				Sheet* sheet = book->getSheet(0);
				if(sheet)
				{
					for (int i=0;i<value_string.size();i++)
					{
						for (int j=0;j<value_index.size();j++)
						{
							UtilTool::setWstring(wstr_temp,value_string[i][value_index[j]].c_str());
							sheet->writeStr(i+1,j,wstr_temp.c_str());
						}
					}
				}
				book->save(wfilepath.c_str()); 
			}

			return ;

	}

	static char* wcharToChar( const wchar_t* szWidechar )
	{   
		ASSERT(szWidechar != NULL);

		int widecharLen = ::WideCharToMultiByte(CP_ACP, 0, szWidechar, -1, NULL, 0, NULL, NULL);
		char* pAnsi = new char[widecharLen+1];
		memset(pAnsi, 0x00, widecharLen+1);
		::WideCharToMultiByte(CP_ACP, 0, szWidechar, -1, pAnsi, widecharLen, NULL, NULL);

		return pAnsi;
	}

	static	wchar_t* charToWchar(const char *str)
	{
		int length = strlen(str)+1;
		wchar_t *t = (wchar_t*)malloc(sizeof(wchar_t)*length);
		memset(t,0,length*sizeof(wchar_t));
		MultiByteToWideChar(CP_ACP,0,str,strlen(str),t,length);
		return t;
	}

	static string getFileText(const char * filepath){
		ASSERT(filepath);
		string resultstr;
		string str;
		FILE * fp=NULL;
		if(fp=fopen(filepath,"r")) 
		{
			//OutputDebugString(_T("打开模板文件成功")); 
		}
		else 
		{
			OutputDebugString(_T("打开模板文件成败"));
			return str;
		}
		for (;;)
		{
			char temp[100]={0};
			char * pdata=fgets(temp,100,fp);
			if (pdata)
			{	
				resultstr+=pdata;
				str+='\n';
			}
			else
				break;
		}
		fclose(fp);
		return resultstr;
	}

	static Func GetFunc(string & dll_name,string & function_name){
		wstring wstring_dll_name;
		wstring wstring_funtion_name;
		UtilTool::setWstring(wstring_dll_name,dll_name.c_str());
		UtilTool::setWstring(wstring_funtion_name,function_name.c_str());
		HMODULE hmod = LoadLibrary(wstring_dll_name.c_str());
		if (hmod) {
			return  (Func)GetProcAddress(hmod,(LPCSTR)wstring_funtion_name.c_str());
		}
		else 
			return NULL;
	}

	static vector<string>  split_string(const char *s,char spilt_char=' ')
	{
		vector<string> stringarrary;
		const	char *p = s;
		int i = 0;
		int j = 0;
		string strtemp;
		while(*p != '\n'&&*p != 0)
		{
			if(*p == '0')
			{
				strtemp += *p;
				p++;
				if (*p == '0')
				{
					strtemp += *p;
					p++;
					///*处理多个空格的情况*/
					while(*p == spilt_char)
					{
						p++;
					}

				  if (*p == '8')
				  {
					  strtemp += *p;
					  p++;
					  if (*p == '7')
					  {
						   strtemp += *p;
						   p++;
						   for (int c=0;c<(62*2);c++)
						   {
								///*处理多个空格的情况*/
								while(*p == spilt_char)
								{
									p++;
								}
							    strtemp += *p;
								p++;
						   }
						   stringarrary.push_back(strtemp);
						   break;
					  }
					  else 	if(*p == '8')
					  {
						   strtemp += *p;
						   p++;
						   for (int c=0;c<(126*2);c++)
						   {
							///*处理多个空格的情况*/
								while(*p == spilt_char)
								{
									p++;
								}
							    strtemp += *p;
								p++;
						   }
						   stringarrary.push_back(strtemp);
						   break;

					  }else{
						  strtemp="";
					  }
				  }else
				  {
					  strtemp="";
				  }
				}else
				{
					strtemp="";
				}
			}
			else
			{
				p++;
				j++;
			}
		}
		return stringarrary;
	}

	static vector<string>  split_strings(const char *s,char spilt_char=' ')
	{
		vector<string> stringarrary;
		const	char *p = s;
		int i = 0;
		int j = 0;
		string strtemp;
		while(*p != '\n'&&*p != 0)
		{
			if(*p == spilt_char)
			{
				strtemp += *p;
				p++;
				if (*p == 'A')
				{
					strtemp += *p;
					p++;
				  if (*p == 'E')
				  {
					  strtemp += *p;
					  p++;
					  if (*p == '2')
					  {
						   strtemp += *p;
						   p++;
						   for(i=0;i<4;i++)
							{
							   strtemp += *p;
							   p++;
						    }
						   if((*p == '0'))
						   {
							  strtemp += *p;
							  p++;
							   if((*p == '0'))
							   {
									strtemp += *p;
									p++;
								   if((*p == 'C'))
								   {
										strtemp += *p;
										p++;
									   if((*p == '1'))
									   {
											strtemp += *p;
											p++;
										   for (int c=0;c<(194*2);c++)
										   {
												strtemp += *p;
												p++;
										   }
										   stringarrary.push_back(strtemp);
										   break;
									   }
									   else
									   {
											strtemp="";
									    }
								   }
								   else
								   {
										strtemp="";
								   }

							   }
							   else
							   {
								    strtemp="";
							   }
						   }
						   else
						   {
								 strtemp="";
						   }
					  }else{
						  strtemp="";
						  //p++;---yww 此处不应该p++，判断该字符不是当前所需，但无法确定是否为重新开始，故应该从该字符开始重新判断
					  }
				  }else
				  {
					  strtemp="";
					  //p++;
				  }
				}else
				{
					strtemp="";
					//p++;
				}



				//stringarrary.push_back(strtemp);
				//strtemp="";
				//i++;
				//j = 0;
				//p++;
				///*处理多个空格的情况*/
				//while(*p == spilt_char)
				//{
				//	p++;
				//}
			}
			else
			{
				/*strtemp += *p;
*/
				p++;
				j++;
			}
		}
		/*if (strtemp.size())
		{
			stringarrary.push_back(strtemp);
		}*/
		return stringarrary;
	}

	static vector<string>  split_stringss(const char *s,char spilt_char=' ')
	{
		vector<string> stringarrary;
		const	char *p = s;

		int i = 0;
		int j = 0;
		string strtemp;
		while(*p != '\n'&&*p != 0)
		{
			strtemp += *p;
			if(*p == ' ')
			{
				stringarrary.push_back(strtemp.c_str());
				strtemp="";
				p++;
				//p++;
			}
			else
			{
				p++;
				j++;
			}
		}
		return stringarrary;
	}


	static vector<string>  get_datatime(const char *s)
	{
		vector<string> stringarrary;
		const	char *p = s;
		int i = 0;
		int j = 0;
		string strtemp;
		while(*p != '\n'&&*p != 0)
		{
			for (int c=0;c<19;c++)
			{
				strtemp += *p;
				p++;
			}
			stringarrary.push_back(strtemp);
			break;
		}
		return stringarrary;
	}

	static vector<string>  split_string_single_spilt(char *s,char spilt_char=' ')
	{
		vector<string> stringarrary;
		char *p = s;
		int i = 0;
		int j = 0;
		string strtemp;
		while(*p != '\n'&&*p != 0)
		{
			if(*p == spilt_char)
			{	

				stringarrary.push_back(strtemp);
				strtemp.clear();
				i++;
				j = 0;
				p++;
				///*处理多个空格的情况*/
				//while(*p == ' ')
				//{
				//	p++;
				//}
			}
			else
			{
				strtemp += *p;

				p++;
				j++;
			}
		}
		if (strtemp.size())
		{
			stringarrary.push_back(strtemp);
		}
		return stringarrary;
	}

	static map<string,string> *getdata(TCHAR *  file){
		ASSERT(file);
		char * filepath=UtilTool::wcharToChar(file);
		FILE *fp; map<string,string> * pdatamap=NULL;
		if(fp=fopen(filepath,"r")) 
		{
			pdatamap=new map<string,string>();
			OutputDebugString(_T("打开文件成功")); 
		}
		else 
			OutputDebugString(_T("打开文件成败"));
		for (;;)
		{
			char temp[100]={0};
			char * pdata=fgets(temp,100,fp);
			if (pdata)
			{	
				std::string str=pdata;
				if (str.size())
				{
					int i=	str.find(':');
					std::string name=str.substr(0,i);
					std::string value=str.substr(i+1);
					if (name.size())
					{
						(*pdatamap)[name]=value;
					}
				}
			}
			else
				break;
		}
		fclose(fp);
		delete filepath;
		return pdatamap;
	}

	static string intToChar(int val){
		char buf[20];
		sprintf(buf,"%d",val);
		return buf;
	}

	static void setString(string & str,const wchar_t* szWidechar){
		char * chaptr=UtilTool::wcharToChar(szWidechar);
		str =chaptr;
		free(chaptr);
	}

	static string getString(const wchar_t* szWidechar){
		ASSERT(szWidechar);
		char * chaptr=UtilTool::wcharToChar(szWidechar);
		string  str =chaptr;
		free(chaptr); 
		return str;
	}

	static wstring getWString(const char * str){
		ASSERT(str);
		wchar_t * wstrbuf=UtilTool::charToWchar(str);
		wstring wstr=wstrbuf;
		free(wstrbuf);
		return wstr;
	}

	static void setWstring(wstring & wstr,const char * str){
		wchar_t * wstrbuf=UtilTool::charToWchar(str);
		wstr=wstrbuf;
		free(wstrbuf);
	}


	static BOOL KillProcessFromName(LPCSTR lpProcessName){
		HANDLE hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
		PROCESSENTRY32 pe;
		pe.dwSize = sizeof(PROCESSENTRY32);
		if(!Process32First(hSnapShot,&pe))
		{
			return FALSE;
		}
		string strProcessName = lpProcessName;
		// strProcessName.MakeLower();
		while (Process32Next(hSnapShot,&pe))
		{
			string scTmp ;UtilTool::setString(scTmp,pe.szExeFile);
			//  scTmp.MakeLower();
			if(scTmp==strProcessName)
			{
				DWORD dwProcessID = pe.th32ProcessID;
				HANDLE hProcess = ::OpenProcess(PROCESS_TERMINATE,FALSE,dwProcessID);
				::TerminateProcess(hProcess,0);
				CloseHandle(hProcess);
				return TRUE;
			}
			// scTmp.ReleaseBuffer();
		}
		//strProcessName.ReleaseBuffer();
		return FALSE;
	}

	static double getval(LPCSTR  strbuf){
		int val=0;
		double doubleval=0;
		wscanf(_T("0x%d"),strbuf,&val);
		if (val!=0)
		{
			doubleval=val;
		}else
			wscanf(_T("%lf"),strbuf,&doubleval);
		return doubleval;
	}

	static TCHAR * GetSaveFilePath(wstring & file_path,wstring & fold_path,const TCHAR * filter_string=L"dat files(*.dat)\0*.dat\0txt files(*.txt)\0*.txt\0",const TCHAR * title_string=NULL	)  
	{

		OPENFILENAME ofn;		
		ZeroMemory(&ofn,sizeof(ofn));
		TCHAR szFold[MAX_PATH*2]={0};
		wstring strPath,strText=L"";
		//	ofn.lpstrFile[0] = '\0'; 
		if (fold_path.size())
		{
			memcpy(szFold,fold_path.c_str(),fold_path.size());
			ofn.lpstrInitialDir=szFold;
		}
		TCHAR szFile[MAX_PATH*2]={0}; // 保存获取文件名称的缓冲区。       
		if (file_path.size())
		{
			memcpy(szFile,file_path.c_str(),file_path.size()*2);
		}


		ofn.lpstrFile=szFile;
		ofn.lStructSize=sizeof(ofn);
		ofn.hwndOwner=NULL;
		ofn.lpstrFilter=filter_string;
		ofn.lpstrCustomFilter=NULL;
		ofn.nFilterIndex=0;
		ofn.nMaxFile=128;
		ofn.lpstrFileTitle=NULL;
		ofn.lpstrTitle=L"保存文本文件\0";
		ofn.Flags=OFN_OVERWRITEPROMPT;
		ofn.lpstrDefExt=NULL;
		if(GetSaveFileName(&ofn))

		{  
			//显示选择的文件。   
			file_path=szFile;
			return szFile;  

		} else 
		{
			file_path=L"";
			return NULL;
		}

	}


	static TCHAR * GetOpenFilePath(wstring & filepath,const TCHAR * foldpath=NULL,const TCHAR * filter_string=L"")  {  
		//     
		OPENFILENAME ofn;      // 公共对话框结构。   

		TCHAR szFile[MAX_PATH*2]={0}; // 保存获取文件名称的缓冲区。       
		if (filepath.size())
		{
			memcpy(szFile,filepath.c_str(),filepath.size()*2);
		}

		// 初始化选择文件对话框。   
		ZeroMemory(&ofn, sizeof(OPENFILENAME));  
		ofn.lStructSize = sizeof(OPENFILENAME);  
		ofn.hwndOwner = NULL;  
		ofn.lpstrFile = szFile;  
		TCHAR szFold[MAX_PATH*2]={0};

		//	ofn.lpstrFile[0] = '\0'; 
		if (foldpath)
		{
			memcpy(szFold,foldpath,wcslen(szFold));
			ofn.lpstrInitialDir=szFold;
		}


		ofn.nMaxFile = sizeof(szFile);  
		ofn.lpstrFilter =filter_string;  
		ofn.nFilterIndex = 1;  
		ofn.lpstrFileTitle = NULL;  
		ofn.nMaxFileTitle = 0;  

		//	ofn.Flags = OFN_PATHMUSTEXIST/* | OFN_FILEMUSTEXIST*/;  

		//ofn.lpTemplateName =  MAKEINTRESOURCE(ID_TEMP_DIALOG);  
		// 显示打开选择文件对话框。   

		if ( GetOpenFileName(&ofn) )  
		{  
			//显示选择的文件。   
			filepath=szFile;
			return szFile;  

		} else 
		{
			filepath=L"";
			return NULL;
		}
	}
	static vector<wstring> GetOpenFilePaths_txt() {
		vector<wstring> szLJName;
		OPENFILENAME ofn;
		TCHAR szOpenFileNames[80 * MAX_PATH] = _T("");
		TCHAR szPath[MAX_PATH];
		TCHAR szFileName[80 * MAX_PATH];


		TCHAR* p;
		int nLen = 0;
		ZeroMemory(&ofn, sizeof(ofn));
		ofn.lStructSize = sizeof(ofn);
		ofn.hwndOwner = NULL;
		ofn.lpstrFile = szOpenFileNames;
		ofn.nMaxFile = sizeof(szOpenFileNames);
		ofn.lpstrFilter = _T("文本文件类型(*.*)\0*.txt*\0");
		ofn.nFilterIndex = 1;
		ofn.lpstrFileTitle = NULL;
		ofn.nMaxFileTitle = 0;
		ofn.lpstrInitialDir = NULL;
		ofn.Flags = OFN_EXPLORER | OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_ALLOWMULTISELECT;

		if (GetOpenFileName(&ofn))
		{
			//把第一个文件名前的复制到szPath,即:
				   //如果只选了一个文件,就复制到最后一个'/'
				   //如果选了多个文件,就复制到第一个NULL字符
			lstrcpyn(szPath, szOpenFileNames, ofn.nFileOffset);

			//当只选了一个文件时,下面这个NULL字符是必需的.
			 //这里不区别对待选了一个和多个文件的情况

			szPath[ofn.nFileOffset] = '\0';
			nLen = lstrlen(szPath);

			if (szPath[nLen - 1] != '\\')   //如果选了多个文件,则必须加上'\\'
			{
				lstrcat(szPath, TEXT("\\"));
			}


			p = szOpenFileNames + ofn.nFileOffset; //把指针移到第一个文件


			while (*p)
			{
				ZeroMemory(szFileName, sizeof(szFileName));
				lstrcat(szFileName, szPath);  //给文件名加上路径  
				lstrcat(szFileName, p);    //加上文件名    
				szLJName.push_back(szFileName);
				lstrcat(szFileName, TEXT("/n")); //换行 
				p += lstrlen(p) + 1;     //移至下一个文件
			}
			return szLJName;
		}
		else
		{

			return szLJName;
		}

	}

	static vector<wstring> GetOpenFilePaths()  {  
		 vector<wstring> szLJName;
		OPENFILENAME ofn;      
		TCHAR szOpenFileNames[80*MAX_PATH] = _T("");
	 TCHAR szPath[MAX_PATH];
		TCHAR szFileName[80*MAX_PATH];

		
	    TCHAR* p;
		    int nLen = 0;
		   ZeroMemory(&ofn, sizeof(ofn));  
		    ofn.lStructSize = sizeof(ofn);
		    ofn.hwndOwner   = NULL;
		    ofn.lpstrFile   = szOpenFileNames;   
		    ofn.nMaxFile    = sizeof(szOpenFileNames);  
		   ofn.lpstrFilter = _T("自设文件类型(*.*)\0*.*\0所有文件(*.*)\0*.*\0\0") ;
		    ofn.nFilterIndex    = 1;  
		    ofn.lpstrFileTitle  = NULL;  
		     ofn.nMaxFileTitle   = 0;  
		    ofn.lpstrInitialDir = NULL;  
		    ofn.Flags = OFN_EXPLORER | OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST |OFN_ALLOWMULTISELECT;

			if( GetOpenFileName( &ofn ) )
			 {  
				 //把第一个文件名前的复制到szPath,即:
				        //如果只选了一个文件,就复制到最后一个'/'
					    //如果选了多个文件,就复制到第一个NULL字符
					   lstrcpyn(szPath, szOpenFileNames, ofn.nFileOffset );
				
					      //当只选了一个文件时,下面这个NULL字符是必需的.
					       //这里不区别对待选了一个和多个文件的情况
					
					        szPath[ ofn.nFileOffset ] = '\0';
				      nLen = lstrlen(szPath);
				 
					        if( szPath[nLen-1] != '\\' )   //如果选了多个文件,则必须加上'\\'
					        {
					           lstrcat(szPath, TEXT("\\"));
						        }
				
					
					       p = szOpenFileNames + ofn.nFileOffset; //把指针移到第一个文件
				
					 
					         while( *p )
					          { 
						            ZeroMemory(szFileName, sizeof(szFileName));
						             lstrcat(szFileName, szPath);  //给文件名加上路径  
						             lstrcat(szFileName, p);    //加上文件名    
									 szLJName.push_back(szFileName);
						            lstrcat(szFileName, TEXT("/n")); //换行 
						           p += lstrlen(p) +1;     //移至下一个文件
						       }
							 return szLJName;
			}else 
			{
				
				return szLJName;
			}
			
	} 

	static vector<wstring> GetOpenFilePaths_xls() {
		vector<wstring> szLJName;
		OPENFILENAME ofn;
		TCHAR szOpenFileNames[80 * MAX_PATH] = _T("");
		TCHAR szPath[MAX_PATH];
		TCHAR szFileName[80 * MAX_PATH];


		TCHAR* p;
		int nLen = 0;
		ZeroMemory(&ofn, sizeof(ofn));
		ofn.lStructSize = sizeof(ofn);
		ofn.hwndOwner = NULL;
		ofn.lpstrFile = szOpenFileNames;
		ofn.nMaxFile = sizeof(szOpenFileNames);
		ofn.lpstrFilter = _T("XLS类型(*.xls*)\0*.*\0XLSX类型(*.xlsx*)\0*.*\0\0");
		ofn.nFilterIndex = 1;
		ofn.lpstrFileTitle = NULL;
		ofn.nMaxFileTitle = 0;
		ofn.lpstrInitialDir = NULL;
		ofn.Flags = OFN_EXPLORER | OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_ALLOWMULTISELECT;

		if (GetOpenFileName(&ofn))
		{
			//把第一个文件名前的复制到szPath,即:
				   //如果只选了一个文件,就复制到最后一个'/'
				   //如果选了多个文件,就复制到第一个NULL字符
			lstrcpyn(szPath, szOpenFileNames, ofn.nFileOffset);

			//当只选了一个文件时,下面这个NULL字符是必需的.
			 //这里不区别对待选了一个和多个文件的情况

			szPath[ofn.nFileOffset] = '\0';
			nLen = lstrlen(szPath);

			if (szPath[nLen - 1] != '\\')   //如果选了多个文件,则必须加上'\\'
			{
				lstrcat(szPath, TEXT("\\"));
			}


			p = szOpenFileNames + ofn.nFileOffset; //把指针移到第一个文件


			while (*p)
			{
				ZeroMemory(szFileName, sizeof(szFileName));
				lstrcat(szFileName, szPath);  //给文件名加上路径  
				lstrcat(szFileName, p);    //加上文件名    
				szLJName.push_back(szFileName);
				lstrcat(szFileName, TEXT("/n")); //换行 
				p += lstrlen(p) + 1;     //移至下一个文件
			}
			return szLJName;
		}
		else
		{

			return szLJName;
		}

	}

	static string GetTimeStringDisplay(){
		SYSTEMTIME st;
		GetLocalTime(&st);
		char temp[100];
		sprintf_s(temp,"%04d-%02d-%02d %02d:%02d:%02d.%03d",st.wYear,st.wMonth,st.wDay,st.wHour,st.wMinute,st.wSecond,st.wMilliseconds);
		string str=temp;
		return str;
	}

	string get_cur_timelogdata()/*获取当前时间字符串*/
	{
		string time_string;
		SYSTEMTIME stime;
		char buf[100]={0};
		GetLocalTime(&stime);
		sprintf(buf,"%04d.%02d.%02d.%02d.%02d.%02d",stime.wYear,stime.wMonth,stime.wDay,stime.wHour,stime.wMinute,stime.wSecond);
		time_string+=buf;
		return time_string;
	}

	static string GetTimeString(){
		SYSTEMTIME st;
		GetLocalTime(&st);
		char temp[100];
		sprintf_s(temp,"%04d-%02d-%02d-%02d-%02d-%02d-%03d",st.wYear,st.wMonth,st.wDay,st.wHour,st.wMinute,st.wSecond,st.wMilliseconds);
		string str=temp;
		return str;
	}
	static string&   replace_all(string&   str,const   string&   old_value,const   string&   new_value)   
	{   
		while(true)   {   
			string::size_type   pos(0);   
			if(   (pos=str.find(old_value))!=string::npos   )   
				str.replace(pos,old_value.length(),new_value);   
			else   break;   
		}   
		return   str;   
	}   
	static	string&   replace_all_distinct(string&   str,const   string&   old_value,const   string&   new_value)   
	{   
		for(string::size_type   pos(0);   pos!=string::npos;   pos+=new_value.length())   {   
			if(   (pos=str.find(old_value,pos))!=string::npos   )   
				str.replace(pos,old_value.length(),new_value);   
			else   break;   
		}   
		return   str;   
	}   
	static char * radixtran(char  * result,unsigned int val,int radix,int add_suffix=1)
	{
		if (radix==0)
		{
			return result;
		}
		char ku[16]={'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
		int zh[32]={0},i=0,j;
		do
		{
			zh[i]=val%radix;
			i++;
			val=val/radix;
		}
		while(val!=0);
		int index=0;
		//char result[32]={0};
		result[i]=0;

		for(i--;i>=0;i--)
		{
			j=zh[i];
			result[index++]=ku[j];
		}
		if (add_suffix)
			switch (radix)
		{
			case 2:
				//memcpy(result,"BIN:",4);	
				//result[i+4]=0;
				result[index]='b';
				break;		
			case 8:
				//memcpy(result,"OCT:",4);
				result[index]='o';
				break;		
			case 16:
				//	memcpy(result,"0x",4);
				//	result[i+2]=0;
				result[index]='h';
				//	index=2;
				break;
			default:
				//memcpy(result,"DEC:",4);
				break;
		}
		return result;
	}

	/*“rt”　　　　　　只读打开一个文本文件，只允许读数据 
	“wt”　　　　　　只写打开或建立一个文本文件，只允许写数据
	“at”　　　　　　追加打开一个文本文件，并在文件末尾写数据
	“rb”　　　　　　只读打开一个二进制文件，只允许读数据
	“wb”　　　　 　 只写打开或建立一个二进制文件，只允许写数据
	“ab” 　　　　 　追加打开一个二进制文件，并在文件末尾写数据
	“rt+”　　　　　 读写打开一个文本文件，允许读和写
	“wt+”　　　　　 读写打开或建立一个文本文件，允许读写
	“at+”　　　　　 读写打开一个文本文件，允许读，或在文件末追加数 据
	“rb+”　　　　　 读写打开一个二进制文件，允许读和写 
	“wb+”　　　　　 读写打开或建立一个二进制文件，允许读和写
	“ab+” 　　　　　读写打开一个二进制文件，允许读，或在文件末追加数据*/

	static int writebin(const char * filepath,void * data ,int len){
		FILE *fp;
		//char ch;
		if((fp=fopen(filepath,"wb"))==NULL)
		{
			return 0;
		}
		fwrite(data,len,1,fp);
		fclose(fp);
		return 0;
	}
	static string GetFullPath(const char * path){
		string full_path;
		if (path)
		{	
			full_path=UtilTool::getPath();
			full_path+=path;
		}
		return full_path;
	}
	static int writefile(const char * filepath,void * data ,int len){
		FILE *fp;
		//char ch;
		if((fp=fopen(filepath,"wt"))==NULL)
		{
			return 0;
		}
		fwrite(data,len,1,fp);
		fclose(fp);
	}
	static vector<string>  split_string_multi_row(char *s,char spilt_char=' ')
	{
		vector<string> stringarrary;
		char *p = s;
		int i = 0;
		int j = 0;
		string strtemp;
		while(*p != 0)
		{
			if(*p == spilt_char)
			{	

				stringarrary.push_back(strtemp);
				strtemp.clear();
				i++;
				j = 0;
				p++;
				//	*处理多个空格的情况*/
				while(*p == spilt_char)
				{
					p++;
				}
			}
			else
			{
				strtemp += *p;

				p++;
				j++;
			}
		}
		if (strtemp.size())
		{
			stringarrary.push_back(strtemp);
		}
		return stringarrary;
	}

	static string StringTm()
	{
		SYSTEMTIME st;
		GetLocalTime(&st);
		char temp[100];
		sprintf_s(temp,"_%04d%02d%02d%02d",st.wYear,st.wMonth,st.wDay,st.wHour);
		string str=temp;
		return str;

	}
	static string StringTmYM()
	{
		SYSTEMTIME st;
		GetLocalTime(&st);
		char temp[100];
		sprintf_s(temp,"%04d%02d",st.wYear,st.wMonth);
		string str=temp;
		return str;
	}


	static int addfile(const char * filepath,void * data ,int len){
		FILE *fp;
		char ch;
		if((fp=fopen(filepath,"at"))==NULL)
		{
			return 0;
		}
		fwrite(data,len,1,fp);
		fclose(fp);
	}
	static void writeData(string paths,string source,string datas)
	{
		string writstr;
		string s2=paths;
		string Tmpath=StringTm();
		Tmpath+=".txt";
		s2+=StringTmYM();
		LPCWSTR ss22=stringtoLPCWSTR(s2);
		if(!PathIsDirectory(ss22))
		{
			SHCreateDirectoryEx(NULL,(ss22),NULL);
		}
		s2+="\\";
		s2+=Tmpath;
		writstr+=GetTimeString();
		writstr+=" ";
		writstr+=source;
		writstr+=" ";
		writstr+=datas;
		writstr+=" ";
		writstr+="\n";
		addfile(s2.c_str(),(void *)writstr.c_str(),writstr.length());
	}

	static LPCWSTR stringtoLPCWSTR(string orig)
	{
		size_t prigsize=orig.length()+1;
		const size_t newseize=100;
		size_t convertedChars=0;
		wchar_t *wcstring=(wchar_t*)malloc(sizeof(wchar_t)*(orig.length()-1));
		mbstowcs_s(&convertedChars,wcstring,prigsize,orig.c_str(),_TRUNCATE);
		return wcstring;
	}



	static unsigned char * readfile(const char * filepath){
		FILE *fp;
		if((fp=fopen(filepath,"rt"))==NULL)
		{
			return NULL;
		}
		fseek(fp,0L,SEEK_END);  
		int size=ftell(fp);  
		unsigned char * data=new unsigned char[size];
		memset(data,0,size);
		fseek(fp, 0, SEEK_SET);
		size=fread(data,sizeof(char),size,fp);
		fclose(fp);
		if (size)
		{	
			return data;
		}
		return NULL;
	}


	static unsigned char * readbin(const char * filepath){
		FILE *fp;
		if((fp=fopen(filepath,"rb"))==NULL)
		{
			return NULL;
		}
		fseek(fp,0L,SEEK_END);  
		int size=ftell(fp);  
		unsigned char * data=new unsigned char[size];
		memset(data,0,size);
		fseek(fp, 0, SEEK_SET);
		size=fread(data,sizeof(char),size,fp);
		fclose(fp);
		if (size)
		{	
			return data;
		}
		return NULL;
	}


	static  vector<vector<string>> readfiledata(char c=' '){
		wstring wfilepath; vector<vector<string>> strdata;
		if (UtilTool::GetOpenFilePath(wfilepath))
		{
			string filepath;UtilTool::setString(filepath,wfilepath.c_str());
			return readblank(filepath.c_str(),c);
		}
		return strdata;
	}


	static vector<vector<string>> readblank(const char * filepath,char c=' ',bool single=false){
		FILE *fp;
		//char ch;
		vector<vector<string>> pblankary;
		if((fp=fopen(filepath,"rt"))==NULL)
		{
			return pblankary;
		}	
		char * buf=NULL;;
		string str;
		for (;;)
		{	
			char data[5000]={0};
			buf=fgets(data,5000,fp);

			if (buf)
			{
				if (single)
					pblankary.push_back(split_string_single_spilt(data,c));
				else 
					pblankary.push_back(split_string(data,c));
			}else
				break;
		}
		fclose(fp);
		return pblankary;
	}
};

struct CustomListItem
{
	wstring index;
	wstring id;
	wstring name;

	wstring value;
	wstring unit;
	wstring source;
	wstring defultvalue;

	wstring markname;
	CustomListItem() 
	{ 
		index = _T("");
		id = _T("");
		name = _T("");

		value = _T("");
		unit = _T("");
		source = _T("");	
		defultvalue = _T("");
	}
	~CustomListItem()
	{
	}
};

class accurateTimmer {
	LARGE_INTEGER nFreq;
	LARGE_INTEGER nBeginTime;
	LARGE_INTEGER nEndTime;
	double time;
public:
	accurateTimmer()
	{
		DWORD dwCPUid = SetThreadAffinityMask(GetCurrentThread(), 1 << 3);
		QueryPerformanceFrequency(&nFreq);
		SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_HIGHEST);
	}

	inline double Sleep(double Span) {
		for (;;)
		{
			time = 0;
			QueryPerformanceCounter(&nBeginTime);
			while (time<Span) {
				QueryPerformanceCounter(&nEndTime);
				time = (double)(nEndTime.QuadPart - nBeginTime.QuadPart) / (double)nFreq.QuadPart;
			}
			nBeginTime = nEndTime;
			return time;
		}
	}

	inline void Start() {
		QueryPerformanceCounter(&nBeginTime);
	}

	inline double End() {
		QueryPerformanceCounter(&nEndTime);
		time = (double)(nEndTime.QuadPart - nBeginTime.QuadPart) / (double)nFreq.QuadPart;
		return time;
	}

	inline void PrintTime(string header){
		End();
		char buf[100]={0};
		sprintf_s(buf,"                                                       消耗时间:%f\n",time);
		string temp;
		temp=buf;
		header+=temp;
		wchar_t * wbuf= UtilTool::charToWchar(header.c_str());
		OutputDebugString(wbuf);
		free(wbuf);
		Start();
	}
};

class CLock{
	CRITICAL_SECTION m_cs;
public:
	CLock(){
		InitializeCriticalSection(&m_cs);
	}
	~CLock(){
		DeleteCriticalSection(&m_cs);
	}

	inline void Lock(){
		EnterCriticalSection(&m_cs);
	}

	inline void UnLock(){
		LeaveCriticalSection(&m_cs);
	}
};

#pragma  once
#pragma  pack (1)
struct DynamicOutData
{
	int			STKShow;        // STK软件显示还是关闭
	int			DeviceNumber;	//器件的个数
	double		QZZ_DYNOUT_01;	// 仿真时间
	double		DeviceX_01;		// 器件1 X
	double		DeviceY_01;		// 器件1 Y
	double		DeviceZ_01;		// 器件1 Z
	double		DeviceX_02;		// 器件2 X
	double		DeviceY_02;		// 器件2 Y
	double		DeviceZ_02;		// 器件2 Z
	double		DeviceX_03;		// 器件3 X
	double		DeviceY_03;		// 器件3 Y
	double		DeviceZ_03;		// 器件3 Z
};

#pragma  pack ();