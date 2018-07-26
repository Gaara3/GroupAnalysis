#include "OriginPoint.h"



int OriginPoint::getTargetID()
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
