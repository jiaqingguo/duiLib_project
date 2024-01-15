#pragma once
#include <mysql.h>
class ConnectMysql
{
public:
	ConnectMysql(void);
	~ConnectMysql(void);
	string getCurrentTime();//��ȡ��ǰʱ��
	bool InitDatabase();//��ʼ�����ݿ�
	bool StartConnectMysql();//�������ݿ�
	bool CreateNewTable(string name);//�����µı�
	bool CreateNewTable(string name,vector<string> vec);//���������ƴ����±�
	//�����ݱ�������һ������
	void addMsgToTable(string name, string x, string y, string z, string gd, string fy, string ph, string dof7, string dof8, string dof9);
	void addMsgToTable(string name,vector<string> vec);//�����ݱ����������

	bool showAllTable(string name_Table);//��ѯ���
	void closeDatabase();
	bool showAllTableName();//��ʾ���ݿ������б�����
	bool SetUpDatabase(string database);
	//vector<vector<string>> mytest_QueryDatabase(string dataName,string name_Table);//��name_Table����Ķ��������Է���ֵ������
	vector<vector<string>> mytest_QueryDatabase(string name_Table);//��name_Table����Ķ��������Է���ֵ������
	string mytest_QueryDatabase_1(string name_Table);//��name_Table����Ķ��������Է���ֵ������
	string mytest_showDbAllName();//�������ݿ������б�����
	string mytest_QueryByTime(string start,string end,string name_Table);//����ĳ��ʱ���ڵ�����
	vector<vector<string>> mytest_QueryByTime(string start, string end, string name_Table,vector<string> vec);//����ѡ����з���ĳ��ʱ���ڵ�����

	string mytest_QueryByColumnName(string name_Table,string columnName);//����id,time,��ָ���е�����
	vector<vector<string>> mytest_QueryByColumnName(string name_Table, string columnName,string data);//���ط���ɸѡ�е�����
	vector<vector<string>> mytest_QueryByColumnName(string name_Table,vector<string> vec);//����ָ����

	vector<string> mytest_QueryColumns(string dbName, string name_Table);//��ȡ���ݱ���������
	vector<string> mytest_QueryDatabase();//��ȡ�������ݿ�����

	bool CreateNewTableAndInsert(string name,vector<string> columnName,vector<string> columnValue);//�����±������һ������
	int m_num;//����
private:
	HANDLE m_mysqlHdl;
	
	MYSQL_RES *res; 
	MYSQL m_mydata;

	int m_ColumnLenght;//�����ݱ�������������
};

