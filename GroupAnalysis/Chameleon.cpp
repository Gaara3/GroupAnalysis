#include "Chameleon.h"
#include "MiningTool.h"
#include <algorithm>


Chameleon::Chameleon(vector<OriginPoint> p, vector<int> i,int k)
{
	int pNum = i.size();
	for (int counter = 0; counter < pNum; ++counter) {
		Points.push_back(ClusterPoint(counter, p[counter]));
	}
	this->k = k;
}

vector<Cluster> Chameleon::chameleonCluster()
{
	knnGenerate(4);
	return vector<Cluster>();
}

void Chameleon::knnDfs(bool **knnMat, int PointNum,bool *visitedFlag,int idx,vector<ClusterPoint> &connectedPoints)
{
	visitedFlag[idx] = true;
	for (int i = 0; i < PointNum; ++i) {
		if (knnMat[idx][i] && knnMat[i][idx])
			connectedPoints.push_back(Points[i]);
	}
}

void Chameleon::knnGenerate(int k)
{
	int pointNum = Points.size();
	double **disMat = new double*[pointNum];
	for (int counter = 0; counter < pointNum; counter++)
		disMat[counter] = new double[pointNum];	//下三角矩阵储存距离信息

	for (int i = 0; i < pointNum; i++) {	//构建邻接矩阵
		for (int j = 0; j < pointNum; j++) {
			if (i < j)
				disMat[i][i] = MiningTool::miningDistance(Points[i], Points[j]);
			else if (i == j)
				disMat[i][i] = 0;
			else
				disMat[i][j] = disMat[j][i];
		}
	}	
	bool* visitedFlag = new bool[pointNum];
	bool** knnMat = new bool*[pointNum];
	double *kDis = new double[pointNum];
	//对每个点寻找到k位距离阈值
	for (int counter = 0; counter < pointNum; ++counter) {
		std::sort(disMat[counter], disMat[counter] + pointNum);
		kDis[counter] = disMat[counter][k];
	}
	for (int i = 0; i < pointNum; ++i) {
		for (int j = 0; j < pointNum; ++j) {
			if (disMat[i][j] >= kDis[i] || i==j)
				knnMat[i][j] = false;
			else
				knnMat[i][j] = true;
		}
	}
	for (int i = 0; i < pointNum; ++i) {
		if (visitedFlag[i])
			continue;
		vector<ClusterPoint> connectedP;
		connectedP.push_back(Points[i]);
		knnDfs(knnMat, pointNum, visitedFlag, i, connectedP);
	}
}



Chameleon::Chameleon()
{
}


Chameleon::~Chameleon()
{
}
