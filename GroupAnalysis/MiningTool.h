#pragma once
#include "OriginPoint.h"
#include "TrackPoint.h"

class MiningTool
{
public:
	MiningTool();
	~MiningTool();

	static double miningDistance(OriginPoint, OriginPoint);
	static double distanceBetweenPoints(double lastLongitude, double lastLatitude, double longitude, double latitude);
	static void analyzeBySnapshot(vector<OriginPoint> &Points, int trackNum, int startTime, int endTime, int timeInterval);
	static void snapshotAnalyze(vector<OriginPoint> Point,vector<int> candidateIdx);
};

