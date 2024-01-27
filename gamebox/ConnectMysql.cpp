#include "stdafx.h"
#include "ConnectMysql.h"
#include <time.h>
#include <iostream>
#include <winsock.h>
#include <string>
#include <sstream>
#include <vector>
using namespace std;
//#pragma comment(lib, "libmysql.lib")

ConnectMysql& ConnectMysql::Instance()
{
	static ConnectMysql instance;
	return instance;
}

ConnectMysql::ConnectMysql()
{
	m_num = 0;
}

ConnectMysql::~ConnectMysql()
{
}

//ConnectMysql::ConnectMysql(void)
//{
//	m_num = 0;
//}
//ConnectMysql::~ConnectMysql(void)
//{
//	//mysql_close(m_mysql);
//	
//    //mysql_free_result(res);    
//    mysql_close(m_mysql);    
//    //free(m_mysql);    
//}
vector<string> my_split_inMysql(string str, string pattern)
{
	string::size_type pos;
	vector<string> result;

	str += pattern;//扩展字符串以方便操作
	int size = str.size();

	for (int i = 0; i < size; i++) {
		pos = str.find(pattern, i);
		if (pos < size) {
			std::string s = str.substr(i, pos - i);
			result.push_back(s);
			i = pos + pattern.size() - 1;
		}
	}
	return result;
}
//获取当前时间
string ConnectMysql::getCurrentTime()
{
	char current_time[40];
	time_t rawtime;
	rawtime = time(NULL);
	struct tm tt;
	//time(&rawtime);
	localtime_s(&tt, &rawtime);
	int year, month, day, hour, min, sec;
	year = 1900 + tt.tm_year;
	month = 1 + tt.tm_mon;
	day = tt.tm_mday;
	hour = tt.tm_hour;
	min = tt.tm_min;
	sec = tt.tm_sec;
	sprintf(current_time,  "%d_%d_%d_%d_%d_%d", year, month, day, hour, min, sec);
	string time_string(current_time);
	return time_string;
}
bool ConnectMysql::InitDatabase()
{
	//MYSQL m_mydata = sql;
	if (mysql_library_init(0, NULL, NULL) == 0)
		cout << "mysql_library_init() succeed" << endl;
	else
	{
		cout << "mysql_library_init() failed" << endl;
		return false;
	}
	//初始化数据结构
	m_mysql = mysql_init(nullptr);

	if (m_mysql == nullptr)
	{
		cout << "mysql_init() failed" << endl;
		return false;
	}//在连接数据库之前，设置额外的连接选项
	//可以设置的选项很多，这里设置字符集，否则无法处理中文
	if (mysql_options(m_mysql, MYSQL_SET_CHARSET_NAME, "gbk") == 0)
		cout << "mysql_options() succeed" << endl;
	else
	{
		cout << "mysql_options() failed" << endl;
		return false;
	}
	return TRUE;
}
//连接数据库
bool ConnectMysql::StartConnectMysql()
{
	//MYSQL m_mydata = sql;
	if (!InitDatabase())
	{
		return false;
	}
	//if (mysql_real_connect(m_mysql, "localhost", "root", "123456", "shoesshop", 3306, NULL, 0) != NULL)	//这里的地址，用户名，密码，端口可以根据自己本地的情况更改
	//if (mysql_real_connect(m_mysql, "127.0.0.1", "root", "123456", "jh_database", 3306, NULL, 0) != NULL)
	if (mysql_real_connect(m_mysql, "127.0.0.1", "root", "123456", "jh_database", 3306, NULL, 0) != NULL)
		cout << "mysql_real_connect() succeed" << endl;
	else
	{
		cout << "mysql_real_connect() failed: " << mysql_error(m_mysql) << endl;
		return false;
	}
	return true;
}
//创建新的表
bool ConnectMysql::CreateNewTable(string name)
{

	//MYSQL m_mydata = sql;
	string sqlstr;
	///////////////////////创建一个表/////////////
	sqlstr = "create table if not exists ";
	sqlstr += name;
	sqlstr += " (";
	sqlstr += "u_id INT UNSIGNED NOT NULL AUTO_INCREMENT PRIMARY KEY COMMENT 'Unique User ID',";
	sqlstr += "u_time datetime COMMENT 'record time',";
	sqlstr += "u_X VARCHAR(30)  NOT NULL DEFAULT 0 COMMENT 'The Summation Of Using Time',";
	sqlstr += "u_Y VARCHAR(30)  NOT NULL DEFAULT 0 COMMENT 'The Summation Of Using Time',";
	sqlstr += "u_Z VARCHAR(30)  NOT NULL DEFAULT 0 COMMENT 'The Summation Of Using Time',";
	sqlstr += "u_GD VARCHAR(30)  NOT NULL DEFAULT 0 COMMENT 'The Summation Of Using Time',";
	sqlstr += "u_FY VARCHAR(30)  NOT NULL DEFAULT 0 COMMENT 'The Summation Of Using Time',";
	sqlstr += "u_PH VARCHAR(30)  NOT NULL DEFAULT 0 COMMENT 'The Summation Of Using Time',";
	sqlstr += "u_Dof7 VARCHAR(30)  NOT NULL DEFAULT 0 COMMENT 'The Summation Of Using Time',";
	sqlstr += "u_Dof8 VARCHAR(30)  NOT NULL DEFAULT 0 COMMENT 'The Summation Of Using Time',";
	sqlstr += "u_Dof9 VARCHAR(30)  NOT NULL DEFAULT 0 COMMENT 'The Summation Of Using Time'";
	sqlstr += ");";
	//sqlstr = "create table if not exists 光纤1_2121 (u_id INT UNSIGNED NOT NULL AUTO_INCREMENT PRIMARY KEY COMMENT 'Unique User ID',u_time VARCHAR(100) CHARACTER SET gb2312 COLLATE gb2312_chinese_ci NULL COMMENT 'Name Of User');";
	cout << sqlstr << endl;
	if (mysql_query(m_mysql, sqlstr.c_str()) == 0)
		cout << "mysql_query() create table succeed" << endl;
	else
	{
		//string str = mysql_error(m_mysql);
		cout << "mysql_query() create table failed" << mysql_error(m_mysql)<<endl;
		mysql_close(m_mysql);
		return FALSE;
	}
	return true;
}
//给了列名称创建新表
bool ConnectMysql::CreateNewTable(string name,vector<string> vec)
{

	//MYSQL m_mydata = sql;
	string sqlstr;
	///////////////////////创建一个表/////////////
	sqlstr = "create table if not exists ";
	sqlstr += name;
	sqlstr += " (";
	//sqlstr += "u_id INT UNSIGNED NOT NULL AUTO_INCREMENT PRIMARY KEY COMMENT 'Unique User ID'";
	for(int i = 0; i < vec.size() - 1; i++)
	{
		
		sqlstr += vec[i];
		sqlstr += " VARCHAR(30) ";
		sqlstr += ",";
	}
	sqlstr += vec[vec.size() - 1];
	sqlstr += " VARCHAR(30) ";
	sqlstr += ");";
	//sqlstr += "u_time datetime COMMENT 'record time',";
	//sqlstr += "u_X VARCHAR(30)  NOT NULL DEFAULT 0 COMMENT 'The Summation Of Using Time',";
	//sqlstr += "u_Y VARCHAR(30)  NOT NULL DEFAULT 0 COMMENT 'The Summation Of Using Time',";
	//sqlstr += "u_Z VARCHAR(30)  NOT NULL DEFAULT 0 COMMENT 'The Summation Of Using Time',";
	//sqlstr += "u_GD VARCHAR(30)  NOT NULL DEFAULT 0 COMMENT 'The Summation Of Using Time',";
	//sqlstr += "u_FY VARCHAR(30)  NOT NULL DEFAULT 0 COMMENT 'The Summation Of Using Time',";
	//sqlstr += "u_PH VARCHAR(30)  NOT NULL DEFAULT 0 COMMENT 'The Summation Of Using Time',";
	//sqlstr += "u_Dof7 VARCHAR(30)  NOT NULL DEFAULT 0 COMMENT 'The Summation Of Using Time',";
	//sqlstr += "u_Dof8 VARCHAR(30)  NOT NULL DEFAULT 0 COMMENT 'The Summation Of Using Time',";
	//sqlstr += "u_Dof9 VARCHAR(30)  NOT NULL DEFAULT 0 COMMENT 'The Summation Of Using Time'";
	//sqlstr += ");";
	//sqlstr = "create table if not exists 光纤1_2121 (u_id INT UNSIGNED NOT NULL AUTO_INCREMENT PRIMARY KEY COMMENT 'Unique User ID',u_time VARCHAR(100) CHARACTER SET gb2312 COLLATE gb2312_chinese_ci NULL COMMENT 'Name Of User');";
	cout << sqlstr << endl;
	if (mysql_query(m_mysql, sqlstr.c_str()) == 0)
		cout << "mysql_query() create table succeed" << endl;
	else
	{
		string str = mysql_error(m_mysql);
		cout << "mysql_query() create table failed" << mysql_error(m_mysql)<<endl;
		cout << str << endl;
		mysql_close(m_mysql);
		return FALSE;
	}
	return true;
}
//向数据表中增加一条数据 insert into 2022_9_20_14_25_43_s values("1","2022_9_20","11","12","13","14","15","16","17","18","19");
void ConnectMysql::addMsgToTable(string name, string x, string y, string z, string gd, string fy, string ph, string dof7, string dof8, string dof9)
{
	m_num += 1;
	string sqlstr;
	string current_time;
	current_time = getCurrentTime();
	string u_id, u_time;
	//int u_x, u_y, u_z, u_GD, u_FY,u_PH ,u_Dof7,u_Dof8 ,u_Dof9;

	//向表中插入数据
	sqlstr = "insert into ";
	sqlstr += name;

	string s_num;
	std::ostringstream streamsssss;
	streamsssss << m_num;
	s_num=streamsssss.str();

	sqlstr += " values('" + s_num + "','" + current_time.c_str() + "','" + x + "','" + y + "','" + z + "','" + gd + "','" + fy +
		"','" + ph + "','" + dof7 + "','" + dof8 +"','" + dof9 + "');";
	cout << sqlstr << endl;
	if (mysql_query(m_mysql, sqlstr.c_str()) == 0)
	{
		cout << "mysql_query() insert data succeed" << endl;
	}
	else
	{
		cout << "mysql_query() insert data failed" << endl;
		mysql_close(m_mysql);
		//return FALSE;
	}
	//return TRUE;
}
//向数据表中添加数据
void ConnectMysql::addMsgToTable(string name,vector<string> vec)
{
	string sqlstr;
	string current_time;
	current_time = getCurrentTime();
	string u_id, u_time;
	//int u_x, u_y, u_z, u_GD, u_FY,u_PH ,u_Dof7,u_Dof8 ,u_Dof9;

	//向表中插入数据
	sqlstr = "insert into ";
	sqlstr += name;

	sqlstr += " values('";
	//sqlstr += "u_id INT UNSIGNED NOT NULL AUTO_INCREMENT PRIMARY KEY COMMENT 'Unique User ID'";
	for(int i = 0; i < vec.size() - 1; i++)
	{
		
		sqlstr += vec[i];
		sqlstr += "','";
	}
	sqlstr += vec[vec.size() - 1];
	sqlstr += "');";
	cout << "";
	if (mysql_query(m_mysql, sqlstr.c_str()) == 0)
	{
		cout << "mysql_query() insert data succeed" << endl;
	}
	else
	{
		cout << "mysql_query() insert data failed" << endl;
		mysql_close(m_mysql);
		//return FALSE;
	}
}
//查询表格
bool ConnectMysql::showAllTable(string name_Table)
{
	string sqlstr;
	char id[20];
	string string_id(id);
	sqlstr = "select * from ";
	sqlstr += name_Table;
	cout << sqlstr << endl;
	//MYSQL_RES *result_select = NULL;
	if (mysql_query(m_mysql, sqlstr.c_str()) == 0)
	{
		cout << "mysql_query() select data succeed" << endl;
		res = mysql_store_result(m_mysql);
		//取得并打印行数
		int rowcount = mysql_num_rows(res);
		cout << "row count: " << rowcount << endl;
		//取得并打印各字段的名称
		unsigned int fieldcount_select = mysql_num_fields(res);
		MYSQL_FIELD *field = NULL;
		for (unsigned int i = 0; i < fieldcount_select; i++)
		{
			field = mysql_fetch_field_direct(res, i);
			cout << field->name << "\t\t";
		}
		cout << endl;
		//打印各行
		MYSQL_ROW row_select = NULL;
		row_select = mysql_fetch_row(res);
		while (NULL != row_select)
		{
			for (int i = 0; i < fieldcount_select; i++)
				cout << row_select[i] << "\t\t";
			cout << endl;
			row_select = mysql_fetch_row(res);
		}
	}
	else
	{
		cout << "mysql_query() select data failed" << endl;
		mysql_close(m_mysql);
		return FALSE;
	}
	return TRUE;
}
void ConnectMysql::closeDatabase()
{
	mysql_close(m_mysql);
	//mysql_shutdown(m_mysql, KILL_CONNECTION);
	//mysql_stmt_close(m_mysql);
	//mysql_stmt_reset(m_mysql);
	
}
//显示数据库中所有表名称
bool ConnectMysql::showAllTableName()
{
	string sqlstr;
	char id[20];
	string string_id(id);
	sqlstr = "show tables;";
	//sqlstr += name_Table;
	cout << sqlstr << endl;
	res = NULL;
	if (mysql_query(m_mysql, sqlstr.c_str()) == 0)
	{
		cout << "mysql_query() select data succeed" << endl;
		res = mysql_store_result(m_mysql);
		//取得并打印行数
		int rowcount = mysql_num_rows(res);
		cout << "row count: " << rowcount << endl;
		//取得并打印各字段的名称
		unsigned int fieldcount_select = mysql_num_fields(res);
		MYSQL_FIELD *field = NULL;
		for (unsigned int i = 0; i < fieldcount_select; i++)
		{
			field = mysql_fetch_field_direct(res, i);
			cout << field->name << "\t\t";
		}
		cout << endl;
		//打印各行
		MYSQL_ROW row_select = NULL;
		row_select = mysql_fetch_row(res);
		while (NULL != row_select)
		{
			for (int i = 0; i < fieldcount_select; i++)
				cout << row_select[i] << "\t\t";
			cout << endl;
			row_select = mysql_fetch_row(res);
		}
	}
	else
	{
		cout << "mysql_query() select data failed" << endl;
		mysql_close(m_mysql);
		return FALSE;
	}
	return TRUE;
}
//设置当前数据库
bool ConnectMysql::SetUpDatabase(string database)
{
	bool returnResult;
	MYSQL_RES *res; //这个结构代表返回行的一个查询结果集  
	MYSQL_ROW column; //一个行数据的类型安全(type-safe)的表示，表示数据行的列 
	char current_c[150]; //查询语句
	string sqlstr, outPut;

	//char id[20];
	//string string_id(id);
	sqlstr = "use ";
	sqlstr += database;
	sqlstr += ";";
	cout << sqlstr << endl;
	//sprintf_s(query, "select * from t_dept"); //执行查询语句，这里是查询所有，user是表名，不用加引号，用strcpy也可以  
	//mysql_query(&mydata, "set names gbk"); //设置编码格式（SET NAMES GBK也行），否则cmd下中文乱码  
	//返回0 查询成功，返回1查询失败  
	if (mysql_query(m_mysql, sqlstr.c_str()))    //执行SQL语句
	{
		printf("Query failed (%s)\n", mysql_error(m_mysql));
		return false;
		//return false;
	}
	else
	{
		printf("query success\n");
		return true;
	}
}
//返回指定数据表中所有内容
vector<vector<string>> ConnectMysql::mytest_QueryDatabase(string name_Table)
{
	vector<vector<string>> vec_result;
	MYSQL_RES *res; //这个结构代表返回行的一个查询结果集  
	MYSQL_ROW column; //一个行数据的类型安全(type-safe)的表示，表示数据行的列 
	char current_c[150]; //查询语句
	string sqlstr,outPut;

	//char id[20];
	//string string_id(id);
	sqlstr = "select * from ";
	sqlstr += name_Table;
	sqlstr += ";";
	cout << sqlstr << endl;
	//sprintf_s(query, "select * from t_dept"); //执行查询语句，这里是查询所有，user是表名，不用加引号，用strcpy也可以  
	//mysql_query(&mydata, "set names gbk"); //设置编码格式（SET NAMES GBK也行），否则cmd下中文乱码  
	//返回0 查询成功，返回1查询失败  
	if (mysql_query(m_mysql, sqlstr.c_str()))    //执行SQL语句
	{
		printf("Query failed (%s)\n", mysql_error(m_mysql));
		//return false;
	}
	else
	{
		printf("query success\n");
	}
	//获取结果集  
	if (!(res = mysql_store_result(m_mysql)))   //获得sql语句结束后返回的结果集  
	{
		printf("Couldn't get result from %s\n", mysql_error(m_mysql));
		//return false;
	}
	unsigned int fieldcount_select = mysql_num_fields(res);
	//unsigned int affect_row = mysql_affected_rows(m_mysql);
	//打印数据行数  
	printf("number of dataline returned: %d\n", mysql_affected_rows(m_mysql));//

	//获取字段的信息  
	char *str_field[256];  //定义一个字符串数组存储字段信息  
	for (int i = 0; i < fieldcount_select; i++)  //在已知字段数量的情况下获取字段名  
	{
		str_field[i] = mysql_fetch_field(res)->name;
	}
	for (int i = 0; i < fieldcount_select; i++)  //打印字段  
		printf("%10s\t", str_field[i]);
	printf("---------------------------\n");

	while (column = mysql_fetch_row(res))   //在已知字段数量情况下，获取并打印下一行  
	{
		vector<string> vec;
		for (int j = 0; j < fieldcount_select; j++)
		{
			//vector<string> vec;
			sprintf(current_c, "%s", column[j]);
			string result(current_c);
			vec.push_back(result);
		}
		vec_result.push_back(vec);

	}

	return vec_result;
}
//返回指定数据表中所有内容
string ConnectMysql::mytest_QueryDatabase_1(string name_Table)
{
	MYSQL_RES *res; //这个结构代表返回行的一个查询结果集  
	MYSQL_ROW column; //一个行数据的类型安全(type-safe)的表示，表示数据行的列 
	char current_c[2048]; //查询语句
	string sqlstr, outPut;

	//char id[20];
	//string string_id(id);
	sqlstr = "select * from ";
	sqlstr += name_Table;
	cout << sqlstr << endl;
	//sprintf_s(query, "select * from t_dept"); //执行查询语句，这里是查询所有，user是表名，不用加引号，用strcpy也可以  
	//mysql_query(&mydata, "set names gbk"); //设置编码格式（SET NAMES GBK也行），否则cmd下中文乱码  
	//返回0 查询成功，返回1查询失败  
	if (mysql_query(m_mysql, sqlstr.c_str()))    //执行SQL语句
	{
		printf("Query failed (%s)\n", mysql_error(m_mysql));
		//return false;
	}
	else
	{
		printf("query success\n");
	}
	//获取结果集  
	if (!(res = mysql_store_result(m_mysql)))   //获得sql语句结束后返回的结果集  
	{
		printf("Couldn't get result from %s\n", mysql_error(m_mysql));
		//return false;
	}
	unsigned int fieldcount_select = mysql_num_fields(res);
	//打印数据行数  
	printf("number of dataline returned: %d\n", mysql_affected_rows(m_mysql));

	//获取字段的信息  
	char *str_field[40];  //定义一个字符串数组存储字段信息  
	for (int i = 0; i < fieldcount_select; i++)  //在已知字段数量的情况下获取字段名  
	{
		str_field[i] = mysql_fetch_field(res)->name;
	}
	for (int i = 0; i < fieldcount_select; i++)  //打印字段  
		printf("%10s\t", str_field[i]);
	printf("---------------------------\n");
	//打印获取的数据  
	while (column = mysql_fetch_row(res))   //在已知字段数量情况下，获取并打印下一行  
	{
		printf("%10s\t%10s\t%10s\t%10s\t%10s\t%10s\t%10s\t%10s\t\n", column[0], column[1], column[2], column[3], column[4], column[5], column[6], column[7]);  //column是列数组  
		snprintf(current_c, 2048, "%s*%s*%s*%s*%s*%s*%s*%s*%s*%s*%s!", column[0], column[1], column[2], column[3], column[4], column[5], column[6], column[7]);
		string result(current_c);
		outPut += result;
		cout << result;
		//result = column[0] + column[1] + column[2] + column[3] + column[4] + column[5] + column[6] + column[7], +column[8] + column[9] + column[10];
	}
	//string result = clumn[0] + ;

	return outPut;
}

