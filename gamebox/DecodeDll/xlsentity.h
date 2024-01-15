#pragma once

#include <string>
#include "libxl.h"
#include "UtilTool.h"
#include <map>
#pragma comment (lib,"libxl.lib")

using namespace libxl;
struct CellContent{
	double double_value;
	std::string string_value;
	libxl::CellType cell_type;
	CellContent(){
		double_value=0;
	}
};
class XlsEntity{//Excel实体类
private: 
	std::map<std::string,int> sheet_index_map;
	bool load_ok;
	Book * book;
	Sheet * sheet; 
public:
	XlsEntity(const char * filepath){
		load_ok=false;
		book=NULL;
		book = xlCreateXMLBook();
			book->setKey(L"Halil Kural", L"windows-2723210a07c4e90162b26966a8jcdboe");
		std::wstring filepath_wtring;
		UtilTool::setWstring(filepath_wtring,filepath);
		if(book)
		{
			book->load(filepath_wtring.c_str());	
			for (int i=0;i<book->sheetCount();i++)
			{
				sheet = book->getSheet(i);
		    	const wchar_t * t=sheet->name();
				std::string sheet_name_string;
				UtilTool::setString(sheet_name_string,t);
				sheet_index_map[sheet_name_string]=i;
			}
		}
		sheet=NULL;
	}
	int GetRowCount(){
		if (sheet)
		{
			return sheet->lastRow();
		}
		return 0;
	}

	int GetColCount(){
		if (sheet)
		{
			return sheet->lastCol();
		}
		return 0;
	}
	void SetSheet(std::string sheet_name){
		if (sheet_index_map.find(sheet_name)==sheet_index_map.end())
		{
			sheet=NULL;
		}else
			sheet=book->getSheet(sheet_index_map[sheet_name]);
	}

	void SetSheet(int index){
		sheet=book->getSheet(index);
	}

	std::string ReadCellString(int row,int col)//读取单元格内容
	{
		std::string string_value;
		if(sheet){
			libxl::CellType celltype = sheet->cellType(row,col);
			switch(celltype)
			{
			case CELLTYPE_NUMBER:
				{
					double intmy =sheet->readNum(row,col);
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
					const wchar_t * t = sheet->readStr(row,col);
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
		return string_value;
	}
};