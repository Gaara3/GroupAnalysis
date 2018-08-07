#include "Chameleon.h"
#include "MiningTool.h"
#include <algorithm>
#include <fstream>
#include <string>
#include <iostream>

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
	for (int i = 0; i < PointNum; ++i) {	//轮询检查所有点，若该点未被访问，且为近邻点
		if (!visitedFlag[i] && knnMat[idx][i]) {
			connectedPoints.push_back(Points[i]);
			visitedFlag[i] = true;
			knnDfs(knnMat, PointNum, visitedFlag, i, connectedPoints);//对符合条件的紧邻点继续深搜
		}
	}
}

void Chameleon::knnGenerate(int k)
{
	int pointNum = Points.size();
	double **disMat = new double*[pointNum];
	for (int counter = 0; counter < pointNum; counter++)
		disMat[counter] = new double[pointNum];	

	for (int i = 0; i < pointNum; i++) {	//构建邻接矩阵
		for (int j = 0; j < pointNum; j++) {
			if (i > j) {
				disMat[i][j] = MiningTool::miningDistance(Points[i], Points[j]);
				disMat[j][i] = disMat[i][j];
			}
			else if (i == j)
				disMat[i][i] = 0;
			else
				break;	//以下三角形式构建距离矩阵，此处break减少循环次数
		}
	}	

	//打印距离矩阵
	std::ofstream outFile;
	outFile.open("data.csv", std::ios::out);
	for (int i = 0; i < pointNum; ++i) {
		for (int j = 0; j < pointNum; ++j)
			outFile << disMat[i][j] << ",";
		outFile << std::endl;
	}
	outFile.close();
	bool* visitedFlag = new bool[pointNum];
	for (int i = 0; i < pointNum; ++i)
		visitedFlag[i] = false;//规范初始化
	bool** knnMat = new bool*[pointNum];
	for (int counter = 0; counter < pointNum; ++counter)
		knnMat[counter] = new bool[pointNum];

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
		//还需检查是否存在近邻点
		vector<ClusterPoint> connectedP;
		connectedP.push_back(Points[i]);
		visitedFlag[i] = true;
		knnDfs(knnMat, pointNum, visitedFlag, i, connectedP);
		clusters.push_back(Cluster(connectedP));
	}
	system("pause");
}



Chameleon::Chameleon()
{
}


Chameleon::~Chameleon()
{
}
