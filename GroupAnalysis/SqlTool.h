#pragma once
#include <stdio.h>
#include <WinSock.h>  //һ��Ҫ�������������winsock2.h
#include <Windows.h>
#include <string>
#include "mysql.h"

class SqlTool
{
private:

public:
	static MYSQL mysql;
	static MYSQL_RES *res;
	static bool connectDB();
	static bool operationExcutor(const char* operation, MYSQL_RES* &res);
	static bool insertExcutor(const char* operation);
	static char* getVariableFromDB(const char* operation);
	//char* uuidGenerator(MYSQL_RES* &res);
	static char* datetimeConvertor(int input);
	SqlTool();
	~SqlTool();
};
