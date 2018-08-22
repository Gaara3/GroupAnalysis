#pragma once
#include<string>
#include<set>

using std::set;
using std::string;

class Tools
{
public:
	Tools();
	~Tools();
	static void writeArray2File(string filename, double** data, int m, int n);
	static void writeArray2File(string filename, bool** data, int m, int n);
	//static void recordSnapshotInfo(int startTime, int endTime, set<set<string>> snapShotGroups);
	static void recordGroupInfo(int startTime, int endTime, set<string>,int ,char*argv[],int &groupID);
};

