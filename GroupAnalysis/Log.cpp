//#include "StdAfx.h"
#include "Log.h"
#include <Windows.h>

static int g_log_num = 0;
CLog g_log;
CLog::CLog(void)
{
	wchar_t exeFullPath[MAX_PATH] = { 0 };
	GetModuleFileName(NULL, exeFullPath, MAX_PATH);

	char CharStr[MAX_PATH] = { 0 };
	size_t convertedChars = 0;
	wcstombs_s(&convertedChars, CharStr, MAX_PATH, exeFullPath, _TRUNCATE);

	string fileName = GetFileName();

	m_logPath = (string)CharStr;
	size_t pos = m_logPath.find_last_of('\\', m_logPath.length());
	m_logPath = m_logPath.substr(0, pos);
	m_logPath += "\\Log\\" + fileName + ".log";
}

CLog::~CLog(void)
{
}

void CLog::WriteLog(const char* pSourcePath, const char* pFunName, const long lLine, const string pLogText)
{
	if (pLogText.empty())
		return;

	FILE* fp = NULL;
	fopen_s(&fp, m_logPath.c_str(), "a");
	if (NULL == fp)
		return;

	//日志编号
	char szNum[10] = { 0 };
	sprintf_s(szNum, "[%d]", ++g_log_num);
	fwrite(szNum, strlen(szNum), 1, fp);
	fwrite(" ", 1, 1, fp);

	//日志时间
	string strTime = GetTime();
	char szTime[32] = { 0 };
	sprintf_s(szTime, "[%s]", strTime.c_str());
	fwrite(szTime, strlen(szTime), 1, fp);
	fwrite(" ", 1, 1, fp);

	//打印日志的函数和行数
	char szFun[128] = { 0 };
	sprintf_s(szFun, "[%s:%d]", pFunName, lLine);
	fwrite(szFun, strlen(szFun), 1, fp);
	fwrite(" ", 1, 1, fp);
	fwrite(" ", 1, 1, fp);

	//日志内容
	fwrite(pLogText.c_str(), strlen(pLogText.c_str()), 1, fp);
	fwrite("\n", 1, 1, fp);
	fwrite("\n", 1, 1, fp);

	fclose(fp);
}

string CLog::GetTime()
{
	SYSTEMTIME st;
	::GetLocalTime(&st);
	char szTime[32] = { 0 };
	sprintf_s(szTime, "%04d-%02d-%02d %02d:%02d:%02d %d", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
	return szTime;
}

string CLog::GetFileName()
{
	SYSTEMTIME st;
	::GetLocalTime(&st);
	char szFile[32] = { 0 };
	sprintf_s(szFile, "%04d-%02d-%02d_%02d-%02d-%02d", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
	return szFile;
}

void CLog::ScanfWrite(const char* pSourcePath, const char* pFunName, const long lLine, const string pLogText, ...)
{
	va_list pArgs;
	va_start(pArgs, pLogText);
	char szBuffer[1024] = { 0 };
	_vsnprintf_s(szBuffer, 1024, pLogText.c_str(), pArgs);
	va_end(pArgs);
	WriteLog(pSourcePath, pFunName, lLine, szBuffer);
}
