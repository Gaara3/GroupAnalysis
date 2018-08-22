#pragma once
#include "ClusterPoint.h"
#include <vector>
using std::vector;
class Cluster
{
private:
	int clusterSize;

public:
	double EC;
	//double **subGraph;
	vector<ClusterPoint> points;
	Cluster();
	~Cluster();
	Cluster(vector<ClusterPoint> points);
	Cluster(vector<ClusterPoint>, double**);
	Cluster(const Cluster &c);
	//void resetSubGraph();
	//void setSubGraph(double **adjMat);
	void setClusterSize(int size);
	void updateClusterSize();
	void initCluster(vector<ClusterPoint> points);	//…Ë÷√æ‡¿Î◊”’Û£¨…Ë÷√clusterSize
	void updateEC(double **adj);	
	//void updateClusterInfo(double **adjMat);
};

