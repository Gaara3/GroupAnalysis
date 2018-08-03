#include "OriginPoint.h"
#include "Track.h"
#include "SqlTool.h"
#include "MiningTool.h"
#include <vector>

int readTrackInfoFromDB(vector<Track> &);	//��ȡhistory������е��¼
vector<OriginPoint> readTrackPointFromDB(vector<Track>);	//��ȡ�ض������ĵ��¼
//int** clusterGenerator(vector<TrackPoint>, int timeInterval);//��vector�ڵĵ㼣��timeIntervalΪ��λ����

int main() {

	int timeInterval = 3600*5;	//TODO read from config
	SqlTool::connectDB();

	vector<Track> tracks;

	int trackNum = readTrackInfoFromDB(tracks);//�������ȡ�����Ϣ

	vector<OriginPoint> Points =readTrackPointFromDB(tracks);//���ӱ��ȡ�㼣��Ϣ

	int minPosixTime = atoi(SqlTool::getVariableFromDB("select min(POSIXTIME) from m_selectedtrack_sub"));
	int maxPosixTime = atoi(SqlTool::getVariableFromDB("select max(POSIXTIME) from m_selectedtrack_sub"));

	MiningTool::analyzeBySnapshot(Points, trackNum, minPosixTime, maxPosixTime, timeInterval);//�����������Ƭ����

	system("pause");
}


int readTrackInfoFromDB(vector<Track> &tracks) {
	char* getMain = new char[256];
	sprintf_s(getMain, 256, "select TRACKID,POINTAMOUNT,TARGETID,STARTTIME,ENDTIME,LENGTH,SOURCE,TASKINFO,CONFIDENCELEVEL,\
OPERATOR,RESERVE1,RESERVE2 from m_selectedtrack_main;");
	SqlTool::operationExcutor(getMain, SqlTool::res);
	//��ȡ�������켣��Ϣ
	while (SqlTool::column = mysql_fetch_row(SqlTool::res)) {
		tracks.push_back(Track(SqlTool::column[0], SqlTool::column[1], SqlTool::column[2],
			SqlTool::column[3], SqlTool::column[4], SqlTool::column[5], SqlTool::column[6],
			SqlTool::column[7], SqlTool::column[8], SqlTool::column[9], SqlTool::column[10], SqlTool::column[11]));
	}
	return (int)tracks.size();
}

vector<OriginPoint> readTrackPointFromDB(vector<Track> tracks) {
	
	int trackNum = tracks.size();
	vector<OriginPoint> res;
	for (int counter = 0; counter < trackNum;counter++) {
		char* getSub = new char[256];
		int tmpTrackID = tracks[counter].getTrackID();
		char* tmpTargetID = tracks[counter].getTargetID();
		sprintf_s(getSub, 256, "select posixtime,centerlongitude,centerlatitude,speed,angle from m_selectedtrack_sub where TRACKID = %d order by posixtime;", tracks[counter].getTrackID());
		SqlTool::operationExcutor(getSub, SqlTool::res);
		while (SqlTool::column = mysql_fetch_row(SqlTool::res)) {
			res.push_back(OriginPoint(tmpTrackID, tmpTargetID, SqlTool::column[0], SqlTool::column[1], SqlTool::column[2], SqlTool::column[3], SqlTool::column[4]));
		}
	}
	return res;
}

