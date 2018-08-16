#include "OriginPoint.h"
#include <string>


OriginPoint::OriginPoint(int trackID, string targetID, char * posixtime, char * centerlongitude, char * cenlatitude, char * speed, char * angle)
{
	setTargetID(targetID);
	this->posixtime = atoi(posixtime);
	this->longitude = atof(centerlongitude);
	this->latitude = atof(cenlatitude);
	this->speed = atof(speed);
	this->angle = atof(angle);
}

void OriginPoint::setTargetID(string targetID)
{
	this->targetID = targetID;
}

string OriginPoint::getTargetID()
{
	return targetID;
}

int OriginPoint::getPosixtime()
{
	return posixtime;
}

double OriginPoint::getSpeed()
{
	return speed;
}

double OriginPoint::getAngle()
{
	return angle;
}

bool OriginPoint::insideSnapshot(int s, int e)
{
	return posixtime>=s && posixtime<e;
}

bool OriginPoint::sameTarget(OriginPoint a, OriginPoint b)
{
	return a.getTargetID() == b.getTargetID() ;
}

double OriginPoint::getLongitude() {
	return longitude;
}

double OriginPoint::getLatitude() {
	return latitude;
}

OriginPoint::OriginPoint()
{
}


OriginPoint::~OriginPoint()
{
}
