#pragma once
#include<string>

using std::string;

class Tools
{
public:
	Tools();
	~Tools();
	static void writeArray2File(string filename, double** data, int m, int n);
	static void writeArray2File(string filename, bool** data, int m, int n);
};

