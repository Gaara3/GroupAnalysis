#include "OriginPoint.h"
#include "Track.h"
#include "Log.h"
#include "SqlTool.h"
#include <deque>
#include "MiningTool.h"
#include <vector>

using std::deque;
int readTrackInfoFromDB(vector<Track> &,int &trackNum, int argc, char* argv[]);	//读取history里的所有点记录
void readTrackPointFromDB(vector<Track>,deque<OriginPoint>&);	//读取特定条件的点记录
//int** clusterGenerator(vector<TrackPoint>, int timeInterval);//对vector内的点迹以timeInterval为单位聚类

double totalThreshold = 1/60;

int main(int argc, char* argv[]) {

	int timeInterval = 3600*5;	//TODO read from config
	SqlTool::connectDB();

	vector<Track> tracks;
	deque<OriginPoint>Points;

	int trackNum = 0;
	readTrackInfoFromDB(tracks, trackNum,argc,argv);//从主表获取轨道信息
	SL_LOG("Read track info from DB completed");

	readTrackPointFromDB(tracks,Points);//从子表获取点迹信息
	SL_LOG("Read track points info from DB completed");

	int minPosixTime = atoi(SqlTool::getVariableFromDB("select min(POSIXTIME) from m_selectedtrack_sub"));
	int maxPosixTime = atoi(SqlTool::getVariableFromDB("select max(POSIXTIME) from m_selectedtrack_sub"));

	SL_LOG("Analyze by snapshot start");
	int groupCount =MiningTool::analyzeBySnapshot(Points, trackNum, minPosixTime, maxPosixTime, timeInterval, argc, argv);//对整体进行切片分析
	SL_LOG("Analyze by snapshot end");

	//system("pause");
	printf("{\"groupCount\":%d}", groupCount);
	return 0;
}


int readTrackInfoFromDB(vector<Track> &tracks,int &trackNum, int argc, char* argv[]) {
	char* getMain = new char[256];
	sprintf_s(getMain, 256, "select TRACKID,POINTAMOUNT,TARGETID,STARTTIME,ENDTIME,LENGTH,SOURCE,TASKINFO,CONFIDENCELEVEL,\
OPERATOR,RESERVE1,RESERVE2 from m_selectedtrack_main where operator = '%s';",argv[argc-1]);
	int res = 0;
	SqlTool::operationExcutor(getMain, SqlTool::res);
	//获取所有主轨迹信息
	while (SqlTool::column = mysql_fetch_row(SqlTool::res)) {
		tracks.push_back(Track(SqlTool::column[0], SqlTool::column[1], SqlTool::column[2],
			SqlTool::column[3], SqlTool::column[4], SqlTool::column[5], SqlTool::column[6],
			SqlTool::column[7], SqlTool::column[8], SqlTool::column[9], SqlTool::column[10], SqlTool::column[11]));

		res += atoi(SqlTool::column[1]);
		trackNum++;
	}
	SqlTool::freeRes();
	return res;
}

void readTrackPointFromDB(vector<Track> tracks,deque<OriginPoint>&res) {
	
	int trackNum = tracks.size();
	for (int counter = 0; counter < trackNum;counter++) {
		char* getSub = new char[256];
		int tmpTrackID = tracks[counter].getTrackID();
		string tmpTargetID = tracks[counter].getTargetID();
		sprintf_s(getSub, 256, "select posixtime,centerlongitude,centerlatitude,speed,angle from m_selectedtrack_sub where TRACKID = %d order by posixtime;", tracks[counter].getTrackID());
		SqlTool::operationExcutor(getSub, SqlTool::res);
		while (SqlTool::column = mysql_fetch_row(SqlTool::res)) {
			res.push_back(OriginPoint(tmpTrackID, tmpTargetID, SqlTool::column[0], SqlTool::column[1], SqlTool::column[2], SqlTool::column[3], SqlTool::column[4]));
		}
		SqlTool::freeRes();
	}
}

