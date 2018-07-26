#include "ConfigReader.h"
#include <stdlib.h>
#include <string.h>
#include <Windows.h>

ConfigReader::ConfigReader()
{
	wchar_t exeFullPath[MAX_PATH] = { 0 };
	GetModuleFileName(NULL, exeFullPath, MAX_PATH);

	char CharStr[MAX_PATH] = { 0 };
	size_t convertedChars = 0;
	wcstombs_s(&convertedChars, CharStr, MAX_PATH, exeFullPath, _TRUNCATE);

	string path = (string)CharStr;
	size_t pos = path.find_last_of('\\', path.length());
	path = path.substr(0, pos);
	path += "\\Config\\Config.ini";

	//调用构造函数时，打开文件
	m_infile.open(path.c_str());
}


ConfigReader::~ConfigReader()
{
	if (m_infile)
	{
		m_infile.close();
	}
}

bool ConfigReader::ReadConfig(const string & key, string & value)
{
	if (!m_infile)
		return false;

	string line, tempKey, tempValue;
	tempKey = key;

	while (getline(m_infile, line))
	{
		if (AnalyseLine(line, tempKey, tempValue))
		{
			value = tempValue;

			return true;
		}
	}

	return false;
}

bool ConfigReader::ReadConfig(const string & key, int & value)
{
	if (!m_infile)
		return false;

	string line, tempKey, tempValue;
	tempKey = key;

	while (getline(m_infile, line))
	{
		if (AnalyseLine(line, tempKey, tempValue))
		{
			value = atoi(tempValue.data());

			return true;
		}
	}

	return false;
}

bool ConfigReader::AnalyseLine(const string & line, string & key, string & value)
{
	if (line.empty())
		return false;

	//行的第一个字符就是注释字符
	if (0 == line.find('#'))
		return false;

	if (string::npos == line.find('='))
		return false;

	if (strstr(line.data(), key.data()))
	{
		const char* p = strstr(line.data(), "=");
		if (p)
		{
			++p;
			value = p;
		}
	}

	return true;
}
