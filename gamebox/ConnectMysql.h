#pragma once
#include <mysql.h>
#include "DatabaseStruct.h"
#include <list>


struct st_mysql_res;

class ConnectMysql
{
public:
	ConnectMysql(void);
	~ConnectMysql(void);
	string getCurrentTime();//获取当前时间
	bool InitDatabase();//初始化数据库
	bool StartConnectMysql();//连接数据库
	bool CreateNewTable(string name);//创建新的表
	bool CreateNewTable(string name,vector<string> vec);//给了列名称创建新表
	//向数据表中增加一条数据
	void addMsgToTable(string name, string x, string y, string z, string gd, string fy, string ph, string dof7, string dof8, string dof9);
	void addMsgToTable(string name,vector<string> vec);//向数据表中添加数据

	bool showAllTable(string name_Table);//查询表格
	void closeDatabase();
	bool showAllTableName();//显示数据库中所有表名称
	bool SetUpDatabase(string database);
	//vector<vector<string>> mytest_QueryDatabase(string dataName,string name_Table);//把name_Table里面的多有数据以返回值传出来
	vector<vector<string>> mytest_QueryDatabase(string name_Table);//把name_Table里面的多有数据以返回值传出来
	string mytest_QueryDatabase_1(string name_Table);//把name_Table里面的多有数据以返回值传出来
	string mytest_showDbAllName();//返回数据库中所有表名称
	string mytest_QueryByTime(string start,string end,string name_Table);//返回某段时间内的数据
	vector<vector<string>> mytest_QueryByTime(string start, string end, string name_Table,vector<string> vec);//根据选择的列返回某段时间内的数据

	string mytest_QueryByColumnName(string name_Table,string columnName);//返回id,time,和指定列的数据
	vector<vector<string>> mytest_QueryByColumnName(string name_Table, string columnName,string data);//返回符合筛选列的数据
	vector<vector<string>> mytest_QueryByColumnName(string name_Table,vector<string> vec);//返回指定列

	vector<string> mytest_QueryColumns(string dbName, string name_Table);//获取数据表中列名称
	vector<string> mytest_QueryDatabase();//获取所有数据库名称

	bool CreateNewTableAndInsert(string name,vector<string> columnName,vector<string> columnValue);//创建新表并且添加一行数据
	int m_num;//行数




	//////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////


	MYSQL_RES* execSqlSelect(const std::string& sql);
	bool execSql(uint32_t& lastId, const std::string& sql);
	// 数据库启动事务;
	bool startupTransaction();
	// 数据库提交事务;
	bool commitTransaction();
	// 数据库回滚事务;
	bool rollbackTransaction();

	// 方案表;

	bool getAllScheme(std::list<stScheme> &listScheme);
	// 增加方案;
	bool addScheme(const std::string &schemeName);

	// 星座数据表;
	//bool getAllXzsjb(sstXZSJB& record_list);
	
	bool getAllXzsjb(std::list<stXZSJB>& listStXZSJB); // 获取所有数据
	bool getAllXzsjbBySchemeID(std::list<stXZSJB>& listStXZSJB,const int & schemeID);

	// 0_单星数据表_1
	bool getAllDxsjb(std::list<stDXSJB>& listStDXSJB);
	bool getAllDxsjbBySchemeID(std::list<stDXSJB>& listStDXSJB, const int& schemeID);

		//0_卫星天线_1
	bool getAllWXTXB(std::list<stWXTXB>& listStWXTXB);
	bool getAllWxtxbBySchemeID(std::list<stWXTXB>& listStWXTXB, const int& schemeID);

		//0_卫星载荷数据表_1
	bool getAllWXZHSJB(std::list<stWXZHSJB>& listStData);
	bool getAllWxzhsjbBySchemeID(std::list<stWXZHSJB>& listStData, const int& schemeID);

	// 1_电信港数据表_1
	bool getAllDxgsjbBySchemeID(std::list<stDXGSJB>& listStData, const int& schemeID);
		// 1_地球站数据表
	bool getAllDqzsjbBySchemeID(std::list<stDQZSJB>& listStData, const int& schemeID);
	// 1_地球站馈源数据表_1
	bool getAllDqzkysjbBySchemeID(std::list<stDQZKYSJB>& listStData, const int& schemeID);
		// 1_地球站波形设备数据表_1
	bool getAllDqzbxsbsjbBySchemeID(std::list<stDQZBXSBSJB>& listStData, const int& schemeID);
	// 1_1_地球站天线_1_1
	bool getAllDqztxbBySchemeID(std::list<stDQTXB>& listStData, const int& schemeID);
private:
	MYSQL* m_mysql; //句柄 先用m_mydata;
private:
	HANDLE m_mysqlHdl;
	
	MYSQL_RES *res; 
	//MYSQL m_mydata;

	int m_ColumnLenght;//把数据表列数放在这里
};

