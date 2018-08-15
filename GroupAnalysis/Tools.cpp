#include "Tools.h"
#include <fstream>
#include <string>

using std::string;

Tools::Tools()
{
}


Tools::~Tools()
{
}

void Tools::writeArray2File(string filename, double** data, int m, int n) {
	std::ofstream outFile;
	outFile.open(filename, std::ios::out);
	for (int i = 0; i < m; ++i) {
		for (int j = 0; j < n; ++j)
			outFile << data[i][j] << ",";
		outFile << std::endl;
	}
	outFile.close();
}
void Tools::writeArray2File(string filename, bool** data, int m, int n) {
	std::ofstream outFile;
	outFile.open(filename, std::ios::out);
	for (int i = 0; i < m; ++i) {
		for (int j = 0; j < n; ++j)
			outFile << data[i][j] << ",";
		outFile << std::endl;
	}
	outFile.close();
}
