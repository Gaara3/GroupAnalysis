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
	for (int i = 0; i < PointNum; ++i) {	//��ѯ������е㣬���õ�δ�����ʣ���Ϊ���ڵ�
		if (!visitedFlag[i] && knnMat[idx][i]) {
			connectedPoints.push_back(Points[i]);
			visitedFlag[i] = true;
			knnDfs(knnMat, PointNum, visitedFlag, i, connectedPoints);//�Է��������Ľ��ڵ��������
		}
	}
}

void Chameleon::knnGenerate(int k)
{
	int pointNum = Points.size();
	double **disMat = new double*[pointNum];
	for (int counter = 0; counter < pointNum; counter++)
		disMat[counter] = new double[pointNum];	

	for (int i = 0; i < pointNum; i++) {	//�����ڽӾ���
		for (int j = 0; j < pointNum; j++) {
			if (i > j) {
				disMat[i][j] = MiningTool::miningDistance(Points[i], Points[j]);
				disMat[j][i] = disMat[i][j];
			}
			else if (i == j)
				disMat[i][i] = 0;
			else
				break;	//����������ʽ����������󣬴˴�break����ѭ������
		}
	}	

	//��ӡ�������
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
		visitedFlag[i] = false;//�淶��ʼ��
	bool** knnMat = new bool*[pointNum];
	for (int counter = 0; counter < pointNum; ++counter)
		knnMat[counter] = new bool[pointNum];

	double *kDis = new double[pointNum];
	//��ÿ����Ѱ�ҵ�kλ������ֵ
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
		//�������Ƿ���ڽ��ڵ�
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
