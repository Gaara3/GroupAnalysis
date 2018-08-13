#pragma once
#include "ClusterPoint.h"
#include <vector>
using std::vector;
class Cluster
{
private:
	double **subGraph;
	int clusterSize;
public:
	vector<ClusterPoint> points;
	Cluster(vector<ClusterPoint> points);
	void initCluster(vector<ClusterPoint> points);	//���þ�����������clusterSize
	double getEC();
	Cluster();
	~Cluster();
};

