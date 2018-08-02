#pragma once
#include <string>
#include <time.h>
#include <vector>

using std::string;
using std::vector;

class TrackPoint
{
private:
	char* TARGETID;
	int TRACKID;
	int ORDERNUMBER;
	int ORIGINORDERNUMBER;
	int TIME;
	char* SOURCE;
	double UPPERLEFTLONGITUDE;
	double UPPERLEFTLATITUDE;
	double UPPERLEFTALTITUDE;
	double UPPERRIGHTLONGITUDE;
	double UPPERRIGHTLATITUDE;
	double UPPERRIGHTALTITUDE;
	double LOWERLEFTLONGITUDE;
	double LOWERLEFTLATITUDE;
	double LOWERLEFTALTITUDE;
	double LOWERRIGHTLONGITUDE;
	double LOWERRIGHTLATITUDE;
	double LOWERRIGHTALTITUDE;
	double speed;
	double angle;
	double CONFIDENCELEVEL;
	char* OPERATOR;
	char* RESERVE1;
	char* RESERVE2;


public:
	double CENTERLONGITUDE;//考虑到计算效率，经纬海拔放到public部分以便直接访问
	double CENTERLATITUDE;
	double CENTERALTITUDE;
	char* insertHisSQL();
	char* insertFreqSQL();
	char* sampledSQL(bool first, int sampleLevel);
	TrackPoint();
	~TrackPoint();
	TrackPoint(int orderNumber, double Longitude, double Latitude);
	TrackPoint(/*char* TARGET,*/char* POSIXTIME, char* SOURCE, double LONGITUDE, double LATITUDE, double ALTITUDE, char* OPERATOR, char* RESERVE1, char* RESERVE2, double speed, double angle);
	TrackPoint(int trackID, char* targetID,char* orderNumber, char* posixTime, char* source, char* centerLONGITUDE, char* centerLatitude, char* centerAltitude, char* speed, char* angle, char* confidenceLevel, char* reserve1, char* reserve2);

	char* getTargetID();
	void setTargetID(char* targetID);
	void setOrderNumber(int orderNumber);
	int getOrderNumber();
	void setTrackID(int trackID);
	int getTrackID();
	int getTime();
	char* getSource();
	char* getOperator();
	double getSpeed();
	double getAngle();
	void setAngle(double);
	void setSpeed(double);
};

