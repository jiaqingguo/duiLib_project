#pragma once
#include <mysql.h>

#include <list>
#include <set>

#include "DatabaseStruct.h"

//struct st_mysql_res;

// ����ģʽ;
class ConnectMysql
{

public:
	
	static ConnectMysql& Instance();

protected:
	ConnectMysql();
	virtual ~ConnectMysql();

	ConnectMysql(const ConnectMysql&) = delete;
	ConnectMysql& operator=(const ConnectMysql&) = delete;
	ConnectMysql(ConnectMysql&&) = delete;
	ConnectMysql& operator=(ConnectMysql&&) = delete;
public:
	//ConnectMysql(void);
	//~ConnectMysql(void);
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

	// ִ��sql;
	MYSQL_RES* execSqlSelect(const std::string& sql);
	bool execSql(const std::string& sql);
	bool execSql(uint32_t& lastId, const std::string& sql);
	// ���ݿ���������;
	bool startupTransaction();
	// ���ݿ��ύ����;
	bool commitTransaction();
	// ���ݿ�ع�����;
	bool rollbackTransaction();
	// �жϱ��Ƿ����;
	bool isTableExists(const std::string& tableName);
	// �жϱ��ֶ��Ƿ����;
	bool isFieldExists(const std::string& tableName, const std::string& fieldName);
	// �������ֶ�;
	bool createTableAndFields(const std::string& table, const std::vector<std::string> &vecFilelds);

	bool createTableAndFields(const std::string& table, const std::set<std::string>& vecFilelds);

	//bool createTableAndFields(const std::wstring& table, const std::set<std::wstring>& vecFilelds);

	// ����� ���ֶζ�Ӧ����  �����������ֶΣ����ֶζ�Ӧֵ��;
	bool insertTableData(const std::string& tableName, const std::vector<std::string>& vecFilelds, const std::vector<std::string>& vecFileldsValue);
	// ɾ����;
	bool delTable(const std::string& tableName);

	// ������;

	bool getAllScheme(std::map<int,stScheme> &listScheme);
	// ���ӷ���;
	bool addScheme(const std::string &schemeName);

	// �������ݱ�;
	//bool getAllXzsjb(sstXZSJB& record_list);
	
	bool getAllXzsjb(std::map<int,stXZSJB>& listStXZSJB); // ��ȡ��������
	bool getAllXzsjbBySchemeID(std::map<int,stXZSJB>& listStXZSJB,const int & schemeID);

	// 0_�������ݱ�_1
	bool getAllDxsjb(std::map<int,stDXSJB>& listStDXSJB);
	bool getAllDxsjbBySchemeID(std::map<int,stDXSJB>& listStDXSJB, const int& schemeID);
	// ���ݷ���id������id��ȡ
	bool getDxsjbBySchemeIDAndXzID(std::map<int, stDXSJB>& listStDXSJB, const int& schemeID, const int & XzID);

		//0_��������_1
	bool getAllWXTXB(std::map<int,stWXTXB>& listStWXTXB);
	bool getAllWxtxbBySchemeID(std::map<int,stWXTXB>& listStWXTXB, const int& schemeID);
	
		//0_�����غ����ݱ�_1
	bool getAllWXZHSJB(std::map<int,stWXZHSJB>& listStData);
	bool getAllWxzhsjbBySchemeID(std::map<int,stWXZHSJB>& listStData, const int& schemeID);

	// 1_���Ÿ����ݱ�_1
	bool getAllDxgsjbBySchemeID(std::map<int,stDXGSJB>& listStData, const int& schemeID);
	// 1_����վ���ݱ�
	bool getAllDqzsjbBySchemeID(std::map<int,stDQZSJB>& listStData, const int& schemeID);
	// 1_����վ��Դ���ݱ�_1
	bool getAllDqzkysjbBySchemeID(std::map<int,stDQZKYSJB>& listStData, const int& schemeID);
	// 1_����վ�����豸���ݱ�_1
	bool getAllDqzbxsbsjbBySchemeID(std::map<int,stDQZBXSBSJB>& listStData, const int& schemeID);
	// 1_1_����վ����_1_1
	bool getAllDqztxbBySchemeID(std::map<int,stDQTXB>& listStData, const int& schemeID);
private:
	MYSQL* m_mysql; //��� ����m_mydata;
private:
	HANDLE m_mysqlHdl;
	
	MYSQL_RES *res; 
	//MYSQL m_mydata;

	int m_ColumnLenght;//�����ݱ�������������
};

