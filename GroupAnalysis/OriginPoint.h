#pragma once
class OriginPoint
{
private:
	int targetID;
	int posixtime;
	char* source;
	double longitude;
	double latitude;
	double altitude;
	double speed;
	double angle;

public:
	int getTargetID();
	int getPosixtime();
	double getLongitude();
	double getLatitude();
	double getSpeed();
	double getAngle();
	OriginPoint();
	~OriginPoint();
};

