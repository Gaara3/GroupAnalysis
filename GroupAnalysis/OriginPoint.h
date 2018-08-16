#pragma once
#include <string>

using std::string;
class OriginPoint
{
private:
	string targetID;
	int posixtime;
	double longitude;
	double latitude;
	double speed;
	double angle;

public:
	OriginPoint(int trackID, string targetID, char*posixtime, char* centerlongitude, char* cenlatitude, char* speed, char* angle);
	void setTargetID(string targetID);
	string getTargetID();
	int getPosixtime();
	double getLongitude();
	double getLatitude();
	double getSpeed();
	double getAngle();
	bool insideSnapshot(int, int);
	static bool sameTarget(OriginPoint a, OriginPoint b);
	OriginPoint();
	~OriginPoint();
};