string ConnectMysql::mytest_showDbAllName()
{
	//MYSQL_RES *res; //这个结构代表返回行的一个查询结果集  
	MYSQL_ROW column; //一个行数据的类型安全(type-safe)的表示，表示数据行的列 
	char current_c[150]; //查询语句
	string sqlstr, outPut;

	//char id[20];
	//string string_id(id);
	sqlstr = "show tables;";
	cout << sqlstr << endl;
	//sprintf_s(query, "select * from t_dept"); //执行查询语句，这里是查询所有，user是表名，不用加引号，用strcpy也可以  
	//mysql_query(&mydata, "set names gbk"); //设置编码格式（SET NAMES GBK也行），否则cmd下中文乱码  
	//返回0 查询成功，返回1查询失败  
	if (mysql_query(m_mysql, sqlstr.c_str()))    //执行SQL语句
	{
		printf("Query failed (%s)\n", mysql_error(m_mysql));
		//return false;
	}
	else
	{
		printf("query success\n");
	}
	//获取结果集  
	if (!(res = mysql_store_result(m_mysql)))   //获得sql语句结束后返回的结果集  
	{
		printf("Couldn't get result from %s\n", mysql_error(m_mysql));
		//return false;
	}
	unsigned int fieldcount_select = mysql_num_fields(res);
	//打印数据行数  
	printf("number of dataline returned: %d\n", mysql_affected_rows(m_mysql));

	//获取字段的信息  
	char *str_field[40];  //定义一个字符串数组存储字段信息  
	for (int i = 0; i < fieldcount_select; i++)  //在已知字段数量的情况下获取字段名  
	{
		str_field[i] = mysql_fetch_field(res)->name;
	}
	for (int i = 0; i < fieldcount_select; i++)  //打印字段  
		printf("%10s\t", str_field[i]);
	printf("---------------------------\n");
	//打印获取的数据  
	while (column = mysql_fetch_row(res))   //在已知字段数量情况下，获取并打印下一行  
	{
		printf("%10s\n", column[0]);  //column是列数组  
		sprintf(current_c, "%s!", column[0]);
		string result(current_c);
		outPut += result;
		//cout << result;
		//result = column[0] + column[1] + column[2] + column[3] + column[4] + column[5] + column[6] + column[7], +column[8] + column[9] + column[10];
	}
	//string result = clumn[0] + ;

	return outPut;
}
//返回某段时间内的数据
string ConnectMysql::mytest_QueryByTime(string start, string end, string name_Table)
{
	//select * from 动力学_2022_09_27_10_21_42_292 where u_time between '2022_9_27 10_21_44'  and '2022-9-27 10:21:46';
	//MYSQL_RES *res; //这个结构代表返回行的一个查询结果集  
	MYSQL_ROW column; //一个行数据的类型安全(type-safe)的表示，表示数据行的列 
	char current_c[150]; //查询语句
	string sqlstr, outPut;

	//char id[20];
	//string string_id(id);
	sqlstr = "select * from ";
	sqlstr += name_Table;
	sqlstr += " where u_time between '";
	sqlstr += start;
	sqlstr += "' and '";
	sqlstr += end;
	sqlstr += "';";
	cout << sqlstr << endl;
	//sprintf_s(query, "select * from t_dept"); //执行查询语句，这里是查询所有，user是表名，不用加引号，用strcpy也可以  
	//mysql_query(&mydata, "set names gbk"); //设置编码格式（SET NAMES GBK也行），否则cmd下中文乱码  
	//返回0 查询成功，返回1查询失败  
	if (mysql_query(m_mysql, sqlstr.c_str()))    //执行SQL语句
	{
		printf("Query failed (%s)\n", mysql_error(m_mysql));
		//return false;
	}
	else
	{
		printf("query success\n");
	}
	//获取结果集  
	if (!(res = mysql_store_result(m_mysql)))   //获得sql语句结束后返回的结果集  
	{
		printf("Couldn't get result from %s\n", mysql_error(m_mysql));
		//return false;
	}
	unsigned int fieldcount_select = mysql_num_fields(res);
	//打印数据行数  
	printf("number of dataline returned: %d\n", mysql_affected_rows(m_mysql));

	//获取字段的信息  
	char *str_field[40];  //定义一个字符串数组存储字段信息  
	for (int i = 0; i < fieldcount_select; i++)  //在已知字段数量的情况下获取字段名  
	{
		str_field[i] = mysql_fetch_field(res)->name;
	}
	for (int i = 0; i < fieldcount_select; i++)  //打印字段  
		printf("%10s\t", str_field[i]);
	printf("---------------------------\n");
	//打印获取的数据  
	while (column = mysql_fetch_row(res))   //在已知字段数量情况下，获取并打印下一行  
	{
		printf("%10s\t%10s\t%10s\t%10s\t%10s\t%10s\t%10s\t%10s\t%10s\t%10s\t%10s\n", column[0], column[1], column[2], column[3], column[4], column[5], column[6], column[7], column[8], column[9], column[10]);  //column是列数组  
		sprintf(current_c, "%s*%s*%s*%s*%s*%s*%s*%s*%s*%s*%s!", column[0], column[1], column[2], column[3], column[4], column[5], column[6], column[7], column[8], column[9], column[10]);
		string result(current_c);
		outPut += result;
		cout << result;
		//result = column[0] + column[1] + column[2] + column[3] + column[4] + column[5] + column[6] + column[7], +column[8] + column[9] + column[10];
	}
	//string result = clumn[0] + ;

	return outPut;
}

