#pragma once
#include <vector>
#include <set>
#include "Cluster.h"
#include "OriginPoint.h"
#include "ClusterPoint.h"

using std::vector;
using std::set;
class Chameleon
{
private:
	int k;
	double **adjMat;	//ÁÚ½Ó¾ØÕó
	vector<ClusterPoint> Points;
	vector<Cluster> clusters;
	double ECofClusters(Cluster a, Cluster b);
	double RIofClusters(Cluster a, Cluster b);
	double RCofClusters(Cluster a, Cluster b);
	double connectionOfClusters(Cluster a, Cluster b);
public:
	Chameleon(vector<OriginPoint>, vector<int>,int k);
	vector<Cluster> chameleonCluster();
	void knnDfs(bool**knnMat, int PointNum,bool*visited,int idx,vector<ClusterPoint>&);
	void knnGenerate(int k);
	set<string> clusterAnalyse(Cluster c);
	Chameleon();
	~Chameleon();
};

