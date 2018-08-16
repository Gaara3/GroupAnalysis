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

	double tDiff = abs(a.getPosixtime() - b.getPosixtime())/60;	//�Է���Ϊ��λ���ݶ������ȺtDiffӦ��20����
	if (tDiff == 0)
		tDiff = 1;
	double distance = distanceBetweenPoints(a.getLongitude(), a.getLatitude(), b.getLongitude(), b.getLatitude())*1000;//��MΪ��λ����,�ݶ�����disΪ1000����log��Ϊ10��
	if (distance == 0)
		distance = 2;
	double angleDiff = fabs(a.getAngle() - b.getAngle());
	double speedDiff = fabs(a.getSpeed() - b.getSpeed());
	double res = (0.6*log(distance) + 0.2*(angleDiff + speedDiff)) * tDiff;
	printf("%10lg\t", res);
	return res;
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
	vector<set<string> > groups;
	//TODO  �Ż��������ͬĿ��֮��������������ж��Ƿ�Ҫ����
	int PointNum = (int)candidateIdx.size();
	//int** disMat = new int*[PointNum];
	//for (int counter = 0; counter < PointNum; ++counter) 
	//	disMat[counter] = new int[PointNum];

	Chameleon chameleon(Point, candidateIdx,4);
	//knn����ͼ����
	chameleon.chameleonCluster();
	//���ɴؽ���Ŀ���������Ŀ���ɾȥ����Ŀ��ؽ��д���ȥ��
	for (Cluster c : chameleon.getClusters()) {
		set<string> tmp= Chameleon::clusterAnalyse(c);
		if (tmp.size() > 1)
			groups.push_back(tmp);
	}	
	//��ز��(��Ϊ��ʱ��knnΪ�������ڽ�Ȩ�صĽ�������������ؼ���EC��SEC)
	//��ξ���	
}
