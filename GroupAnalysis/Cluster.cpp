#include "Cluster.h"
#include "MiningTool.h"


Cluster::Cluster(vector<ClusterPoint> points)
{
	this->points.assign(points.begin(), points.end());
}

void Cluster::initCluster(vector<ClusterPoint> points)
{
	int clusterSize = (int)points.size();
	this->subGraph = new double*[clusterSize];

	for(int i=0;i<clusterSize;++i)
		this->subGraph[i] = new double[clusterSize];

	for (int i = 0; i < clusterSize; ++i) {		
		for (int j = 0; j < clusterSize; ++j) {
			if (i < j)
				subGraph[i][j] = MiningTool::miningDistance(points[i], points[j]);
			else if (i == j)
				subGraph[i][i] = 0;
			else
				subGraph[i][j] = subGraph[j][i];
		}
	}

	this->clusterSize = clusterSize;
}

double Cluster::getEC()
{
	double tempEC = 0;
	for (int i = 0; i < clusterSize; ++i) {
		for (int j = 0; j < clusterSize; ++j) {
			if (i < j)
				tempEC += subGraph[i][j];
		}
	}
	return tempEC/clusterSize;
}

Cluster::Cluster()
{
}


Cluster::~Cluster()
{
}
