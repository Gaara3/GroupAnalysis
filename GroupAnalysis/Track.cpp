//#include "stdafx.h"
#include "Track.h"
#include <string>


using std::string;
using std::to_string;



const char* Track::getTargetsQuery = "SELECT distinct TARGETID FROM m_preprocessing;";

//extern vector<Track> HistoryTracks;

char* Track::getTargetRecords(char* targetID) {
	char* res = new char[800];
	sprintf_s(res, 800, "SELECT TARGETID,POSIXTIME,SOURCE,LONGITUDE,LATITUDE,ALTITUDE,OPERATOR,RESERVE1,RESERVE2,TASKINFO,SPEED,ANGLE FROM m_preprocessing WHERE TARGETID = '%s' ORDER BY POSIXTIME; ", targetID);
	return res;
	//return string("SELECT TARGETID,POSIXTIME,SOURCE,LONGITUDE,LATITUDE,ALTITUDE,OPERATOR,RESERVE1,RESERVE2,TASKINFO FROM preprocessing_copy WHERE TARGETID = '").append(targetID).append("'ORDER BY POSIXTIME; ");
}


Track::Track()
{
}

Track::Track(int trackID, char* TARGETID, char* SOURCE, char* TASKINFO, char* OPERATOR, int STARTTIME) {
	this->TRACKID = trackID;
	this->TARGETID = TARGETID;
	this->SOURCE = SOURCE;
	this->TASKINFO = TASKINFO;
	this->OPERATOR = OPERATOR;
	this->POINTAMOUNT = 1;
	this->STARTTIME = STARTTIME;
	this->CONFIDENCELEVEL = 1;
}

Track::Track(int trackID, char * TARGETID, char * SOURCE, char * TASKINFO, char * OPERATOR, char * STARTTIME)
{
	this->TRACKID = trackID;
	this->TARGETID = TARGETID;
	this->SOURCE = SOURCE;
	this->TASKINFO = TASKINFO;
	this->OPERATOR = OPERATOR;
	this->POINTAMOUNT = 1;
	this->STARTTIME = atoi(STARTTIME);
	this->CONFIDENCELEVEL = 1;
}

Track::Track(char * trackID, char * POINTAMOUNT, char * TARGETID, char * STARTTIME, char * ENDTIME, char * LENGTH, char * SOURCE, char * TASKINFO, char * CONFIDENCELEVEL, char * OPERATOR, char * RESERVE1, char * RESERVE2)
{
	this->TRACKID = atoi(trackID);
	this->POINTAMOUNT = atoi(POINTAMOUNT);
	this->TARGETID = TARGETID;
	this->STARTTIME = atoi(STARTTIME);
	this->ENDTIME = atoi(ENDTIME);
	this->length = atof(LENGTH);
	this->SOURCE = SOURCE;
	this->TASKINFO = TASKINFO;
	this->CONFIDENCELEVEL = atof(CONFIDENCELEVEL);
	this->RESERVE1 = RESERVE1;
	this->RESERVE2 = RESERVE2;
}

int Track::getTrackID() {
	return this->TRACKID;
}
void Track::setEndTime(int endTime) {
	this->ENDTIME = endTime;
}

int Track::getEndTime()
{
	return this->ENDTIME;
}

int Track::getStartTime()
{
	return this->STARTTIME;
}

int Track::getPointAmount()
{
	return this->POINTAMOUNT;
}

void Track::setPointAmount(int pointAmount) {
	this->POINTAMOUNT = pointAmount;
}

void Track::setStartTime(int startTime)
{
	this->STARTTIME = startTime;
}

void Track::setTrackID(int trackID) {
	this->TRACKID = trackID;
}

void Track::setTrackIDofPoint(int trackID) {
	for (vector<TrackPoint>::iterator i = this->historyPoint.begin(); i != historyPoint.end(); i++) {
		(*i).setTrackID(trackID);
	}
}
void Track::setLength(double length) {
	this->length = length;
}

void Track::setTargetID(char * id)
{
	this->TARGETID = id;
}

string Track::getTargetID()
{
	return this->TARGETID;
}

string Track::insertHisSQL() {

	string insertSql = "insert into m_historytrack_main (GUID,TRACKID,POINTAMOUNT,TARGETID,STARTTIME,ENDTIME,SOURCE,TASKINFO,CONFIDENCELEVEL,OPERATOR,LENGTH) values(";
	insertSql.append("UUID(),").append(to_string(this->TRACKID)).append(",").append(to_string(POINTAMOUNT)).append(",'").append(this->TARGETID).append("','")
		.append(SqlTool::datetimeConvertor(this->STARTTIME)).append("','").append(SqlTool::datetimeConvertor(this->ENDTIME)).append("','").append(this->SOURCE).append("','").append(this->TASKINFO)
		.append("',").append(to_string(this->CONFIDENCELEVEL)).append(",'").append(this->OPERATOR).append("',").append(to_string(this->length)).append(");");

	return insertSql;
}

char * Track::insertFreqSQL()
{
	char res[1000];
	sprintf_s(res, 1000, "insert into m_frequentlytrack_main (GUID,TRACKID,POINTAMOUNT,TARGETID,STARTTIME,ENDTIME,LENGTH,CONFIDENCELEVEL,OPERATOR) VALUES\
(UUID(),%d,%d,%s,'%s','%s',%lf,1,'%s')", TRACKID, POINTAMOUNT, TARGETID.data(), SqlTool::datetimeConvertor(this->STARTTIME), SqlTool::datetimeConvertor(this->ENDTIME), length, OPERATOR);
	return res;
}


void Track::trackEndProcession(int endTime, int pointAmount, vector<TrackPoint>details, double totallength) {
	this->setEndTime(endTime);
	this->setPointAmount(pointAmount);
	this->setLength(totallength);
	this->historyPoint.assign(details.begin(), details.end());
}

Track::~Track()
{
}