//根据选择的列返回某段时间内的数据
vector<vector<string>> ConnectMysql::mytest_QueryByTime(string start, string end, string name_Table,vector<string> vec)
{
	vector<vector<string>> vec_result;
	//select u_id,u_time,u_X from 动力学_2022_10_09_09_47_51_593 where u_time between '2019_9_27 10_21_44'  and '2022-9-27 10:21:46';
	//MYSQL_RES *res; //这个结构代表返回行的一个查询结果集  
	MYSQL_ROW column; //一个行数据的类型安全(type-safe)的表示，表示数据行的列 
	char current_c[150]; //查询语句
	string sqlstr, outPut;

	sqlstr = "select ";
	for(int i = 0; i < vec.size() - 1; i++)
	{
		sqlstr += vec[i];
		sqlstr += ",";
	}
	sqlstr += vec[vec.size() - 1];
	sqlstr += " from ";
	sqlstr += name_Table;
	sqlstr += " where u_time between '";
	sqlstr += start;
	sqlstr += "' and '";
	sqlstr += end;
	sqlstr += "';";
	cout << sqlstr << endl;
	//sprintf_s(query, "select * from t_dept"); //执行查询语句，这里是查询所有，user是表名，不用加引号，用strcpy也可以  
	//mysql_query(&mydata, "set names gbk"); //设置编码格式（SET NAMES GBK也行），否则cmd下中文乱码  
	//返回0 查询成功，返回1查询失败  
	if (mysql_query(m_mysql, sqlstr.c_str()))    //执行SQL语句
	{
		printf("Query failed (%s)\n", mysql_error(m_mysql));
		//return false;
	}
	else
	{
		printf("query success\n");
	}
	//获取结果集  
	if (!(res = mysql_store_result(m_mysql)))   //获得sql语句结束后返回的结果集  
	{
		printf("Couldn't get result from %s\n", mysql_error(m_mysql));
		//return false;
	}
	unsigned int fieldcount_select = mysql_num_fields(res);
	//打印数据行数  
	printf("number of dataline returned: %d\n", mysql_affected_rows(m_mysql));

	//获取字段的信息  
	char *str_field[40];  //定义一个字符串数组存储字段信息  
	for (int i = 0; i < fieldcount_select; i++)  //在已知字段数量的情况下获取字段名  
	{
		str_field[i] = mysql_fetch_field(res)->name;
	}
	for (int i = 0; i < fieldcount_select; i++)  //打印字段  
		printf("%10s\t", str_field[i]);
	printf("---------------------------\n");
	//打印获取的数据  
	while (column = mysql_fetch_row(res))   //在已知字段数量情况下，获取并打印下一行  
	{
		vector<string> vec;
		for (int j = 0; j < fieldcount_select; j++)
		{
			//vector<string> vec;
			sprintf(current_c, "%s", column[j]);
			string result(current_c);
			vec.push_back(result);
		}
		vec_result.push_back(vec);

	}
	return vec_result;
}

