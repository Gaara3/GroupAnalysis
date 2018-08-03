//#include "stdafx.h"
#include "TrackPoint.h"
#include <string>
#include "SqlTool.h"

using std::to_string;


TrackPoint::TrackPoint()
{
}

TrackPoint::TrackPoint(/*char* TARGETID,*/char* POSIXTIME, char* SOURCE, double LONGITUDE, double LATITUDE, double ALTITUDE, char* OPERATOR, char* RESERVE1, char* RESERVE2, double speed, double angle) {
	this->TIME = atoi(POSIXTIME);
	this->SOURCE = SOURCE;
	this->ORDERNUMBER = 1;
	this->CONFIDENCELEVEL = 1;

	this->CENTERLONGITUDE = LONGITUDE;
	this->UPPERLEFTLONGITUDE = LONGITUDE;
	this->UPPERRIGHTLONGITUDE = LONGITUDE;
	this->LOWERLEFTLONGITUDE = LONGITUDE;
	this->LOWERRIGHTLONGITUDE = LONGITUDE;

	this->CENTERALTITUDE = ALTITUDE;
	this->UPPERLEFTALTITUDE = ALTITUDE;
	this->UPPERRIGHTALTITUDE = ALTITUDE;
	this->LOWERLEFTALTITUDE = ALTITUDE;
	this->LOWERRIGHTALTITUDE = ALTITUDE;

	this->CENTERLATITUDE = LATITUDE;
	this->UPPERLEFTLATITUDE = LATITUDE;
	this->UPPERRIGHTLATITUDE = LATITUDE;
	this->LOWERLEFTLATITUDE = LATITUDE;
	this->LOWERRIGHTLATITUDE = LATITUDE;

	this->OPERATOR = OPERATOR;
	this->RESERVE1 = RESERVE1;
	this->RESERVE2 = RESERVE2;
	this->speed = speed;
	this->angle = angle;
}


TrackPoint::~TrackPoint()
{
}

TrackPoint::TrackPoint(int orderNumber, double Longitude, double Latitude) :ORDERNUMBER(orderNumber)
{
	this->CENTERLONGITUDE = Longitude;
	this->UPPERLEFTLONGITUDE = Longitude;
	this->UPPERRIGHTLONGITUDE = Longitude;
	this->LOWERLEFTLONGITUDE = Longitude;
	this->LOWERRIGHTLONGITUDE = Longitude;


	this->CENTERLATITUDE = Latitude;
	this->UPPERLEFTLATITUDE = Latitude;
	this->UPPERRIGHTLATITUDE = Latitude;
	this->LOWERLEFTLATITUDE = Latitude;
	this->LOWERRIGHTLATITUDE = Latitude;
}

TrackPoint::TrackPoint(int trackID, char* targetID,char* orderNumber, char* posixTime, char* source, char* centerLONGITUDE, char* centerLatitude, char* centerAltitude, char* speed, char* angle, char* confidenceLevel, char* reserve1, char* reserve2) {
	setTargetID(targetID);
	this->TRACKID = trackID;
	this->ORDERNUMBER = atoi(orderNumber);
	this->TIME = atoi(posixTime);
	this->SOURCE = source;
	this->CENTERLONGITUDE = atof(centerLONGITUDE);
	this->CENTERLATITUDE = atof(centerLatitude);
	this->CENTERALTITUDE = atof(centerAltitude);
	this->speed = atof(speed);
	this->angle = atof(angle);
	this->CONFIDENCELEVEL = atof(confidenceLevel);
	this->RESERVE1 = reserve1;
	this->RESERVE2 = reserve2;
}

TrackPoint::TrackPoint(int trackID, char * targetID, char * posixtime, char * centerlongitude, char * cenlatitude, char * speed, char * angle)
{
	setTargetID(targetID);
	this->TRACKID = trackID;
	this->TIME = atoi(posixtime);
	this->CENTERLONGITUDE = atof(centerlongitude);
	this->CENTERLATITUDE = atof(cenlatitude);
	this->speed = atof(speed);
	this->angle = atof(angle);
}

char * TrackPoint::getTargetID()
{
	return this->TARGETID;
}

void TrackPoint::setTargetID(char * targetID)
{
	this->TARGETID = targetID;
}

