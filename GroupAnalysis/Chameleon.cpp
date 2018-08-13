#include "Chameleon.h"
#include "MiningTool.h"
#include <algorithm>
#include <fstream>
#include <string>
#include <iostream>

double Chameleon::ECofClusters(Cluster a, Cluster b)
{
	vector<ClusterPoint> ap= a.points;
	vector<ClusterPoint> bp = b.points;
	int aSize = ap.size();
	int bSize = bp.size();
	double res = 1/this->adjMat[0][1];		//TODO  这里应该是权重矩阵，而非距离矩阵
	for (int i = 0; i < aSize; ++i) {
		for (int j = 0; j < bSize; ++j) {
			if (i == j)
				continue;
			double tmpEC = 1 / adjMat[i][j];
			if (tmpEC < res)
				res = tmpEC;
		}
	}
	return res;
}

double Chameleon::RIofClusters(Cluster a, Cluster b)
{
	double ECofA = a.getEC();
	double ECofB = b.getEC();
	double	ECofAB = ECofClusters(a, b);
	return 2*ECofAB /(ECofA+ECofB);
}

double Chameleon::RCofClusters(Cluster a, Cluster b)
{
	int sizeA = a.points.size(),sizeB = b.points.size();
	double EcA = a.getEC(), Ecb = b.getEC();
	double ECofAB = ECofClusters(a, b);
	return ECofAB *(sizeA+sizeB)/ (sizeA*Ecb+sizeB*EcA);
}

double Chameleon::connectionOfClusters(Cluster a, Cluster b)
{
	return RIofClusters(a,b) * RCofClusters(a,b);
}

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
	knnGenerate(4);	//生成knn子图
	return vector<Cluster>();
}

void Chameleon::knnDfs(bool **knnMat, int PointNum,bool *visitedFlag,int idx,vector<ClusterPoint> &connectedPoints)
{
	for (int i = 0; i < PointNum; ++i) {	//轮询检查所有点，若该点未被访问，且为近邻点
		if (knnMat[idx][i] && !visitedFlag[i]) {
			connectedPoints.push_back(Points[i]);
			visitedFlag[i] = true;
			knnDfs(knnMat, PointNum, visitedFlag, i, connectedPoints);//对符合条件的紧邻点继续深搜
		}
	}
}

void printLine(double* a, int size) {
	for (int counter = 0; counter < size; ++counter) {
		printf("%10lg", a[counter]);
	}
	printf("\n");
}

void writeArray2File(string filename, double** data, int m, int n) {
	std::ofstream outFile;
	outFile.open(filename, std::ios::out);
	for (int i = 0; i < m; ++i) {
		for (int j = 0; j < n; ++j)
			outFile << data[i][j]<<",";
		outFile << std::endl;
	}
	outFile.close();
}
void writeArray2File(string filename, bool** data, int m, int n) {
	std::ofstream outFile;
	outFile.open(filename, std::ios::out);
	for (int i = 0; i < m; ++i) {
		for (int j = 0; j < n; ++j)
			outFile << data[i][j]<<",";
		outFile << std::endl;
	}
	outFile.close();
}

void Chameleon::knnGenerate(int k)
{
	int pointNum = Points.size();
	double  **tmpDisMat = new double*[pointNum] , *kDis = new double[pointNum];
	bool* visitedFlag = new bool[pointNum] , ** knnMat = new bool*[pointNum];

	this->adjMat = new double*[pointNum];
	for (int i = 0; i < pointNum; ++i) {
		visitedFlag[i] = false;
		knnMat[i] = new bool[pointNum];
		adjMat[i] = new double[pointNum];
		tmpDisMat[i] = new double[pointNum];
	}		

	for (int i = 0; i < pointNum; i++) {	//构建邻接矩阵
		for (int j = 0; j < pointNum; j++) {
			if (i > j) {
				adjMat[i][j] = MiningTool::miningDistance(Points[i], Points[j]);
				adjMat[j][i] = adjMat[i][j];
				tmpDisMat[i][j] = adjMat[i][j];
				tmpDisMat[j][i] = adjMat[i][j];
			}
			else if (i == j) {
				adjMat[i][i] = 0;
				tmpDisMat[i][j] = 0;
			}							
			else
				break;	//以下三角形式构建距离矩阵，此处break减少循环次数
		}
	}	
	//writeArray2File("originDis.csv", disMat, pointNum, pointNum); for debug
	//对每个点寻找到k位距离阈值
	for (int counter = 0; counter < pointNum; ++counter) {
		std::sort(tmpDisMat[counter], tmpDisMat[counter] + pointNum);
		kDis[counter] = tmpDisMat[counter][k];
	}
	
	for (int i = 0; i < pointNum; ++i) {
		delete[] tmpDisMat[i];
	}
	delete[] tmpDisMat;
	//writeArray2File("orderedDis.csv", tmpDisMat, pointNum, pointNum);			For debug
	//根据第k位距离置knn表征矩阵
	for (int i = 0; i < pointNum; ++i) {
		for (int j = 0; j < pointNum; ++j) {
			if (adjMat[i][j] >= kDis[i] || i==j)
				knnMat[i][j] = false;
			else
				knnMat[i][j] = true;
		}
	}
	//writeArray2File("knn.csv", knnMat, pointNum, pointNum);

	for (int i = 0; i < pointNum; ++i) {
		if (visitedFlag[i])
			continue;
		//还需检查是否存在近邻点
		vector<ClusterPoint> connectedP;
		connectedP.push_back(Points[i]);
		visitedFlag[i] = true;
		knnDfs(knnMat, pointNum, visitedFlag, i, connectedP);
		if (connectedP.size() == 1)
			continue;
		clusters.push_back(Cluster(connectedP));
	}
	system("pause");
}

set<string> Chameleon::clusterAnalyse(Cluster c)
{
	set<string> res;
	for (ClusterPoint cp : c.points) {
		res.insert(cp.getTargetID());
	}
	return set<string>();
}



Chameleon::Chameleon()
{
}


Chameleon::~Chameleon()
{
}
