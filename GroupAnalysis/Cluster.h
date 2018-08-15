#pragma once
#include "ClusterPoint.h"
#include <vector>
using std::vector;
class Cluster
{
private:
	int clusterSize;
public:
	double **subGraph;
	vector<ClusterPoint> points;
	Cluster(vector<ClusterPoint> points);
	Cluster(vector<ClusterPoint>, double**);
	void resetSubGraph();
	void setSubGraph(double **adjMat);
	void setClusterSize(int size);
	void initCluster(vector<ClusterPoint> points);	//…Ë÷√æ‡¿Î◊”’Û£¨…Ë÷√clusterSize
	double getEC();
	Cluster();
	~Cluster();
};