//查询数据表中各个列名称
//SELECT GROUP_CONCAT(COLUMN_NAME SEPARATOR "*") FROM information_schema.COLUMNS WHERE TABLE_SCHEMA = '111111' AND TABLE_NAME = '动力学_2022_10_09_09_47_51_593';
vector<string> ConnectMysql::mytest_QueryColumns(string dbName,string name_Table)
{
	m_ColumnLenght = 0;
	vector<string> vec;
	//MYSQL_RES *res; //这个结构代表返回行的一个查询结果集  
	MYSQL_ROW column; //一个行数据的类型安全(type-safe)的表示，表示数据行的列 
	char current_c[150]; //查询语句
	string sqlstr, outPut;

	//char id[20];
	//string string_id(id);
	//sqlstr = "SELECT GROUP_CONCAT(COLUMN_NAME SEPARATOR '*') FROM information_schema.COLUMNS WHERE TABLE_SCHEMA = '";
	sqlstr = "SELECT GROUP_CONCAT(COLUMN_NAME SEPARATOR '*') FROM information_schema.COLUMNS WHERE TABLE_SCHEMA = '";
	sqlstr += dbName;
	sqlstr += "' AND TABLE_NAME = '";
	sqlstr += name_Table;
	sqlstr += "';";
	cout << sqlstr << endl;
	//sprintf_s(query, "select * from t_dept"); //执行查询语句，这里是查询所有，user是表名，不用加引号，用strcpy也可以  
	//mysql_query(&mydata, "set names gbk"); //设置编码格式（SET NAMES GBK也行），否则cmd下中文乱码  
	//返回0 查询成功，返回1查询失败  
	if (mysql_query(m_mysql, sqlstr.c_str()))    //执行SQL语句
	{
		printf("Query failed (%s)\n", mysql_error(m_mysql));
		//return false;
	}
	else
	{
		printf("query success\n");
	}
	//获取结果集  
	if (!(res = mysql_store_result(m_mysql)))   //获得sql语句结束后返回的结果集  
	{
		printf("Couldn't get result from %s\n", mysql_error(m_mysql));
		//return false;
	}
	unsigned int fieldcount_select = mysql_num_fields(res);
	//打印数据行数  
	printf("number of dataline returned: %d\n", mysql_affected_rows(m_mysql));

	//获取字段的信息  
	char *str_field[40];  //定义一个字符串数组存储字段信息  
	for (int i = 0; i < fieldcount_select; i++)  //在已知字段数量的情况下获取字段名  
	{
		str_field[i] = mysql_fetch_field(res)->name;
	}
	for (int i = 0; i < fieldcount_select; i++)  //打印字段  
		printf("%10s\t", str_field[i]);
	printf("---------------------------\n");
	//打印获取的数据  
	while (column = mysql_fetch_row(res))   //在已知字段数量情况下，获取并打印下一行  
	{
		printf("%s", column[0]);  //column是列数组  
		sprintf(current_c, "%s", column[0]);
		string result(current_c);
		outPut += result;
		cout << result << endl;
		//result = column[0] + column[1] + column[2] + column[3] + column[4] + column[5] + column[6] + column[7], +column[8] + column[9] + column[10];
	}
	//string result = clumn[0] + ;
	vec = my_split_inMysql(outPut,"*");
	m_ColumnLenght = vec.size();

	return vec;
}
//获取所有数据库名称
vector<string> ConnectMysql::mytest_QueryDatabase()
{
	vector<string > vec;
	vector<vector<string>> vec_result;
	MYSQL_RES *res; //这个结构代表返回行的一个查询结果集  
	MYSQL_ROW column; //一个行数据的类型安全(type-safe)的表示，表示数据行的列 
	char current_c[150]; //查询语句
	string sqlstr, outPut;

	//char id[20];
	//string string_id(id);
	sqlstr = "show databases;";

	cout << sqlstr << endl;
	//sprintf_s(query, "select * from t_dept"); //执行查询语句，这里是查询所有，user是表名，不用加引号，用strcpy也可以  
	//mysql_query(&mydata, "set names gbk"); //设置编码格式（SET NAMES GBK也行），否则cmd下中文乱码  
	//返回0 查询成功，返回1查询失败  
	if (mysql_query(m_mysql, sqlstr.c_str()))    //执行SQL语句
	{
		printf("Query failed (%s)\n", mysql_error(m_mysql));
		//return false;
	}
	else
	{
		printf("query success\n");
	}
	//获取结果集  
	if (!(res = mysql_store_result(m_mysql)))   //获得sql语句结束后返回的结果集  
	{
		printf("Couldn't get result from %s\n", mysql_error(m_mysql));
		//return false;
	}
	unsigned int fieldcount_select = mysql_num_fields(res);
	//unsigned int affect_row = mysql_affected_rows(m_mysql);
	//打印数据行数  
	printf("number of dataline returned: %d\n", mysql_affected_rows(m_mysql));//

	//获取字段的信息  
	char *str_field[40];  //定义一个字符串数组存储字段信息  
	for (int i = 0; i < fieldcount_select; i++)  //在已知字段数量的情况下获取字段名  
	{
		str_field[i] = mysql_fetch_field(res)->name;
	}
	for (int i = 0; i < fieldcount_select; i++)  //打印字段  
		printf("%10s\t", str_field[i]);
	printf("---------------------------\n");

	while (column = mysql_fetch_row(res))   //在已知字段数量情况下，获取并打印下一行  
	{
		//vector<string> vec;
		for (int j = 0; j < fieldcount_select; j++)
		{
			//vector<string> vec;
			sprintf(current_c, "%s", column[j]);
			string result(current_c);
			vec.push_back(result);
		}
		//vec_result.push_back(vec);

	}

	return vec;

	//m_ColumnLenght = 0;
	//vector<string> vec;
	////MYSQL_RES *res; //这个结构代表返回行的一个查询结果集  
	//MYSQL_ROW column; //一个行数据的类型安全(type-safe)的表示，表示数据行的列 
	//char current_c[150]; //查询语句
	//string sqlstr, outPut;

	////char id[20];
	////string string_id(id);
	////sqlstr = "SELECT GROUP_CONCAT(COLUMN_NAME SEPARATOR '*') FROM information_schema.COLUMNS WHERE TABLE_SCHEMA = '";
	//sqlstr = "show databases;";

	//cout << sqlstr << endl;
	////sprintf_s(query, "select * from t_dept"); //执行查询语句，这里是查询所有，user是表名，不用加引号，用strcpy也可以  
	////mysql_query(&mydata, "set names gbk"); //设置编码格式（SET NAMES GBK也行），否则cmd下中文乱码  
	////返回0 查询成功，返回1查询失败  
	//if (mysql_query(m_mysql, sqlstr.c_str()))    //执行SQL语句
	//{
	//	printf("Query failed (%s)\n", mysql_error(m_mysql));
	//	//return false;
	//}
	//else
	//{
	//	printf("query success\n");
	//}
	////获取结果集  
	//if (!(res = mysql_store_result(m_mysql)))   //获得sql语句结束后返回的结果集  
	//{
	//	printf("Couldn't get result from %s\n", mysql_error(m_mysql));
	//	//return false;
	//}
	//unsigned int fieldcount_select = mysql_num_fields(res);
	////打印数据行数  
	//printf("number of dataline returned: %d\n", mysql_affected_rows(m_mysql));

	////获取字段的信息  
	//char *str_field[40];  //定义一个字符串数组存储字段信息  
	//for (int i = 0; i < fieldcount_select; i++)  //在已知字段数量的情况下获取字段名  
	//{
	//	str_field[i] = mysql_fetch_field(res)->name;
	//}
	//for (int i = 0; i < fieldcount_select; i++)  //打印字段  
	//	printf("%10s\t", str_field[i]);
	//printf("---------------------------\n");
	////打印获取的数据  
	//while (column = mysql_fetch_row(res))   //在已知字段数量情况下，获取并打印下一行  
	//{
	//	printf("%s", column[0]);  //column是列数组  
	//	sprintf(current_c, "%s", column[0]);
	//	string result(current_c);
	//	outPut += result;
	//	cout << result << endl;
	//	//result = column[0] + column[1] + column[2] + column[3] + column[4] + column[5] + column[6] + column[7], +column[8] + column[9] + column[10];
	//}
	////string result = clumn[0] + ;
	////vec = my_split_inMysql(outPut, "*");


	//return vec;
}
bool ConnectMysql::CreateNewTableAndInsert(string name,vector<string> columnName,vector<string> columnValue)//columnName是列名 columnValue是数据的vector
{
	if(CreateNewTable(name,columnName))
	{
		cout << "创建数据表成功";
	}else
	{
		cout << "创建数据表失败";
	}
	string sqlstr;
	//insert into 2022_9_20_14_25_43_s values("1","2022_9_20","11","12","13","14","15","16","17","18","19");
	sqlstr = "insert into ";
	sqlstr += name;
	sqlstr += " values('";
	//sqlstr += "u_id INT UNSIGNED NOT NULL AUTO_INCREMENT PRIMARY KEY COMMENT 'Unique User ID'";
	for(int i = 0; i < columnValue.size() - 1; i++)
	{

		sqlstr += columnValue[i];
		sqlstr += "','";
	}
	sqlstr += columnValue[columnValue.size() - 1];
	sqlstr += "');";
	if (mysql_query(m_mysql, sqlstr.c_str()) == 0)
	{
		cout << "mysql_query() insert data succeed" << endl;
	}
	else
	{
		cout << "mysql_query() insert data failed" << endl;
		mysql_close(m_mysql);
		return false;
	}
	return true;
}

