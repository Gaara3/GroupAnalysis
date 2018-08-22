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

	double tDiff = fabs(a.getPosixtime() - b.getPosixtime())/60;	//�Է���Ϊ��λ���ݶ������ȺtDiffӦ��20����
	if (tDiff == 0)
		tDiff = 1;
	double distance = distanceBetweenPoints(a.getLongitude(), a.getLatitude(), b.getLongitude(), b.getLatitude());//km����
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
	
	std::sort(Points.begin(), Points.end(), posixSort);//��ʱ������

	Points.erase(std::unique(Points.begin(), Points.end(),samePoint), Points.end());//ȥ��

	int tmpStartTime = startTime, tmpEndTime = startTime + timeInterval;
	int pCounter = 0,groupCounter =0, pNum = Points.size();
	int groupID = atoi(SqlTool::getVariableFromDB("SELECT MAX(GROUPID) FROM M_SELECTED_GROUP_MAIN"));
	map<set<string>, pair<int, int>> tmpGroups;
	while (tmpStartTime <= endTime&& pCounter < pNum ) {		//ÿ��ʱ����Ƭ��ѯ
		vector<int> candidateIdx;
		set<string>targets;

		do{	
			if (Points[pCounter].insideSnapshot(tmpStartTime, tmpEndTime)) {	//�õ㼣����ʱ��Ƭ��
				targets.insert(Points[pCounter].getTargetID());
				candidateIdx.push_back(pCounter);
				++pCounter;
			}
			else 	//����ʱ��Ƭ�ڣ���������������һʱ��Ƭ
				break;
		} while (pCounter<pNum);
		//printf("target Size:%5d\n", targets.size());
		if (targets.size() > 3) {//��Ŀ��������ٽ��о���
			set<set<string> > snapshotGroups = snapshotAnalyze(Points, candidateIdx);	//ʱ��Ƭ�ڵĶ��Ǳ�ڱ�Ⱥ
			
			adjacentSnapshotMerge(tmpGroups, snapshotGroups, tmpStartTime, tmpEndTime, timeInterval,argc,argv,groupCounter,groupID);
		}			
		tmpStartTime = tmpEndTime;//����ʱ����Ƭ
		tmpEndTime += timeInterval;
	}
	return groupCounter;
}

set<set<string> > MiningTool::snapshotAnalyze(deque<OriginPoint> Point,vector<int>candidateIdx)
{
	set<set<string> > groups;
	//TODO  �Ż��������ͬĿ��֮��������������ж��Ƿ�Ҫ����
	int PointNum = (int)candidateIdx.size();

	Chameleon chameleon(Point, candidateIdx,4);
	//knn����ͼ����
	
	//���ɴؽ���Ŀ���������Ŀ���ɾȥ����Ŀ��ؽ��д���ȥ��
	for (Cluster c : chameleon.chameleonCluster()) {
		set<string> tmp= Chameleon::clusterAnalyse(c);	//����ȥ��
		if (tmp.size() > 1)		//��Ŀ����޳�
			groups.insert(tmp);
	}	

	return groups;
}

void MiningTool::adjacentSnapshotMerge(map<set<string>, pair<int, int>>& tmpGroup,set<set<string>> &snapshotGroup,int startTime,int endTime,int interval,int argc,char*argv[],int &groupCounter,int& groupID)
{
	//��һ������map������ѯ�����³����ı�Ⱥ�����޳����ٳ����ı�Ⱥ
	for (map<set<string>, pair<int, int>>::iterator i = tmpGroup.begin(); i != tmpGroup.end();     /*TODO*/) {

		if (snapshotGroup.count((*i).first)) {	//����ʱ��Ƭ�ڱ�Ⱥ�Դ��ڣ�����ʱ�䣬ɾ��set��¼
			(*i).second.second = endTime;
			snapshotGroup.erase((*i).first);
			++i;
		}
		else {	//��ʱ��Ƭ�ı�Ⱥ�����ü�¼����:�ٱ�Ⱥά��ʱ��Σ�ֱ��ɾ��  �ڱ�Ⱥ����ʱ�䳤����¼����ɾ��
			if (startTime - (*i).second.first > interval) { 	//�ñ�Ⱥ���˽�ֹ,����ʱ�䳬��interval����¼�ñ�Ⱥ��Ϣ
				++groupCounter;
				++groupID;
				Tools::recordGroupInfo((*i).second.first, (*i).second.second, (*i).first,argc,argv,groupID);
				
			}
			i = tmpGroup.erase(i);
		}
	}
	//�����³��ֵı�Ⱥ�������tmpGroup��������ֹ��Ϣ
	for (set<string> ss:snapshotGroup) {
		tmpGroup[ss].first = startTime;
		tmpGroup[ss].second = endTime;
	}
}
