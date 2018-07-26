#pragma once
#include "OriginPoint.h"

class MiningTool
{
public:
	MiningTool();
	~MiningTool();

	static double miningDistance(OriginPoint, OriginPoint);
	static double distanceBetweenPoints(double lastLongitude, double lastLatitude, double longitude, double latitude);
};

