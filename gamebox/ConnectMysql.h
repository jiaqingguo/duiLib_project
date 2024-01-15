#pragma once
#include <mysql.h>
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
private:
	HANDLE m_mysqlHdl;
	
	MYSQL_RES *res; 
	MYSQL m_mydata;

	int m_ColumnLenght;//把数据表列数放在这里
};

