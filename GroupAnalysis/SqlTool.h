#pragma once
//#include "stdafx.h"
#include <stdio.h>
#include <WinSock.h>  //一定要包含这个，或者winsock2.h
#include <Windows.h>
#include <string>
#include "mysql.h"

class SqlTool
{
private:

public:
	static MYSQL mysql;
	static MYSQL_RES *res;
	static MYSQL_ROW column;

	static bool connectDB();
	static void freeRes();
	static bool operationExcutor(const char* operation, MYSQL_RES* &res);
	static bool insertExcutor(const char* operation);
	static char* getVariableFromDB(const char* operation);
	//char* uuidGenerator(MYSQL_RES* &res);
	static char* datetimeConvertor(int input);
	static bool setCharSetEncoding(const char* CharSetEncoding);
	SqlTool();
	~SqlTool();
};