MYSQL_RES* ConnectMysql::execSqlSelect(const std::string& sql)
{
	MYSQL_RES* result = nullptr;

	if (m_mysql == nullptr)
		return nullptr;
	if (mysql_real_query(m_mysql, sql.c_str(), (unsigned long)sql.size()) != 0)
	{
		std::string error = mysql_error(m_mysql);
		printf("SQL：%s\nError：%s", sql.c_str(), error.c_str());
		return nullptr;
	}
	else
	{
		result = mysql_store_result(m_mysql);
	}

	return result;
}

bool ConnectMysql::execSql(const std::string& sql)
{
	if (m_mysql == nullptr)
		return false;
	if (mysql_real_query(m_mysql, sql.c_str(), (unsigned long)sql.size()) != 0)
	{
		std::string error = mysql_error(m_mysql);
		printf("SQL：%s\nError：%s", sql.c_str(), error.c_str());
		return false;
	}

	return true;
}

bool ConnectMysql::execSql(uint32_t& lastId, const std::string& sql)
{
	if (m_mysql == nullptr)
		return false;
	if (mysql_real_query(m_mysql, sql.c_str(), (unsigned long)sql.size()) != 0)
	{
		std::string error = mysql_error(m_mysql);
		printf("SQL：%s\nError：%s", sql.c_str(), error.c_str());
		return false;
	}

	// 获取被更新行数id;
	lastId = (unsigned int)mysql_insert_id(m_mysql);

	return true;
}

bool ConnectMysql::startupTransaction()
{
	if (m_mysql == nullptr)
		return false;

	// 启动事务;
	if (mysql_real_query(m_mysql, "START TRANSACTION", (unsigned long)strlen("START TRANSACTION")))
	{
		std::string error = mysql_error(m_mysql);
		printf("启动事务失败!\nError: %s", error.c_str());
		return false;
	}

	return true;
}

bool ConnectMysql::commitTransaction()
{
	if (m_mysql == nullptr)
		return false;

	// 提交事务;
	if (mysql_real_query(m_mysql, "COMMIT", (unsigned long)strlen("COMMIT")))
	{
		std::string error = mysql_error(m_mysql);
		printf("提交事务失败!\nError: %s", error.c_str());
		return false;
	}

	return true;
}

bool ConnectMysql::rollbackTransaction()
{
	if (m_mysql == nullptr)
		return false;

	// 回滚事务;
	if (mysql_real_query(m_mysql, "ROLLBACK", (unsigned long)strlen("ROLLBACK")))
	{
		std::string error = mysql_error(m_mysql);
		printf("回滚事务失败!\nError: %s", error.c_str());
		return false;
	}

	return true;
}

bool ConnectMysql::isTableExists(const std::string& tableName)
{
	// 结果集声明;
	MYSQL_ROW sql_row;

	std::string str_sql = "SELECT COUNT(*) FROM information_schema.tables WHERE table_schema = DATABASE() AND table_name = '" + tableName + "'";

	MYSQL_RES* result = execSqlSelect(str_sql);
	if (result == nullptr)
		return false;

	int rowCount = 0;

	while (sql_row = mysql_fetch_row(result))
	{
		 rowCount = std::stoi(sql_row[0]);
	}

	//mysql_free_result(result);
	return (rowCount > 0);
}

bool ConnectMysql::isFieldExists(const std::string& tableName, const std::string& fieldName)
{
	std::string strSql = "SHOW COLUMNS FROM " + tableName + " LIKE '" + fieldName + "'";
	MYSQL_RES* result = execSqlSelect(strSql);
	if (result == nullptr)
	{
		return false;
	}

	return true;
}


bool ConnectMysql::createTableAndFields(const string& strTable, const std::vector<std::string>& fieldNames)
{
	if (isTableExists(strTable))
	{
		if (!delTable(strTable))
		{
			return false;
		}
	}

	// 启动事务;
	if (!startupTransaction())
		return false;

	std::string strSql = "CREATE TABLE `" + strTable + "` (";
	
	strSql += "id INT PRIMARY KEY AUTO_INCREMENT, ";
	for (int i = 0; i < fieldNames.size(); ++i) {
		strSql += fieldNames[i] + " VARCHAR(255)";
		if (i != fieldNames.size() - 1) {
			strSql += ", ";
		}
	}
	strSql += ", time TIMESTAMP DEFAULT CURRENT_TIMESTAMP)";  // 加入当前时间
	//strSql += ", time TIMESTAMP DEFAULT CURRENT_TIMESTAMP)";  // 加入当前时间

	if (!execSql(strSql))
	{
		// 回滚事务;
		if (!rollbackTransaction())
			return false;
		// 修改数据失败;
		return false;
	}

	// 提交事务;
	if (!commitTransaction())
		return false;

	// 根据需要看是否获取新插入的id
	//st.id = last_id;

	return true;
}

bool ConnectMysql::getFilelds(const std::string& table_name,  std::vector<std::string>& vecFilelds)
{
	vecFilelds.clear();

	// 结果集声明;
	MYSQL_ROW sql_row;

	// 执行SQL语句;
	
	//std::string str_sql = "SELECT COLUMN_NAME FROM INFORMATION_SCHEMA.COLUMNS WHERE TABLE_NAME = '" + table_name + "'";
	std::string str_sql = "DESC " + table_name;
	MYSQL_RES* result = execSqlSelect(str_sql);
	if (result == nullptr)
		return false;


	// 获取查询结果
	

	// 获取结果集的字段数
	unsigned int num_fields = mysql_num_fields(result);

	while (sql_row = mysql_fetch_row(result))
	{
		vecFilelds.push_back(sql_row[0]);
	}
	return true;
}

bool ConnectMysql::createTableAndFields(const std::string& strTable, const std::set<std::string>& fieldNames)
{
	if (isTableExists(strTable))
	{
		if (!delTable(strTable))
		{
			return false;
		}
	}

	// 启动事务;
	if (!startupTransaction())
		return false;

	std::string strSql = "CREATE TABLE `" + strTable + "` (";

	strSql += "id INT PRIMARY KEY AUTO_INCREMENT, ";
	int i = 0;
	for (const auto& value : fieldNames) 
	{
		strSql += "`" +value + "` VARCHAR(255)";
		//strSql += "\"" + value + "\" VARCHAR(255)";
		//strSql += value + " VARCHAR(255)";
		if (i != fieldNames.size() - 1) 
		{
			strSql += ", ";
		}
		i++;
	}
	
	strSql += ", time TIMESTAMP DEFAULT CURRENT_TIMESTAMP)";  // 加入当前时间
	//strSql += ", time TIMESTAMP(6) DEFAULT CURRENT_TIMESTAMP(6))";  // 加入当前时间

	if (!execSql(strSql))
	{
		// 回滚事务;
		if (!rollbackTransaction())
			return false;
		// 修改数据失败;
		return false;
	}

	// 提交事务;
	if (!commitTransaction())
		return false;

	// 根据需要看是否获取新插入的id
	//st.id = last_id;

	return true;
}



bool ConnectMysql::insertTableData(const std::string& tableName, const std::vector<std::string>& vecFilelds, const std::vector<std::string>& vecFileldsValue)
{
	if (vecFilelds.size() != vecFileldsValue.size())
		return false;

	// 启动事务;
	if (!startupTransaction())
		return false;

	uint32_t last_id = 0;

	// 构造 SQL 插入语句
	//std::string query = "INSERT INTO " + tableName +" (";
	std::string query = "INSERT INTO `" + tableName + "` (";

	for (size_t i = 0; i < vecFilelds.size(); ++i)
	{
		//query += vecFilelds[i];
		query += "`" + vecFilelds[i] + "`";
		if (i != vecFilelds.size() - 1) 
		{
			query += ",";
		}
	}

	query += ") VALUES (";

	for (size_t i = 0; i < vecFileldsValue.size(); ++i) {
		query += "'" + vecFileldsValue[i] + "'";
		//query += "`" + vecFileldsValue[i] + "`";
		if (i != vecFileldsValue.size() - 1) {
			query += ",";
		}
	}

	query += ")";

	if (!execSql(last_id, query))
	{
		// 回滚事务;
		if (!rollbackTransaction())
			return false;
		// 修改数据失败;
		return false;
	}

	// 提交事务;
	if (!commitTransaction())
		return false;

	// 根据需要看是否获取新插入的id
	//st.id = last_id;

	return true;
}