char* TrackPoint::insertHisSQL() {
	char* res = new char[1500];
	sprintf_s(res, 1500, "insert into m_historytrack_sub(GUID,TRACKID,ORDERNUMBER,POSIXTIME,TIME,SOURCE,UPPERLEFTLONGITUDE,\
	UPPERLEFTLATITUDE,UPPERLEFTALTITUDE, UPPERRIGHTLONGITUDE,UPPERRIGHTLATITUDE,UPPERRIGHTALTITUDE,LOWERRIGHTLONGITUDE,LOWERRIGHTLATITUDE,LOWERRIGHTALTITUDE,\
	LOWERLEFTLONGITUDE,LOWERLEFTLATITUDE,LOWERLEFTALTITUDE,CENTERLONGITUDE,CENTERLATITUDE,\
	CENTERALTITUDE,SPEED,ANGLE,CONFIDENCELEVEL,RESERVE1,RESERVE2) \
	values(UUID(),%d,%d,%d,'%s','%s',%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,1,'%s','%s')",
		TRACKID, ORDERNUMBER, TIME, SqlTool::datetimeConvertor(this->TIME), SOURCE, UPPERLEFTLONGITUDE, UPPERLEFTLATITUDE, UPPERLEFTALTITUDE,
		UPPERRIGHTLONGITUDE, UPPERRIGHTLATITUDE, UPPERRIGHTALTITUDE, LOWERRIGHTLONGITUDE, LOWERRIGHTLATITUDE, LOWERRIGHTALTITUDE,
		LOWERLEFTLONGITUDE, LOWERLEFTLATITUDE, LOWERLEFTALTITUDE, CENTERLONGITUDE, CENTERLATITUDE, CENTERALTITUDE, speed, angle, RESERVE1, RESERVE2);

	return res;
}

char * TrackPoint::insertFreqSQL()
{
	char res[1000];
	sprintf_s(res, 1000, "insert into m_frequentlytrack_sub(GUID,TRACKID,ORDERNUMBER,CENTERLONGITUDE,CENTERLATITUDE,CENTERALTITUDE,UPPERLEFTLONGITUDE,UPPERLEFTLATITUDE,UPPERLEFTALTITUDE,\
	UPPERRIGHTLONGITUDE,UPPERRIGHTLATITUDE,UPPERRIGHTALTITUDE,LOWERRIGHTLONGITUDE,LOWERRIGHTLATITUDE,LOWERRIGHTALTITUDE,LOWERLEFTLONGITUDE,LOWERLEFTLATITUDE,LOWERLEFTALTITUDE,\
CONFIDENCELEVEL,AVGSPEED) VALUES(UUID(),%d,%d,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,1,%lf);", TRACKID, ORDERNUMBER, CENTERLONGITUDE, CENTERLATITUDE, CENTERALTITUDE, UPPERLEFTLONGITUDE, UPPERLEFTLATITUDE, UPPERLEFTALTITUDE,
UPPERRIGHTLONGITUDE, UPPERRIGHTLATITUDE, UPPERRIGHTALTITUDE, LOWERRIGHTLONGITUDE, LOWERRIGHTLATITUDE, LOWERRIGHTALTITUDE, LOWERLEFTLONGITUDE, LOWERLEFTLATITUDE, LOWERLEFTALTITUDE, speed);
	return res;
}

char * TrackPoint::sampledSQL(bool first, int sampleLevel)
{
	char* res = new char[256];
	const char* sql = first ? "(UUID(), %d, %d, %d, '%s', %d, %d, '%s', %lf, %lf, %lf, %lf, %lf, 1)" : ",(UUID(), %d, %d, %d, '%s', %d, %d, '%s', %lf, %lf, %lf, %lf, %lf, 1)";
	sprintf_s(res, 256, sql,
		TRACKID, ORDERNUMBER, ORIGINORDERNUMBER, SqlTool::datetimeConvertor(TIME), TIME, sampleLevel, SOURCE, CENTERLONGITUDE,
		CENTERLATITUDE, CENTERALTITUDE, speed, angle, RESERVE1, RESERVE2);
	return res;
}

void  TrackPoint::setOrderNumber(int orderNumber) {
	this->ORDERNUMBER = orderNumber;
}

int TrackPoint::getOrderNumber()
{
	return this->ORDERNUMBER;
}

void TrackPoint::setTrackID(int trackID) {
	this->TRACKID = trackID;
}

int TrackPoint::getTrackID()
{
	return this->TRACKID;
}

int TrackPoint::getTime() {
	return this->TIME;
}

char* TrackPoint::getSource() {
	return this->SOURCE;
}

char* TrackPoint::getOperator() {
	return this->OPERATOR;
}

double TrackPoint::getSpeed()
{
	return this->speed;
}

void TrackPoint::setAngle(double a)
{
	this->angle = a;
}

double TrackPoint::getAngle() {
	return this->angle;
}

void TrackPoint::setSpeed(double speed)
{
	this->speed = speed;
}
