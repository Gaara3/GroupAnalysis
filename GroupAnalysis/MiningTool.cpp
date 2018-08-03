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
	//ͬһĿ�겻ͬ�㼣����������룬����ʱֱ�ӹ���
	/*if (a.getTargetID() == b.getTargetID())
		return -1;*/		//��һ����࣬�ڴ�����ȥ��

	int tDiff = abs(a.getPosixtime() - b.getPosixtime());
	double distance = distanceBetweenPoints(a.getLongitude(), a.getLatitude(), b.getLongitude(), b.getLatitude());
	double angleDiff = fabs(a.getAngle() - b.getAngle());
	double speedDiff = fabs(a.getSpeed() - b.getSpeed());

	return 0;
}


double MiningTool::distanceBetweenPoints(double lastLongitude, double lastLatitude, double longitude, double latitude) {
	double res = 0;
	if (lastLatitude <= 90 && lastLongitude <= 180) {	//��һ�ι켣��ᣬ�����쳣ֵ�Ա�����һ�μ���
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
	
	std::sort(Points.begin(), Points.end(), posixSort);//��ʱ������

	Points.erase(std::unique(Points.begin(), Points.end(),samePoint), Points.end());//ȥ��

	int tmpStartTime = startTime, tmpEndTime = startTime + timeInterval;
	int pCounter = 0, pNum = Points.size();
	while (tmpStartTime <= endTime) {		//ÿ��ʱ����Ƭ��ѯ
		vector<int> candidateIdx;
		bool multiTarget = false;
		do{
			if (Points[pCounter].insideSnapshot(tmpStartTime, tmpEndTime)) {	//�õ㼣����ʱ��Ƭ��:1.�ж��Ƿ�Ϊ��Ŀ�� 2.push���ѡ��
				if (!multiTarget && !candidateIdx.empty() && !OriginPoint::sameTarget(Points[pCounter], Points[candidateIdx.back()]))//����candidate����Ԫ�رȶ�
					multiTarget = true;//��multiTarget�������������ٺ����ж�

				candidateIdx.push_back(pCounter);
			}else	//����ʱ��Ƭ�ڣ���������������һʱ��Ƭ
				break;			
		} while (++pCounter < pNum);
		
		if(multiTarget)//��Ŀ��������ٽ��о���
			snapshotAnalyze(Points,candidateIdx);

		tmpStartTime = tmpEndTime;//����ʱ����Ƭ
		tmpEndTime += timeInterval;
	}
}

void MiningTool::snapshotAnalyze(vector<OriginPoint> Point,vector<int>candidateIdx)
{
		return;
}
