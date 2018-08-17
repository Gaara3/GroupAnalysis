#include "Chameleon.h"
#include "MiningTool.h"
#include "Tools.h"
#include <algorithm>
#include <fstream>
#include <string>
#include <iostream>
#include <limits>
#include <cstdio>

//extern double totalTheshold;

Chameleon::Chameleon(vector<OriginPoint> p, vector<int> i, int k)
{
	int pNum = i.size();
	//为簇内点赋值id
	for (int counter = 0; counter < pNum; ++counter) {
		Points.push_back(ClusterPoint(counter, p[i[counter]]));
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
				if(MiningTool::miningDistance(Points[i], Points[j]))	
					adjMat[i][j] = 1 / MiningTool::miningDistance(Points[i], Points[j]);	//构造的其实是权重矩阵
				else//挖掘距离为0，则置权重为最大值
					adjMat[i][j] = std::numeric_limits< double >::max();
				adjMat[j][i] = adjMat[i][j];
			}
			else if (i == j) {
				adjMat[i][i] = 0;
			}
		}
	}
	Tools::writeArray2File("originDis.csv", adjMat, pNum, pNum);
}


double Chameleon::ECofClusters(const Cluster &a, const Cluster &b)
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

double Chameleon::RIofClusters(const Cluster &a,const Cluster &b)
{
	double ECofA = a.EC;
	double ECofB = b.EC;
	double	ECofAB = ECofClusters(a, b);
	double res=2*ECofAB /(ECofA+ECofB);
	return res;
}

double Chameleon::RCofClusters(const Cluster &a,const Cluster &b)
{
	int sizeA = a.points.size(),sizeB = b.points.size();
	double EcA = a.EC, Ecb = b.EC;
	double ECofAB = ECofClusters(a, b);
	double res= ECofAB *( sizeA + sizeB )/ ( sizeA * Ecb + sizeB * EcA );
	return res;
}

double Chameleon::connectionOfClusters(const Cluster &a, const Cluster &b)
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

pair<int, int> Chameleon::findClusters2Merge(double &maxCon)
{
	int cSize = (int)clusters.size();
	maxCon = connectionOfClusters(clusters[0], clusters[1]);
	pair<int, int>res(0,1);
	for (int i = 0; i < cSize; ++i) {
		for (int j = 0; j < i; ++j) {	//减少重复计算次数
			if (i == j)
				continue;
			double tmp = connectionOfClusters(clusters[i],clusters[j]);
			if (tmp > maxCon) {	//取到连接权重最大的两簇
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
	//将cluster2内的点逐个加入cluster1内
	while (!c2.points.empty()) {
		c1.points.push_back(c2.points.back());
		c2.points.pop_back();
	}
	c2.points.clear();
	{
		vector<ClusterPoint> tmp = c2.points;
		c2.points.swap(tmp);
	}
	//更新c1的子图矩阵、EC值
	c1.updateClusterInfo(adjMat);
}

void Chameleon::clusterAlgorithm()
{		
	double maxCon = 0;
	double totalThreshold = 1.0/60.0;
	while (clusters.size()>1) {//while(最近的簇，中位距离<阈值)

		//找到应合并的两簇，找出下标
		pair<int,int> targets = findClusters2Merge(maxCon);
		
		//最近簇的邻接权重太小，则结束聚类
		
		if (maxCon < totalThreshold)		//TimeThreshold(min) * DistThreshold(km)
			break;
		//合并两簇，更新新簇内的子图
		merge2Clusters(clusters[targets.first],clusters[targets.second]);
		clusters.erase(clusters.begin() + targets.second );	//簇队列移除该簇

	}
}

deque<Cluster> Chameleon::getClusters()
{
	return this->clusters;
}

deque<Cluster> Chameleon::chameleonCluster()
{
	knnGenerate(3);	//生成knn子图
	//聚类出最终类，存入clusters中
	//clusterPartition();
	clusterAlgorithm();
	remove("knn.csv");
	remove("orderedDis.csv");
	remove("originDis.csv");
	remove("tmpDis.csv");
	remove("subGraph.csv");
	return this->clusters;
}

/*对Idx点进行深度优先搜索*/
void Chameleon::knnDfs(bool **knnMat, int PointNum, bool *visitedFlag, int idx, vector<ClusterPoint> &connectedPoints)
{
	for (int i = 0; i < PointNum; ++i) {	//轮询检查所有点，若该点未被访问，且为近邻点
		if (!visitedFlag[i] && knnMat[idx][i] && knnMat[i][idx]) {
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
			if (adjMat[i][j] < kDis[i] || i==j)
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
		this->clusters.push_back(Cluster(connectedP,adjMat));
	}
	//Tools::writeArray2File("afterSubGraph.csv", clusters.back().subGraph, clusters.back().points.size(), clusters.back().points.size());
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
	for (int i = 0; i < Points.size(); ++i)
		delete[] adjMat[i];

	delete[] adjMat;
	Points.~Points();
	clusters.~clusters();
}