bool ConnectMysql::delTable(const std::string& tableName)
{

	// 启动事务;
	if (!startupTransaction())
		return false;

	// 构造 SQL 删除表语句
	std::string sql = "DROP TABLE "+ tableName;

	// 执行删除表操作
	if (!execSql(sql))
	{
		// 回滚事务;
		if (!rollbackTransaction())
			return false;
		// 修改数据失败;
		return false;
	}

	// 提交事务;
	if (!commitTransaction())
		return false;

	return true;
}


bool ConnectMysql::getAllScheme(std::map<int,stScheme> & listStData)
{
	listStData.clear();

	// 结果集声明;
	MYSQL_ROW sql_row;

	// 执行SQL语句;
	std::string str_sql = "select * from scheme order by id asc";

	MYSQL_RES* result = execSqlSelect(str_sql);
	if (result == nullptr)
		return false;

	stScheme st;
	while (sql_row = mysql_fetch_row(result))
	{
		st.id = std::atoi(sql_row[0]);
		st.schemeName = sql_row[1];
	
		listStData[st.id] = st;
	}
	return true;
}

bool ConnectMysql::isExisrSchemName(bool& bExist, const std::string& schemeName)
{
	std::string str_sql = "SELECT COUNT(*) FROM scheme WHERE schemeName = '"+schemeName+"'";

	MYSQL_RES* result = execSqlSelect(str_sql);
	if (result == nullptr) {
		return false;
	}

	MYSQL_ROW sql_row = mysql_fetch_row(result);
	int count = atoi(sql_row[0]);

	if (count > 0) 
	{
		bExist = true;
	}
	else 
	{
		bExist = false;
	}

	return true;
}

bool ConnectMysql::addScheme(const std::string& schemeName)
{
	// 启动事务;
	if (!startupTransaction())
		return false;

	uint32_t last_id = 0;

	// 执行SQL语句;
	char sql[256] = { 0 };
	sprintf_s(sql, "insert into scheme(schemeName) values('%s')",
		schemeName.c_str());

	if (!execSql(last_id, sql))
	{
		// 回滚事务;
		if (!rollbackTransaction())
			return false;
		// 修改数据失败;
		return false;
	}

	// 提交事务;
	if (!commitTransaction())
		return false;

	// 根据需要看是否获取新插入的id
	//st.id = last_id;

	return true;
}



bool ConnectMysql::getAllXzsjb(std::map<int,stXZSJB>& listStData)
{
	listStData.clear();

	// 结果集声明;
	MYSQL_ROW sql_row;

	// 执行SQL语句;
	std::string str_sql = "select * from 0_星座数据表_1 order by id asc";

	MYSQL_RES* result = execSqlSelect(str_sql);
	if (result == nullptr)
		return false;

	stXZSJB st;
	while (sql_row = mysql_fetch_row(result))
	{
		st.id = std::atoi(sql_row[0]);
		st.constellationName   = sql_row[1];
		st.constellationNation = sql_row[2];
		st.constellationCorp   = sql_row[3];
		st.constellationYear   = sql_row[4];
		st.constellationPhase  = sql_row[5];
		st.satNum				= sql_row[6];
		st.COL                 = sql_row[7];
		st.satName				= sql_row[8];
		st.planeNum				= sql_row[9];
		st.schemeID		 = std::atoi(sql_row[10]);
		
		listStData[st.id] = st;
	}
	return true;
}

bool ConnectMysql::getAllXzsjbBySchemeID(std::map<int,stXZSJB>& listStData, const int& schemeID)
{

	listStData.clear();

	// 结果集声明;
	MYSQL_ROW sql_row;

	// 执行SQL语句;where id = '%d' ORDER BY id Desc
//	std::string str_sql = "select * from 0_星座数据表_1 where id = '%d' order by id asc";
	char sql[1024] = { 0 };
	sprintf_s(sql, "select * from 0_星座数据表_1 where schemeID = '%d' ORDER BY id asc", schemeID);

	MYSQL_RES* result = execSqlSelect(sql);
	if (result == nullptr)
		return false;

	stXZSJB st;
	while (sql_row = mysql_fetch_row(result))
	{
		st.id = std::atoi(sql_row[0]);
		st.constellationName = sql_row[1];
		st.constellationNation = sql_row[2];
		st.constellationCorp = sql_row[3];
		st.constellationYear = sql_row[4];
		st.constellationPhase = sql_row[5];
		st.satNum = sql_row[6];
		st.COL = sql_row[7];
		st.satName = sql_row[8];
		st.planeNum = sql_row[9];
		st.schemeID = std::atoi(sql_row[10]);

		listStData[st.id] = st;
	}
	return true;
}

bool ConnectMysql::getAllDxsjb(std::map<int,stDXSJB>& listStData)
{
	listStData.clear();

	// 结果集声明;
	MYSQL_ROW sql_row;

	// 执行SQL语句;
	std::string str_sql = "select * from 0_单星数据表_1 order by id asc";

	MYSQL_RES* result = execSqlSelect(str_sql);
	if (result == nullptr)
		return false;

	stDXSJB st;
	while (sql_row = mysql_fetch_row(result))
	{
		st.id = std::atoi(sql_row[0]);
		st.satName = sql_row[1];
		st.xzID = std::atoi(sql_row[2]);
		st.constellationName = sql_row[3];
		st.satNation = sql_row[4];
		st.satCorp = sql_row[5];
		st.satType = sql_row[6];
		st.orbitType = sql_row[7];
		st.NORAD = sql_row[8];
		st.COPAR = sql_row[9];
		st.isPublic = std::string(sql_row[10]);
		st.SOL = std::string(sql_row[11]);
		st.satSemiMajor = (sql_row[12]);
		st.satEcc = std::string(sql_row[13]);
		st.satOblique = (sql_row[14]);
		st.satRAAN = std::string(sql_row[15]);
		st.satOmega = std::string(sql_row[16]);
		st.satTrueAnomaly = std::string(sql_row[17]);
		st.satLoadName = std::string(sql_row[18]);
		st.satAntennaName = std::string(sql_row[19]);
		st.schemeID = std::atoi(sql_row[20]);

		listStData[st.id] = st;

	}
	return true;
}

bool ConnectMysql::getAllDxsjbBySchemeID(std::map<int,stDXSJB>& listStData, const int& schemeID)
{
	listStData.clear();

	// 结果集声明;
	MYSQL_ROW sql_row;

	char sql[1024] = { 0 };
	sprintf_s(sql, "select * from 0_单星数据表_1 where schemeID = '%d'  ORDER BY id asc", schemeID);

	MYSQL_RES* result = execSqlSelect(sql);
	if (result == nullptr)
		return false;

	stDXSJB st;
	while (sql_row = mysql_fetch_row(result))
	{
		st.id = std::atoi(sql_row[0]);
		st.satName = sql_row[1];
		st.xzID = std::atoi(sql_row[2]);
		st.constellationName = sql_row[3];
		st.satNation = sql_row[4];
		st.satCorp = sql_row[5];
		st.satType = sql_row[6];
		st.orbitType = sql_row[7];
		st.NORAD = sql_row[8];
		st.COPAR = sql_row[9];
		st.isPublic = std::string(sql_row[10]);
		st.SOL = std::string(sql_row[11]);
		st.satSemiMajor = (sql_row[12]);
		st.satEcc = std::string(sql_row[13]);
		st.satOblique = (sql_row[14]);
		st.satRAAN = std::string(sql_row[15]);
		st.satOmega = std::string(sql_row[16]);
		st.satTrueAnomaly = std::string(sql_row[17]);
		st.satLoadName = std::string(sql_row[18]);
		st.satAntennaName = std::string(sql_row[19]);
		st.schemeID = std::atoi(sql_row[20]);

		listStData[st.id] = st;

	}
	return true;
}

bool ConnectMysql::getDxsjbBySchemeIDAndXzID(std::map<int, stDXSJB>& mapStData, const int& schemeID, const int& XzID)
{
	mapStData.clear();

	// 结果集声明;
	MYSQL_ROW sql_row;

	char sql[1024] = { 0 };
	sprintf_s(sql, "select * from 0_单星数据表_1 where schemeID = '%d' AND xzID = '%d' ORDER BY id asc", schemeID, XzID);

	MYSQL_RES* result = execSqlSelect(sql);
	if (result == nullptr)
		return false;

	stDXSJB st;
	while (sql_row = mysql_fetch_row(result))
	{
		st.id = std::atoi(sql_row[0]);
		st.satName = sql_row[1];
		st.xzID = std::atoi(sql_row[2]);
		st.constellationName = sql_row[3];
		st.satNation = sql_row[4];
		st.satCorp = sql_row[5];
		st.satType = sql_row[6];
		st.orbitType = sql_row[7];
		st.NORAD = sql_row[8];
		st.COPAR = sql_row[9];
		st.isPublic = std::string(sql_row[10]);
		st.SOL = std::string(sql_row[11]);
		st.satSemiMajor = (sql_row[12]);
		st.satEcc = std::string(sql_row[13]);
		st.satOblique = (sql_row[14]);
		st.satRAAN = std::string(sql_row[15]);
		st.satOmega = std::string(sql_row[16]);
		st.satTrueAnomaly = std::string(sql_row[17]);
		st.satLoadName = std::string(sql_row[18]);
		st.satAntennaName = std::string(sql_row[19]);
		st.schemeID = std::atoi(sql_row[20]);

		mapStData[st.id] = st;

	}
	return true;
}



