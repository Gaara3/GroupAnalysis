#ifndef __READ_CONFIG_H__
#define __READ_CONFIG_H__

#include <string>
#include <fstream>

using namespace std;

class CReadCfg
{
public:
	CReadCfg();
	~CReadCfg();

	bool ReadConfig(const string& key, string& value);
	bool ReadConfig(const string& key, int& value);
	bool AnalyseLine(const string& line, string& key, string& value);

private:
	ifstream m_infile;
};

#endif //__READ_CONFIG_H__