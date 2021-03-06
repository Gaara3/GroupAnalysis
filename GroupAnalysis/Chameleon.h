#pragma once
#include <vector>
#include <deque>
#include <set>
#include <utility>
#include "Cluster.h"
#include "OriginPoint.h"
#include "ClusterPoint.h"

using std::vector;
using std::set;
using std::deque;
using std::pair;
class Chameleon
{
private:
	int k;	//knn近邻子图
	double **adjMat;	//邻接矩阵
	vector<ClusterPoint> Points;
	deque<Cluster> clusters;	//需要随机存取，也有大量删除，故使用deque
	double ECofClusters(const Cluster &a,const Cluster &b);
	double RIofClusters(const Cluster &a,const Cluster &b);
	double RCofClusters(const Cluster &a,const Cluster &b);
	double connectionOfClusters(const Cluster &a, const Cluster &b);
	//void setClusterSubGraph(Cluster & cluster);
	pair<int, int> findClusters2Merge();
	void merge2Clusters(Cluster& c1, Cluster& c2);
	//void clusterPartition();
	void clusterAlgorithm();
	double maxWeightOf2Clusters(const Cluster&c1, const Cluster&c2);
	void printInfo();
public:
	double findMaxWeight();
	deque<Cluster> getClusters();
	Chameleon(deque<OriginPoint>, vector<int>,int k);
	deque<Cluster> chameleonCluster();
	void knnDfs(bool**knnMat, int PointNum,bool*visited,int idx,vector<ClusterPoint>&);
	void knnGenerate(int k);
	static set<string> clusterAnalyse(Cluster c);
	Chameleon();
	~Chameleon();
};