bool ConnectMysql::getAllWXTXB(std::map<int,stWXTXB>& listStData)
{
	listStData.clear();

	// 结果集声明;
	MYSQL_ROW sql_row;

	// 执行SQL语句;
	std::string str_sql = "select * from 0_卫星天线_1 order by id asc";

	MYSQL_RES* result = execSqlSelect(str_sql);
	if (result == nullptr)
		return false;

	stWXTXB st;
	while (sql_row = mysql_fetch_row(result))
	{
		st.id = std::atoi(sql_row[0]);
		st.satAntennaName = sql_row[1];
		st.dxID = std::atoi(sql_row[2]);
		st.satName = sql_row[3];
		st.satAntennaType = sql_row[4];
		st.satAntennaCaliber = sql_row[5];
		st.satAntennaGain = sql_row[6];
		st.satAntennaEff = sql_row[7];
		st.satAntennaPointerLoss = sql_row[8];
		st.satAntennaPolarLoss = sql_row[9];
		st.schemeID = std::atoi(sql_row[10]);

		listStData[st.id] = st;
	}
	return true;
}

bool ConnectMysql::getAllWxtxbBySchemeID(std::map<int,stWXTXB>& listStData, const int& schemeID)
{
	listStData.clear();

	// 结果集声明;
	MYSQL_ROW sql_row;

	// 执行SQL语句;
	char sql[1024] = { 0 };
	sprintf_s(sql, "select * from  0_卫星天线_1 where schemeID = '%d' ORDER BY id asc", schemeID);

	MYSQL_RES* result = execSqlSelect(sql);
	if (result == nullptr)
		return false;

	stWXTXB st;
	while (sql_row = mysql_fetch_row(result))
	{
		st.id = std::atoi(sql_row[0]);
		st.satAntennaName = sql_row[1];
		st.dxID = std::atoi(sql_row[2]);
		st.satName = sql_row[3];
		st.satAntennaType = sql_row[4];
		st.satAntennaCaliber = sql_row[5];
		st.satAntennaGain = sql_row[6];
		st.satAntennaEff = sql_row[7];
		st.satAntennaPointerLoss = sql_row[8];
		st.satAntennaPolarLoss = sql_row[9];
		st.schemeID = std::atoi(sql_row[10]);

		listStData[st.id] = st;
	}
	return true;
}

bool ConnectMysql::getAllWXZHSJB(std::map<int,stWXZHSJB>& listStData)
{
	listStData.clear();

	// 结果集声明;
	MYSQL_ROW sql_row;

	// 执行SQL语句;
	std::string str_sql = "select * from 0_卫星载荷数据表_1 order by id asc";

	MYSQL_RES* result = execSqlSelect(str_sql);
	if (result == nullptr)
		return false;

	stWXZHSJB st;
	while (sql_row = mysql_fetch_row(result))
	{
		st.id = std::atoi(sql_row[0]);
		st.satLoadName = sql_row[1];
		st.dxID = std::atoi(sql_row[2]);
		st.satLoadWaveBand = sql_row[3];
		st.satLoadCorp = sql_row[4];
		st.satLoadSendEIRP = sql_row[5];
		st.satLoadSendSFD = sql_row[6];
		st.satLoadRecvGT = sql_row[7];
		st.satLoadBandwidth = sql_row[8];
		st.schemeID = std::atoi(sql_row[9]);

		listStData[st.id] = st;
	}
	return true;
}

bool ConnectMysql::getAllWxzhsjbBySchemeID(std::map<int,stWXZHSJB>& listStData, const int& schemeID)
{
	listStData.clear();

	// 结果集声明;
	MYSQL_ROW sql_row;

	// 执行SQL语句;
	//std::string str_sql = "select * from 0_卫星载荷数据表_1 order by id asc";
	char sql[1024] = { 0 };
	sprintf_s(sql, "select * from 0_卫星载荷数据表_1 where schemeID = '%d' ORDER BY id asc", schemeID);

	MYSQL_RES* result = execSqlSelect(sql);
	if (result == nullptr)
		return false;

	stWXZHSJB st;
	while (sql_row = mysql_fetch_row(result))
	{
		st.id = std::atoi(sql_row[0]);
		st.satLoadName = sql_row[1];
		st.dxID = std::atoi(sql_row[2]);
		st.satLoadWaveBand = sql_row[3];
		st.satLoadCorp = sql_row[4];
		st.satLoadSendEIRP = sql_row[5];
		st.satLoadSendSFD = sql_row[6];
		st.satLoadRecvGT = sql_row[7];
		st.satLoadBandwidth = sql_row[8];
		st.schemeID = std::atoi(sql_row[9]);

		listStData[st.id] = st;
	}
	return true;
}

bool ConnectMysql::getAllDxgsjbBySchemeID(std::map<int,stDXGSJB>& listStData, const int& schemeID)
{
	listStData.clear();

	// 结果集声明;
	MYSQL_ROW sql_row;

	// 执行SQL语句;
	char sql[1024] = { 0 };
	sprintf_s(sql, "select * from 1_电信港数据表_1 where schemeID = '%d' ORDER BY id asc", schemeID);

	MYSQL_RES* result = execSqlSelect(sql);
	if (result == nullptr)
		return false;

	stDXGSJB st;
	while (sql_row = mysql_fetch_row(result))
	{
		st.id = std::atoi(sql_row[0]);
		st.teleportName = sql_row[1];
		st.teleportNation = (sql_row[2]);
		st.teleportCorp = sql_row[3];
		st.teleportType = sql_row[4];
		st.teleportYear = sql_row[5];
		st.teleportLon = sql_row[6];
		st.teleportLat = sql_row[7];
		st.stationName = sql_row[8];
		st.schemeID = std::atoi(sql_row[9]);

		listStData[st.id] = st;
	}
	return true;
}

bool ConnectMysql::getAllDqzsjbBySchemeID(std::map<int,stDQZSJB>& listStData, const int& schemeID)
{
	listStData.clear();

	// 结果集声明;
	MYSQL_ROW sql_row;

	// 执行SQL语句;
	char sql[1024] = { 0 };
	sprintf_s(sql, "select * from 1_地球站数据表_1 where schemeID = '%d' ORDER BY id asc", schemeID);

	MYSQL_RES* result = execSqlSelect(sql);
	if (result == nullptr)
		return false;

	stDQZSJB st;
	while (sql_row = mysql_fetch_row(result))
	{
		st.id = std::atoi(sql_row[0]);
		st.stationName = sql_row[1];
		st.dxgID = std::atoi(sql_row[2]);
		st.teleportName = sql_row[3];
		st.stationCorp = sql_row[4];
		st.stationType = sql_row[5];
		st.stationLon = sql_row[6];
		st.stationLat = sql_row[7];
		st.stationAlt = sql_row[8];
		st.stationLoadName = sql_row[9];
		st.stationWaveName = sql_row[10];
		st.stationAntennaName = sql_row[11];
		st.schemeID = std::atoi(sql_row[12]);

		listStData[st.id]=st;
	}
	return true;
}

bool ConnectMysql::getAllDqzkysjbBySchemeID(std::map<int,stDQZKYSJB>& listStData, const int& schemeID)
{
	listStData.clear();

	// 结果集声明;
	MYSQL_ROW sql_row;

	// 执行SQL语句;
	char sql[1024] = { 0 };
	sprintf_s(sql, "select * from 1_地球站馈源数据表_1 where schemeID = '%d' ORDER BY id asc", schemeID);

	MYSQL_RES* result = execSqlSelect(sql);
	if (result == nullptr)
		return false;

	stDQZKYSJB st;
	while (sql_row = mysql_fetch_row(result))
	{
		st.id = std::atoi(sql_row[0]);
		st.stationLoadName = sql_row[1];
		st.stationID = std::atoi(sql_row[2]);
		st.stationLoadWaveBand = sql_row[3];
		st.stationLoadCorp = sql_row[4];
		st.stationLoadSendPower = sql_row[5];
		st.stationLoadSendCarrierFreq = sql_row[6];
		st.stationLoadSendFEC = sql_row[7];
		st.stationLoadUpBPS = sql_row[8];
		st.stationLoadName = sql_row[9];
		st.stationLoadRecvGT = sql_row[10];
		st.stationLoadRecvCarrierFreq = sql_row[11];
		st.stationLoadDownBPS = sql_row[12];
		st.schemeID = std::atoi(sql_row[13]);

		listStData[st.id] = st;
	}
	return true;
}

bool ConnectMysql::getAllDqzbxsbsjbBySchemeID(std::map<int,stDQZBXSBSJB>& listStData, const int& schemeID)
{
	listStData.clear();

	// 结果集声明;
	MYSQL_ROW sql_row;

	// 执行SQL语句;
	char sql[1024] = { 0 };
	sprintf_s(sql, "select * from 1_地球站波形设备数据表_1 where schemeID = '%d' ORDER BY id asc", schemeID);

	MYSQL_RES* result = execSqlSelect(sql);
	if (result == nullptr)
		return false;

	stDQZBXSBSJB st;
	while (sql_row = mysql_fetch_row(result))
	{
		st.id = std::atoi(sql_row[0]);
		st.stationWaveName = sql_row[1];
		st.stationID = std::atoi(sql_row[2]);
		st.stationName = sql_row[3];
		st.stationWaveModel = sql_row[4];
		st.stationWaveModType = sql_row[5];
		st.stationWaveDemoType = sql_row[6];
		st.stationWaveCodingType = sql_row[7];
		st.stationWaveCarrierType = sql_row[8];
		st.stationWaveBandwidth = sql_row[9];
		st.stationWaveType = sql_row[10];
		st.schemeID = std::atoi(sql_row[11]);

		listStData[st.id] = st;
	}
	return true;
}

