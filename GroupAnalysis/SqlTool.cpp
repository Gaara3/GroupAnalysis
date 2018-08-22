#include "SqlTool.h"
#include "ReadConfig.h"
#include "Define.h"
#include "Log.h"
#include <string>
#include <time.h>


SqlTool::SqlTool()
{
}

MYSQL SqlTool::mysql;
MYSQL_RES* SqlTool::res;
MYSQL_ROW SqlTool::column;

SqlTool::~SqlTool()
{
	mysql_free_result(res);
	mysql_close(&mysql);
}

bool SqlTool::connectDB() {//TODO   封装传入参数
	string host, user, passwd, db;
	int port = 0;
	CReadCfg cfg;
	cfg.ReadConfig("HostIP", host);
	cfg.ReadConfig("Port", port);
	cfg.ReadConfig("UserName", user);
	cfg.ReadConfig("Password", passwd);
	cfg.ReadConfig("DatabaseName", db);


	MysqlInfo info;
	strcpy_s(info.host, host.c_str());
	strcpy_s(info.user, user.c_str());
	strcpy_s(info.passwd, passwd.c_str());
	strcpy_s(info.db, db.c_str());
	info.port = port;
	info.client_flag = 0;

	mysql_init(&mysql);
	if (!mysql_real_connect(&mysql,host.data(), user.data(), passwd.data(),db.data(), port, NULL, 0)) {
		SL_LOG("Something wrong when connecting to the Database:%s", mysql_error(&mysql));
		return false;
	}
	SL_LOG("Connected to the database!");
	return true;
}

void SqlTool::freeRes()
{
	return mysql_free_result(res);
}

bool SqlTool::operationExcutor(const char* operation, MYSQL_RES* &res) {
	//printf("SQL:%s\n", operation);
	if (mysql_query(&mysql, operation))        //执行SQL语句  
	{
		SL_LOG("operation failed (%s)\n", mysql_error(&mysql));
		//printf("SQL:%s\n", operation);
		return false;
	}
	res = mysql_store_result(&mysql);
	return true;
}

bool SqlTool::insertExcutor(const char* operation) {
	if (mysql_query(&mysql, operation))        //执行SQL语句  
	{
		SL_LOG("operation failed (%s)\n", mysql_error(&mysql));
		//printf("SQL:%s\n", operation);
		return false;
	}
	return true;
}

char* SqlTool::getVariableFromDB(const char* operation) {	//获取统计信息max、min或数据库信息version、char等   单一返回值
	mysql_query(&mysql, operation);
	res = mysql_store_result(&mysql);
	int resCount = mysql_num_rows(res);	//结果为空
	MYSQL_ROW column = mysql_fetch_row(res);

	return column[0] ==nullptr? const_cast<char*>("0") :column[0];
}

char* SqlTool::datetimeConvertor(int input) {
	time_t t = input;
	tm ltm;
	localtime_s(&ltm, &t);
	char* res = new char[20];
	sprintf_s(res, 20, "%4d-%02d-%02d %02d:%02d:%02d", 1900 + ltm.tm_year, 1 + ltm.tm_mon, ltm.tm_mday, ltm.tm_hour, ltm.tm_min, ltm.tm_sec);
	return res;
}

bool SqlTool::setCharSetEncoding(const char* CharSetEncoding) {
	char sql[256] = { 0 };
	sprintf_s(sql, 256, "set names  %s; ", CharSetEncoding);
	//strcpy_s(sql, "set names gbk");
	if (mysql_query(&mysql, sql))        //执行SQL语句  
	{
		SL_LOG("operation failed (%s)\n", mysql_error(&mysql));
		//printf("SQL:%s\n", sql);
		return false;
	}
}