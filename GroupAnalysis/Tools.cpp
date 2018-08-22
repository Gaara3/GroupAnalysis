#include "Tools.h"
#include "SqlTool.h"
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
	outFile.open(filename, std::ios::out+std::ios::ate);
	for (int i = 0; i < m; ++i) {
		for (int j = 0; j < n; ++j)
			outFile << data[i][j] << ",";
		outFile << std::endl;
	}
	outFile << "-----------------------------------"<<std::endl;
	outFile.close();
}

/*void Tools::recordSnapshotInfo(int startTime, int endTime, set<set<string>> snapShotGroups)
{
	for (set<set<string>>::iterator i = snapShotGroups.begin(); i != snapShotGroups.end(); ++i) {
		recordGroupInfo(startTime, endTime, *i,argc,argv);
	}
}*/

void Tools::recordGroupInfo(int startTime, int endTime, set<string> groupInfo, int argc, char*argv[], int & groupID)
{
	char* mainSql = new char[256];
	sprintf_s(mainSql, 256, "insert into m_selected_group_main (GUID,GROUPID,RECTIME,TARGETAMOUNT,STARTTIME,ENDTIME,OPERATOR) VALUES (uuid(), %d,now(), %d, '%s', '%s','%s');", groupID,groupInfo.size(), SqlTool::datetimeConvertor(startTime), SqlTool::datetimeConvertor(endTime),argv[argc-1]);
	SqlTool::insertExcutor(mainSql);

	//int ID = atoi(SqlTool::getVariableFromDB("select LAST_INSERT_ID();"));		//注意，此处有单线程限制

	string subSql("insert into m_selected_group_sub (GUID,GROUPID,TARGETID,OPERATOR) VALUES");

	int groupSize = groupInfo.size();
	for (set<string>::iterator i = groupInfo.begin(); i != groupInfo.end();) {
		char* tmp = new char[128];
		sprintf_s(tmp, 128, "(UUID(),%d,'%s','%s')", groupID, (*i).data(),argv[argc-1]);
		subSql.append(string(tmp));
		++i;
		if (i != groupInfo.end())
			subSql.append(",");
	}
	SqlTool::insertExcutor(subSql.data());
}
