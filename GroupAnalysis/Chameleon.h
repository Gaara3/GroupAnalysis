#pragma once
#include <vector>
#include "Cluster.h"
#include "OriginPoint.h"
#include "ClusterPoint.h"

using std::vector;
class Chameleon
{
private:
	int k;
	double **adjMat;	//ÁÚ½Ó¾ØÕó
	vector<ClusterPoint> Points;
	vector<Cluster> clusters;
public:
	Chameleon(vector<OriginPoint>, vector<int>,int k);
	vector<Cluster> chameleonCluster();
	void knnDfs(bool**knnMat, int PointNum,bool*visited,int idx,vector<ClusterPoint>&);
	void knnGenerate(int k);
	Chameleon();
	~Chameleon();
};

