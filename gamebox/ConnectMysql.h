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




	//////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////


	MYSQL_RES* execSqlSelect(const std::string& sql);
	bool execSql(uint32_t& lastId, const std::string& sql);
	// ���ݿ���������;
	bool startupTransaction();
	// ���ݿ��ύ����;
	bool commitTransaction();
	// ���ݿ�ع�����;
	bool rollbackTransaction();

	// ������;

	bool getAllScheme(std::list<stScheme> &listScheme);
	// ���ӷ���;
	bool addScheme(const std::string &schemeName);

	// �������ݱ�;
	//bool getAllXzsjb(sstXZSJB& record_list);
	
	bool getAllXzsjb(std::list<stXZSJB>& listStXZSJB); // ��ȡ��������
	bool getAllXzsjbBySchemeID(std::list<stXZSJB>& listStXZSJB,const int & schemeID);

	// 0_�������ݱ�_1
	bool getAllDxsjb(std::list<stDXSJB>& listStDXSJB);
	bool getAllDxsjbBySchemeID(std::list<stDXSJB>& listStDXSJB, const int& schemeID);

		//0_��������_1
	bool getAllWXTXB(std::list<stWXTXB>& listStWXTXB);
	bool getAllWxtxbBySchemeID(std::list<stWXTXB>& listStWXTXB, const int& schemeID);

		//0_�����غ����ݱ�_1
	bool getAllWXZHSJB(std::list<stWXZHSJB>& listStData);
	bool getAllWxzhsjbBySchemeID(std::list<stWXZHSJB>& listStData, const int& schemeID);

	// 1_���Ÿ����ݱ�_1
	bool getAllDxgsjbBySchemeID(std::list<stDXGSJB>& listStData, const int& schemeID);
		// 1_����վ���ݱ�
	bool getAllDqzsjbBySchemeID(std::list<stDQZSJB>& listStData, const int& schemeID);
	// 1_����վ��Դ���ݱ�_1
	bool getAllDqzkysjbBySchemeID(std::list<stDQZKYSJB>& listStData, const int& schemeID);
		// 1_����վ�����豸���ݱ�_1
	bool getAllDqzbxsbsjbBySchemeID(std::list<stDQZBXSBSJB>& listStData, const int& schemeID);
	// 1_1_����վ����_1_1
	bool getAllDqztxbBySchemeID(std::list<stDQTXB>& listStData, const int& schemeID);
private:
	MYSQL* m_mysql; //��� ����m_mydata;
private:
	HANDLE m_mysqlHdl;
	
	MYSQL_RES *res; 
	//MYSQL m_mydata;

	int m_ColumnLenght;//�����ݱ�������������
};

