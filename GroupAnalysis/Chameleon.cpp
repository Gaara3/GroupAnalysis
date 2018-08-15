#include "Chameleon.h"
#include "MiningTool.h"
#include "Tools.h"
#include <algorithm>
#include <fstream>
#include <string>
#include <iostream>


Chameleon::Chameleon(vector<OriginPoint> p, vector<int> i, int k)
{
	int pNum = i.size();
	//为簇内点赋值id
	for (int counter = 0; counter < pNum; ++counter) {
		Points.push_back(ClusterPoint(counter, p[counter]));
	}
	this->k = k;

	//初始化邻接矩阵
	adjMat = new double*[pNum];
	for (int i = 0; i < pNum; ++i)
		adjMat[i] = new double[pNum];

	//构建邻接矩阵
	for (int i = 0; i < pNum; i++) {
		for (int j = 0; j <=i; j++) {
			if (i > j) {
				adjMat[i][j] = 1 / MiningTool::miningDistance(Points[i], Points[j]);	//构造的其实是权重矩阵
				adjMat[j][i] = adjMat[i][j];
			}
			else if (i == j) {
				adjMat[i][i] = 0;
			}
		}
	}
	Tools::writeArray2File("originDis.csv", adjMat, pNum, pNum);
}


double Chameleon::ECofClusters(Cluster a, Cluster b)
{
	vector<ClusterPoint> ap= a.points;
	vector<ClusterPoint> bp = b.points;
	int aSize = ap.size();
	int bSize = bp.size();
	double res =adjMat[0][1];	
	for (int i = 0; i < aSize; ++i) {
		for (int j = 0; j < bSize; ++j) {
			if (i == j)
				continue;
			double tmpEC = adjMat[a.points[i].id][b.points[j].id];	//TODO  这里有问题，应该是原始id，不是簇内序数
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

void Chameleon::setClusterSubGraph(Cluster& cluster)
{
	int pNum = cluster.points.size();
	cluster.subGraph = new double*[pNum];
	for (int i = 0; i < pNum; ++i)
		cluster.subGraph[i] = new double[pNum];
	
	for (int i = 0; i < pNum; ++i) {
		for (int j = 0; j < pNum; ++j)
			cluster.subGraph[i][j] = this->adjMat[cluster.points[i].id][cluster.points[j].id];
	}
}

pair<int, int> Chameleon::findClusters2Merge()
{
	int cSize = (int)clusters.size();
	double maxCon = connectionOfClusters(clusters[0], clusters[1]);
	pair<int, int>res(0,1);
	for (int i = 0; i < cSize; ++i) {
		for (int j = 0; j < i; ++j) {	//减少重复计算次数
			if (i == j)
				continue;
			double tmp = connectionOfClusters(clusters[i],clusters[j]);
			if (tmp > maxCon) {
				maxCon = tmp;
				res.first = i;
				res.second = j;
			}
		}
	}
	return res;
}

void Chameleon::merge2Clusters(Cluster &c1, Cluster& c2)
{
	//重置cluster1的子图矩阵
	c1.resetSubGraph();
	//将cluster2内的点逐个加入cluster1内
	while (!c2.points.empty()) {
		c1.points.push_back(c2.points.back());
		c2.points.pop_back();
	}
	c1.setClusterSize((int)c1.points.size());
	//更新簇子图矩阵
	c1.setSubGraph(adjMat);
}

void Chameleon::clusterAlgorithm()
{		
	while (true) {//while(最近的簇，中位距离<阈值)

		//找到应合并的两簇，找出下标
		pair<int,int> targets = findClusters2Merge();

		//合并两簇，更新新簇内的子图
		merge2Clusters(clusters[targets.first],clusters[targets.second]);
		clusters.erase(clusters.begin() + targets.second );	//簇向量移除该簇

		//更新全局簇连接图
	}
}

deque<Cluster> Chameleon::getClusters()
{
	return this->clusters;
}

vector<Cluster> Chameleon::chameleonCluster()
{
	knnGenerate(4);	//生成knn子图
	//聚类出最终类，存入clusters中
	//clusterPartition();
	//clusterAlgorithm();
	return vector<Cluster>();
}


void Chameleon::knnDfs(bool **knnMat, int PointNum, bool *visitedFlag, int idx, vector<ClusterPoint> &connectedPoints)
{
	for (int i = 0; i < PointNum; ++i) {	//轮询检查所有点，若该点未被访问，且为近邻点
		if (knnMat[idx][i] && !visitedFlag[i]) {
			connectedPoints.push_back(Points[i]);
			visitedFlag[i] = true;
			knnDfs(knnMat, PointNum, visitedFlag, i, connectedPoints);//对符合条件的紧邻点继续深搜
		}
	}
}

void Chameleon::knnGenerate(int k)
{
	int pointNum = Points.size();
	double  **tmpDisMat = new double*[pointNum] , *kDis = new double[pointNum];
	bool* visitedFlag = new bool[pointNum] , ** knnMat = new bool*[pointNum];

	for (int i = 0; i < pointNum; ++i) {
		visitedFlag[i] = false;
		knnMat[i] = new bool[pointNum];
		tmpDisMat[i] = new double[pointNum];
	}		

	for (int i = 0; i < pointNum; i++) {
		for (int j = 0; j < pointNum; j++) {
			if (i > j) {
				tmpDisMat[i][j] = adjMat[i][j];
				tmpDisMat[j][i] =adjMat[i][j];
			}
			else if (i == j) 
				tmpDisMat[i][j] = 0;
			else
				break;	//以下三角形式构建距离矩阵，此处break减少循环次数
		}
	}
	Tools::writeArray2File("tmpDis.csv", tmpDisMat, pointNum, pointNum);
	//对每个点寻找到k位距离阈值
	for (int counter = 0; counter < pointNum; ++counter) {
		std::sort(tmpDisMat[counter], tmpDisMat[counter] + pointNum);
		kDis[counter] = tmpDisMat[counter][pointNum-k];
	}
	Tools::writeArray2File("orderedDis.csv", tmpDisMat, pointNum, pointNum);

	for (int i = 0; i < pointNum; ++i) {
		delete[] tmpDisMat[i];
	}
	delete[] tmpDisMat;
	
	//根据第k位距离置knn表征矩阵
	for (int i = 0; i < pointNum; ++i) {
		for (int j = 0; j < pointNum; ++j) {
			if (adjMat[i][j] <= kDis[i] || i==j)
				knnMat[i][j] = false;
			else
				knnMat[i][j] = true;
		}
	}
	Tools::writeArray2File("knn.csv", knnMat, pointNum, pointNum);

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
		clusters.push_back(Cluster(connectedP,adjMat));
	}
}

set<string> Chameleon::clusterAnalyse(Cluster c)
{
	set<string> res;
	for (ClusterPoint cp : c.points) {
		res.insert(cp.getTargetID());
	}
	return res;
}



Chameleon::Chameleon()
{
}


Chameleon::~Chameleon()
{
}