bool ConnectMysql::getAllDqztxbBySchemeID(std::map<int,stDQTXB>& listStData, const int& schemeID)
{
	listStData.clear();

	// 结果集声明;
	MYSQL_ROW sql_row;

	// 执行SQL语句;
	char sql[1024] = { 0 };
	sprintf_s(sql, "select * from 1_地球站天线_1 where schemeID = '%d' ORDER BY id asc", schemeID);

	MYSQL_RES* result = execSqlSelect(sql);
	if (result == nullptr)
		return false;

	stDQTXB st;
	while (sql_row = mysql_fetch_row(result))
	{
		st.id = std::atoi(sql_row[0]);
		st.stationAntennaName = sql_row[1];
		st.stationID = std::atoi(sql_row[2]);
		st.stationName = sql_row[3];
		st.stationAntennaType = sql_row[4];
		st.stationAntennaCaliber = sql_row[5];
		st.stationAntennaGain = sql_row[6];
		st.stationAntennaEff = sql_row[7];
		st.stationAntennaPointerLoss = sql_row[8];
		st.stationAntennaPolarLoss = sql_row[9];
		st.schemeID = std::atoi(sql_row[10]);

		listStData[st.id] = st;
	}
	return true;
}

string ConnectMysql::mytest_QueryByColumnName(string name_Table,string columnName)
{
	//select u_id,u_time,u_X from 动力学_2022_10_09_09_47_51_593;
	//MYSQL_RES *res; //这个结构代表返回行的一个查询结果集  
	MYSQL_ROW column; //一个行数据的类型安全(type-safe)的表示，表示数据行的列 
	char current_c[150]; //查询语句
	string sqlstr, outPut;

	//char id[20];
	//string string_id(id);
	sqlstr = "select u_id,u_time,";
	sqlstr += columnName;
	sqlstr += " from ";
	sqlstr += name_Table;
	sqlstr += ";";
	cout << sqlstr << endl;
	//sprintf_s(query, "select * from t_dept"); //执行查询语句，这里是查询所有，user是表名，不用加引号，用strcpy也可以  
	//mysql_query(&mydata, "set names gbk"); //设置编码格式（SET NAMES GBK也行），否则cmd下中文乱码  
	//返回0 查询成功，返回1查询失败  
	if (mysql_query(m_mysql, sqlstr.c_str()))    //执行SQL语句
	{
		printf("Query failed (%s)\n", mysql_error(m_mysql));
		//return false;
	}
	else
	{
		printf("query success\n");
	}
	//获取结果集  
	if (!(res = mysql_store_result(m_mysql)))   //获得sql语句结束后返回的结果集  
	{
		printf("Couldn't get result from %s\n", mysql_error(m_mysql));
		//return false;
	}
	unsigned int fieldcount_select = mysql_num_fields(res);
	//打印数据行数  
	printf("number of dataline returned: %d\n", mysql_affected_rows(m_mysql));

	//获取字段的信息  
	char *str_field[40];  //定义一个字符串数组存储字段信息  
	for (int i = 0; i < fieldcount_select; i++)  //在已知字段数量的情况下获取字段名  
	{
		str_field[i] = mysql_fetch_field(res)->name;
	}
	for (int i = 0; i < fieldcount_select; i++)  //打印字段  
		printf("%10s\t", str_field[i]);
	printf("---------------------------\n");
	//打印获取的数据  
	while (column = mysql_fetch_row(res))   //在已知字段数量情况下，获取并打印下一行  
	{
		printf("%10s\t%10s\t%10s", column[0], column[1], column[2]);  //column是列数组  
		sprintf(current_c, "%s*%s*%s!", column[0], column[1], column[2]);
		string result(current_c);
		outPut += result;
		cout << result;
		//result = column[0] + column[1] + column[2] + column[3] + column[4] + column[5] + column[6] + column[7], +column[8] + column[9] + column[10];
	}
	//string result = clumn[0] + ;

	return outPut;
}

vector<vector<string>> ConnectMysql::mytest_QueryByColumnName(string name_Table, string columnName, string data)
{
	vector<vector<string>> vec_result;
	MYSQL_ROW column; //一个行数据的类型安全(type-safe)的表示，表示数据行的列 
	char current_c[150]; //查询语句
	string sqlstr, outPut;
	//select * from 0_星座数据表_1 where constellationName="StarLink";
	//char id[20];
	//string string_id(id);
	sqlstr = "select * ";
	sqlstr += " from ";
	sqlstr += name_Table;
	sqlstr += " where ";
	sqlstr += columnName;
	sqlstr += "='";
	sqlstr += data;
	sqlstr += "'";
	cout << sqlstr << endl;
	//sprintf_s(query, "select * from t_dept"); //执行查询语句，这里是查询所有，user是表名，不用加引号，用strcpy也可以  
	//mysql_query(&mydata, "set names gbk"); //设置编码格式（SET NAMES GBK也行），否则cmd下中文乱码  
	//返回0 查询成功，返回1查询失败  
	if (mysql_query(m_mysql, sqlstr.c_str()))    //执行SQL语句
	{
		printf("Query failed (%s)\n", mysql_error(m_mysql));
		//return false;
	}
	else
	{
		printf("query success\n");
	}
	//获取结果集  
	if (!(res = mysql_store_result(m_mysql)))   //获得sql语句结束后返回的结果集  
	{
		printf("Couldn't get result from %s\n", mysql_error(m_mysql));
		//return false;
	}
	unsigned int fieldcount_select = mysql_num_fields(res);
	//打印数据行数  
	printf("number of dataline returned: %d\n", mysql_affected_rows(m_mysql));

	//获取字段的信息  
	char *str_field[40];  //定义一个字符串数组存储字段信息  
	for (int i = 0; i < fieldcount_select; i++)  //在已知字段数量的情况下获取字段名  
	{
		str_field[i] = mysql_fetch_field(res)->name;
	}
	for (int i = 0; i < fieldcount_select; i++)  //打印字段  
		printf("%10s\t", str_field[i]);
	printf("---------------------------\n");
	//打印获取的数据  


	while (column = mysql_fetch_row(res))   //在已知字段数量情况下，获取并打印下一行  
	{
		vector<string> vec;
		for (int j = 0; j < fieldcount_select; j++)
		{
			//vector<string> vec;
			sprintf(current_c, "%s", column[j]);
			string result(current_c);
			vec.push_back(result);
		}
		vec_result.push_back(vec);

	}

	return vec_result;

}
//select u_id,u_time,u_X from 转台_2022_10_09_09_39_43_545;
vector<vector<string>> ConnectMysql::mytest_QueryByColumnName(string name_Table,vector<string> vec)
{
	vector<vector<string>> vec_result;
	MYSQL_RES *res; //这个结构代表返回行的一个查询结果集  
	MYSQL_ROW column; //一个行数据的类型安全(type-safe)的表示，表示数据行的列 
	char current_c[150]; //查询语句
	string sqlstr,outPut;

	//char id[20];
	//string string_id(id);
	sqlstr = "select ";
	for(int i = 0; i < vec.size() - 1; i++)
	{
		sqlstr += vec[i];
		sqlstr += ",";
	}
	sqlstr += vec[vec.size() - 1];
	sqlstr += " from ";
	sqlstr += name_Table;
	sqlstr += ";";
	//sqlstr += "u_id INT UNSIGNED NOT NULL AUTO_INCREMENT PRIMARY KEY COMMENT 'Unique User ID'";

	//sprintf_s(query, "select * from t_dept"); //执行查询语句，这里是查询所有，user是表名，不用加引号，用strcpy也可以  
	//mysql_query(&mydata, "set names gbk"); //设置编码格式（SET NAMES GBK也行），否则cmd下中文乱码  
	//返回0 查询成功，返回1查询失败  
	if (mysql_query(m_mysql, sqlstr.c_str()))    //执行SQL语句
	{
		printf("Query failed (%s)\n", mysql_error(m_mysql));
		//return false;
	}
	else
	{
		printf("query success\n");
	}
	//获取结果集  
	if (!(res = mysql_store_result(m_mysql)))   //获得sql语句结束后返回的结果集  
	{
		printf("Couldn't get result from %s\n", mysql_error(m_mysql));
		//return false;
	}
	unsigned int fieldcount_select = mysql_num_fields(res);
	//unsigned int affect_row = mysql_affected_rows(m_mysql);
	//打印数据行数  
	printf("number of dataline returned: %d\n", mysql_affected_rows(m_mysql));//

	//获取字段的信息  
	char *str_field[40];  //定义一个字符串数组存储字段信息  
	for (int i = 0; i < fieldcount_select; i++)  //在已知字段数量的情况下获取字段名  
	{
		str_field[i] = mysql_fetch_field(res)->name;
	}
	for (int i = 0; i < fieldcount_select; i++)  //打印字段  
		printf("%10s\t", str_field[i]);
	printf("---------------------------\n");

	while (column = mysql_fetch_row(res))   //在已知字段数量情况下，获取并打印下一行  
	{
		vector<string> vec;
		for (int j = 0; j < fieldcount_select; j++)
		{
			//vector<string> vec;
			sprintf(current_c, "%s", column[j]);
			string result(current_c);
			vec.push_back(result);
		}
		vec_result.push_back(vec);

	}

	return vec_result;
}