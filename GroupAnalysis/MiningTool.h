#pragma once
#include "OriginPoint.h"
#include "TrackPoint.h"
#include <vector>
#include <utility>
#include <map>
#include <set>
#include "Cluster.h"
#include "Chameleon.h"


using std::vector;
using std::pair;
using std::map;
using std::set;

class MiningTool
{
public:
	MiningTool();
	~MiningTool();

	static double miningDistance(OriginPoint, OriginPoint);
	static double distanceBetweenPoints(double lastLongitude, double lastLatitude, double longitude, double latitude);
	static int analyzeBySnapshot(deque<OriginPoint> &Points, int trackNum, int startTime, int endTime, int timeInterval,int argc,char* argv[]);
	static set<set<string> > snapshotAnalyze(deque<OriginPoint> Point,vector<int> candidateIdx);
	static void adjacentSnapshotMerge(map<set<string> , pair<int, int>> &,set<set<string>>&,int startTime,int endTime,int interVal,int argc,char*argv[],int &groupCounter,int &groupID);
	
};

