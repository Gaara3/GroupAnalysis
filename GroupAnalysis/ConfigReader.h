#pragma once
#ifndef __CONFIGREADER_H__
#define __CONFIGREADER_H__

#include <string>
#include <fstream>

using std::string;
using std::ifstream;

class ConfigReader
{
public:
	ConfigReader();
	~ConfigReader();

	bool ReadConfig(const string& key, string& value);
	bool ReadConfig(const string& key, int& value);
	bool AnalyseLine(const string& line, string& key, string& value);

private:
	ifstream m_infile;
};
#endif // !__CONFIGREADER_H__



