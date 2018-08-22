#include "MiningTool.h"
#include "Tools.h"
#include "SqlTool.h"
#include <math.h>
#include <algorithm> 


MiningTool::MiningTool()
{
}


MiningTool::~MiningTool()
{
}
/*int main() {
	double testL0 = MiningTool::distanceBetweenPoints(124.07833, 21.4033,124.105, 21.4);
	double testL20 = MiningTool::distanceBetweenPoints(0, 20, 0.03, 20);
	double testL40 = MiningTool::distanceBetweenPoints(0, 40, 0.03, 40);
	double testL60 = MiningTool::distanceBetweenPoints(0, 60, 0.03, 60);
	printf("test0:%10lg", testL0);
	printf("test20:%10lg", testL20);
	printf("test40:%10lg", testL40);
	printf("test60:%10lg", testL60);

	return 0;
}*/
double MiningTool::miningDistance(OriginPoint a, OriginPoint b) {

	double tDiff = fabs(a.getPosixtime() - b.getPosixtime())/60;	//以分钟为单位，暂定理想编群tDiff应在20以内
	if (tDiff == 0)
		tDiff = 1;
	double distance = distanceBetweenPoints(a.getLongitude(), a.getLatitude(), b.getLongitude(), b.getLatitude());//km级别
	if (distance == 0)
		distance = 2;
	//double angleDiff = fabs(a.getAngle() - b.getAngle());
	//double speedDiff = fabs(a.getSpeed() - b.getSpeed());
	//double res = (0.6*distance + 0.2*(angleDiff + speedDiff)) * tDiff;

	return tDiff*distance;
}


double MiningTool::distanceBetweenPoints(double lastLongitude, double lastLatitude, double longitude, double latitude) {
	double res = 0;
	if (lastLatitude <= 90 && lastLongitude <= 180) {	
		double p = 0.017453292519943295;    // Math.PI / 180
		double a = 0.5 - cos((latitude - lastLatitude) * p) / 2 + cos(latitude * p) * cos(lastLatitude * p) *(0.5 - cos((longitude - lastLongitude) * p) / 2);

		res = 6371.393 * asin(sqrt(a)); // 2 * R; R = 6371 km
	}
	return res;
}   

bool posixSort(OriginPoint a, OriginPoint b) {
	return a.getPosixtime() < b.getPosixtime();
}

bool samePoint(OriginPoint a, OriginPoint b) {
	if (a.getTargetID()!=b.getTargetID())
		return false;
	return a.getLongitude() == b.getLongitude() && a.getPosixtime() == b.getPosixtime() && a.getLatitude() == b.getLatitude();
}

int MiningTool::analyzeBySnapshot(deque<OriginPoint> &Points, int trackNum, int startTime, int endTime, int timeInterval,int argc,char *argv[]) {
	
	std::sort(Points.begin(), Points.end(), posixSort);//按时间排序

	Points.erase(std::unique(Points.begin(), Points.end(),samePoint), Points.end());//去重

	int tmpStartTime = startTime, tmpEndTime = startTime + timeInterval;
	int pCounter = 0,groupCounter =0, pNum = Points.size();
	int groupID = atoi(SqlTool::getVariableFromDB("SELECT MAX(GROUPID) FROM M_SELECTED_GROUP_MAIN"));
	map<set<string>, pair<int, int>> tmpGroups;
	while (tmpStartTime <= endTime&& pCounter < pNum ) {		//每个时间切片轮询
		vector<int> candidateIdx;
		set<string>targets;

		do{	
			if (Points[pCounter].insideSnapshot(tmpStartTime, tmpEndTime)) {	//该点迹落于时间片内
				targets.insert(Points[pCounter].getTargetID());
				candidateIdx.push_back(pCounter);
				++pCounter;
			}
			else 	//不在时间片内，则跳出，进入下一时间片
				break;
		} while (pCounter<pNum);
		//printf("target Size:%5d\n", targets.size());
		if (targets.size() > 3) {//多目标情况下再进行聚类
			set<set<string> > snapshotGroups = snapshotAnalyze(Points, candidateIdx);	//时间片内的多个潜在编群
			
			adjacentSnapshotMerge(tmpGroups, snapshotGroups, tmpStartTime, tmpEndTime, timeInterval,argc,argv,groupCounter,groupID);
		}			
		tmpStartTime = tmpEndTime;//更新时间切片
		tmpEndTime += timeInterval;
	}
	return groupCounter;
}

set<set<string> > MiningTool::snapshotAnalyze(deque<OriginPoint> Point,vector<int>candidateIdx)
{
	set<set<string> > groups;
	//TODO  优化，算出不同目标之间最近点距离进行判断是否要聚类
	int PointNum = (int)candidateIdx.size();

	Chameleon chameleon(Point, candidateIdx,4);
	//knn连接图生成
	
	//生成簇进行目标分析，单目标簇删去，多目标簇进行簇内去重
	for (Cluster c : chameleon.chameleonCluster()) {
		set<string> tmp= Chameleon::clusterAnalyse(c);	//簇内去重
		if (tmp.size() > 1)		//单目标簇剔除
			groups.insert(tmp);
	}	

	return groups;
}

void MiningTool::adjacentSnapshotMerge(map<set<string>, pair<int, int>>& tmpGroup,set<set<string>> &snapshotGroup,int startTime,int endTime,int interval,int argc,char*argv[],int &groupCounter,int& groupID)
{
	//第一步：对map进行轮询，更新持续的编群、或剔除不再持续的编群
	for (map<set<string>, pair<int, int>>::iterator i = tmpGroup.begin(); i != tmpGroup.end();     /*TODO*/) {

		if (snapshotGroup.count((*i).first)) {	//相邻时间片内编群仍存在：更新时间，删除set记录
			(*i).second.second = endTime;
			snapshotGroup.erase((*i).first);
			++i;
		}
		else {	//新时间片的编群不含该记录，则:①编群维持时间段，直接删除  ②编群持续时间长，记录后再删除
			if (startTime - (*i).second.first > interval) { 	//该编群到此截止,持续时间超过interval：记录该编群信息
				++groupCounter;
				++groupID;
				Tools::recordGroupInfo((*i).second.first, (*i).second.second, (*i).first,argc,argv,groupID);
				
			}
			i = tmpGroup.erase(i);
		}
	}
	//对于新出现的编群，则放入tmpGroup，设置起止信息
	for (set<string> ss:snapshotGroup) {
		tmpGroup[ss].first = startTime;
		tmpGroup[ss].second = endTime;
	}
}
