#include "MiningTool.h"
#include <math.h>
#include <algorithm> 

MiningTool::MiningTool()
{
}


MiningTool::~MiningTool()
{
}

double MiningTool::miningDistance(OriginPoint a, OriginPoint b) {
	//同一目标不同点迹，不计算距离，聚类时直接过滤
	/*if (a.getTargetID() == b.getTargetID())
		return -1;*/		//先一起聚类，在簇内再去重

	int tDiff = abs(a.getPosixtime() - b.getPosixtime());
	double distance = distanceBetweenPoints(a.getLongitude(), a.getLatitude(), b.getLongitude(), b.getLatitude());
	double angleDiff = fabs(a.getAngle() - b.getAngle());
	double speedDiff = fabs(a.getSpeed() - b.getSpeed());

	return 0;
}


double MiningTool::distanceBetweenPoints(double lastLongitude, double lastLatitude, double longitude, double latitude) {
	double res = 0;
	if (lastLatitude <= 90 && lastLongitude <= 180) {	//当一段轨迹完结，设置异常值以便于新一段计算
		double p = 0.017453292519943295;    // Math.PI / 180
		double a = 0.5 - cos((latitude - lastLatitude) * p) / 2 + cos(latitude * p) * cos(lastLatitude * p) *(0.5 - cos((longitude - lastLongitude) * p) / 2);

		res = 12742 * asin(sqrt(a)); // 2 * R; R = 6371 km
	}
	return res;
}

bool posixSort(OriginPoint a, OriginPoint b) {
	return a.getPosixtime() < b.getPosixtime();
}

bool samePoint(OriginPoint a, OriginPoint b) {
	if (strcmp(a.getTargetID(), b.getTargetID()) != 0)
		return false;
	return a.getLongitude() == b.getLongitude() && a.getPosixtime() == b.getPosixtime() && a.getLatitude() == b.getLatitude();
}

void MiningTool::analyzeBySnapshot(vector<OriginPoint> &Points, int trackNum, int startTime, int endTime, int timeInterval) {
	
	std::sort(Points.begin(), Points.end(), posixSort);//按时间排序

	Points.erase(std::unique(Points.begin(), Points.end(),samePoint), Points.end());//去重

	int tmpStartTime = startTime, tmpEndTime = startTime + timeInterval;
	int pCounter = 0, pNum = Points.size();
	while (tmpStartTime <= endTime) {		//每个时间切片轮询
		vector<int> candidateIdx;
		bool multiTarget = false;
		do{
			if (Points[pCounter].insideSnapshot(tmpStartTime, tmpEndTime)) {	//该点迹落于时间片内:1.判断是否为新目标 2.push入候选集
				if (!multiTarget && !candidateIdx.empty() && !OriginPoint::sameTarget(Points[pCounter], Points[candidateIdx.back()]))//先与candidate最新元素比对
					multiTarget = true;//将multiTarget加入条件，减少后续判断

				candidateIdx.push_back(pCounter);
			}else	//不在时间片内，则跳出，进入下一时间片
				break;			
		} while (++pCounter < pNum);
		
		if(multiTarget)//多目标情况下再进行聚类
			snapshotAnalyze(Points,candidateIdx);

		tmpStartTime = tmpEndTime;//更新时间切片
		tmpEndTime += timeInterval;
	}
}

void MiningTool::snapshotAnalyze(vector<OriginPoint> Point,vector<int>candidateIdx)
{
		return;
}
